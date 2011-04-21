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
SDComment: Quest support: 12166, 12499/12500(end sequenze). Taxi paths Wyrmrest temple.
SDCategory: Dragonblight
EndScriptData */

/* ContentData
npc_afrasastrasz
npc_alexstrasza_wr_gate
npc_liquid_fire_of_elune
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
        NPC_TAUNKALE_REFUGEE            = 26179,    //creature counted as Q credit, there are 2 types of taunka refugees in DB
        POINT_NEAR_CRATE                = 1,
        POINT_REFUGEE_CAMP_EXIT         = 2,
        SPELL_TAUNKA_TRANSFORM_MALE     = 47022,
        SPELL_TAUNKA_TRANSFORM_FEMALE   = 47023,
        SAY_REFUGE                      = -1999926,

        NPC_TEXT_1_1                    = 12611,
        NPC_TEXT_1_2                    = GOSSIP_SENDER_MAIN, // to fill it up! "we are a people..."
        NPC_TEXT_1_3                    = GOSSIP_SENDER_MAIN, // may the lich king
        NPC_TEXT_2                      = 12618,
        NPC_TEXT_3                      = 12619,
        NPC_TEXT_4                      = 12620
};

uint32 NPC_TEXT_1[3] = {NPC_TEXT_1_1, NPC_TEXT_1_2, NPC_TEXT_1_3};

#define GOSSIP_ITEM_TAUNKA_REFUGEE_1    "Worry no more, taunka. The Horde will save and protect you and your people, but first you must swear allegiance to the Warchief by taking the blood oath of the Horde."
#define GOSSIP_ITEM_TAUNKA_REFUGEE_2    "Yes, taunka. Retribution is a given right of all members of the Horde."
#define GOSSIP_ITEM_TAUNKA_REFUGEE_3    "Than repeat after me: \"Lok'tar ogar! Victory or death - it is these words that bind me to the Horde. For they are the most sacred and fundamental of truths to warrior of the Horde./n/nI give my flesh and blood freely to the Warchief. I am the instrument of my Warchief\'s desire. I am a weapon of my Warchief\'s command./n/nFrom this moment until the end of days I live and die - For the Horde!\""
#define GOSSIP_ITEM_TAUNKA_REFUGEE_4    "For the horde!/n/nArm yourself  from the crates that surround us and report to Agmar\'s Hammer, east of here. Your first trial as a member of the Horde is survive the journey./n/nLok\'tar ogar!"

float fWestwindRefugeGatePos[2][3] = 
{
    {3698.48f, 2828.36f, 88.08f},
    {3690.35f, 2881.30f, 92.32f}
};

bool GossipHello_npc_taunkale_refuge(Player* pPlayer, Creature* pCreature)
{
    if (pPlayer->GetQuestStatus(QUEST_BLOOD_OATH_OF_THE_HORDE) == QUEST_STATUS_INCOMPLETE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_TAUNKA_REFUGEE_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

    pPlayer->SEND_GOSSIP_MENU(NPC_TEXT_1[urand(0,2)], pCreature->GetGUID());
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
            pPlayer->KilledMonsterCredit(NPC_TAUNKALE_REFUGEE, pCreature->GetGUID());
            if (GameObject* pCrate = GetClosestGameObjectWithEntry(pCreature, GO_RECOVERED_HORDE_ARMAMENTS, DEFAULT_VISIBILITY_DISTANCE))
            {
                float fDestX, fDestY, fDestZ;
                pCrate->GetNearPoint(pCrate, fDestX, fDestY, fDestZ, pCrate->GetObjectBoundingRadius(), 1.0f, 0.0f);
                pCreature->GetMotionMaster()->Clear();
                pCreature->SetSpeedRate(MOVE_RUN, 1.0f, true);
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
                        if (GameObject* pCrate = GetClosestGameObjectWithEntry(m_creature, GO_RECOVERED_HORDE_ARMAMENTS, 40.0f))
                            m_creature->SetFacingToObject(pCrate);
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
                            m_creature->GetMotionMaster()->MovePoint(POINT_REFUGEE_CAMP_EXIT, fWestwindRefugeGatePos[1][0], fWestwindRefugeGatePos[1][1], fWestwindRefugeGatePos[1][2]);
                        else
                            m_creature->GetMotionMaster()->MovePoint(POINT_REFUGEE_CAMP_EXIT, fWestwindRefugeGatePos[0][0], fWestwindRefugeGatePos[0][1], fWestwindRefugeGatePos[0][2]);
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

void AddSC_dragonblight()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "npc_afrasastrasz";
    newscript->pGossipHello = &GossipHello_npc_afrasastrasz;
    newscript->pGossipSelect = &GossipSelect_npc_afrasastrasz;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_alexstrasza_wr_gate";
    newscript->pGossipHello = &GossipHello_npc_alexstrasza_wr_gate;
    newscript->pGossipSelect = &GossipSelect_npc_alexstrasza_wr_gate;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_tariolstrasz";
    newscript->pGossipHello = &GossipHello_npc_tariolstrasz;
    newscript->pGossipSelect = &GossipSelect_npc_tariolstrasz;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_torastrasza";
    newscript->pGossipHello = &GossipHello_npc_torastrasza;
    newscript->pGossipSelect = &GossipSelect_npc_torastrasza;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_taunkale_refuge";
    newscript->GetAI = &GetAI_npc_taunkale_refuge;
    newscript->pGossipHello = &GossipHello_npc_taunkale_refuge;
    newscript->pGossipSelect = &GossipSelect_npc_taunkale_refuge;
    newscript->RegisterSelf();
}
