UPDATE `creature_template` SET `ScriptName` = 'boss_krystallus' WHERE `entry` =27977 LIMIT 1 ;

UPDATE `gameobject_template` SET `faction` = '114',`data0` = '0' WHERE `entry` =191293;
UPDATE `gameobject_template` SET `faction` = '0', `flags` = '0'  WHERE `entry` IN (193996,190586);
