-- Wormhole (portal creature)
UPDATE creature_template SET 
npcflag = npcflag|1,
gossip_menu_id = 10668
WHERE entry = 35646;

DELETE FROM gossip_menu WHERE entry = 35646;
INSERT INTO gossip_menu (entry, text_id) VALUES
(35646,14785);

DELETE FROM gossip_menu_option WHERE menu_id = 10668;
INSERT INTO gossip_menu_option (menu_id,id,option_icon,option_text,option_id,npc_option_npcflag,action_script_id, cond_1,cond_1_val_1,cond_1_val_2, cond_2,cond_2_val_1,cond_2_val_2) VALUES
(10668, 0, 0, 'Borean Tundra', 1, 1, 1066801,  2,48933,1, 7,202,415),
(10668, 1, 0, 'Howling Fjord', 1, 1, 1066802,  2,48933,1, 7,202,415),
(10668, 2, 0, 'Sholazar Basin', 1, 1, 1066803,  2,48933,1, 7,202,415),
(10668, 3, 0, 'Icecrown', 1, 1, 1066804,  2,48933,1, 7,202,415),
(10668, 4, 0, 'Storm Peaks', 1, 1, 1066805,  2,48933,1, 7,202,415),
(10668, 5, 0, 'Underground...', 1, 1, 1066806,  2,48933,1, 7,202,415);

DELETE FROM gossip_scripts WHERE id IN (1066801,1066802,1066803,1066804,1066805,1066806);
INSERT INTO gossip_scripts (id,command,datalong,comments) VALUES
(1066801, 15, 67834,'Wormhole Generator - tele Borean Tundra'),
(1066802, 15, 67838,'Wormhole Generator - tele Howling Fjord'),
(1066803, 15, 67835,'Wormhole Generator - tele Sholazar Basin'),
(1066804, 15, 67836,'Wormhole Generator - tele Icecrown'),
(1066805, 15, 67837,'Wormhole Generator - tele Storm Peaks'),
(1066806, 15, 68081,'Wormhole Generator - tele Underground...');
