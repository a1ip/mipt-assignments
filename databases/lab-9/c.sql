SELECT current_rating.chessplayer_id, current_rating.value FROM current_rating, chessplayer
  WHERE current_rating.chessplayer_id = chessplayer.chessplayer_id
    AND chessplayer.country_id = 1003;
SELECT current_title.chessplayer_id, current_title.title_id FROM current_title, chessplayer
  WHERE current_title.chessplayer_id = chessplayer.chessplayer_id
    AND chessplayer.country_id = 1003;

UPDATE chessplayer SET initial_rating = 5000 WHERE country_id = 1003;

SELECT current_rating.chessplayer_id, current_rating.value FROM current_rating, chessplayer
  WHERE current_rating.chessplayer_id = chessplayer.chessplayer_id
    AND chessplayer.country_id = 1003;
SELECT current_title.chessplayer_id, current_title.title_id FROM current_title, chessplayer
  WHERE current_title.chessplayer_id = chessplayer.chessplayer_id
    AND chessplayer.country_id = 1003;

