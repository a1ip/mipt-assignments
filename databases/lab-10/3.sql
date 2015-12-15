T linesize 128;
SET pagesize 10000;

SET AUTOTRACE ON;

SELECT customers.country, orders.freight, products.unitprice, order_details.quantity
  FROM customers, orders, products, order_details
  WHERE customers.customerid = orders.customerid
    AND orders.orderid = order_details.orderid
    AND order_details.productid = products.productid
    AND customers.customerid LIKE '%R%'
    AND orders.freight > 710
    AND products.unitprice > 260
    AND order_details.quantity < 10;

CREATE INDEX c_cid_idx ON customers(customerid);
CREATE INDEX o_cid_idx ON orders(customerid);
CREATE INDEX o_oid_idx ON orders(orderid);
CREATE INDEX od_oid_idx ON order_details(orderid);
CREATE INDEX od_pid_idx ON order_details(productid);
CREATE INDEX p_pid_idx ON products(productid);
CREATE INDEX o_f_idx ON orders(freight);
CREATE INDEX p_up_idx ON products(unitprice);
CREATE INDEX od_q_idx ON order_details(quantity);

SELECT customers.country, orders.freight, products.unitprice, order_details.quantity
  FROM customers, orders, products, order_details
  WHERE customers.customerid = orders.customerid
    AND orders.orderid = order_details.orderid
    AND order_details.productid = products.productid
    AND customers.customerid LIKE '%R%'
    AND orders.freight > 710
    AND products.unitprice > 260
    AND order_details.quantity < 10;

DROP INDEX c_cid_idx;
DROP INDEX o_cid_idx;
DROP INDEX o_oid_idx;
DROP INDEX od_oid_idx;
DROP INDEX od_pid_idx;
DROP INDEX p_pid_idx;
DROP INDEX o_f_idx;
DROP INDEX p_up_idx;
DROP INDEX od_q_idx;

SET AUTOTRACE OFF;

/*
Execution Plan
----------------------------------------------------------
Plan hash value: 577758160

--------------------------------------------------------------------------------------
| Id  | Operation	           | Name	         | Rows  | Bytes | Cost (%CPU)| Time     |
--------------------------------------------------------------------------------------
|   0 | SELECT STATEMENT     |		           |	  19 |	2052 |	  36   (6)| 00:00:01 |
|*  1 |  HASH JOIN	         |		           |	  19 |	2052 |	  36   (6)| 00:00:01 |
|*  2 |   HASH JOIN	         |		           |	   3 |	 285 |	  24   (5)| 00:00:01 |
|*  3 |    HASH JOIN	       |		           |	   9 |	 621 |	  17   (6)| 00:00:01 |
|*  4 |     TABLE ACCESS FULL| ORDERS	       |	   6 |	 180 |	   9   (0)| 00:00:01 |
|*  5 |     TABLE ACCESS FULL| ORDER_DETAILS |	 854 | 33306 |	   7   (0)| 00:00:01 |
|*  6 |    TABLE ACCESS FULL | PRODUCTS      |	  22 |	 572 |	   7   (0)| 00:00:01 |
|*  7 |   TABLE ACCESS FULL  | CUSTOMERS     |	 660 |	8580 |	  11   (0)| 00:00:01 |
--------------------------------------------------------------------------------------

Predicate Information (identified by operation id):
---------------------------------------------------

   1 - access("CUSTOMERS"."CUSTOMERID"="ORDERS"."CUSTOMERID")
   2 - access("ORDER_DETAILS"."PRODUCTID"="PRODUCTS"."PRODUCTID")
   3 - access("ORDERS"."ORDERID"="ORDER_DETAILS"."ORDERID")
   4 - filter("ORDERS"."FREIGHT">710 AND "ORDERS"."CUSTOMERID" LIKE '%R%')
   5 - filter("ORDER_DETAILS"."QUANTITY"<10)
   6 - filter("PRODUCTS"."UNITPRICE">260)
   7 - filter("CUSTOMERS"."CUSTOMERID" LIKE '%R%')

Note
-----
   - dynamic sampling used for this statement


Statistics
----------------------------------------------------------
	740  recursive calls
	  0  db block gets
	311  consistent gets
	  0  physical reads
	  0  redo size
	450  bytes sent via SQL*Net to client
	373  bytes received via SQL*Net from client
	  1  SQL*Net roundtrips to/from client
	 20  sorts (memory)
	  0  sorts (disk)
	  0  rows processed
*/
/*
Execution Plan
----------------------------------------------------------
Plan hash value: 1689833922

---------------------------------------------------------------------------------------------
| Id  | Operation		                | Name	        | Rows  | Bytes | Cost (%CPU)| Time     |
---------------------------------------------------------------------------------------------
|   0 | SELECT STATEMENT	          |		            |	   19 |  2052 |	   30   (4)| 00:00:01 |
|   1 |  TABLE ACCESS BY INDEX ROWID| CUSTOMERS     |	    7 |	   91 |	    2   (0)| 00:00:01 |
|   2 |   NESTED LOOPS		          |		            |	   19 |  2052 |	   30   (4)| 00:00:01 |
|*  3 |    HASH JOIN		            |		            |	    3 |	  285 |	   24   (5)| 00:00:01 |
|*  4 |     HASH JOIN		            |		            |	    9 |	  621 |	   17   (6)| 00:00:01 |
|*  5 |      TABLE ACCESS FULL	    | ORDERS	      |	    6 |	  180 |	    9   (0)| 00:00:01 |
|*  6 |      TABLE ACCESS FULL	    | ORDER_DETAILS |  	854 | 33306 |	    7   (0)| 00:00:01 |
|*  7 |     TABLE ACCESS FULL	      | PRODUCTS	    |	   22 |	  572 |	    7   (0)| 00:00:01 |
|*  8 |    INDEX RANGE SCAN	        | C_CID_IDX     |	    1 |	      |	    1   (0)| 00:00:01 |
---------------------------------------------------------------------------------------------

Predicate Information (identified by operation id):
---------------------------------------------------

   3 - access("ORDER_DETAILS"."PRODUCTID"="PRODUCTS"."PRODUCTID")
   4 - access("ORDERS"."ORDERID"="ORDER_DETAILS"."ORDERID")
   5 - filter("ORDERS"."FREIGHT">710 AND "ORDERS"."CUSTOMERID" LIKE '%R%')
   6 - filter("ORDER_DETAILS"."QUANTITY"<10)
   7 - filter("PRODUCTS"."UNITPRICE">260)
   8 - access("CUSTOMERS"."CUSTOMERID"="ORDERS"."CUSTOMERID")
       filter("CUSTOMERS"."CUSTOMERID" LIKE '%R%')

Note
-----
   - dynamic sampling used for this statement


Statistics
----------------------------------------------------------
	 25  recursive calls
	  0  db block gets
	185  consistent gets
	  8  physical reads
	  0  redo size
	450  bytes sent via SQL*Net to client
	373  bytes received via SQL*Net from client
	  1  SQL*Net roundtrips to/from client
	  4  sorts (memory)
	  0  sorts (disk)
	  0  rows processed
*/

