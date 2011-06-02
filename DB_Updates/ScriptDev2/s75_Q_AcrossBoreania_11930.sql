UPDATE creature_template SET ScriptName = 'npc_taunkale_evacuee' WHERE entry = 26167;
UPDATE quest_template SET StartScript = 11930 WHERE entry = 11930;
DELETE FROM quest_start_scripts WHERE id = 11930;
INSERT INTO quest_start_scripts (id,command,datalong,datalong2,comments) VALUES
(11930,15,46657,2,'Summon Taunka\'le Evacuee');

DELETE FROM script_texts WHERE entry BETWEEN -1999839 AND -1999832;
INSERT INTO script_texts (entry,content_default,TYPE,COMMENT) VALUES
(-1999839,'%s cries.',2,''),
(-1999838,'Stop!',0,''),
(-1999837,'%s places a hond on the ground.',2,''),
(-1999836,'What will become of us?',0,''),
(-1999835,'%s points northeast.',2,''),
(-1999834,'The smoke in the distance... It is Wartook. He is trying to tell us something...',0,''),
(-1999833,'No... It cannot be true...',0,''),
(-1999832,'Icemist... Not Icemist... NO!!! Come quickly, hero! Follow me!',0,'');
