SET linesize 128;
SET pagesize 10000;

SET AUTOTRACE ON;

-- = 'URL'

SELECT customers.customerid FROM customers WHERE customerid LIKE '%U%' AND country = 'France';

CREATE INDEX ccid_idx ON customers(customerid);
CREATE INDEX cc_idx ON customers(country);

SELECT * FROM customers WHERE customerid = 'URL';

DROP INDEX ccid_idx;
DROP INDEX cc_idx;

SET AUTOTRACE OFF;

/*
Execution Plan
----------------------------------------------------------
Plan hash value: 2008213504

-------------------------------------------------------------------------------
| Id  | Operation	        | Name      | Rows  | Bytes | Cost (%CPU)| Time     |
-------------------------------------------------------------------------------
|   0 | SELECT STATEMENT  |	          |    20 |  3080 |    11	  (0)| 00:00:01 |
|*  1 |  TABLE ACCESS FULL| CUSTOMERS |    20 |  3080 |    11  	(0)| 00:00:01 |
-------------------------------------------------------------------------------

Predicate Information (identified by operation id):
---------------------------------------------------

   1 - filter("COUNTRY"='France' AND "CUSTOMERID" LIKE '%U%')

Note
-----
   - dynamic sampling used for this statement


Statistics
----------------------------------------------------------
	198  recursive calls
	  0  db block gets
	109  consistent gets
	  0  physical reads
	  0  redo size
       1375  bytes sent via SQL*Net to client
	395  bytes received via SQL*Net from client
	  3  SQL*Net roundtrips to/from client
	  4  sorts (memory)
	  0  sorts (disk)
	 20  rows processed


/*
Execution Plan
----------------------------------------------------------
Plan hash value: 2008213504

-------------------------------------------------------------------------------
| Id  | Operation	        | Name      | Rows  | Bytes | Cost (%CPU)| Time     |
-------------------------------------------------------------------------------
|   0 | SELECT STATEMENT  |	          |    20 |  3080 |    11  	(0)| 00:00:01 |
|*  1 |  TABLE ACCESS FULL| CUSTOMERS |    20 |  3080 |    11	  (0)| 00:00:01 |
-------------------------------------------------------------------------------

Predicate Information (identified by operation id):
---------------------------------------------------

   1 - filter("COUNTRY"='France' AND "CUSTOMERID" LIKE '%U%')

Note
-----
   - dynamic sampling used for this statement


Statistics
----------------------------------------------------------
	  5  recursive calls
	  0  db block gets
	 80  consistent gets
	  0  physical reads
	  0  redo size
       1375  bytes sent via SQL*Net to client
	395  bytes received via SQL*Net from client
	  3  SQL*Net roundtrips to/from client
	  0  sorts (memory)
	  0  sorts (disk)
	 20  rows processed
*/

