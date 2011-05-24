/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: npc_companion_handler
SDAuthor: ckegg
SD%Complete: 0%
SDComment: 
SDCategory: Pet behavior
INSERT INTO spell_script_target VALUES
(67400,1,34694);

UPDATE creature_template SET ScriptNAme = 'npc_companion_handler' WHERE entry IN (
-- Ammen Vale Lashling, Durotar Scorpion, Elwynn Lamb, Frigid Frostling
33205,33198,33200,40198,
-- Mechanopeep,Zergling,Grunty
33274,11327,34694);
UPDATE creature_template SET ScriptNAme = 'npc_companion_helper' WHERE entry IN (
-- Elwynn Wolf
33286);
DELETE FROM script_texts WHERE entry BETWEEN -1999799 AND -1999796;
INSERT INTO `script_texts` (`entry`, `content_default`, `comment`) VALUES
('-1999799','Feed me, $N',''),
('-1999798','Feed me all night long',''),
('-1999797','Cause if you feed me, $N',''),
('-1999796','I can grow up big and strong','');
EndScriptData */

#include "precompiled.h"
#include "pet_ai.h"

enum
{

    NPC_PEANUT                  = 23286,
    NPC_PEANUT_PERIODIC         = 41019,

    NPC_PANDA                   = 11325,
    SPELL_PANDA_SLEEP           = 19231,
    SPELL_PANDA_ROAR            = 40664,

    NPC_DIABLO                  = 11326,
    SPELL_DIABLO_FLAME          = 18874,
    //SPELL_DIABLO              = 18873,

    NPC_ZERGLING                = 11327,
    SPELL_ZERGLING_PERIODIC     = 19226,
    SPELL_ZERGLING_PASSIVE      = 67399,

    NPC_GRUNTY                  = 34694,
    SPELL_GRUNTY_PERIODIC       = 67370,

    NPC_WILLY                   = 23231,
    SPELL_WILLY_SLEEP           = 40663,
    //SPELL_WILLY               = 40619,
    //SPELL_WILLY               = 40638,

    NPC_DRAGON_KITE             = 25110,
    SPELL_DRAGON_KITE_LIGHTNING = 45197,

    NPC_BATTLE_BOT              = 34587,
    SPELL_BATTLE_BOT_MELEE      = 65694,

    NPC_MURKY                   = 15186,
    NPC_LURKY                   = 15358,
    SPELL_MURKY_DANCE           = 25165,

    NPC_EGBERT                  = 23258,
    SPELL_EGBERT_HAPPYNESS      = 40669,

    //NPC_KIRIN                   = ,
    SPELL_KIRIN_ARCANE          = 61479,

    NPC_MULGORE                 = 33219,
    SPELL_MULGORE_HAPPYNESS     = 62585,

    NPC_PENGU                   = 32595,
    NPC_CHILLY                  = 31128,
    SPELL_PENGU_SOUND           = 61635,

    NPC_STINKER                 = 23274,
    SPELL_STINKER_BROKEN_HEART  = 62004,

    NPC_SCORCHLING              = 25706,
    SPELL_SCORCHLING_BLAST      = 45889,

    NPC_SPRING_RABBIT           = 32791,
    SPELL_SPRING_RABBIT_IN_LOVE = 61728,
    SPELL_SPRING_RABBIT_JUMP    = 61724,

    NPC_STRAND_CRAWLERT         = 33226,
    SPELL_STRAND_CRAWLERT_BUBBLE = 62587,

    NPC_TELDRASSIL_SPROUTLING   = 33188,
    SPELL_SPROUTLING_SLEEP      = 62499,
    SPELL_SPROUTLING_DANCE      = 62504,

    NPC_TIRISFAL_BATLING        = 33197,
    NPC_VAMPIRIC_BATLING        = 28513,
    SPELL_TIRISFAL_BATLING_SOUND = 62545,

    NPC_TYRAEL                  = 29089,
    SPELL_TYRAEL_DANCE          = 54398,

    NPC_TRAIN_WRECKER           = 33404,
    SPELL_TRAIN_WRECKER_BLOW    = 62943,

    NPC_DUNMOROGH_CUB           = 33194,
    NPC_BLIZZARD_BEAR           = 32841,
    SPELL_BLIZZARD_BEAR_SIT     = 61853,

    NPC_MOJO                    = 24480,
    SPELL_FEELING_FROGGY        = 43906,
    SPELL_FROG_LOVE             = 62537,

    EMOTE_KISS                  = -1799979,

    NPC_ORPHANED_MAMMOTH        = 25861,
    SPELL_MAMMOTH_CREDIT        = 46231,

    NPC_AMMEN_VALE_LASHING      = 33205,
    SAY_AMMEN_VALE_LASHING_1    = -1999799,
    SAY_AMMEN_VALE_LASHING_2    = -1999798,
    SAY_AMMEN_VALE_LASHING_3    = -1999797,
    SAY_AMMEN_VALE_LASHING_4    = -1999796,

    NPC_DUROTAR_SCORPION        = 33198,
    SPELL_SCORPION_PERIODIC     = 62679,

    NPC_ELWYNN_LAMB             = 33200,
    NPC_ELWYNN_WOLF             = 33286,
    SPELL_SUMMON_WOLF_PERIODIC  = 62703,
    SPELL_WOLF_ATTACK           = 62701,
    SPELL_PLAY_DEAD             = 19822,
    SPELL_SLEEP_VISUAL          = 34664,

    NPC_FRIGID_FROSTLING        = 40198,
    SPELL_FROSTLING_PERIODIC    = 74957,

    NPC_MECHANOPEEP             = 33274,
    SPELL_MECHANOPEEP_PERIODIC  = 62676
};

struct MANGOS_DLL_DECL npc_companion_handlerAI : public ScriptedPetAI
{
    npc_companion_handlerAI(Creature* pCreature) : ScriptedPetAI(pCreature){Reset();}

    uint32 m_uiActionTimer;
    uint32 m_uiBreakAction;
    uint32 m_uiHelperVar;

    void Reset()
    {
        m_uiHelperVar = 0;
        m_uiActionTimer = 3000;
        m_uiBreakAction = m_uiActionTimer + 7000;
        m_creature->GetCharmInfo()->SetReactState(REACT_PASSIVE);
        m_creature->GetCharmInfo()->SetCommandState(COMMAND_FOLLOW);
    }

    bool m_bOwnerIsIdle()
    {
        Unit* pOwner = m_creature->GetCharmerOrOwner();
        if (pOwner && pOwner->GetTypeId() == TYPEID_PLAYER && !((Player*)pOwner)->isMoving())
            return true;

        return false;
    }

    void AttackStart(Unit* who){}

    void ReceiveEmote(Player* pPlayer, uint32 emote)
    {
        if (!pPlayer)
            return;
        switch (m_creature->GetEntry())
        {
            case NPC_MOJO:
                if (emote == TEXTEMOTE_KISS)
                {
                    m_creature->SetTargetGuid(pPlayer->GetObjectGuid());
                    DoScriptText(EMOTE_KISS,m_creature,pPlayer);
                    pPlayer->CastSpell(pPlayer,SPELL_FEELING_FROGGY,true);
                    DoCastSpellIfCan(m_creature,SPELL_FROG_LOVE,true);
                }
                break;
            default: break;
        }
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (!pWho)
            return;

        switch (m_creature->GetEntry())
        {
            case NPC_ORPHANED_MAMMOTH:
            {
                if (!m_creature->GetOwner())
                    return;
                if ((m_creature->GetOwner())->GetTypeId() != TYPEID_PLAYER)
                    return;
                if (pWho->GetEntry() == 25862 && (m_creature->GetDistance(pWho) < 2*INTERACTION_DISTANCE))
                {
                    m_creature->GetMotionMaster()->MoveIdle();
                    pWho->SetFacingToObject(m_creature);
                    m_creature->SetFacingToObject(pWho);
                    pWho->HandleEmoteCommand(EMOTE_ONESHOT_ROAR);
                    DoCastSpellIfCan(m_creature, SPELL_MAMMOTH_CREDIT);
                    m_creature->ForcedDespawn();
                }
                break;
            }
            default: break;
        }
    }


    void UpdatePetOOCAI(const uint32 uiDiff)
    {
        // Do pet's action
        if (m_uiActionTimer < uiDiff)
        {
            m_uiActionTimer = 10000 + rand()%30000;
            m_uiBreakAction = 7000;
            
            if (!m_bOwnerIsIdle())
                return;
            
            PetAction(m_creature->GetEntry());
        }
        else
            m_uiActionTimer -= uiDiff;

        //
        if (m_uiBreakAction < uiDiff)
        {
            if (m_bOwnerIsIdle())
            {
                m_uiBreakAction = 3000;
                return;
            }

            m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_ONESHOT_NONE);
            m_uiBreakAction = m_uiActionTimer + 7000;
        }
        else
            m_uiBreakAction -= uiDiff;

    }

    void PetAction(uint32 uiPetEntry)
    {
        switch(uiPetEntry)
        {
            case NPC_PEANUT:
            {
                if (!m_creature->HasAura(NPC_PEANUT_PERIODIC))
                    DoCastSpellIfCan(m_creature, NPC_PEANUT_PERIODIC);
                break;
            }
            case NPC_PANDA:
            {
                switch(rand()%2)
                {
                    case 0: DoCastSpellIfCan(m_creature, SPELL_PANDA_SLEEP); break;
                    case 1: DoCastSpellIfCan(m_creature, SPELL_PANDA_ROAR);  break;
                }
                break;
            }
            case NPC_DIABLO:
            {
                DoCastSpellIfCan(m_creature, SPELL_DIABLO_FLAME);
                break;
            }
            case NPC_ZERGLING:
            {
                if (!m_creature->HasAura(67399))
                    DoCastSpellIfCan(m_creature, SPELL_ZERGLING_PASSIVE);

                if (!m_creature->HasAura(SPELL_ZERGLING_PERIODIC))
                    DoCastSpellIfCan(m_creature, SPELL_ZERGLING_PERIODIC);
                break;
            }
            case NPC_GRUNTY:
            {
                if (!m_creature->HasAura(SPELL_GRUNTY_PERIODIC))
                    DoCastSpellIfCan(m_creature, SPELL_GRUNTY_PERIODIC);
                break;
            }
            case NPC_WILLY:
            {
                DoCastSpellIfCan(m_creature, SPELL_WILLY_SLEEP);
                break;
            }
            case NPC_DRAGON_KITE:
            {
                if (Unit* pOwner = m_creature->GetCharmerOrOwner())
                    DoCastSpellIfCan(pOwner, SPELL_DRAGON_KITE_LIGHTNING);
                break;
            }
            case NPC_BATTLE_BOT:
            {
                if (Creature* pBattleBot = GetClosestCreatureWithEntry(m_creature, NPC_BATTLE_BOT, 3.0f))
                {
                    m_creature->GetMotionMaster()->MovePoint(0, pBattleBot->GetPositionX()+rand()%5, pBattleBot->GetPositionY()+rand()%5, pBattleBot->GetPositionZ());
                    DoCastSpellIfCan(m_creature, SPELL_BATTLE_BOT_MELEE);
                    pBattleBot->CastSpell(pBattleBot, SPELL_BATTLE_BOT_MELEE, false);
                    m_uiActionTimer = 3000; // make bot repeats the action
                }
                break;
            }
            case NPC_MURKY:
            case NPC_LURKY:
            {
                m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_DANCE);
                DoCastSpellIfCan(m_creature, SPELL_MURKY_DANCE);
                break;
            }
            case NPC_EGBERT:
            {
                DoCastSpellIfCan(m_creature, SPELL_EGBERT_HAPPYNESS);
                break;
            }
            case NPC_MULGORE:
            {
                DoCastSpellIfCan(m_creature, SPELL_MULGORE_HAPPYNESS);
                break;
            }
            case NPC_PENGU:
            case NPC_CHILLY:
            {
                DoCastSpellIfCan(m_creature, SPELL_PENGU_SOUND);
                m_uiActionTimer = 5000 + rand()%5000; // repeat
                break;
            }
            case NPC_STINKER:
            {
                DoCastSpellIfCan(m_creature, SPELL_STINKER_BROKEN_HEART);
                break;
            }
            case NPC_SCORCHLING:
            {
                DoCastSpellIfCan(m_creature, SPELL_SCORCHLING_BLAST);
                break;
            }
            case NPC_SPRING_RABBIT:
            {
                switch(rand()%2)
                {
                    case 0: DoCastSpellIfCan(m_creature, SPELL_SPRING_RABBIT_IN_LOVE); break;
                    case 1: DoCastSpellIfCan(m_creature, SPELL_SPRING_RABBIT_JUMP);    break;
                }
                break;
            }
            case NPC_STRAND_CRAWLERT:
            {
                DoCastSpellIfCan(m_creature, SPELL_STRAND_CRAWLERT_BUBBLE);
                break;
            }
            case NPC_TELDRASSIL_SPROUTLING:
            {
                switch(rand()%2)
                {
                    case 0: DoCastSpellIfCan(m_creature, SPELL_SPROUTLING_SLEEP); break;
                    case 1: DoCastSpellIfCan(m_creature, SPELL_SPROUTLING_DANCE); break;
                }
                break;
            }
            case NPC_TIRISFAL_BATLING:
            case NPC_VAMPIRIC_BATLING:
            {
                DoCastSpellIfCan(m_creature, SPELL_TIRISFAL_BATLING_SOUND);
                break;
            }
            case NPC_TYRAEL:
            {
                DoCastSpellIfCan(m_creature, SPELL_TYRAEL_DANCE);
                break;
            }
            case NPC_TRAIN_WRECKER:
            {
                DoCastSpellIfCan(m_creature, SPELL_TRAIN_WRECKER_BLOW);
                break;
            }
            case NPC_DUNMOROGH_CUB:
            case NPC_BLIZZARD_BEAR:
            {
                DoCastSpellIfCan(m_creature, SPELL_BLIZZARD_BEAR_SIT);
                break;
            }
            case NPC_AMMEN_VALE_LASHING:
            {
                m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_SUBMERGED);

                if (m_uiHelperVar < 4)
                {
                    DoScriptText(SAY_AMMEN_VALE_LASHING_1 + m_uiHelperVar, m_creature, m_creature->GetCharmerOrOwner());
                    m_uiActionTimer = 3000;
                    ++m_uiHelperVar;
                }
                else
                {
                    m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_NONE);
                    m_uiHelperVar = 0;
                }

                break;
            }
            case NPC_DUROTAR_SCORPION:
            {
                if (!m_creature->HasAura(SPELL_SCORPION_PERIODIC))
                    DoCastSpellIfCan(m_creature, SPELL_SCORPION_PERIODIC);
                break;
            }
            case NPC_ELWYNN_LAMB:
            {
                if (!m_creature->HasAura(SPELL_SUMMON_WOLF_PERIODIC))
                    DoCastSpellIfCan(m_creature, SPELL_SUMMON_WOLF_PERIODIC);
                break;
            }
            case NPC_FRIGID_FROSTLING:
            {
                if (!m_creature->HasAura(SPELL_FROSTLING_PERIODIC))
                    DoCastSpellIfCan(m_creature, SPELL_FROSTLING_PERIODIC);
                break;
            }
            case NPC_MECHANOPEEP:
            {
                if (!m_creature->HasAura(SPELL_MECHANOPEEP_PERIODIC))
                    DoCastSpellIfCan(m_creature, SPELL_MECHANOPEEP_PERIODIC);
                break;
            }
            default: break;
        }
    }
};

CreatureAI* GetAI_npc_companion_handler(Creature* pCreature)
{
    return new npc_companion_handlerAI(pCreature);
}

// special cases (non pets)
// for now only Elwynn Wolf
struct MANGOS_DLL_DECL npc_companion_helperAI : public ScriptedAI
{
    npc_companion_helperAI(Creature* pCreature) : ScriptedAI(pCreature){Reset();}

    void Reset()
    {
        if (Unit* pUnit = m_creature->GetMap()->GetUnit(m_creature->GetCreatorGuid()))
        {
            m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
            m_creature->GetMotionMaster()->MoveFollow(pUnit, 0.0f, m_creature->GetAngle(pUnit));
        }
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (pWho->GetObjectGuid() != m_creature->GetCreatorGuid() || pWho->GetDistance(m_creature) >= ATTACK_DISTANCE/3 ||
            m_creature->HasAura(SPELL_SLEEP_VISUAL) || pWho->HasAura(SPELL_PLAY_DEAD))
            return;
        
        DoCastSpellIfCan(pWho, SPELL_WOLF_ATTACK, CAST_TRIGGERED);
        pWho->CastSpell(pWho, SPELL_PLAY_DEAD, true);
        DoCastSpellIfCan(m_creature, SPELL_SLEEP_VISUAL, CAST_TRIGGERED);
        m_creature->ForcedDespawn(5000);
    }
};

CreatureAI* GetAI_npc_companion_helper(Creature* pCreature)
{
    return new npc_companion_helperAI(pCreature);
}

void AddSC_npc_companion_handler()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "npc_companion_handler";
    newscript->GetAI = &GetAI_npc_companion_handler;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_companion_helper";
    newscript->GetAI = &GetAI_npc_companion_helper;
    newscript->RegisterSelf();
}
