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
SDName: Hellfire_Peninsula
SD%Complete: 100
SDComment: Quest support: 9375, 9410, 9418, 10129, 10146, 10162, 10163, 10340, 10346, 10347, 10382 (Special flight paths), 10838
SDCategory: Hellfire Peninsula
EndScriptData */

/* ContentData
npc_aeranas
go_haaleshi_altar
npc_ancestral_wolf
npc_demoniac_scryer
npc_gryphoneer_windbellow
npc_naladu
npc_tracy_proudwell
npc_trollbane
npc_wing_commander_dabiree
npc_wing_commander_brack
npc_wounded_blood_elf
EndContentData */

#include "precompiled.h"
#include "escort_ai.h"

/*######
## npc_aeranas
######*/

#define SAY_SUMMON                      -1000138
#define SAY_FREE                        -1000139

#define FACTION_HOSTILE                 16
#define FACTION_FRIENDLY                35

#define SPELL_ENVELOPING_WINDS          15535
#define SPELL_SHOCK                     12553

#define C_AERANAS                       17085

struct MANGOS_DLL_DECL npc_aeranasAI : public ScriptedAI
{
    npc_aeranasAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    uint32 Faction_Timer;
    uint32 EnvelopingWinds_Timer;
    uint32 Shock_Timer;

    void Reset()
    {
        Faction_Timer = 8000;
        EnvelopingWinds_Timer = 9000;
        Shock_Timer = 5000;

        m_creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
        m_creature->setFaction(FACTION_FRIENDLY);

        DoScriptText(SAY_SUMMON, m_creature);
    }

    void UpdateAI(const uint32 diff)
    {
        if (Faction_Timer)
        {
            if (Faction_Timer <= diff)
            {
                m_creature->setFaction(FACTION_HOSTILE);
                Faction_Timer = 0;
            }else Faction_Timer -= diff;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_creature->GetHealthPercent() < 30.0f)
        {
            m_creature->setFaction(FACTION_FRIENDLY);
            m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
            m_creature->RemoveAllAuras();
            m_creature->DeleteThreatList();
            m_creature->CombatStop(true);
            DoScriptText(SAY_FREE, m_creature);
            return;
        }

        if (Shock_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_SHOCK);
            Shock_Timer = 10000;
        }else Shock_Timer -= diff;

        if (EnvelopingWinds_Timer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_ENVELOPING_WINDS);
            EnvelopingWinds_Timer = 25000;
        }else EnvelopingWinds_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_aeranas(Creature* pCreature)
{
    return new npc_aeranasAI(pCreature);
}

/*######
## go_haaleshi_altar
######*/

bool GOUse_go_haaleshi_altar(Player* pPlayer, GameObject* pGo)
{
    pGo->SummonCreature(C_AERANAS, -1321.79f, 4043.80f, 116.24f, 1.25f, TEMPSUMMON_TIMED_DESPAWN, 180000);
    return false;
}

/*######
## npc_ancestral_wolf
######*/

enum
{
    EMOTE_WOLF_LIFT_HEAD            = -1000496,
    EMOTE_WOLF_HOWL                 = -1000497,
    SAY_WOLF_WELCOME                = -1000498,

    SPELL_ANCESTRAL_WOLF_BUFF       = 29981,

    NPC_RYGA                        = 17123
};

struct MANGOS_DLL_DECL npc_ancestral_wolfAI : public npc_escortAI
{
    npc_ancestral_wolfAI(Creature* pCreature) : npc_escortAI(pCreature)
    {
        if (pCreature->GetOwner() && pCreature->GetOwner()->GetTypeId() == TYPEID_PLAYER)
            Start(false, pCreature->GetOwner()->GetGUID());
        else
            error_log("SD2: npc_ancestral_wolf can not obtain owner or owner is not a player.");

        Reset();
    }

    Unit* pRyga;

    void Reset()
    {
        pRyga = NULL;
        m_creature->CastSpell(m_creature, SPELL_ANCESTRAL_WOLF_BUFF, true);
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (!pRyga && pWho->GetTypeId() == TYPEID_UNIT && pWho->GetEntry() == NPC_RYGA && m_creature->IsWithinDistInMap(pWho, 15.0f))
            pRyga = pWho;

        npc_escortAI::MoveInLineOfSight(pWho);
    }

    void WaypointReached(uint32 uiPointId)
    {
        switch(uiPointId)
        {
            case 0:
                DoScriptText(EMOTE_WOLF_LIFT_HEAD, m_creature);
                break;
            case 2:
                DoScriptText(EMOTE_WOLF_HOWL, m_creature);
                break;
            case 50:
                if (pRyga && pRyga->isAlive() && !pRyga->isInCombat())
                    DoScriptText(SAY_WOLF_WELCOME, pRyga);
                break;
        }
    }
};

CreatureAI* GetAI_npc_ancestral_wolf(Creature* pCreature)
{
    return new npc_ancestral_wolfAI(pCreature);
}

/*######
## npc_demoniac_scryer
######*/

#define GOSSIP_ITEM_ATTUNE          "Yes, Scryer. You may possess me."

enum
{
    GOSSIP_TEXTID_PROTECT           = 10659,
    GOSSIP_TEXTID_ATTUNED           = 10643,

    QUEST_DEMONIAC                  = 10838,
    NPC_HELLFIRE_WARDLING           = 22259,
    NPC_BUTTRESS                    = 22267,                //the 4x nodes
    NPC_SPAWNER                     = 22260,                //just a dummy, not used

    MAX_BUTTRESS                    = 4,
    TIME_TOTAL                      = MINUTE*10*IN_MILLISECONDS,

    SPELL_SUMMONED_DEMON            = 7741,                 //visual spawn-in for demon
    SPELL_DEMONIAC_VISITATION       = 38708,                //create item

    SPELL_BUTTRESS_APPERANCE        = 38719,                //visual on 4x bunnies + the flying ones
    SPELL_SUCKER_CHANNEL            = 38721,                //channel to the 4x nodes
    SPELL_SUCKER_DESPAWN_MOB        = 38691
};

//script is basic support, details like end event are not implemented
struct MANGOS_DLL_DECL npc_demoniac_scryerAI : public ScriptedAI
{
    npc_demoniac_scryerAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_bIsComplete = false;
        m_uiSpawnDemonTimer = 15000;
        m_uiSpawnButtressTimer = 45000;
        m_uiButtressCount = 0;
        Reset();
    }

    bool m_bIsComplete;

    uint32 m_uiSpawnDemonTimer;
    uint32 m_uiSpawnButtressTimer;
    uint32 m_uiButtressCount;

    void Reset() {}

    //we don't want anything to happen when attacked
    void AttackedBy(Unit* pEnemy) {}
    void AttackStart(Unit* pEnemy) {}

    void DoSpawnButtress()
    {
        ++m_uiButtressCount;

        float fAngle;

        switch(m_uiButtressCount)
        {
            case 1: fAngle = 0.0f; break;
            case 2: fAngle = M_PI_F+M_PI_F/2; break;
            case 3: fAngle = M_PI_F/2; break;
            case 4: fAngle = M_PI_F; break;
        }

        float fX, fY, fZ;
        m_creature->GetNearPoint(m_creature, fX, fY, fZ, 0.0f, 5.0f, fAngle);

        uint32 uiTime = TIME_TOTAL - (m_uiSpawnButtressTimer * m_uiButtressCount);
        m_creature->SummonCreature(NPC_BUTTRESS, fX, fY, fZ, m_creature->GetAngle(fX, fY), TEMPSUMMON_TIMED_DESPAWN, uiTime);
    }

    void DoSpawnDemon()
    {
        float fX, fY, fZ;
        m_creature->GetRandomPoint(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 20.0f, fX, fY, fZ);

        m_creature->SummonCreature(NPC_HELLFIRE_WARDLING, fX, fY, fZ, 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() == NPC_HELLFIRE_WARDLING)
        {
            pSummoned->CastSpell(pSummoned, SPELL_SUMMONED_DEMON, false);
            pSummoned->AI()->AttackStart(m_creature);
        }
        else
        {
            if (pSummoned->GetEntry() == NPC_BUTTRESS)
            {
                pSummoned->CastSpell(pSummoned, SPELL_BUTTRESS_APPERANCE, false);
                pSummoned->CastSpell(m_creature, SPELL_SUCKER_CHANNEL, true);
            }
        }
    }

    void SpellHitTarget(Unit* pTarget, const SpellEntry* pSpell)
    {
        if (pTarget->GetEntry() == NPC_HELLFIRE_WARDLING && pSpell->Id == SPELL_SUCKER_DESPAWN_MOB)
            ((Creature*)pTarget)->ForcedDespawn();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_bIsComplete || !m_creature->isAlive())
            return;

        if (m_uiSpawnButtressTimer <= uiDiff)
        {
            if (m_uiButtressCount >= MAX_BUTTRESS)
            {
                m_creature->CastSpell(m_creature, SPELL_SUCKER_DESPAWN_MOB, false);

                if (m_creature->isInCombat())
                {
                    m_creature->DeleteThreatList();
                    m_creature->CombatStop();
                }

                m_bIsComplete = true;
                return;
            }

            m_uiSpawnButtressTimer = 45000;
            DoSpawnButtress();
        }
        else
            m_uiSpawnButtressTimer -= uiDiff;

        if (m_uiSpawnDemonTimer <= uiDiff)
        {
            DoSpawnDemon();
            m_uiSpawnDemonTimer = 15000;
        }
        else
            m_uiSpawnDemonTimer -= uiDiff;
    }
};

CreatureAI* GetAI_npc_demoniac_scryer(Creature* pCreature)
{
    return new npc_demoniac_scryerAI(pCreature);
}

bool GossipHello_npc_demoniac_scryer(Player* pPlayer, Creature* pCreature)
{
    if (npc_demoniac_scryerAI* pScryerAI = dynamic_cast<npc_demoniac_scryerAI*>(pCreature->AI()))
    {
        if (pScryerAI->m_bIsComplete)
        {
            if (pPlayer->GetQuestStatus(QUEST_DEMONIAC) == QUEST_STATUS_INCOMPLETE)
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_ATTUNE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

            pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_ATTUNED, pCreature->GetGUID());
            return true;
        }
    }

    pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_PROTECT, pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_demoniac_scryer(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF + 1)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pCreature->CastSpell(pPlayer, SPELL_DEMONIAC_VISITATION, false);
    }

    return true;
}

/*######
## npc_gryphoneer_windbellow
######*/

enum
{
    QUEST_ABYSSAL_A             = 10163,
    QUEST_RETURN_ABYSSAL_A      = 10346,
    QUEST_TO_THE_FRONT          = 10382,
    SPELL_TAXI_AERIAL_ASSULT    = 33899,
    SPELL_TAXI_TO_BEACH_HEAD    = 35065
};

#define GOSSIP_ITEM1_WIN        "Fly me to The Abyssal Shelf"
#define GOSSIP_ITEM2_WIN        "Fly me to Honor Point"

bool GossipHello_npc_gryphoneer_windbellow(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    //Mission: The Abyssal Shelf || Return to the Abyssal Shelf
    if (pPlayer->GetQuestStatus(QUEST_ABYSSAL_A) == QUEST_STATUS_INCOMPLETE ||
        pPlayer->GetQuestStatus(QUEST_RETURN_ABYSSAL_A) == QUEST_STATUS_INCOMPLETE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM1_WIN, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    //Go to the Front
    if (pPlayer->GetQuestStatus(QUEST_TO_THE_FRONT) == QUEST_STATUS_COMPLETE ||
        pPlayer->GetQuestRewardStatus(QUEST_TO_THE_FRONT))
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM2_WIN, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_gryphoneer_windbellow(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        //TaxiPath 589
        pPlayer->CastSpell(pPlayer,SPELL_TAXI_AERIAL_ASSULT,true);
    }
    if (uiAction == GOSSIP_ACTION_INFO_DEF+2)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        //TaxiPath 607
        pPlayer->CastSpell(pPlayer,SPELL_TAXI_TO_BEACH_HEAD,true);
    }
    return true;
}

/*######
## npc_naladu
######*/

#define GOSSIP_NALADU_ITEM1 "Why don't you escape?"

enum
{
    GOSSIP_TEXTID_NALADU1   = 9788
};

bool GossipHello_npc_naladu(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_NALADU_ITEM1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_naladu(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
        pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_NALADU1, pCreature->GetGUID());

    return true;
}

/*######
## npc_tracy_proudwell
######*/

#define GOSSIP_TEXT_REDEEM_MARKS        "I have marks to redeem!"
#define GOSSIP_TRACY_PROUDWELL_ITEM1    "I heard that your dog Fei Fei took Klatu's prayer beads..."
#define GOSSIP_TRACY_PROUDWELL_ITEM2    "<back>"

enum
{
    GOSSIP_TEXTID_TRACY_PROUDWELL1       = 10689,
    QUEST_DIGGING_FOR_PRAYER_BEADS       = 10916
};

bool GossipHello_npc_tracy_proudwell(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    if (pCreature->isVendor())
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, GOSSIP_TEXT_REDEEM_MARKS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);

    if (pPlayer->GetQuestStatus(QUEST_DIGGING_FOR_PRAYER_BEADS) == QUEST_STATUS_INCOMPLETE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_TRACY_PROUDWELL_ITEM1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_tracy_proudwell(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    switch(uiAction)
    {
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_TRACY_PROUDWELL_ITEM2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
            pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_TRACY_PROUDWELL1, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_TRADE:
            pPlayer->SEND_VENDORLIST(pCreature->GetGUID());
            break;
    }

    return true;
}

/*######
## npc_trollbane
######*/

#define GOSSIP_TROLLBANE_ITEM1      "Tell me of the Sons of Lothar."
#define GOSSIP_TROLLBANE_ITEM2      "<more>"
#define GOSSIP_TROLLBANE_ITEM3      "Tell me of your homeland."

enum
{
    GOSSIP_TEXTID_TROLLBANE1        = 9932,
    GOSSIP_TEXTID_TROLLBANE2        = 9933,
    GOSSIP_TEXTID_TROLLBANE3        = 8772
};

bool GossipHello_npc_trollbane(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_TROLLBANE_ITEM1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_TROLLBANE_ITEM3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_trollbane(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    switch(uiAction)
    {
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_TROLLBANE_ITEM2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
            pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_TROLLBANE1, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_TROLLBANE2, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+3:
            pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_TROLLBANE3, pCreature->GetGUID());
            break;
    }

    return true;
}

/*######
## npc_wing_commander_dabiree
######*/

enum
{
    SPELL_TAXI_TO_GATEWAYS      = 33768,
    SPELL_TAXI_TO_SHATTER       = 35069,
    QUEST_MISSION_GATEWAYS_A    = 10146,
    QUEST_SHATTER_POINT         = 10340
};

#define GOSSIP_ITEM1_DAB        "Fly me to Murketh and Shaadraz Gateways"
#define GOSSIP_ITEM2_DAB        "Fly me to Shatter Point"

bool GossipHello_npc_wing_commander_dabiree(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    //Mission: The Murketh and Shaadraz Gateways
    if (pPlayer->GetQuestStatus(QUEST_MISSION_GATEWAYS_A) == QUEST_STATUS_INCOMPLETE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM1_DAB, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    //Shatter Point
    if (pPlayer->GetQuestStatus(QUEST_SHATTER_POINT) == QUEST_STATUS_COMPLETE ||
        pPlayer->GetQuestRewardStatus(QUEST_SHATTER_POINT))
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM2_DAB, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_wing_commander_dabiree(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        //TaxiPath 585
        pPlayer->CastSpell(pPlayer,SPELL_TAXI_TO_GATEWAYS,true);
    }
    if (uiAction == GOSSIP_ACTION_INFO_DEF+2)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        //TaxiPath 612
        pPlayer->CastSpell(pPlayer,SPELL_TAXI_TO_SHATTER,true);
    }
    return true;
}

/*######
## npc_wing_commander_brack
######*/

enum
{
    QUEST_MISSION_GATEWAYS_H    = 10129,
    QUEST_ABYSSAL_H             = 10162,
    QUEST_RETURN_ABYSSAL_H      = 10347,
    QUEST_SPINEBREAKER          = 10242,
    SPELL_TAXI_GATEWAYS_H       = 33659,
    SPELL_TAXI_ASSULT_H         = 33825,
    SPELL_TAXI_SPINEBREAKER     = 34578
};

#define GOSSIP_ITEM1_BRA        "I'm on a bombing mission for Forward Commander To'arch. I need a wyvern destroyer!"
#define GOSSIP_ITEM2_BRA        "Fly me to The Abyssal Shelf"
#define GOSSIP_ITEM3_BRA        "Lend me a Wind Rider, I'm going to Spinebreaker Post."

bool GossipHello_npc_wing_commander_brack(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    //Mission: The Murketh and Shaadraz Gateways
    if (pPlayer->GetQuestStatus(QUEST_MISSION_GATEWAYS_H) == QUEST_STATUS_INCOMPLETE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM1_BRA, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    //Mission: The Abyssal Shelf || Return to the Abyssal Shelf
    if (pPlayer->GetQuestStatus(QUEST_ABYSSAL_H) == QUEST_STATUS_INCOMPLETE ||
        pPlayer->GetQuestStatus(QUEST_RETURN_ABYSSAL_H) == QUEST_STATUS_INCOMPLETE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM2_BRA, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);

    //Spinebreaker Post
    if (pPlayer->GetQuestStatus(QUEST_SPINEBREAKER) == QUEST_STATUS_COMPLETE ||
        pPlayer->GetQuestRewardStatus(QUEST_SPINEBREAKER))
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM3_BRA, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_wing_commander_brack(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    switch(uiAction)
    {
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->CLOSE_GOSSIP_MENU();
            //TaxiPath 584
            pPlayer->CastSpell(pPlayer,SPELL_TAXI_GATEWAYS_H,true);
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            pPlayer->CLOSE_GOSSIP_MENU();
            //TaxiPath 587
            pPlayer->CastSpell(pPlayer,SPELL_TAXI_ASSULT_H,true);
            break;
        case GOSSIP_ACTION_INFO_DEF+3:
            pPlayer->CLOSE_GOSSIP_MENU();
            //TaxiPath 604
            pPlayer->CastSpell(pPlayer,SPELL_TAXI_SPINEBREAKER,true);
            break;
    }
    return true;
}

/*######
## npc_wounded_blood_elf
######*/

#define SAY_ELF_START               -1000117
#define SAY_ELF_SUMMON1             -1000118
#define SAY_ELF_RESTING             -1000119
#define SAY_ELF_SUMMON2             -1000120
#define SAY_ELF_COMPLETE            -1000121
#define SAY_ELF_AGGRO               -1000122

#define QUEST_ROAD_TO_FALCON_WATCH  9375

struct MANGOS_DLL_DECL npc_wounded_blood_elfAI : public npc_escortAI
{
    npc_wounded_blood_elfAI(Creature* pCreature) : npc_escortAI(pCreature) {Reset();}

    void WaypointReached(uint32 i)
    {
        Player* pPlayer = GetPlayerForEscort();

        if (!pPlayer)
            return;

        switch (i)
        {
            case 0:
                DoScriptText(SAY_ELF_START, m_creature, pPlayer);
                break;
            case 9:
                DoScriptText(SAY_ELF_SUMMON1, m_creature, pPlayer);
                // Spawn two Haal'eshi Talonguard
                DoSpawnCreature(16967, -15, -15, 0, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);
                DoSpawnCreature(16967, -17, -17, 0, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);
                break;
            case 13:
                DoScriptText(SAY_ELF_RESTING, m_creature, pPlayer);
                break;
            case 14:
                DoScriptText(SAY_ELF_SUMMON2, m_creature, pPlayer);
                // Spawn two Haal'eshi Windwalker
                DoSpawnCreature(16966, -15, -15, 0, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);
                DoSpawnCreature(16966, -17, -17, 0, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);
                break;
            case 27:
                DoScriptText(SAY_ELF_COMPLETE, m_creature, pPlayer);
                // Award quest credit
                pPlayer->GroupEventHappens(QUEST_ROAD_TO_FALCON_WATCH, m_creature);
                break;
        }
    }

    void Reset() { }

    void Aggro(Unit* who)
    {
        if (HasEscortState(STATE_ESCORT_ESCORTING))
            DoScriptText(SAY_ELF_AGGRO, m_creature);
    }

    void JustSummoned(Creature* summoned)
    {
        summoned->AI()->AttackStart(m_creature);
    }
};

CreatureAI* GetAI_npc_wounded_blood_elf(Creature* pCreature)
{
    return new npc_wounded_blood_elfAI(pCreature);
}

bool QuestAccept_npc_wounded_blood_elf(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    if (pQuest->GetQuestId() == QUEST_ROAD_TO_FALCON_WATCH)
    {
        // Change faction so mobs attack
        pCreature->setFaction(FACTION_ESCORT_H_PASSIVE);

        if (npc_wounded_blood_elfAI* pEscortAI = dynamic_cast<npc_wounded_blood_elfAI*>(pCreature->AI()))
            pEscortAI->Start(false, pPlayer->GetGUID(), pQuest);
    }

    return true;
}

/*######
## quest: seer's relic
######*/

enum
{
    SPELL_EXECUTE   = 30462,

    NPC_ESCORT      = 17417,
    NPC_AMBUSH      = 17418,
    NPC_KRUN        = 17405,

    //Texts
    //Sedai
    TEXT_SEDAI1     = -1174041, //I've failed... peace is impossible.
    TEXT_SEDAI2     = -1174042, //What in the Light's name...?
    TEXT_SEDAI3     = -1174043, //Fel orcs!
    TEXT_SEDAI4     = -1174044, //The cycle of bloodshed is unending. Is there nothing I can do?
    //Orcs
    TEXT_ORCS1      = -1174171, //Do not return, draenei scum. Next time we won't spare your life, unarmed or not!
    //Krun
    TEXT_KRUN1      = -1174051 //You can die!
};

struct MANGOS_DLL_DECL npc_Vindicator_SedaiAI : public ScriptedAI
{
    npc_Vindicator_SedaiAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint8       Phase;
    uint32      TextTimer;
    ObjectGuid  Escort1;
    ObjectGuid  Escort2;

    void Reset()
    {
        if(m_creature->GetAreaId() == 3483)
        {
            Phase = 0;
            TextTimer = 2000;
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE);

            if(Creature* pEscort1 = m_creature->SummonCreature(NPC_ESCORT, 226.950f, 4120.283f, 82.68f, m_creature->GetOrientation(), TEMPSUMMON_CORPSE_DESPAWN, 20000))
                Escort1 = pEscort1->GetGUID();
            if(Creature* pEscort2 = m_creature->SummonCreature(NPC_ESCORT, 224.703f, 4117.889f, 82.19f, m_creature->GetOrientation(), TEMPSUMMON_CORPSE_DESPAWN, 20000))
                Escort2 = pEscort2->GetGUID();
        }
    }
    void UpdateAI(const uint32 uiDiff)
    {
        if(TextTimer <= uiDiff)
        {
            if(m_creature->GetAreaId() == 3483) //Hellfire peninsula - it's useful when someone summon npc in other location ;)
            {
                Creature* MagEscort1 = m_creature->GetMap()->GetCreature(Escort1);
                Creature* MagEscort2 = m_creature->GetMap()->GetCreature(Escort2);
                Creature* pKrun = GetClosestCreatureWithEntry(m_creature, NPC_KRUN, 200.0f);

            if(MagEscort1 && MagEscort2)
            {
                switch(Phase)
                {
                    case 0:
                    {
                        m_creature->GetMotionMaster()->MovePoint(1, 199.298f, 4141.842f, 75.08f);
                        MagEscort1->GetMotionMaster()->MovePoint(2, 201.895f, 4140.031f, 75.68f);
                        MagEscort2->GetMotionMaster()->MovePoint(3, 199.046f, 4137.802f, 75.10f);
                        TextTimer = 16000;
                        Phase++;
                        break;
                    }
                    case 1:
                    {
                        int i = urand(0,1);
                        switch(i)
                        {
                            case 0: MagEscort1->HandleEmote(EMOTE_ONESHOT_KICK); DoScriptText(TEXT_ORCS1, MagEscort1); break;
                            case 1: MagEscort2->HandleEmote(EMOTE_ONESHOT_KICK); DoScriptText(TEXT_ORCS1, MagEscort2); break;
                            default: break;
                        }
                        //There need to be other Emote? this don't work ;/
                        //Workaround
                        m_creature->CastSpell(m_creature, 39656, true);
                        //m_creature->HandleEmoteState(EMOTE_STATE_KNEEL);
                        TextTimer = 3000;
                        Phase++;
                        break;
                    }
                    case 2:
                    {
                        DoScriptText(TEXT_SEDAI1, m_creature);
                        MagEscort1->GetMotionMaster()->MovePoint(4, 225.0f, 4125.0f, 82.26f);
                        MagEscort2->GetMotionMaster()->MovePoint(5, 228.0f, 4120.0f, 82.26f);
                        TextTimer = 3000;
                        Phase++;
                        break;
                    }
                    case 3: m_creature->SummonCreature(NPC_AMBUSH, 225.5f, 4120.2f, 82.26f, 2.4f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 25000); TextTimer = 8000; Phase++; break;
                    case 4:
                    {
                        if(Creature* pAmbush = GetClosestCreatureWithEntry(m_creature, NPC_AMBUSH, 150.0f))
                        {
                            MagEscort1->AI()->AttackStart(pAmbush);
                            MagEscort2->AI()->AttackStart(pAmbush);
                        }
                        TextTimer = 3000;
                        Phase++;
                        break;
                    }
                    case 5: DoScriptText(TEXT_SEDAI2, m_creature); TextTimer = 4000; Phase++; break;
                    case 6: m_creature->RemoveAurasDueToSpell(39656); m_creature->GetMotionMaster()->MovePoint(6, 211.074f, 4131.552f, 78.74f); TextTimer = 8000; Phase++; break;
                    case 7: DoScriptText(TEXT_SEDAI3, m_creature); TextTimer = 8000; Phase++; break;
                    case 8:
                    {
                        if(Creature* pAmbush = GetClosestCreatureWithEntry(m_creature, NPC_AMBUSH, 100.0f))
                            pAmbush->GetMotionMaster()->MovePoint(7, 247.64f, 4111.432f, 87.4f);

                        m_creature->GetMotionMaster()->MovePoint(8, 192.873f, 4149.576f, 73.66f);
                        TextTimer = 11000;
                        Phase++;
                        break;
                    }
                    case 9: m_creature->SummonCreature(NPC_KRUN, 225.5f, 4120.2f, 82.26f, 2.4f, TEMPSUMMON_TIMED_DESPAWN, 27000); m_creature->CastSpell(m_creature, 39656, true); TextTimer = 2000; Phase++; break;
                    if(pKrun)
                    {
                        case 10: pKrun->GetMotionMaster()->MovePoint(9, 194.150f, 4147.903f, 73.84f); TextTimer = 12000; Phase++; break;
                        case 11: DoScriptText(TEXT_SEDAI4, m_creature); TextTimer = 8000; Phase++; break;
                        case 12: m_creature->RemoveAurasDueToSpell(39656); m_creature->HandleEmoteState(EMOTE_STATE_COWER); TextTimer = 750; Phase++; break;
                        case 13: DoScriptText(TEXT_KRUN1, pKrun); pKrun->CastSpell(m_creature, SPELL_EXECUTE, true); TextTimer = 2000; Phase++; break;
                        case 14: pKrun->HandleEmoteState(EMOTE_STATE_ROAR); Phase++; break;
                    }
                    default: break;
                }
            }
            }
        } else TextTimer -= uiDiff;
    }
};

CreatureAI* GetAI_npc_Vindicator_Sedai(Creature* pCreature)
{
    return new npc_Vindicator_SedaiAI(pCreature);
}

void AddSC_hellfire_peninsula()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "npc_aeranas";
    newscript->GetAI = &GetAI_npc_aeranas;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_haaleshi_altar";
    newscript->pGOUse = &GOUse_go_haaleshi_altar;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_ancestral_wolf";
    newscript->GetAI = &GetAI_npc_ancestral_wolf;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_demoniac_scryer";
    newscript->GetAI = &GetAI_npc_demoniac_scryer;
    newscript->pGossipHello = &GossipHello_npc_demoniac_scryer;
    newscript->pGossipSelect = &GossipSelect_npc_demoniac_scryer;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_gryphoneer_windbellow";
    newscript->pGossipHello = &GossipHello_npc_gryphoneer_windbellow;
    newscript->pGossipSelect = &GossipSelect_npc_gryphoneer_windbellow;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_naladu";
    newscript->pGossipHello = &GossipHello_npc_naladu;
    newscript->pGossipSelect = &GossipSelect_npc_naladu;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_tracy_proudwell";
    newscript->pGossipHello = &GossipHello_npc_tracy_proudwell;
    newscript->pGossipSelect = &GossipSelect_npc_tracy_proudwell;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_trollbane";
    newscript->pGossipHello = &GossipHello_npc_trollbane;
    newscript->pGossipSelect = &GossipSelect_npc_trollbane;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_wing_commander_dabiree";
    newscript->pGossipHello = &GossipHello_npc_wing_commander_dabiree;
    newscript->pGossipSelect = &GossipSelect_npc_wing_commander_dabiree;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_wing_commander_brack";
    newscript->pGossipHello = &GossipHello_npc_wing_commander_brack;
    newscript->pGossipSelect = &GossipSelect_npc_wing_commander_brack;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_wounded_blood_elf";
    newscript->GetAI = &GetAI_npc_wounded_blood_elf;
    newscript->pQuestAcceptNPC = &QuestAccept_npc_wounded_blood_elf;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_Vindicator_Sedai";
    newscript->GetAI = &GetAI_npc_Vindicator_Sedai;
    newscript->RegisterSelf();
}
