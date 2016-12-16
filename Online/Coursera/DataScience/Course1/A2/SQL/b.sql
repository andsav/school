SELECT COUNT(*) FROM (
	SELECT SUM(count) AS c FROM frequency GROUP BY docid HAVING c > 300
);


