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
SDName: Zuldrak
SD%Complete: 100
SDComment: Quest support: 12934.
SDCategory: Zuldrak
EndScriptData */

/* ContentData
npc_gurgthock
EndContentData */

#include "precompiled.h"
#include "follower_ai.h"
#include "Vehicle.h"

/*######
## npc_gurgthock
######*/

enum
{
    QUEST_FROM_BEYOND = 12934,

    NPC_AZBARIN       = 30026,
    NPC_DUKE_SINGEN   = 30019,
    NPC_ERATHIUS      = 30025,
    NPC_GARGORAL      = 30024
};

static float m_afSpawnLocation[] = {5768.71f, -2969.29f, 273.816f};
static uint32 m_auiBosses[] = {NPC_AZBARIN, NPC_DUKE_SINGEN, NPC_ERATHIUS, NPC_GARGORAL};

struct MANGOS_DLL_DECL npc_gurgthockAI : public ScriptedAI
{
    npc_gurgthockAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    uint64 m_uiPlayerGUID;

    void SetPlayerGUID(uint64 uiPlayerGUID) { m_uiPlayerGUID = uiPlayerGUID; }

    void Reset()
    {
        m_uiPlayerGUID = 0;
    }

    void SummonedCreatureJustDied(Creature* pSummoned)
    {
        uint32 uiEntry = pSummoned->GetEntry();
        for(uint8 i = 0; i < 4; ++i)
        {
            if (uiEntry == m_auiBosses[i])
            {
                if (Player* pPlayer = m_creature->GetMap()->GetPlayer(m_uiPlayerGUID))
                    pPlayer->GroupEventHappens(QUEST_FROM_BEYOND, m_creature);

                m_uiPlayerGUID = 0;
                return;
            }
        }
    }
};

bool QuestAccept_npc_gurgthock(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    if (pQuest->GetQuestId() == QUEST_FROM_BEYOND)
    {
        pCreature->SummonCreature(m_auiBosses[urand(0, 3)], m_afSpawnLocation[0], m_afSpawnLocation[1], m_afSpawnLocation[2], 0.0f, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 600000);

        if (npc_gurgthockAI* pGurthockAI = dynamic_cast<npc_gurgthockAI*>(pCreature->AI()))
            pGurthockAI->SetPlayerGUID(pPlayer->GetGUID());
    }
    return true;
}

CreatureAI* GetAI_npc_gurgthock(Creature* pCreature)
{
    return new npc_gurgthockAI(pCreature);
}

/*###################
# Quest: Leave no one behind
###################*/

enum Crusaders
{
    NPC_LAMOOF                          = 28142,
    NPC_LAMOOF_TRIGGER                  = 28141,
    NPC_JONATHAN                        = 28136,
    NPC_JONATHAN_TRIGGER                = 28133,
    NPC_JOSEPHINE_TRIGGER               = 28143,
    NPC_JOSEPHINE                       = 28148,
    NPC_DR_ROGERS                       = 28125,

    // forcing cast summon spell spells
    SPELL_SUMMON_LAMOOF                 = 50673,
    SPELL_SUMMON_JONATHAN               = 50663,
    SPELL_SUMMON_JOSEPHINE              = 50691,

    SPELL_DYING_LAMOOF                  = 50679,
    SPELL_DYING_JONATHAN                = 50666,
    SPELL_DYING_JOSEPHINE               = 50693,

    SPELL_KILLCREDIT_LAMOOF             = 50684,
    SPELL_KILLCREDIT_JONATHAN           = 50680,
    SPELL_KILLCREDIT_JOSEPHINE          = 50699,

    // unfortunately found only for LAMOOF :(
    SAY_LAMOOF_MOAN                     = -1999855,

    SAY_LAMOOF_QUESTCREDIT              = -1999854,
    SAY_JONATHAN_QUESTCREDIT            = -1999853,
    SAY_JOSEPHINE_QUESTCREDIT           = -1999852,

    QUEST_LEAVENOONEBEHID               = 12512
};

struct MANGOS_DLL_DECL mob_crusaderAI : public FollowerAI
{
    mob_crusaderAI(Creature* pCreature) : FollowerAI(pCreature)
    {
        bBleeding = false;
        bReady = true;
        bApplied = false;
        Reset();
    }

    bool bBleeding;
    bool bReady;
    bool bApplied;

    void Reset() {}

    void JustDied(Unit* pKiller)
    {
        Player* pFollowed = GetLeaderForFollower();
        if (!pFollowed)
            return;

        uint32 spell = 0;

        switch(m_creature->GetEntry())
        {
            case NPC_LAMOOF: spell = SPELL_DYING_LAMOOF; break;
            case NPC_JONATHAN: spell = SPELL_DYING_JONATHAN; break;
            case NPC_JOSEPHINE: spell = SPELL_DYING_JOSEPHINE; break;
            default: break;
        }

        if ((spell > 0) && pFollowed->HasAura(spell))
            pFollowed->RemoveAurasDueToSpellByCancel(spell);
    }

    void MoveInLineOfSight(Unit *pWho)
    {
        if (!pWho || !m_creature->isAlive())
            return;

        Player* pFollowed = GetLeaderForFollower();
        if (!pFollowed)
            return;

        if ((pWho->GetEntry() == NPC_DR_ROGERS) && (m_creature->GetDistance(pWho) < INTERACTION_DISTANCE))
        {
            uint32 spell = 0;
            uint32 dispell = 0;
            uint32 textid = 0;

            switch (m_creature->GetEntry())
            {
                case NPC_LAMOOF: 
                    spell = SPELL_KILLCREDIT_LAMOOF;
                    dispell = SPELL_DYING_LAMOOF;
                    textid = SAY_LAMOOF_QUESTCREDIT;
                    break;
                case NPC_JONATHAN: 
                    spell = SPELL_KILLCREDIT_JONATHAN;
                    dispell = SPELL_DYING_JONATHAN;
                    textid = SAY_JONATHAN_QUESTCREDIT;
                    break;
                case NPC_JOSEPHINE: 
                    spell = SPELL_KILLCREDIT_JOSEPHINE;
                    dispell = SPELL_DYING_JOSEPHINE;
                    textid = SAY_JOSEPHINE_QUESTCREDIT;
                    break;
                default: break;
            }

            if ((spell > 0) && (dispell > 0))
            {
                if (pFollowed->HasAura(dispell))
                {
                    pFollowed->InterruptNonMeleeSpells(false);
                    pFollowed->RemoveAurasDueToSpell(dispell);
                }
                if (m_creature->HasAura(dispell))
                    m_creature->RemoveAurasDueToSpell(dispell);
                pFollowed->CastSpell(pFollowed, spell, true);
                DoScriptText(textid, m_creature);
            }
            SetFollowComplete(true);
        }
    }

    void UpdateFollowerAI(const uint32 uiDiff)
    {
        if (m_creature->isInCombat() || m_creature->SelectHostileTarget())
            return;

        if (bBleeding & !bApplied)
        {
            if (Player* pFollowed = GetLeaderForFollower())
            {
                switch(m_creature->GetEntry())
                {
                    case NPC_LAMOOF: 
                        DoCastSpellIfCan(pFollowed, SPELL_DYING_LAMOOF, CAST_TRIGGERED);
                        DoCastSpellIfCan(m_creature, SPELL_DYING_LAMOOF, CAST_TRIGGERED);
                        break;
                    case NPC_JONATHAN: 
                        DoCastSpellIfCan(pFollowed, SPELL_DYING_JONATHAN, CAST_TRIGGERED);
                        DoCastSpellIfCan(m_creature, SPELL_DYING_JONATHAN, CAST_TRIGGERED);
                        break;
                    case NPC_JOSEPHINE: 
                        DoCastSpellIfCan(pFollowed, SPELL_DYING_JOSEPHINE, CAST_TRIGGERED);
                        DoCastSpellIfCan(m_creature, SPELL_DYING_JOSEPHINE, CAST_TRIGGERED);
                        break;
                }
            }
            bApplied = true;
        }
    }
};
CreatureAI* GetAI_mob_crusader(Creature* pCreature)
{
    return new mob_crusaderAI(pCreature);
}

struct MANGOS_DLL_DECL mob_crusader_triggerAI : public FollowerAI
{
    mob_crusader_triggerAI(Creature* pCreature) : FollowerAI(pCreature)
    {Reset();}

    uint32 m_uiMoan_Timer;

    void Reset() 
    {
        m_uiMoan_Timer = 45000;
    }

    void SpellHit(Unit *pCaster, const SpellEntry *pSpell)
    {
        if (!pCaster || !pSpell || (pCaster->GetTypeId() != TYPEID_PLAYER))
            return;
        
        uint32 spell;
        uint32 summon;
        float range = 15.0f; // summoning spell range

        switch(m_creature->GetEntry())
        {
            case NPC_LAMOOF_TRIGGER:
                spell = SPELL_SUMMON_LAMOOF;
                summon = NPC_LAMOOF;
                break;
            case NPC_JONATHAN_TRIGGER:
                spell = SPELL_SUMMON_JONATHAN;
                summon = NPC_JONATHAN;
                break;
            case NPC_JOSEPHINE_TRIGGER:
                spell = SPELL_SUMMON_JOSEPHINE;
                summon = NPC_JOSEPHINE;
                break;
            default: break;
        }
        DoCastSpellIfCan(pCaster, spell, CAST_TRIGGERED);
        if (Creature* pSummoned = GetClosestCreatureWithEntry(pCaster, summon, range))
            if (pSummoned->AI())
                if (mob_crusaderAI* pFollowerAI = dynamic_cast<mob_crusaderAI*>(pSummoned->AI()))
                    if (pFollowerAI->bReady)
                    {
                        pFollowerAI->StartFollow(((Player*)pCaster), FACTION_ESCORT_N_FRIEND_PASSIVE);
                        pFollowerAI->bBleeding = true;
                        pFollowerAI->bReady = false;
                    }
        m_creature->ForcedDespawn();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_creature->isInCombat() || m_creature->SelectHostileTarget())
            return;

        if (m_uiMoan_Timer <= uiDiff)
        {
            switch(m_creature->GetEntry())
            {
                case NPC_LAMOOF_TRIGGER: DoScriptText(SAY_LAMOOF_MOAN, m_creature);break;
                default: break;
            }
            m_uiMoan_Timer = 45000;
        }
        else
            m_uiMoan_Timer -= uiDiff;
    }
};
CreatureAI* GetAI_mob_crusader_trigger(Creature* pCreature)
{
    return new mob_crusader_triggerAI(pCreature);
}


/*######
## npc_ghoul_feeding_bunny
######*/
enum 
{
    SPELL_ATTRACT_GHOUL     = 52037,
    POINT_NEAR_BOWL         = 0,
    SPELL_KILL_KREDIT       = 52030,
    SPELL_KC_SCRIPT_EFFECT  = 52038, // not used
    SPELL_KC                = 52039  // not used
};

struct MANGOS_DLL_DECL npc_deacying_ghoulAI : public ScriptedAI
{
    npc_deacying_ghoulAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint64 m_uiPlayerGUID;

    void Reset()
    {
        m_uiPlayerGUID = 0;
    }

    void SpellHit(Unit* pCaster, const SpellEntry* pSpell)
    {
        if (pSpell->Id == SPELL_ATTRACT_GHOUL && pCaster->GetTypeId() == TYPEID_UNIT)
        {
            if (Player* pPlayer = m_creature->GetMap()->GetPlayer(((Creature*)pCaster)->GetCreatorGuid()))
                m_uiPlayerGUID = pPlayer->GetGUID();
            SetCombatMovement(false);
            float fDestX, fDestY, fDestZ;
            pCaster->GetNearPoint(pCaster, fDestX, fDestY, fDestZ, pCaster->GetObjectBoundingRadius(), 4.0f, pCaster->GetAngle(m_creature));
            m_creature->GetMotionMaster()->Clear();
            m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
            m_creature->GetMotionMaster()->MovePoint(POINT_NEAR_BOWL, fDestX, fDestY, fDestZ);
        }
    }

    void MovementInform(uint32 uiType, uint32 uiPointId)
    {
        if (uiType != POINT_MOTION_TYPE)
            return;

        if (uiPointId == POINT_NEAR_BOWL)
        {
            m_creature->HandleEmote(EMOTE_ONESHOT_EAT);
            m_creature->ForcedDespawn(3000);
            if (Player* pPlayer = m_creature->GetMap()->GetPlayer(m_uiPlayerGUID))
                DoCastSpellIfCan(pPlayer, SPELL_KILL_KREDIT);
        }
    }
};
CreatureAI* GetAI_npc_deacying_ghoul(Creature* pCreature)
{
    return new npc_deacying_ghoulAI(pCreature);
}

/*######
## npc_gorebag
######*/

enum
{
    SAY_1               = -1999852,
    SAY_2               = -1999853,
    SAY_3               = -1999854,
    SAY_4               = -1999855,
    SAY_5               = -1999856,
    SAY_6               = -1999857,

    SPELL_KILL_CREDIT               = 52220,
    SPELL_SUMMON_GARGOYLE_VEHICLE   = 52194, // force cast 52190
    SPELL_SCOURGE_DISGUISE          = 52193, // force cast 52192
    SPELL_ABANDON_VEHICLE           = 52203,
    QUEST_REUNITED                  = 12663,
    NPC_FLYING_FIEND                = 28669,
    NPC_OVERLORD_DRAKURU            = 28717,
    MAX_NODE                        = 35
};

struct NodeData
{
    float fPosX;
    float fPosY;
    float fPosZ;
    int32 uiSay;
};

NodeData NodeInfo[MAX_NODE] =
{
    {6067.32f, -2091.65f, 435.74f},         // tarrace
    {6034.02f, -2108.75f, 439.35f},
    {5943.64f, -2194.36f, 395.83f, SAY_1},  // SAY_1
    {5389.98f, -2298.10f, 400.00f, SAY_2},  // SAY2
    {5338.34f, -2559.86f, 354.92f},
    {5572.49f, -3228.05f, 437.08f},
    {5749.56f, -3505.60f, 416.47f},         // Zim'Torga
    {5790.66f, -3675.21f, 407.28f},
    {6192.40f, -3871.92f, 502.15f},
    {6503.82f, -4182.26f, 491.74f},
    {6658.25f, -4282.02f, 496.58f},
    {6911.33f, -4325.27f, 557.03f, SAY_3},  // SAY3
    {7009.48f, -4345.52f, 549.51f},
    {7017.40f, -4407.66f, 539.99f},
    {6723.51f, -4696.85f, 558.85f},
    {6667.91f, -4693.66f, 543.37f, SAY_4},  // SAY4
    {6245.38f, -4051.44f, 512.56f},
    {6045.13f, -4067.68f, 468.06f},         // rynna
    {5592.85f, -3620.97f, 456.91f},
    {5399.42f, -3367.07f, 410.16f},
    {5212.14f, -3381.96f, 362.84f, SAY_5},  //SAY5
    {5032.75f, -3191.35f, 358.82f},
    {5016.12f, -3036.15f, 330.83f},
    {5367.02f, -2677.88f, 342.06f},
    {5430.66f, -2589.67f, 357.61f},         // The Argent Stand
    {5452.66f, -2287.13f, 357.07f},
    {5650.79f, -2150.54f, 299.41f},
    {5779.82f, -2095.38f, 276.80f},
    {6018.34f, -2082.39f, 278.90f},
    {6079.77f, -1922.40f, 369.19f, SAY_6},  // SAY6
    {5952.33f, -1899.84f, 485.97f},
    {5962.76f, -2083.12f, 469.24f},
    {6034.02f, -2108.75f, 439.35f},
    {6057.18f, -2100.97f, 429.08f},
    {6071.75f, -2087.93f, 426.97f}    
};

struct MANGOS_DLL_DECL npc_flying_fiend_vehicleAI : public ScriptedAI
{
    npc_flying_fiend_vehicleAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        Reset();
        error_log("npc_flying_fiend_vehicle initiated flightpath for player %u", m_creature->GetCreatorGuid());
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetOwnerGuid(m_creature->GetCreatorGuid());
        m_creature->SetSpeedRate(MOVE_FLIGHT, 2.0f);
        m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
        DoCastSpellIfCan(m_creature, SPELL_SCOURGE_DISGUISE);
        uiNode = 0;
        FlyToNextNode();
    }

    uint32 uiNode;

    void Reset(){}

    void FlyToNextNode()
    {
        m_creature->GetMotionMaster()->MovePoint(uiNode, NodeInfo[uiNode].fPosX, NodeInfo[uiNode].fPosY, NodeInfo[uiNode].fPosZ);
    }

    void MovementInform(uint32 uiType, uint32 uiPoint)
    {
        if (uiType != POINT_MOTION_TYPE)
            return;

        if (uiPoint == uiNode)
        {
            Unit* pPassinger = m_creature->GetMap()->GetUnit(m_creature->GetCreatorGuid());
            if (!pPassinger)
            {
                m_creature->ForcedDespawn();
                return;
            }

            if (NodeInfo[uiNode].uiSay)
                if (Creature* pOverlord = GetClosestCreatureWithEntry(m_creature, NPC_OVERLORD_DRAKURU, DEFAULT_VISIBILITY_DISTANCE))
                    DoScriptText(NodeInfo[uiNode].uiSay, pOverlord, pPassinger);

            if (++uiNode < MAX_NODE)
                FlyToNextNode();
            else
            {
                pPassinger->RemoveAurasDueToSpell(52192);                
                DoCastSpellIfCan(m_creature, SPELL_ABANDON_VEHICLE,CAST_TRIGGERED);
                DoCastSpellIfCan(m_creature, SPELL_KILL_CREDIT, CAST_TRIGGERED);
                m_creature->ForcedDespawn(3000);
            }
        }
    }

};
CreatureAI* GetAI_npc_flying_fiend_vehicle(Creature* pCreature)
{
    return new npc_flying_fiend_vehicleAI(pCreature);
}

void AddSC_zuldrak()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "npc_flying_fiend_vehicle";
    pNewScript->GetAI = &GetAI_npc_flying_fiend_vehicle;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_gurgthock";
    pNewScript->GetAI = &GetAI_npc_gurgthock;
    pNewScript->pQuestAcceptNPC = &QuestAccept_npc_gurgthock;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_crusader";
    pNewScript->GetAI = &GetAI_mob_crusader;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_crusader_trigger";
    pNewScript->GetAI = &GetAI_mob_crusader_trigger;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_deacying_ghoul";
    pNewScript->GetAI = &GetAI_npc_deacying_ghoul;
    pNewScript->RegisterSelf();
}
