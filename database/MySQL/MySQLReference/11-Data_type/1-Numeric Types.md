# 数据类型
mysql支持多种sql数据类型，包括：数值型、日期和时间类型、字符串类型、空间类型、json类型。在本章的学习中，具有如下约定：  
 - M表示整数类型的最大***显示***宽度。对于浮点型和定点型，M是可以存储的总位数（精度）。对于字符串类型，M是最大长度。最大允许值M取决于数据类型。    
 - D适用于浮点类型和定点类型，表示小数点后的位数（比例）。最大可能值是30，但是不应大于M-2。    
 - fsp适用于TIME，DATETIME和TIMESTAMP类型，表示秒小数部分的精度。也就是小数点后的位数。fsp值必须在0到6的范围内。值为0表示不存在小数部分。如果省略，则默认精度为0（这是为了兼容以前版本的mysql，这与SQL标准的默认值为6有所不同）。  
 - 方括号（[和]）表示类型定义的可选部分。    

---
## 1、数值型
数值型分为如下几类：  
 - 整数  
 - 定点数  
 - 浮点数  
 - bit值  

下边将对这些类型一一详述。  
对于数值型，如果指定ZEROFILL，MySQL会自动将UNSIGNED(无符号的)属性添加到该列。默认情况下为有符号的。例如：  
```sql
use test;
mysql> create table t( a int ZEROFILL);
Query OK, 0 rows affected (0.05 sec)

mysql> show create table t;
+-------+-------------------------------------------------------------------------+
| Table | Create Table                                                            |
+-------+-------------------------------------------------------------------------+
| t     | CREATE TABLE `t` (
  `a` int(10) unsigned zerofill DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 |
+-------+-------------------------------------------------------------------------+
1 row in set (0.00 sec)

mysql> create table t1( a decimal(10, 6) ZEROFILL);
Query OK, 0 rows affected (0.06 sec)

mysql> show create table t1;
+-------+-------------------------------------------------------------------------+
| Table | Create Table                                                            |
+-------+-------------------------------------------------------------------------+
| t1    | CREATE TABLE `t1` (
  `a` decimal(10,6) unsigned zerofill DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 |
+-------+-------------------------------------------------------------------------+
1 row in set (0.00 sec)

```
### 1.1 整数
mysql支持SQL标准的整数类型 INTEGER（或INT）和 SMALLINT。同时，作为扩展，MySQL也支持TINYINT，MEDIUMINT和 BIGINT，下表展示了每种类型的存储空间和数值范围：  

|类型名|存储空间(字节)|有符号最小值|无符号最小值|有符号最大值|无符号最大值|  
|-|-|-|-|-|-|  
|TINYINT|1|-128|0|127|255|  
|SMALLINT|2|-32768|0|32767|65535|  
|MEDIUMINT|3|-8388608|0|8388607|16777215|  
|INT|4|-2147483648|0|2147483647|4294967295|  
|BIGINT|8|-2^63|0|2^63-1|2^64-1|  
  
有编程经验的人对这些数据类型都不陌生。  
我们在定义列的数据类型时，可以指定如下属性：[(M)] [UNSIGNED] [ZEROFILL]。对于整数而言，M表示最大显示宽度，这个值与整数可以存储的值无关。  
SERIAL 是BIGINT UNSIGNED NOT NULL AUTO_INCREMENT UNIQUE的别名。  
在整数列的定义中SERIAL DEFAULT VALUE是NOT NULL AUTO_INCREMENT UNIQUE的别名。  
在UNSIGNED类型的整数值之间使用减法时，除非启用NO_UNSIGNED_SUBTRACTION SQL模式，否则结果是无符号的。  
BOOL, BOOLEAN与TINYINT(1)是等价的，0表示false，非0表示true：  
```sql
mysql> SELECT IF(0, 'true', 'false');
+------------------------+
| IF(0, 'true', 'false') |
+------------------------+
| false                  |
+------------------------+

mysql> SELECT IF(1, 'true', 'false');
+------------------------+
| IF(1, 'true', 'false') |
+------------------------+
| true                   |
+------------------------+

mysql> SELECT IF(2, 'true', 'false');
+------------------------+
| IF(2, 'true', 'false') |
+------------------------+
| true                   |
+------------------------+
```
但是，值TRUE和FALSE仅为1和0的别名，如下所示：  
```sql
mysql> SELECT IF(0 = FALSE, 'true', 'false');
+--------------------------------+
| IF(0 = FALSE, 'true', 'false') |
+--------------------------------+
| true                           |
+--------------------------------+

mysql> SELECT IF(1 = TRUE, 'true', 'false');
+-------------------------------+
| IF(1 = TRUE, 'true', 'false') |
+-------------------------------+
| true                          |
+-------------------------------+

mysql> SELECT IF(2 = TRUE, 'true', 'false');
+-------------------------------+
| IF(2 = TRUE, 'true', 'false') |
+-------------------------------+
| false                         |
+-------------------------------+

mysql> SELECT IF(2 = FALSE, 'true', 'false');
+--------------------------------+
| IF(2 = FALSE, 'true', 'false') |
+--------------------------------+
| false                          |
+--------------------------------+
```
在使用整数时，要注意溢出、类型的隐式转换等等一些问题。  

### 1.2 定点型
DECIMAL和NUMERIC。二者等价。  
定义格式为：DECIMAL[(M[,D])] [UNSIGNED] [ZEROFILL]  
DEC[(M[,D])] [UNSIGNED] [ZEROFILL], NUMERIC[(M[,D])] [UNSIGNED] [ZEROFILL], FIXED[(M[,D])] [UNSIGNED] [ZEROFILL]与上边含义一样。  
M：表示数字的总数，D表示小数点后的位数(小数点和符号不计入M)。最大位数（M）为65。支持的最大小数位数（D）为30。如果省略D，则缺省值为0.如果省略M，则缺省值为10。mysql在使用DECIMAL做基础计算(+,-,*,/)时都使用65位精度。例如：  
```sql
salary DECIMAL(5,2)
```
salary可以存储的值范围从-999.99至999.99。  

### 1.3 浮点型
FLOAT和DOUBLE。  
FLOAT占四个字节，DOUBLE占8个字节。为了获得最大程度的可移植性，最好不使用浮点型，特别是金额类的字段。如果必须使用浮点型，代码应使用FLOAT或 DOUBLE PRECISION，而不制定精度或数字位数。  

### 1.4 bit
BIT类型用于存储bit值，BIT(M)能够存储M位值，M的范围是1--64.  
b'value'表示bit值得字面值。例如：b'111' 和 b'10000000' 表示 7 和 128, 如果为BIT（M）列分配一个小于M位长的值，左侧的bit用0填充。例如，将b'101'的值分配给BIT（6）列实际上与分配b'000101'相同。  

整数或浮点数据类型可以具有附加属性AUTO_INCREMENT。将NULL插入AUTO_INCREMENT列时，该列的值被设置为下一个序列值，通常，这是value + 1，其中value是表中当前列的最大值，AUTO_INCREMENT序列以1开始。  
除非启用了NO_AUTO_VALUE_ON_ZERO模式，否则将0存储到AUTO_INCREMENT列与存储NULL具有相同的效果。  
**插入NULL以生成AUTO_INCREMENT值需要将列声明为NOT NULL，如果列声明为NULL，则插入NULL将存储NULL，将任何其他值插入AUTO_INCREMENT列时，会将列设置为该值并重置序列，以便从插入的值开始产生自增序列。**  
在MySQL 8.0中，AUTO_INCREMENT列不支持负值。  

### 1.5 越界和溢出的处理
当将超过存储范围的值存储到mysql中时，具体的行为依赖于mysql的sql模式。  
 - 如果启用了严格的SQL模式，则MySQL会根据SQL标准拒绝带有错误的超出范围的值，并且插入失败。  
 - 如果没有启用，mysql将对值进行截断存储。  
例如，有一张表：
```sql
CREATE TABLE t1 (i1 TINYINT, i2 TINYINT UNSIGNED);
```
启用严格的SQL模式后，会发生超出范围的错误：  
```sql
mysql> SET sql_mode = 'TRADITIONAL';
mysql> INSERT INTO t1 (i1, i2) VALUES(256, 256);
ERROR 1264 (22003): Out of range value for column 'i1' at row 1
mysql> SELECT * FROM t1;
Empty set (0.00 sec)
```
如果未启用严格的SQL模式，则会发生带有警告的截断：  
```sql
mysql> SET sql_mode = '';
mysql> INSERT INTO t1 (i1, i2) VALUES(256, 256);
mysql> SHOW WARNINGS;
+---------+------+---------------------------------------------+
| Level   | Code | Message                                     |
+---------+------+---------------------------------------------+
| Warning | 1264 | Out of range value for column 'i1' at row 1 |
| Warning | 1264 | Out of range value for column 'i2' at row 1 |
+---------+------+---------------------------------------------+
mysql> SELECT * FROM t1;
+------+------+
| i1   | i2   |
+------+------+
|  127 |  255 |
+------+------+
```
未启用严格SQL模式时， 对于ALTER TABLE, LOAD DATA INFILE, UPDATE, multiple-row INSERT语句引起的溢出，mysql将给出警告。如果启用了严格模式，这些语句将失败，并且部分或全部值未插入或更改，具体取决于表是否为事务表。  

由此我们可以看出，对于数据质量要求很高的业务，还是启用严格的SQL模式，同时使用事务表InnoDB。这可以避免很多意想不到的错误发生。[见服务器SQL模式](https://dev.mysql.com/doc/refman/8.0/en/sql-mode.html).  

上边学习了存储时的溢出，下边学习计算中的溢出。  
数值表达式求值过程中的溢出会导致错误。例如，最大的带符号BIGINT值为9223372036854775807，因此以下表达式会产生错误：  
```sql
mysql> SELECT 9223372036854775807 + 1;
ERROR 1690 (22003): BIGINT value is out of range in '(9223372036854775807 + 1)'
```
要在这种情况下使操作成功，请将值转换为unsigned;  
```sql
mysql> SELECT CAST(9223372036854775807 AS UNSIGNED) + 1;
+-------------------------------------------+
| CAST(9223372036854775807 AS UNSIGNED) + 1 |
+-------------------------------------------+
|                       9223372036854775808 |
+-------------------------------------------+
```
是否发生溢出取决于操作数的范围，因此处理前一个表达式的另一种方法是使用精度算术，因为DECIMAL值的范围大于整数：  
```sql
mysql> SELECT 9223372036854775807.0 + 1;
+---------------------------+
| 9223372036854775807.0 + 1 |
+---------------------------+
|     9223372036854775808.0 |
+---------------------------+
```

两个整数做减法，其中一个是UNSIGNED，则结果为UNSIGNED，如果为负数，则错误发生：  
```sql
mysql> SET sql_mode = '';
Query OK, 0 rows affected (0.00 sec)

mysql> SELECT CAST(0 AS UNSIGNED) - 1;
ERROR 1690 (22003): BIGINT UNSIGNED value is out of range in '(cast(0 as unsigned) - 1)'
```
如果启用了NO_UNSIGNED_SUBTRACTION SQL模式，则结果为负：  
```sql
mysql> SET sql_mode = 'NO_UNSIGNED_SUBTRACTION';
mysql> SELECT CAST(0 AS UNSIGNED) - 1;
+-------------------------+
| CAST(0 AS UNSIGNED) - 1 |
+-------------------------+
|                      -1 |
+-------------------------+
```
如果使用此类操作的结果来更新UNSIGNED整数列,结果将截断为列类型的最大值（-1就是所有的bit都为1，也就是unsigned的最大值）。如果启用NO_UNSIGNED_SUBTRACTION，则为0。如果启用了严格的SQL模式，则会发生错误并且列保持不变。  

### 1.6 自己的心得
在设计存储时，宁可浪费一点空间，也不要因为空间不够用而发生溢出从而产生各种奇怪的问题。有的人会反驳，但是我觉得如果真的是因为空间不够用了，应该审视自己的架构设计是否合理，应该做分库分表，因为存储不够用的时候查询也会很慢了，同时，为了正确性，以及后续的可维护性，多浪费一点存储也是值得的。毕竟大的趋势是存储越来越便宜（毕竟只是多浪费一点存储）。  

数值类型就学习到这里，接下来学习日期和时间类型。  
