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
我们在定义列的数据类型时，为类型[(M)] [UNSIGNED] [ZEROFILL]。对于整数而言，M表示最大显示宽度，这个值与整数可以存储的值无关。  
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


### 1.4 bit
