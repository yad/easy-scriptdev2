-- NPC Sergeant Kregga

UPDATE creature_template SET minlevel = 80, maxlevel = 80, minhealth = 12600, maxhealth = 12600, armor = 9729, faction_A = 1085, faction_H = 1085, mindmg = 422, maxdmg = 586, attackpower = 642, minrangedmg = 345, rangedattackpower = 103 WHERE entry = 31440;

DELETE FROM creature WHERE id = 31440;
INSERT INTO creature (id, map, spawnMask, phaseMask, modelid, equipment_id, position_x, position_y, position_z, orientation, spawntimesecs, spawndist, currentwaypoint, curhealth, curmana, DeathState, MovementType)
VALUES (31440,571,1,1,0,0,5907.31543,1998.581421,517.083374,3.132847,180,0,0,12600,0,0,0);
