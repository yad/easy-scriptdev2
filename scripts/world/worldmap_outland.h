/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef WORLDMAP_OUTLAND_H
#define WORLDMAP_OUTLAND_H

enum
{
    MAX_TOWERS                              = 3,

    TYPE_OVERLOOK_CONTROLLER                = 1,
    TYPE_STADIUM_CONTROLLER                 = 2,
    TYPE_BROKEN_HILL_CONTROLLER             = 3,
    TYPE_OVERLOOK_STATE                     = 4,
    TYPE_STADIUM_STATE                      = 5,
    TYPE_BROKEN_HILL_STATE                  = 6,

    // tower ids
    // they are used in the process event functions
    TOWER_ID_OVERLOOK                       = 1,
    TOWER_ID_STADIUM                        = 2,
    TOWER_ID_BROKEN_HILL                    = 3,

    // spells
    SPELL_HELLFIRE_TOWER_TOKEN_ALY          = 32155,
    SPELL_HELLFIRE_TOWER_TOKEN_HORDE        = 32158,
    SPELL_HELLFIRE_SUPERIORITY_ALY          = 32071,
    SPELL_HELLFIRE_SUPERIORITY_HORDE        = 32049,

    // zone ids
    ZONE_ID_HELLFIRE_PENINSULA              = 3483,
    ZONE_ID_HELLFIRE_CITADEL                = 3563,
    ZONE_ID_HELLFIRE_RAMPARTS               = 3562,
    ZONE_ID_BLOOD_FURNACE                   = 3713,
    ZONE_ID_SHATTERED_HALLS                 = 3714,
    ZONE_ID_MAGTHERIDON_LAIR                = 3836,

    // npcs
    NPC_CAPTURE_CREDIT_OVERLOOK             = 19028,
    NPC_CAPTURE_CREDIT_STADIUM              = 19029,
    NPC_CAPTURE_CREDIT_BROKEN_HILL          = 19032,

    // gameobjects
    GO_TOWER_BANNER_OVERLOOK                = 182525,
    GO_TOWER_BANNER_STADIUM                 = 183515,
    GO_TOWER_BANNER_BRONEK_HILL             = 183514,

    // capture points
    GO_HELLFIRE_BANNER_OVERLOOK             = 182174,
    GO_HELLFIRE_BANNER_STADIUM              = 182173,
    GO_HELLFIRE_BANNER_BROKEN_HILL          = 182175,

    // events
    EVENT_OVERLOOK_WIN_ALLIANCE             = 11398,
    EVENT_OVERLOOK_WIN_HORDE                = 11397,
    EVENT_OVERLOOK_CONTEST_ALLIANCE         = 11392,
    EVENT_OVERLOOK_CONTEST_HORDE            = 11391,
    EVENT_OVERLOOK_PROGRESS_ALLIANCE        = 11396,
    EVENT_OVERLOOK_PROGRESS_HORDE           = 11395,
    EVENT_OVERLOOK_NEUTRAL_ALLIANCE         = 11394,
    EVENT_OVERLOOK_NEUTRAL_HORDE            = 11393,

    EVENT_STADIUM_WIN_ALLIANCE              = 11390,
    EVENT_STADIUM_WIN_HORDE                 = 11389,
    EVENT_STADIUM_CONTEST_ALLIANCE          = 11384,
    EVENT_STADIUM_CONTEST_HORDE             = 11383,
    EVENT_STADIUM_PROGRESS_ALLIANCE         = 11388,
    EVENT_STADIUM_PROGRESS_HORDE            = 11387,
    EVENT_STADIUM_NEUTRAL_ALLIANCE          = 11386,
    EVENT_STADIUM_NEUTRAL_HORDE             = 11385,

    EVENT_BROKEN_HILL_WIN_ALLIANCE          = 11406,
    EVENT_BROKEN_HILL_WIN_HORDE             = 11405,
    EVENT_BROKEN_HILL_CONTEST_ALLIANCE      = 11400,
    EVENT_BROKEN_HILL_CONTEST_HORDE         = 11399,
    EVENT_BROKEN_HILL_PROGRESS_ALLIANCE     = 11404,
    EVENT_BROKEN_HILL_PROGRESS_HORDE        = 11403,
    EVENT_BROKEN_HILL_NEUTRAL_ALLIANCE      = 11402,
    EVENT_BROKEN_HILL_NEUTRAL_HORDE         = 11401,

    // tower artkits
    GO_ARTKIT_BROKEN_HILL_ALY               = 65,
    GO_ARTKIT_BROKEN_HILL_HORDE             = 64,
    GO_ARTKIT_BROKEN_HILL_NEUTRAL           = 66,

    GO_ARTKIT_OVERLOOK_ALY                  = 62,
    GO_ARTKIT_OVERLOOK_HORDE                = 61,
    GO_ARTKIT_OVERLOOK_NEUTRAL              = 63,

    GO_ARTKIT_STADIUM_ALY                   = 67,
    GO_ARTKIT_STADIUM_HORDE                 = 68,
    GO_ARTKIT_STADIUM_NEUTRAL               = 69,

    GO_ARTKIT_BANNER_ALLIANCE               = 2,
    GO_ARTKIT_BANNER_HORDE                  = 1,
    GO_ARTKIT_BANNER_NEUTRAL                = 21,

    // world states
    WORLD_STATE_TOWER_COUNT_ALY             = 0x9ac,
    WORLD_STATE_TOWER_COUNT_HORDE           = 0x9ae,

    WORLD_STATE_BROKEN_HILL_ALY             = 0x9b3,
    WORLD_STATE_BROKEN_HILL_HORDE           = 0x9b4,
    WORLD_STATE_BROKEN_HILL_NEUTRAL         = 0x9b5,

    WORLD_STATE_OVERLOOK_ALY                = 0x9b0,
    WORLD_STATE_OVERLOOK_HORDE              = 0x9b1,
    WORLD_STATE_OVERLOOK_NEUTRAL            = 0x9b2,

    WORLD_STATE_STADIUM_ALY                 = 0x9a7,
    WORLD_STATE_STADIUM_HORDE               = 0x9a6,
    WORLD_STATE_STADIUM_NEUTRAL             = 0x9a8,
};

enum CaptureState
{
    NEUTRAL     = 0,
    PROGRESS    = 1,
    CONTESTED   = 2,
    WIN         = 3
};

class MANGOS_DLL_DECL worldmap_outland : public ScriptedInstance
{
    public:
        worldmap_outland(Map* pMap);
        ~worldmap_outland() {}

        void OnPlayerEnterZone(Player* pPlayer, uint32 uiZoneId);
        void OnPlayerDeath(Player* pPlayer);
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

        uint32 m_uiOverlookController;
        uint32 m_uiStadiumController;
        uint32 m_uiBrokenHillController;
        uint32 m_uiOverlookState;
        uint32 m_uiStadiumState;
        uint32 m_uiBrokenHillState;
        uint32 m_uiOverlookWorldState;
        uint32 m_uiStadiumWorldState;
        uint32 m_uiBrokenHillWorldState;
        uint32 m_uiTowersAlly;
        uint32 m_uiTowersHorde;

        uint64 m_uiTowerBannerOverlookGUID;
        uint64 m_uiTowerBannerStadiumGUID;
        uint64 m_uiTowerBannerBrokenHillGUID;
        uint64 m_uiTowerPointOverlookGUID;
        uint64 m_uiTowerPointStadiumGUID;
        uint64 m_uiTowerPointBrokenHillGUID;

        std::set<uint64> m_suiHellfirePlayers;

        std::list<uint64> m_lTowerBanners;
};

#endif
