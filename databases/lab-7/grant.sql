DROP ROLE chess_select_role;
DROP ROLE chess_modify_role;
DROP ROLE chess_view_select_role;

CREATE ROLE chess_select_role IDENTIFIED BY password;
GRANT SELECT ON chessplayer TO chess_select_role;
GRANT SELECT ON tournament TO chess_select_role;
GRANT SELECT ON participation TO chess_select_role;
GRANT SELECT ON title TO chess_select_role;
GRANT SELECT ON current_title TO chess_select_role;
GRANT SELECT ON city TO chess_select_role;
GRANT SELECT ON country TO chess_select_role;

CREATE ROLE chess_modify_role IDENTIFIED BY password;
GRANT INSERT, SELECT, DELETE, UPDATE ON chessplayer TO chess_modify_role;
GRANT SELECT, UPDATE (name) ON city TO chess_modify_role;

CREATE ROLE chess_view_select_role IDENTIFIED BY password;
GRANT SELECT ON prizewinner_info TO chess_view_select_role;

GRANT chess_select_role TO side;
GRANT chess_modify_role TO side;
GRANT chess_view_select_role TO side;

