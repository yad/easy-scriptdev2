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
SDName: instance_draktharon_keep
SD%Complete: 50%
SDComment:
SDCategory: Drak'Tharon Keep
EndScriptData */

#include "precompiled.h"
#include "draktharon_keep.h"

instance_draktharon_keep::instance_draktharon_keep(Map* pMap) : ScriptedInstance(pMap),
    m_uiDreadAddsKilled(0),
    m_uiNovosGUID(0),
    m_uiNovosChannelTargetGUID(0),
    m_bNovosAddGrounded(false),
    m_bTrollgoreConsume(true)
{
    Initialize();
}

void instance_draktharon_keep::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
    memset(&m_auiRitualCrystalGUID, 0, sizeof(m_auiRitualCrystalGUID));
}

void instance_draktharon_keep::OnCreatureCreate(Creature* pCreature)
{
    switch(pCreature->GetEntry())
    {
        case NPC_NOVOS: m_uiNovosGUID = pCreature->GetGUID(); break;
        case NPC_CRYSTAL_CHANNEL_TARGET:
            // channel target npc right above novos head
            if (pCreature->GetDistance(-379.578f, -738.532f, 37.9125f) < INTERACTION_DISTANCE)
                m_uiNovosChannelTargetGUID = pCreature->GetGUID();
            break;
    }
}

void instance_draktharon_keep::OnObjectCreate(GameObject* pGo)
{
    switch(pGo->GetEntry())
    {
        case GO_RITUAL_CRYSTAL_SW:
        case GO_RITUAL_CRYSTAL_SE:
        case GO_RITUAL_CRYSTAL_NW:
        case GO_RITUAL_CRYSTAL_NE:
            for (uint8 i = 0; i < CRYSTAL_NUMBER; ++i)
                if (!m_auiRitualCrystalGUID[i])
                {
                    m_auiRitualCrystalGUID[i] = pGo->GetGUID();
                    break;
                }
    }
}

void instance_draktharon_keep::OnCreatureEnterCombat(Creature* pCreature)
{
    if (pCreature->GetEntry() == NPC_KING_DRED)
        SetData(TYPE_KING_DRED, IN_PROGRESS);
}

void instance_draktharon_keep::OnCreatureEvade(Creature* pCreature)
{
    if (pCreature->GetEntry() == NPC_KING_DRED)
        SetData(TYPE_KING_DRED, FAIL);
}

void instance_draktharon_keep::OnCreatureDeath(Creature* pCreature)
{
    if ((pCreature->GetEntry() == NPC_DRAKKARI_GUTRIPPER || pCreature->GetEntry() == NPC_DRAKKARI_SCYTHECLAW) && m_auiEncounter[TYPE_KING_DRED] == IN_PROGRESS)
        m_uiDreadAddsKilled++;

    if (pCreature->GetEntry() == NPC_KING_DRED)
        SetData(TYPE_KING_DRED, DONE);
}

bool instance_draktharon_keep::CheckAchievementCriteriaMeet(uint32 uiCriteriaId, Player const* pSource, Unit const* pTarget, uint32 uiMiscValue1 /* = 0*/)
{
    switch (uiCriteriaId)
    {
        case ACHIEV_CRIT_BETTER_OFF_DREAD: return m_uiDreadAddsKilled >= 6;
        case ACHIEV_CRIT_OH_NOVOS:         return !m_bNovosAddGrounded;
        case ACHIEV_CRIT_CONSUME_JUNCTION: return m_bTrollgoreConsume;
        default:
            return false;
    }
}

void instance_draktharon_keep::SetData(uint32 uiType, uint32 uiData)
{
    switch(uiType)
    {
        case TYPE_TROLLGORE:
            if (uiData == IN_PROGRESS)
                m_bTrollgoreConsume = true;
            if (uiData == SPECIAL)
                m_bTrollgoreConsume = false;
            m_auiEncounter[uiType] = uiData;
            break;
        case TYPE_NOVOS:
            if (uiData == IN_PROGRESS)
                m_bNovosAddGrounded = false;
            if (uiData == SPECIAL)
                m_bNovosAddGrounded = true;
            m_auiEncounter[uiType] = uiData;
            break;
        case TYPE_CRYSTAL_EVENT:
            ManageCrystals(uiData);
            break;
        case TYPE_KING_DRED:
            if (uiData == IN_PROGRESS)
                m_uiDreadAddsKilled = 0;
            m_auiEncounter[uiType] = uiData;
            break;
        case TYPE_THARONJA:
            m_auiEncounter[uiType] = uiData;
            break;
    }

    if (uiData == DONE)
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;
        saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " " << m_auiEncounter[3];

        strInstData = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

void instance_draktharon_keep::Load(const char* chrIn)
{
    if (!chrIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(chrIn);

    std::istringstream loadStream(chrIn);
    loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >> m_auiEncounter[3];

    for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            m_auiEncounter[i] = NOT_STARTED;
    }

    OUT_LOAD_INST_DATA_COMPLETE;
}

uint32 instance_draktharon_keep::GetData(uint32 uiType)
{
    switch(uiType)
    {
        case TYPE_TROLLGORE: return m_auiEncounter[uiType];
        case TYPE_NOVOS:     return m_auiEncounter[uiType];
        case TYPE_KING_DRED: return m_auiEncounter[uiType];
        case TYPE_THARONJA:  return m_auiEncounter[uiType];
        default:
            return 0;
    }
}

uint64 instance_draktharon_keep::GetData64(uint32 uiType)
{
    switch(uiType)
    {
        case NPC_NOVOS:     return m_uiNovosGUID;
        default:            return 0;
    }
}

void instance_draktharon_keep::ManageCrystals(uint32 action)
{
    switch(action)
    {
        case DEACTIVATE_ONE:
            for (uint8 i = 0; i < CRYSTAL_NUMBER; ++i)
                if (GameObject* pCrystal = instance->GetGameObject(m_auiRitualCrystalGUID[i]))
                    if (pCrystal->GetGoState() == GO_STATE_ACTIVE)
                    {
                        pCrystal->SetGoState(GO_STATE_READY);
                        if (Creature* pChannelTarget = GetClosestCreatureWithEntry(pCrystal, NPC_CRYSTAL_CHANNEL_TARGET, INTERACTION_DISTANCE))
                        {
                            // it does not work this way :(
                            pChannelTarget->InterruptNonMeleeSpells(false);
                            // hack
                            pChannelTarget->SetVisibility(VISIBILITY_OFF);
                        }
                        break;
                    }
            break;
        case RESET:
            for (uint8 i = 0; i < CRYSTAL_NUMBER; ++i)
                if (GameObject* pCrystal = instance->GetGameObject(m_auiRitualCrystalGUID[i]))
                {
                    pCrystal->SetGoState(GO_STATE_ACTIVE);
                    if (Creature* pChannelTarget = GetClosestCreatureWithEntry(pCrystal, NPC_CRYSTAL_CHANNEL_TARGET, INTERACTION_DISTANCE))
                    {
                        // it does not work this way :( 
                        pChannelTarget->InterruptNonMeleeSpells(false);
                        // hack
                        pChannelTarget->SetVisibility(VISIBILITY_OFF);
                    }
                }
            break;
        case ACTIVATE_BEAMS:
            for (uint8 i = 0; i < CRYSTAL_NUMBER; ++i)
                if (GameObject* pCrystal = instance->GetGameObject(m_auiRitualCrystalGUID[i]))
                    if (Creature* pChannelTarget = GetClosestCreatureWithEntry(pCrystal, NPC_CRYSTAL_CHANNEL_TARGET, INTERACTION_DISTANCE))
                    {
                        if (Creature* pNovosChannelTarget = instance->GetCreature(m_uiNovosChannelTargetGUID))
                            pChannelTarget->CastSpell(pNovosChannelTarget, SPELL_CHANNEL_BEAM, true);
                        // hack
                        pChannelTarget->SetVisibility(VISIBILITY_ON);
                    }
            break;
    }
}

InstanceData* GetInstanceData_instance_draktharon_keep(Map* pMap)
{
    return new instance_draktharon_keep(pMap);
}

void AddSC_instance_draktharon_keep()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_draktharon_keep";
    pNewScript->GetInstanceData = &GetInstanceData_instance_draktharon_keep;
    pNewScript->RegisterSelf();
}
