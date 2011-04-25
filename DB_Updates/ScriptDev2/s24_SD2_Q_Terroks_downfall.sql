DELETE FROM `script_texts` WHERE `entry` BETWEEN '-1021838' AND '-1021845';

INSERT INTO `script_texts` (`entry`, `content_default`, `type`, `comment`) VALUES
('-1021839', 'Who calls me to this world? The stars are not yet aligned... my powers fail me! You will pay for this!', '0', 'Terokk - Spawn Text'),
('-1021840', 'Enemy sighted! Fall into formation and prepare for bombing maneuvers!', '1', 'Skyguard Ace - Spawn Yell'),
('-1021841', 'Show me what you\'re made of, $N!', '1', 'Terokk - Chosen one Text at Target'),
('-1021842', 'Quickly! Use the flames and support the ground troops. Its ancient magic should cleanse Terokk\'s shield.', '1', 'Skyguard Ace - When Terokk cast Immune'),
('-1021843', 'Kwa! You cannot kill me, I am immortal!', '1', 'Terokk - Cast Immune'),
('-1021844', 'Terokk becomes enraged as his shield shatters.', '3', 'Terokk - Remove Immune'),
('-1021845', 'They did it! Enemy down! Return to base!', '1', 'Skyguard Ace - After Terokk die');
