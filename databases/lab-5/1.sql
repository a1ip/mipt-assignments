/*
Звание, количество шахматистов в этом звании, количество побед на турнирах в текущем году, среднее место на турнирах в текущем году.
*/
CREATE OR REPLACE VIEW title_info(id, name, cnt, wins, avg_place) AS
  SELECT title.title_id, title.name, id_cnt.cnt, NVL2(id_wins.wins, id_wins.wins, 0), NVL2(id_avg_place.avg_place, id_avg_place.avg_place, 0)
  FROM title, (
    SELECT title.title_id AS id, COUNT(current_title.title_id) AS cnt
      FROM title
      LEFT OUTER JOIN current_title
        ON title.title_id = current_title.title_id
      GROUP BY title.title_id
  ) id_cnt
  LEFT OUTER JOIN (
    SELECT title.title_id AS id, COUNT(*) AS wins
      FROM tournament, participation, chessplayer, current_title, title
      WHERE tournament.tournament_id = participation.tournament_id
        AND participation.chessplayer_id = chessplayer.chessplayer_id
        AND chessplayer.chessplayer_id = current_title.chessplayer_id
        AND current_title.title_id = title.title_id
        AND participation.place = 1
        --AND TO_CHAR(SYSDATE, 'YYYY') = TO_CHAR(tournament.end_date, 'YYYY')
      GROUP BY title.title_id
  ) id_wins
    ON id_cnt.id = id_wins.id
  LEFT OUTER JOIN (
    SELECT title.title_id AS id, AVG(participation.place) AS avg_place
      FROM tournament, participation, chessplayer, current_title, title
      WHERE tournament.tournament_id = participation.tournament_id
        AND participation.chessplayer_id = chessplayer.chessplayer_id
        AND chessplayer.chessplayer_id = current_title.chessplayer_id
        AND current_title.title_id = title.title_id
        --AND TO_CHAR(SYSDATE, 'YYYY') = TO_CHAR(tournament.end_date, 'YYYY')
      GROUP BY title.title_id
  ) id_avg_place
    ON id_cnt.id = id_avg_place.id
  WHERE title.title_id = id_cnt.id;

SELECT * FROM title_info ORDER BY title_info.id;

/*
Выбрать звание с максимальным количеством шахматистов в нем.
*/
/*
SELECT title.name
  FROM title, title_info
    WHERE title.title_id = title_info.id
      AND title_info.cnt = (
        SELECT MAX(title_info.cnt)
          FROM title_info
      );
*/

-- *    *    *    *    *    *    *    *    *    *    *    *    *    *    *    *    *    *    *    *    *    *

/*
Звание, количество шахматистов в этом звании.
*/
/*
SELECT title.title_id AS id, COUNT(current_title.title_id) AS cnt
  FROM title
  LEFT OUTER JOIN current_title
    ON title.title_id = current_title.title_id
  GROUP BY title.title_id;
*/

/*
Звание, количество побед на турнирах в текущем году (только те, кто побеждали).
*/
/*
SELECT title.title_id AS title_id, COUNT(*) AS wins
  FROM tournament, participation, chessplayer, current_title, title
  WHERE tournament.tournament_id = participation.tournament_id
    AND participation.chessplayer_id = chessplayer.chessplayer_id
    AND chessplayer.chessplayer_id = current_title.chessplayer_id
    AND current_title.title_id = title.title_id
    AND participation.place = 1
    --AND TO_CHAR(SYSDATE, 'YYYY') = TO_CHAR(tournament.end_date, 'YYYY')
  GROUP BY title.title_id;
*/

/*
Звание, среднее место на турнирах в текущем году (только те, кто участвовали).
*/
/*
SELECT title.title_id AS title_id, AVG(participation.place) AS avg_place
  FROM tournament, participation, chessplayer, current_title, title
  WHERE tournament.tournament_id = participation.tournament_id
    AND participation.chessplayer_id = chessplayer.chessplayer_id
    AND chessplayer.chessplayer_id = current_title.chessplayer_id
    AND current_title.title_id = title.title_id
    --AND TO_CHAR(SYSDATE, 'YYYY') = TO_CHAR(tournament.end_date, 'YYYY')
  GROUP BY title.title_id;
*/

