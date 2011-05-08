/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef WORLDMAP_EASTERN_KINGDOMS_H
#define WORLDMAP_EASTERN_KINGDOMS_H

enum
{
    TYPE_CROWNGUARD_CONTROLLER              = 1,
    TYPE_EASTWALL_CONTROLLER                = 2,
    TYPE_NORTHPASS_CONTROLLER               = 3,
    TYPE_PLAGUEWOOD_CONTROLLER              = 4,
    TYPE_CROWNGUARD_STATE                   = 5,
    TYPE_EASTWALL_STATE                     = 6,
    TYPE_NORTHPASS_STATE                    = 7,
    TYPE_PLAGUEWOOD_STATE                   = 8,

    // tower ids
    // they are used in the process event functions
    TOWER_ID_CROWNGUARD                     = 1,
    TOWER_ID_EASTWALL                       = 2,
    TOWER_ID_NORTHPASS                      = 3,
    TOWER_ID_PLAGUEWOOD                     = 4,

    // spells
    // aly
    SPELL_ECOES_OF_LORDAERON_ALY_1          = 11413,
    SPELL_ECOES_OF_LORDAERON_ALY_2          = 11414,
    SPELL_ECOES_OF_LORDAERON_ALY_3          = 11415,
    SPELL_ECOES_OF_LORDAERON_ALY_4          = 1386,

    // horde
    SPELL_ECOES_OF_LORDAERON_HORDE_1        = 30880,
    SPELL_ECOES_OF_LORDAERON_HORDE_2        = 30683,
    SPELL_ECOES_OF_LORDAERON_HORDE_3        = 30682,
    SPELL_ECOES_OF_LORDAERON_HORDE_4        = 29520,

    // zone ids
    ZONE_ID_EASTERN_PLAGUELANDS             = 139,
    ZONE_ID_STRATHOLME                      = 2017,
    ZONE_ID_SCHOLOMANCE                     = 2057,

    // graveyards
    GRAVEYARD_ZONE_EASTERN_PLAGUE           = 139,
    GRAVEYARD_ID_EASTERN_PLAGUE             = 927,

    // taxi nodes
    TAXI_NODE_PLAGUEWOOD_TOWER              = 84,
    TAXI_NODE_NORTHPASS_TOWER               = 85,
    TAXI_NODE_EASTWALL_TOWER                = 86,
    TAXI_NODE_CROWNGUARD_TOWER              = 87,

    // npcs
    NPC_SPECTRAL_FLIGHTMASTER               = 17209,

    // quest
    NPC_CROWNGUARD_TOWER_QUEST_DOODAD       = 17689,
    NPC_EASTWALL_TOWER_QUEST_DOODAD         = 17690,
    NPC_NORTHPASS_TOWER_QUEST_DOODAD        = 17696,
    NPC_PLAGUEWOOD_TOWER_QUEST_DOODAD       = 17698,

    // aly
    NPC_LORDAERON_COMMANDER                 = 17635,
    NPC_LORDAERON_SOLDIER                   = 17647,

    // horde
    NPC_LORDAERON_VETERAN                   = 17995,
    NPC_LORDAERON_FIGHTER                   = 17996,

    // gameobjects
    GO_LORDAERON_SHRINE                     = 181682,
    GO_TOWER_FLAG                           = 182106,

    // capture points
    GO_BATTLEFIELD_BANNER_PLAGUELANDS_1     = 181899,   // northpass
    GO_BATTLEFIELD_BANNER_PLAGUELANDS_2     = 182096,   // crownguard
    GO_BATTLEFIELD_BANNER_PLAGUELANDS_3     = 182097,   // eastwall
    GO_BATTLEFIELD_BANNER_PLAGUELANDS_4     = 182098,   // plaguewood

    // events
    // crownguard - banner 1
    EVENT_CROWNGUARD_WIN_ALLIANCE           = 10568,
    EVENT_CROWNGUARD_WIN_HORDE              = 10556,
    EVENT_CROWNGUARD_CONTEST_ALLIANCE       = 10697,
    EVENT_CROWNGUARD_CONTEST_HORDE          = 10696,
    EVENT_CROWNGUARD_PROGRESS_ALLIANCE      = 10699,
    EVENT_CROWNGUARD_PROGRESS_HORDE         = 10698,
    EVENT_CROWNGUARD_NEUTRAL_ALLIANCE       = 11151,
    EVENT_CROWNGUARD_NEUTRAL_HORDE          = 11150,

    // eastwall - banner 2
    EVENT_EASTWALL_WIN_ALLIANCE             = 10570,
    EVENT_EASTWALL_WIN_HORDE                = 10566,
    EVENT_EASTWALL_CONTEST_ALLIANCE         = 10703,
    EVENT_EASTWALL_CONTEST_HORDE            = 10702,
    EVENT_EASTWALL_PROGRESS_ALLIANCE        = 10705,
    EVENT_EASTWALL_PROGRESS_HORDE           = 10704,
    EVENT_EASTWALL_NEUTRAL_ALLIANCE         = 11155,
    EVENT_EASTWALL_NEUTRAL_HORDE            = 11154,

    // northpass - banner 3
    EVENT_NORTHPASS_WIN_ALLIANCE            = 10569,
    EVENT_NORTHPASS_WIN_HORDE               = 10565,
    EVENT_NORTHPASS_CONTEST_ALLIANCE        = 10689,
    EVENT_NORTHPASS_CONTEST_HORDE           = 10690,
    EVENT_NORTHPASS_PROGRESS_ALLIANCE       = 10691,
    EVENT_NORTHPASS_PROGRESS_HORDE          = 10692,
    EVENT_NORTHPASS_NEUTRAL_ALLIANCE        = 11149,
    EVENT_NORTHPASS_NEUTRAL_HORDE           = 11148,

    // plaguewood - banner 4
    EVENT_PLAGUEWOOD_WIN_ALLIANCE           = 10567,
    EVENT_PLAGUEWOOD_WIN_HORDE              = 10564,
    EVENT_PLAGUEWOOD_CONTEST_ALLIANCE       = 10687,
    EVENT_PLAGUEWOOD_CONTEST_HORDE          = 10688,
    EVENT_PLAGUEWOOD_PROGRESS_ALLIANCE      = 10701,
    EVENT_PLAGUEWOOD_PROGRESS_HORDE         = 10700,
    EVENT_PLAGUEWOOD_NEUTRAL_ALLIANCE       = 11153,
    EVENT_PLAGUEWOOD_NEUTRAL_HORDE          = 11152,

    // world states
    WORLD_STATE_TOWER_COUNT_ALY             = 2327,
    WORLD_STATE_TOWER_COUNT_HORDE           = 2328,

    // plaguewood tower
    WORLD_STATE_PLAGUEWOOD_CONT_ALY         = 2366,
    WORLD_STATE_PLAGUEWOOD_CONT_HORDE       = 2367,
    WORLD_STATE_PLAGUEWOOD_PROG_ALY         = 2368,
    WORLD_STATE_PLAGUEWOOD_PROG_HORDE       = 2369,
    WORLD_STATE_PLAGUEWOOD_ALY              = 2370,
    WORLD_STATE_PLAGUEWOOD_HORDE            = 2371,
    WORLD_STATE_PLAGUEWOOD_NEUTRAL          = 2353,

    // northpass tower
    WORLD_STATE_NORTHPASS_CONT_ALY          = 2362,
    WORLD_STATE_NORTHPASS_CONT_HORDE        = 2363,
    WORLD_STATE_NORTHPASS_PROG_ALY          = 2364,
    WORLD_STATE_NORTHPASS_PROG_HORDE        = 2365,
    WORLD_STATE_NORTHPASS_ALY               = 2372,
    WORLD_STATE_NORTHPASS_HORDE             = 2373,
    WORLD_STATE_NORTHPASS_NEUTRAL           = 2352,

    // eastwall tower
    WORLD_STATE_EASTWALL_CONT_ALY           = 2359,
    WORLD_STATE_EASTWALL_CONT_HORDE         = 2360,
    WORLD_STATE_EASTWALL_PROG_ALY           = 2357,
    WORLD_STATE_EASTWALL_PROG_HORDE         = 2358,
    WORLD_STATE_EASTWALL_ALY                = 2354,
    WORLD_STATE_EASTWALL_HORDE              = 2356,
    WORLD_STATE_EASTWALL_NEUTRAL            = 2361,

    // crownguard tower
    WORLD_STATE_CROWNGUARD_CONT_ALY         = 2374,
    WORLD_STATE_CROWNGUARD_CONT_HORDE       = 2375,
    WORLD_STATE_CROWNGUARD_PROG_ALY         = 2376,
    WORLD_STATE_CROWNGUARD_PROG_HORDE       = 2377,
    WORLD_STATE_CROWNGUARD_ALY              = 2378,
    WORLD_STATE_CROWNGUARD_HORDE            = 2379,
    WORLD_STATE_CROWNGUARD_NEUTRAL          = 2355,
};

enum CaptureState
{
    NEUTRAL     = 0,
    PROGRESS    = 1,
    CONTESTED   = 2,
    WIN         = 3
};

struct sTowerBuffs
{
    uint32 uiTowers, uiSpellIdAlly, uiSpellIdHorde;
};

static sTowerBuffs m_aPlaguelandsTowerBuffs[] =
{
    {1, SPELL_ECOES_OF_LORDAERON_ALY_1, SPELL_ECOES_OF_LORDAERON_HORDE_1},
    {2, SPELL_ECOES_OF_LORDAERON_ALY_2, SPELL_ECOES_OF_LORDAERON_HORDE_2},
    {3, SPELL_ECOES_OF_LORDAERON_ALY_3, SPELL_ECOES_OF_LORDAERON_HORDE_3},
    {4, SPELL_ECOES_OF_LORDAERON_ALY_4, SPELL_ECOES_OF_LORDAERON_HORDE_4}
};

struct sSpawnLocations
{
    float m_fX, m_fY, m_fZ, m_fO;
};

// the summon positions are not correct
// summons should be at the eastwall tower
// the npcs should move to the northpass tower by pass
// NOTE: this locations are for summoning the mobs straight to the northpass tower
static sSpawnLocations m_aPlaguelandSoldiersSpawnLocs[] =
{
    {3167.61f, -4352.09f, 138.20f, 4.5811f},   // leader
    {3172.74f, -4352.99f, 139.14f, 4.9873f},   // soldiers
    {3165.89f, -4354.46f, 138.67f, 3.7244f},
    {3164.65f, -4350.26f, 138.22f, 2.4794f},
    {3169.91f, -4349.68f, 138.37f, 0.7444f}
};

static sSpawnLocations m_aPlaguelandFlightmasterSpawnLocs[] =
{
    {2987.5f, -3049.11f, 120.126f, 5.75959f}
};

class MANGOS_DLL_DECL worldmap_eastern_kingdoms : public ScriptedInstance
{
    public:
        worldmap_eastern_kingdoms(Map* pMap);
        ~worldmap_eastern_kingdoms() {}

        void OnPlayerEnterZone(Player* pPlayer, uint32 uiZoneId);
        void OnObjectCreate(GameObject* pGo);

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);

        void ProcessCaptureEvent(uint32 uiCaptureType, uint32 uiTeam, uint32 uiTower);

    protected:
        void UpdateZoneWorldState(uint32 uiStateId, uint32 uiStateValue);
        void SendPlayerWorldState(Player* pPlayer);

        // Sends world state update to all players in the current zone; they are stored in a PlayerSet
        void DoUpdateZoneWorldState(uint32 uiStateId, uint32 uiStateData);
        // Process a spell a spell event (cast or remove) to a specific team in a specific zone;  they are stored in a PlayerSet
        void DoProcessTeamBuff(Team uiTeamId, uint32 uiSpellId, bool bRemove = false);

        // plaguewood bonus
        void DoSummonFlightMasterIfCan();
        void DoUnsummonFlightMaster();
        // eastwall bonus
        void DoSummonSoldiersIfCan();
        void DoUnsummonSoldiers();
        // northpass bonus = shrine respawn
        //void DoRespawnShrine();
        // crownguard bonus = graveyard
        //void DoSetGraveyard();

        uint32 m_uiPlaguewoodController;
        uint32 m_uiEastwallController;
        uint32 m_uiNorthpassController;
        uint32 m_uiCrownguardController;
        uint32 m_uiPlaguewoodState;
        uint32 m_uiNorthpassState;
        uint32 m_uiEastwallState;
        uint32 m_uiCrownguardState;
        uint32 m_uiPlaguewoodWorldState;
        uint32 m_uiNorthpassWorldState;
        uint32 m_uiEastwallWorldState;
        uint32 m_uiCrownguardWorldState;
        uint32 m_uiTowersAlly;
        uint32 m_uiTowersHorde;

        uint64 m_uiFlightMasterGUID;
        uint64 m_uiLordaeronShrineGUID;

        std::set<uint64> m_suiPlaguelandsPlayers;

        std::list<uint64> m_lTowerBanners;
};

#endif