DROP TABLE customers;
DROP TABLE orders;
DROP TABLE products;
DROP TABLE order_details;

CREATE TABLE customers (
	CustomerID varchar2 (5) NOT NULL ,
	CompanyName varchar2 (40) NOT NULL ,
	ContactName varchar2 (30) NULL ,
	ContactTitle varchar2 (30) NULL ,
	Address varchar2 (60) NULL ,
	City varchar2 (15) NULL ,
	Region varchar2 (15) NULL ,
	PostalCode varchar2 (10) NULL ,
	Country varchar2 (15) NULL ,
	Phone varchar2 (24) NULL ,
	Fax varchar2 (24) NULL);

CREATE TABLE orders (
	OrderID number NOT NULL ,
	CustomerID varchar2 (5) NULL ,
	EmployeeID number NULL ,
	OrderDate varchar2(15) NULL ,
	RequiredDate varchar2(15) NULL ,
	ShippedDate varchar2(15) NULL ,
	ShipVia number NULL ,
	Freight number NULL,
	ShipName varchar2 (40) NULL ,
	ShipAddress varchar2 (60) NULL ,
	ShipCity varchar2 (15) NULL ,
	ShipRegion varchar2 (15) NULL ,
	ShipPostalCode varchar2 (10) NULL ,
	ShipCountry varchar2 (15) NULL);

CREATE TABLE products (
	ProductID number  NOT NULL ,
	ProductName varchar2 (40) NOT NULL ,
	SupplierID number NULL ,
	CategoryID number NULL ,
	QuantityPerUnit varchar2 (20) NULL ,
	UnitPrice number NULL ,
	UnitsInStock number NULL ,
	UnitsOnOrder number NULL ,
	ReorderLevel number NULL ,
	Discontinued number NOT NULL);

CREATE TABLE order_details (
	OrderID number NOT NULL ,
	ProductID number NOT NULL ,
	UnitPrice number NOT NULL ,
	Quantity number NOT NULL ,
	Discount number NOT NULL );

INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;
INSERT INTO customers SELECT * FROM northwind.customers;


INSERT INTO orders SELECT * FROM northwind.orders;
INSERT INTO order_details SELECT * FROM northwind.order_details;

INSERT INTO products SELECT * FROM northwind.products;
INSERT INTO products SELECT * FROM northwind.products;
INSERT INTO products SELECT * FROM northwind.products;
INSERT INTO products SELECT * FROM northwind.products;
INSERT INTO products SELECT * FROM northwind.products;
INSERT INTO products SELECT * FROM northwind.products;
INSERT INTO products SELECT * FROM northwind.products;
INSERT INTO products SELECT * FROM northwind.products;
INSERT INTO products SELECT * FROM northwind.products;
INSERT INTO products SELECT * FROM northwind.products;
INSERT INTO products SELECT * FROM northwind.products;


