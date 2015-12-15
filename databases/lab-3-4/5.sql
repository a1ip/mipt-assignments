/*
Определить среднее место, занятое каждым шахматистом на всех турнирах.
*/

WITH id_avg_place AS (
  SELECT chessplayer.chessplayer_id AS id, AVG(participation.place) AS avg_place
    FROM chessplayer, participation
    WHERE participation.chessplayer_id = chessplayer.chessplayer_id
    GROUP BY chessplayer.chessplayer_id
)
SELECT chessplayer.lastname, chessplayer.firstname, id_avg_place.avg_place
  FROM chessplayer, id_avg_place
  WHERE chessplayer.chessplayer_id = id_avg_place.id;
