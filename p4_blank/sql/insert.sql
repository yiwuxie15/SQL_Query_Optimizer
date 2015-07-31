DROP TABLE soaps;
DROP TABLE stars;
-- create relations
CREATE TABLE soaps(soapid integer, name char(32), 
                   network char(4), rating double);
CREATE TABLE stars(starid integer, real_name char(20), 
                   plays char(12), soapid integer);

---------insert with order------------------

INSERT INTO stars(starid, real_name, plays, soapid)   VALUES (1, 'Kay', 'sha', 1);

INSERT INTO stars(starid, real_name, plays, soapid)   VALUES (2, 'Ann, Parker', 'Arbor', 3);

INSERT INTO stars(starid, real_name, plays, soapid)   VALUES (3, 'Titan', 'Seng', 5);

INSERT INTO stars(starid, real_name, plays, soapid)   VALUES (4, 'song', 'kkk', 6);

INSERT INTO stars(starid, real_name, plays, soapid)   VALUES (5, 'Tang', 'Hao', 7);

INSERT INTO stars(starid, real_name, plays, soapid)   VALUES (6, 'Zeng', 'Hen', 9);

INSERT INTO stars(starid, real_name, plays, soapid)   VALUES (7, 'Ross', 'Robit', 12);

INSERT INTO stars(starid, real_name, plays, soapid)   VALUES (8, 'liu', 'lll', 12);

INSERT INTO stars(starid, real_name, plays, soapid)   VALUES (9, 'NI', 'HAO', 12);

INSERT INTO stars(starid, real_name, plays, soapid)   VALUES (10, 'China', 'USA', 12);

---------insert without order------------

INSERT INTO stars(real_name, starid, plays, soapid)   VALUES ('Ge', 9, 'Di', 9);

INSERT INTO stars(real_name, starid, plays, soapid)   VALUES ('Jiuli', 10, 'Timber', 10);

INSERT INTO stars(real_name, starid, plays, soapid)   VALUES ('Ke', 13, 'Zhu', 12);

----------insert with null-------------------

INSERT INTO stars(real_name, starid, plays)   VALUES ('Ted', 9, 'peter');

----------insert exceeding limit-------------------

INSERT INTO stars(real_name, starid, plays, soapid)   VALUES ('Ted', 9, 'peterrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr', 11);

---------insert some data into soap-----------------

INSERT INTO soaps (soapid, name, network, rating) VALUES (1, 'Ann', 'A', 1.00);

INSERT INTO soaps (soapid, name, network, rating) VALUES (2, 'Baltimore', 'B', 2.00);

INSERT INTO soaps (soapid, name, network, rating) VALUES (3, 'Celitc', 'C', 1.20);

INSERT INTO soaps (soapid, name, network, rating) VALUES (6, 'Dallas', 'D', 3.00);

INSERT INTO soaps (soapid, name, network, rating) VALUES (6, 'Ellington', 'E', 3.40);

INSERT INTO soaps (soapid, name, network, rating) VALUES (7, 'Flton', 'F', 3.68);

INSERT INTO soaps (soapid, name, network, rating) VALUES (9, 'Gallon', 'G', 3.78);

INSERT INTO soaps (soapid, name, network, rating) VALUES (10, 'Hilton', 'H', 3.88);

INSERT INTO soaps (soapid, name, network, rating) VALUES (12, 'Illinois', 'I', 4.00);