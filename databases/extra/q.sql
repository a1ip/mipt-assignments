SET linesize 128;
SET pagesize 10000;

DESC price;

SET AUTOTRACE ON;

CREATE INDEX p_idx ON price(end_date, product_id);

SELECT * FROM price WHERE end_date IS NULL;

DROP INDEX p_idx;

SET AUTOTRACE OFF;

