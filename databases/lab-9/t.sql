SET serveroutput ON;

CREATE OR REPLACE PROCEDURE check_title(cp_id IN NUMBER, do_insert IN NUMBER) IS
    cp_rating NUMBER;
    cp_title NUMBER;
  BEGIN
    SELECT current_rating.value INTO cp_rating FROM current_rating
      WHERE current_rating.chessplayer_id = cp_id;

    IF cp_rating >= 2700 THEN cp_title := 01;
    ELSIF cp_rating >= 2650 THEN cp_title := 02;
    ELSIF cp_rating >= 2600 THEN cp_title := 03;
    ELSIF cp_rating >= 2500 THEN cp_title := 04;
    ELSIF cp_rating >= 2400 THEN cp_title := 05;
    ELSIF cp_rating >= 2200 THEN cp_title := 06;
    ELSIF cp_rating >= 2000 THEN cp_title := 07;
    ELSIF cp_rating >= 1800 THEN cp_title := 08;
    ELSIF cp_rating >= 1600 THEN cp_title := 09;
    ELSIF cp_rating >= 1400 THEN cp_title := 10;
    ELSIF cp_rating >= 1200 THEN cp_title := 11;
    ELSIF cp_rating >= 1000 THEN cp_title := 12;
    ELSE cp_title := 13;
    END IF;

    --DBMS_OUTPUT.PUT_LINE('!' || cp_id || ' ' || cp_rating || ' ' || cp_title || '!');

    IF do_insert <> 0 THEN
      INSERT INTO current_title VALUES(cp_id, cp_title);
    ELSE
      UPDATE current_title SET current_title.title_id = cp_title
        WHERE current_title.chessplayer_id = cp_id;
    END IF;
  END;
/

--SHOW ERRORS PROCEDURE check_title;

/*
Триггеры на chessplayer.
*/

CREATE OR REPLACE TRIGGER on_chessplayer_insert
  AFTER INSERT ON chessplayer
  FOR EACH ROW
  BEGIN
    INSERT INTO current_rating VALUES(:NEW.chessplayer_id, :NEW.initial_rating);
    check_title(:NEW.chessplayer_id, 1);
  END;
/

--SHOW ERRORS TRIGGER on_chessplayer_insert;

CREATE OR REPLACE TRIGGER on_chessplayer_update
  AFTER UPDATE ON chessplayer
  FOR EACH ROW
  BEGIN
    IF :OLD.chessplayer_id = :NEW.chessplayer_id THEN
      UPDATE current_rating
        SET current_rating.value = (current_rating.value - :OLD.initial_rating + :NEW.initial_rating)
        WHERE current_rating.chessplayer_id = :NEW.chessplayer_id;
      check_title(:NEW.chessplayer_id, 0);
    ELSE
      DELETE FROM current_rating WHERE current_rating.chessplayer_id = :OLD.chessplayer_id;
      INSERT INTO current_rating VALUES(:NEW.chessplayer_id, :NEW.initial_rating);
      DELETE FROM current_title WHERE current_title.chessplayer_id = :OLD.chessplayer_id;
      check_title(:NEW.chessplayer_id, 1);
    END IF;
  END;
/

--SHOW ERRORS TRIGGER on_chessplayer_update;

/*
Триггеры на participation.
*/

CREATE OR REPLACE TRIGGER on_participation_insert
  AFTER INSERT ON participation
  FOR EACH ROW
  BEGIN
    UPDATE current_rating
      SET current_rating.value = current_rating.value + :NEW.rating_change
      WHERE current_rating.chessplayer_id = :NEW.chessplayer_id;
    check_title(:NEW.chessplayer_id, 0);
  END;
/

--SHOW ERRORS TRIGGER on_participation_insert;

CREATE OR REPLACE TRIGGER on_participation_update
  AFTER UPDATE ON participation
  FOR EACH ROW
  BEGIN
    UPDATE current_rating
      SET current_rating.value = current_rating.value - :OLD.rating_change
      WHERE current_rating.chessplayer_id = :OLD.chessplayer_id;
    UPDATE current_rating
      SET current_rating.value = current_rating.value + :NEW.rating_change
      WHERE current_rating.chessplayer_id = :NEW.chessplayer_id;
    check_title(:NEW.chessplayer_id, 0);
  END;
/

--SHOW ERRORS TRIGGER on_participation_update;

CREATE OR REPLACE TRIGGER on_participation_delete
  AFTER DELETE ON participation
  FOR EACH ROW
  BEGIN
    UPDATE current_rating
      SET current_rating.value = current_rating.value - :OLD.rating_change
      WHERE current_rating.chessplayer_id = :OLD.chessplayer_id;
    check_title(:OLD.chessplayer_id, 0);
  END;
/

--SHOW ERRORS TRIGGER on_participation_delete;

