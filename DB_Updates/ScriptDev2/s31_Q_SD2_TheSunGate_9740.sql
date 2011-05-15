-- Stabilize Sun Gate I - IV targeted on npc Sun Gate Event Controller
DELETE FROM spell_script_target WHERE entry IN (31411,31412,31413,31414);
INSERT INTO spell_script_target VALUES
(31411,1,17889),
(31412,1,17889),
(31413,1,17889),
(31414,1,17889);

-- Stabilize Sun Gate I - IV to npc portal controllers
DELETE FROM creature_addon WHERE guid IN (SELECT guid FROM creature WHERE id = 17886);
INSERT INTO creature_addon (guid, auras) VALUES
(63582,'31411'),
(63583,'31412'),
(63584,'31413'),
(63585,'31414');

UPDATE gameobject SET spawntimesecs = 90 WHERE id IN (182026,184850);
UPDATE creature SET spawntimesecs = 90 WHERE id = 17886;

-- npc portal controllers should be spawned at go portal controllers positions that visual effect of Stabilize Sun Gate spell match
UPDATE creature SET
position_x = -2098.62,
position_y = -10688.5,
position_z = 65.2181
WHERE guid = 63582;

UPDATE creature SET
position_x = -2147.35,
position_y = -10741.1,
position_z = 73.9034
WHERE guid = 63583;

UPDATE creature SET
position_x = -2128.94,
position_y = -10726,
position_z = 66.3358
WHERE guid = 63584;

UPDATE creature SET
position_x = -2107.07,
position_y = -10703,
position_z = 65.1894
WHERE guid = 63585;

UPDATE gameobject_template SET ScriptName = 'go_sunhawk_portal_controller' WHERE entry = 184850;
