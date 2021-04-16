### Hackday 2021: pg_hint_plan for GPDB

##### A port of the open-source postgres pg_hint_plan module for use in GPDB's planner

- Works well for planner!
- Based on https://github.com/ossc-db/pg_hint_plan, with minor modifications to work with GPDB's codebase

#### Some nice features:
 - Force a particular scan type (index, seq, bitmap, noindex, etc.)
 - Force a particular join type (nested loop, hash, merge, nohash)
 - Force a particular join order, or force that two tables are directly joined together
 - Set a guc on a query level
 - Set the cardinality of an inaccurate join estimate
 - Can use the hint table extension to apply a hint to queries that match a particular pattern

#### Some potential use cases:
 - Improving developer workflow:
	 - Creating very targeted ICW tests by forcing a particular query plan. Makes the intent of the test obvious and reduces noise due to other parts of the plan changing.
	   As an example, we can "force" a bad, invalid plan, and make sure that doesn't get selected. Currently we do this by setting/unsetting session gucs in tests, but that's cumbersome
	 - Performance testing, testing changes to cost models
	 - Testing how a cardinality change in the optimizer affects the overall plan
 - Improving user experience (more controversial):
 - shipping this to users, either undocumented and only used by us for debugging, or documented and allowing customers to get themselves out of a bind due to poor cardinality estimates or an overall bad plan.

#### How to use:

 Run `make && make install` in this directory
For session level, run `LOAD 'pg_hint_plan';`
For cluster level, add to postgresql.conf:
```
# postgresql.conf
shared_preload_libraries = 'pg_hint_plan'
custom_variable_classes = 'pg_hint_plan' 
```

Use `set pg_hint_plan.debug_print=on` to see which hints are used.

Example cases:

1) Test that AO table uses a bitmap scan, not an index scan (which isn't supported in the executor, so the planner shouldn't generate)
```
/*+IndexScan(ao)*/ explain select * from ao where a=3;
                                           QUERY PLAN
-------------------------------------------------------------------------------------------------
 Gather Motion 1:1  (slice1; segments: 1)  (cost=10000000004.90..10000000041.49 rows=96 width=4)
   ->  Bitmap Heap Scan on ao  (cost=10000000004.90..10000000040.21 rows=32 width=4)
         Recheck Cond: (a = 3)
         ->  Bitmap Index Scan on ao_idx  (cost=0.00..4.89 rows=32 width=0)
               Index Cond: (a = 3)
 Optimizer: Postgres query optimizer
(6 rows)
```

instead of 
```
set enable_seqscan=off;
set enable_indexscan=off;
explain select * from ao where a=3;
reset enable_seqscan;
reset enable_indexscanscan;
```

2) Force a merge join plan
```
test=# /*+MergeJoin(t1 t2)*/ EXPLAIN (COSTS false) SELECT * FROM t1, t2 WHERE t1.id = t2.id;
                 QUERY PLAN
--------------------------------------------
 Gather Motion 3:1  (slice1; segments: 3)
   ->  Merge Join
         Merge Cond: (t1.id = t2.id)
         ->  Index Scan using t1_pkey on t1
         ->  Sort
               Sort Key: t2.id
               ->  Seq Scan on t2
 Optimizer: Postgres query optimizer
(8 rows)
```

3) Force a specific join order
```
/*+Leading((((t1 t2)t3)(t4 t5)))*/
EXPLAIN (COSTS false) SELECT * FROM t1, t2, t3, t4, t5 WHERE t1.id = t2.id AND t1.id = t3.id AND t1.id = t4.id AND t1.id = t5.id;
                   QUERY PLAN
------------------------------------------------
 Gather Motion 3:1  (slice1; segments: 3)
   ->  Hash Join
         Hash Cond: (t1.id = t4.id)
         ->  Hash Join
               Hash Cond: (t1.id = t3.id)
               ->  Hash Join
                     Hash Cond: (t1.id = t2.id)
                     ->  Seq Scan on t1
                     ->  Hash
                           ->  Seq Scan on t2
               ->  Hash
                     ->  Seq Scan on t3
         ->  Hash
               ->  Hash Join
                     Hash Cond: (t4.id = t5.id)
                     ->  Seq Scan on t4
                     ->  Hash
                           ->  Seq Scan on t5
 Optimizer: Postgres query optimizer
(19 rows)
```
