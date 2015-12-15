SET linesize 128;
SET pagesize 10000;

SET AUTOTRACE ON;

SELECT customers.country, orders.orderdate FROM customers, orders
  WHERE customers.customerid = orders.customerid
    AND customers.customerid LIKE '%U%'
    AND orders.freight > 1000;

CREATE INDEX c_cid_idx ON customers(customerid);
CREATE INDEX o_cid_idx ON orders(customerid);
CREATE INDEX o_f_idx ON orders(freight);

SELECT customers.country, orders.orderdate FROM customers, orders
  WHERE customers.customerid = orders.customerid
    AND customers.customerid LIKE '%U%'
    AND orders.freight > 1000;

DROP INDEX c_cid_idx;
DROP INDEX o_cid_idx;
DROP INDEX o_f_idx;

SET AUTOTRACE OFF;

/*
Execution Plan
----------------------------------------------------------
Plan hash value: 23084738

--------------------------------------------------------------------------------
| Id  | Operation	         | Name      | Rows  | Bytes | Cost (%CPU)| Time     |
--------------------------------------------------------------------------------
|   0 | SELECT STATEMENT   |	         |     7 |   273 |    21	 (5)| 00:00:01 |
|*  1 |  HASH JOIN	       |	         |     7 |   273 |    21	 (5)| 00:00:01 |
|*  2 |   TABLE ACCESS FULL| ORDERS    |     2 |    52 |     9	 (0)| 00:00:01 |
|*  3 |   TABLE ACCESS FULL| CUSTOMERS |   300 |  3900 |    11	 (0)| 00:00:01 |
--------------------------------------------------------------------------------

Predicate Information (identified by operation id):
---------------------------------------------------

   1 - access("CUSTOMERS"."CUSTOMERID"="ORDERS"."CUSTOMERID")
   2 - filter("ORDERS"."FREIGHT">1000 AND "ORDERS"."CUSTOMERID" LIKE
	      '%U%')
   3 - filter("CUSTOMERS"."CUSTOMERID" LIKE '%U%')

Note
-----
   - dynamic sampling used for this statement


Statistics
----------------------------------------------------------
	365  recursive calls
	  0  db block gets
	197  consistent gets
	  0  physical reads
	  0  redo size
	964  bytes sent via SQL*Net to client
	406  bytes received via SQL*Net from client
	  4  SQL*Net roundtrips to/from client
	 10  sorts (memory)
	  0  sorts (disk)
	 40  rows processed
*/
/*
Execution Plan
----------------------------------------------------------
Plan hash value: 34009282

-------------------------------------------------------------------------------------------
| Id  | Operation		                  | Name	    | Rows  | Bytes | Cost (%CPU)| Time	    |
-------------------------------------------------------------------------------------------
|   0 | SELECT STATEMENT	            | 	        |	    7 |   273 |	    8   (0)| 00:00:01 |
|   1 |  TABLE ACCESS BY INDEX ROWID  | CUSTOMERS |	    3 |    39 |	    2   (0)| 00:00:01 |
|   2 |   NESTED LOOPS		            | 	        |   	7 |   273 |		  8   (0)| 00:00:01 |
|*  3 |    TABLE ACCESS BY INDEX ROWID| ORDERS	  |	    2 |    52 |		  4   (0)| 00:00:01 |
|*  4 |     INDEX RANGE SCAN	        | O_F_IDX   |	    2 |	      |		  2   (0)| 00:00:01 |
|*  5 |    INDEX RANGE SCAN	          | C_CID_IDX |    	1 |	      |	  	1   (0)| 00:00:01 |
-------------------------------------------------------------------------------------------

Predicate Information (identified by operation id):
---------------------------------------------------

   3 - filter("ORDERS"."CUSTOMERID" LIKE '%U%')
   4 - access("ORDERS"."FREIGHT">1000)
   5 - access("CUSTOMERS"."CUSTOMERID"="ORDERS"."CUSTOMERID")
       filter("CUSTOMERS"."CUSTOMERID" LIKE '%U%')

Note
-----
   - dynamic sampling used for this statement


Statistics
----------------------------------------------------------
	  9  recursive calls
	  0  db block gets
	126  consistent gets
	  8  physical reads
	  0  redo size
	964  bytes sent via SQL*Net to client
	406  bytes received via SQL*Net from client
	  4  SQL*Net roundtrips to/from client
	  2  sorts (memory)
	  0  sorts (disk)
	 40  rows processed
*/

