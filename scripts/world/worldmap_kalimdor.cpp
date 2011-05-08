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
SDName: Worldmap_kalimdor
SD%Complete:
SDComment:
SDCategory: Kalimdor
EndScriptData */

#include "precompiled.h"
#include "worldmap_kalimdor.h"

worldmap_kalimdor::worldmap_kalimdor(Map* pMap) : ScriptedInstance(pMap),
    m_uiResourcesAly(0),
    m_uiResourcesHorde(0),
    m_uiLastControllerTeam(0){ }

void worldmap_kalimdor::OnPlayerEnterZone(Player* pPlayer, uint32 uiZoneId)
{
    if (uiZoneId == ZONE_ID_SILITHUS)
    {
        if(pPlayer->GetTeam() == m_uiLastControllerTeam)
            pPlayer->CastSpell(pPlayer, SPELL_CENARION_FAVOR, false);

        // add to the player set
        m_suiSilithusPlayers.insert(pPlayer->GetGUID());

        // send actual world states
        SendPlayerWorldState(pPlayer);
    }
    else
    {
        if (pPlayer->HasAura(SPELL_CENARION_FAVOR))
            pPlayer->RemoveAurasDueToSpell(SPELL_CENARION_FAVOR);

        // remove from the player set
        if (m_suiSilithusPlayers.find(pPlayer->GetGUID()) != m_suiSilithusPlayers.end())
            m_suiSilithusPlayers.erase(pPlayer->GetGUID());
    }
}

void worldmap_kalimdor::SetData(uint32 uiType, uint32 uiData)
{
    switch(uiType)
    {
        case TYPE_ALLIANCE_SILITHYSTS:
            if (uiData)
                m_uiResourcesAly += uiData;
            else
                m_uiResourcesAly = uiData;

            if (GetData(TYPE_ALLIANCE_SILITHYSTS) == MAX_SILITHYST)
            {
                SetData(TYPE_CONTROLLER_FACTION, ALLIANCE);
                DoProcessTeamBuff(ALLIANCE, SPELL_CENARION_FAVOR);

                // send zone emote
                //sWorld.SendZoneText(ZONE_ID_SILITHUS, ZONE_EMOTE_ALY_CAPTURE);

                // reset counters
                SetData(TYPE_ALLIANCE_SILITHYSTS, 0);
                SetData(TYPE_HORDE_SILITHYSTS, 0);
            }
            break;
        case TYPE_HORDE_SILITHYSTS:
            if (uiData)
                m_uiResourcesHorde += uiData;
            else
                m_uiResourcesHorde = uiData;

            if (GetData(TYPE_HORDE_SILITHYSTS) == MAX_SILITHYST)
            {
                SetData(TYPE_CONTROLLER_FACTION, HORDE);
                DoProcessTeamBuff(HORDE, SPELL_CENARION_FAVOR);

                // send zone emote
                //sWorld.SendZoneText(ZONE_ID_SILITHUS, ZONE_EMOTE_HORDE_CAPTURE);

                // reset counters
                SetData(TYPE_ALLIANCE_SILITHYSTS, 0);
                SetData(TYPE_HORDE_SILITHYSTS, 0);
            }
            break;
        case TYPE_CONTROLLER_FACTION:
            m_uiLastControllerTeam = uiData;
            break;
    }

    // update states
    UpdateZoneWorldState();
}

uint32 worldmap_kalimdor::GetData(uint32 uiType)
{
    switch(uiType)
    {
        case TYPE_ALLIANCE_SILITHYSTS:
            return m_uiResourcesAly;
        case TYPE_HORDE_SILITHYSTS:
            return m_uiResourcesHorde;
        case TYPE_CONTROLLER_FACTION:
            return m_uiLastControllerTeam;
    }
    return 0;
}

void worldmap_kalimdor::UpdateZoneWorldState()
{
    DoUpdateZoneWorldState(WORLD_STATE_SI_GATHERED_A, m_uiResourcesAly);
    DoUpdateZoneWorldState(WORLD_STATE_SI_GATHERED_H, m_uiResourcesHorde);
}

void worldmap_kalimdor::SendPlayerWorldState(Player* pPlayer)
{
    pPlayer->SendUpdateWorldState(WORLD_STATE_SI_GATHERED_A, m_uiResourcesAly);
    pPlayer->SendUpdateWorldState(WORLD_STATE_SI_GATHERED_H, m_uiResourcesHorde);
    pPlayer->SendUpdateWorldState(WORLD_STATE_SI_SILITHYST_MAX, MAX_SILITHYST);
}

void worldmap_kalimdor::DoUpdateZoneWorldState(uint32 uiStateId, uint32 uiStateData)
{
    for(std::set<uint64>::const_iterator itr = m_suiSilithusPlayers.begin(); itr != m_suiSilithusPlayers.end(); ++itr)
    {
        if (Player* pPlayer = instance->GetPlayer(*itr))
            pPlayer->SendUpdateWorldState(uiStateId, uiStateData);
    }
}

void worldmap_kalimdor::DoProcessTeamBuff(Team uiTeamId, uint32 uiSpellId, bool bRemove /*= false*/)
{
    for(std::set<uint64>::const_iterator itr = m_suiSilithusPlayers.begin(); itr != m_suiSilithusPlayers.end(); ++itr)
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

void worldmap_kalimdor::OnPlayerDroppedFlag(Player* pPlayer, uint32 uiSpellId)
{
    if (uiSpellId != SPELL_SILITHYST)
        return;

    // don't drop flag at area trigger
    switch(pPlayer->GetTeam())
    {
        case ALLIANCE:
            if (pPlayer->IsWithinDist3d(m_aSilithusLocs[0].m_fX, m_aSilithusLocs[0].m_fY, m_aSilithusLocs[0].m_fZ, 5.0f))
                return;
            break;
        case HORDE:
            if (pPlayer->IsWithinDist3d(m_aSilithusLocs[1].m_fX, m_aSilithusLocs[1].m_fY, m_aSilithusLocs[1].m_fZ, 5.0f))
                return;
            break;
    }

    // drop the flag in other case
    pPlayer->CastSpell(pPlayer, SPELL_SILITHYST_FLAG_DROP, true);
}

bool AreaTrigger_at_silithus(Player* pPlayer, AreaTriggerEntry const* pAt)
{
    if (pPlayer->isGameMaster() || pPlayer->isDead())
        return false;

    worldmap_kalimdor* pWorldMap = (worldmap_kalimdor*)pPlayer->GetInstanceData();

    if (!pWorldMap)
        return false;

    if (pAt->id == AREATRIGGER_SILITHUS_ALY)
    {
        if(pPlayer->GetTeam() == ALLIANCE && pPlayer->HasAura(SPELL_SILITHYST))
        {
            // remove aura
            pPlayer->RemoveAurasDueToSpell(SPELL_SILITHYST);
            pWorldMap->SetData(TYPE_ALLIANCE_SILITHYSTS, 1);

            // reward the player
            pPlayer->CastSpell(pPlayer, SPELL_TRACES_OF_SILITHYST, false);
            pPlayer->RewardHonor(NULL, 1, HONOR_REWARD_SILITHYST);
            //pPlayer->GetReputationMgr().ModifyReputation(sFactionStore.LookupEntry(FACTION_CENARION_CIRCLE), REPUTATION_REWARD_SILITHYST);

            // complete quest
            if (pPlayer->GetQuestStatus(QUEST_SCOURING_DESERT_ALY) == QUEST_STATUS_INCOMPLETE)
                pPlayer->KilledMonsterCredit(NPC_SILITHUS_DUST_QUEST_ALY);
        }
    }
    else if (pAt->id == AREATRIGGER_SILITHUS_HORDE)
    {
        if(pPlayer->GetTeam() == HORDE && pPlayer->HasAura(SPELL_SILITHYST))
        {
            // remove aura
            pPlayer->RemoveAurasDueToSpell(SPELL_SILITHYST);
            pWorldMap->SetData(TYPE_HORDE_SILITHYSTS, 1);

            // reward the player
            pPlayer->CastSpell(pPlayer, SPELL_TRACES_OF_SILITHYST, false);
            pPlayer->RewardHonor(NULL, 1, HONOR_REWARD_SILITHYST);
            //pPlayer->GetReputationMgr().ModifyReputation(sFactionStore.LookupEntry(FACTION_CENARION_CIRCLE), REPUTATION_REWARD_SILITHYST);

            // complete quest
            if (pPlayer->GetQuestStatus(QUEST_SCOURING_DESERT_HORDE) == QUEST_STATUS_INCOMPLETE)
                pPlayer->KilledMonsterCredit(NPC_SILITHUS_DUST_QUEST_HORDE);
        }
    }

    return false;
}

bool GOUse_go_silithyst(Player* pPlayer, GameObject* pGo)
{
    // ToDo - in the old DBCs this GO has a spell, so it doesn't need script
    pPlayer->CastSpell(pPlayer, SPELL_SILITHYST, false);
    pGo->Delete();

    return true;
}

InstanceData* GetInstanceData_worldmap_kalimdor(Map* pMap)
{
    return new worldmap_kalimdor(pMap);
}

void AddSC_worldmap_kalimdor()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "at_silithus";
    pNewScript->pAreaTrigger = &AreaTrigger_at_silithus;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "go_silithyst";
    pNewScript->pGOUse = &GOUse_go_silithyst;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "worldmap_kalimdor";
    pNewScript->GetInstanceData = &GetInstanceData_worldmap_kalimdor;
    pNewScript->RegisterSelf();
}