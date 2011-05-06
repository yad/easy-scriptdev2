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
SDName: Dragonblight
SD%Complete: 100
SDComment: Quest support: 12166, 12261, 12499/12500(end sequenze). Taxi paths Wyrmrest temple.
SDCategory: Dragonblight
EndScriptData */

/* ContentData
npc_afrasastrasz
npc_alexstrasza_wr_gate
npc_destructive_ward
npc_tariolstrasz
npc_torastrasza
npc_taunkale_refugee
EndContentData */

#include "precompiled.h"

/*######
## npc_afrasastrasz
######*/

enum
{
    TAXI_PATH_ID_MIDDLE_DOWN            = 882,
    TAXI_PATH_ID_MIDDLE_TOP             = 881
};

#define GOSSIP_ITEM_TAXI_MIDDLE_DOWN    "I would like to take a flight to the ground, Lord Of Afrasastrasz."
#define GOSSIP_ITEM_TAXI_MIDDLE_TOP     "My Lord, I must go to the upper floor of the temple."

bool GossipHello_npc_afrasastrasz(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_TAXI_MIDDLE_DOWN, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_TAXI_MIDDLE_TOP, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_afrasastrasz(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pPlayer->ActivateTaxiPathTo(TAXI_PATH_ID_MIDDLE_DOWN);
    }
    if (uiAction == GOSSIP_ACTION_INFO_DEF+2)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pPlayer->ActivateTaxiPathTo(TAXI_PATH_ID_MIDDLE_TOP);
    }
    return true;
}

/*######
## npc_alexstrasza_wr_gate
######*/

enum
{
    QUEST_RETURN_TO_AG_A    = 12499,
    QUEST_RETURN_TO_AG_H    = 12500,
    MOVIE_ID_GATES          = 14
};

#define GOSSIP_ITEM_WHAT_HAPPENED   "Alexstrasza, can you show me what happened here?"

bool GossipHello_npc_alexstrasza_wr_gate(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    if (pPlayer->GetQuestRewardStatus(QUEST_RETURN_TO_AG_A) || pPlayer->GetQuestRewardStatus(QUEST_RETURN_TO_AG_H))
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_WHAT_HAPPENED, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_alexstrasza_wr_gate(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pPlayer->SendMovieStart(MOVIE_ID_GATES);
    }

    return true;
}

/*#####
# npc_destructive_ward
#####*/

enum
{
    SAY_WARD_POWERUP                    = -1000664,
    SAY_WARD_CHARGED                    = -1000665,

    SPELL_DESTRUCTIVE_PULSE             = 48733,
    SPELL_DESTRUCTIVE_BARRAGE           = 48734,
    SPELL_DESTRUCTIVE_WARD_POWERUP      = 48735,

    SPELL_SUMMON_SMOLDERING_SKELETON    = 48715,
    SPELL_SUMMON_SMOLDERING_CONSTRUCT   = 48718,
    SPELL_DESTRUCTIVE_WARD_KILL_CREDIT  = 52409,

    MAX_STACK                           = 1,
};

// Script is based on real event from you-know-where.
// Some sources show the event in a bit different way, for unknown reason.
// Devs decided to add it in the below way, until more details can be obtained.

// It will be only two power-up's, where other sources has a different count (2-4 stacks has been observed)
// Probably caused by either a change in a patch (bugfix?) or the powerup has a condition (some
// sources suggest this, but without any explanation about what this might be)

struct MANGOS_DLL_DECL npc_destructive_wardAI : public Scripted_NoMovementAI
{
    npc_destructive_wardAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
    {
        m_uiPowerTimer = 30000;
        m_uiStack = 0;
        m_uiSummonTimer = 2000;
        m_bCanPulse = false;
        m_bFirst = true;
        Reset();
    }

    uint32 m_uiPowerTimer;
    uint32 m_uiStack;
    uint32 m_uiSummonTimer;
    bool m_bFirst;
    bool m_bCanPulse;

    void Reset() { }

    void JustSummoned(Creature* pSummoned)
    {
        pSummoned->AI()->AttackStart(m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_bCanPulse)
        {
            if (DoCastSpellIfCan(m_creature, m_uiStack > MAX_STACK ? SPELL_DESTRUCTIVE_BARRAGE : SPELL_DESTRUCTIVE_PULSE) == CAST_OK)
                m_bCanPulse = false;
        }

        if (m_uiSummonTimer)
        {
            if (m_uiSummonTimer <= uiDiff)
            {
                if (m_bFirst)
                    m_uiSummonTimer = 25000;
                else
                    m_uiSummonTimer = 0;

                switch(m_uiStack)
                {
                    case 0:
                        DoCastSpellIfCan(m_creature, SPELL_SUMMON_SMOLDERING_SKELETON, CAST_TRIGGERED);
                        break;
                    case 1:
                        DoCastSpellIfCan(m_creature, SPELL_SUMMON_SMOLDERING_CONSTRUCT, CAST_TRIGGERED);

                        if (m_bFirst)
                            break;

                        DoCastSpellIfCan(m_creature, SPELL_SUMMON_SMOLDERING_CONSTRUCT, CAST_TRIGGERED);
                        break;
                    case 2:
                        if (m_bFirst)
                            break;

                        DoCastSpellIfCan(m_creature, SPELL_SUMMON_SMOLDERING_SKELETON, CAST_TRIGGERED);
                        DoCastSpellIfCan(m_creature, SPELL_SUMMON_SMOLDERING_SKELETON, CAST_TRIGGERED);
                        DoCastSpellIfCan(m_creature, SPELL_SUMMON_SMOLDERING_CONSTRUCT, CAST_TRIGGERED);
                        break;
                }

                m_bFirst = !m_bFirst;
            }
            else
                m_uiSummonTimer -= uiDiff;
        }

        if (!m_uiPowerTimer)
            return;

        if (m_uiPowerTimer <= uiDiff)
        {
            if (m_uiStack > MAX_STACK)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_DESTRUCTIVE_WARD_KILL_CREDIT) == CAST_OK)
                {
                    DoScriptText(SAY_WARD_CHARGED, m_creature, m_creature->GetOwner());
                    m_uiPowerTimer = 0;
                    m_uiSummonTimer = 0;
                    m_bCanPulse = true;
                }
            }
            else if (DoCastSpellIfCan(m_creature, SPELL_DESTRUCTIVE_WARD_POWERUP) == CAST_OK)
            {
                DoScriptText(SAY_WARD_POWERUP, m_creature, m_creature->GetOwner());

                m_uiPowerTimer = 30000;
                m_uiSummonTimer = 2000;

                m_bFirst = true;
                m_bCanPulse = true;                         // pulse right after each charge

                ++m_uiStack;
            }
        }
        else
            m_uiPowerTimer -= uiDiff;
    }
};

CreatureAI* GetAI_npc_destructive_ward(Creature* pCreature)
{
    return new npc_destructive_wardAI(pCreature);
}

/*######
## npc_tariolstrasz
######*/

enum
{
    QUEST_INFORM_QUEEN_A                = 12123,            //need to check if quests are required before gossip available
    QUEST_INFORM_QUEEN_H                = 12124,
    TAXI_PATH_ID_BOTTOM_TOP             = 878,
    TAXI_PATH_ID_BOTTOM_MIDDLE          = 883
};

#define GOSSIP_ITEM_TAXI_BOTTOM_TOP     "My Lord, I must go to the upper floor of the temple."
#define GOSSIP_ITEM_TAXI_BOTTOM_MIDDLE  "Can you spare a drake to travel to Lord Of Afrasastrasz, in the middle of the temple?"

bool GossipHello_npc_tariolstrasz(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_TAXI_BOTTOM_TOP, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_TAXI_BOTTOM_MIDDLE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_tariolstrasz(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pPlayer->ActivateTaxiPathTo(TAXI_PATH_ID_BOTTOM_TOP);
    }
    if (uiAction == GOSSIP_ACTION_INFO_DEF+2)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pPlayer->ActivateTaxiPathTo(TAXI_PATH_ID_BOTTOM_MIDDLE);
    }
    return true;
}

/*######
## npc_torastrasza
######*/

enum
{
    TAXI_PATH_ID_TOP_MIDDLE             = 880,
    TAXI_PATH_ID_TOP_BOTTOM             = 879
};

#define GOSSIP_ITEM_TAXI_TOP_MIDDLE     "I would like to see Lord Of Afrasastrasz, in the middle of the temple."
#define GOSSIP_ITEM_TAXI_TOP_BOTTOM     "Yes, Please. I would like to return to the ground floor of the temple."

bool GossipHello_npc_torastrasza(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_TAXI_TOP_MIDDLE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_TAXI_TOP_BOTTOM, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_torastrasza(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pPlayer->ActivateTaxiPathTo(TAXI_PATH_ID_TOP_MIDDLE);
    }
    if (uiAction == GOSSIP_ACTION_INFO_DEF+2)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pPlayer->ActivateTaxiPathTo(TAXI_PATH_ID_TOP_BOTTOM);
    }
    return true;
}

/*######
## npc_taunkale_refuge
######*/

enum
{
    QUEST_BLOOD_OATH_OF_THE_HORDE   = 11983,
    GO_RECOVERED_HORDE_ARMAMENTS    = 188252,   //crate
    POINT_NEAR_CRATE                = 1,
    POINT_REFUGEE_CAMP_EXIT         = 2,
    SPELL_TAUNKA_TRANSFORM_MALE     = 47022,
    SPELL_TAUNKA_TRANSFORM_FEMALE   = 47023,
    SAY_REFUGE                      = -1999926,

    NPC_TEXT_1                      = 12611,
    NPC_TEXT_2                      = 12618,
    NPC_TEXT_3                      = 12619,
    NPC_TEXT_4                      = 12620
};

#define GOSSIP_ITEM_TAUNKA_REFUGEE_1    "Worry no more, taunka. The Horde will save and protect you and your people, but first you must swear allegiance to the Warchief by taking the blood oath of the Horde."
#define GOSSIP_ITEM_TAUNKA_REFUGEE_2    "Yes, taunka. Retribution is a given right of all members of the Horde."
#define GOSSIP_ITEM_TAUNKA_REFUGEE_3    "Than repeat after me: \"Lok'tar ogar! Victory or death - it is these words that bind me to the Horde. For they are the most sacred and fundamental of truths to warrior of the Horde.\n\nI give my flesh and blood freely to the Warchief. I am the instrument of my Warchief\'s desire. I am a weapon of my Warchief\'s command.\n\nFrom this moment until the end of days I live and die - For the Horde!\""
#define GOSSIP_ITEM_TAUNKA_REFUGEE_4    "For the horde!\n\nArm yourself  from the crates that surround us and report to Agmar\'s Hammer, east of here. Your first trial as a member of the Horde is survive the journey.\n\nLok\'tar ogar!"

float fWestwindRefugeGatePos[2][3] = 
{
    {3698.48f, 2828.36f, 88.08f},
    {3690.35f, 2881.30f, 92.32f}
};

bool GossipHello_npc_taunkale_refuge(Player* pPlayer, Creature* pCreature)
{
    if (pPlayer->GetQuestStatus(QUEST_BLOOD_OATH_OF_THE_HORDE) == QUEST_STATUS_INCOMPLETE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_TAUNKA_REFUGEE_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

    pPlayer->SEND_GOSSIP_MENU(NPC_TEXT_1, pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_taunkale_refuge(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    switch(uiAction)
    {
        case GOSSIP_ACTION_INFO_DEF:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_TAUNKA_REFUGEE_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
            pPlayer->SEND_GOSSIP_MENU(NPC_TEXT_2, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_TAUNKA_REFUGEE_3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
            pPlayer->SEND_GOSSIP_MENU(NPC_TEXT_3, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_TAUNKA_REFUGEE_4, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);
            pPlayer->SEND_GOSSIP_MENU(NPC_TEXT_4, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+3:
            pPlayer->CLOSE_GOSSIP_MENU();
            pCreature->SetStandState(UNIT_STAND_STATE_STAND);
            pPlayer->KilledMonster(pCreature->GetCreatureInfo(), pCreature->GetGUID()); // should be done like this instead KilledMonsterCredit()
            if (GameObject* pCrate = GetClosestGameObjectWithEntry(pCreature, GO_RECOVERED_HORDE_ARMAMENTS, DEFAULT_VISIBILITY_DISTANCE))
            {
                float fDestX, fDestY, fDestZ;
                pCrate->GetNearPoint(pCrate, fDestX, fDestY, fDestZ, pCrate->GetObjectBoundingRadius(), 1.0f, pCrate->GetAngle(pCreature));
                pCreature->GetMotionMaster()->Clear();
                pCreature->SetSpeedRate(MOVE_RUN, 1.0f, true);
                pCreature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                pCreature->GetMotionMaster()->MovePoint(POINT_NEAR_CRATE, fDestX, fDestY, fDestZ);
            }
            // cant talk now
            pCreature->SetUInt32Value(UNIT_NPC_FLAGS, 0);
            break;
    }
    return true;
}

struct MANGOS_DLL_DECL npc_taunkale_refugeAI : public ScriptedAI
{
    npc_taunkale_refugeAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    bool m_bArming;
    uint8 m_uiSubevent;
    uint32 m_uiArming_Timer;

    void Reset()
    {
        m_bArming = false;
        m_uiArming_Timer = 1000;
        m_uiSubevent = 0;
        m_creature->AddSplineFlag(SPLINEFLAG_WALKMODE);
    }

    void MovementInform(uint32 uiType, uint32 uiPointId)
    {
        if (uiType != POINT_MOTION_TYPE)
            return;

        switch (uiPointId)
        {
            case 1: m_bArming = true; break;
            case 2: m_creature->ForcedDespawn(); break;
            default: break;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_bArming)
        {
            if (m_uiArming_Timer < uiDiff)
            {
                switch (m_uiSubevent)
                {
                    case 0:
                        m_creature->HandleEmote(EMOTE_STATE_USESTANDING);
                        break;
                    case 1:
                        m_creature->HandleEmote(EMOTE_STATE_NONE);
                        DoCastSpellIfCan(m_creature, m_creature->getGender() == GENDER_MALE ? SPELL_TAUNKA_TRANSFORM_MALE : SPELL_TAUNKA_TRANSFORM_FEMALE);
                        DoScriptText(SAY_REFUGE, m_creature);
                        break;
                    case 2:
                        m_creature->GetMotionMaster()->Clear();
                        if (m_creature->GetDistance(fWestwindRefugeGatePos[0][0], fWestwindRefugeGatePos[0][1], fWestwindRefugeGatePos[0][2]) > m_creature->GetDistance(fWestwindRefugeGatePos[1][0], fWestwindRefugeGatePos[1][1], fWestwindRefugeGatePos[1][2]))
                            m_creature->GetMotionMaster()->MovePoint(POINT_REFUGEE_CAMP_EXIT, fWestwindRefugeGatePos[1][0], fWestwindRefugeGatePos[1][1], fWestwindRefugeGatePos[1][2], true);
                        else
                            m_creature->GetMotionMaster()->MovePoint(POINT_REFUGEE_CAMP_EXIT, fWestwindRefugeGatePos[0][0], fWestwindRefugeGatePos[0][1], fWestwindRefugeGatePos[0][2], true);
                        m_bArming = false;
                        return;
                }
                ++m_uiSubevent;
                m_uiArming_Timer = 3000;
            }
            else
                m_uiArming_Timer -= uiDiff;
        }
    }
};

CreatureAI* GetAI_npc_taunkale_refuge(Creature* pCreature)
{
    return new npc_taunkale_refugeAI(pCreature);
}

/*######
## npc_hourglass_of_eternity
######*/
/*Support for 'Future you' is currently missing*/ 
enum
{
    NPC_INFINITE_CHRONO_MAGUS    = 27898,
    NPC_INFINITE_ASSAILANT       = 27896,
    NPC_INFINITE_DESTROYER       = 27897,
    NPC_INFINITE_TIMERENDER      = 27900,
    QUEST_MYSTERY_OF_INFINITE    = 12470

};

struct MANGOS_DLL_DECL npc_hourglassAI : public ScriptedAI
{
    npc_hourglassAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }
    
    uint32 uiWaveTimer;
    uint32 uiWaveCounter;

     void Reset()
     {
         uiWaveTimer = 5000;
         uiWaveCounter = 0;
     }

     void JustSummoned(Creature* pSummoned)
     {
         pSummoned->AI()->AttackStart(m_creature);
     }  

     void JustDied(Unit* pKiller)
     {
        if(Player *pPlayer = m_creature->GetMap()->GetPlayer(m_creature->GetOwnerGuid()))
        {
            pPlayer->FailQuest(QUEST_MYSTERY_OF_INFINITE);
        }
     }

     void SummonWave()
     {
         switch(uiWaveCounter)
         {
            case 0: m_creature->SummonCreature(NPC_INFINITE_CHRONO_MAGUS, m_creature->GetPositionX()+5,m_creature->GetPositionY(),m_creature->GetPositionZ(), 0, TEMPSUMMON_CORPSE_DESPAWN, 0);
                    m_creature->SummonCreature(NPC_INFINITE_ASSAILANT, m_creature->GetPositionX()-5,m_creature->GetPositionY(),m_creature->GetPositionZ(), 0, TEMPSUMMON_CORPSE_DESPAWN, 0);
                    break;
            case 1: m_creature->SummonCreature(NPC_INFINITE_CHRONO_MAGUS, m_creature->GetPositionX()+5,m_creature->GetPositionY(),m_creature->GetPositionZ(), 0, TEMPSUMMON_CORPSE_DESPAWN, 0);
                    m_creature->SummonCreature(NPC_INFINITE_CHRONO_MAGUS, m_creature->GetPositionX()-5,m_creature->GetPositionY(),m_creature->GetPositionZ(), 0, TEMPSUMMON_CORPSE_DESPAWN, 0);
                    break;
            case 2: m_creature->SummonCreature(NPC_INFINITE_CHRONO_MAGUS, m_creature->GetPositionX()+5,m_creature->GetPositionY(),m_creature->GetPositionZ(), 0, TEMPSUMMON_CORPSE_DESPAWN, 0);
                    m_creature->SummonCreature(NPC_INFINITE_ASSAILANT, m_creature->GetPositionX()-5,m_creature->GetPositionY(),m_creature->GetPositionZ(), 0, TEMPSUMMON_CORPSE_DESPAWN, 0);
                    m_creature->SummonCreature(NPC_INFINITE_DESTROYER, m_creature->GetPositionX()+5,m_creature->GetPositionY()+5 ,m_creature->GetPositionZ(), 0, TEMPSUMMON_CORPSE_DESPAWN, 0);
                    break;
            case 3: m_creature->SummonCreature(NPC_INFINITE_CHRONO_MAGUS, m_creature->GetPositionX()+5,m_creature->GetPositionY(),m_creature->GetPositionZ(), 0, TEMPSUMMON_CORPSE_DESPAWN, 0);
                    m_creature->SummonCreature(NPC_INFINITE_ASSAILANT, m_creature->GetPositionX()-5,m_creature->GetPositionY(),m_creature->GetPositionZ(), 0, TEMPSUMMON_CORPSE_DESPAWN, 0);
                    m_creature->SummonCreature(NPC_INFINITE_DESTROYER, m_creature->GetPositionX()+5,m_creature->GetPositionY()+5 ,m_creature->GetPositionZ(), 0, TEMPSUMMON_CORPSE_DESPAWN, 0);
                    break;
            case 4: m_creature->SummonCreature(NPC_INFINITE_TIMERENDER, m_creature->GetPositionX()+5,m_creature->GetPositionY(),m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 300000);
                    break;

         }
     }
     void UpdateAI(const uint32 uiDiff)
     {
            if (uiWaveTimer <= uiDiff)
            {
                if(uiWaveCounter<=4)
                {
                    SummonWave();
                    uiWaveTimer = 15000;
                    uiWaveCounter++;
                }
                else m_creature->ForcedDespawn();

            } else uiWaveTimer -= uiDiff;
     }
};

CreatureAI* GetAI_npc_hourglass(Creature* pCreature)
{
    return new npc_hourglassAI(pCreature);
}

/*######
## npc_area_limiter
######*/
enum 
{
    NPC_WYRMREST_DEFENDER      = 27629,
    NPC_WINTERGARDE_GRYPHOON   = 27258
};

struct MANGOS_DLL_DECL npc_area_limiterAI : public ScriptedAI
{
    npc_area_limiterAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    uint32 uiCheckTimer;
    void Reset() 
    {
        uiCheckTimer = 15000;
    }

    void UpdateAI(const uint32 uiDiff)
     {
            if (uiCheckTimer <= uiDiff)
            {
                switch(m_creature->GetEntry())
                {
                    case NPC_WYRMREST_DEFENDER: if(m_creature->GetAreaId() != 4254 && m_creature->GetAreaId() != 4183 && m_creature->GetAreaId() != 4161)
                                                 {
                                                     m_creature->ForcedDespawn();
                                                 }
                                                 break;
                    case NPC_WINTERGARDE_GRYPHOON: if(m_creature->GetAreaId() != 4177 && m_creature->GetAreaId() != 4188)
                                                    {
                                                        m_creature->ForcedDespawn();
                                                    }
                                                    break;
                }
                uiCheckTimer = 10000;

            } else uiCheckTimer -= uiDiff;
     }

    
};
CreatureAI* GetAI_npc_area_limiter(Creature* pCreature)
{
    return new npc_area_limiterAI(pCreature);
}

/*######
## npc_ravaged_giant
######*/
enum
{
    QUEST_SLIM_PICKINGS               = 12075,
    ITEM_SAMPLE_OF_ROCKFLESH          = 36765
};

bool GossipHello_npc_ravaged_giant(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    if(pPlayer->GetQuestStatus(QUEST_SLIM_PICKINGS ) == QUEST_STATUS_INCOMPLETE)
    {
            ItemPosCountVec dest;
            uint8 msg = pPlayer->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, ITEM_SAMPLE_OF_ROCKFLESH, 1, false);
            if (msg == EQUIP_ERR_OK)
                pPlayer->StoreNewItem(dest, ITEM_SAMPLE_OF_ROCKFLESH, 1, true);
    }
    return true;
}

/*######
## npc_vengeful_geist
######*/

enum
{
    NPC_TRAPPED_VILLAGER             = 27359,
    QUEST_RESCUE_FROM_TOWN_SQUARE    = 12253,
    SPELL_DESPAWN_SELF               = 43014

};

struct MANGOS_DLL_DECL npc_vengeful_geistAI : public ScriptedAI
{
    npc_vengeful_geistAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }
    
    void Reset(){}
    void JustDied(Unit* pKiller)
    {
        if(Creature* pVillager = GetClosestCreatureWithEntry(m_creature, NPC_TRAPPED_VILLAGER, 15.0f))
        { 
            if(Player *pPlayer = m_creature->GetMap()->GetPlayer(pKiller->GetGUID()))
            {
                if(pPlayer->GetQuestStatus(QUEST_RESCUE_FROM_TOWN_SQUARE) == QUEST_STATUS_INCOMPLETE)
                {
                    pPlayer->KilledMonsterCredit(NPC_TRAPPED_VILLAGER);
                    pVillager->CastSpell(pVillager, SPELL_DESPAWN_SELF, false);
                }
            }
        }
    }

};

CreatureAI* GetAI_npc_vengeful_geist(Creature* pCreature)
{
    return new npc_vengeful_geistAI(pCreature);
}
/*######
## npc_inquisitor_hallard
######*/

enum
{
    NPC_MAYOR_GODFREY                = 27577,
    QUEST_RIGHTEOUS_SERMON           = 12321,
    SPELL_INQUISITOR_PENANCE         = 49062,

    HALLARD_SAY_1                    = -1789900,
    HALLARD_SAY_2                    = -1789901,
    GODFREY_EMOTE_1                  = -1789902,
    HALLARD_SAY_3                    = -1789903,
    HALLARD_SAY_4                    = -1789904,
    GODFREY_EMOTE_2                  = -1789905,
    GODFREY_SAY_1                    = -1789906,
    GODFREY_SAY_2                    = -1789907,
    GODFREY_SAY_3                    = -1789908,
    HALLARD_SAY_5                    = -1789909,
    HALLARD_SAY_6                    = -1789927,
    HALLARD_SAY_7                    = -1789910,
    HALLARD_SAY_8                    = -1789911,
    HALLARD_SAY_9                    = -1789912,
    HALLARD_SAY_10                   = -1789913,
    GODFREY_SAY_4                    = -1789914,
    GODFREY_SAY_5                    = -1789915,
    HALLARD_SAY_11                   = -1789916,
    GODFREY_SAY_6                    = -1789917,
    HALLARD_SAY_12                   = -1789918,
    GODFREY_SAY_7                    = -1789919,
    GODFREY_SAY_8                    = -1789920,
    HALLARD_SAY_13                   = -1789921,
    GODFREY_SAY_9                    = -1789922,
    GODFREY_SAY_10                   = -1789923,
    HALLARD_SAY_14                   = -1789924,
    HALLARD_SAY_15                   = -1789925,
    HALLARD_SAY_16                   = -1789926

};

struct MANGOS_DLL_DECL npc_inquisitor_hallardAI : public ScriptedAI
{
    npc_inquisitor_hallardAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_uiMayorGodfreyGUID = 0;
        Reset();
    }

    uint64 m_uiPlayerGUID;
    uint64 m_uiMayorGodfreyGUID;
    uint32 m_uiEventTimer;
    bool m_bEventStarted;
    uint32 m_uiPhase;


    void Reset()
    {
        m_uiPlayerGUID = 0;
        m_bEventStarted = false;
        m_uiPhase = 0;
        m_uiEventTimer = 2000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if(m_bEventStarted)
        {
            if (m_uiEventTimer < uiDiff)
            {
                Creature* pGodfrey = m_creature->GetMap()->GetCreature(m_uiMayorGodfreyGUID);
                if (!pGodfrey)
                    EnterEvadeMode();

                switch(m_uiPhase)
                {
                    case 0:
                        m_creature->SetFacingToObject(pGodfrey);
                        if(Player *pPlayer = m_creature->GetMap()->GetPlayer(m_uiPlayerGUID))
                            DoScriptText(HALLARD_SAY_1, m_creature, pPlayer);
                        m_uiEventTimer = 4000;
                        break;
                    case 1:
                        DoScriptText(HALLARD_SAY_2, m_creature);
                        m_uiEventTimer = 2000;
                        break;
                    case 2:
                        DoScriptText(GODFREY_EMOTE_1, pGodfrey);
                        m_uiEventTimer = 2000;
                        break;
                    case 3:
                        DoScriptText(HALLARD_SAY_3, m_creature);
                        m_uiEventTimer = 4000;
                        break;
                    case 4:
                        DoScriptText(HALLARD_SAY_4, m_creature);
                        m_uiEventTimer = 2000;
                        break;
                    case 5:
                        DoScriptText(GODFREY_EMOTE_2, pGodfrey);
                        m_uiEventTimer = 3000;
                        break;
                    case 6:
                        DoScriptText(GODFREY_SAY_1, pGodfrey);
                        m_uiEventTimer = 5000;
                        break;
                    case 7:
                        DoScriptText(GODFREY_SAY_2, pGodfrey);
                        m_uiEventTimer = 5000;
                        break;
                    case 8:
                        DoScriptText(GODFREY_SAY_3, pGodfrey);
                        m_uiEventTimer = 5000;
                        break;
                    case 9:
                        DoScriptText(HALLARD_SAY_5, m_creature);
                        m_uiEventTimer = 7000;
                        break; 
                    case 10:
                        DoScriptText(HALLARD_SAY_6, m_creature);
                        m_uiEventTimer = 8000;
                        break;
                    case 11:
                        DoScriptText(HALLARD_SAY_7, m_creature);
                        m_uiEventTimer = 9000;
                        break;
                    case 12:
                        DoScriptText(HALLARD_SAY_8, m_creature);
                        m_uiEventTimer = 9000;
                        break;
                    case 13:
                        DoCastSpellIfCan(pGodfrey, SPELL_INQUISITOR_PENANCE);
                        DoScriptText(HALLARD_SAY_9, m_creature);
                        m_uiEventTimer = 4000;
                        break;
                    case 14:
                        DoScriptText(HALLARD_SAY_10,m_creature);
                        m_uiEventTimer = 4000;
                        break;
                    case 15:
                        DoCastSpellIfCan(pGodfrey, SPELL_INQUISITOR_PENANCE);
                        DoScriptText(HALLARD_SAY_9, m_creature);
                        m_uiEventTimer = 4000;
                        break;
                    case 16:
                        DoScriptText(GODFREY_SAY_4, pGodfrey);
                        m_uiEventTimer = 2000;
                        break;
                    case 17:
                        DoScriptText(GODFREY_SAY_5, pGodfrey);
                        m_uiEventTimer = 5000;
                        break;
                    case 18:
                        DoCastSpellIfCan(pGodfrey, SPELL_INQUISITOR_PENANCE);
                        DoScriptText(HALLARD_SAY_9, m_creature);
                        m_uiEventTimer = 4000;
                        break;
                    case 19:
                        DoScriptText(GODFREY_SAY_4, pGodfrey);
                        m_uiEventTimer = 4000;
                        break;
                    case 20:
                        DoScriptText(HALLARD_SAY_11, m_creature);
                        m_uiEventTimer = 7000;
                        break;
                    case 21:
                        DoScriptText(GODFREY_SAY_6, pGodfrey);
                        m_uiEventTimer = 7000;
                        break;
                    case 22:
                        DoScriptText(HALLARD_SAY_12, m_creature);
                        m_uiEventTimer = 10000;
                        break;
                    case 23:
                        DoScriptText(GODFREY_SAY_7, pGodfrey);
                        m_uiEventTimer = 8000;
                        break;
                    case 24:
                        DoScriptText(GODFREY_SAY_8, pGodfrey);
                        m_uiEventTimer = 8000;
                        break;
                    case 25:
                        DoScriptText(HALLARD_SAY_13, m_creature);
                        m_uiEventTimer = 10000;
                        break;
                    case 26:
                        DoScriptText(GODFREY_SAY_9, pGodfrey);
                        m_uiEventTimer = 7000;
                        break;
                    case 27:
                        DoScriptText(GODFREY_SAY_10, pGodfrey);
                        m_uiEventTimer = 9000;
                        break;
                    case 28:
                        DoScriptText(HALLARD_SAY_14, m_creature);
                        m_uiEventTimer = 10000;
                        break;
                    case 29:
                        if(Player *pPlayer = m_creature->GetMap()->GetPlayer(m_uiPlayerGUID))
                        {
                            m_creature->SetFacingToObject(pPlayer);
                            DoScriptText(HALLARD_SAY_15, m_creature, pPlayer);
                        }
                        m_uiEventTimer = 12000;
                        break;
                    case 30:
                        DoScriptText(HALLARD_SAY_16, m_creature); 
                        if(Player *pPlayer = m_creature->GetMap()->GetPlayer(m_uiPlayerGUID))
                            pPlayer->AreaExploredOrEventHappens(QUEST_RIGHTEOUS_SERMON);
                        m_creature->ForcedDespawn();
                        break;
                }
                m_uiPhase++;
            }
            else
                m_uiEventTimer -= uiDiff;
        }
    }
};

CreatureAI* GetAI_npc_inquisitor_hallard(Creature* pCreature)
{
    return new npc_inquisitor_hallardAI(pCreature);
}

bool QuestAccept_npc_inquisitor_hallard(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    switch(pQuest->GetQuestId())
    {
        case QUEST_RIGHTEOUS_SERMON:
        {
            if (npc_inquisitor_hallardAI* pHallardAI = dynamic_cast<npc_inquisitor_hallardAI*>(pCreature->AI()))
            {
                pCreature->GetMotionMaster()->MovePoint(0, 3801.38f , -679.13f , 213.73f );
                pHallardAI->m_bEventStarted = true;
                pHallardAI->m_uiPlayerGUID = (pPlayer->GetGUID());
                if (Creature *pGodfrey = GetClosestCreatureWithEntry(pCreature, NPC_MAYOR_GODFREY, DEFAULT_VISIBILITY_DISTANCE))
                    pHallardAI->m_uiMayorGodfreyGUID = pGodfrey->GetGUID();
            }
        }
        break;
    }
    return true;
}

/*######
## npc_wintergarde_bomb
######*/
enum 
{
    NPC_UPPER_MINE_SHAFT            = 27436,
    NPC_LOWER_MINE_SHAFT            = 27437,
    QUEST_LEAVE_NOTHING_TO_CHANCE   = 12277
 
};

struct MANGOS_DLL_DECL npc_wintergarde_bombAI : public ScriptedAI
{
    npc_wintergarde_bombAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    uint32 uiCheckTimer;
    void Reset() 
    {
        uiCheckTimer = 2000;
    }

    void UpdateAI(const uint32 uiDiff)
     {
            if (uiCheckTimer <= uiDiff)
            {
                if(Player *pPlayer = m_creature->GetMap()->GetPlayer(m_creature->GetCreatorGuid()))
                {
                    if(pPlayer->GetQuestStatus(QUEST_LEAVE_NOTHING_TO_CHANCE) == QUEST_STATUS_INCOMPLETE)
                    {
                        if (m_creature->GetPositionZ() < 118.0f)
                            pPlayer->KilledMonsterCredit(NPC_LOWER_MINE_SHAFT);
                        else pPlayer->KilledMonsterCredit(NPC_UPPER_MINE_SHAFT);
                    }
                    m_creature->ForcedDespawn();
                }                
            } else uiCheckTimer -= uiDiff;
     }    
};
CreatureAI* GetAI_npc_wintergarde_bomb(Creature* pCreature)
{
    return new npc_wintergarde_bombAI(pCreature);
}

void AddSC_dragonblight()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "npc_afrasastrasz";
    pNewScript->pGossipHello = &GossipHello_npc_afrasastrasz;
    pNewScript->pGossipSelect = &GossipSelect_npc_afrasastrasz;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_alexstrasza_wr_gate";
    pNewScript->pGossipHello = &GossipHello_npc_alexstrasza_wr_gate;
    pNewScript->pGossipSelect = &GossipSelect_npc_alexstrasza_wr_gate;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_destructive_ward";
    pNewScript->GetAI = &GetAI_npc_destructive_ward;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_tariolstrasz";
    pNewScript->pGossipHello = &GossipHello_npc_tariolstrasz;
    pNewScript->pGossipSelect = &GossipSelect_npc_tariolstrasz;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_torastrasza";
    pNewScript->pGossipHello = &GossipHello_npc_torastrasza;
    pNewScript->pGossipSelect = &GossipSelect_npc_torastrasza;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_taunkale_refuge";
    pNewScript->GetAI = &GetAI_npc_taunkale_refuge;
    pNewScript->pGossipHello = &GossipHello_npc_taunkale_refuge;
    pNewScript->pGossipSelect = &GossipSelect_npc_taunkale_refuge;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_hourglass";
    pNewScript->GetAI = &GetAI_npc_hourglass;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_area_limiter";
    pNewScript->GetAI = &GetAI_npc_area_limiter;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_ravaged_giant";
    pNewScript->pGossipHello = &GossipHello_npc_ravaged_giant;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_vengeful_geist";
    pNewScript->GetAI = &GetAI_npc_vengeful_geist;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_inquisitor_hallard";
    pNewScript->GetAI = &GetAI_npc_inquisitor_hallard;
    pNewScript->pQuestAcceptNPC = &QuestAccept_npc_inquisitor_hallard;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_wintergarde_bomb";
    pNewScript->GetAI = &GetAI_npc_wintergarde_bomb;
    pNewScript->RegisterSelf();
}
