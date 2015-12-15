/*
Выбрать всех шахматистов, побеждавших в каком-либо турнире.
*/

SELECT DISTINCT chessplayer.firstname, chessplayer.lastname
  FROM chessplayer, participation
  WHERE participation.chessplayer_id = chessplayer.chessplayer_id
    AND participation.place = 1;
