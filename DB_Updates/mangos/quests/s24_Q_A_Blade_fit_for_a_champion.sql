DELETE FROM creature_ai_scripts WHERE creature_id IN (33211,33224,33220);
INSERT INTO creature_ai_scripts VALUES 
(3321101, 33211, 22,0,100,1, 58,0,0,0, 11,62536,6,6, 11,62537,0,6, 41,30000,0,0,'Lake Frog - Responce Frog Kiss on kiss emote - frog love visual aura'),
(3322401, 33224, 22,0,100,1, 58,0,0,0, 11,62550,0,6, 36,33220,0,0, 1,-332111,0,0, 'Lake Frog - Responce Frog Kiss on kiss emote - transform in Maiden of Ashwood Lake'),
(3322402, 33224, 22,0,100,1, 58,0,0,0, 41,30000,0,0, 0,0,0,0, 0,0,0,0, 'Lake Frog - Forced Despawn');

UPDATE creature_template SET
npcflag = npcflag|1,
gossip_menu_id = 33220
WHERE entry = 33220;

DELETE FROM gossip_menu WHERE entry = 33220;
INSERT INTO gossip_menu (entry,text_id) VALUES
(33220,14319);

DELETE FROM gossip_menu_option WHERE menu_id = 33220;
INSERT INTO gossip_menu_option (menu_id,id,option_icon,option_text,option_id,npc_option_npcflag,action_script_id,cond_1,cond_1_val_1) VALUES
(33220,0,0,'Glad to help, my Lady. I\'m told you were once the guardian of fabled sword. Do you know where I might find it?', 1,1,33220,9,13603),
(33220,1,0,'Glad to help, my Lady. I\'m told you were once the guardian of fabled sword. Do you know where I might find it?', 1,1,33220,9,13666),
(33220,2,0,'Glad to help, my Lady. I\'m told you were once the guardian of fabled sword. Do you know where I might find it?', 1,1,33220,9,13673),
(33220,3,0,'Glad to help, my Lady. I\'m told you were once the guardian of fabled sword. Do you know where I might find it?', 1,1,33220,9,13741),
(33220,4,0,'Glad to help, my Lady. I\'m told you were once the guardian of fabled sword. Do you know where I might find it?', 1,1,33220,9,13746),
(33220,5,0,'Glad to help, my Lady. I\'m told you were once the guardian of fabled sword. Do you know where I might find it?', 1,1,33220,9,13752),
(33220,6,0,'Glad to help, my Lady. I\'m told you were once the guardian of fabled sword. Do you know where I might find it?', 1,1,33220,9,13757),
(33220,7,0,'Glad to help, my Lady. I\'m told you were once the guardian of fabled sword. Do you know where I might find it?', 1,1,33220,9,13762),
(33220,8,0,'Glad to help, my Lady. I\'m told you were once the guardian of fabled sword. Do you know where I might find it?', 1,1,33220,9,13768),
(33220,9,0,'Glad to help, my Lady. I\'m told you were once the guardian of fabled sword. Do you know where I might find it?', 1,1,33220,9,13773),
(33220,10,0,'Glad to help, my Lady. I\'m told you were once the guardian of fabled sword. Do you know where I might find it?', 1,1,33220,9,13778),
(33220,11,0,'Glad to help, my Lady. I\'m told you were once the guardian of fabled sword. Do you know where I might find it?', 1,1,33220,9,13783);
 
 DELETE FROM gossip_scripts WHERE id = 33220;
 INSERT INTO gossip_scripts (id,command,datalong,comments) VALUES
 (33220,15,62554,'Maiden of Ashwood Lake - award sword by Summon Ashwood Brand');

 -- Lake Frogs
UPDATE creature_template SET AIName = 'EventAI' WHERE entry IN (33211,33224);

DELETE FROM creature_ai_texts WHERE entry = -332111;
INSERT INTO creature_ai_texts VALUES (-332111, 'Can it really be? Free after all this years?', NULL, NULL, NULL, NULL, NULL, NULL, NULL,'', 0, 0, 0, 0, 33211);
