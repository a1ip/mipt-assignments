INSERT INTO chessplayer VALUES(2000, 'Tomashevsky', 'Evgeny', 100, NULL);
INSERT INTO chessplayer VALUES(2010, 'Tomashevsky', NULL, NULL, NULL);
INSERT INTO tournament VALUES(4010, 'New World Cup', '07/10/2006', '16/10/2005', 3008);

UPDATE current_rating SET current_rating.value = -100 WHERE current_rating.chessplayer_id = 2002;
UPDATE current_rating SET current_rating.chessplayer_id = 2010 WHERE current_rating.chessplayer_id = 2001;

SELECT * FROM current_rating WHERE current_rating.chessplayer_id = 2002;
DELETE FROM chessplayer WHERE chessplayer.chessplayer_id = 2002;
SELECT * FROM current_rating WHERE current_rating.chessplayer_id = 2002;

