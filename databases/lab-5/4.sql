/*
Город, год, шахматист, звание, место - только призеры.
*/

CREATE OR REPLACE VIEW prizewinner_info(city_id, year, cp_id, title_id, place) AS
  SELECT city.city_id, TO_CHAR(tournament.end_date, 'YYYY'), chessplayer.chessplayer_id, current_title.title_id, participation.place
    FROM current_title, chessplayer, participation, tournament, city
    WHERE current_title.chessplayer_id = chessplayer.chessplayer_id
      AND chessplayer.chessplayer_id = participation.chessplayer_id
      AND participation.tournament_id = tournament.tournament_id
      AND tournament.city_id = city.city_id
      AND participation.place <= 3;

SELECT * FROM prizewinner_info;

/*
Призеры 2005 года.
*/
/*
SELECT chessplayer.firstname, chessplayer.lastname
  FROM chessplayer, prizewinner_info
  WHERE chessplayer.chessplayer_id = prizewinner_info.cp_id
    AND prizewinner_info.year = 2005;
*/

