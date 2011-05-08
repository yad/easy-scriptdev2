/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* ScriptData
SDName: Worldmap_eastern_kingdoms
SD%Complete:
SDComment:
SDCategory: Eastern Kingdoms
EndScriptData */

#include "precompiled.h"
#include "worldmap_eastern_kingdoms.h"

/*  Comments
 * This implements the basics of tower capturing in Eastern Plaguelands
 * When a player faction captures a capture point then it triggers some event (helpers + buff)
 * The tower states are processed by the capture point events
 */

worldmap_eastern_kingdoms::worldmap_eastern_kingdoms(Map* pMap) : ScriptedInstance(pMap),
    m_uiPlaguewoodController(0),
    m_uiEastwallController(0),
    m_uiNorthpassController(0),
    m_uiCrownguardController(0),
    m_uiPlaguewoodWorldState(WORLD_STATE_PLAGUEWOOD_NEUTRAL),
    m_uiNorthpassWorldState(WORLD_STATE_NORTHPASS_NEUTRAL),
    m_uiEastwallWorldState(WORLD_STATE_EASTWALL_NEUTRAL),
    m_uiCrownguardWorldState(WORLD_STATE_CROWNGUARD_NEUTRAL),
    m_uiPlaguewoodState(NEUTRAL),
    m_uiNorthpassState(NEUTRAL),
    m_uiEastwallState(NEUTRAL),
    m_uiCrownguardState(NEUTRAL),
    m_uiFlightMasterGUID(0),
    m_uiLordaeronShrineGUID(0),
    m_uiTowersAlly(0),
    m_uiTowersHorde(0) { }

void worldmap_eastern_kingdoms::OnObjectCreate(GameObject* pGo)
{
    switch(pGo->GetEntry())
    {
        case GO_LORDAERON_SHRINE:
            m_uiLordaeronShrineGUID = pGo->GetGUID();
            break;
    }
}

void worldmap_eastern_kingdoms::OnPlayerEnterZone(Player* pPlayer, uint32 uiZoneId)
{
    if (uiZoneId == ZONE_ID_EASTERN_PLAGUELANDS)
    {
        // cast buff the the player which enters the zone
        switch(pPlayer->GetTeam())
        {
            case ALLIANCE:
                for (uint8 i = 0; i < 4; i++)
                {
                    if (m_uiTowersAlly == m_aPlaguelandsTowerBuffs[i].uiTowers)
                        pPlayer->CastSpell(pPlayer, m_aPlaguelandsTowerBuffs[i].uiSpellIdAlly, false);
                }
                break;
            case HORDE:
                for (uint8 i = 0; i < 4; i++)
                {
                    if (m_uiTowersHorde == m_aPlaguelandsTowerBuffs[i].uiTowers)
                        pPlayer->CastSpell(pPlayer, m_aPlaguelandsTowerBuffs[i].uiSpellIdHorde, false);
                }
                break;
        }

        // add to the player set
        m_suiPlaguelandsPlayers.insert(pPlayer->GetGUID());

        // send actual world states
        SendPlayerWorldState(pPlayer);
    }
    else
    {
        // remove the buff from the player
        for (uint8 i = 0; i < 4; i++)
        {
            if (pPlayer->HasAura(pPlayer->GetTeam() == ALLIANCE ? m_aPlaguelandsTowerBuffs[i].uiSpellIdAlly : m_aPlaguelandsTowerBuffs[i].uiSpellIdHorde))
                pPlayer->RemoveAurasDueToSpell(pPlayer->GetTeam() == ALLIANCE ? m_aPlaguelandsTowerBuffs[i].uiSpellIdAlly : m_aPlaguelandsTowerBuffs[i].uiSpellIdHorde);
        }

        // remove from the player set
        if (m_suiPlaguelandsPlayers.find(pPlayer->GetGUID()) != m_suiPlaguelandsPlayers.end())
            m_suiPlaguelandsPlayers.erase(pPlayer->GetGUID());
    }
}

void worldmap_eastern_kingdoms::SetData(uint32 uiType, uint32 uiData)
{
    switch(uiType)
    {
        case TYPE_CROWNGUARD_CONTROLLER:
            m_uiCrownguardController = uiData;
            break;
        case TYPE_EASTWALL_CONTROLLER:
            m_uiEastwallController = uiData;
            break;
        case TYPE_NORTHPASS_CONTROLLER:
            m_uiNorthpassController = uiData;
            break;
        case TYPE_PLAGUEWOOD_CONTROLLER:
            if (uiData == ALLIANCE)
            {
                if (GetData(TYPE_PLAGUEWOOD_STATE) == NEUTRAL)
                {
                    // in neutral case the team id is the opposite team
                    // the team who captured the tower and set it to neutral
                    --m_uiTowersHorde;
                    m_uiPlaguewoodWorldState = WORLD_STATE_PLAGUEWOOD_NEUTRAL;
                    // unsummon flightmaster
                    DoUnsummonFlightMaster();
                }
                else if (GetData(TYPE_PLAGUEWOOD_STATE) == PROGRESS)
                {
                    m_uiPlaguewoodWorldState = WORLD_STATE_PLAGUEWOOD_PROG_ALY;
                    // increase tower count only if the controller is changed
                    if (uiData != GetData(TYPE_PLAGUEWOOD_CONTROLLER))
                    {
                        DoSummonFlightMasterIfCan();
                        ++m_uiTowersAlly;
                    }
                }
                // contested states are also sent by the opposite team
                // alliance contests horde tower
                else if (GetData(TYPE_PLAGUEWOOD_STATE) == CONTESTED)
                    m_uiPlaguewoodWorldState = WORLD_STATE_NORTHPASS_CONT_HORDE;
                else if (GetData(TYPE_PLAGUEWOOD_STATE) == WIN)
                    m_uiPlaguewoodWorldState = WORLD_STATE_NORTHPASS_ALY;
            }
            else if (uiData == HORDE)
            {
                if (GetData(TYPE_PLAGUEWOOD_STATE) == NEUTRAL)
                {
                    // in neutral case the team id is the opposite team
                    // the team who captured the tower and set it to neutral
                    --m_uiTowersAlly;
                    m_uiPlaguewoodWorldState = WORLD_STATE_PLAGUEWOOD_NEUTRAL;
                    // unsummon flightmaster
                    DoUnsummonFlightMaster();
                }
                if (GetData(TYPE_PLAGUEWOOD_STATE) == PROGRESS)
                {
                    m_uiPlaguewoodWorldState = WORLD_STATE_PLAGUEWOOD_PROG_HORDE;
                    // increase tower count only if the controller is changed
                    if (uiData != GetData(TYPE_PLAGUEWOOD_CONTROLLER))
                    {
                        DoSummonFlightMasterIfCan();
                        ++m_uiTowersHorde;
                    }
                }
                // contested states are also sent by the opposite team
                // horde contests alliance tower
                else if (GetData(TYPE_PLAGUEWOOD_STATE) == CONTESTED)
                    m_uiPlaguewoodWorldState = WORLD_STATE_NORTHPASS_CONT_ALY;
                else if (GetData(TYPE_PLAGUEWOOD_STATE) == WIN)
                    m_uiPlaguewoodWorldState = WORLD_STATE_NORTHPASS_HORDE;
            }
            // set controller only for progress and neutral
            if (GetData(TYPE_PLAGUEWOOD_STATE) == PROGRESS)
                m_uiPlaguewoodController = uiData;
            else if (GetData(TYPE_PLAGUEWOOD_STATE) == NEUTRAL)
                m_uiPlaguewoodController = 0;
            break;
        case TYPE_CROWNGUARD_STATE:
            m_uiCrownguardState = uiData;
            break;
        case TYPE_EASTWALL_STATE:
            m_uiEastwallState = uiData;
            break;
        case TYPE_NORTHPASS_STATE:
            m_uiNorthpassState = uiData;
            break;
        case TYPE_PLAGUEWOOD_STATE:
            m_uiPlaguewoodState = uiData;
            break;
    }

    // update buffs
    for (uint8 i = 0; i < 4; i++)
    {
        // buff alliance
        if (m_uiTowersAlly == m_aPlaguelandsTowerBuffs[i].uiTowers)
            DoProcessTeamBuff(ALLIANCE, m_aPlaguelandsTowerBuffs[i].uiSpellIdAlly);
        // buff horde
        if (m_uiTowersHorde == m_aPlaguelandsTowerBuffs[i].uiTowers)
            DoProcessTeamBuff(HORDE, m_aPlaguelandsTowerBuffs[i].uiSpellIdHorde);
    }

    // debuff players if towers == 0; spell to remove will be always the first
    if (m_uiTowersHorde == 0)
        DoProcessTeamBuff(HORDE, m_aPlaguelandsTowerBuffs[0].uiSpellIdHorde, true);
    if (m_uiTowersAlly == 0)
        DoProcessTeamBuff(ALLIANCE, m_aPlaguelandsTowerBuffs[0].uiSpellIdAlly, true);

    // update states counters
    // the map tower states are updated in the ProcessCaptureEvent function
    UpdateZoneWorldState(WORLD_STATE_TOWER_COUNT_ALY, m_uiTowersAlly);
    UpdateZoneWorldState(WORLD_STATE_TOWER_COUNT_HORDE, m_uiTowersHorde);
}

uint32 worldmap_eastern_kingdoms::GetData(uint32 uiType)
{
    switch(uiType)
    {
        case TYPE_CROWNGUARD_CONTROLLER:
            return m_uiCrownguardController;
        case TYPE_EASTWALL_CONTROLLER:
            return m_uiEastwallController;
        case TYPE_NORTHPASS_CONTROLLER:
            return m_uiNorthpassController;
        case TYPE_PLAGUEWOOD_CONTROLLER:
            return m_uiPlaguewoodController;
        case TYPE_CROWNGUARD_STATE:
            return m_uiCrownguardState;
        case TYPE_EASTWALL_STATE:
            return m_uiEastwallState;
        case TYPE_NORTHPASS_STATE:
            return m_uiNorthpassState;
        case TYPE_PLAGUEWOOD_STATE:
            return m_uiPlaguewoodState;
    }
    return 0;
}

void worldmap_eastern_kingdoms::DoSummonFlightMasterIfCan()
{
    Player* pPlayer = GetPlayerInMap();
    if (!pPlayer)
        return;

    // return if already summoned
    if (m_uiFlightMasterGUID)
        return;

    // summon the flightmaster
    if (Creature* pFlightMaster = pPlayer->SummonCreature(NPC_SPECTRAL_FLIGHTMASTER, m_aPlaguelandFlightmasterSpawnLocs[0].m_fX, m_aPlaguelandFlightmasterSpawnLocs[0].m_fY, m_aPlaguelandFlightmasterSpawnLocs[0].m_fZ, m_aPlaguelandFlightmasterSpawnLocs[0].m_fO, TEMPSUMMON_MANUAL_DESPAWN, 0))
        m_uiFlightMasterGUID = pFlightMaster->GetGUID();
}

void worldmap_eastern_kingdoms::DoUnsummonFlightMaster()
{
    if (!m_uiFlightMasterGUID)
        return;

    if (Creature* pFlightMaster = instance->GetCreature(m_uiFlightMasterGUID))
        pFlightMaster->ForcedDespawn();
}

void worldmap_eastern_kingdoms::DoSummonSoldiersIfCan()
{
}

void worldmap_eastern_kingdoms::DoUnsummonSoldiers()
{
}

void worldmap_eastern_kingdoms::UpdateZoneWorldState(uint32 uiStateId, uint32 uiStateValue)
{
    DoUpdateWorldState(uiStateId, uiStateValue);
}

void worldmap_eastern_kingdoms::SendPlayerWorldState(Player* pPlayer)
{
    // counters
    pPlayer->SendUpdateWorldState(WORLD_STATE_TOWER_COUNT_ALY, m_uiTowersAlly);
    pPlayer->SendUpdateWorldState(WORLD_STATE_TOWER_COUNT_HORDE, m_uiTowersHorde);
    // towers
    pPlayer->SendUpdateWorldState(m_uiCrownguardWorldState, 1);
    pPlayer->SendUpdateWorldState(m_uiEastwallWorldState, 1);
    pPlayer->SendUpdateWorldState(m_uiNorthpassWorldState, 1);
    pPlayer->SendUpdateWorldState(m_uiPlaguewoodWorldState, 1);
}

void worldmap_eastern_kingdoms::ProcessCaptureEvent(uint32 uiCaptureType, uint32 uiTeam, uint32 uiTower)
{
    switch(uiTower)
    {
        case TOWER_ID_CROWNGUARD:
            break;
        case TOWER_ID_EASTWALL:
            break;
        case TOWER_ID_NORTHPASS:
            break;
        case TOWER_ID_PLAGUEWOOD:
            // remove old state
            UpdateZoneWorldState(m_uiPlaguewoodWorldState, 0);
            // update data
            SetData(TYPE_PLAGUEWOOD_STATE, uiCaptureType);
            SetData(TYPE_PLAGUEWOOD_CONTROLLER, uiTeam);
            // send new state
            UpdateZoneWorldState(m_uiPlaguewoodWorldState, 1);
            break;
    }
}

void worldmap_eastern_kingdoms::DoUpdateZoneWorldState(uint32 uiStateId, uint32 uiStateData)
{
    for(std::set<uint64>::const_iterator itr = m_suiPlaguelandsPlayers.begin(); itr != m_suiPlaguelandsPlayers.end(); ++itr)
    {
        if (Player* pPlayer = instance->GetPlayer(*itr))
            pPlayer->SendUpdateWorldState(uiStateId, uiStateData);
    }
}

void worldmap_eastern_kingdoms::DoProcessTeamBuff(Team uiTeamId, uint32 uiSpellId, bool bRemove /*= false*/)
{
    for(std::set<uint64>::const_iterator itr = m_suiPlaguelandsPlayers.begin(); itr != m_suiPlaguelandsPlayers.end(); ++itr)
    {
        if (Player* pPlayer = instance->GetPlayer(*itr))
        {
            if (pPlayer->GetTeam() == uiTeamId)
            {
                if (!bRemove)
                    pPlayer->CastSpell(pPlayer, uiSpellId, true);
                else
                {
                    if (pPlayer->HasAura(uiSpellId))
                        pPlayer->RemoveAurasDueToSpell(uiSpellId);
                }
            }
        }
    }
}

InstanceData* GetInstanceData_worldmap_eastern_kingdoms(Map* pMap)
{
    return new worldmap_eastern_kingdoms(pMap);
}

bool ProcessEventId_event_crownguard_tower(uint32 uiEventId, Object* pSource, Object* pTarget, bool bIsStart)
{
    if (bIsStart && pSource->GetTypeId() == TYPEID_PLAYER)
    {
        if (worldmap_eastern_kingdoms* pOutdoorPvp = (worldmap_eastern_kingdoms*)((Player*)pSource)->GetInstanceData())
        {
            switch(uiEventId)
            {
                case EVENT_CROWNGUARD_WIN_ALLIANCE:
                case EVENT_CROWNGUARD_WIN_HORDE:
                case EVENT_CROWNGUARD_CONTEST_ALLIANCE:
                case EVENT_CROWNGUARD_CONTEST_HORDE:
                case EVENT_CROWNGUARD_PROGRESS_ALLIANCE:
                case EVENT_CROWNGUARD_PROGRESS_HORDE:
                case EVENT_CROWNGUARD_NEUTRAL_ALLIANCE:
                case EVENT_CROWNGUARD_NEUTRAL_HORDE:
                    pOutdoorPvp->ProcessCaptureEvent(NEUTRAL, ((Player*)pSource)->GetTeam(), TOWER_ID_CROWNGUARD);
                    break;
            }
            return true;
        }
    }
    return false;
}

bool ProcessEventId_event_eastwall_tower(uint32 uiEventId, Object* pSource, Object* pTarget, bool bIsStart)
{
    if (bIsStart && pSource->GetTypeId() == TYPEID_PLAYER)
    {
        if (worldmap_eastern_kingdoms* pOutdoorPvp = (worldmap_eastern_kingdoms*)((Player*)pSource)->GetInstanceData())
        {
            switch(uiEventId)
            {
                case EVENT_EASTWALL_WIN_ALLIANCE:
                case EVENT_EASTWALL_WIN_HORDE:
                case EVENT_EASTWALL_CONTEST_ALLIANCE:
                case EVENT_EASTWALL_CONTEST_HORDE:
                case EVENT_EASTWALL_PROGRESS_ALLIANCE:
                case EVENT_EASTWALL_PROGRESS_HORDE:
                case EVENT_EASTWALL_NEUTRAL_ALLIANCE:
                case EVENT_EASTWALL_NEUTRAL_HORDE:
                    pOutdoorPvp->ProcessCaptureEvent(NEUTRAL, ((Player*)pSource)->GetTeam(), TOWER_ID_EASTWALL);
                    break;
            }
            return true;
        }
    }
    return false;
}

bool ProcessEventId_event_northpass_tower(uint32 uiEventId, Object* pSource, Object* pTarget, bool bIsStart)
{
    if (bIsStart && pSource->GetTypeId() == TYPEID_PLAYER)
    {
        if (worldmap_eastern_kingdoms* pOutdoorPvp = (worldmap_eastern_kingdoms*)((Player*)pSource)->GetInstanceData())
        {
            switch(uiEventId)
            {
                case EVENT_NORTHPASS_WIN_ALLIANCE:
                case EVENT_NORTHPASS_WIN_HORDE:
                case EVENT_NORTHPASS_CONTEST_ALLIANCE:
                case EVENT_NORTHPASS_CONTEST_HORDE:
                case EVENT_NORTHPASS_PROGRESS_ALLIANCE:
                case EVENT_NORTHPASS_PROGRESS_HORDE:
                case EVENT_NORTHPASS_NEUTRAL_ALLIANCE:
                case EVENT_NORTHPASS_NEUTRAL_HORDE:
                    pOutdoorPvp->ProcessCaptureEvent(NEUTRAL, ((Player*)pSource)->GetTeam(), TOWER_ID_NORTHPASS);
                    break;
            }
            return true;
        }
    }
    return false;
}

bool ProcessEventId_event_plaguewood_tower(uint32 uiEventId, Object* pSource, Object* pTarget, bool bIsStart)
{
    if (bIsStart && pSource->GetTypeId() == TYPEID_PLAYER)
    {
        if (worldmap_eastern_kingdoms* pOutdoorPvp = (worldmap_eastern_kingdoms*)((Player*)pSource)->GetInstanceData())
        {
            switch(uiEventId)
            {
                case EVENT_PLAGUEWOOD_WIN_ALLIANCE:
                case EVENT_PLAGUEWOOD_WIN_HORDE:
                    // handle event
                    pOutdoorPvp->ProcessCaptureEvent(WIN, ((Player*)pSource)->GetTeam(), TOWER_ID_PLAGUEWOOD);
                    // complete quest
                    ((Player*)pSource)->KilledMonsterCredit(NPC_PLAGUEWOOD_TOWER_QUEST_DOODAD);
                    break;
                case EVENT_PLAGUEWOOD_CONTEST_ALLIANCE:
                case EVENT_PLAGUEWOOD_CONTEST_HORDE:
                    // handle event - in this case player's team will be the opposite team
                    pOutdoorPvp->ProcessCaptureEvent(CONTESTED, ((Player*)pSource)->GetTeam(), TOWER_ID_PLAGUEWOOD);
                    break;
                case EVENT_PLAGUEWOOD_PROGRESS_ALLIANCE:
                case EVENT_PLAGUEWOOD_PROGRESS_HORDE:
                    // handle event
                    pOutdoorPvp->ProcessCaptureEvent(PROGRESS, ((Player*)pSource)->GetTeam(), TOWER_ID_PLAGUEWOOD);
                    // handle the zone emote
                    // ToDO:
                    break;
                case EVENT_PLAGUEWOOD_NEUTRAL_ALLIANCE:
                case EVENT_PLAGUEWOOD_NEUTRAL_HORDE:
                    // handle event - in this case player's team will be the opposite team
                    pOutdoorPvp->ProcessCaptureEvent(NEUTRAL, ((Player*)pSource)->GetTeam(), TOWER_ID_PLAGUEWOOD);
                    // handle the zone emote
                    // ToDo:
                    break;
            }
            return true;
        }
    }
    return false;
}

void AddSC_worldmap_eastern_kingdoms()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "worldmap_eastern_kingdoms";
    pNewScript->GetInstanceData = &GetInstanceData_worldmap_eastern_kingdoms;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "event_crownguard_tower";
    pNewScript->pProcessEventId = &ProcessEventId_event_crownguard_tower;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "event_eastwall_tower";
    pNewScript->pProcessEventId = &ProcessEventId_event_eastwall_tower;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "event_northpass_tower";
    pNewScript->pProcessEventId = &ProcessEventId_event_northpass_tower;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "event_plaguewood_tower";
    pNewScript->pProcessEventId = &ProcessEventId_event_plaguewood_tower;
    pNewScript->RegisterSelf();
}
