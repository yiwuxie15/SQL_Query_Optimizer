CREATE TABLE table1 (
        id integer,
	name char(7)
);
CREATE TABLE table2 (
        id2 integer,
        name2 char(10),
        class integer
);

INSERT INTO table1(id, name) VALUES (1, 'bob1');
INSERT INTO table1(id, name) VALUES (1, 'bob2');
INSERT INTO table1(id, name) VALUES (1, 'bob3');
INSERT INTO table1(id, name) VALUES (2, 'jim');
INSERT INTO table1(id, name) VALUES (3, 'timothy');
INSERT INTO table1(id, name) VALUES (4, 'sally');
INSERT INTO table1(id, name) VALUES (5, 'peter1');
INSERT INTO table1(id, name) VALUES (5, 'peter2');
INSERT INTO table1(id, name) VALUES (5, 'peter3');
INSERT INTO table1(id, name) VALUES (5, 'peter4');
INSERT INTO table1(id, name) VALUES (6, 'pablo');
INSERT INTO table1(id, name) VALUES (7, 'jjwatt');
INSERT INTO table1(id, name) VALUES (8, 'sammy');
INSERT INTO table1(id, name) VALUES (9, 'garris');
INSERT INTO table1(id, name) VALUES (10, 'joseph');

INSERT INTO table2(id2, name2, class) VALUES (1, 'todd', 111);
INSERT INTO table2(id2, name2, class) VALUES (2, 'terry', 222);
INSERT INTO table2(id2, name2, class) VALUES (3, 'tom1', 333);
INSERT INTO table2(id2, name2, class) VALUES (3, 'tom2', 334);
INSERT INTO table2(id2, name2, class) VALUES (3, 'tom3', 335);
INSERT INTO table2(id2, name2, class) VALUES (3, 'tom4', 336);
INSERT INTO table2(id2, name2, class) VALUES (3, 'tom5', 337);
INSERT INTO table2(id2, name2, class) VALUES (4, 'sandy', 444);
INSERT INTO table2(id2, name2, class) VALUES (5, 'honduras', 555);
INSERT INTO table2(id2, name2, class) VALUES (6, 'pepe', 666);
INSERT INTO table2(id2, name2, class) VALUES (7, 'mack', 777);
INSERT INTO table2(id2, name2, class) VALUES (8, 'CJ', 888);
INSERT INTO table2(id2, name2, class) VALUES (9, 'VJ', 999);
INSERT INTO table2(id2, name2, class) VALUES (10, 'TJ', 101010);
INSERT INTO table2(id2, name2, class) VALUES (11, 'joeshmo', 1234567);

SELECT * FROM table1;

SELECT * FROM table2;

CREATE INDEX table1(id);

SELECT table1.name, table2.class FROM table1, table2 WHERE table1.id = table2.id2;

CREATE INDEX table2(id2);

SELECT table1.name, table2.class FROM table1, table2 WHERE table1.id = table2.id2;

DROP INDEX table1(id);

SELECT table1.name, table2.class FROM table1, table2 WHERE table1.id =  table2.id2;

DROP INDEX table2(id2);

DROP TABLE table1;

DROP TABLE table2;

