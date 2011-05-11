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
SDName: Howling_Fjord
SD%Complete: ?
SDComment: Quest support: 11221, 11300, 11464, 11343
SDCategory: Howling Fjord
EndScriptData */

/* ContentData
npc_ancient_male_vrykul
at_ancient_male_vrykul
npc_daegarn
npc_deathstalker_razael - TODO, can be moved to database
npc_dark_ranger_lyana - TODO, can be moved to database
npc_silvermoon_harry
EndContentData */

#include "precompiled.h"

enum
{
    SPELL_ECHO_OF_YMIRON                    = 42786,
    SPELL_SECRET_OF_NIFFELVAR               = 43458,
    QUEST_ECHO_OF_YMIRON                    = 11343,
    NPC_MALE_VRYKUL                         = 24314,
    NPC_FEMALE_VRYKUL                       = 24315,

    SAY_VRYKUL_CURSED                       = -1000635,
    EMOTE_VRYKUL_POINT                      = -1000636,
    EMOTE_VRYKUL_SOB                        = -1000637,
    SAY_VRYKUL_DISPOSE                      = -1000638,
    SAY_VRYKUL_BEG                          = -1000639,
    SAY_VRYKUL_WHAT                         = -1000640,
    SAY_VRYKUL_HIDE                         = -1000641,
};

struct MANGOS_DLL_DECL npc_ancient_male_vrykulAI : public ScriptedAI
{
    npc_ancient_male_vrykulAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    bool m_bEventInProgress;
    uint32 m_uiPhase;
    uint32 m_uiPhaseTimer;

    void Reset()
    {
        m_bEventInProgress = false;
        m_uiPhase = 0;
        m_uiPhaseTimer = 0;
    }

    void StartEvent()
    {
        if (m_bEventInProgress)
            return;

        m_bEventInProgress = true;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_bEventInProgress)
            return;

        if (m_uiPhaseTimer < uiDiff)
            m_uiPhaseTimer = 5000;
        else
        {
            m_uiPhaseTimer -= uiDiff;
            return;
        }

        Creature* pFemale = GetClosestCreatureWithEntry(m_creature, NPC_FEMALE_VRYKUL, 10.0f);

        switch(m_uiPhase)
        {
            case 0:
                DoScriptText(SAY_VRYKUL_CURSED, m_creature);
                DoScriptText(EMOTE_VRYKUL_POINT, m_creature);
                break;
            case 1:
                if (pFemale)
                    DoScriptText(EMOTE_VRYKUL_SOB, pFemale);
                DoScriptText(SAY_VRYKUL_DISPOSE, m_creature);
                break;
            case 2:
                if (pFemale)
                    DoScriptText(SAY_VRYKUL_BEG, pFemale);
                break;
            case 3:
                DoScriptText(SAY_VRYKUL_WHAT, m_creature);
                break;
            case 4:
                if (pFemale)
                    DoScriptText(SAY_VRYKUL_HIDE, pFemale);
                break;
            case 5:
                DoCastSpellIfCan(m_creature, SPELL_SECRET_OF_NIFFELVAR);
                break;
            case 6:
                Reset();
                return;
        }

        ++m_uiPhase;
    }
};

CreatureAI* GetAI_npc_ancient_male_vrykul(Creature* pCreature)
{
    return new npc_ancient_male_vrykulAI(pCreature);
}

bool AreaTrigger_at_ancient_male_vrykul(Player* pPlayer, AreaTriggerEntry const* pAt)
{
    if (pPlayer->isAlive() && pPlayer->GetQuestStatus(QUEST_ECHO_OF_YMIRON) == QUEST_STATUS_INCOMPLETE &&
        pPlayer->HasAura(SPELL_ECHO_OF_YMIRON))
    {
        if (Creature* pCreature = GetClosestCreatureWithEntry(pPlayer, NPC_MALE_VRYKUL, 20.0f))
        {
            if (npc_ancient_male_vrykulAI* pVrykulAI = dynamic_cast<npc_ancient_male_vrykulAI*>(pCreature->AI()))
                pVrykulAI->StartEvent();
        }
    }

    return true;
}

/*######
## npc_daegarn
######*/

enum
{
    QUEST_DEFEAT_AT_RING            = 11300,

    NPC_FIRJUS                      = 24213,
    NPC_JLARBORN                    = 24215,
    NPC_YOROS                       = 24214,
    NPC_OLUF                        = 23931,

    NPC_PRISONER_1                  = 24253,                // looks the same but has different abilities
    NPC_PRISONER_2                  = 24254,
    NPC_PRISONER_3                  = 24255,
};

static float afSummon[] = {838.81f, -4678.06f, -94.182f};
static float afCenter[] = {801.88f, -4721.87f, -96.143f};

// TODO: make prisoners help (unclear if summoned or using npc's from surrounding cages (summon inside small cages?))
struct MANGOS_DLL_DECL npc_daegarnAI : public ScriptedAI
{
    npc_daegarnAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    bool m_bEventInProgress;
    uint64 m_uiPlayerGUID;

    void Reset()
    {
        m_bEventInProgress = false;
        m_uiPlayerGUID = 0;
    }

    void StartEvent(Player* pPlayer)
    {
        if (m_bEventInProgress)
            return;

        m_uiPlayerGUID = pPlayer->GetGUID();

        SummonGladiator(NPC_FIRJUS);
    }

    void JustSummoned(Creature* pSummon)
    {
        if (Player* pPlayer = m_creature->GetMap()->GetPlayer(m_uiPlayerGUID))
        {
            if (pPlayer->isAlive())
            {
                pSummon->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                pSummon->GetMotionMaster()->MovePoint(0, afCenter[0], afCenter[1], afCenter[2]);
                return;
            }
        }

        Reset();
    }

    void SummonGladiator(uint32 uiEntry)
    {
        m_creature->SummonCreature(uiEntry, afSummon[0], afSummon[1], afSummon[2], 0.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 20*IN_MILLISECONDS);
    }

    void SummonedMovementInform(Creature* pSummoned, uint32 uiMotionType, uint32 uiPointId)
    {
        Player* pPlayer = m_creature->GetMap()->GetPlayer(m_uiPlayerGUID);

        // could be group, so need additional here.
        if (!pPlayer || !pPlayer->isAlive())
        {
            Reset();
            return;
        }

        if (pSummoned->IsWithinDistInMap(pPlayer, 75.0f))   // ~the radius of the ring
            pSummoned->AI()->AttackStart(pPlayer);
    }

    void SummonedCreatureDespawn(Creature* pSummoned)
    {
        uint32 uiEntry = 0;

        // will eventually reset the event if something goes wrong
        switch(pSummoned->GetEntry())
        {
            case NPC_FIRJUS:    uiEntry = NPC_JLARBORN; break;
            case NPC_JLARBORN:  uiEntry = NPC_YOROS;    break;
            case NPC_YOROS:     uiEntry = NPC_OLUF;     break;
            case NPC_OLUF:      Reset();                return;
        }

        SummonGladiator(uiEntry);
    }
};

bool QuestAccept_npc_daegarn(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    if (pQuest->GetQuestId() == QUEST_DEFEAT_AT_RING)
    {
        if (npc_daegarnAI* pDaegarnAI = dynamic_cast<npc_daegarnAI*>(pCreature->AI()))
            pDaegarnAI->StartEvent(pPlayer);
    }

    return true;
}

CreatureAI* GetAI_npc_daegarn(Creature* pCreature)
{
    return new npc_daegarnAI(pCreature);
}

/*######
## npc_deathstalker_razael - TODO, can be moved to database
######*/

#define GOSSIP_ITEM_DEATHSTALKER_RAZAEL "High Executor Anselm requests your report."

enum
{
    QUEST_REPORTS_FROM_THE_FIELD       = 11221,
    SPELL_RAZAEL_KILL_CREDIT           = 42756,
    GOSSIP_TEXTID_DEATHSTALKER_RAZAEL1 = 11562,
    GOSSIP_TEXTID_DEATHSTALKER_RAZAEL2 = 11564
};

bool GossipHello_npc_deathstalker_razael(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    if (pPlayer->GetQuestStatus(QUEST_REPORTS_FROM_THE_FIELD) == QUEST_STATUS_INCOMPLETE)
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_DEATHSTALKER_RAZAEL, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_DEATHSTALKER_RAZAEL1, pCreature->GetGUID());
    }
    else
        pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());

    return true;
}

bool GossipSelect_npc_deathstalker_razael(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    switch(uiAction)
    {
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_DEATHSTALKER_RAZAEL2, pCreature->GetGUID());
            pCreature->CastSpell(pPlayer, SPELL_RAZAEL_KILL_CREDIT, true);
            break;
    }

    return true;
}

/*######
## npc_dark_ranger_lyana - TODO, can be moved to database
######*/

#define GOSSIP_ITEM_DARK_RANGER_LYANA "High Executor Anselm requests your report."

enum
{
    GOSSIP_TEXTID_DARK_RANGER_LYANA1    = 11586,
    GOSSIP_TEXTID_DARK_RANGER_LYANA2    = 11588,
    SPELL_DARK_RANGER_LYANA_KILL_CREDIT = 42799
};

bool GossipHello_npc_dark_ranger_lyana(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    if (pPlayer->GetQuestStatus(QUEST_REPORTS_FROM_THE_FIELD) == QUEST_STATUS_INCOMPLETE)
    {
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_DARK_RANGER_LYANA, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_DARK_RANGER_LYANA1, pCreature->GetGUID());
    }
    else
        pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());

    return true;
}

bool GossipSelect_npc_dark_ranger_lyana(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    switch(uiAction)
    {
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->SEND_GOSSIP_MENU(GOSSIP_TEXTID_DARK_RANGER_LYANA2, pCreature->GetGUID());
            pCreature->CastSpell(pPlayer, SPELL_DARK_RANGER_LYANA_KILL_CREDIT, true);
            break;
    }

    return true;
}

/*######
## npc_greer_orehammer
######*/

enum
{
    GOSSIP_ITEM_TAXI                        = -3000106,
    GOSSIP_ITEM_GET_BOMBS                   = -3000107,
    GOSSIP_ITEM_FLIGHT                      = -3000108,

    QUEST_MISSION_PLAGUE_THIS               = 11332,
    ITEM_PRECISION_BOMBS                    = 33634,
    TAXI_PATH_PLAGUE_THIS                   = 745,
};

bool GossipHello_npc_greer_orehammer(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    if (pPlayer->GetQuestStatus(QUEST_MISSION_PLAGUE_THIS) == QUEST_STATUS_INCOMPLETE)
    {
        if (!pPlayer->HasItemCount(ITEM_PRECISION_BOMBS, 1, true))
            pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ITEM_GET_BOMBS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

        pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_CHAT, GOSSIP_ITEM_FLIGHT, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
    }

    if (pCreature->isTaxi())
        pPlayer->ADD_GOSSIP_ITEM_ID(GOSSIP_ICON_TAXI, GOSSIP_ITEM_TAXI, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());

    return true;
}

bool GossipSelect_npc_greer_orehammer(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    switch(uiAction)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:
            if (Item* pItem = pPlayer->StoreNewItemInInventorySlot(ITEM_PRECISION_BOMBS, 10))
                pPlayer->SendNewItem(pItem, 10, true, false);

            pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF + 2:
            pPlayer->CLOSE_GOSSIP_MENU();
            pPlayer->ActivateTaxiPathTo(TAXI_PATH_PLAGUE_THIS);
            break;
        case GOSSIP_ACTION_INFO_DEF + 3:
            pPlayer->GetSession()->SendTaxiMenu(pCreature);
            break;
    }

    return true;
}

/*######
## npc_silvermoon_harry
######*/

enum Sivermoon_Harry
{
    QUEST_GAMBLING_DEBT             = 11464,
    SPELL_BLASTWAVE                 = 15091,
    SPELL_SCORCH                    = 50183,
    ITEM_SILVERMOON_HARRYS_DEBT     = 34115,
    SAY_SILVERMOON_HARRY_AGGRO      = -1999820,
    SAY_SILVERMOON_HARRY_SURRENDER  = -1999819
};

#define GOSSIP_ITEM_SILVERMOON_HARRY_1 "Taruk send me to collect what you owe."
#define GOSSIP_ITEM_SILVERMOON_HARRY_2 "Alright, here's your cut."

struct MANGOS_DLL_DECL npc_silvermoon_harryAI : public ScriptedAI
{
    npc_silvermoon_harryAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    bool bTriggeredByEvent;
    bool bEventDone;

    uint32 m_uiEventTimer;
    uint64 PlayerGUID;
    GUIDList lPlayersEventDone;

    void Reset() 
    {
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->setFaction(m_creature->GetCreatureInfo()->faction_A);
        bTriggeredByEvent = false;
        bEventDone = false;
        m_uiEventTimer = urand(3000,6000);
        PlayerGUID = 0;
    }

    void AttackStart(Unit* pWho)
    {
        if (!pWho)
            return;

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        ScriptedAI::AttackStart(pWho);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        // Silvermoon Harry is aggresive for players that not have complered prequests
        // or are not at quest, so in that case lets switch this part off
        if (bTriggeredByEvent)
        {
            if (m_creature->GetHealthPercent() < 33.0f)
            {
                ThreatList const& tList = m_creature->getThreatManager().getThreatList();
                for (ThreatList::const_iterator itr = tList.begin();itr != tList.end(); ++itr)
                {
                    if ((*itr))
                    {
                        if (Unit* pUnit = m_creature->GetMap()->GetUnit((*itr)->getUnitGuid()))
                        {
                            if (pUnit->GetTypeId() == TYPEID_PLAYER)
                                lPlayersEventDone.push_back((*itr)->getUnitGuid());
                        }
                    }
                }
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                m_creature->setFaction(m_creature->GetCreatureInfo()->faction_A);
                m_creature->CombatStop();
                m_creature->DeleteThreatList();
                DoScriptText(SAY_SILVERMOON_HARRY_SURRENDER,m_creature);
                m_uiEventTimer = 30000;
                bEventDone = true;
            }
        }

        // reset creature after amount of time
        if (bEventDone)
        {
            if (m_uiEventTimer <= uiDiff)
            {
                lPlayersEventDone.clear();
                m_creature->AI()->EnterEvadeMode();
            }else m_uiEventTimer -= uiDiff;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiEventTimer <= uiDiff)
        {
            DoCast(m_creature->getVictim(),(urand(0,1) == 0 ? SPELL_BLASTWAVE : SPELL_SCORCH),false);
            m_uiEventTimer = urand(3000,6000);
        }else m_uiEventTimer -= uiDiff;
        
        DoMeleeAttackIfReady();
    }

    // two functions managing players that have joined encounter and are supposed to 
    // get quest item at the end
    bool CheckoutPlayerEventDone(uint64 PlayerGUID)
    {
        if (lPlayersEventDone.empty())
            return false;

        for(GUIDList::iterator itr = lPlayersEventDone.begin(); itr != lPlayersEventDone.end(); ++itr)
        {
            if ((*itr) && (*itr) == PlayerGUID)
                return true;
        }

        return false;
    }
};

bool GossipHello_npc_silvermoon_harry(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    if (pCreature->isVendor())
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, GOSSIP_TEXT_BROWSE_GOODS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);

    if (pPlayer->GetQuestStatus(QUEST_GAMBLING_DEBT) == QUEST_STATUS_INCOMPLETE)
    {
        if (((npc_silvermoon_harryAI*)pCreature->AI())->CheckoutPlayerEventDone(pPlayer->GetGUID()))
        {
            if (pPlayer->GetItemCount(ITEM_SILVERMOON_HARRYS_DEBT,false) < 1)
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_SILVERMOON_HARRY_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
        }
        else pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_SILVERMOON_HARRY_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
    }

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_silvermoon_harry(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    switch (uiAction)
    {
        case GOSSIP_ACTION_TRADE:
            pPlayer->SEND_VENDORLIST(pCreature->GetGUID());
            break;

        case GOSSIP_ACTION_INFO_DEF+1: 
            ((npc_silvermoon_harryAI*)pCreature->AI())->AttackStart(pPlayer);
            ((npc_silvermoon_harryAI*)pCreature->AI())->bTriggeredByEvent = true;
            pCreature->setFaction(16);
            DoScriptText(SAY_SILVERMOON_HARRY_AGGRO,pCreature);
            pPlayer->CLOSE_GOSSIP_MENU();
            break;

        case GOSSIP_ACTION_INFO_DEF+2:
            ItemPosCountVec dest;
            uint8 msg = pPlayer->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, ITEM_SILVERMOON_HARRYS_DEBT, 1, false);
            if (msg == EQUIP_ERR_OK)
                pPlayer->StoreNewItem(dest, ITEM_SILVERMOON_HARRYS_DEBT, 1, true);
            pPlayer->CLOSE_GOSSIP_MENU();
            break;            
    }

    return true;
}

CreatureAI* GetAI_npc_silvermoon_harry(Creature* pCreature)
{
    return new npc_silvermoon_harryAI(pCreature);
}

/*######
## npc_alliance_banner
######*/

enum
{
    NPC_WINTERSKORN_DEFENDER    = 24015,
	QUEST_DROP_IT_THEN_ROCK_IT	= 11429

};

struct MANGOS_DLL_DECL npc_bannerAI : public ScriptedAI
{
    npc_bannerAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }
    
     
	 uint64 uiWaveTimer;
     uint32 uiWaveCounter;

     void Reset()
     {
		 uiWaveTimer = 2000;
		 uiWaveCounter = 0;
     }

     void JustSummoned(Creature* pSummoned)
     {
		 pSummoned->Attack(m_creature, true);
         pSummoned->AddThreat(m_creature, 999.9f, true);
     }              
     void UpdateAI(const uint32 uiDiff)
        {
				if (uiWaveTimer <= uiDiff)
				{										
					if(uiWaveCounter == 4)
					{
						if(Player *pPlayer = m_creature->GetMap()->GetPlayer(m_creature->GetOwnerGuid()))
						{
							pPlayer->AreaExploredOrEventHappens(QUEST_DROP_IT_THEN_ROCK_IT);
                            m_creature->ForcedDespawn(1000);
						}
					}
					else m_creature->SummonCreature(NPC_WINTERSKORN_DEFENDER, 1481.52f, -5326.88f, 194.52f, 0, TEMPSUMMON_CORPSE_DESPAWN, 0);	
				  uiWaveTimer = 5000;
				  uiWaveCounter++;
				} else uiWaveTimer -= uiDiff;
			
        }
};

CreatureAI* GetAI_npc_banner(Creature* pCreature)
{
    return new npc_bannerAI(pCreature);
}

/*######
## npc_king_ymiron
######*/

enum
{
    AC_VRYKUL_SAY_1			= -1799000,
	AC_VRYKUL_SAY_2			= -1799001,
	AC_VRYKUL_SAY_3			= -1799002,
	AC_VRYKUL_SAY_4			= -1799003,
	AC_VRYKUL_SAY_5			= -1799004,
    AC_VRYKUL_SAY_6			= -1799005,
	AC_VRYKUL_SAY_7			= -1799006,
	AC_VRYKUL_SAY_8	        = -1799008,

    KING_YMIRON_SAY_1       = -1799007, 
    KING_YMIRON_SAY_2       = -1799009,
    KING_YMIRON_SAY_3       = -1799010,
    KING_YMIRON_SAY_4       = -1799012,
    KING_YMIRON_SAY_5       = -1799013,
    KING_YMIRON_SAY_6       = -1799014,
    KING_YMIRON_SAY_7       = -1799015,
    KING_YMIRON_SAY_8       = -1799016,
    KING_YMIRON_SAY_9       = -1799018,
    KING_YMIRON_SAY_10      = -1799020,

    TEXT_EMOTE_1            = -1799011,
    TEXT_EMOTE_2            = -1799017,
    TEXT_EMOTE_3            = -1799019,

	NPC_ANCIENT_CITIZEN_M   = 24323,
    NPC_ANCIENT_CITIZEN_F   = 24322,
	QUEST_ANGUISH_NIFFLEVAR	= 11344

};

struct MANGOS_DLL_DECL npc_king_ymironAI : public ScriptedAI
{
    npc_king_ymironAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }
    
     uint64 uiPlayerGUID;
	 uint64 uiSpeechTimer;
     bool bEventStarted;
	 uint32 uiPhase;

     void Reset()
     {
         uiPlayerGUID = 0;
         bEventStarted = false;
		 uiPhase = 0;
		 uiSpeechTimer = 2000;
     }

              
     void MoveInLineOfSight(Unit *pWho)
     {
         ScriptedAI::MoveInLineOfSight(pWho);


		 if (pWho->GetTypeId() == TYPEID_PLAYER && m_creature->IsWithinDistInMap(pWho, 40.0f) )
         {
             if(!bEventStarted)
             {
			    uiPlayerGUID = pWho->GetGUID();
                bEventStarted = true;  
             }
         }
      }
     
     void UpdateAI(const uint32 uiDiff)
        {
			if(bEventStarted)
			{
				if (uiSpeechTimer <= uiDiff)
				{
					switch(uiPhase)
					{
						case 0: if(Creature *pFemaleVrykul= GetClosestCreatureWithEntry(m_creature, NPC_ANCIENT_CITIZEN_M, 40.0f))
									{
										DoScriptText(AC_VRYKUL_SAY_1, pFemaleVrykul);
									}
								uiPhase++; uiSpeechTimer = 5000; break;
						case 1: if(Creature *pFemaleVrykul= GetClosestCreatureWithEntry(m_creature, NPC_ANCIENT_CITIZEN_F, 40.0f))
									{
										DoScriptText(AC_VRYKUL_SAY_2, pFemaleVrykul);
									}
								uiPhase++; uiSpeechTimer = 5000; break;
                        case 2: if(Creature *pFemaleVrykul= GetClosestCreatureWithEntry(m_creature, NPC_ANCIENT_CITIZEN_M, 40.0f))
									{
										DoScriptText(AC_VRYKUL_SAY_3, pFemaleVrykul);
									}
								uiPhase++; uiSpeechTimer = 5000; break;
						case 3: if(Creature *pFemaleVrykul= GetClosestCreatureWithEntry(m_creature, NPC_ANCIENT_CITIZEN_F, 40.0f))
									{
										DoScriptText(AC_VRYKUL_SAY_4, pFemaleVrykul);
									}
								uiPhase++; uiSpeechTimer = 5000; break;
                        case 4: if(Creature *pFemaleVrykul= GetClosestCreatureWithEntry(m_creature, NPC_ANCIENT_CITIZEN_M, 40.0f))
									{
										DoScriptText(AC_VRYKUL_SAY_5, pFemaleVrykul);
									}
								uiPhase++; uiSpeechTimer = 5000; break;
						case 5: if(Creature *pFemaleVrykul= GetClosestCreatureWithEntry(m_creature, NPC_ANCIENT_CITIZEN_F, 40.0f))
									{
										DoScriptText(AC_VRYKUL_SAY_6, pFemaleVrykul);
									}
								uiPhase++; uiSpeechTimer = 5000; break;
                        case 6: if(Creature *pFemaleVrykul= GetClosestCreatureWithEntry(m_creature, NPC_ANCIENT_CITIZEN_M, 40.0f))
									{
										DoScriptText(AC_VRYKUL_SAY_7, pFemaleVrykul);
									}
								uiPhase++; uiSpeechTimer = 5000; break;
                        case 7: DoScriptText(KING_YMIRON_SAY_1, m_creature); uiPhase++; uiSpeechTimer = 2000; break;
                        case 8: if(Creature *pFemaleVrykul= GetClosestCreatureWithEntry(m_creature, NPC_ANCIENT_CITIZEN_F, 40.0f))
									{
										DoScriptText(AC_VRYKUL_SAY_8, pFemaleVrykul);
									}
								uiPhase++; uiSpeechTimer = 5000; break;
                        case 9: DoScriptText(KING_YMIRON_SAY_2, m_creature); uiPhase++; uiSpeechTimer = 5000; break;
                        case 10: DoScriptText(TEXT_EMOTE_1, m_creature); uiPhase++; uiSpeechTimer = 5000; break;
                        case 11: DoScriptText(KING_YMIRON_SAY_3, m_creature); uiPhase++; uiSpeechTimer = 5000; break;
                        case 12: DoScriptText(KING_YMIRON_SAY_4, m_creature); uiPhase++; uiSpeechTimer = 5000; break;
                        case 13: DoScriptText(KING_YMIRON_SAY_5, m_creature); uiPhase++; uiSpeechTimer = 5000; break;
                        case 14: DoScriptText(KING_YMIRON_SAY_6, m_creature); uiPhase++; uiSpeechTimer = 5000; break;
                        case 15: DoScriptText(KING_YMIRON_SAY_7, m_creature); uiPhase++; uiSpeechTimer = 2000; break;
                        case 16: DoScriptText(TEXT_EMOTE_2, m_creature); uiPhase++; uiSpeechTimer = 2000; break;
                        case 17: DoScriptText(KING_YMIRON_SAY_8, m_creature); uiPhase++; uiSpeechTimer = 2000; break;
                        case 18: DoScriptText(TEXT_EMOTE_3, m_creature); uiPhase++; uiSpeechTimer = 2000; break;
                        case 19: DoScriptText(KING_YMIRON_SAY_9, m_creature); uiPhase++; uiSpeechTimer = 5000; break;
                        case 20: { DoScriptText(KING_YMIRON_SAY_10, m_creature);
								    if (Player *pPlayer = m_creature->GetMap()->GetPlayer(uiPlayerGUID))
									{
										pPlayer->AreaExploredOrEventHappens(QUEST_ANGUISH_NIFFLEVAR);
								    }
									uiPhase = 0; 
									bEventStarted = false;
									m_creature->ForcedDespawn(1000);
									break;
								}

					}

				} else uiSpeechTimer -= uiDiff;

			}

        }
};



CreatureAI* GetAI_npc_king_ymiron(Creature* pCreature)
{
    return new npc_king_ymironAI(pCreature);
}

/*######
## npc_feknut_bunny
######*/

enum
{
    NPC_DARKCLAW_BAT        = 23959,
    SPELL_SUMMON_GUANO      = 43307

};

struct MANGOS_DLL_DECL npc_feknut_bunnyAI : public ScriptedAI
{
    npc_feknut_bunnyAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }
    
     
	 uint64 uiCheckTimer;
     bool bChecked;

     void Reset()
     {
		 uiCheckTimer = 1000;
         bChecked = false;
     }
         
     void UpdateAI(const uint32 uiDiff)
        {
            if(!bChecked)
            {
                if (uiCheckTimer <= uiDiff)
                {	
                    if(Creature *pBat = GetClosestCreatureWithEntry(m_creature, NPC_DARKCLAW_BAT, 35.0f))
                    {
                        if(pBat->isAlive())
                            pBat->CastSpell(m_creature, SPELL_SUMMON_GUANO, false);
                        bChecked = true;
                        if(Player *pPlayer = m_creature->GetMap()->GetPlayer(m_creature->GetOwnerGuid()))
                        {
                            pBat->Attack(pPlayer, true);
                            pBat->AddThreat(pPlayer, 999.9f, true);
                            pBat->GetMotionMaster()->MoveChase(pPlayer);

                        }                            
                    }        	   
                } else uiCheckTimer -= uiDiff;
            }			
        }
};

CreatureAI* GetAI_npc_feknut_bunny(Creature* pCreature)
{
    return new npc_feknut_bunnyAI(pCreature);
}

void AddSC_howling_fjord()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "npc_ancient_male_vrykul";
    pNewScript->GetAI = &GetAI_npc_ancient_male_vrykul;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "at_ancient_male_vrykul";
    pNewScript->pAreaTrigger = &AreaTrigger_at_ancient_male_vrykul;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_daegarn";
    pNewScript->GetAI = &GetAI_npc_daegarn;
    pNewScript->pQuestAcceptNPC = &QuestAccept_npc_daegarn;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_deathstalker_razael";
    pNewScript->pGossipHello = &GossipHello_npc_deathstalker_razael;
    pNewScript->pGossipSelect = &GossipSelect_npc_deathstalker_razael;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_dark_ranger_lyana";
    pNewScript->pGossipHello = &GossipHello_npc_dark_ranger_lyana;
    pNewScript->pGossipSelect = &GossipSelect_npc_dark_ranger_lyana;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_greer_orehammer";
    pNewScript->pGossipHello = &GossipHello_npc_greer_orehammer;
    pNewScript->pGossipSelect = &GossipSelect_npc_greer_orehammer;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_silvermoon_harry";
    pNewScript->GetAI = &GetAI_npc_silvermoon_harry;
    pNewScript->pGossipHello = &GossipHello_npc_silvermoon_harry;
    pNewScript->pGossipSelect = &GossipSelect_npc_silvermoon_harry;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_banner";
    pNewScript->GetAI = &GetAI_npc_banner;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_king_ymiron";
    pNewScript->GetAI = &GetAI_npc_king_ymiron;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_feknut_bunny";
    pNewScript->GetAI = &GetAI_npc_feknut_bunny;
    pNewScript->RegisterSelf();
}
