/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef WORLDMAP_KALIMDOR_H
#define WORLDMAP_KALIMDOR_H

enum
{
    TYPE_ALLIANCE_SILITHYSTS        = 1,
    TYPE_HORDE_SILITHYSTS           = 2,
    TYPE_CONTROLLER_FACTION         = 3,

    // npcs
    NPC_SILITHUS_DUST_QUEST_ALY     = 17090,        // dummy npcs for quest credit
    NPC_SILITHUS_DUST_QUEST_HORDE   = 18199,

    // game objects
    GO_SILITHYST_MOUND              = 181597,       // created when a player drops the flag
    GO_SILITHYST_GEYSER             = 181598,       // spawn on the map by default

    // spells
    SPELL_SILITHYST_OBJECT          = 29518,        // unk, related to the GO
    SPELL_SILITHYST                 = 29519,        // buff recieved when you are carrying a silithyst
    SPELL_TRACES_OF_SILITHYST       = 29534,        // individual buff recieved when succesfully delivered a silithyst
    SPELL_CENARION_FAVOR            = 30754,        // zone buff recieved when a faction gathers 200 silithysts
    SPELL_SILITHYST_FLAG_DROP       = 29533,

    // quests
    QUEST_SCOURING_DESERT_ALY       = 9419,
    QUEST_SCOURING_DESERT_HORDE     = 9422,

    // zone ids
    ZONE_ID_SILITHUS                = 1377,
    ZONE_ID_TEMPLE_OF_AQ            = 3428,         // ToDo - research
    ZONE_ID_RUINS_OF_AQ             = 3429,         // don't know yet how to handle the buff inside the instances
    ZONE_ID_GATES_OF_AQ             = 3478,         // not sure if needed

    // area triggers
    AREATRIGGER_SILITHUS_ALY        = 4162,         // areatriggers ids
    AREATRIGGER_SILITHUS_HORDE      = 4168,

    // zone emotes
    //ZONE_EMOTE_HORDE_CAPTURE        = -1001000,
    //ZONE_EMOTE_ALY_CAPTURE          = -1001001,

    FACTION_CENARION_CIRCLE         = 609,
    HONOR_REWARD_SILITHYST          = 19,
    REPUTATION_REWARD_SILITHYST     = 20,
    MAX_SILITHYST                   = 200,

    // world states
    WORLD_STATE_SI_GATHERED_A       = 2313,         // world state ids
    WORLD_STATE_SI_GATHERED_H       = 2314,
    WORLD_STATE_SI_SILITHYST_MAX    = 2317,
};

struct sKalimdorLocation
{
    float m_fX, m_fY, m_fZ;
};

static sKalimdorLocation m_aSilithusLocs[2] =
{
    {-7142.04f, 1397.92f, 4.327f},      // aly
    {-7588.48f, 756.806f, -16.425f}     // horde
};

class MANGOS_DLL_DECL worldmap_kalimdor : public ScriptedInstance
{
    public:
        worldmap_kalimdor(Map* pMap);
        ~worldmap_kalimdor() {}

        void OnPlayerEnterZone(Player* pPlayer, uint32 uiZoneId);
        void OnPlayerDroppedFlag(Player* pPlayer, uint32 uiSpellId);

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);

    protected:
        void UpdateZoneWorldState();
        void SendPlayerWorldState(Player* pPlayer);

        // Sends world state update to all players in the current zone; they are stored in a PlayerSet
        void DoUpdateZoneWorldState(uint32 uiStateId, uint32 uiStateData);
        // Process a spell a spell event (cast or remove) to a specific team in a specific zone;  they are stored in a PlayerSet
        void DoProcessTeamBuff(Team uiTeamId, uint32 uiSpellId, bool bRemove = false);

        uint32 m_uiResourcesAly;
        uint32 m_uiResourcesHorde;
        uint32 m_uiLastControllerTeam;

        std::set<uint64> m_suiSilithusPlayers;
};

#endif