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
SDName: Grizzly_Hills
SD%Complete:
SDComment: Quest support: 12138, 12198
SDCategory: Grizzly Hills
EndScriptData */

/* ContentData
npc_depleted_war_golem
EndContentData */

#include "precompiled.h"
#include "pet_ai.h"
#include "escort_ai.h"

/*######
## npc_harrison_jones_gh
######*/

enum
{
    QUEST_DUN_DA_DUN_TAH    = 12082,

    GO_HARRISON_CAGE        = 188465,
    GO_ADARRA_CAGE          = 188487,
    GO_FIRE_DOOR            = 188480,

    NPC_ADDARIA             = 24405,
    NPC_TECAHUNA            = 26865,

    SAY_ESCORT_START        = -1999850,
    SAY_BEFOR_CHAMBER       = -1999849,
    SAY_OPENING_CAGE        = -1999848,
    SAY_FREEING_ADARRA      = -1999847,
    SAY_ADARRA_THANX        = -1999846,
    SAY_AFTER_GONG_BANG     = -1999845,
    SAY_TOWARD_DOOR         = -1999844,
    SAY_DOOR_IN_FLAMES      = -1999843,
    SAY_HATE_SNAKES         = -1999842,
    SAY_ATTACK_TECAHUNA     = -1999841,
    SAY_QUEST_COMPLETED     = -1999840,

    SPELL_BANG_RITUAL_GONG  = 75313,

    // subevent
    SE_NONE                 = 0,
    SE_ADARRIA_CAGE_OPENED  = 1,
    SE_ADARRIA_ESCAPE       = 2,
    SE_FIRST_GONG_BANG      = 3,
    SE_SECOND_GONG_BANG     = 4,
    SE_LEAVE_GONG           = 5,
    SE_HATE_SNAKES          = 6,
    SE_ATTACK_TECAHUNA      = 7,
    SE_IS_TECAHUNA_DEAD     = 8,
    SE_RETURN_TO_ESCORT     = 9
};

float fTecahunaPos[4] = {4909.56f, -4820.97f, 32.58f, 2.39f};

struct MANGOS_DLL_DECL npc_harrison_jones_ghAI : public npc_escortAI
{
    npc_harrison_jones_ghAI(Creature* pCreature) : npc_escortAI(pCreature)
    {
        Reset();
        subevent = SE_NONE;
    }

    ObjectGuid m_HarrisonCageGuid;
    ObjectGuid m_AddariaGuid;
    ObjectGuid m_FireDoorGuid;
    ObjectGuid m_AddariaCageGuid;
    ObjectGuid m_TecahunaGuid;

    uint32 m_uiSubeventTimer;
    uint8 subevent;
    
    void Reset(){}

    void JustRespawned()
    {
        if (Creature* pAdarria = m_creature->GetMap()->GetCreature(m_AddariaGuid))
            if (!pAdarria->isAlive())
                pAdarria->Respawn();

        if (GameObject* pAddariaCage = m_creature->GetMap()->GetGameObject(m_AddariaCageGuid))
            if (pAddariaCage->GetGoState() == GO_STATE_ACTIVE)
                pAddariaCage->SetGoState(GO_STATE_READY);

        if (GameObject* pHarrisonCage = m_creature->GetMap()->GetGameObject(m_HarrisonCageGuid))
            if (pHarrisonCage->GetGoState() == GO_STATE_ACTIVE)
                pHarrisonCage->SetGoState(GO_STATE_READY);

        npc_escortAI::JustRespawned();
    }
    
    void JustStartedEscort()
    {
        m_creature->HandleEmoteCommand(EMOTE_ONESHOT_KICK);
        if (GameObject* pHarrisonCage = GetClosestGameObjectWithEntry(m_creature, GO_HARRISON_CAGE, INTERACTION_DISTANCE))
            if (pHarrisonCage->GetGoState() == GO_STATE_READY)
            {
                m_HarrisonCageGuid = pHarrisonCage->GetObjectGuid();
                pHarrisonCage->SetGoState(GO_STATE_ACTIVE);
            }

        if (GameObject* pFireDoor = GetClosestGameObjectWithEntry(m_creature, GO_FIRE_DOOR, DEFAULT_VISIBILITY_DISTANCE))
        {
            pFireDoor->SetGoState(GO_STATE_ACTIVE);
            m_FireDoorGuid = pFireDoor->GetObjectGuid();
        }
    }

    void WaypointReached(uint32 m_uiPointId)
    {
        switch(m_uiPointId)
        {
            case 0:
                if (Player* pPlayer = GetPlayerForEscort())
                    m_creature->SetFacingToObject(pPlayer);
                DoScriptText(SAY_ESCORT_START, m_creature);
                m_creature->RemoveSplineFlag(SPLINEFLAG_WALKMODE);
                break;
            case 6:
                DoScriptText(SAY_BEFOR_CHAMBER, m_creature);
                m_creature->AddSplineFlag(SPLINEFLAG_WALKMODE);
                break;
            case 8:
                SetEscortPaused(true);
                DoScriptText(SAY_OPENING_CAGE, m_creature);
                m_creature->HandleEmoteCommand(EMOTE_STATE_USESTANDING);
                subevent = SE_ADARRIA_CAGE_OPENED;
                break;
            case 11:
                SetEscortPaused(true);
                subevent = SE_FIRST_GONG_BANG;
                break;
            case 12: DoScriptText(SAY_TOWARD_DOOR, m_creature); break;
            case 13:
                if (GameObject* pFireDoor = m_creature->GetMap()->GetGameObject(m_FireDoorGuid))
                    pFireDoor->SetGoState(GO_STATE_READY);
                break;
            case 15:
                DoScriptText(SAY_DOOR_IN_FLAMES, m_creature);
                if (Creature* pTecahuna = m_creature->SummonCreature(NPC_TECAHUNA, fTecahunaPos[0], fTecahunaPos[1], fTecahunaPos[2], fTecahunaPos[3], TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 180000))
                    m_TecahunaGuid = pTecahuna->GetObjectGuid();
                break;
            case 18:
                SetEscortPaused(true);
                subevent = SE_HATE_SNAKES;
                break;
            case 56:
                if (Player* pPlayer = GetPlayerForEscort())
                {
                    m_creature->SetFacingToObject(pPlayer);
                    pPlayer->GroupEventHappens(QUEST_DUN_DA_DUN_TAH, m_creature);
                }
                DoScriptText(SAY_QUEST_COMPLETED, m_creature);
                break;

            default: break;
        }
        m_uiSubeventTimer = 3000;
    }

    void UpdateEscortAI(const uint32 uiDiff)
    {
        if (subevent)
            if (m_uiSubeventTimer < uiDiff)
            {
                m_uiSubeventTimer = 3000;
                switch(subevent)
                {
                    case SE_ADARRIA_CAGE_OPENED:
                        m_creature->HandleEmoteCommand(EMOTE_STATE_NONE);
                        DoScriptText(SAY_FREEING_ADARRA, m_creature);
                        if (Creature* pAdarria = GetClosestCreatureWithEntry(m_creature, NPC_ADDARIA, INTERACTION_DISTANCE))
                        {
                            m_AddariaGuid = pAdarria->GetObjectGuid();
                            if (GameObject* pAddariaCage = GetClosestGameObjectWithEntry(m_creature, GO_ADARRA_CAGE, INTERACTION_DISTANCE))
                            {
                                m_AddariaCageGuid = pAddariaCage->GetObjectGuid();
                                if (pAddariaCage->GetGoState() == GO_STATE_READY)
                                    pAddariaCage->SetGoState(GO_STATE_ACTIVE);
                            }
                            pAdarria->HandleEmoteCommand(EMOTE_ONESHOT_CHEER);
                        }
                        subevent = SE_ADARRIA_ESCAPE;
                        break;
                    case SE_ADARRIA_ESCAPE:
                        
                        if (Creature* pAdarria = m_creature->GetMap()->GetCreature(m_AddariaGuid))
                        {
                            DoScriptText(SAY_ADARRA_THANX, pAdarria);
                            pAdarria->ForcedDespawn(3000);
                            if (GameObject* pFireDoor = m_creature->GetMap()->GetGameObject(m_FireDoorGuid))
                            {
                                m_FireDoorGuid = pFireDoor->GetObjectGuid();
                                float fDestX, fDestY, fDestZ;
                                pFireDoor->GetPosition(fDestX, fDestY, fDestZ);
                                pAdarria->SendMonsterMove(fDestX, fDestY, fDestZ, SPLINETYPE_NORMAL, SPLINEFLAG_NONE, 3000);
                            }
                        }
                        subevent = SE_RETURN_TO_ESCORT;
                        break;
                    case SE_FIRST_GONG_BANG:
                        DoCastSpellIfCan(m_creature, SPELL_BANG_RITUAL_GONG);
                        subevent = SE_SECOND_GONG_BANG;
                        break;
                    case SE_SECOND_GONG_BANG:
                        DoCastSpellIfCan(m_creature, SPELL_BANG_RITUAL_GONG);
                        subevent = SE_LEAVE_GONG;
                        break;
                    case SE_LEAVE_GONG:
                        DoScriptText(SAY_AFTER_GONG_BANG, m_creature);
                        subevent = SE_RETURN_TO_ESCORT;
                        break;
                    case SE_HATE_SNAKES:
                        if (Creature* pTecahuna = m_creature->GetMap()->GetCreature(m_TecahunaGuid))
                            m_creature->SetFacingToObject(pTecahuna);
                        DoScriptText(SAY_HATE_SNAKES, m_creature);
                        subevent = SE_ATTACK_TECAHUNA;
                        break;
                    case SE_ATTACK_TECAHUNA:
                        DoScriptText(SAY_ATTACK_TECAHUNA, m_creature);
                        if (Creature* pTecahuna = m_creature->GetMap()->GetCreature(m_TecahunaGuid))
                            m_creature->AI()->AttackStart(pTecahuna);
                        subevent = SE_IS_TECAHUNA_DEAD;
                        break;
                    case SE_IS_TECAHUNA_DEAD:
                        if (Creature* pTecahuna = m_creature->GetMap()->GetCreature(m_TecahunaGuid))
                            if (!pTecahuna->isAlive())
                            {
                                if (GameObject* pFireDoor = m_creature->GetMap()->GetGameObject(m_FireDoorGuid))
                                    pFireDoor->SetGoState(GO_STATE_ACTIVE);
                                subevent = SE_RETURN_TO_ESCORT;
                            }
                        break;
                    case SE_RETURN_TO_ESCORT:
                        subevent = SE_NONE;
                        SetEscortPaused(false);
                        break;

                    default: break;
                }
            }
            else
                m_uiSubeventTimer -= uiDiff;


        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }

};

CreatureAI* GetAI_npc_harrison_jones_gh(Creature* pCreature)
{
    return new npc_harrison_jones_ghAI(pCreature);
}

bool QuestAccept_npc_harrison_jones_gh(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    if (pQuest->GetQuestId() == QUEST_DUN_DA_DUN_TAH)
    {
        if (npc_harrison_jones_ghAI* pEscortAI = dynamic_cast<npc_harrison_jones_ghAI*>(pCreature->AI()))
            pEscortAI->Start(false, pPlayer, pQuest);
    }
    return true;
}

/*######
## npc_depleted_war_golem
######*/

enum
{
    SAY_GOLEM_CHARGE            = -1000626,
    SAY_GOLEM_COMPLETE          = -1000627,

    NPC_LIGHTNING_SENTRY        = 26407,

    SPELL_CHARGE_GOLEM          = 47799,
    SPELL_GOLEM_CHARGE_CREDIT   = 47797,
};

struct MANGOS_DLL_DECL npc_depleted_war_golemAI : public ScriptedPetAI
{
    npc_depleted_war_golemAI(Creature* pCreature) : ScriptedPetAI(pCreature) { Reset(); }

    void Reset() { }

    void OwnerKilledUnit(Unit* pVictim)
    {
        if (pVictim->GetTypeId() == TYPEID_UNIT && pVictim->GetEntry() == NPC_LIGHTNING_SENTRY)
        {
            // Is distance expected?
            if (m_creature->IsWithinDistInMap(pVictim, 10.0f))
                m_creature->CastSpell(m_creature, SPELL_CHARGE_GOLEM, true);
        }
    }
};

CreatureAI* GetAI_npc_depleted_war_golem(Creature* pCreature)
{
    return new npc_depleted_war_golemAI(pCreature);
}

bool EffectAuraDummy_npc_depleted_war_golem(const Aura* pAura, bool bApply)
{
    if (pAura->GetId() != SPELL_CHARGE_GOLEM)
        return true;

    Creature* pCreature = (Creature*)pAura->GetTarget();

    if (!pCreature)
        return true;

    if (pAura->GetEffIndex() == EFFECT_INDEX_0)
    {
        if (bApply)
        {
            DoScriptText(SAY_GOLEM_CHARGE, pCreature);
            pCreature->addUnitState(UNIT_STAT_STUNNED);
        }
        else
        {
            DoScriptText(SAY_GOLEM_COMPLETE, pCreature);
            pCreature->clearUnitState(UNIT_STAT_STUNNED);

            // targets master
            pCreature->CastSpell(pCreature, SPELL_GOLEM_CHARGE_CREDIT, true);
        }
    }

    return true;
}

/*######
##mob_overseer
######*/
enum Overseer
{
    GO_RUNE_BRUNON                  = 188507,
    GO_RUNE_LOCHLI                  = 188506,
    GO_RUNE_KORGAN                  = 188505,
    GO_RUNE_DURVAL                  = 188471,
    GO_RUNE_BASIS                   = 188492,

    NPC_BRUNON                      = 26923,
    NPC_LOCHLI                      = 26922,
    NPC_KORGAN                      = 26921,
    NPC_DURVAL                      = 26920,

    MOB_RUNE_WEAVER                 = 26820,
    MAX_WEAVERS                     = 4,

    FACTION_FRIENDLY_ALL            = 35,
    FACTION_DARK_IRON               = 754,

    SPAWN_DISTANCE                  = 15,

    // Durval
    SPELL_RUNE_OF_DESTRUCTION       = 52715,
    // Korgan
    SPELL_CALL_LIGHTNING            = 32018,
    SPELL_SPELL_REVITALIZING_RUNE   = 52714,
    // Lohli
    SPELL_THUNDERSTORM              = 52717,

    SPELL_WEAVING_OOC               = 47463,

    SAY_AGGRO_1                     = -1999844,
    SAY_AGGRO_2                     = -1999843,
    SAY_AGGRO_3                     = -1999842
};

struct MANGOS_DLL_DECL mob_overseerAI : public ScriptedAI
{
    mob_overseerAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
        ManageWeavers();
    }

    float WeaverPOSITION[MAX_WEAVERS][3];
    uint64 WeaverGUID[MAX_WEAVERS];
    uint32 m_uiCheckoutTimer;
    uint32 m_uiRuneEntry;
    uint32 m_uiEventTimer;
    uint8 WeaversDead;

    void Reset()
    {
        m_uiCheckoutTimer = 1000;
        m_uiRuneEntry = 0;
        m_uiEventTimer = 10000;
    }

    void ManageWeavers()
    {
        // summoning weavers
        for (int i = 0; i < MAX_WEAVERS; ++i)
        {
            // if somehow overseer entered combat and than evaded before all weavers are dead and despawed (30sec from encounter)
            if (Creature* pWeaver = m_creature->GetMap()->GetCreature(WeaverGUID[i]))
            {
                if (!pWeaver->isAlive())
                {
                    pWeaver->Respawn();
                    pWeaver->Relocate(WeaverPOSITION[i][0], WeaverPOSITION[i][1], WeaverPOSITION[i][2]);
                    pWeaver->SetFacingToObject(m_creature);
                }
            }
            else 
            {
                // spawn weavers around overseer in regular distance
                float x, y, z; 
                m_creature->GetNearPoint(m_creature, x, y, z, m_creature->GetObjectBoundingRadius(), SPAWN_DISTANCE, (i*(6.20f/MAX_WEAVERS)));
                if (Creature* pWeaver = m_creature->SummonCreature(MOB_RUNE_WEAVER, x, y, z, 0.0f, TEMPSUMMON_MANUAL_DESPAWN, 0))
                {
                    WeaverGUID[i] = pWeaver->GetGUID();
                    WeaverPOSITION[i][0] = x;
                    WeaverPOSITION[i][1] = y;
                    WeaverPOSITION[i][2] = z;                  
                    pWeaver->SetFacingToObject(m_creature);
                }
            }
        }

        switch(m_creature->GetEntry())
        {
            case NPC_BRUNON: m_uiRuneEntry = GO_RUNE_BRUNON; break;
            case NPC_LOCHLI: m_uiRuneEntry = GO_RUNE_LOCHLI; break;
            case NPC_KORGAN: m_uiRuneEntry = GO_RUNE_KORGAN; break;
            case NPC_DURVAL: m_uiRuneEntry = GO_RUNE_DURVAL; break;
        }

        // every reset
        if (GameObject* pGo = GetClosestGameObjectWithEntry(m_creature, m_uiRuneEntry, INTERACTION_DISTANCE))
            if (pGo->GetGoState() == GO_STATE_READY)
                pGo->SetGoState(GO_STATE_ACTIVE);
        if (GameObject* pGo = GetClosestGameObjectWithEntry(m_creature, GO_RUNE_BASIS, INTERACTION_DISTANCE))
            if (pGo->GetGoState() == GO_STATE_READY)
                pGo->SetGoState(GO_STATE_ACTIVE);

        m_creature->SetDisplayId(11686);
        SetEquipmentSlots(false, EQUIP_UNEQUIP, EQUIP_UNEQUIP, EQUIP_UNEQUIP);
        m_creature->setFaction(FACTION_FRIENDLY_ALL);

        WeaversDead = 0;
    }

    void JustReachedHome()
    {
        ManageWeavers();
    }

    void JustRespawned()
    {
        ManageWeavers();
    }

    void Aggro(Unit* pWho)
    {
        if (!pWho)
            return;

        switch(urand(0,2))
        {
            case 0: DoScriptText(SAY_AGGRO_1,m_creature); break;
            case 1: DoScriptText(SAY_AGGRO_2,m_creature); break;
            case 2: DoScriptText(SAY_AGGRO_3,m_creature); break;
        }
        ScriptedAI::Aggro(pWho);
    }

    void SummonedCreatureJustDied(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() == MOB_RUNE_WEAVER)
            ++WeaversDead;

            if (WeaversDead >= MAX_WEAVERS)
            {
                m_creature->setFaction(FACTION_DARK_IRON);
                m_creature->SetDisplayId(m_creature->GetNativeDisplayId());
                SetEquipmentSlots(true);

                if (GameObject* pGo = GetClosestGameObjectWithEntry(m_creature, m_uiRuneEntry, INTERACTION_DISTANCE))
                    if (pGo->GetGoState() == GO_STATE_ACTIVE)
                        pGo->SetGoState(GO_STATE_READY);
                if (GameObject* pGo = GetClosestGameObjectWithEntry(m_creature, GO_RUNE_BASIS, INTERACTION_DISTANCE))
                    if (pGo->GetGoState() == GO_STATE_ACTIVE)
                        pGo->SetGoState(GO_STATE_READY);
            }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        // OOC
        if (m_uiCheckoutTimer <= uiDiff)
        {
            for (int i = 0; i < MAX_WEAVERS; ++i)
            {
                if (WeaverGUID[i] == 0)
                    continue;

                Creature* pWeaver = m_creature->GetMap()->GetCreature(WeaverGUID[i]);
                // return weavers to orginal spawn point
                if (pWeaver && pWeaver->isAlive())
                {
                    if (!pWeaver->SelectHostileTarget() || !pWeaver->getVictim())
                    {
                        if ((m_creature->GetDistance(pWeaver) > (SPAWN_DISTANCE + 1.5)) || (m_creature->GetDistance(pWeaver) < (SPAWN_DISTANCE - 1.5)))
                        {
                            pWeaver->GetMotionMaster()->Clear();
                            pWeaver->GetMotionMaster()->MovePoint(0, WeaverPOSITION[i][0], WeaverPOSITION[i][1], WeaverPOSITION[i][2]);
                        }
                        else if (!pWeaver->GetCurrentSpell(CURRENT_CHANNELED_SPELL))
                            pWeaver->CastSpell(m_creature, SPELL_WEAVING_OOC, false);
                    }
                    else if (pWeaver->HasAura(SPELL_WEAVING_OOC)) 
                        pWeaver->InterruptSpell(CURRENT_CHANNELED_SPELL);
                }
            }
            m_uiCheckoutTimer = 1000;
        } m_uiCheckoutTimer -= uiDiff;

        // in combat
        if(!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiEventTimer <= uiDiff)
        {
            if (Unit* pVictim = m_creature->getVictim())
            {
                switch(m_creature->GetEntry())
                {
                    case NPC_DURVAL: DoCastSpellIfCan(pVictim, SPELL_RUNE_OF_DESTRUCTION);  break;
                    case NPC_LOCHLI: DoCastSpellIfCan(pVictim, SPELL_THUNDERSTORM);         break;
                    case NPC_KORGAN:
                        if (m_creature->GetHealthPercent() < 80.0f)
                            DoCastSpellIfCan(m_creature, SPELL_SPELL_REVITALIZING_RUNE);
                        else DoCastSpellIfCan(pVictim, SPELL_CALL_LIGHTNING);
                        break;
                    default: break;
                }
            }
            m_uiEventTimer = 30000;
        } else m_uiEventTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }                 
};

CreatureAI* GetAI_mob_overseer(Creature* pCreature)
{
    return new mob_overseerAI(pCreature);
}

enum SmokeEmOut
{
    SAY_SEO1                                     = -1603535,
    SAY_SEO2                                     = -1603536,
    SAY_SEO3                                     = -1603537,
    SAY_SEO4                                     = -1603538,
    QUEST_SMOKE_EM_OUT_A                         = 12323,
    QUEST_SMOKE_EM_OUT_H                         = 12324,
    SPELL_SMOKE_BOMB                             = 49075,
    SPELL_CHOP                                   = 43410,
    NPC_VENTURE_CO_STABLES_KC                    = 27568
};


struct MANGOS_DLL_DECL npc_venture_co_stragglerAI : public ScriptedAI
{
    npc_venture_co_stragglerAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }
        

        uint64 uiPlayerGUID;
        uint32 uiRunAwayTimer;
        uint32 uiTimer;
        uint32 uiChopTimer;

        void Reset()
        {
            uiPlayerGUID = 0;
            uiTimer = 0;
            uiChopTimer = urand(10000,12500);
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_OOC_NOT_ATTACKABLE);
           
        }

        void UpdateAI(const uint32 uiDiff)
        {
            if (uiRunAwayTimer <= uiDiff)
            {
                if (Player *pPlayer = m_creature->GetMap()->GetPlayer(uiPlayerGUID))
                {
                    switch (uiTimer)
                    {
                        case 0:
                            if (pPlayer->GetQuestStatus(QUEST_SMOKE_EM_OUT_A) == QUEST_STATUS_INCOMPLETE ||
                                pPlayer->GetQuestStatus(QUEST_SMOKE_EM_OUT_H) == QUEST_STATUS_INCOMPLETE)
                                pPlayer->KilledMonsterCredit(NPC_VENTURE_CO_STABLES_KC);
                            m_creature->GetMotionMaster()->MovePoint(0, m_creature->GetPositionX()-7, m_creature->GetPositionY()+7, m_creature->GetPositionZ());
                            uiRunAwayTimer = 2500;
                            ++uiTimer;
                            break;
                        case 1:
                            DoScriptText((SAY_SEO1, SAY_SEO2, SAY_SEO3, SAY_SEO4), m_creature);
                            m_creature->GetMotionMaster()->MovePoint(0, m_creature->GetPositionX()-7, m_creature->GetPositionY()-5, m_creature->GetPositionZ());
                            uiRunAwayTimer = 2500;
                            ++uiTimer;
                            break;
                        case 2:
                            m_creature->GetMotionMaster()->MovePoint(0, m_creature->GetPositionX()-5, m_creature->GetPositionY()-5, m_creature->GetPositionZ());
                            uiRunAwayTimer = 2500;
                            ++uiTimer;
                            break;
                        
                    }
                }
            }
            else
                uiRunAwayTimer -= uiDiff;

            
            if (uiChopTimer <= uiDiff)
            {
                DoCast(m_creature->getVictim(), SPELL_CHOP);
                uiChopTimer = urand(10000,12000);
            }
            else
                uiChopTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }

        void SpellHit(Unit *pCaster, const SpellEntry *pSpell)
        {
            if (pCaster && pCaster->GetTypeId() == TYPEID_PLAYER && pSpell->Id == SPELL_SMOKE_BOMB)
            {
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_OOC_NOT_ATTACKABLE);
                m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE);
                m_creature->CombatStop(false);
                uiPlayerGUID = pCaster->GetGUID();
                uiRunAwayTimer = 3500;
		        m_creature->ForcedDespawn(7000);
            }
        }
    };

CreatureAI* GetAI_npc_venture_co_straggler(Creature* pCreature)
{
    return new npc_venture_co_stragglerAI(pCreature);
}

void AddSC_grizzly_hills()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "npc_harrison_jones_gh";
    pNewScript->GetAI = &GetAI_npc_harrison_jones_gh;
    pNewScript->pQuestAcceptNPC = &QuestAccept_npc_harrison_jones_gh;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_depleted_war_golem";
    pNewScript->GetAI = &GetAI_npc_depleted_war_golem;
    pNewScript->pEffectAuraDummy = &EffectAuraDummy_npc_depleted_war_golem;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_overseer";
    pNewScript->GetAI = &GetAI_mob_overseer;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_venture_co_straggler";
    pNewScript->GetAI = &GetAI_npc_venture_co_straggler;
    pNewScript->RegisterSelf();
}
