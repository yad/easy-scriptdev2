-- First Aid (129) fix trainers by Grandres 

-- Doctor Gustaf VanHowzen (12939) and Doctor Gregory Victor (12920) aren't trainers. 
DELETE FROM npc_trainer WHERE entry IN (12939,12920);
-- They are only quest givers.
UPDATE creature_template SET npcflag=npcflag|2 WHERE entry IN (12939,12920);

-- Burko (18990)Aresella (18991) add trainer and vendor flag.
UPDATE creature_template SET npcflag=npcflag|1|16|64, trainer_type=2 WHERE entry IN (18990,18991);

DELETE FROM npc_trainer WHERE entry IN 
(16662,22477,17214,17424,23734,18991,3373,26992,18990,6094,4211,
19478,33621,3181,33589,16272,4591,2329,19184,5150,29233,5759,16731,
28706,2798,5943,26956,2327,2326,5939);

-- FA Apprentice (3279)

INSERT INTO npc_trainer VALUES
(16662,3279,100,129,0,0), 	-- Alestus (16662)
(22477,3279,100,129,0,0), 	-- Anchorite Ensham (22477)
(17214,3279,100,129,0,0), 	-- Anchorite Fateema (17214)
(17424,3279,100,129,0,0), 	-- Anchorite Paetheus (17424)
(23734,3279,100,129,0,0), 	-- Anchorite Yazmina (23734)
(18991,3279,100,129,0,0), 	-- Aresella (18991)
(3373,3279,100,129,0,0),  	-- Arnok (3373)
(26992,3279,100,129,0,0), 	-- Brynna Wilson (26992)
(18990,3279,100,129,0,0), 	-- Burko (18990)
(6094,3279,100,129,0,0),	-- Byancie (6094)
(4211,3279,100,129,0,0),	-- Dannelor (4211)
(19478,3279,100,129,0,0),	-- Fera Palerunner (19478)
(33621,3279,100,129,0,0),	-- First Aid (33621)
(3181,3279,100,129,0,0),	-- Fremal Doohickey (3181)
(33589,3279,100,129,0,0),	-- Joseph Wilson (33589)
(16272,3279,100,129,0,0),	-- Kanaria (16272)
(4591,3279,100,129,0,0),	-- Mary Edras (4591)
(2329,3279,100,129,0,0),	-- Michelle Belle (2329)
(19184,3279,100,129,0,0),	-- Mildred Fletcher (19184)
(5150,3279,100,129,0,0),	-- Nissa Firestone (5150)
(29233,3279,100,129,0,0),	-- Nurse Applewood (29233)
(5759,3279,100,129,0,0),	-- Nurse Neela (5759)
(16731,3279,100,129,0,0),	-- Nus (16731)
(28706,3279,100,129,0,0),	-- Olisarra the Kind (28706)
(2798,3279,100,129,0,0),	-- Pand Stonebinder (2798)
(5943,3279,100,129,0,0),	-- Rawrk (5943)
(26956,3279,100,129,0,0),	-- Sally Tompkins (26956)
(2327,3279,100,129,0,0),	-- Shaina Fuller (2327)
(2326,3279,100,129,0,0),	-- Thamner Pol (2326)
(5939,3279,100,129,0,0),	-- Vira Younghoof (5939)

-- Heavy Linen Bandage

(16662,3276,100,129,40,0),
(22477,3276,100,129,40,0),
(17214,3276,100,129,40,0),
(17424,3276,100,129,40,0),
(23734,3276,100,129,40,0),
(18991,3276,100,129,40,0),
(3373,3276,100,129,40,0),
(26992,3276,100,129,40,0),
(18990,3276,100,129,40,0),
(6094,3276,100,129,40,0),
(4211,3276,100,129,40,0),
(19478,3276,100,129,40,0),
(33621,3276,100,129,40,0),
(3181,3276,100,129,40,0),
(33589,3276,100,129,40,0),
(16272,3276,100,129,40,0),
(4591,3276,100,129,40,0),
(2329,3276,100,129,40,0),
(19184,3276,100,129,40,0),
(5150,3276,100,129,40,0),
(29233,3276,100,129,40,0),
(5759,3276,100,129,40,0),
(16731,3276,100,129,40,0),
(28706,3276,100,129,40,0),
(2798,3276,100,129,40,0),
(5943,3276,100,129,40,0),
(26956,3276,100,129,40,0),
(2327,3276,100,129,40,0),
(2326,3276,100,129,40,0),
(5939,3276,100,129,40,0),

-- FA Journeyman (3280)

(16662,3280,500,129,50,0),
(22477,3280,500,129,50,0),
(17214,3280,500,129,50,0),
(17424,3280,500,129,50,0),
(23734,3280,500,129,50,0),
(18991,3280,500,129,50,0),
(3373,3280,500,129,50,0),
(26992,3280,500,129,50,0),
(18990,3280,500,129,50,0),
(6094,3280,500,129,50,0),
(4211,3280,500,129,50,0),
(19478,3280,500,129,50,0),
(33621,3280,500,129,50,0),
(3181,3280,500,129,50,0),
(33589,3280,500,129,50,0),
(16272,3280,500,129,50,0),
(4591,3280,500,129,50,0),
(2329,3280,500,129,50,0),
(19184,3280,500,129,50,0),
(5150,3280,500,129,50,0),
(29233,3280,500,129,50,0),
(5759,3280,500,129,50,0),
(16731,3280,500,129,50,0),
(28706,3280,500,129,50,0),
(2798,3280,500,129,50,0),
(5943,3280,500,129,50,0),
(26956,3280,500,129,50,0),
(2327,3280,500,129,50,0),

-- Anti-Venom (7934)

(16662,7934,250,129,80,0),
(22477,7934,250,129,80,0),
(17214,7934,250,129,80,0),
(17424,7934,250,129,80,0),
(23734,7934,250,129,80,0),
(18991,7934,250,129,80,0),
(3373,7934,250,129,80,0),
(26992,7934,250,129,80,0),
(18990,7934,250,129,80,0),
(6094,7934,250,129,80,0),
(4211,7934,250,129,80,0),
(19478,7934,250,129,80,0),
(33621,7934,250,129,80,0),
(3181,7934,250,129,80,0),
(33589,7934,250,129,80,0),
(16272,7934,250,129,80,0),
(4591,7934,250,129,80,0),
(2329,7934,250,129,80,0),
(19184,7934,250,129,80,0),
(5150,7934,250,129,80,0),
(29233,7934,250,129,80,0),
(5759,7934,250,129,80,0),
(16731,7934,250,129,80,0),
(28706,7934,250,129,80,0),
(2798,7934,250,129,80,0),
(5943,7934,250,129,80,0),
(26956,7934,250,129,80,0),
(2327,7934,250,129,80,0),
(2326,7934,250,129,80,0),
(5939,7934,250,129,80,0),

-- Wool Bandage (3277)

(16662,3277,250,129,80,0),
(22477,3277,250,129,80,0),
(17214,3277,250,129,80,0),
(17424,3277,250,129,80,0),
(23734,3277,250,129,80,0),
(18991,3277,250,129,80,0),
(3373,3277,250,129,80,0),
(26992,3277,250,129,80,0),
(18990,3277,250,129,80,0),
(6094,3277,250,129,80,0),
(4211,3277,250,129,80,0),
(19478,3277,250,129,80,0),
(33621,3277,250,129,80,0),
(3181,3277,250,129,80,0),
(33589,3277,250,129,80,0),
(16272,3277,250,129,80,0),
(4591,3277,250,129,80,0),
(2329,3277,250,129,80,0),
(19184,3277,250,129,80,0),
(5150,3277,250,129,80,0),
(29233,3277,250,129,80,0),
(5759,3277,250,129,80,0),
(16731,3277,250,129,80,0),
(28706,3277,250,129,80,0),
(2798,3277,250,129,80,0),
(5943,3277,250,129,80,0),
(26956,3277,250,129,80,0),
(2327,3277,250,129,80,0),
(2326,3277,250,129,80,0),
(5939,3277,250,129,80,0),

-- Heavy Wool Bandage (3278)

(16662,3278,1000,129,115,0),
(22477,3278,1000,129,115,0),
(17214,3278,1000,129,115,0),
(17424,3278,1000,129,115,0),
(23734,3278,1000,129,115,0),
(18991,3278,1000,129,115,0),
(3373,3278,1000,129,115,0),
(26992,3278,1000,129,115,0),
(18990,3278,1000,129,115,0),
(6094,3278,1000,129,115,0),
(4211,3278,1000,129,115,0),
(19478,3278,1000,129,115,0),
(33621,3278,1000,129,115,0),
(3181,3278,1000,129,115,0),
(33589,3278,1000,129,115,0),
(16272,3278,1000,129,115,0),
(4591,3278,1000,129,115,0),
(2329,3278,1000,129,115,0),
(19184,3278,1000,129,115,0),
(5150,3278,1000,129,115,0),
(29233,3278,1000,129,115,0),
(5759,3278,1000,129,115,0),
(16731,3278,1000,129,115,0),
(28706,3278,1000,129,115,0),
(2798,3278,1000,129,115,0),
(5943,3278,1000,129,115,0),
(26956,3278,1000,129,115,0),
(2327,3278,1000,129,115,0),
(2326,3278,1000,129,115,0),
(5939,3278,1000,129,115,0),

-- FA Expert (54254)

(16662,54254,1000,129,125,0),
(22477,54254,1000,129,125,0),
(17214,54254,1000,129,125,0),
(17424,54254,1000,129,125,0),
(23734,54254,1000,129,125,0),
(18991,54254,1000,129,125,0),
(3373,54254,1000,129,125,0),
(26992,54254,1000,129,125,0),
(18990,54254,1000,129,125,0),
(6094,54254,1000,129,125,0),
(4211,54254,1000,129,125,0),
(19478,54254,1000,129,125,0),
(33621,54254,1000,129,125,0),
(3181,54254,1000,129,125,0),
(33589,54254,1000,129,125,0),
(16272,54254,1000,129,125,0),
(4591,54254,1000,129,125,0),
(2329,54254,1000,129,125,0),
(19184,54254,1000,129,125,0),
(5150,54254,1000,129,125,0),
(29233,54254,1000,129,125,0),
(5759,54254,1000,129,125,0),
(16731,54254,1000,129,125,0),
(28706,54254,1000,129,125,0),
(2798,54254,1000,129,125,0),
(5943,54254,1000,129,125,0),
(26956,54254,1000,129,125,0),
(2327,54254,1000,129,125,0),
(2326,54254,1000,129,125,0),
(5939,54254,1000,129,125,0),

-- Silk Bandage (7928)

(16662,7928,5000,129,150,0),
(22477,7928,5000,129,150,0),
(17214,7928,5000,129,150,0),
(17424,7928,5000,129,150,0),
(23734,7928,5000,129,150,0),
(18991,7928,5000,129,150,0),
(3373,7928,5000,129,150,0),
(26992,7928,5000,129,150,0),
(18990,7928,5000,129,150,0),
(6094,7928,5000,129,150,0),
(4211,7928,5000,129,150,0),
(19478,7928,5000,129,150,0),
(33621,7928,5000,129,150,0),
(3181,7928,5000,129,150,0),
(33589,7928,5000,129,150,0),
(16272,7928,5000,129,150,0),
(4591,7928,5000,129,150,0),
(2329,7928,5000,129,150,0),
(19184,7928,5000,129,150,0),
(5150,7928,5000,129,150,0),
(29233,7928,5000,129,150,0),
(5759,7928,5000,129,150,0),
(16731,7928,5000,129,150,0),
(28706,7928,5000,129,150,0),
(2798,7928,5000,129,150,0),
(5943,7928,5000,129,150,0),
(26956,7928,5000,129,150,0),
(2327,7928,5000,129,150,0),
(2326,7928,5000,129,150,0),
(5939,7928,5000,129,150,0),

-- Heavy Silk Bandage (7929)

(16662,7929,25000,129,180,0),
(22477,7929,25000,129,180,0),
(17214,7929,25000,129,180,0),
(17424,7929,25000,129,180,0),
(23734,7929,25000,129,180,0),
(18991,7929,25000,129,180,0),
(3373,7929,25000,129,180,0),
(26992,7929,25000,129,180,0),
(18990,7929,25000,129,180,0),
(6094,7929,25000,129,180,0),
(4211,7929,25000,129,180,0),
(19478,7929,25000,129,180,0),
(33621,7929,25000,129,180,0),
(3181,7929,25000,129,180,0),
(33589,7929,25000,129,180,0),
(16272,7929,25000,129,180,0),
(4591,7929,25000,129,180,0),
(2329,7929,25000,129,180,0),
(19184,7929,25000,129,180,0),
(5150,7929,25000,129,180,0),
(29233,7929,25000,129,180,0),
(5759,7929,25000,129,180,0),
(16731,7929,25000,129,180,0),
(28706,7929,25000,129,180,0),
(2798,7929,25000,129,180,0),
(5943,7929,25000,129,180,0),
(26956,7929,25000,129,180,0),
(2327,7929,25000,129,180,0),
(2326,7929,25000,129,180,0),
(5939,7929,25000,129,180,0),

-- FA Artisan (10847)

(16662,10847,25000,129,200,35),
(22477,10847,25000,129,200,35),
(17214,10847,25000,129,200,35),
(17424,10847,25000,129,200,35),
(23734,10847,25000,129,200,35),
(18991,10847,25000,129,200,35),
(3373,10847,25000,129,200,35),
(26992,10847,25000,129,200,35),
(18990,10847,25000,129,200,35),
(6094,10847,25000,129,200,35),
(4211,10847,25000,129,200,35),
(19478,10847,25000,129,200,35),
(33621,10847,25000,129,200,35),
(3181,10847,25000,129,200,35),
(33589,10847,25000,129,200,35),
(16272,10847,25000,129,200,35),
(4591,10847,25000,129,200,35),
(2329,10847,25000,129,200,35),
(19184,10847,25000,129,200,35),
(5150,10847,25000,129,200,35),
(29233,10847,25000,129,200,35),
(5759,10847,25000,129,200,35),
(16731,10847,25000,129,200,35),
(28706,10847,25000,129,200,35),
(2798,10847,25000,129,200,35),
(5943,10847,25000,129,200,35),
(26956,10847,25000,129,200,35),
(2327,10847,25000,129,200,35),
(2326,10847,25000,129,200,35),
(5939,10847,25000,129,200,35),

-- Mageweave Bandage (10840)

(16662,10840,60000,129,210,0),
(22477,10840,60000,129,210,0),
(17214,10840,60000,129,210,0),
(17424,10840,60000,129,210,0),
(23734,10840,60000,129,210,0),
(18991,10840,60000,129,210,0),
(3373,10840,60000,129,210,0),
(26992,10840,60000,129,210,0),
(18990,10840,60000,129,210,0),
(6094,10840,60000,129,210,0),
(4211,10840,60000,129,210,0),
(19478,10840,60000,129,210,0),
(33621,10840,60000,129,210,0),
(3181,10840,60000,129,210,0),
(33589,10840,60000,129,210,0),
(16272,10840,60000,129,210,0),
(4591,10840,60000,129,210,0),
(2329,10840,60000,129,210,0),
(19184,10840,60000,129,210,0),
(5150,10840,60000,129,210,0),
(29233,10840,60000,129,210,0),
(5759,10840,60000,129,210,0),
(16731,10840,60000,129,210,0),
(28706,10840,60000,129,210,0),
(2798,10840,60000,129,210,0),
(5943,10840,60000,129,210,0),
(26956,10840,60000,129,210,0),
(2327,10840,60000,129,210,0),
(2326,10840,60000,129,210,0),
(5939,10840,60000,129,210,0),

-- Heavy Mageweave Bandage (10841)
(16662,10841,100000,129,240,0),
(22477,10841,100000,129,240,0),
(17214,10841,100000,129,240,0),
(17424,10841,100000,129,240,0),
(23734,10841,100000,129,240,0),
(18991,10841,100000,129,240,0),
(3373,10841,100000,129,240,0),
(26992,10841,100000,129,240,0),
(18990,10841,100000,129,240,0),
(6094,10841,100000,129,240,0),
(4211,10841,100000,129,240,0),
(19478,10841,100000,129,240,0),
(33621,10841,100000,129,240,0),
(3181,10841,100000,129,240,0),
(33589,10841,100000,129,240,0),
(16272,10841,100000,129,240,0),
(4591,10841,100000,129,240,0),
(2329,10841,100000,129,240,0),
(19184,10841,100000,129,240,0),
(5150,10841,100000,129,240,0),
(29233,10841,100000,129,240,0),
(5759,10841,100000,129,240,0),
(16731,10841,100000,129,240,0),
(28706,10841,100000,129,240,0),
(2798,10841,100000,129,240,0),
(5943,10841,100000,129,240,0),
(26956,10841,100000,129,240,0),
(2327,10841,100000,129,240,0),
(2326,10841,100000,129,240,0),
(5939,10841,100000,129,240,0),

-- Runecloth Bandage (18629)

(16662,18629,125000,129,260,0),
(22477,18629,125000,129,260,0),
(17214,18629,125000,129,260,0),
(17424,18629,125000,129,260,0),
(23734,18629,125000,129,260,0),
(18991,18629,125000,129,260,0),
(3373,18629,125000,129,260,0),
(26992,18629,125000,129,260,0),
(18990,18629,125000,129,260,0),
(6094,18629,125000,129,260,0),
(4211,18629,125000,129,260,0),
(19478,18629,125000,129,260,0),
(33621,18629,125000,129,260,0),
(3181,18629,125000,129,260,0),
(33589,18629,125000,129,260,0),
(16272,18629,125000,129,260,0),
(4591,18629,125000,129,260,0),
(2329,18629,125000,129,260,0),
(19184,18629,125000,129,260,0),
(5150,18629,125000,129,260,0),
(29233,18629,125000,129,260,0),
(5759,18629,125000,129,260,0),
(16731,18629,125000,129,260,0),
(28706,18629,125000,129,260,0),
(2798,18629,125000,129,260,0),
(5943,18629,125000,129,260,0),
(26956,18629,125000,129,260,0),
(2327,18629,125000,129,260,0),
(2326,18629,125000,129,260,0),
(5939,18629,125000,129,260,0),

-- Heavy Runecloth Bandage (18630)

(16662,18630,150000,129,290,0),
(22477,18630,150000,129,290,0),
(17214,18630,150000,129,290,0),
(17424,18630,150000,129,290,0),
(23734,18630,150000,129,290,0),
(18991,18630,150000,129,290,0),
(3373,18630,150000,129,290,0),
(26992,18630,150000,129,290,0),
(18990,18630,150000,129,290,0),
(6094,18630,150000,129,290,0),
(4211,18630,150000,129,290,0),
(19478,18630,150000,129,290,0),
(33621,18630,150000,129,290,0),
(3181,18630,150000,129,290,0),
(33589,18630,150000,129,290,0),
(16272,18630,150000,129,290,0),
(4591,18630,150000,129,290,0),
(2329,18630,150000,129,290,0),
(19184,18630,150000,129,290,0),
(5150,18630,150000,129,290,0),
(29233,18630,150000,129,290,0),
(5759,18630,150000,129,290,0),
(16731,18630,150000,129,290,0),
(28706,18630,150000,129,290,0),
(2798,18630,150000,129,290,0),
(5943,18630,150000,129,290,0),
(26956,18630,150000,129,290,0),
(2327,18630,150000,129,290,0),
(2326,18630,150000,129,290,0),
(5939,18630,150000,129,290,0),

-- FA Master (54255)

(23734,27028,100000,129,300,0),
(18991,27028,100000,129,300,0),
(26992,27028,100000,129,300,0),
(18990,27028,100000,129,300,0),
(33589,27028,100000,129,300,0),
(29233,27028,100000,129,300,0),
(28706,27028,100000,129,300,0),
(26956,27028,100000,129,300,0),

-- Netherweave Bandage (27032)

(23734,27032,200000,129,300,0),
(18991,27032,200000,129,300,0),
(26992,27032,200000,129,300,0),
(18990,27032,200000,129,300,0),
(33589,27032,200000,129,300,0),
(29233,27032,200000,129,300,0),
(28706,27032,200000,129,300,0),
(26956,27032,200000,129,300,0),
-- Heavy Netherweave Bandage (27033)

(23734,27033,400000,129,330,0),
(18991,27033,400000,129,330,0),
(26992,27033,400000,129,330,0),
(18990,27033,400000,129,330,0),
(33589,27033,400000,129,330,0),
(29233,27033,400000,129,330,0),
(28706,27033,400000,129,330,0),
(26956,27033,400000,129,330,0),

-- Frostweave Bandage (45545)

(23734,45545,500000,129,350,0),
(26992,45545,500000,129,350,0),
(33589,45545,500000,129,350,0),
(29233,45545,500000,129,350,0),
(28706,45545,500000,129,350,0),
(26956,45545,500000,129,350,0),

-- FA Grand Master (50299)

(23734,45542,350000,129,350,0),
(26992,45542,350000,129,350,0),
(33589,45542,350000,129,350,0),
(29233,45542,350000,129,350,0),
(28706,45542,350000,129,350,0),
(26956,45542,350000,129,350,0);