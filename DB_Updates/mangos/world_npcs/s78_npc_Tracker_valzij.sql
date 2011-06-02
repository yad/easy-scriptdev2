-- NPC Tracker Val'zij

UPDATE creature_template SET minlevel = 80, maxlevel = 80, minhealth = 12600, maxhealth = 12600, armor = 9729, faction_A = 1085, faction_H = 1085, mindmg = 422, maxdmg = 586, attackpower = 642, minrangedmg = 345, rangedattackpower = 103 WHERE entry = 30469;

DELETE FROM creature WHERE id = 30469;
INSERT INTO creature (id, map, spawnMask, phaseMask, modelid, equipment_id, position_x, position_y, position_z, orientation, spawntimesecs, spawndist, currentwaypoint, curhealth, curmana, DeathState, MovementType)
VALUES (30469,571,1,1,0,0,7620.71,-1608.97,969.206,1.02267,180,0,0,12600,0,0,0);
