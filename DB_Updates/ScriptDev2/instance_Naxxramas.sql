-- Instance Naxxramas
-- Instance last changes: a657
###################   boss_faerlina   ##########################################################

DELETE FROM script_texts WHERE entry IN (-1999776,-1999775);
INSERT INTO script_texts (`entry`,`content_default`,`type`,`comment`) VALUES
(-1999776,'%s goes into a frenzy!',3,'Grand Widow Faerlina - EMOTE_FRENZY'),
(-1999775,'%s is affected by Widow\'s Embrace!',3,'Grand Widow Faerlina - EMOTE_WIDOWS_EMBRACE');

###################   boss_gluth  ##############################################################

DELETE FROM script_texts WHERE entry IN (-1999781,-1999780,-1999779,-1533119);
INSERT INTO script_texts (`entry`,`content_default`,`type`,`comment`) VALUES
(-1999781, '%s deciamtes all nearby flesh',3,'Gluth - Decimated Players'),
(-1999780, '%s devours all nearby zombies',2,'Gluth - Decimated Zombie'),
(-1999779, '%s becomes enraged',3,'Gluth - Periodic Enrage'),
(-1533119, '%s spots zombie to devour!',2,'Gluth - Spoted Zombie');

####################   boss_sapphiron    #########################################################

DELETE FROM script_texts WHERE entry IN (-1999768,-1999767,-1533083,-1533082);
INSERT INTO script_texts (`entry`,`content_default`,`type`,`comment`) VALUES
(-1999768, '%s lifts off into the air',3,'Sapphiron goes up'),
(-1999767, '%s resumes to hit attacks',3,'Sapphiron returns to fight'),
(-1533083, '%s enrages!',3,'sapphiron EMOTE_ENRAGE'),
(-1533082,'%s takes in a deep breath...',3,'sapphiron EMOTE_BREATH');
