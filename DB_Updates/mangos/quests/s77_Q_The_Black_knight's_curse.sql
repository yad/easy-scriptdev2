-- Support for quest The Black Knight's Curse (14016)

-- Add EventAi for Black Knight's Grave (34735)
UPDATE creature_template SET AIName='EventAI' WHERE entry=34735;
DELETE FROM creature_ai_scripts WHERE id=3473500;
INSERT INTO creature_ai_scripts VALUES
(3473500,34735,10,0,100,1,1,15,180000,180000,32,35116,0,35116,32,35127,1,35127,0,0,0,0,'Black Knight Grave  - on LOS 15y summon Cult Saboteur and Cult Assassin');

-- Add texts , EventAi and small changes for Cult Saboteur (35116)
-- Text
DELETE FROM creature_ai_texts WHERE entry IN (-351161,-351162,-351163);
INSERT INTO creature_ai_texts VALUES
(-351161,'What do you think you\'re doing?',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,0,0,0,'Cult Saboteur Say1'),
(-351162,'You\'re too late to stop our plan. ',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,0,0,0,'Cult Saboteur Say2'),
(-351163,'See to it that I\'m not followed.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,0,0,0,'Cult Saboteur Say3');

-- EventAi
DELETE FROM creature_ai_scripts WHERE id IN (3511600,3511601,3511602);
INSERT INTO creature_ai_scripts VALUES
(3511600,35116,1,0,100,0,1000,1000,0,0,1,-351161,0,0,0,0,0,0,0,0,0,0,'Cult Saboteur Say1'),
(3511601,35116,1,0,100,0,5000,5000,0,0,1,-351162,0,0,0,0,0,0,0,0,0,0,'Cult Saboteur Say2'),
(3511602,35116,1,0,100,0,9000,9000,0,0,1,-351163,0,0,0,0,0,0,0,0,0,0,'Cult Saboteur Say2');

UPDATE `creature_template` SET minhealth=12600, maxhealth=12600,equipment_id=3334,minlevel=80,maxlevel=80,AIName='EventAI'  WHERE entry=35116;

-- Cult Saboteur Summon
DELETE FROM creature_ai_summons WHERE id=35116;
INSERT INTO creature_ai_summons VALUES
(35116,8452.4902,459.6663,596.07244,0.437163,15000,'Cult Saboteur Spawn The Black Knight Curse');

-- Add texts, EventAi and small changes for Cult Assassin (35127)
-- Text
DELETE FROM creature_ai_texts WHERE entry=-351271;
INSERT INTO creature_ai_texts VALUES
(-351271,'Gladly.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0,0,0,0,'Cult Assassin Say1');
-- EventAi
DELETE FROM creature_ai_scripts WHERE id IN (3512700,3512701,3512702,3512703);
INSERT INTO creature_ai_scripts VALUES
(3512700,35127,1,0,100,0,12000,12000,0,0,1,-351271,0,0,2,128,0,0,0,0,0,0,'Cult Assassin Say1 and change faction'),
(3512701,35127,0,0,100,1,1000,2000,16000,17000,11,60842,0,0,0,0,0,0,0,0,0,0,'Cult Assassin cast Expose Armor in combat'),
(3512702,35127,0,0,100,1,3000,5000,3000,5000,11,14873,0,0,0,0,0,0,0,0,0,0,'Cult Assassin cast Sinister Strike in combat'),
(3512703,35127,6,0,100,1,0,0,0,0,15,14016,6,0,0,0,0,0,0,0,0,0,'Cult Assassin gives quest credit upon death');

UPDATE `creature_template` SET minhealth=12600,maxhealth=12600,minmana=7988,maxmana=7988,minlevel=80,maxlevel=80,equipment_id=479,mindmg=346,maxdmg=499,attackpower=287,baseattacktime=2000, AIName='EventAI'  WHERE entry=35127;

-- Cult Assassin Summon
DELETE FROM creature_ai_summons WHERE id=35127;
INSERT INTO creature_ai_summons VALUES
(35127,8456.0713,458.9455,596.0724,2.083357,180000,'Cult Assassin Spawn The Black Knight Curse');

UPDATE quest_template SET QuestFlags=QuestFlags | 4 WHERE entry=14016;
