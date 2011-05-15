UPDATE creature_template SET
ScriptName='',
AIName = 'EventAI'
WHERE entry = 29856;

UPDATE creature_template SET
AIName = 'EventAI',
ScriptName='npc_deacying_ghoul'
WHERE entry = 28565;

DELETE FROM creature_ai_scripts WHERE creature_id = 28591;
INSERT INTO creature_ai_scripts VALUES 
(2859101, 28591, 1,0,100,1, 0,0,10000,10000, 11,52037,0,6, 0,0,0,0, 0,0,0,0, 'Ghoul Feeding KC Bunny - attract ghoul');

DELETE FROM spell_script_target WHERE entry = 52037;
INSERT INTO spell_script_target VALUES
(52037,1,28565);
