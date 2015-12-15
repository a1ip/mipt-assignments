/*
Страна, количество шахматистов из этой страны, принимавших участие в турнирах, количество побед шахматистов из этой страны, количество побед на родине.
*/
CREATE OR REPLACE VIEW country_info(id, name, cnt, wins, home_wins) AS
  SELECT country.country_id, country.name, id_cnt.cnt, NVL2(id_wins.wins, id_wins.wins, 0), NVL2(id_home_wins.home_wins, id_home_wins.home_wins, 0)
  FROM country, (
    SELECT country.country_id AS id, COUNT(DISTINCT participation.chessplayer_id) AS cnt
      FROM country
      LEFT OUTER JOIN chessplayer
        ON country.country_id = chessplayer.country_id
      LEFT OUTER JOIN participation
        ON chessplayer.chessplayer_id = participation.chessplayer_id
      GROUP BY country.country_id
  ) id_cnt
  LEFT OUTER JOIN (
    SELECT country.country_id AS id, COUNT(*) AS wins
      FROM country, chessplayer, participation
      WHERE country.country_id = chessplayer.country_id
        AND chessplayer.chessplayer_id = participation.chessplayer_id
        AND participation.place = 1
      GROUP BY country.country_id
  ) id_wins
    ON id_cnt.id = id_wins.id
  LEFT OUTER JOIN (
    SELECT country.country_id AS id, COUNT(*) AS home_wins
      FROM country, chessplayer, participation, tournament, city
      WHERE country.country_id = chessplayer.country_id
        AND chessplayer.chessplayer_id = participation.chessplayer_id
        AND participation.tournament_id = tournament.tournament_id
        AND tournament.city_id = city.city_id
        AND city.country_id = country.country_id
        AND participation.place = 1
      GROUP BY country.country_id
  ) id_home_wins
    ON id_cnt.id = id_home_wins.id
  WHERE country.country_id = id_cnt.id;

SELECT * FROM country_info ORDER BY country_info.id;

/*
Выбрать страну с максимальным количеством шахматистов в ней.
*/
/*
SELECT country.name
  FROM country, country_info
    WHERE country.country_id = country_info.id
      AND country_info.cnt = (
        SELECT MAX(country_info.cnt)
          FROM country_info
      );
*/

-- *    *    *    *    *    *    *    *    *    *    *    *    *    *    *    *    *    *    *    *    *    *

/*
Страна, количество шахматистов из этой страны, принимавших участие в турнирах.
*/
/*
SELECT country.country_id AS id, COUNT(DISTINCT participation.chessplayer_id) AS cnt
  FROM country
  LEFT OUTER JOIN chessplayer
    ON country.country_id = chessplayer.country_id
  LEFT OUTER JOIN participation
    ON chessplayer.chessplayer_id = participation.chessplayer_id
  GROUP BY country.country_id;
*/

/*
Страна, количество шахматистов из этой страны, количество побед шахматистов из этой страны (только тех, что побеждали).
*/
/*
SELECT country.country_id AS id, COUNT(*) AS wins
  FROM country, chessplayer, participation
  WHERE country.country_id = chessplayer.country_id
    AND chessplayer.chessplayer_id = participation.chessplayer_id
    AND participation.place = 1
  GROUP BY country.country_id;
*/

/*
Страна, количество шахматистов из этой страны, количество побед шахматистов на родине (только тех, что побеждали).
*/
/*
SELECT country.country_id AS id, COUNT(*) AS home_wins
  FROM country, chessplayer, participation, tournament, city
  WHERE country.country_id = chessplayer.country_id
    AND chessplayer.chessplayer_id = participation.chessplayer_id
    AND participation.tournament_id = tournament.tournament_id
    AND tournament.city_id = city.city_id
    AND city.country_id = country.country_id
    AND participation.place = 1
  GROUP BY country.country_id;
*/

