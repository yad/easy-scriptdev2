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
SDName: Boss_Novos
SD%Complete: 20%
SDComment:
SDCategory: Drak'Tharon Keep
EndScriptData */

#include "precompiled.h"
#include "draktharon_keep.h"

enum
{
    SAY_AGGRO                               = -1600005,
    SAY_DEATH                               = -1600006,
    SAY_KILL                                = -1600007,
    SAY_ADDS                                = -1600008,
    SAY_BUBBLE_1                            = -1600009,
    SAY_BUBBLE_2                            = -1600010,

    EMOTE_ASSISTANCE                        = -1600011,

    NPC_CRYSTAL_HANDLER                     = 26627,
    NPC_NOVOS_SUMMON_TARGET                 = 27583,
    NPC_NOVOS_SPELL_DUMMY                   = 27669, // not used
    
    // spells
    SPELL_FIELD                             = 47346,
    
    // normal
    SPELL_FROSTBOLT                         = 49037,
    SPELL_MISERY                            = 50089,
    SPELL_ARCANE                            = 49198,
    SPELL_BLIZZARD                          = 49034,
    SPELL_SUMMON_RISEN_SHADOWCASTER         = 49105,
    SPELL_SUMMON_SUMMON_FETID_TROLL_CORPSE  = 49103,
    SPELL_SUMMON_HULKING_CORPSE             = 49104,
    SPELL_SUMMON_CRISTAL_HANDLER            = 49179,

    SPELL_DESPAWN_CRYSTAL_HANDLERS          = 51403, // not used

    // heroic
    SPELL_FROSTBOLT_H                       = 59855,
    SPELL_MISERY_H                          = 59856,
    SPELL_ARCANE_H                          = 59909,
    SPELL_BLIZZARD_H                        = 59854,
    SPELL_SUMMON_TROLL_CORPSE               = 59910,

    PHASE_SUMMONING                         = 0,
    PHASE_ACTIVE_ATTACKING                  = 1,
    SUMMON_TARGETS_NO                       = 3
};

float fNovosSummonerPosition[3][3] = 
{
    {-379.13f, -813.89f, 59.72f}, // top of stairs
    {-313.18f, -699.73f, 27.29f}, // entrance
    {-421.47f, -716.83f, 28.57f}  // exit
};

// there is higher chance to summon non-elite add
uint32 m_uiRandomSummoningSpell[9] = {SPELL_SUMMON_RISEN_SHADOWCASTER, SPELL_SUMMON_RISEN_SHADOWCASTER, 
SPELL_SUMMON_SUMMON_FETID_TROLL_CORPSE, SPELL_SUMMON_SUMMON_FETID_TROLL_CORPSE, SPELL_SUMMON_RISEN_SHADOWCASTER, 
SPELL_SUMMON_RISEN_SHADOWCASTER, SPELL_SUMMON_SUMMON_FETID_TROLL_CORPSE, SPELL_SUMMON_SUMMON_FETID_TROLL_CORPSE,
SPELL_SUMMON_HULKING_CORPSE};

/*######
## boss_novos
######*/

struct MANGOS_DLL_DECL boss_novosAI : public ScriptedAI
{
    boss_novosAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    
    bool m_bIsRegularMode;
    bool m_bHandlerFromEntrance;
    bool m_bAddsReachedFloor;

    uint8  m_uiPhase;
    uint8  m_uiCrystalsDeactivated;
    uint32 m_uiCheckoutAdds_Timer;
    uint32 m_uiSummonRandomAdd_Timer;
    uint32 m_uiSummonCrystalChanneler_Timer;
    uint32 m_uiFrostbolt_Timer;
    uint32 m_uiMisery_Timer;
    uint32 m_uiArcane_Timer;
    uint32 m_uiBlizzard_Timer;
    uint32 m_uiSummonTrollCorpse_Timer;
    uint64 m_uiSummonTargetGUID[SUMMON_TARGETS_NO];

    GUIDList lSummonGUIDs;

    void Reset()
    {
        m_bHandlerFromEntrance = true;
        m_bAddsReachedFloor = false;
        m_uiPhase = PHASE_SUMMONING;
        m_uiCheckoutAdds_Timer = 0;
        m_uiSummonRandomAdd_Timer = 5000;
        m_uiSummonCrystalChanneler_Timer = 15000;
        m_uiFrostbolt_Timer = 1000;
        m_uiMisery_Timer = 10000;
        m_uiArcane_Timer = 25000;
        m_uiBlizzard_Timer = 30000;
        m_uiSummonTrollCorpse_Timer = 30000;
        m_uiCrystalsDeactivated = 0;
    }

    void DespawnAdds()
    {
        if (!lSummonGUIDs.empty())
        {
            for (GUIDList::iterator itr = lSummonGUIDs.begin(); itr != lSummonGUIDs.end(); ++itr)
                if (Creature* pSummon = m_creature->GetMap()->GetCreature(*itr))
                    pSummon->ForcedDespawn();

            lSummonGUIDs.clear();
        }
    }

    void JustReachedHome()
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_CRYSTAL_EVENT, RESET);
            m_pInstance->SetData(TYPE_NOVOS, FAIL);
        }

        DespawnAdds();
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
    }

    void Aggro(Unit* pWho)
    {
        SetCombatMovement(false);

        for (uint8 i = 0; i < SUMMON_TARGETS_NO; ++i)
        {
            if (Creature* pSummonTarget = m_creature->SummonCreature(NPC_NOVOS_SUMMON_TARGET, fNovosSummonerPosition[i][0], fNovosSummonerPosition[i][1], fNovosSummonerPosition[i][2], 0.0f, TEMPSUMMON_CORPSE_DESPAWN, 1))
            {
                lSummonGUIDs.push_back(pSummonTarget->GetGUID());
                m_uiSummonTargetGUID[i] = pSummonTarget->GetGUID();
            }
        }

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        DoScriptText(SAY_AGGRO, m_creature);
        DoCastSpellIfCan(m_creature, SPELL_FIELD);

        if (!m_pInstance)
            return;

        m_pInstance->SetData(TYPE_NOVOS, IN_PROGRESS);
        m_pInstance->SetData(TYPE_CRYSTAL_EVENT, ACTIVATE_BEAMS);
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(SAY_KILL, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);
        DespawnAdds();

        if (!m_pInstance)
            return;

        if (m_bAddsReachedFloor) 
             m_pInstance->SetData(TYPE_NOVOS, SPECIAL);
        m_pInstance->SetData(TYPE_NOVOS, DONE);
    }

    void JustSummoned(Creature* pSummoned)
    {
        lSummonGUIDs.push_back(pSummoned->GetGUID());
    }

    void SummonedCreatureJustDied(Creature* pSummoned)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_NOVOS) == IN_PROGRESS)
            if (pSummoned->GetEntry() == NPC_CRYSTAL_HANDLER)
            {
                ++m_uiCrystalsDeactivated;
                if (m_pInstance)
                    m_pInstance->SetData(TYPE_CRYSTAL_EVENT, DEACTIVATE_ONE);
            }
    }

    void UpdateAI(const uint32 uiDiff)
    {

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!m_bAddsReachedFloor)
        {
            if (m_uiCheckoutAdds_Timer < uiDiff)
            {
                for (GUIDList::iterator itr = lSummonGUIDs.begin(); itr != lSummonGUIDs.end(); ++itr)
                    if (Creature* pCreature = m_pInstance->instance->GetCreature(*itr))
                        if (pCreature->GetCreatureInfo()->type ==  CREATURE_TYPE_UNDEAD && pCreature->GetPositionY() > -770.0f)
                        {
                            m_bAddsReachedFloor = true;
                            break;
                        }
                m_uiCheckoutAdds_Timer = 1000;
            }
            else
                m_uiCheckoutAdds_Timer -= uiDiff;
        }

        if (m_uiPhase == PHASE_SUMMONING)
        {
            if (m_uiCrystalsDeactivated >= 4)
            {
                m_creature->InterruptSpell(CURRENT_CHANNELED_SPELL);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                SetCombatMovement(true);
                m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                m_uiPhase = PHASE_ACTIVE_ATTACKING;
            }
            
            if (m_uiSummonRandomAdd_Timer < uiDiff)
            {
                if (m_pInstance)
                    // summon random add
                    if (Creature* pSummonTarget = m_pInstance->instance->GetCreature(m_uiSummonTargetGUID[0]))
                        pSummonTarget->CastSpell(pSummonTarget, m_uiRandomSummoningSpell[urand(0, 8)], true);

                m_uiSummonRandomAdd_Timer = urand(2000, 4000);
            }
            else
                m_uiSummonRandomAdd_Timer -= uiDiff;
                

            if (m_uiSummonCrystalChanneler_Timer < uiDiff)
            {
                if (m_pInstance)
                    // summon random add
                    if (Creature* pSummonTarget = m_pInstance->instance->GetCreature(m_uiSummonTargetGUID[m_bHandlerFromEntrance ? 1 : 2]))
                        pSummonTarget->CastSpell(pSummonTarget, SPELL_SUMMON_CRISTAL_HANDLER, true);

                m_bHandlerFromEntrance = !m_bHandlerFromEntrance;
                DoScriptText(SAY_ADDS, m_creature);
                m_uiSummonCrystalChanneler_Timer = 25000;
            }
            else
                m_uiSummonCrystalChanneler_Timer -= uiDiff;
        }
        else
        {               
            if (m_uiFrostbolt_Timer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_FROSTBOLT : SPELL_FROSTBOLT_H);
                m_uiFrostbolt_Timer = 2000;
            }
            else
                m_uiFrostbolt_Timer -= uiDiff;
            
            if (m_uiMisery_Timer < uiDiff)
            {
                DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_MISERY : SPELL_MISERY_H, CAST_INTERRUPT_PREVIOUS);
                m_uiMisery_Timer = urand(15000, 35000);
            }
            else
                m_uiMisery_Timer -= uiDiff;
            
            if (m_uiArcane_Timer < uiDiff)
            {
                DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_ARCANE : SPELL_ARCANE_H, CAST_INTERRUPT_PREVIOUS);
                m_uiArcane_Timer = urand(5000, 20000);
            }
            else
                m_uiArcane_Timer -= uiDiff;
            
            if (m_uiBlizzard_Timer < uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_BLIZZARD : SPELL_BLIZZARD_H, CAST_INTERRUPT_PREVIOUS);
                m_uiBlizzard_Timer = urand(25000, 35000);
            }
            else
                m_uiBlizzard_Timer -= uiDiff;
            

            if (!m_bIsRegularMode)
            {
                if (m_uiSummonTrollCorpse_Timer < uiDiff)
                {
                    DoCastSpellIfCan(m_creature, SPELL_SUMMON_TROLL_CORPSE, CAST_INTERRUPT_PREVIOUS);
                    m_uiSummonTrollCorpse_Timer = urand(30000, 35000);
                }
                else
                    m_uiSummonTrollCorpse_Timer -= uiDiff;
            }
        }
    }
};

CreatureAI* GetAI_boss_novos(Creature* pCreature)
{
    return new boss_novosAI(pCreature);
}

struct MANGOS_DLL_DECL npc_novos_summon_targetAI : public ScriptedAI
{
    npc_novos_summon_targetAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    void Reset() {}
    void JustSummoned(Creature* pSummoned)
    {
        if (Creature* pNovos = m_pInstance->instance->GetCreature(m_pInstance->GetData64(NPC_NOVOS)))
        {
            pNovos->AI()->JustSummoned(pSummoned);
            if (pNovos->getVictim())
                pSummoned->AI()->AttackStart(pNovos->getVictim());
        }
    }

    void SummonedCreatureJustDied(Creature* pSummoned)
    {
        if (Creature* pNovos = m_pInstance->instance->GetCreature(m_pInstance->GetData64(NPC_NOVOS)))
            pNovos->AI()->SummonedCreatureJustDied(pSummoned);
    }

    void UpdateAI (const uint32 uiDiff) {}
};

CreatureAI* GetAI_npc_novos_summon_target(Creature* pCreature)
{
    return new npc_novos_summon_targetAI(pCreature);
}

void AddSC_boss_novos()
{
    Script *pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_novos";
    pNewScript->GetAI = &GetAI_boss_novos;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_novos_summon_target";
    pNewScript->GetAI = &GetAI_npc_novos_summon_target;
    pNewScript->RegisterSelf();
}