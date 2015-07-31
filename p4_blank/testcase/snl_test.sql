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
INSERT INTO table1(id, name) VALUES (2, 'jim1');
INSERT INTO table1(id, name) VALUES (2, 'jim2');
INSERT INTO table1(id, name) VALUES (2, 'jim3');
INSERT INTO table1(id, name) VALUES (3, 'timothy');
INSERT INTO table1(id, name) VALUES (4, 'sally');

INSERT INTO table2(id2, name2, class) VALUES (1, 'todd1', 8);
INSERT INTO table2(id2, name2, class) VALUES (1, 'todd2', 8);
INSERT INTO table2(id2, name2, class) VALUES (2, 'terr', 7);
INSERT INTO table2(id2, name2, class) VALUES (3, 'tom123', 6);
INSERT INTO table2(id2, name2, class) VALUES (4, 'salv1', 10);
INSERT INTO table2(id2, name2, class) VALUES (4, 'salv2', 11);
INSERT INTO table2(id2, name2, class) VALUES (4, 'salv3', 12);
INSERT INTO table2(id2, name2, class) VALUES (4, 'salv4', 13);
INSERT INTO table2(id2, name2, class) VALUES (4, 'salv5', 14);

SELECT * FROM table1;

SELECT * FROM table2;

SELECT table1.name, table2.class FROM table1, table2 WHERE table1.id > table2.id2;

CREATE INDEX table1(id);

SELECT table1.name, table2.class FROM table1, table2 WHERE table1.id > table2.id2;

SELECT table1.name, table2.class FROM table1, table2 WHERE table1.id >= table2.id2;

SELECT table1.name, table2.class FROM table1, table2 WHERE table1.id < table2.id2;

SELECT table1.name, table2.class FROM table1, table2 WHERE table1.id <= table2.id2;

SELECT table1.name, table2.class FROM table1, table2 WHERE table1.id <> table2.id2;

DROP TABLE table1;

DROP TABLE table2;

