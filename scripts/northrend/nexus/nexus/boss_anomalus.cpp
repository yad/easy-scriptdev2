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
SDName: Boss_Anomalus
SD%Complete: 50%
SDComment: TODO: remove hacks, add support for rift charging
SDCategory: Nexus
EndScriptData */

#include "precompiled.h"
#include "nexus.h"

enum
{
    SAY_AGGRO                       = -1576006,
    SAY_RIFT                        = -1576007,
    SAY_SHIELD                      = -1576008,
    SAY_KILL                        = -1576009,
    SAY_DEATH                       = -1576010,
    EMOTE_OPEN_RIFT                 = -1576021,
    EMOTE_SHIELD                    = -1576022,

    // Anomalus
    SPELL_CREATE_RIFT               = 47743,
    SPELL_CHARGE_RIFT               = 47747,
    SPELL_RIFT_SHIELD               = 47748,
    SPELL_CLOSE_RIFTS               = 47745,

    SPELL_SPARK                     = 47751,
    SPELL_SPARK_H                   = 57062,

    PHASE_BEFORE_RIFT               = 0,
    PHASE_RIFT_OPENED               = 1,
    PHASE_AFTER_RIFT                = 2
};

/*######
## boss_anomalus
######*/

struct MANGOS_DLL_DECL boss_anomalusAI : public ScriptedAI
{
    boss_anomalusAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    uint8   uiPhase;
    uint32 m_uiSparkTimer;
    ObjectGuid m_RiftGuid;

    void Reset()
    {
        uiPhase = PHASE_BEFORE_RIFT;
        m_uiSparkTimer = 5000;
    }

    void JustReachedHome()
    {
        if (Creature* pRift = m_creature->GetMap()->GetCreature(m_RiftGuid))
            pRift->ForcedDespawn();

        m_creature->InterruptSpell(CURRENT_CHANNELED_SPELL, true);
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO, m_creature);
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_ANOMALUS, DONE);
    }

    void KilledUnit(Unit* pVictim)
    {
        if (urand(0, 1))
            DoScriptText(SAY_KILL, m_creature);
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            pSummoned->AI()->AttackStart(pTarget);
        m_RiftGuid = pSummoned->GetObjectGuid();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
             return;

        switch(uiPhase)
        {
            case PHASE_BEFORE_RIFT:
                if (m_creature->GetHealthPercent() < 50.0f)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_CREATE_RIFT, CAST_TRIGGERED) == CAST_OK)
                    {
                        DoScriptText(SAY_SHIELD, m_creature);
                        DoScriptText(EMOTE_SHIELD, m_creature);                    
                        DoScriptText(EMOTE_OPEN_RIFT, m_creature);
                        DoCastSpellIfCan(m_creature, SPELL_RIFT_SHIELD);
                        DoCastSpellIfCan(m_creature, SPELL_CHARGE_RIFT);
                        uiPhase = PHASE_RIFT_OPENED;
                        return;
                    }
                }
                break;

            case PHASE_RIFT_OPENED: 
                if (Creature* pRift = m_creature->GetMap()->GetCreature(m_RiftGuid))
                    if (pRift->HasAura(SPELL_CHARGE_RIFT))
                        return;                
              
                m_creature->RemoveAurasDueToSpell(SPELL_RIFT_SHIELD);
                m_creature->InterruptSpell(CURRENT_CHANNELED_SPELL, true);
                uiPhase = PHASE_AFTER_RIFT;
                return;

            default: break;
        }

        if (m_uiSparkTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_SPARK : SPELL_SPARK_H);

            m_uiSparkTimer = 5000;
        }
        else
            m_uiSparkTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_anomalus(Creature* pCreature)
{
    return new boss_anomalusAI(pCreature);
}

void AddSC_boss_anomalus()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_anomalus";
    pNewScript->GetAI = &GetAI_boss_anomalus;
    pNewScript->RegisterSelf();
}
