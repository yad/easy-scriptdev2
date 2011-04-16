-- Azjol-Nerub from MaxXx2021
UPDATE creature_template SET ScriptName = 'boss_hadronox' WHERE entry = 28921;

-- Anub'ar Warrior
INSERT INTO `creature_ai_Scripts` (`id`, `creature_id`, `event_type`, `event_inverse_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action1_type`, `action1_param1`, `action1_param2`, `action1_param3`, `action2_type`, `action2_param1`, `action2_param2`, `action2_param3`, `action3_type`, `action3_param1`, `action3_param2`, `action3_param3`, `comment`) VALUES
('2873203','28732','0','0','100','7','1000','2000','18000','27000','13','-100','1','0','11','52538','3','0','0','0','0','0','Anub\'ar Warrior - Cast Charge');
