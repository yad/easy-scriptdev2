UPDATE world_template SET scriptName='worldmap_eastern_kingdoms' WHERE map=0;
UPDATE world_template SET scriptName='worldmap_kalimdor' WHERE map=1;
UPDATE world_template SET scriptName='worldmap_outland' WHERE map=530;

DELETE FROM scripted_event_id WHERE id IN (11406,11405,11400,11399,11404,11403,11402,11401);
INSERT INTO scripted_event_id VALUES
(11406,'event_broken_hill_tower'),
(11405,'event_broken_hill_tower'),
(11400,'event_broken_hill_tower'),
(11399,'event_broken_hill_tower'),
(11404,'event_broken_hill_tower'),
(11403,'event_broken_hill_tower'),
(11402,'event_broken_hill_tower'),
(11401,'event_broken_hill_tower');
DELETE FROM scripted_event_id WHERE id IN (11390,11389,11384,11383,11388,11387,11386,11385);
INSERT INTO scripted_event_id VALUES
(11390,'event_stadium_tower'),
(11389,'event_stadium_tower'),
(11384,'event_stadium_tower'),
(11383,'event_stadium_tower'),
(11388,'event_stadium_tower'),
(11387,'event_stadium_tower'),
(11386,'event_stadium_tower'),
(11385,'event_stadium_tower');
DELETE FROM scripted_event_id WHERE id IN (11398,11397,11392,11391,11396,11395,11394,11393);
INSERT INTO scripted_event_id VALUES
(11398,'event_overlook_tower'),
(11397,'event_overlook_tower'),
(11392,'event_overlook_tower'),
(11391,'event_overlook_tower'),
(11396,'event_overlook_tower'),
(11395,'event_overlook_tower'),
(11394,'event_overlook_tower'),
(11393,'event_overlook_tower');
DELETE FROM scripted_event_id WHERE id IN (10567,10564,10687,10688,10701,10700,11153,11152);
INSERT INTO scripted_event_id VALUES
(10567,'event_plaguewood_tower'),
(10564,'event_plaguewood_tower'),
(10687,'event_plaguewood_tower'),
(10688,'event_plaguewood_tower'),
(10701,'event_plaguewood_tower'),
(10700,'event_plaguewood_tower'),
(11153,'event_plaguewood_tower'),
(11152,'event_plaguewood_tower');

UPDATE gameobject_template SET ScriptName='go_silithyst' WHERE entry IN (181597,181598);
DELETE FROM scripted_areatrigger WHERE entry IN (4162,4168);
INSERT INTO scripted_areatrigger VALUES
(4162,'at_silithus'),
(4168,'at_silithus');

DELETE FROM gameobject WHERE id IN (182173, 182174, 182175, 182096, 182097, 182098, 181899);
INSERT INTO gameobject
  (id, map, spawnMask, phaseMask, position_x, position_y, position_z, orientation, rotation0, rotation1, rotation2, rotation3, spawntimesecs, animprogress, state)
VALUES
  (182173, 530, 1, 1, "-290,016", "3702,42", "56,6729", "0,034907", 0, 0, "0,017452", "0,999848", 180, 100, 1);

INSERT INTO gameobject
  (id, map, spawnMask, phaseMask, position_x, position_y, position_z, orientation, rotation0, rotation1, rotation2, rotation3, spawntimesecs, animprogress, state)
VALUES
  (182174, 530, 1, 1, "-184,889", "3476,93", "38,205", "-0,017453", 0, 0, "0,008727", "-0,999962", 180, 100, 1);

INSERT INTO gameobject
  (id, map, spawnMask, phaseMask, position_x, position_y, position_z, orientation, rotation0, rotation1, rotation2, rotation3, spawntimesecs, animprogress, state)
VALUES
  (182175, 530, 1, 1, "-471,462", "3451,09", "34,6432", "0,174533", 0, 0, "0,087156", "0,996195", 180, 100, 1);

INSERT INTO gameobject
  (id, map, spawnMask, phaseMask, position_x, position_y, position_z, orientation, rotation0, rotation1, rotation2, rotation3, spawntimesecs, animprogress, state)
VALUES
  (182097, 0, 1, 1, "2574,51", "-4794,89", "144,704", "-1,45003", "-0,097056", "0,095578", "-0,656229", "0,742165", 180, 100, 1);

INSERT INTO gameobject
  (id, map, spawnMask, phaseMask, position_x, position_y, position_z, orientation, rotation0, rotation1, rotation2, rotation3, spawntimesecs, animprogress, state)
VALUES
  (182096, 0, 1, 1, "1860,85", "-3731,23", "196,716", "-2,53214", "0,033967", "-0,131914", "0,944741", "-0,298177", 180, 100, 1);

INSERT INTO gameobject
  (id, map, spawnMask, phaseMask, position_x, position_y, position_z, orientation, rotation0, rotation1, rotation2, rotation3, spawntimesecs, animprogress, state)
VALUES
  (182098, 0, 1, 1, "2962,71", "-3042,31", "154,789", "2,08426", "-0,074807", "-0,113837", "0,855928", "0,49883", 900, 100, 1);

INSERT INTO gameobject
  (id, map, spawnMask, phaseMask, position_x, position_y, position_z, orientation, rotation0, rotation1, rotation2, rotation3, spawntimesecs, animprogress, state)
VALUES
  (181899, 0, 1, 1, "3181,08", "-4379,36", "174,123", "-2,03472", "-0,065392", "0,119494", "-0,842275", "0,521553", 900, 100, 1);