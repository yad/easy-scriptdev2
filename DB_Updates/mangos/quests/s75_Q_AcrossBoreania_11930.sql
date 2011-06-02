UPDATE creature_template SET ScriptName = 'npc_taunkale_evacuee' WHERE entry = 26167;
UPDATE quest_template SET StartScript = 11930 WHERE entry = 11930;
DELETE FROM quest_start_scripts WHERE id = 11930;
INSERT INTO quest_start_scripts (id,command,datalong,datalong2,comments) VALUES
(11930,15,46657,2,'Summon Taunka\'le Evacuee');
