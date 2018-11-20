---
title: 了解查询执行计划
---
# 8 了解查询执行计划
上边我们学习了mysql的优化技术相关的理论，不过毕竟是理论，而且有可能文档跟不上代码的变化是常有的事，因此，具体了解mysql为某个查询生成的执行计划（也称为EXPLAIN计划）将是非常有必要的，通过观察mysql生成的查询计划，来进行更进一步的优化。  

## 8.1 用EXPLAIN优化查询
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

## 8.2 EXPLAIN输出格式
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
   

