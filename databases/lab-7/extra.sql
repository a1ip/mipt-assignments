ALTER SESSION SET current_schema = chess;

SET ROLE chess_select_role;

UPDATE chessplayer SET chessplayer.firstname = 'MODIFIED' WHERE chessplayer.chessplayer_id = 2010;
SELECT * FROM city;
