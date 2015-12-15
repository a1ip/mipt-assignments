/*
Выбрать тех шахматистов, которые выйграли не менее двух турниров в течение 2005 г.
*/

WITH winners_ids AS (
  SELECT chessplayer.chessplayer_id AS id
    FROM chessplayer, participation, tournament
    WHERE participation.chessplayer_id = chessplayer.chessplayer_id
      AND participation.tournament_id = tournament.tournament_id
      AND participation.place = 1
      AND TO_CHAR(tournament.start_date, 'YYYY') = '2005'
      AND TO_CHAR(tournament.end_date, 'YYYY') = '2005'
    GROUP BY chessplayer.chessplayer_id
      HAVING COUNT(*) >= 2
)
SELECT chessplayer.lastname, chessplayer.firstname
  FROM chessplayer, winners_ids
  WHERE chessplayer.chessplayer_id = winners_ids.id;

