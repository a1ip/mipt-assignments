/*
Определить турниры, в которых участник с самым высоким рейтингом занял последнее место.
*/

WITH
  tournament_last_place AS (
    SELECT participation.tournament_id AS t_id, MAX(participation.place) AS last_place
      FROM participation
      GROUP BY participation.tournament_id
  ),
  tournament_highest_rating AS (
    SELECT participation.tournament_id AS t_id, MAX(current_rating.value) AS rating
      FROM participation, chessplayer, current_rating
      WHERE participation.chessplayer_id = chessplayer.chessplayer_id
        AND chessplayer.chessplayer_id = current_rating.chessplayer_id
      GROUP BY participation.tournament_id
  )
SELECT tournament.name
  FROM tournament, participation, chessplayer, current_rating, tournament_last_place, tournament_highest_rating
  WHERE tournament.tournament_id = participation.tournament_id
    AND participation.chessplayer_id = chessplayer.chessplayer_id
    AND chessplayer.chessplayer_id = current_rating.chessplayer_id
    AND tournament_last_place.t_id = tournament.tournament_id
    AND tournament_last_place.last_place = participation.place
    AND tournament_highest_rating.t_id = tournament.tournament_id
    AND tournament_highest_rating.rating = current_rating.value;

