-- Fix for You've Really Done It This Time, Kul [AT] (by Bastek)
DELETE FROM script_texts WHERE entry IN (-1780002, -1780003);
INSERT INTO script_texts (entry, content_default, comment) VALUES
(-1780002, 'I thought I\'d never make it out of here alive.', 'Captive Aspirant - Say Thanks'),
(-1780003, 'Thanks the Light! I\'ll never do anything this foolish again!', 'Kul The Reckless - Say Thanks');
