/*
Определить количество шахматистов из каждой страны.
*/

WITH id_count AS (
  SELECT country.country_id AS id, COUNT(chessplayer.chessplayer_id) AS cnt
    FROM country
    LEFT OUTER JOIN chessplayer
      ON chessplayer.country_id = country.country_id
    GROUP BY country.country_id
)
SELECT country.name, id_count.cnt
  FROM country, id_count
  WHERE country.country_id = id_count.id;

