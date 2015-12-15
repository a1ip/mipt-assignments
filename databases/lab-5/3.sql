/*
Шахматист, рейтинг, статистика по всем проводившимся турнирам в текущем году, изменение рейтинга за 6 лет.
*/
CREATE OR REPLACE VIEW chessplayer_info(id, rating, change) AS
  SELECT current_rating.chessplayer_id, current_rating.value, NVL2(id_change.change, id_change.change, 0)
  FROM current_rating
  LEFT OUTER JOIN (
    SELECT participation.chessplayer_id AS id, SUM(participation.rating_change) AS change
      FROM tournament, participation
      WHERE participation.tournament_id = tournament.tournament_id
        AND SYSDATE - tournament.end_date <= 6 * 365
      GROUP BY participation.chessplayer_id
  ) id_change
    ON current_rating.chessplayer_id = id_change.id;

SELECT * FROM chessplayer_info ORDER BY chessplayer_info.id;

/*
  Выбрать шахматиста с максимальным изменением рейтинга за последние 6 лет.
*/
/*
SELECT chessplayer.firstname, chessplayer.lastname
  FROM chessplayer, chessplayer_info
  WHERE chessplayer.chessplayer_id = chessplayer_info.id
    AND chessplayer_info.change = (
      SELECT MAX(chessplayer_info.change)
        FROM chessplayer_info
    );
*/

-- *    *    *    *    *    *    *    *    *    *    *    *    *    *    *    *    *    *    *    *    *    *

/*
Шахматист, рейтинг
*/
/*
SELECT current_rating.chessplayer_id, current_rating.value
  FROM current_rating;
*/

/*
Шахматист, изменение рейтинга за 6 лет.
*/
/*
SELECT participation.chessplayer_id AS id, SUM(participation.rating_change) AS change
  FROM tournament, participation
  WHERE participation.tournament_id = tournament.tournament_id
    AND SYSDATE - tournament.end_date <= 6 * 365
  GROUP BY participation.chessplayer_id;
*/

/*
SELECT chessplayer.chessplayer_id AS id, SUM(NVL2(id_change.change, id_change.change, 0)) AS change
  FROM chessplayer
  LEFT OUTER JOIN (
    SELECT participation.chessplayer_id AS id, participation.rating_change AS change
      FROM participation, tournament
      WHERE participation.tournament_id = tournament.tournament_id
        AND SYSDATE - tournament.end_date <= 365
  ) id_change
    ON chessplayer.chessplayer_id = id_change.id
  GROUP BY chessplayer.chessplayer_id;
*/
