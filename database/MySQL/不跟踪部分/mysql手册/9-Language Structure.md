# 语言结构
我们在使用一门语言、一种工具的时候，明白它的语言结构是很重要的，以避免某些奇怪的错误发生。本章主要学习在使用MySQL中编写SQL语句时要用到的以下元素的规则：  
 - 字面值，如字符串和数字  
 - 标识符，如数据库，表和列名等  
 - 关键字和保留字  
 - 用户定义变量和系统变量  
 - 注释  

---

## 1、字面值
本节描述了如何在MySQL中写字面值。包括字符串、数值、十六进制值、位值、布尔值和NULL。本节还包含MySQL在处理这些基本类型时的各种细微差别。  
### 1.1 字符串字面值
字符串是用单引号或者双引号引起来的字符序列，彼此相邻的字符串链接成单个字符串，例如下列是等价的。  
```s
'a string'和'a' ' ' 'string'是等价的
```
如果启用了SQL模式ANSI_QUOTES，则只能在单引号内引用字符串，因为双引号内引用的字符串将被解释为标识符。  
字符串分为二进制字符串和非二进制字符串，二进制字符串的单元为字节，非二进制字符串的单元为字符（有些编码，一个字符用多个字节表示，因此不一定为一个字节），二进制字符串排序时比较的为字符串中各个字节的数字值，非二进制字符串比较的为各个字符对应的数值码值（可见，对于非二进制字符串，不同字符集下，同一个字符串底层存储可以不同，同一个码值也可以不同，因此排序大小也就不同了）。二进制字符串可以理解为字节流。字符串文字可以包含一个可选的字符集引导器和COLLATE子句，以将其指定为特定的字符集和排序规则:  
[_charset_name]'string' [COLLATE collation_name]  
例如：  
```sql
SELECT _latin1'string'; 
SELECT _binary'string'; 
SELECT _utf8'string' COLLATE utf8_danish_ci;
```
时候MySQL就将字符串按指定的字符集编码，例子如下：  
```sql
mysql> select _latin1'许富博';
+--------------------+
| è®¸å¯Œåš          |
+--------------------+
| è®¸å¯Œåš          |
+--------------------+
1 row in set (0.00 sec)

mysql> select _gbk'许富博';
+--------------+
| 璁稿瘜鍗     |
+--------------+
| 璁稿瘜鍗     |
+--------------+
1 row in set, 1 warning (0.00 sec)
```  
为什么作图第一个为乱码呢？因为MySQL将字符按照latin1编码，而我的屏幕是UTF8编码，这样，第一个就是乱码了。再比如，你用gbk，依然是乱码.  
你也可以使用N'literal'(or n'literal') 创建国家字符集的字符串。例如，如果国家字符集为utf8，则以下等价：  
```sql
SELECT N'some text';
SELECT n'some text';
SELECT _utf8'some text';
```
有关这些字符串语法形式的信息,见[The National Character Set](https://dev.mysql.com/doc/refman/8.0/en/charset-national.html)，[Character Set Introducers](https://dev.mysql.com/doc/refman/8.0/en/charset-introducer.html)。  
在字符串中，某些序列具有特殊的含义，除非启用了SQL模式NO_BACKSLASH_ESCAPES。这些序列都以反斜杠（\）开头，称为转义字符。MySQL识别下边的转义字符。对不在表中的其他转义序列，会忽略反斜线，像没有被转义一样。例如：\x表示x。转义序列区分大小写。例如， \b被解释为退格，但 \B被解释为B。MySQL依据系统变量 character_set_connection指定的字符集对转义进行处理。即使在前面指定了不同字符集导引器的字符串也是如此。  
