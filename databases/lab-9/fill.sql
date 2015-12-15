ALTER SESSION SET NLS_DATE_FORMAT = 'DD/MM/YYYY';

DELETE FROM participation;
DELETE FROM chessplayer;
DELETE FROM tournament;
DELETE FROM city;
DELETE FROM country;
DELETE FROM title;
--DELETE FROM current_rating;
--DELETE FROM current_title;

INSERT INTO title VALUES(01, 'Senior Master');
INSERT INTO title VALUES(02, 'National Master');
INSERT INTO title VALUES(03, 'Expert');
INSERT INTO title VALUES(04, 'Class A');
INSERT INTO title VALUES(05, 'Class B');
INSERT INTO title VALUES(06, 'Class C');
INSERT INTO title VALUES(07, 'Class D');
INSERT INTO title VALUES(08, 'Class E');
INSERT INTO title VALUES(09, 'Class F');
INSERT INTO title VALUES(10, 'Class G');
INSERT INTO title VALUES(11, 'Class H');
INSERT INTO title VALUES(12, 'Class I');     
INSERT INTO title VALUES(13, 'Class J');

INSERT INTO country VALUES(1000, 'The United States');
INSERT INTO country VALUES(1001, 'Norway');
INSERT INTO country VALUES(1002, 'Armenia');
INSERT INTO country VALUES(1003, 'Russia');
INSERT INTO country VALUES(1004, 'India');
INSERT INTO country VALUES(1005, 'Azerbaijan');
INSERT INTO country VALUES(1006, 'Bulgaria');
INSERT INTO country VALUES(1007, 'Ukraine');
INSERT INTO country VALUES(1008, 'Qatar');
INSERT INTO country VALUES(1009, 'Poland');
INSERT INTO country VALUES(1010, 'Albania');
INSERT INTO country VALUES(1011, 'France');
INSERT INTO country VALUES(1012, 'Slovenia');
INSERT INTO country VALUES(1013, 'Lithuania');

INSERT INTO city VALUES(3000, 'Doha', 1008);
INSERT INTO city VALUES(3001, 'Krakow', 1009);
INSERT INTO city VALUES(3002, 'Kazan', 1003);
INSERT INTO city VALUES(3003, 'Khanty Mansiysk', 1003);
INSERT INTO city VALUES(3004, 'Tirana', 1010);
INSERT INTO city VALUES(3005, 'Aix-les-Bains', 1011);
INSERT INTO city VALUES(3006, 'Rogaska Slatina', 1012);
INSERT INTO city VALUES(3007, 'Druskininkai', 1013);
INSERT INTO city VALUES(3008, 'Merlimont', 1011);
INSERT INTO city VALUES(3009, 'Warsaw', 1009);

INSERT INTO tournament VALUES(4000, 'World Individual Championship', '30/04/2005', '08/05/2005', 3002);
INSERT INTO tournament VALUES(4001, 'World Cup', '26/08/2005', '21/09/2005', 3003);
INSERT INTO tournament VALUES(4002, 'World Championship Match', '13/10/2005', '30/10/2005', 3004);
INSERT INTO tournament VALUES(4003, 'European Chess Championship', '21/03/2005', '03/04/2005', 3005);
INSERT INTO tournament VALUES(4004, 'Mitropa Cup', '07/10/2006', '16/10/2006', 3008);

INSERT INTO chessplayer VALUES(2000, 'Carlsen', 'Magnus', 2800, 1001);
INSERT INTO chessplayer VALUES(2001, 'Aronian', 'Levon', 2700, 1002);
INSERT INTO chessplayer VALUES(2002, 'Kramnik', 'Vladimir', 2100, 1003);
INSERT INTO chessplayer VALUES(2003, 'Anand', 'Viswanathan', 1900, 1004);
INSERT INTO chessplayer VALUES(2004, 'Radjabov', 'Teimour', 1600, 1005);
INSERT INTO chessplayer VALUES(2005, 'Topalov', 'Veselin', 1600, 1006);
INSERT INTO chessplayer VALUES(2006, 'Karjakin', 'Sergey', 1500, 1007);
INSERT INTO chessplayer VALUES(2007, 'Ivanchuk', 'Vassily', 1200, 1007);
INSERT INTO chessplayer VALUES(2008, 'Morozevich', 'Alexander', 1000, 1003);
INSERT INTO chessplayer VALUES(2009, 'Gashimov', 'Vugar', 900, 1005);
INSERT INTO chessplayer VALUES(2010, 'Konovalov', 'Andrey', 1000, 1003);

INSERT INTO participation VALUES(2000, 4000, 25, 1);
INSERT INTO participation VALUES(2001, 4000, 20, 2);
INSERT INTO participation VALUES(2002, 4000, 10, 3);
INSERT INTO participation VALUES(2003, 4000, -10, 4);
INSERT INTO participation VALUES(2004, 4000, -20, 5);
INSERT INTO participation VALUES(2005, 4000, -25, 6);

INSERT INTO participation VALUES(2003, 4001, 13, 1);
INSERT INTO participation VALUES(2004, 4001, 7, 2);
INSERT INTO participation VALUES(2005, 4001, 3, 3);
INSERT INTO participation VALUES(2006, 4001, -7, 5);
INSERT INTO participation VALUES(2007, 4001, -3, 4);
INSERT INTO participation VALUES(2008, 4001, -13, 6);

INSERT INTO participation VALUES(2004, 4002, 30, 1);
INSERT INTO participation VALUES(2005, 4002, 13, 2);
INSERT INTO participation VALUES(2006, 4002, 2, 3);
INSERT INTO participation VALUES(2007, 4002, 1, 4);
INSERT INTO participation VALUES(2008, 4002, -13, 5);
INSERT INTO participation VALUES(2009, 4002, -33, 6);

INSERT INTO participation VALUES(2000, 4003, 37, 1);
INSERT INTO participation VALUES(2001, 4003, 29, 2);
INSERT INTO participation VALUES(2002, 4003, -11, 5);
INSERT INTO participation VALUES(2003, 4003, -39, 6);
INSERT INTO participation VALUES(2004, 4003, 11, 3);
INSERT INTO participation VALUES(2005, 4003, -27, 4);

INSERT INTO participation VALUES(2001, 4004, -23, 6);
INSERT INTO participation VALUES(2002, 4004, 23, 1);
INSERT INTO participation VALUES(2003, 4004, 14, 2);
INSERT INTO participation VALUES(2004, 4004, 11, 3);
INSERT INTO participation VALUES(2005, 4004, -15, 4);
INSERT INTO participation VALUES(2006, 4004, -10, 5);

/*
INSERT INTO current_rating VALUES(2000, 2800 + 37 + 25);
INSERT INTO current_rating VALUES(2001, 2700 + 20 + 29 - 23);
INSERT INTO current_rating VALUES(2002, 2100 + 10 - 11 + 23);
INSERT INTO current_rating VALUES(2003, 1900 - 10 + 13 - 39 + 14);
INSERT INTO current_rating VALUES(2004, 1600 - 20 + 30 + 7 + 11 + 11);
INSERT INTO current_rating VALUES(2005, 1600 - 25 + 3 + 13 - 27 - 15);
INSERT INTO current_rating VALUES(2006, 1500 - 7 + 2 - 10);
INSERT INTO current_rating VALUES(2007, 1200 - 3 + 1);
INSERT INTO current_rating VALUES(2008, 1000 - 13 - 13);
INSERT INTO current_rating VALUES(2009, 900 - 33);
INSERT INTO current_rating VALUES(2010, 1000 + 0);

INSERT INTO current_title VALUES(2000, 01);
INSERT INTO current_title VALUES(2001, 01);
INSERT INTO current_title VALUES(2002, 03);
INSERT INTO current_title VALUES(2003, 04);
INSERT INTO current_title VALUES(2004, 05);
INSERT INTO current_title VALUES(2005, 06);
INSERT INTO current_title VALUES(2006, 06);
INSERT INTO current_title VALUES(2007, 08);
INSERT INTO current_title VALUES(2008, 09);
INSERT INTO current_title VALUES(2009, 09);
*/
