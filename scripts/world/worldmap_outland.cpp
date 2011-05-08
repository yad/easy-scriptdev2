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
SDName: Worldmap_outland
SD%Complete:
SDComment:
SDCategory: Outland
EndScriptData */

#include "precompiled.h"
#include "worldmap_outland.h"

/*  Comments
 * This implements the basics of tower capturing in Eastern Plaguelands
 * When a player faction captures a capture point then it triggers some event (helpers + buff)
 * The tower states are processed by the capture point events
 */

worldmap_outland::worldmap_outland(Map* pMap) : ScriptedInstance(pMap),

    m_uiOverlookController(0),
    m_uiStadiumController(0),
    m_uiBrokenHillController(0),
    m_uiOverlookState(NEUTRAL),
    m_uiStadiumState(NEUTRAL),
    m_uiBrokenHillState(NEUTRAL),
    m_uiOverlookWorldState(WORLD_STATE_OVERLOOK_NEUTRAL),
    m_uiStadiumWorldState(WORLD_STATE_STADIUM_NEUTRAL),
    m_uiBrokenHillWorldState(WORLD_STATE_BROKEN_HILL_NEUTRAL),
    m_uiTowersAlly(0),
    m_uiTowersHorde(0),
    m_uiTowerBannerOverlookGUID(0),
    m_uiTowerBannerStadiumGUID(0),
    m_uiTowerBannerBrokenHillGUID(0),
    m_uiTowerPointOverlookGUID(0),
    m_uiTowerPointStadiumGUID(0),
    m_uiTowerPointBrokenHillGUID(0) { }

void worldmap_outland::OnObjectCreate(GameObject* pGo)
{
    switch(pGo->GetEntry())
    {
        case GO_TOWER_BANNER_OVERLOOK:
            m_uiTowerBannerOverlookGUID = pGo->GetGUID();
            pGo->SetGoArtKit(GO_ARTKIT_OVERLOOK_NEUTRAL);
            break;
        case GO_TOWER_BANNER_STADIUM:
            m_uiTowerBannerStadiumGUID = pGo->GetGUID();
            pGo->SetGoArtKit(GO_ARTKIT_STADIUM_NEUTRAL);
            break;
        case GO_TOWER_BANNER_BRONEK_HILL:
            m_uiTowerBannerBrokenHillGUID = pGo->GetGUID();
            pGo->SetGoArtKit(GO_ARTKIT_BROKEN_HILL_NEUTRAL);
            break;
        case GO_HELLFIRE_BANNER_OVERLOOK:
            m_uiTowerPointOverlookGUID = pGo->GetGUID();
            pGo->SetGoArtKit(GO_ARTKIT_BANNER_NEUTRAL);
            break;
        case GO_HELLFIRE_BANNER_STADIUM:
            m_uiTowerPointStadiumGUID = pGo->GetGUID();
            pGo->SetGoArtKit(GO_ARTKIT_BANNER_NEUTRAL);
            break;
        case GO_HELLFIRE_BANNER_BROKEN_HILL:
            m_uiTowerPointBrokenHillGUID = pGo->GetGUID();
            pGo->SetGoArtKit(GO_ARTKIT_BANNER_NEUTRAL);
            break;
    }
}

void worldmap_outland::OnPlayerEnterZone(Player* pPlayer, uint32 uiZoneId)
{
    if (uiZoneId == ZONE_ID_HELLFIRE_PENINSULA)
    {
        // cast buff the the player which enters the zone
        switch(pPlayer->GetTeam())
        {
            case ALLIANCE:
                if (m_uiTowersAlly == MAX_TOWERS)
                    pPlayer->CastSpell(pPlayer, SPELL_HELLFIRE_SUPERIORITY_ALY, false);
                break;
            case HORDE:
                if (m_uiTowersHorde == MAX_TOWERS)
                    pPlayer->CastSpell(pPlayer, SPELL_HELLFIRE_SUPERIORITY_HORDE, false);
                break;
        }

        // add to the player set
        m_suiHellfirePlayers.insert(pPlayer->GetGUID());

        // send actual world states
        SendPlayerWorldState(pPlayer);
    }
    else
    {
        if (pPlayer->HasAura(pPlayer->GetTeam() == ALLIANCE ? SPELL_HELLFIRE_SUPERIORITY_ALY : SPELL_HELLFIRE_SUPERIORITY_HORDE))
            pPlayer->RemoveAurasDueToSpell(pPlayer->GetTeam() == ALLIANCE ? SPELL_HELLFIRE_SUPERIORITY_ALY : SPELL_HELLFIRE_SUPERIORITY_HORDE);

        // remove from the player set
        if (m_suiHellfirePlayers.find(pPlayer->GetGUID()) != m_suiHellfirePlayers.end())
            m_suiHellfirePlayers.erase(pPlayer->GetGUID());
    }
}

void worldmap_outland::SetData(uint32 uiType, uint32 uiData)
{
    switch(uiType)
    {
        case TYPE_OVERLOOK_CONTROLLER:
            if (uiData == ALLIANCE)
            {
                if (GetData(TYPE_OVERLOOK_STATE) == NEUTRAL)
                {
                    // in neutral case the team id is the opposite team
                    // the team who captured the tower and set it to neutral
                    --m_uiTowersHorde;
                    m_uiOverlookWorldState = WORLD_STATE_OVERLOOK_NEUTRAL;

                    // set artkit
                    if (GameObject* pTower = instance->GetGameObject(m_uiTowerBannerOverlookGUID))
                        pTower->SetGoArtKit(GO_ARTKIT_OVERLOOK_NEUTRAL);
                    if (GameObject* pCapture = instance->GetGameObject(m_uiTowerPointOverlookGUID))
                        pCapture->SetGoArtKit(GO_ARTKIT_BANNER_NEUTRAL);
                }
                else if (GetData(TYPE_OVERLOOK_STATE) == PROGRESS)
                {
                    m_uiOverlookWorldState = WORLD_STATE_OVERLOOK_ALY;
                    // increase tower count only if the controller is changed
                    if (uiData != GetData(TYPE_OVERLOOK_CONTROLLER))
                        ++m_uiTowersAlly;

                    // set artkit
                    if (GameObject* pTower = instance->GetGameObject(m_uiTowerBannerOverlookGUID))
                        pTower->SetGoArtKit(GO_ARTKIT_OVERLOOK_ALY);
                    if (GameObject* pCapture = instance->GetGameObject(m_uiTowerPointOverlookGUID))
                        pCapture->SetGoArtKit(GO_ARTKIT_BANNER_ALLIANCE);
                }
            }
            else if (uiData == HORDE)
            {
                if (GetData(TYPE_OVERLOOK_STATE) == NEUTRAL)
                {
                    // in neutral case the team id is the opposite team
                    // the team who captured the tower and set it to neutral
                    --m_uiTowersAlly;
                    m_uiOverlookWorldState = WORLD_STATE_OVERLOOK_NEUTRAL;

                    // set artkit
                    if (GameObject* pTower = instance->GetGameObject(m_uiTowerBannerOverlookGUID))
                        pTower->SetGoArtKit(GO_ARTKIT_OVERLOOK_NEUTRAL);
                    if (GameObject* pCapture = instance->GetGameObject(m_uiTowerPointOverlookGUID))
                        pCapture->SetGoArtKit(GO_ARTKIT_BANNER_NEUTRAL);
                }
                if (GetData(TYPE_OVERLOOK_STATE) == PROGRESS)
                {
                    m_uiOverlookWorldState = WORLD_STATE_OVERLOOK_HORDE;
                    // increase tower count only if the controller is changed
                    if (uiData != GetData(TYPE_OVERLOOK_CONTROLLER))
                        ++m_uiTowersHorde;

                    // set artkit
                    if (GameObject* pTower = instance->GetGameObject(m_uiTowerBannerOverlookGUID))
                        pTower->SetGoArtKit(GO_ARTKIT_OVERLOOK_HORDE);
                    if (GameObject* pCapture = instance->GetGameObject(m_uiTowerPointOverlookGUID))
                        pCapture->SetGoArtKit(GO_ARTKIT_BANNER_HORDE);
                }
            }
            // set controller only for progress and neutral
            if (GetData(TYPE_OVERLOOK_STATE) == PROGRESS)
                m_uiOverlookController = uiData;
            else if (GetData(TYPE_OVERLOOK_STATE) == NEUTRAL)
                m_uiOverlookController = 0;
            break;
        case TYPE_STADIUM_CONTROLLER:
            if (uiData == ALLIANCE)
            {
                if (GetData(TYPE_STADIUM_STATE) == NEUTRAL)
                {
                    // in neutral case the team id is the opposite team
                    // the team who captured the tower and set it to neutral
                    --m_uiTowersHorde;
                    m_uiStadiumWorldState = WORLD_STATE_STADIUM_NEUTRAL;

                    // set artkit
                    if (GameObject* pTower = instance->GetGameObject(m_uiTowerBannerStadiumGUID))
                        pTower->SetGoArtKit(GO_ARTKIT_STADIUM_NEUTRAL);
                    if (GameObject* pCapture = instance->GetGameObject(m_uiTowerPointStadiumGUID))
                        pCapture->SetGoArtKit(GO_ARTKIT_BANNER_NEUTRAL);
                }
                else if (GetData(TYPE_STADIUM_STATE) == PROGRESS)
                {
                    m_uiStadiumWorldState = WORLD_STATE_STADIUM_ALY;
                    // increase tower count only if the controller is changed
                    if (uiData != GetData(TYPE_STADIUM_CONTROLLER))
                        ++m_uiTowersAlly;

                    // set artkit
                    if (GameObject* pTower = instance->GetGameObject(m_uiTowerBannerStadiumGUID))
                        pTower->SetGoArtKit(GO_ARTKIT_STADIUM_ALY);
                    if (GameObject* pCapture = instance->GetGameObject(m_uiTowerPointStadiumGUID))
                        pCapture->SetGoArtKit(GO_ARTKIT_BANNER_ALLIANCE);
                }
            }
            else if (uiData == HORDE)
            {
                if (GetData(TYPE_STADIUM_STATE) == NEUTRAL)
                {
                    // in neutral case the team id is the opposite team
                    // the team who captured the tower and set it to neutral
                    --m_uiTowersAlly;
                    m_uiStadiumWorldState = WORLD_STATE_STADIUM_NEUTRAL;

                    // set artkit
                    if (GameObject* pTower = instance->GetGameObject(m_uiTowerBannerStadiumGUID))
                        pTower->SetGoArtKit(GO_ARTKIT_STADIUM_NEUTRAL);
                    if (GameObject* pCapture = instance->GetGameObject(m_uiTowerPointStadiumGUID))
                        pCapture->SetGoArtKit(GO_ARTKIT_BANNER_NEUTRAL);
                }
                if (GetData(TYPE_STADIUM_STATE) == PROGRESS)
                {
                    m_uiStadiumWorldState = WORLD_STATE_STADIUM_HORDE;
                    // increase tower count only if the controller is changed
                    if (uiData != GetData(TYPE_STADIUM_CONTROLLER))
                        ++m_uiTowersHorde;

                    // set artkit
                    if (GameObject* pTower = instance->GetGameObject(m_uiTowerBannerStadiumGUID))
                        pTower->SetGoArtKit(GO_ARTKIT_STADIUM_HORDE);
                    if (GameObject* pCapture = instance->GetGameObject(m_uiTowerPointStadiumGUID))
                        pCapture->SetGoArtKit(GO_ARTKIT_BANNER_HORDE);
                }
            }
            // set controller only for progress and neutral
            if (GetData(TYPE_STADIUM_STATE) == PROGRESS)
                m_uiStadiumController = uiData;
            else if (GetData(TYPE_STADIUM_STATE) == NEUTRAL)
                m_uiStadiumController = 0;
            break;
        case TYPE_BROKEN_HILL_CONTROLLER:
            if (uiData == ALLIANCE)
            {
                if (GetData(TYPE_BROKEN_HILL_STATE) == NEUTRAL)
                {
                    // in neutral case the team id is the opposite team
                    // the team who captured the tower and set it to neutral
                    --m_uiTowersHorde;
                    m_uiBrokenHillWorldState = WORLD_STATE_BROKEN_HILL_NEUTRAL;

                    // set artkit
                    if (GameObject* pTower = instance->GetGameObject(m_uiTowerBannerBrokenHillGUID))
                        pTower->SetGoArtKit(GO_ARTKIT_BROKEN_HILL_NEUTRAL);
                    if (GameObject* pCapture = instance->GetGameObject(m_uiTowerPointBrokenHillGUID))
                        pCapture->SetGoArtKit(GO_ARTKIT_BANNER_NEUTRAL);
                }
                else if (GetData(TYPE_BROKEN_HILL_STATE) == PROGRESS)
                {
                    m_uiBrokenHillWorldState = WORLD_STATE_BROKEN_HILL_ALY;
                    // increase tower count only if the controller is changed
                    if (uiData != GetData(TYPE_BROKEN_HILL_CONTROLLER))
                        ++m_uiTowersAlly;

                    // set artkit
                    if (GameObject* pTower = instance->GetGameObject(m_uiTowerBannerBrokenHillGUID))
                        pTower->SetGoArtKit(GO_ARTKIT_BROKEN_HILL_ALY);
                    if (GameObject* pCapture = instance->GetGameObject(m_uiTowerPointBrokenHillGUID))
                        pCapture->SetGoArtKit(GO_ARTKIT_BANNER_ALLIANCE);
                }
            }
            else if (uiData == HORDE)
            {
                if (GetData(TYPE_BROKEN_HILL_STATE) == NEUTRAL)
                {
                    // in neutral case the team id is the opposite team
                    // the team who captured the tower and set it to neutral
                    --m_uiTowersAlly;
                    m_uiBrokenHillWorldState = WORLD_STATE_BROKEN_HILL_NEUTRAL;

                    // set artkit
                    if (GameObject* pTower = instance->GetGameObject(m_uiTowerBannerBrokenHillGUID))
                        pTower->SetGoArtKit(GO_ARTKIT_BROKEN_HILL_NEUTRAL);
                    if (GameObject* pCapture = instance->GetGameObject(m_uiTowerPointBrokenHillGUID))
                        pCapture->SetGoArtKit(GO_ARTKIT_BANNER_NEUTRAL);
                }
                if (GetData(TYPE_BROKEN_HILL_STATE) == PROGRESS)
                {
                    m_uiBrokenHillWorldState = WORLD_STATE_BROKEN_HILL_HORDE;
                    // increase tower count only if the controller is changed
                    if (uiData != GetData(TYPE_BROKEN_HILL_CONTROLLER))
                        ++m_uiTowersHorde;

                    // set artkit
                    if (GameObject* pTower = instance->GetGameObject(m_uiTowerBannerBrokenHillGUID))
                        pTower->SetGoArtKit(GO_ARTKIT_BROKEN_HILL_HORDE);
                    if (GameObject* pCapture = instance->GetGameObject(m_uiTowerPointBrokenHillGUID))
                        pCapture->SetGoArtKit(GO_ARTKIT_BANNER_HORDE);
                }
            }
            // set controller only for progress and neutral
            if (GetData(TYPE_BROKEN_HILL_STATE) == PROGRESS)
                m_uiBrokenHillController = uiData;
            else if (GetData(TYPE_BROKEN_HILL_STATE) == NEUTRAL)
                m_uiBrokenHillController = 0;
            break;
        case TYPE_OVERLOOK_STATE:
            m_uiOverlookState = uiData;
            break;
        case TYPE_STADIUM_STATE:
            m_uiStadiumState = uiData;
            break;
        case TYPE_BROKEN_HILL_STATE:
            m_uiBrokenHillState = uiData;
            break;
    }

    // buff alliance
    if (m_uiTowersAlly == MAX_TOWERS)
        DoProcessTeamBuff(ALLIANCE, SPELL_HELLFIRE_SUPERIORITY_ALY);
    // buff horde
    if (m_uiTowersHorde == MAX_TOWERS)
        DoProcessTeamBuff(HORDE, SPELL_HELLFIRE_SUPERIORITY_HORDE);

    // debuff players if towers == 0; spell to remove will be always the first
    if (m_uiTowersHorde < MAX_TOWERS)
        DoProcessTeamBuff(HORDE, SPELL_HELLFIRE_SUPERIORITY_HORDE, true);
    if (m_uiTowersAlly < MAX_TOWERS)
        DoProcessTeamBuff(ALLIANCE, SPELL_HELLFIRE_SUPERIORITY_ALY, true);

    // update states counters
    // the map tower states are updated in the ProcessCaptureEvent function
    UpdateZoneWorldState(WORLD_STATE_TOWER_COUNT_ALY, m_uiTowersAlly);
    UpdateZoneWorldState(WORLD_STATE_TOWER_COUNT_HORDE, m_uiTowersHorde);
}

uint32 worldmap_outland::GetData(uint32 uiType)
{
    switch(uiType)
    {
        case TYPE_OVERLOOK_CONTROLLER:
            return m_uiOverlookController;
        case TYPE_STADIUM_CONTROLLER:
            return m_uiStadiumController;
        case TYPE_BROKEN_HILL_CONTROLLER:
            return m_uiBrokenHillController;
        case TYPE_OVERLOOK_STATE:
            return m_uiOverlookState;
        case TYPE_STADIUM_STATE:
            return m_uiStadiumState;
        case TYPE_BROKEN_HILL_STATE:
            return m_uiBrokenHillState;
    }
    return 0;
}

void worldmap_outland::UpdateZoneWorldState(uint32 uiStateId, uint32 uiStateValue)
{
    DoUpdateWorldState(uiStateId, uiStateValue);
}

void worldmap_outland::SendPlayerWorldState(Player* pPlayer)
{
    // counters
    pPlayer->SendUpdateWorldState(WORLD_STATE_TOWER_COUNT_ALY, m_uiTowersAlly);
    pPlayer->SendUpdateWorldState(WORLD_STATE_TOWER_COUNT_HORDE, m_uiTowersHorde);
    // towers
    pPlayer->SendUpdateWorldState(m_uiOverlookWorldState, 1);
    pPlayer->SendUpdateWorldState(m_uiStadiumWorldState, 1);
    pPlayer->SendUpdateWorldState(m_uiBrokenHillWorldState, 1);
}

void worldmap_outland::ProcessCaptureEvent(uint32 uiCaptureType, uint32 uiTeam, uint32 uiTower)
{
    switch(uiTower)
    {
        case TOWER_ID_OVERLOOK:
            // remove old state
            UpdateZoneWorldState(m_uiOverlookWorldState, 0);
            // update data
            SetData(TYPE_OVERLOOK_STATE, uiCaptureType);
            SetData(TYPE_OVERLOOK_CONTROLLER, uiTeam);
            // send new state
            UpdateZoneWorldState(m_uiOverlookWorldState, 1);
            break;
        case TOWER_ID_STADIUM:
            // remove old state
            UpdateZoneWorldState(m_uiStadiumWorldState, 0);
            // update data
            SetData(TYPE_STADIUM_STATE, uiCaptureType);
            SetData(TYPE_STADIUM_CONTROLLER, uiTeam);
            // send new state
            UpdateZoneWorldState(m_uiStadiumWorldState, 1);
            break;
        case TOWER_ID_BROKEN_HILL:
            // remove old state
            UpdateZoneWorldState(m_uiBrokenHillWorldState, 0);
            // update data
            SetData(TYPE_BROKEN_HILL_STATE, uiCaptureType);
            SetData(TYPE_BROKEN_HILL_CONTROLLER, uiTeam);
            // send new state
            UpdateZoneWorldState(m_uiBrokenHillWorldState, 1);
            break;
    }
}

void worldmap_outland::DoUpdateZoneWorldState(uint32 uiStateId, uint32 uiStateData)
{
    for(std::set<uint64>::const_iterator itr = m_suiHellfirePlayers.begin(); itr != m_suiHellfirePlayers.end(); ++itr)
    {
        if (Player* pPlayer = instance->GetPlayer(*itr))
            pPlayer->SendUpdateWorldState(uiStateId, uiStateData);
    }
}

void worldmap_outland::DoProcessTeamBuff(Team uiTeamId, uint32 uiSpellId, bool bRemove /*= false*/)
{
    for(std::set<uint64>::const_iterator itr = m_suiHellfirePlayers.begin(); itr != m_suiHellfirePlayers.end(); ++itr)
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

void worldmap_outland::OnPlayerDeath(Player* pPlayer)
{
    // Todo: handle bonus spell when a player kills an enemy inside a capture point
    // killer cast self SPELL_HELLFIRE_TOWER_TOKEN_ALY for alliance or SPELL_HELLFIRE_TOWER_TOKEN_HORDE for horde
}

InstanceData* GetInstanceData_worldmap_outland(Map* pMap)
{
    return new worldmap_outland(pMap);
}

bool ProcessEventId_event_overlook_tower(uint32 uiEventId, Object* pSource, Object* pTarget, bool bIsStart)
{
    if (bIsStart && pSource->GetTypeId() == TYPEID_PLAYER)
    {
        if (worldmap_outland* pWorldMap = (worldmap_outland*)((Player*)pSource)->GetInstanceData())
        {
            switch(uiEventId)
            {
                case EVENT_OVERLOOK_WIN_ALLIANCE:
                case EVENT_OVERLOOK_WIN_HORDE:
                    // handle event
                    pWorldMap->ProcessCaptureEvent(WIN, ((Player*)pSource)->GetTeam(), TOWER_ID_OVERLOOK);
                    // complete quest; ToDo: need to check on blizz if this is on win or on progress
                    ((Player*)pSource)->KilledMonsterCredit(NPC_CAPTURE_CREDIT_OVERLOOK);
                    break;
                case EVENT_OVERLOOK_CONTEST_ALLIANCE:
                case EVENT_OVERLOOK_CONTEST_HORDE:
                    // handle event - in this case player's team will be the opposite team
                    pWorldMap->ProcessCaptureEvent(CONTESTED, ((Player*)pSource)->GetTeam(), TOWER_ID_OVERLOOK);
                    break;
                case EVENT_OVERLOOK_PROGRESS_ALLIANCE:
                case EVENT_OVERLOOK_PROGRESS_HORDE:
                    // handle event
                    pWorldMap->ProcessCaptureEvent(PROGRESS, ((Player*)pSource)->GetTeam(), TOWER_ID_OVERLOOK);
                    // handle the zone emote
                    // ToDO:
                    break;
                case EVENT_OVERLOOK_NEUTRAL_ALLIANCE:
                case EVENT_OVERLOOK_NEUTRAL_HORDE:
                    pWorldMap->ProcessCaptureEvent(NEUTRAL, ((Player*)pSource)->GetTeam(), TOWER_ID_OVERLOOK);
                    break;
            }
            return true;
        }
    }
    return false;
}

bool ProcessEventId_event_stadium_tower(uint32 uiEventId, Object* pSource, Object* pTarget, bool bIsStart)
{
    if (bIsStart && pSource->GetTypeId() == TYPEID_PLAYER)
    {
        if (worldmap_outland* pWorldMap = (worldmap_outland*)((Player*)pSource)->GetInstanceData())
        {
            switch(uiEventId)
            {
                case EVENT_STADIUM_WIN_ALLIANCE:
                case EVENT_STADIUM_WIN_HORDE:
                    // handle event
                    pWorldMap->ProcessCaptureEvent(WIN, ((Player*)pSource)->GetTeam(), TOWER_ID_STADIUM);
                    // complete quest; ToDo: need to check on blizz if this is on win or on progress
                    ((Player*)pSource)->KilledMonsterCredit(NPC_CAPTURE_CREDIT_STADIUM);
                    break;
                case EVENT_STADIUM_CONTEST_ALLIANCE:
                case EVENT_STADIUM_CONTEST_HORDE:
                    // handle event - in this case player's team will be the opposite team
                    pWorldMap->ProcessCaptureEvent(CONTESTED, ((Player*)pSource)->GetTeam(), TOWER_ID_STADIUM);
                    break;
                case EVENT_STADIUM_PROGRESS_ALLIANCE:
                case EVENT_STADIUM_PROGRESS_HORDE:
                    // handle event
                    pWorldMap->ProcessCaptureEvent(PROGRESS, ((Player*)pSource)->GetTeam(), TOWER_ID_STADIUM);
                    // handle the zone emote
                    // ToDO:
                    break;
                case EVENT_STADIUM_NEUTRAL_ALLIANCE:
                case EVENT_STADIUM_NEUTRAL_HORDE:
                    // handle event - in this case player's team will be the opposite team
                    pWorldMap->ProcessCaptureEvent(NEUTRAL, ((Player*)pSource)->GetTeam(), TOWER_ID_STADIUM);
                    // handle the zone emote
                    // ToDo:
                    break;
            }
            return true;
        }
    }
    return false;
}

bool ProcessEventId_event_broken_hill_tower(uint32 uiEventId, Object* pSource, Object* pTarget, bool bIsStart)
{
    if (bIsStart && pSource->GetTypeId() == TYPEID_PLAYER)
    {
        if (worldmap_outland* pWorldMap = (worldmap_outland*)((Player*)pSource)->GetInstanceData())
        {
            switch(uiEventId)
            {
                case EVENT_BROKEN_HILL_WIN_ALLIANCE:
                case EVENT_BROKEN_HILL_WIN_HORDE:
                    // handle event
                    pWorldMap->ProcessCaptureEvent(WIN, ((Player*)pSource)->GetTeam(), TOWER_ID_BROKEN_HILL);
                    // complete quest; ToDo: need to check on blizz if this is on win or on progress
                    ((Player*)pSource)->KilledMonsterCredit(NPC_CAPTURE_CREDIT_BROKEN_HILL);
                    break;
                case EVENT_BROKEN_HILL_CONTEST_ALLIANCE:
                case EVENT_BROKEN_HILL_CONTEST_HORDE:
                    // handle event - in this case player's team will be the opposite team
                    pWorldMap->ProcessCaptureEvent(CONTESTED, ((Player*)pSource)->GetTeam(), TOWER_ID_BROKEN_HILL);
                    break;
                case EVENT_BROKEN_HILL_PROGRESS_ALLIANCE:
                case EVENT_BROKEN_HILL_PROGRESS_HORDE:
                    // handle event
                    pWorldMap->ProcessCaptureEvent(PROGRESS, ((Player*)pSource)->GetTeam(), TOWER_ID_BROKEN_HILL);
                    // handle the zone emote
                    // ToDO:
                    break;
                case EVENT_BROKEN_HILL_NEUTRAL_ALLIANCE:
                case EVENT_BROKEN_HILL_NEUTRAL_HORDE:
                    // handle event - in this case player's team will be the opposite team
                    pWorldMap->ProcessCaptureEvent(NEUTRAL, ((Player*)pSource)->GetTeam(), TOWER_ID_BROKEN_HILL);
                    // handle the zone emote
                    // ToDo:
                    break;
            }
            return true;
        }
    }
    return false;
}

void AddSC_worldmap_outland()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "worldmap_outland";
    pNewScript->GetInstanceData = &GetInstanceData_worldmap_outland;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "event_stadium_tower";
    pNewScript->pProcessEventId = &ProcessEventId_event_stadium_tower;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "event_overlook_tower";
    pNewScript->pProcessEventId = &ProcessEventId_event_overlook_tower;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "event_broken_hill_tower";
    pNewScript->pProcessEventId = &ProcessEventId_event_broken_hill_tower;
    pNewScript->RegisterSelf();
}
