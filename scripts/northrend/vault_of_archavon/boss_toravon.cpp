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
SDName: Toravon the Ice Watcher
SDAuthor: Lutik
SD%Complete: 100%
SDComment:
SDCategory: Vault of Archavon
EndScriptData */

#include "precompiled.h"
#include "vault_of_archavon.h"

enum
{
    SPELL_WHITEOUT          = 72034,
    SPELL_WHITEOUT_H        = 72096,
    SPELL_FREEZING_GROUND   = 72090,
    SPELL_FREEZING_GROUND_H = 72104,
    SPELL_FROZEN_MALLET     = 71993,

    FROZEN_ORB_AURA         = 72081,

    CR_FROZEN_ORB           = 38456
};


struct MANGOS_DLL_DECL boss_toravonAI : public ScriptedAI
{
    boss_toravonAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        m_iOrbsNum = m_bIsRegularMode ? 1 : 3;
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    bool m_bIsRegularMode;
    ScriptedInstance *m_pInstance;

    int m_iOrbsNum;
    uint32 m_uiWhiteoutTimer;
    uint32 m_uiOrbsTimer;
    uint32 m_uiFreezeTimer;

    void Reset()
    {
        m_uiWhiteoutTimer = 40000;
        m_uiOrbsTimer = 15000;
        m_uiFreezeTimer = 20000 + rand()%5000;

        if(m_pInstance)
            m_pInstance->SetData(TYPE_TORAVON, NOT_STARTED);
    }

    void Aggro(Unit *who)
    {
        DoCastSpellIfCan(m_creature, SPELL_FROZEN_MALLET);

        if(m_pInstance)
            m_pInstance->SetData(TYPE_TORAVON, IN_PROGRESS);
    }

    void JustDied(Unit *killer)
    {
        if(m_pInstance)
            m_pInstance->SetData(TYPE_TORAVON, DONE);
    }

    void JustSummoned(Creature *pOrb)
    {
        pOrb->CastSpell(pOrb, FROZEN_ORB_AURA, false);
        pOrb->SetInCombatWithZone();
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if(m_uiWhiteoutTimer < diff)
        {
            DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_WHITEOUT : SPELL_WHITEOUT_H);
            m_uiWhiteoutTimer = 40000;
        }
        else
            m_uiWhiteoutTimer -= diff;

        if(m_uiOrbsTimer < diff)
        {
            for(int i=0; i<m_iOrbsNum; ++i)
            {
                if(Unit *pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    m_creature->SummonCreature(CR_FROZEN_ORB, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 3000);
            }
            m_uiOrbsTimer = 40000;
        }
        else
            m_uiOrbsTimer -= diff;

        if(m_uiFreezeTimer < diff)
        {
            DoCastSpellIfCan(m_creature, SPELL_FREEZING_GROUND);
            m_uiFreezeTimer = 20000 + rand()%5000;
        }
        else
            m_uiFreezeTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_toravon(Creature *pCreature)
{
    return new boss_toravonAI (pCreature);
};

void AddSC_boss_toravon()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_toravon";
    newscript->GetAI = &GetAI_boss_toravon;
    newscript->RegisterSelf();
};
