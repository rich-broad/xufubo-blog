# tutorial
本章主要介绍通过mysql这个程序链接到MySQL服务器，并对服务器进行使用。mysql程序是一个终端交互程序，用于链接到MySQL服务器，提交查询并查看结果。mysql也可在批处理模式下使用：先将查询放在一个文件中，然后告诉 mysql执行文件的内容。本章介绍这两种使用方式。本章只是一个简单的入门教程。要查看mysql提供的列表，请使用mysql –help。  
本章主要是对mysql简单的使用。首先有个感性的认识。  

---
## 1、连接和断开服务器
连接时通常需要指定要链接的主机、端口、用户名、密码。  
```shell
shell> mysql -h host -u user –P 3306 -p 
Enter password: ********
```
如果客户端与服务器在一台机器上，可以：  
```shell
shell> mysql -u user -p
```
键入QUIT来退出mysql程序。   

---
## 2、输入查询
MySQL可以包含多个数据库，如上与服务器建立连接后并未选择任何数据库。此时依然可以与mysqld进行交互。例如像MySQL服务器询问版本和当前日期：  
```sql
SELECT VERSION(), CURRENT_DATE;
select version(), current_date;
SeLeCt vErSiOn(), current_DATE;
```
如上可见，对于关键字，MySQL不区分大小写。  
同一个查询也可以位于多行：  
```sql
mysql> SELECT
     -> USER()
     -> ,     
     -> CURRENT_DATE; 
+---------------+--------------+ 
| USER()        | CURRENT_DATE |
+---------------+--------------+ 
| jon@localhost | 2010-08-06   | 
+---------------+--------------+
```
此例中，在第一行提示符为mysql>，后续的为->,表示mysql还未看到一个完整的语句。正在等待其余的输入。如果不想执行正在输入的查询，可以输入\c，如下：  
```sql
mysql> SELECT
-> USER()     
-> \c 
mysql>
```
输入\c之后，提示mysql>，表示mysql准备好接受新的查询。  
下边是mysql程序会出现的提示符即对应的含义，了解这些是有用的。  
  
|提示|含义|
|-|-|
|mysql>|准备新的查询|
|->|	等待多行查询的下一行|
|'>|	等待下一行，等待完成一个以单引号（'）开始的字符串|
|">|	等待下一行，等待完成一个以双引号（"）开头的字符串|
|`>|	等待下一行，等待完成一个以反引号（`）开头的标识符|
|/*>|	等待下一行，等待完成一个以/*开始的注释|  
  
例如你输入如下查询：  
```sql
mysql> SELECT * FROM my_table WHERE name = 'Smith AND age < 30;
     '>
```
这时你会感到奇怪，我的查询怎么不执行？这时要注意mysql程序的提示符，mysql客户端程序认为你的字符串缺少单引号。但是你却知道，实际上你是输入错误，那么接下来可以如下输入，从而取消本查询：  
```sql
mysql> SELECT * FROM my_table WHERE name = 'Smith AND age < 30;
'> '\c 
mysql>
```
注意要输入'\c，只输入\c不管用哈。  

---
## 3、创建和使用数据库
### 3.1 创建和选择数据库
先看看当前服务器上存在哪些数据库：  
```sql
SHOW DATABASES;
```
该语句不显示你没有权限的数据库。  
选择某个数据库：  
```sql
USE test
```
将某个数据库的所有权限赋予一个用户：  
```sql
GRANT ALL ON menagerie.* TO 'your_mysql_name'@'your_client_host';
```
your_mysql_name是分配给你的MySQL用户名，your_client_host是要连接到服务器的主机。  
创建一个数据库：  
```sql
CREATE DATABASE menagerie;
```
创建之后，并不会默认选择该数据库，你必须手动执行USE语句。数据库只需要创建一次，但是每次开始mysql 会话时都必须选择它。可以通过发出USE语句来执行此操作 。或者，可以在命令行调用mysql时选择数据库。只需在连接参数最后指定其名称。例如：  
```shell
mysql -h host -u user -p menagerie
Enter password: ********
```
注意：menagerie为数据库，并不是密码，如果要在p之后跟随密码，p和密码之间不可以有空格。但是，不建议将密码放在命令行上，因为这样做会使其被登录在您的计算机上的其他用户所窥探。
您可以随时查看当前使用哪个数据库。 SELECT DATABASE()  

### 3.2 创建表
先看看当前数据库中有哪些表：  
```sql
SHOW TABLES; 
Empty set (0.00 sec)
```
创建一张宠物表：  
```sql
CREATE TABLE pet (name VARCHAR(20), owner VARCHAR(20),
     species VARCHAR(20), sex CHAR(1), birth DATE, death DATE);
```
该表记录宠物的最为基本的信息：名称，所有者，物种，性别，出生和死亡。  

### 3.3 将数据装载到表中
```sql
LOAD DATA LOCAL INFILE '/path/pet.txt' INTO TABLE pet;
```
如果在Windows上使用\r\n用作行终止符的编辑器创建该文件 ，则应改用以下语句：  
```sql
LOAD DATA LOCAL INFILE '/path/pet.txt' INTO TABLE pet LINES TERMINATED BY '\r\n';
```
如果需要，可以在LOAD DATA语句中明确指定列值之间的分隔符和行尾标记，默认值为制表符和换行符。  
也可以插入一条记录：  
```sql
INSERT INTO pet VALUES ('Puffball','Diane','hamster','f','1999-03-30',NULL);
```

### 3.4 从表中查询信息
#### 3.4.1 查询所有数据：
从表 中检索所有内容的最简单的形式：  
```sql
SELECT * FROM pet;
+----------+--------+---------+------+------------+------------+ 
| name     | owner  | species | sex  | birth      | death      | 
+----------+--------+---------+------+------------+------------+ 
| Fluffy   | Harold | cat     | f    | 1993-02-04 | NULL       | 
| Claws    | Gwen   | cat     | m    | 1994-03-17 | NULL       | 
| Buffy    | Harold | dog     | f    | 1989-05-13 | NULL       | 
| Fang     | Benny  | dog     | m    | 1990-08-27 | NULL       | 
| Bowser   | Diane  | dog     | m    | 1979-08-31 | 1995-07-29 | 
| Chirpy   | Gwen   | bird    | f    | 1998-09-11 | NULL       | 
| Whistler | Gwen   | bird    | NULL | 1997-12-09 | NULL       | 
| Slim     | Benny  | snake   | m    | 1996-04-29 | NULL       | 
| Puffball | Diane  | hamster | f    | 1999-03-30 | NULL       | 
+----------+--------+---------+------+------------+------------+
```
如果你发现表格中某些数据不正确，可以使用update语句：  
```sql
UPDATE pet SET birth = '1989-08-31' WHERE name = 'Bowser';
```
#### 3.4.2 选择特定的行
```sql
SELECT * FROM pet WHERE name = 'Bowser';
SELECT * FROM pet WHERE birth >= '1998-1-1';
SELECT * FROM pet WHERE species = 'dog' AND sex = 'f';
SELECT * FROM pet WHERE species = 'snake' OR species = 'bird';
```
AND和OR很容易混乱，尽管***AND优先级高于OR***。但是在使用时最好明确用括号包起来：  
```sql
SELECT * FROM pet WHERE (species = 'cat' AND sex = 'm') OR (species = 'dog' AND sex = 'f');
```

#### 3.4.3 选择特定的列
```sql
SELECT name, birth FROM pet;
SELECT owner FROM pet;
SELECT DISTINCT owner FROM pet;
SELECT name, species, birth FROM pet WHERE species = 'dog' OR species = 'cat';
```

#### 3.4.4 排序行
按日期排序：  
```sql
SELECT name, birth FROM pet ORDER BY birth;
```
默认为升序，降序排列需要明确指定：  
```sql
SELECT name, birth FROM pet ORDER BY birth DESC;
```
可以按多个列排序，例如：按照动物类型升序（默认）排列，按照出生日期降序排列：  
```sql
SELECT name, species, birth FROM pet ORDER BY species, birth DESC;
+----------+---------+------------+ 
| name     | species | birth      | 
+----------+---------+------------+ 
| Chirpy   | bird    | 1998-09-11 | 
| Whistler | bird    | 1997-12-09 | 
| Claws    | cat     | 1994-03-17 | 
| Fluffy   | cat     | 1993-02-04 | 
| Fang     | dog     | 1990-08-27 | 
| Bowser   | dog     | 1989-08-31 | 
| Buffy    | dog     | 1989-05-13 | 
| Puffball | hamster | 1999-03-30 | 
| Slim     | snake   | 1996-04-29 | 
+----------+---------+------------+
```
***DESC关键字仅适用于它之前的列名（birth）; 不影响species列的排序顺序。***  

#### 3.4.5 日期计算
MySQL提供了几种可用于执行日期计算的功能，例如计算年龄或提取部分日期。要确定你宠物的年龄，请使用 TIMESTAMPDIFF()函数。它依据你希望的表达单位和两个日期的差异求的计算结果。以下查询显示每只宠物的出生日期，当前日期和年龄。使用 别名（age）来使最终输出列标签更有意义。  
```sql
SELECT name, birth, CURDATE(),
TIMESTAMPDIFF(YEAR,birth,CURDATE()) AS age
FROM pet; 
+----------+------------+------------+------+ 
| name     | birth      | CURDATE()  | age  | 
+----------+------------+------------+------+ 
| Fluffy   | 1993-02-04 | 2003-08-19 |   10 | 
| Claws    | 1994-03-17 | 2003-08-19 |    9 | 
| Buffy    | 1989-05-13 | 2003-08-19 |   14 | 
| Fang     | 1990-08-27 | 2003-08-19 |   12 | 
| Bowser   | 1989-08-31 | 2003-08-19 |   13 | 
| Chirpy   | 1998-09-11 | 2003-08-19 |    4 | 
| Whistler | 1997-12-09 | 2003-08-19 |    5 | 
| Slim     | 1996-04-29 | 2003-08-19 |    7 | 
| Puffball | 1999-03-30 | 2003-08-19 |    4 | 
+----------+------------+------------+------+
```
也可以将结果以某种顺序输出:  
```sql
SELECT name, birth, CURDATE(),
TIMESTAMPDIFF(YEAR,birth,CURDATE()) AS age     
FROM pet ORDER BY name; 
+----------+------------+------------+------+ 
| name     | birth      | CURDATE()  | age  | 
+----------+------------+------------+------+ 
| Bowser   | 1989-08-31 | 2003-08-19 |   13 | 
| Buffy    | 1989-05-13 | 2003-08-19 |   14 | 
| Chirpy   | 1998-09-11 | 2003-08-19 |    4 | 
| Claws    | 1994-03-17 | 2003-08-19 |    9 | 
| Fang     | 1990-08-27 | 2003-08-19 |   12 | 
| Fluffy   | 1993-02-04 | 2003-08-19 |   10 | 
| Puffball | 1999-03-30 | 2003-08-19 |    4 | 
| Slim     | 1996-04-29 | 2003-08-19 |    7 | 
| Whistler | 1997-12-09 | 2003-08-19 |    5 | 
+----------+------------+------------+------+
```
如果你想知道下个月有哪些动物过生日呢？对于这种类型的计算，年和日是无关紧要的; 你只需要提取birth列的月份部分 。MySQL提供了用于提取日期的部分，如一些函数 YEAR()， MONTH()和 DAYOFMONTH()。 MONTH()是现在需要的函数，看看它如何工作：  
```sql
SELECT name, birth, MONTH(birth) FROM pet; 
+----------+------------+--------------+ 
| name     | birth      | MONTH(birth) | 
+----------+------------+--------------+ 
| Fluffy   | 1993-02-04 |            2 | 
| Claws    | 1994-03-17 |            3 | 
| Buffy    | 1989-05-13 |            5 | 
| Fang     | 1990-08-27 |            8 | 
| Bowser   | 1989-08-31 |            8 | 
| Chirpy   | 1998-09-11 |            9 | 
| Whistler | 1997-12-09 |           12 | 
| Slim     | 1996-04-29 |            4 | 
| Puffball | 1999-03-30 |            3 | 
+----------+------------+--------------+
```
假设当月是4月。找出在五月（月5）里出生的动物：  
```sql
SELECT name, birth FROM pet WHERE MONTH(birth) = 5;

```
如果当月为12月，那么总不能写成13月吧。你可以这么写：  
```sql
SELECT name, birth FROM pet
     WHERE MONTH(birth) = MONTH(DATE_ADD(CURDATE(),INTERVAL 1 MONTH));
```
DATE_ADD()函数可以参考MySQL手册或者网上查查其用法。或者：  
```sql
SELECT name, birth FROM pet WHERE MONTH(birth) = MOD(MONTH(CURDATE()), 12) + 1;
```

#### 3.4.6 使用NULL值
NULL在概念上是一个未知的值。要测试NULL，使用 IS NULL和IS NOT NULL运算符，如下所示：  
```sql
SELECT 1 IS NULL, 1 IS NOT NULL; 
+-----------+---------------+ 
| 1 IS NULL | 1 IS NOT NULL | 
+-----------+---------------+ 
|         0 |             1 | 
+-----------+---------------+
```
不能使用比较运算符，如：=、<、>、<>等。因为一个不知道的值与其他的值比结果还是不知道即：NULL，一次作比较无任何意义。GROUP BY中两个NULL值被认为是相等的。当执行ORDER BY时，如果执行ORDER BY ... ASC，则首先显示NULL值，如果执行ORDER BY ... DESC，则最后显示NULL值。

#### 3.4.2 模式匹配
MySQL提供标准的SQL模式匹配以及基于扩展的正则表达式的模式匹配，类似于Unix实用程序（如vi，grep和sed）所使用的正则表达式。SQL模式匹配中：_匹配任意单个字符，%匹配任意多个字符（包括0个）。在MySQL中，默认情况下，SQL模式不区分大小写。这里显示了一些示例。当您使用SQL模式时，不要使用=或<>。应该使用LIKE或NOT LIKE比较运算符。  
***在MySQL中，默认情况下，SQL模式不区分大小写这一点请看下边的例子***  
找到名字以b开头的数据：  
```sql
SELECT * FROM pet WHERE name LIKE 'b%';
+--------+--------+---------+------+------------+------------+
| name   | owner  | species | sex  | birth      | death      |
+--------+--------+---------+------+------------+------------+
| Buffy  | Harold | dog     | f    | 1989-05-13 | NULL       |
| Bowser | Diane  | dog     | m    | 1989-08-31 | 1995-07-29 |
+--------+--------+---------+------+------------+------------+
-- 注意，Buffy、Bowser均为首字母大写，而模式中为小写b，即：'在MySQL中，默认情况下，SQL模式不区分大小写'
```
找到名字以fy结尾的数据：  
```sql
mysql> SELECT * FROM pet WHERE name LIKE '%fy';
```
发现名字中包含w的数据：  
```sql
mysql> SELECT * FROM pet WHERE name LIKE '%w%';
```
发现名字中恰好包含5个字符的数据，使用五个_：  
```sql
mysql> SELECT * FROM pet WHERE name LIKE '_____';
```
MySQL提供的其他类型的模式匹配使用扩展的正则表达式。当你测试这种类型的模式匹配时，请使用 REGEXP和NOT REGEXP操作符（或者 RLIKE和 NOT RLIKE，它们是同义词）。以下描述了扩展的正则表达式的一些特征：  
- . 匹配任何单个字符。
- 字符类[...]会匹配括号内的任何字符。例如， [abc]匹配a， b或c。要命名一个字符范围，请使用破折号。[a-z] 匹配任何字母，而[0-9] 匹配任何数字。
- \*匹配其前面的事物的零个或多个实例。例如，x\* 匹配任何数量的x字符， [0-9]\*匹配任意数量的数字，.\*匹配任何数量的任何东西。
- REGEXP的时候，当模式匹配被测值中的任何位置时匹配成功。（这与LIKE模式匹配不同 ，只有当模式匹配整个值时才能成功）
- 为了表达必须与正在测试的值的开头或结尾相匹配，需要在模式开始处使用^或在模式结尾处使用$
#### 3.4.2 选择特定的行

#### 3.4.2 选择特定的行

---
## 4、获取数据库和表信息
如果你忘记了当前数据库、数据库名、表名等等该怎么办？  
展示服务器管理的各个数据库名字：  
```sql
SHOW DATABASE;
```
查看当前选择的数据库：  
```sql
SELECT DATABASE();
```
查看数据库中有那些表：  
```sql
SHOW TABLES;
```
了解表的结构：  
```sql
DESCRIBE pet;
```
也可以使用DESC作为简写，还可以使用SHOW CREATE TABLE tab_name来看某个表的定义。   

---
## 5、在批处理模式下使用mysql