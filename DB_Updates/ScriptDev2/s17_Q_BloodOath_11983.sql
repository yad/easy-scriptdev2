DELETE FROM script_texts WHERE entry = -1999926;
INSERT INTO script_texts (entry, content_default, TYPE, emote, COMMENT) VALUES
(-1999926, 'Victory or death! For the Horde!',1,22,'npc_taunkala_refuge - quset credit yell');

UPDATE creature_template SET
npcflag = npcflag |1,
ScriptName = 'npc_taunkale_refuge'
WHERE entry IN (26179,26184);
