CREATE TABLE table1 (
        id integer,
        name char(7)
);
CREATE TABLE table2 (
        id2 integer,
        name2 char(10),
        class integer
);

INSERT INTO table1(id, name) VALUES (1, 'bob');
INSERT INTO table1(id, name) VALUES (2, 'jim');
INSERT INTO table1(id, name) VALUES (2, 'andy');
INSERT INTO table1(id, name) VALUES (2, 'nathan');
INSERT INTO table1(id, name) VALUES (3, 'timothy');
INSERT INTO table1(id, name) VALUES (4, 'sally');

INSERT INTO table2(id2, name2, class) VALUES (1, 'todd', 8);
INSERT INTO table2(id2, name2, class) VALUES (1, 'tommy', 8);
INSERT INTO table2(id2, name2, class) VALUES (2, 'terr', 7);
INSERT INTO table2(id2, name2, class) VALUES (3, 'tom123', 6);
INSERT INTO table2(id2, name2, class) VALUES (4, 'salvador', 10);
INSERT INTO table2(id2, name2, class) VALUES (4, 'benson', 11);
INSERT INTO table2(id2, name2, class) VALUES (4, 'kyle', 12);
INSERT INTO table2(id2, name2, class) VALUES (4, 'alyssa', 13);
INSERT INTO table2(id2, name2, class) VALUES (4, 'benji', 14);

SELECT * FROM table1;

SELECT * FROM table2;

SELECT table1.name FROM table1;

SELECT table2.name2, table2.id2 FROM table2;

SELECT table2.name2, table2.id2 FROM table2 WHERE table2.id2 = 4;

CREATE INDEX table2(id2);

SELECT table2.name2, table2.id2 FROM table2 WHERE table2.id2 = 4;

SELECT table2.name2, table2.id2 FROM table2 WHERE table2.id2 < 4;

SELECT table2.name2, table2.id2 FROM table2 WHERE table2.id2 > 2;

SELECT table2.name2, table2.id2 FROM table2 WHERE table2.id2 <= 4;

SELECT table2.name2, table2.class, table2.id2 FROM table2 WHERE table2.class >= 11;

SELECT table2.name2, table2.id2 FROM table2 WHERE table2.id2 <> 4;

DROP TABLE table1;

DROP TABLE table2;




