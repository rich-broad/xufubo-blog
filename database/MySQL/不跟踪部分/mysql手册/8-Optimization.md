# 优化
## 引言
对系统性能的优化是每一个工程师孜孜不倦追求的目标，现在随着数据量爆炸式的增长，一个系统的架构越来越复杂，几乎每一个系统都是分布式系统，每一个系统都分为多个层次，为了提高系统性能，系统中存在大量的缓存。目前来看，一个系统大概分为如下几层：
 - 接入层
 - 逻辑层
 - cache层（redis、memcahed等）
 - 持久层（mysql集群、分布式文件系统等）。
 每一层都需要做优化，系统的性能瓶颈取决于性能最低的地方。接入层通常来说主要是负载均衡和命令字转发的功能。就是网络包的透传，而且可以很容易的实现线性扩展。逻辑层通常基于RPC，也可以很容易的线性扩展，cache层，通常是对内存的直接操作，速度很快，而且通过k-v可以很容易的做到分布式，最为复杂麻烦的就在于持久层。这一层通常难以扩展，而且对于数据库而言，也有很多点可以优化。现在互联网企业多用mysql集群作为持久层。掌握好mysql的优化，将变得很有意义。接下来探讨和学习mysql的优化，接下来的内容主要来自mysql8.0的官方手册，也会有自己的一些理解。
 ## 1 概述
 优化是一门很大的学问，而且涉及到方方面面不同层次的优化。而且不同的角色，优化关注的点不尽相同。你可以例如，对于DBA而言，优化服务器的配置（缓存区、线程数等）、与开发人员一起优化表的设计，根据业务，配置合适的复制等就非常重要，对于开发人员，不仅要和DBA讨论业务，设计出合理的表结构，数据类型，同时，也需要理解mysql的查询执行，优化器的优化程度，依据业务特点设计合理的索引等。接下来就学习mysql优化的方方面面。
数据库的性能取决于数据库各个级别的因素，如：配置、表设计、查询语句等。在处理数据库性能时，应该首先掌握**基本的规则和准则**，随着学习的深入，再去了解内部发生的事情，这时候，就需要深入到源码级别进行优化。
一般的用户旨在从现有的软件和硬件配置中获得最佳的数据库性能。高级用户通常需要结合自己的业务，改进MySQL软件本身（例如国内的bat都需要定制和改造mysql，因为现有的软件无法满足业务需求），或者开发自己的存储引擎和硬件设备来扩展MySQL生态系统。
对于优化通常有如下几点：
 - 数据库级别的优化
 - 硬件级别的优化
 - 在可移植性和性能之间平衡
### 数据库级别的优化
使数据库应用程序加快的最重要因素是其基本设计：
 - 表结构设计是否合理？数据类型是否正确？
 - 正确的索引舍否建立？
 - 选择的存储引擎是否合理？
 - 表格是否使用合适的行格式？
 - 应用程序是否使用合适的锁策略？例如，在可能的情况下允许共享访问，以便数据库操作可以并发运行，在适当时请求独占访问，从而使关键操作成为最高优先级。同样，存储引擎的选择也很重要。 InnoDB存储引擎可处理大多数锁问题，而无需您的参与，从而实现数据库更好的并发性并减少代码的实验和调整量。
 - 内存缓冲区配置是否合理？主要涉及到InnoDB的缓冲池和MyISAM的key缓存。
 - 线程数的设置是否合理？
 - 依据业务的特点，事务的隔离级别选择是否合理？
 - 复制设置是否合适？

### 硬件级别的优化
随着数据库变得越来越繁忙，任何数据库应用程序最终都会遇到硬件限制。DBA必须评估是否可以调整应用程序或重新配置服务器以避免这些瓶颈，或者是否需要更多硬件资源。系统瓶颈通常来自这些：
 - 磁盘寻到，对于磁盘而言，主要是寻道时间，新的次磁盘也不会有数量级上的提升，因此，对于磁盘的优化，优化查找时间的方法是将数据分配到多个磁盘上，通过并行处理来提高。
 - 磁盘的读取和写入，也可以通过多个磁盘并行读取来优化。
 - CPU时钟周期，当数据在内存中时，我们必须对其进行处理以得到我们的结果。与内存数量相比，大表是最常见的限制因素。但对于小表，速度通常不是问题。
 - 内存带宽，当CPU需要的数据量超过CPU缓存容量时，内存带宽就成为瓶颈。对大多数系统来说，这是一个罕见的瓶颈，但需要注意的一个瓶颈。

 ### 在可移植性和性能之间平衡
为了使用mysql特有的性能，又为了使应用程序具有可移植性，你可以将mysql特定的关键字包含在/*! */中，其他的SQL服务器会忽略这些注释。
通常情况下，对于一个软件工程师而言，硬件级别的优化通常不可控，但是数据库级别的优化却是完全可以的，通常也是优化的重点，接下来着重学习数据库级别的优化。

## 2 优化SQL语句
数据库应用程序的核心逻辑是通过SQL语句来完成的。这里主要研究mysql对SQL语句的优化。从而写出mysql更加喜欢的SQL语句。

### 2.1 优化查询语句
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

 #### 2.1.1 where条件优化
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
#### 2.1.2 范围优化


## 8 了解查询执行计划
上边我们学习了mysql的优化技术相关的理论，不过毕竟是理论，而且有可能文档跟不上代码的变化是常有的事，因此，具体了解mysql为某个查询生成的执行计划（也称为EXPLAIN计划）将是非常有必要的，通过观察mysql生成的查询计划，来进行更进一步的优化。  

### 8.1 用EXPLAIN优化查询
EXPLAIN语句提供了有关MySQL如何执行语句的信息：  
 - EXPLAIN与SELECT，DELETE，INSERT，REPLACE和UPDATE语句一起使用。
 - 当EXPLAIN与可解释的语句一起使用时，MySQL会显示来自优化器的关于语句执行计划的信息。也就是说，MySQL解释了它将如何处理该语句，包括有关表如何连接以及按何种顺序处理等信息。接下来会详细说明。  
 - 当EXPLAIN与FOR CONNECTION connection_id而不是可解释的语句一起使用时，它会显示在命名连接中执行的语句的执行计划，见[8.8.4 Obtaining Execution Plan Information for a Named Connection](https://dev.mysql.com/doc/refman/8.0/en/explain-for-connection.html)。  
 - 对于SELECT语句，EXPLAIN会生成可以使用SHOW WARNINGS显示的其他执行计划信息  
 - EXPLAIN对于检查涉及分区表的查询很有用。  
 - FORMAT选项可用于选择输出格式。TRADITIONAL表示以表格形式呈现输出结果，这是默认值。JSON格式以JSON格式显示信息。  
在EXPLAIN的帮助下，你可以看到应该在哪里添加索引，以便通过使用索引来更快地执行语句。也可以使用EXPLAIN来检查优化器是否以最佳顺序join表。为了提示优化器使用与SELECT语句中的表命名顺序相对应的连接顺序，用SELECT STRAIGHT_JOIN而不是SELECT来开始查询语句，但是，STRAIGHT_JOIN可能会阻止使用索引，因为它会禁用半连接转换（semi-join transformations），见[ Section 8.2.2.1, “Optimizing Subqueries, Derived Tables, View References, and Common Table Expressions with Semi-Join Transformations](https://dev.mysql.com/doc/refman/8.0/en/semi-joins.html)。   
优化器跟踪有时可以提供与EXPLAIN的信息互补的信息。但是，优化器跟踪的格式和内容在版本之间可能会发生变化，具体见[MySQL Internals: Tracing the Optimizer](https://dev.mysql.com/doc/internals/en/optimizer-tracing.html)。  
如果你通过EXPLAIN确认了mysql优化器没有使用应该使用的索引，那么，运行ANALYZE TABLE来更新表的统计信息。见[Section 13.7.3.1 ANALYZE TABLE Syntax](https://dev.mysql.com/doc/refman/8.0/en/analyze-table.html)。  
 > - **注意：**  
  EXPLAIN也可以用于获取有关表中列的信息，EXPLAIN tbl_name与DESCRIBE tbl_name和SHOW COLUMNS FROM tbl_name同义。

### 8.2 EXPLAIN输出格式
EXPLAIN为SELECT语句中使用的每个表返回一行信息。mysql按照在处理语句时读取它们的顺序列出输出中的表。MySQL使用嵌套循环(nested-loop)连接方法解析所有连接。这意味着MySQL从第一个表中读取一行，然后在第二个表，第三个表中找到匹配的行，等等。  
**EXPLAIN的输出列**  
```sql
mysql> explain select * from t1;
+----+-------------+-------+------------+------+---------------+------+---------+------+------+----------+-------+
| id | select_type | table | partitions | type | possible_keys | key  | key_len | ref  | rows | filtered | Extra |
+----+-------------+-------+------------+------+---------------+------+---------+------+------+----------+-------+
|  1 | SIMPLE      | t1    | NULL       | ALL  | NULL          | NULL | NULL    | NULL |    1 |   100.00 | NULL  |
+----+-------------+-------+------------+------+---------------+------+---------+------+------+----------+-------+
1 row in set, 1 warning (0.01 sec)
```
如上，输出的列解释如下：  
  
|列名|JSON名称|含义|  
|-|-|-|  
|id|select_id|该SELECT标识符|  
|select_type|没有|该SELECT类型|  
|table|table_name|表名|  
|partitions|partitions|匹配的分区|  
|type|access_type|连接类型|  
|possible_keys|possible_keys|可能的索引选择|  
|key|key|实际选择的索引|  
|key_len|key_length|所选键的长度|  
|ref|ref|与键列中指定的索引进行比较的列|  
|rows|rows|要检查的行数（越小越好）|  
|filtered|filtered|符合条件的结果行所占检查行的百分比(越大越好)|  
|Extra|没有|附加信息（很重要）|  
  
举例：  
```sql
mysql> show create table t2;
+-------+---------------------------------------------------------------------------+
| Table | Create Table                                                              |
+-------+---------------------------------------------------------------------------+
| t2    | CREATE TABLE `t2` (
  `id` int(11) DEFAULT NULL,
  `col` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 |
+-------+---------------------------------------------------------------------------+
1 row in set (0.04 sec)

mysql> select * from t2;
+------+------+
| id   | col  |
+------+------+
|    1 |    1 |
|    2 |    2 |
|    3 |    7 |
+------+------+
3 rows in set (0.00 sec)

mysql> explain select * from t2 where id = 1;
+----+-------------+-------+------------+------+---------------+------+---------+------+------+----------+-------------+
| id | select_type | table | partitions | type | possible_keys | key  | key_len | ref  | rows | filtered | Extra       |
+----+-------------+-------+------------+------+---------------+------+---------+------+------+----------+-------------+
|  1 | SIMPLE      | t2    | NULL       | ALL  | NULL          | NULL | NULL    | NULL |    3 |    33.33 | Using where |
+----+-------------+-------+------------+------+---------------+------+---------+------+------+----------+-------------+
1 row in set, 1 warning (0.01 sec)

mysql> alter table t2 add unique key uk_id(`id`);

mysql> explain select * from t2 where id = 1;
+----+-------------+-------+------------+-------+---------------+-------+---------+-------+------+----------+-------+
| id | select_type | table | partitions | type  | possible_keys | key   | key_len | ref   | rows | filtered | Extra |
+----+-------------+-------+------------+-------+---------------+-------+---------+-------+------+----------+-------+
|  1 | SIMPLE      | t2    | NULL       | const | uk_id         | uk_id | 5       | const |    1 |   100.00 | NULL  |
+----+-------------+-------+------------+-------+---------------+-------+---------+-------+------+----------+-------+
1 row in set, 1 warning (0.00 sec)
```
```sql
mysql> show create table t;
+-------+------------------------------------------------------------------------------+
| Table | Create Table                                                                 |
+-------+------------------------------------------------------------------------------+
| t     | CREATE TABLE `t` (
  `a` int(10) unsigned zerofill DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 |
+-------+------------------------------------------------------------------------------+
1 row in set (0.00 sec)

mysql> show create table t2;
+-------+------------------------------------------------------------------------------+
| Table | Create Table                                                                 |
+-------+------------------------------------------------------------------------------+
| t2    | CREATE TABLE `t2` (
  `id` int(11) DEFAULT NULL,
  `col` int(11) DEFAULT NULL,
  UNIQUE KEY `uk_id` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 |
+-------+------------------------------------------------------------------------------+
1 row in set (0.00 sec)

mysql> show create table t4;
+-------+------------------------------------------------------------------------------+
| Table | Create Table                                                                 |
+-------+------------------------------------------------------------------------------+
| t4    | CREATE TABLE `t4` (
  `id` int(11) DEFAULT NULL,
  `col` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 |
+-------+------------------------------------------------------------------------------+
1 row in set (0.00 sec)

mysql> select * from t;
+------------+
| a          |
+------------+
| 0000001000 |
| 0000000657 |
| 0000000657 |
+------------+
3 rows in set (0.01 sec)

mysql> select * from t2;
+------+------+
| id   | col  |
+------+------+
|    1 |    1 |
|    2 |    2 |
|    3 |    7 |
+------+------+
3 rows in set (0.00 sec)

mysql> select * from t4;
+------+------+
| id   | col  |
+------+------+
|    1 |    1 |
|    2 |    2 |
|    3 |    7 |
+------+------+
3 rows in set (0.00 sec)
mysql> explain select * from t4, t where t4.id = t.a;
+----+-------------+-------+------------+------+---------------+------+---------+------+------+----------+----------------------------------------------------+
| id | select_type | table | partitions | type | possible_keys | key  | key_len | ref  | rows | filtered | Extra                                              |
+----+-------------+-------+------------+------+---------------+------+---------+------+------+----------+----------------------------------------------------+
|  1 | SIMPLE      | t     | NULL       | ALL  | NULL          | NULL | NULL    | NULL |    2 |   100.00 | NULL                                               |
|  1 | SIMPLE      | t4    | NULL       | ALL  | NULL          | NULL | NULL    | NULL |    3 |    33.33 | Using where; Using join buffer (Block Nested Loop) |
+----+-------------+-------+------------+------+---------------+------+---------+------+------+----------+----------------------------------------------------+
2 rows in set, 1 warning (0.00 sec)

mysql> explain select * from t2, t where t2.id = t.a;
+----+-------------+-------+------------+------+---------------+-------+---------+----------+------+----------+-----------------------+
| id | select_type | table | partitions | type | possible_keys | key   | key_len | ref      | rows | filtered | Extra                 |
+----+-------------+-------+------------+------+---------------+-------+---------+----------+------+----------+-----------------------+
|  1 | SIMPLE      | t     | NULL       | ALL  | NULL          | NULL  | NULL    | NULL     |    2 |   100.00 | Using where           |
|  1 | SIMPLE      | t2    | NULL       | ref  | uk_id         | uk_id | 5       | test.t.a |    1 |   100.00 | Using index condition |
+----+-------------+-------+------------+------+---------------+-------+---------+----------+------+----------+-----------------------+
2 rows in set, 1 warning (0.00 sec)

```
  
如上是一些例子，接下来，详细的对各个列的含义进行说明：  
 - id（JSON名： select_id）：SELECT标识符。这是SELECT查询的内部连续编号。
 - select_type （JSON名称：无）：可以是下表的任何类型。  
  
 |select_type值|JSON名称|含义|  
 |-|-|-|   
 |SIMPLE|无|简单SELECT（不使用 UNION或子查询）|  
 |PRIMARY|无|Outermost SELECT|  
 |UNION|无||  
 |DEPENDENT UNION|dependent (true)||  
 |UNION RESULT|union_result||  
 |SUBQUERY|无||  
 |DEPENDENT SUBQUERY|dependent (true)||  
 |DERIVED|无||  
 |MATERIALIZED|materialized_from_subquery||  
 |UNCACHEABLE SUBQUERY|cacheable (false)||  
 |UNCACHEABLE UNION|cacheable (false)||  
   

