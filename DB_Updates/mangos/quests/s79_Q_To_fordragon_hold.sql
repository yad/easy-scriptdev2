-- Support for quest 'To Fordragon Hold!' (12474)

DELETE FROM creature_involvedrelation WHERE quest=12474;
INSERT INTO creature_involvedrelation (id, quest) VALUES
(27872,12474);
