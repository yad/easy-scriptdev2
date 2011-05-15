DELETE FROM script_texts WHERE entry IN (-1999890,-1999889,-1999888);
INSERT INTO script_texts (entry, content_default, TYPE, COMMENT) VALUES
(-1999890,'Anub\'ar fear nothing, mortal...',0,'npc_warsong_battle_standart SAY_INVIDER_DESTROYED_STANDARD'),
(-1999889,'The Anub\'ar have destroyed Warsong Battle Standard!',2,'npc_warsong_battle_standart SAY_EMOTE_STANDARD_DESTROYED'),
(-1999888,'Show yourselves so that I may crack your insectoid skeletons with my bare hands!',0,'npc_warsong_battle_standart SAY_PLAYER_PLANTED_STANDARD');
