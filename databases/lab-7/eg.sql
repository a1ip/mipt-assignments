DROP ROLE chess_modify_role;
DROP ROLE chess_modify_role_1;

/**/

CREATE ROLE chess_modify_role;
GRANT INSERT, SELECT, DELETE, UPDATE ON chessplayer TO chess_modify_role;
GRANT SELECT, UPDATE (name) ON city TO chess_modify_role;

GRANT chess_modify_role TO side;

/**/

CREATE ROLE chess_modify_role_1;
GRANT SELECT, UPDATE (name) ON city TO chess_modify_role_1;

GRANT chess_modify_role_1 TO side;

ALTER USER side DEFAULT ROLE ALL EXCEPT chess_modify_role;
