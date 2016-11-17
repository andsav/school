SELECT COUNT(*) FROM (
	SELECT SUM(CASE WHEN term LIKE "%transaction%" THEN 1 ELSE 0 END) AS tr,
	SUM(CASE WHEN term LIKE "%world%" THEN 1 ELSE 0 END) AS w
	FROM frequency GROUP BY docid
	HAVING tr > 0 AND w > 0
);


