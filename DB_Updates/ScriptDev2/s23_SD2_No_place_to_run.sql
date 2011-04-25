DELETE from script_texts WHERE entry IN(-1799900,-1799901);
INSERT into script_texts (entry,content_default,type,comment) VALUES
(-1799900,'The destructive ward gains the power',3,'WARD_EMOTE_1'),
(-1799901,'The destructive ward is fully charged',3,'WARD_EMOTE_2');
