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
SDName: Blades_Edge_Mountains
SD%Complete: 90
SDComment: Quest support: 10503, 10504, 10556, 10609, 10682, 10980. Ogri'la->Skettis Flight. (npc_daranelle needs bit more work before consider complete)
SDCategory: Blade's Edge Mountains
EndScriptData */

/* ContentData
mobs_bladespire_ogre
mobs_nether_drake
npc_daranelle
npc_overseer_nuaar
npc_saikkal_the_elder
npc_skyguard_handler_irena
EndContentData */

#include "precompiled.h"

/*######
## mobs_bladespire_ogre
######*/

//TODO: add support for quest 10512 + creature abilities
struct MANGOS_DLL_DECL mobs_bladespire_ogreAI : public ScriptedAI
{
    mobs_bladespire_ogreAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    void Reset() { }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mobs_bladespire_ogre(Creature* pCreature)
{
    return new mobs_bladespire_ogreAI(pCreature);
}

/*######
## mobs_nether_drake
######*/

#define SAY_NIHIL_1                 -1000169
#define SAY_NIHIL_2                 -1000170
#define SAY_NIHIL_3                 -1000171
#define SAY_NIHIL_4                 -1000172
#define SAY_NIHIL_INTERRUPT         -1000173

#define ENTRY_WHELP                 20021
#define ENTRY_PROTO                 21821
#define ENTRY_ADOLE                 21817
#define ENTRY_MATUR                 21820
#define ENTRY_NIHIL                 21823

#define SPELL_T_PHASE_MODULATOR     37573

#define SPELL_ARCANE_BLAST          38881
#define SPELL_MANA_BURN             38884
#define SPELL_INTANGIBLE_PRESENCE   36513

struct MANGOS_DLL_DECL mobs_nether_drakeAI : public ScriptedAI
{
    mobs_nether_drakeAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    bool IsNihil;
    uint32 NihilSpeech_Timer;
    uint32 NihilSpeech_Phase;

    uint32 ArcaneBlast_Timer;
    uint32 ManaBurn_Timer;
    uint32 IntangiblePresence_Timer;

    void Reset()
    {
        IsNihil = false;
        NihilSpeech_Timer = 3000;
        NihilSpeech_Phase = 0;

        ArcaneBlast_Timer = 7500;
        ManaBurn_Timer = 10000;
        IntangiblePresence_Timer = 15000;
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
            return;

        ScriptedAI::MoveInLineOfSight(who);
    }

    //in case creature was not summoned (not expected)
    void MovementInform(uint32 type, uint32 id)
    {
        if (type != POINT_MOTION_TYPE)
            return;

        if (id == 0)
        {
            m_creature->SetDeathState(JUST_DIED);
            m_creature->RemoveCorpse();
            m_creature->SetHealth(0);
        }
    }

    void SpellHit(Unit *caster, const SpellEntry *spell)
    {
        if (spell->Id == SPELL_T_PHASE_MODULATOR && caster->GetTypeId() == TYPEID_PLAYER)
        {
            const uint32 entry_list[4] = {ENTRY_PROTO, ENTRY_ADOLE, ENTRY_MATUR, ENTRY_NIHIL};
            int cid = rand()%(4-1);

            if (entry_list[cid] == m_creature->GetEntry())
                ++cid;

            //we are nihil, so say before transform
            if (m_creature->GetEntry() == ENTRY_NIHIL)
            {
                DoScriptText(SAY_NIHIL_INTERRUPT, m_creature);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                IsNihil = false;
            }

            if (m_creature->UpdateEntry(entry_list[cid]))
            {
                if (entry_list[cid] == ENTRY_NIHIL)
                {
                    EnterEvadeMode();
                    m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    IsNihil = true;
                }else
                    AttackStart(caster);
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (IsNihil)
        {
            if (NihilSpeech_Timer <= diff)
            {
                switch(NihilSpeech_Phase)
                {
                    case 0:
                        DoScriptText(SAY_NIHIL_1, m_creature);
                        ++NihilSpeech_Phase;
                        break;
                    case 1:
                        DoScriptText(SAY_NIHIL_2, m_creature);
                        ++NihilSpeech_Phase;
                        break;
                    case 2:
                        DoScriptText(SAY_NIHIL_3, m_creature);
                        ++NihilSpeech_Phase;
                        break;
                    case 3:
                        DoScriptText(SAY_NIHIL_4, m_creature);
                        ++NihilSpeech_Phase;
                        break;
                    case 4:
                        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        //take off to location above
                        m_creature->GetMotionMaster()->MovePoint(0, m_creature->GetPositionX()+50.0f, m_creature->GetPositionY(), m_creature->GetPositionZ()+50.0f);
                        ++NihilSpeech_Phase;
                        break;
                }
                NihilSpeech_Timer = 5000;
            }else NihilSpeech_Timer -=diff;

            //anything below here is not interesting for Nihil, so skip it
            return;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (IntangiblePresence_Timer <= diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_INTANGIBLE_PRESENCE);
            IntangiblePresence_Timer = urand(15000, 30000);
        }else IntangiblePresence_Timer -= diff;

        if (ManaBurn_Timer <= diff)
        {
            Unit* target = m_creature->getVictim();
            if (target && target->getPowerType() == POWER_MANA)
                DoCastSpellIfCan(target,SPELL_MANA_BURN);
            ManaBurn_Timer = urand(8000, 16000);
        }else ManaBurn_Timer -= diff;

        if (ArcaneBlast_Timer <= diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(),SPELL_ARCANE_BLAST);
            ArcaneBlast_Timer = urand(2500, 7500);
        }else ArcaneBlast_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mobs_nether_drake(Creature* pCreature)
{
    return new mobs_nether_drakeAI(pCreature);
}

/*######
## npc_daranelle
######*/

enum
{
    SAY_SPELL_INFLUENCE     = -1000174,
    NPC_KALIRI_AURA_DISPEL  = 21511,
    SPELL_LASHHAN_CHANNEL   = 36904
};

struct MANGOS_DLL_DECL npc_daranelleAI : public ScriptedAI
{
    npc_daranelleAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    void Reset() { }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (pWho->GetTypeId() == TYPEID_PLAYER)
        {
            if (pWho->HasAura(SPELL_LASHHAN_CHANNEL, EFFECT_INDEX_0) && m_creature->IsWithinDistInMap(pWho, 10.0f))
            {
                DoScriptText(SAY_SPELL_INFLUENCE, m_creature, pWho);

                //TODO: Move the below to updateAI and run if this statement == true
                ((Player*)pWho)->KilledMonsterCredit(NPC_KALIRI_AURA_DISPEL, m_creature->GetObjectGuid());
                pWho->RemoveAurasDueToSpell(SPELL_LASHHAN_CHANNEL);
            }
        }

        ScriptedAI::MoveInLineOfSight(pWho);
    }
};

CreatureAI* GetAI_npc_daranelle(Creature* pCreature)
{
    return new npc_daranelleAI(pCreature);
}

/*######
## npc_overseer_nuaar
######*/

bool GossipHello_npc_overseer_nuaar(Player* pPlayer, Creature* pCreature)
{
    if (pPlayer->GetQuestStatus(10682) == QUEST_STATUS_INCOMPLETE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Overseer, I am here to negotiate on behalf of the Cenarion Expedition.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    pPlayer->SEND_GOSSIP_MENU(10532, pCreature->GetGUID());

    return true;
}

bool GossipSelect_npc_overseer_nuaar(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        pPlayer->SEND_GOSSIP_MENU(10533, pCreature->GetGUID());
        pPlayer->AreaExploredOrEventHappens(10682);
    }
    return true;
}

/*######
## npc_saikkal_the_elder
######*/

bool GossipHello_npc_saikkal_the_elder(Player* pPlayer, Creature* pCreature)
{
    if (pPlayer->GetQuestStatus(10980) == QUEST_STATUS_INCOMPLETE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Yes... yes, it's me.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    pPlayer->SEND_GOSSIP_MENU(10794, pCreature->GetGUID());

    return true;
}

bool GossipSelect_npc_saikkal_the_elder(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    switch(uiAction)
    {
        case GOSSIP_ACTION_INFO_DEF+1:
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Yes elder. Tell me more of the book.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
            pPlayer->SEND_GOSSIP_MENU(10795, pCreature->GetGUID());
            break;
        case GOSSIP_ACTION_INFO_DEF+2:
            pPlayer->TalkedToCreature(pCreature->GetEntry(), pCreature->GetGUID());
            pPlayer->SEND_GOSSIP_MENU(10796, pCreature->GetGUID());
            break;
    }
    return true;
}

/*######
## npc_skyguard_handler_irena
######*/

#define GOSSIP_SKYGUARD "Fly me to Skettis please"

bool GossipHello_npc_skyguard_handler_irena(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver())
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());

    if (pPlayer->GetReputationRank(1031) >= REP_HONORED)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_SKYGUARD, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());

    return true;
}

bool GossipSelect_npc_skyguard_handler_irena(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pPlayer->CastSpell(pPlayer,41278,true);               //TaxiPath 706
    }
    return true;
}

/*######
## Test Flight quest chain
######*/

//#define GOSSIP_MORE "I want to fly to an old location!"
#define GOSSIP_RIDGE "Take me to Singing Ridge."
#define GOSSIP_ZEPHYRIUM "I'm ready for my test flight!"
#define GOSSIP_RUUAN "Take me to Ruuan Weald."
#define GOSSIP_RAZZAN "Take me to Razaan's Landing."

enum
{
    QUEST_ZEPHYRIUM = 10557,
    QUEST_RIDGE     = 10710,
    QUEST_RUUAN     = 10712,
    QUEST_RAZZAN    = 10711,

    ITEM_WAIVER = 30539,

    SPELL_ZEPHYRIUM = 37910,
    SPELL_RIDGE = 37962,
    SPELL_RUUAN = 37968,
    SPELL_RAZZAN = 36812,

    //Require spell_script_target
    SPELL_CHANNELED = 36795,

    SPELL_CANNONCHARGE_SELF = 36860,
    SPELL_2STATE = 36790,
    SPELL_3STATE = 36792,
    SPELL_4STATE = 36800,

    SPELL_CANNONCHARGE01_PORT = 36801,
    SPELL_CANNONCHARGE02_PLATFORM = 36785,

    SPELL_TELEPORT = 37908,
    SPELL_SOARING = 37108,

    NPC_CHANNELER = 21393,
    NPC_CHARGE = 21413
};

struct MANGOS_DLL_DECL mob_cannon_chanellerAI : public Scripted_NoMovementAI
{
    mob_cannon_chanellerAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature) { Reset(); }

    bool IsRunning;
    bool Test;
    bool Ridge;
    bool Razzan;
    bool Ruuan;
    uint64 m_uiPLAYERGUID;
    uint32 m_uiLaunchTimer;
    uint32 m_uiPhaseTimer;
    uint8 Phase;

    void Reset()
    {
        Test      = false;
        Ridge     = false;
        Razzan    = false;
        Ruuan     = false;
        IsRunning = false;
        m_uiPLAYERGUID  = 0;
        m_uiLaunchTimer = 12000;
        m_uiPhaseTimer  = 0;
        Phase = 0;

        if(Creature* pCharge = GetClosestCreatureWithEntry(m_creature, NPC_CHARGE, 50.0f))
        {
            pCharge->ForcedDespawn();
        }
    }

    void StartChanneling(Player* pPlayer)
    {
        if(pPlayer && pPlayer->isAlive())
        {
            m_uiPLAYERGUID = pPlayer->GetGUID();
            Player* pPlayer = m_creature->GetMap()->GetPlayer(m_uiPLAYERGUID);
            if(!IsRunning)
            {
                if(Creature* pCharge = GetClosestCreatureWithEntry(m_creature, NPC_CHARGE, 100.0f))
                    IsRunning = true;
            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if(!IsRunning)
            return;

        if(m_uiPhaseTimer < diff)
        {
           if(Creature* pCharge = GetClosestCreatureWithEntry(m_creature, NPC_CHARGE, 50.0f))
           {
               switch(Phase)
               {
                    case 0:
                        m_creature->CastSpell(pCharge, SPELL_CHANNELED, true);
                        pCharge->CastSpell(pCharge, SPELL_CANNONCHARGE_SELF, true);
                        Phase = 1;
                        m_uiPhaseTimer = 3000;
                        break;
                    case 1:
                        pCharge->CastSpell(pCharge, SPELL_2STATE, true);
                        Phase = 2;
                        m_uiPhaseTimer = 3000;
                        break;
                    case 2:
                        pCharge->CastSpell(pCharge, SPELL_3STATE, true);
                        Phase = 3;
                        m_uiPhaseTimer = 3000;
                        break;
                    case 3:
                        pCharge->CastSpell(pCharge, SPELL_4STATE, true);
                        Phase = 4;
                        m_uiPhaseTimer = 3000;
                        break;
                    case 4:
                        IsRunning = false;
                        Phase = 0;
                        break;
                    default: break;
                }
            }
        } else m_uiPhaseTimer -= diff;

        if(m_uiLaunchTimer <= diff)
        {
            Player* pPlayer = m_creature->GetMap()->GetPlayer(m_uiPLAYERGUID);
            if(Test && pPlayer)
            {
                pPlayer->SetOrientation(5.10f);
                pPlayer->CastSpell(pPlayer, SPELL_ZEPHYRIUM, true);
            }

            if(Ridge && pPlayer)
            {
                pPlayer->SetOrientation(1.91f);
                pPlayer->CastSpell(pPlayer, SPELL_RIDGE, true);
            }

            if(Razzan && pPlayer)
            {
                pPlayer->SetOrientation(2.52f);
                pPlayer->CastSpell(pPlayer, SPELL_RAZZAN, true);
            }

            if(Ruuan && pPlayer)
            {
                pPlayer->SetOrientation(3.02f);
                pPlayer->CastSpell(pPlayer, SPELL_RUUAN, true);
            }
            Reset();
        }
        else m_uiLaunchTimer -= diff;
    }
};

CreatureAI* GetAI_mob_cannon_chaneller(Creature* pCreature)
{
    return new mob_cannon_chanellerAI(pCreature);
}

bool GossipHello_npc_tally(Player* pPlayer, Creature* pCreature)
{
    if (Creature* pCannoner = GetClosestCreatureWithEntry(pPlayer, NPC_CHANNELER, 100.0f))
    {
        if (mob_cannon_chanellerAI* CannonerAI = dynamic_cast<mob_cannon_chanellerAI*>(pCannoner->AI()))
        {
            if(!CannonerAI->IsRunning)
            {
                if((pPlayer->GetQuestStatus(QUEST_ZEPHYRIUM) == QUEST_STATUS_INCOMPLETE) && !pPlayer->HasAura(SPELL_SOARING))
                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ZEPHYRIUM, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
                    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());

                if((pPlayer->GetQuestStatus(QUEST_RIDGE) == QUEST_STATUS_INCOMPLETE) && pPlayer->GetItemByEntry(ITEM_WAIVER) && !pPlayer->HasAura(SPELL_SOARING))
                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_RIDGE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);
                    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());

                if((pPlayer->GetQuestStatus(QUEST_RAZZAN) == QUEST_STATUS_INCOMPLETE) && !pPlayer->HasAura(SPELL_SOARING))
                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_RAZZAN, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+4);
                    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());

                if((pPlayer->GetQuestStatus(QUEST_RUUAN) == QUEST_STATUS_INCOMPLETE) && !pPlayer->HasAura(SPELL_SOARING))
                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_RUUAN, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+3);
                    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
                return true;
           }
       }
   }
   return false;
}

bool GossipSelect_npc_tally(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pPlayer->CastSpell(pPlayer, SPELL_TELEPORT, true);
        pPlayer->CastSpell(pPlayer, SPELL_CANNONCHARGE01_PORT, true);
        //pPlayer->TeleportTo(530, 1920.022f, 5581.916f, 270.2f, 5.25f, 0);
        if (Creature* pCannoner = GetClosestCreatureWithEntry(pPlayer, NPC_CHANNELER, 100.0f))
            if (mob_cannon_chanellerAI* CannonerAI = dynamic_cast<mob_cannon_chanellerAI*>(pCannoner->AI()))
            {
                CannonerAI->StartChanneling(pPlayer);
                CannonerAI->Test = true;
            }
    }
    if(uiAction == GOSSIP_ACTION_INFO_DEF+2)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pPlayer->CastSpell(pPlayer, SPELL_TELEPORT, true);
        pPlayer->CastSpell(pPlayer, SPELL_CANNONCHARGE01_PORT, true);
        //pPlayer->TeleportTo(530, 1920.022f, 5581.916f, 270.2f, 5.25f, 0);
        if (Creature* pCannoner = GetClosestCreatureWithEntry(pPlayer, NPC_CHANNELER, 100.0f))
            if (mob_cannon_chanellerAI* CannonerAI = dynamic_cast<mob_cannon_chanellerAI*>(pCannoner->AI()))
            {
                CannonerAI->StartChanneling(pPlayer);
                CannonerAI->Ridge = true;
            }
    }
    if(uiAction == GOSSIP_ACTION_INFO_DEF+3)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pPlayer->CastSpell(pPlayer, SPELL_TELEPORT, true);
        pPlayer->CastSpell(pPlayer, SPELL_CANNONCHARGE01_PORT, true);
        //pPlayer->TeleportTo(530, 1920.022f, 5581.916f, 270.2f, 5.25f, 0);
        if (Creature* pCannoner = GetClosestCreatureWithEntry(pPlayer, NPC_CHANNELER, 100.0f))
            if (mob_cannon_chanellerAI* CannonerAI = dynamic_cast<mob_cannon_chanellerAI*>(pCannoner->AI()))
            {
                CannonerAI->StartChanneling(pPlayer);
                CannonerAI->Ruuan = true;
            }
    }
    if(uiAction == GOSSIP_ACTION_INFO_DEF+4)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pPlayer->CastSpell(pPlayer, SPELL_TELEPORT, true);
        pPlayer->CastSpell(pPlayer, SPELL_CANNONCHARGE01_PORT, true);
        //pPlayer->TeleportTo(530, 1920.022f, 5581.916f, 270.2f, 5.25f, 0);
        if (Creature* pCannoner = GetClosestCreatureWithEntry(pPlayer, NPC_CHANNELER, 100.0f))
            if (mob_cannon_chanellerAI* CannonerAI = dynamic_cast<mob_cannon_chanellerAI*>(pCannoner->AI()))
            {
                CannonerAI->StartChanneling(pPlayer);
                CannonerAI->Razzan = true;
            }
    }
    return true;
}

/*######
## Wrangle Some Aether Rays!
######*/

enum
{
    NPC_AETHERRAY        = 22181,
    NPC_AETHERRAYF       = 23343,

    SPELL_WRANGLING_ROPE = 40856,

    AETHER_RAY_EMOTE     = -1522181
};

struct MANGOS_DLL_DECL npc_AetherRayAI : public ScriptedAI
{
    npc_AetherRayAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    bool m_bDoEmote;
    bool m_bCanBeWrangled;
    ObjectGuid m_playerGuid;
    uint32 m_uiWrangleTimer;

    void Reset()
    {
        m_bDoEmote = false;
        m_bCanBeWrangled = false;
        m_uiWrangleTimer = 5000;
    }
    void UpdateAI(const uint32 uiDiff)
    {
        if(!m_bDoEmote && (m_creature->GetHealthPercent() < 40.0f))
        {
            DoScriptText(AETHER_RAY_EMOTE, m_creature);
            m_bDoEmote = true;
        }

        if(m_bCanBeWrangled)
        {
            if(m_uiWrangleTimer < uiDiff)
            {
                if(Player* pPlayer = m_creature->GetMap()->GetPlayer(m_playerGuid))
                {
                    pPlayer->KilledMonsterCredit(NPC_AETHERRAYF);
                    m_creature->UpdateEntry(NPC_AETHERRAYF);
                    m_creature->DeleteThreatList();
                    m_creature->CombatStop(true);
                    m_bCanBeWrangled = false;
                    m_creature->ForcedDespawn(140000);
                    m_uiWrangleTimer = 5000;
                }
            } else m_uiWrangleTimer -= uiDiff;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_AetherRay(Creature* pCreature)
{
    return new npc_AetherRayAI(pCreature);
}

bool EffectAuraDummy_spell_aura_WranglingRay(const Aura* pAura, bool bApply)
{
    if(pAura->GetId() == SPELL_WRANGLING_ROPE)
    {
        if(bApply)
        {
            if (Creature* pRay = GetClosestCreatureWithEntry(pAura->GetCaster(), NPC_AETHERRAY, 100.0f))
            {
                if(pRay->GetHealthPercent() < 40.0f)
                {
                    if (npc_AetherRayAI* pRayAI = dynamic_cast<npc_AetherRayAI*>(pRay->AI()))
                    {
                        pRayAI->m_bCanBeWrangled = true;
                        pRayAI->m_playerGuid = pAura->GetCasterGuid();
                        return true;
                    }
                }
                return false;
            }
        }
    }
    return false;
};

/*######
## AddSC
######*/

void AddSC_blades_edge_mountains()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "mobs_bladespire_ogre";
    newscript->GetAI = &GetAI_mobs_bladespire_ogre;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mobs_nether_drake";
    newscript->GetAI = &GetAI_mobs_nether_drake;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_daranelle";
    newscript->GetAI = &GetAI_npc_daranelle;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_overseer_nuaar";
    newscript->pGossipHello = &GossipHello_npc_overseer_nuaar;
    newscript->pGossipSelect = &GossipSelect_npc_overseer_nuaar;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_saikkal_the_elder";
    newscript->pGossipHello = &GossipHello_npc_saikkal_the_elder;
    newscript->pGossipSelect = &GossipSelect_npc_saikkal_the_elder;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_skyguard_handler_irena";
    newscript->pGossipHello =  &GossipHello_npc_skyguard_handler_irena;
    newscript->pGossipSelect = &GossipSelect_npc_skyguard_handler_irena;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_rally";
    newscript->pGossipHello = &GossipHello_npc_tally;
    newscript->pGossipSelect = &GossipSelect_npc_tally;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_channeler";
    newscript->GetAI = &GetAI_mob_cannon_chaneller;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_AetherRay";
    newscript->GetAI = &GetAI_npc_AetherRay;
    newscript->pEffectAuraDummy = EffectAuraDummy_spell_aura_WranglingRay;
    newscript->RegisterSelf();
}
