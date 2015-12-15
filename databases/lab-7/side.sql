ALTER SESSION SET current_schema = chess;

SET ROLE NONE;
SET ROLE chess_select_role IDENTIFIED BY password;

/*
Из предыдущих лаб.
Выбрать турнир с наибольшим количеством гроссмейстеров.
*/

WITH sm_count AS (
  SELECT participation.tournament_id, COUNT(*) AS cnt
    FROM participation, chessplayer, current_title, title
    WHERE participation.chessplayer_id = chessplayer.chessplayer_id
      AND chessplayer.chessplayer_id = current_title.chessplayer_id
      AND current_title.title_id = title.title_id
      AND title.name = 'Senior Master'
    GROUP BY participation.tournament_id
)
SELECT tournament.name, sm_count.cnt
  FROM tournament, sm_count
  WHERE tournament.tournament_id = sm_count.tournament_id
    AND sm_count.cnt = (
      SELECT MAX(sm_count.cnt)
        FROM sm_count
    );


/*
Запросы на update-delete.
*/

--SET ROLE NONE;
SET ROLE chess_modify_role IDENTIFIED BY password;

SELECT * FROM title;

DELETE FROM title;

SELECT * FROM city;
UPDATE city SET city.name = 'MODIFIED' WHERE city.city_id = 3000;
SELECT * FROM city WHERE city.city_id = 3000;
UPDATE city SET city.name = 'Doha' WHERE city.city_id = 3000;
SELECT * FROM city WHERE city.city_id = 3000;

/*
Работа с представлением.
*/

SET ROLE NONE;
SET ROLE chess_view_select_role IDENTIFIED BY password;

SELECT * FROM prizewinner_info;

