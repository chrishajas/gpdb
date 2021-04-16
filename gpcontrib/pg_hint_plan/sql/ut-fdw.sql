LOAD 'pg_hint_plan';
SET search_path TO public;
SET pg_hint_plan.debug_print TO on;
SET client_min_messages TO LOG;
SET pg_hint_plan.enable_hint TO on;

CREATE EXTENSION file_fdw;
CREATE SERVER file_server FOREIGN DATA WRAPPER file_fdw;
CREATE USER MAPPING FOR PUBLIC SERVER file_server;
CREATE FOREIGN TABLE ft1 (id int, val int) SERVER file_server OPTIONS (format 'csv', filename '/Users/chajas/workspace/pg_hint_plan/data/data.csv');

-- foreign table test
SELECT * FROM ft1;
\t
EXPLAIN (COSTS false) SELECT * FROM s1.t1, ft1 ft_1, ft1 ft_2 WHERE t1.c1 = ft_1.id AND t1.c1 = ft_2.id;

----
---- No. S-1-5 object type for the hint
----

-- No. S-1-5-6
/*+SeqScan(t1)SeqScan(ft_1)SeqScan(ft_2)*/
EXPLAIN (COSTS false) SELECT * FROM s1.t1, ft1 ft_1, ft1 ft_2 WHERE t1.c1 = ft_1.id AND t1.c1 = ft_2.id;

----
---- No. J-1-6 object type for the hint
----

-- No. J-1-6-6
/*+MergeJoin(ft_1 ft_2)Leading(ft_1 ft_2 t1)*/
EXPLAIN (COSTS false) SELECT * FROM s1.t1, ft1 ft_1, ft1 ft_2 WHERE t1.c1 = ft_1.id AND t1.c1 = ft_2.id;

----
---- No. L-1-6 object type for the hint
----

-- No. L-1-6-6
/*+Leading(ft_1 ft_2 t1)*/
EXPLAIN (COSTS false) SELECT * FROM s1.t1, ft1 ft_1, ft1 ft_2 WHERE t1.c1 = ft_1.id AND t1.c1 = ft_2.id;

----
---- No. R-1-6 object type for the hint
----

-- No. R-1-6-6
\o results/ut-fdw.tmpout
/*+Rows(ft_1 ft_2 #1)Leading(ft_1 ft_2 t1)*/
EXPLAIN SELECT * FROM s1.t1, ft1 ft_1, ft1 ft_2 WHERE t1.c1 = ft_1.id AND t1.c1 = ft_2.id;
\o
\! sql/maskout.sh results/ut-fdw.tmpout
\! rm results/ut-fdw.tmpout
