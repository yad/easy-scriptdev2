UPDATE creature_template SET ScriptName = 'boss_krick' WHERE entry = 36477;
UPDATE creature_template SET ScriptName = 'boss_ick' WHERE entry = 36476;
UPDATE creature_template SET ScriptName = 'mob_exploding_orb' WHERE entry = 36610;
UPDATE creature_template SET ScriptName = 'boss_scourgelord_tyrannus' WHERE entry = 36658;
UPDATE creature_template SET ScriptName = 'boss_rimefang' WHERE entry = 36661;
UPDATE creature_template SET ScriptName = 'boss_malygos' WHERE entry = 28859;
UPDATE creature_template SET ScriptName = 'npc_power_spark' WHERE entry = 30084;
UPDATE creature_template SET ScriptName = 'npc_nexus_lord' WHERE entry = 30245;
UPDATE creature_template SET ScriptName = 'npc_scion_of_eternity' WHERE entry = 30249;
UPDATE creature_template SET ScriptName = 'npc_hover_disk' WHERE entry IN (30234,30248);
UPDATE creature_template SET ScriptName = 'npc_whyrmrest_skytalon' WHERE entry = 30161;
UPDATE creature_template SET ScriptName = 'npc_alexstrasza' WHERE entry = 31253;
UPDATE gameobject_template SET ScriptName = 'go_focusing_iris' WHERE entry IN (193960,193958);
UPDATE instance_template SET ScriptName = 'instance_eye_of_eternity' WHERE map = 616;
UPDATE creature_template SET ScriptName = 'mob_tribuna_controller' WHERE entry = 28234;
UPDATE creature_template SET ScriptName = 'mob_dark_rune_acolyte' WHERE entry IN (32886,32957,33110);
UPDATE creature_template SET ScriptName = 'mob_boombot' WHERE entry = 33346;
UPDATE creature_template SET ScriptName = 'mob_orange_gas_stalker' WHERE entry = 36659;
UPDATE creature_template SET ScriptName = 'npc_well_of_soul' WHERE entry = 36536;
UPDATE creature_template SET ScriptName = 'npc_unleashed_soul' WHERE entry = 36595;
UPDATE creature_template SET ScriptName = 'mob_soul_fragment' WHERE entry = 36535;
UPDATE instance_template SET ScriptName = 'instance_halls_of_reflection' WHERE map = 668;
UPDATE creature_template SET ScriptName = 'npc_frostworn_general' WHERE entry = 36723;
UPDATE creature_template SET ScriptName = 'npc_spiritual_reflection' WHERE entry IN (37068,37107);
UPDATE creature_template SET ScriptName = 'npc_queldelar_hor' WHERE entry = 37704; -- ???
UPDATE creature_template SET ScriptName = 'boss_falric' WHERE entry = 38112;
UPDATE creature_template SET ScriptName = 'boss_marwyn' WHERE entry = 38113;
UPDATE creature_template SET ScriptName = 'mob_shadowy_construct' WHERE entry = 23111;
UPDATE creature_template SET ScriptName = 'npc_twilight_volunteer' WHERE entry = 30385;
UPDATE creature_template SET ScriptName = 'mob_flame_orb' WHERE entry = 30702;
UPDATE creature_template SET ScriptName = 'mob_twisted_visage' WHERE entry IN (30621,30622,30623,30624,30625,31476,31477,31478,31479,31480);
UPDATE creature_template SET ScriptName = 'mob_ancient_void' WHERE entry = 30622;
UPDATE creature_template SET ScriptName = 'mob_assault_bot' WHERE entry = 34057;
UPDATE creature_template SET ScriptName = 'mob_dark_rune_evoker' WHERE entry = 32878;
UPDATE instance_template SET ScriptName = 'instance_forge_of_souls' WHERE map = 632;

/* still missing
2011-04-15 23:44:55 ERROR:SD2: Script registering but ScriptName outdoor_pvp_eastern_plaguelands is not assigned in database.
2011-04-15 23:44:55 ERROR:SD2: Script registering but ScriptName outdoor_pvp_silithus is not assigned in database.
2011-04-15 23:44:55 ERROR:SD2: Script registering but ScriptName at_tyrannus is not assigned in database.
2011-04-15 23:44:55 ERROR:SD2: Script registering but ScriptName npc_jaina_or_sylvanas_POSintro is not assigned in database.
2011-04-15 23:44:55 ERROR:SD2: Script registering but ScriptName npc_jaina_or_sylvana_POSoutro is not assigned in database.
2011-04-15 23:44:55 ERROR:SD2: Script registering but ScriptName npc_jaina_and_sylvana_FSintro is not assigned in database.
2011-04-15 23:44:55 ERROR:SD2: Script registering but ScriptName npc_jaina_and_sylvana_FSextro is not assigned in database.
2011-04-15 23:44:55 ERROR:SD2: Script registering but ScriptName npc_jaina_and_sylvana_HRintro is not assigned in database.
2011-04-15 23:44:55 ERROR:SD2: Script registering but ScriptName npc_jaina_and_sylvana_HRextro is not assigned in database.
2011-04-15 23:44:55 ERROR:SD2: Script registering but ScriptName boss_lich_king_hr is not assigned in database.
2011-04-15 23:44:55 ERROR:SD2: Script registering but ScriptName boss_lich_king_intro_hor is not assigned in database.
2011-04-15 23:44:55 ERROR:SD2: Script registering but ScriptName npc_undead_hor is not assigned in database.
*/
