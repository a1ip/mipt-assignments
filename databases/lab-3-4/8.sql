/*
Выбрать все турниры, в которых участвовал хотя бы один гроссмейстер.
*/

WITH ids AS (
  SELECT participation.tournament_id AS id
    FROM participation, chessplayer, current_title, title
    WHERE participation.chessplayer_id = chessplayer.chessplayer_id
      AND chessplayer.chessplayer_id = current_title.chessplayer_id
      AND current_title.title_id = title.title_id
      AND title.name = 'Senior Master'
    GROUP BY participation.tournament_id
      HAVING COUNT(*) >= 1
)
SELECT tournament.name
  FROM tournament, ids
  WHERE tournament.tournament_id = ids.id;

