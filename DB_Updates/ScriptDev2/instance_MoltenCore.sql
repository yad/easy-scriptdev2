-- Instance Molten Core
-- Instance last changes: a143

-- script texts
DELETE FROM `script_texts` WHERE `entry` = -1409019;
INSERT INTO `script_texts` (`entry`, `content_default`, `sound`, `type`, `language`, `emote`, `comment`) VALUES
(-1409019, "Very well, $N.", 0, 0, 0, 0, "majordomo SAY_GOSSIP_SUMMON");

-- Emote Yell for Ragnaros (very nervous guy)
UPDATE `script_texts` SET `emote` = 1 WHERE `entry` BETWEEN -1409018 AND -1409009;
