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
SDName: boss_koralon
SD%Complete: 100%
SDComment: 
SDCategory: Vault of Archavon
EndScriptData */

#include "precompiled.h"
#include "vault_of_archavon.h"

#define SPELL_BURNING_FURY         66721 // main spell
#define SPELL_BURNING_FURY_AURA    66895
#define SPELL_BURNING_FURY_AURA2   68168

#define SPELL_BURNING_BREATH       66665
#define SPELL_BURNING_BREATH_H     67328 //DBM
#define SPELL_BB_EFFECT            66670
#define SPELL_BB_EFFECT_H          67329

#define SPELL_METEOR_FISTS         66725 //DBM
#define SPELL_METEOR_FISTS_H       68161       
#define SPELL_METEOR_FISTS_EFF     66765
#define SPELL_METEOR_FISTS_EFF_H   67333

#define SPELL_CINDER               66684
#define SPELL_CINDER_H             67332

struct MANGOS_DLL_DECL boss_koralonAI : public ScriptedAI
{
    boss_koralonAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegular = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegular;
    bool m_bIsBB;
    uint32 m_uiBurningBreathTimer;
    uint32 m_uiMeteorFistsTimer;
    uint32 m_uiFlamesTimer;
    uint32 m_uiBurningFuryTimer;

    uint32 m_uiBBTickTimer;
    uint32 m_uiBBTicks;

    void Reset()
    {
        m_uiBurningBreathTimer = 25000;
        m_uiBurningFuryTimer = 20000;
        m_uiMeteorFistsTimer = 47000;
        m_uiFlamesTimer = 15000;

        m_bIsBB = false;

        if (m_pInstance)
            m_pInstance->SetData(TYPE_KORALON, NOT_STARTED);
    }

    void Aggro(Unit *who)
    {
        DoCastSpellIfCan(m_creature, SPELL_BURNING_FURY);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_KORALON, IN_PROGRESS);
    };

    void JustDied(Unit *killer)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_KORALON, DONE);
    };

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(m_uiBurningFuryTimer < diff)
        {
            DoCastSpellIfCan(m_creature, SPELL_BURNING_FURY);
            m_uiBurningFuryTimer = 20000;
        }
        else m_uiBurningFuryTimer -= diff;

        if(m_uiBurningBreathTimer < diff)
        {
            DoCastSpellIfCan(m_creature, m_bIsRegular ? SPELL_BURNING_BREATH : SPELL_BURNING_BREATH_H);
            m_uiBurningBreathTimer = 45000;

            m_bIsBB = true;
            m_uiBBTickTimer = 1000;
            m_uiBBTicks = 0;
        }
        else m_uiBurningBreathTimer -= diff;

        if(m_uiFlamesTimer < diff)
        {
            int8 iFlames = m_bIsRegular ? 3 : 5;

            for(int8 i=0; i< iFlames; ++i)
            {
                if (Unit *target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    DoCastSpellIfCan(target, m_bIsRegular ? SPELL_CINDER : SPELL_CINDER_H);
            }
            m_uiFlamesTimer = 20000;
        }
        else m_uiFlamesTimer -= diff;

        if(m_uiMeteorFistsTimer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_METEOR_FISTS_EFF);
            m_uiMeteorFistsTimer = 45000;
        }
        else m_uiMeteorFistsTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_koralonAI(Creature* pCreature)
{
    return new boss_koralonAI(pCreature);
}

void AddSC_boss_koralon()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_koralon";
    newscript->GetAI = &GetAI_boss_koralonAI;
    newscript->RegisterSelf();
}
