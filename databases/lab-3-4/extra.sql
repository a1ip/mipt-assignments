/*
Сделать полный пересчет таблицы current_rating.
*/

DELETE FROM current_rating;

SELECT * FROM current_rating;

INSERT INTO current_rating
  SELECT chessplayer.chessplayer_id, chessplayer.initial_rating + id_change.change
    FROM chessplayer, (
      SELECT chessplayer.chessplayer_id AS id, SUM(NVL2(participation.rating_change, participation.rating_change, 0)) AS change
        FROM chessplayer
        LEFT OUTER JOIN participation
          ON chessplayer.chessplayer_id = participation.chessplayer_id
        GROUP BY chessplayer.chessplayer_id
    ) id_change
    WHERE chessplayer.chessplayer_id = id_change.id;

SELECT * FROM current_rating;

/*
UPDATE current_rating
  SET current_rating.value = (
    SELECT chessplayer.initial_rating + id_change.change
      FROM chessplayer, (
        SELECT chessplayer.chessplayer_id AS id, SUM(NVL2(participation.rating_change, participation.rating_change, 0)) AS change
          FROM chessplayer
          LEFT OUTER JOIN participation
            ON chessplayer.chessplayer_id = participation.chessplayer_id
          GROUP BY chessplayer.chessplayer_id
      ) id_change
      WHERE chessplayer.chessplayer_id = id_change.id
        AND current_rating.chessplayer_id = chessplayer.chessplayer_id
  );
*/
