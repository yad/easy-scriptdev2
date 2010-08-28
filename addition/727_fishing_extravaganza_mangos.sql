-- set ai for riggle
UPDATE `creature_template` SET `ScriptName`='npc_riggle_bassbait' WHERE entry=15077;
-- updated start time for event Fishing Extravaganza
UPDATE `game_event` SET `start_time`='2009-06-14 14:00:00', `occurence`=10080, `length`=120 where entry=15;
-- Create Jang
DELETE FROM `creature` WHERE `id`=15078;
INSERT INTO `creature` (`id`,`map`,`spawnMask`,`phaseMask`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`DeathState`,`MovementType`) VALUES
(15078, 0, 1, 1, 0, 1711, -14438, 473.661, 15.3137, 3.65362, 25, 0, 0, 3200, 0, 0, 0);
-- Create Fishbot 5000
DELETE FROM `creature` WHERE `id`=15079;
INSERT INTO `creature` (`id`,`map`,`spawnMask`,`phaseMask`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`DeathState`,`MovementType`) VALUES
(15079, 0, 1, 1, 0, 0, -14440, 477.446, 15.25, 3.71802, 25, 0, 0, 2600, 0, 0, 0);
-- Create Riggle Bassbait
DELETE FROM `creature` WHERE `id`=15077;
INSERT INTO `creature` (`id`,`map`,`spawnMask`,`phaseMask`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`DeathState`,`MovementType`) VALUES
(15077, 0, 1, 1, 0, 13, -14439.3, 475.42, 15.892, 3.68503, 25, 0, 0, 3700, 0, 0, 0);
-- Create Soapbox gameobject
DELETE FROM `gameobject` WHERE `id`=180403;
INSERT INTO `gameobject` (`id`,`map`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`) VALUES
(180403, 0, 1, 1, -14439.3, 475.42, 15.2791, 3.68503, 0, 0, 0.963311, -0.268388, 25, 0, 1);
