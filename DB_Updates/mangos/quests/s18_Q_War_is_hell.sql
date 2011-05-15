-- support for War is Hell - quest 11270

-- set  EAI for Alliance (hooray) and Forsaken Corpse 

UPDATE creature_template SET  AIName='EventAI' WHERE entry IN (24009,24010);

DELETE FROM creature_ai_scripts WHERE creature_id IN (24009, 24010);
INSERT IGNORE INTO creature_ai_scripts (id, creature_id,event_type,event_inverse_phase_mask,event_chance,event_flags,event_param1,event_param2,event_param3,event_param4,action1_type,action1_param1,action1_param2,action1_param3,action2_type,action2_param1,action2_param2,action2_param3,action3_type,action3_param1,action3_param2,action3_param3,comment)
VALUES (2400901,24009,8,0,100,1,42793,-1,60000,60000,33,24008,6,0,41,0,0,0,0,0,0,0,'Alliance Corpse - kill credit');
INSERT IGNORE INTO creature_ai_scripts (id, creature_id,event_type,event_inverse_phase_mask,event_chance,event_flags,event_param1,event_param2,event_param3,event_param4,action1_type,action1_param1,action1_param2,action1_param3,action2_type,action2_param1,action2_param2,action2_param3,action3_type,action3_param1,action3_param2,action3_param3,comment)
VALUES (2401001,24010,8,0,100,1,42793,-1,60000,60000,33,24008,6,0,41,0,0,0,0,0,0,0,'Forsaken Corpse - kill credit');

-- set correct type of target for spell 42793
UPDATE IGNORE `spell_script_target` SET type = 1 WHERE entry = 42793;