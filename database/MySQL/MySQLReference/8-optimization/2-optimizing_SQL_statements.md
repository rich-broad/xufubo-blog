数据库应用程序的核心逻辑是通过SQL语句来完成的。这里主要研究mysql对SQL语句的优化。从而写出mysql更加喜欢的SQL语句。

# 1 优化查询语句
查询以select语句的形式执行数据库中的查找操作。select语句由很多子语句组成，优化和调整这些子语句是重要的。除select外，查询优化的技术也适用于CREATE TABLE...AS SELECT, INSERT INTO...SELECT,和在DELETE和UPDATE的WHERE条件。这些语句具有额外的性能考虑，因为它们将写操作与面向读取的查询操作结合在一起。优化查询的主要考虑因素是：
 - 要让SELECT ... WHERE查询更快，首先要检查的是是否可以添加索引。为WHERE子句中使用的列设置索引，以加速过滤和最终检索结果。为了避免浪费磁盘空间，以及维护过多的索引带来的额外开销，通常构建一组索引，以加快应用程序中使用的许多相关查询。
 - 对于引用不同表的查询，索引尤其重要，可以使用连接和外键等功能。您可以使用EXPLAIN语句来确定哪些索引用于SELECT。
 - 隔离并调整查询的任何部分，例如，能在结果集上使用函数就不在表的每行上使用。
 - 尽量减少查询中全表扫描的次数，特别是对于大表格。
 - mysql的查询优化器是基于统计的，因此，通过定期使用ANALYZE TABLE语句保持表的统计数据是最新的，所以优化器具有构建高效执行计划所需的信息。
 - 了解和学习特定于存储引擎的技术。
 - 避免以难以理解的方式转换查询，特别是如果优化器自动执行一些相同的转换。
 - 如果性能问题不能通过基本准则轻松解决，请通过阅读EXPLAIN计划并调整索引，WHERE子句，连接子句等来研究特定查询的内部详细信息（这个非常重要）。
 - 合理的调整mysql的缓冲区，特别是InnoDB缓冲池和NyISAM的key缓冲的大小。以尽可能使查询过程中的I/O次数减少。
 - 即使对于使用高速缓存区快速运行的查询，您仍可能进一步优化，以便它们需要较少的高速缓存内存，从而使您的应用程序更具可扩展性。可扩展意味着您的应用程序可以同时处理更多的用户，更大的请求等等，而不会出现性能大幅下降的情况。
 - 处理锁定问题，其中查询速度可能会受到其他会话同时访问表的影响。

 ## 1.1 where条件优化
 接下来研究mysql对where条件的优化，当然了，mysql的优化器一直也在发展，这里记录的不一定完整。文档通常都是比源代码滞后的。对于where条件的优化，
  - 移除不必要的括号:
```sql
   ((a AND b) AND c OR (((a AND b) AND (c AND d))))
-> (a AND b AND c) OR (a AND b AND c AND d)
```
 - Constant folding:
 ```sql
   (a<b AND b=c) AND a=5
-> b>5 AND b=c AND a=5
 ```
 - 确定条件的去除
 ```sql
   (B>=5 AND B=5) OR (B=6 AND 5=5) OR (B=7 AND 5=6)
-> B=5 OR B=6
 ```
 - 由索引使用的常量表达式仅计算一次
 - 对于没有WHERE的单个表上的COUNT（*），直接从MyISAM和MEMORY表的表信息中查询，当仅与一个表一起使用时，这也适用于任何NOT NULL表达式。
 - 及时检测无效的常量表达式。 MySQL会快速检测到某些SELECT语句中的where条件不会发生，并且不返回任何行。
 - 如果不使用GROUP BY或聚合函数（COUNT（），MIN（）等），HAVING将与WHERE合并。
 - 对于连接中的每个表，构造一个更简单的WHERE以加快过滤。
 - 在查询任何其他表之前首先读取所有常量表。 常量表是以下任何一项：
    - 空表或者包含一行的表
    - 与PRIMARY KEY或UNIQUE索引中的WHERE子句一起使用的表，其中所有索引部分都与常量表达式进行比较并定义为NOT NULL。以下所有表格均用作常量表格：
```sql
SELECT * FROM t WHERE primary_key=1;
SELECT * FROM t1,t2
  WHERE t1.primary_key=1 AND t2.primary_key=t1.id;
```
 - 通过尝试所有可能性来找到联表的最佳联合组合。如果ORDER BY和GROUP BY子句中的所有列都来自同一个表，那么在连接时首选该表。
 - 如果存在ORDER BY子句和不同的GROUP BY子句，或者如果ORDER BY或GROUP BY包含来自联接队列中第一个表以外的表的列，则会创建一个临时表。
 - 如果您使用SQL_SMALL_RESULT修饰符，则MySQL使用内存中的临时表。
 - 除非优化器认为使用表扫描更高效，否则将查询每个表索引，并使用最佳索引。在之前，根据最佳索引是否超过表格的30％来使用扫描，但固定百分比这种做法，不再用于决定是使用索引还是全表扫描。优化器现在更加复杂，会基于其他因素进行考量，如表大小，行数和I/O块大小。
 - 在某些情况下，MySQL可以从索引中读取行，而无需查询数据文件。如果索引中使用的所有列都是数字，则仅使用索引树来解析查询。
 - 在输出每行之前，会跳过与HAVING子句不匹配的那些行。
一些很快的查询例子：
```sql
SELECT COUNT(*) FROM tbl_name;

SELECT MIN(key_part1),MAX(key_part1) FROM tbl_name;

SELECT MAX(key_part2) FROM tbl_name
  WHERE key_part1=constant;

SELECT ... FROM tbl_name
  ORDER BY key_part1,key_part2,... LIMIT 10;

SELECT ... FROM tbl_name
  ORDER BY key_part1 DESC, key_part2 DESC, ... LIMIT 10;
```
假设索引列是数值类型，那么mysql将仅使用索引树来解析如下查询：
```sql
SELECT key_part1,key_part2 FROM tbl_name WHERE key_part1=val;

SELECT COUNT(*) FROM tbl_name
  WHERE key_part1=val1 AND key_part2=val2;

SELECT key_part2 FROM tbl_name GROUP BY key_part1;
```
以下查询使用索引来按照排序顺序检索行，而无需单独的排序：
```sql
SELECT ... FROM tbl_name
  ORDER BY key_part1,key_part2,... ;

SELECT ... FROM tbl_name
  ORDER BY key_part1 DESC, key_part2 DESC, ... ;
```
## 1.2 范围优化