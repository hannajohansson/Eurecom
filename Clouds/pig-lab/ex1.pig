A = LOAD '/laboratory/gutenberg_big.txt';
DUMP A;
B = FOREACH A GENERATE FLATTEN(TOKENIZE((chararray)$0)) AS word;
DUMP B;
C = FILTER B BY word MATCHES '\\w+';
DUMP C;
D = GROUP C BY word PARALLEL 3;
DUMP D;
E = FOREACH D GENERATE group, COUNT(C);
DUMP E;
STORE E INTO './WORD_COUNT/';

