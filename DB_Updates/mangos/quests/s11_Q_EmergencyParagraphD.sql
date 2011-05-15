# Quest: Emergency Protocol: Section 8.2, Paragraph D (11796)

UPDATE creature_template SET AIName='EventAI', unit_flags=unit_flags|33554432  WHERE entry IN (25847,25846,25845);

DELETE FROM creature WHERE id IN (25847,25846,25845);
INSERT INTO creature (id, map, spawnMask, phaseMask, modelid, equipment_id, position_x,	position_y, position_z,	orientation, spawntimesecs, spawndist, currentwaypoint, curhealth, curmana, DeathState,	MovementType) VALUES
(25845,571,1,1,17188,0,3606.57,5063,14.4285,0.785398,300,0,0,8982,3155,0,0),
(25845,571,1,1,17188,0,3739.39,5047.41,-0.865776,2.14675,300,0,0,8982,3155,0,0),
(25846,571,1,1,17188,0,3357.67,5049.81,14.1139,0.314159,300,0,0,8982,3155,0,0),
(25846,571,1,1,17188,0,3390.86,5135.81,14.4616,1.18682,300,0,0,8982,3155,0,0),
(25847,571,1,1,17188,0,3515.69,5016.58,-0.550566,1.74533,300,0,0,8982,3155,0,0),
(25847,571,1,1,17188,0,3475.71,4917.4,13.3331,4.93928,300,0,0,8982,3155,0,0);

DELETE FROM creature_ai_scripts WHERE creature_id IN (25847,25846,25845);
INSERT INTO creature_ai_scripts VALUES
(2584701,25847,8,0,100,1,46171,-1,0,0,33,25847,6,0,0,0,0,0,0,0,0,0,'Northwest Crash - Quest Credit'),
(2584601,25846,8,0,100,1,46171,-1,0,0,33,25846,6,0,0,0,0,0,0,0,0,0,'South Crash - Quest Credit'),
(2584501,25845,8,0,100,1,46171,-1,0,0,33,25845,6,0,0,0,0,0,0,0,0,0,'East Crash - Quest Credit');

DELETE FROM spell_script_target WHERE entry=46171;
INSERT INTO spell_script_target VALUES
(46171,1,25847),
(46171,1,25846),
(46171,1,25845);
