COLUMN name FORMAT A100;

SELECT * FROM (
  SELECT name FROM v$controlfile
  UNION
  SELECT name FROM v$datafile
  UNION
  SELECT name FROM v$archived_log
  UNION
  SELECT member FROM gv$logfile
);

