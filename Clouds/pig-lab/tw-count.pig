dataset = LOAD 'tw.txt' AS (id: long, fr: long);

-- TODO: check if user IDs are valid (e.g. not null) and clean the dataset
SPLIT dataset INTO valid_users IF id is not NULL, invalid_users IF id is NULL;

-- TODO: organize data such that each node ID is associated to a list of neighbors
nodes = GROUP valid_users BY fr PARALLEL 3;
DUMP nodes;
DESCRIBE nodes;


-- TODO: foreach node ID generate an output relation consisting of the node ID and the number of "friends"
friends = FOREACH nodes GENERATE group, COUNT(valid_users) AS cnt;
DUMP friends;
DESCRIBE friends;
EXPLAIN friends;
--final = ORDER friends BY group DESC;
--DUMP final;
--DESCRIBE final;

STORE friends INTO './ex2output';
