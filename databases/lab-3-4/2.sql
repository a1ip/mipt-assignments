/*
Выбрать те турниры, где ни одно призовое место не занял представитель страны-хозяина турнира.

Призовых мест занятых не представителями страны-хозяина турнира = 3.
*/

WITH good_tournament_ids AS (
  SELECT tournament.tournament_id
    FROM tournament, city, country, participation, chessplayer
    WHERE tournament.city_id = city.city_id
      AND city.country_id = country.country_id
      AND chessplayer.country_id <> country.country_id
      AND participation.chessplayer_id = chessplayer.chessplayer_id
      AND participation.tournament_id = tournament.tournament_id
      AND participation.place <= 3
    GROUP BY tournament.tournament_id
      HAVING COUNT(*) = 3
)
SELECT tournament.name
  FROM tournament, good_tournament_ids
    WHERE tournament.tournament_id = good_tournament_ids.tournament_id;

