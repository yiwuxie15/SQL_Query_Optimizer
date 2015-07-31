-------------simple “select” case------------------------

SELECT * FROM stars;
SELECT * FROM soaps;

SELECT stars.starid FROM stars;

SELECT stars.starid, stars.real_name FROM stars;
------------project in different order------------------------

SELECT stars.real_name,stars.starid from stars;

------------select with filter-----------------------
SELECT * from stars where stars.soapid=6;

SELECT * from stars where stars.soapid>6;

SELECT * from stars where stars.real_name='Ted';

--------------create index----------------

CREATE INDEX stars(soapid);

SELECT * from stars where stars.soapid=6;

SELECT * from stars where stars.soapid>6;

DROP INDEX stars(soapid);

