-- TODO: load the input dataset, located in ./local-input/OSN/tw.txt
A = LOAD '/laboratory/twitter-big.txt' AS (id1: long, fr1: long);

B = LOAD '/laboratory/twitter-big.txt' AS (id2: long, fr2: long);

-- TODO: compute all the two-hop paths 
twohop = JOIN A BY fr1, B BY id2 PARALLEL 20;
--DUMP twohop;
--DESCRIBE twohop;

-- TODO: project the twohop relation such that in output you display only the start and end nodes of the two hop path
p_result = FOREACH twohop GENERATE id1,fr2;
--DUMP p_result;
--DESCRIBE p_result; 

-- TODO: make sure you avoid loops (e.g., if user 12 and 13 follow eachother) 
result = FILTER p_result BY id1 != fr2;
f_result = DISTINCT result PARALLEL 20;
--DUMP f_result;
STORE f_result INTO './twjbig/';
