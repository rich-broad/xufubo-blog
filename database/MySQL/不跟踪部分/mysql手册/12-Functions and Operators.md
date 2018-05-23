# 函数和操作符
表达式可以在SQL语句的很多部分使用，表达式中可以包含函数调用。例如在ORDER BY、SELECT、DELETE、UPDATE、HAVING等。但是需要注意，默认情况下，函数名称和其后面的括号之间不能有空格。这有助于MySQL解析器区分函数调用和对与函数名称相同的表或列的引用。但是，允许函数参数之间有空格。为了允许函数之间有空格，要配置--sql-mode=IGNORE_SPACE来告诉MySQL服务器在函数名称后面使用空格。单个客户端程序可以在mysql_real_connect()的时候使用CLIENT_IGNORE_SPACE 选项来获得此行为。在任一种情况下，所有函数名都将成为保留字。  
本节学习mysql的函数和操作符。具体该怎么学习呢？挨个学习每个函数没什么意义，主要是从宏观上把我mysql的函数和操作符即可，至于细节，则直接查看[mysql官方手册](https://dev.mysql.com/doc/refman/8.0/en/functions.html)。  

---
## 1、函数和操作符指引
见[12.1 Function and Operator Reference](https://dev.mysql.com/doc/refman/8.0/en/func-op-summary-ref.html)。  
  
---
## 2、表达式求值中的类型转换



## 3、操作符
mysql支持的操作符如下表：  
  
|名字|描述|  
|-|-|  
|AND, &&|逻辑与|  
|=|赋值（作为SET 语句的一部分 ，或作为语句中的SET子句的 一部分UPDATE）|  
|:=|赋值|  
|BETWEEN ... AND ...|检查一个值是否在一个范围内|  
|BINARY|将字符串转换为二进制字符串|  
|&|按位与|  
|~|按位取反|  
|\||按位或|  
|^|按位XOR|  
|CASE|case操作符|  
|DIV|整数除法|  
|/|除以操作符|  
|=|等于|  
|<=>|用于安全的比较NULL|  
|>|大于|  
|>=|大于等于|  
|IS|用于测试某个值是不是boolean值，例如：1 IS TRUE，0 IS FALSE|  
|IS NOT|用于测试某个值是不是boolean值|  
|IS NOT NULL|NOT NULL值测试|  
|IS NULL|NULL值测试|  
|->|相当于JSON_EXTRACT（），返回json数据中的某个值|  
|->>|相当于JSON_UNQUOTE(JSON_EXTRACT())，也是返回json的某个值|  
|<<|左移|  
|<|小于|  
|<=|小于等于|  
|LIKE|简单的模式匹配|  
|-|减|  
|%, MOD|求余|  
|NOT, !|非|  
|NOT BETWEEN ... AND ...|检查一个值是否不在一个值范围内|  
|!=, <>|不等于|  
|NOT LIKE|不像，与LIKE相反|  
|NOT REGEXP|字符串是否不匹配正则表达式|  
|\|\|, OR|逻辑或|  
|+|加|  
|REGEXP|字符串是否匹配正则表达式|  
|>>|右移|  
|RLIKE|字符串是否匹配正则表达式|  
|SOUNDS LIKE|expr1 SOUNDS LIKE expr2等价于SOUNDEX(expr1) = SOUNDEX(expr2)|  
|*|乘法|  
|-|符号 取反|  
|XOR|逻辑XOR|  
  
以上表格是mysql的操作符，接下来对其中我们不熟悉的操作符，或者mysql特有的操作符进行说明。如果有没有列出的操作符，请查看mysql手册。  

### 3.1 操作符优先级
运算符优先级显示在以下列表中，从最高优先级到最低优先级。显示在一行上的运算符具有相同的优先级。  
```
INTERVAL
BINARY, COLLATE
!
- (符号取反), ~ (按位取反)
^
*, /, DIV, %, MOD
-, +
<<, >>
&
|
= (比较运算符), <=>, >=, >, <=, <, <>, !=, IS, LIKE, REGEXP, IN
BETWEEN, CASE, WHEN, THEN, ELSE
NOT
AND, &&
XOR
OR, ||
= (赋值), :=
```
一些运算符的含义取决于SQL模式：  
 - 默认情况下，|| 是逻辑OR运算符。在 PIPES_AS_CONCAT启用的情况下， ||是字符串连接，优先级位于^和一元运算符之间。  
 - 默认情况下，!优先级高于NOT。在HIGH_NOT_PRECEDENCE启用的情况下，!和NOT具有相同的优先级。  

### 3.2 比较函数和操作符
这里主要列出mysql特有的操作符和函数。  
 - <=>: 该运算符与=运算符执行相等比较，但如果两个操作数均为NULL，则返回1而不是NULL;如果一个操作数为NULL，则返回0而不是NULL。  
```sql
SELECT 1 <=> 1, NULL <=> NULL, 1 <=> NULL;
    -> 1, 1, 0
SELECT 1 = 1, NULL = NULL, 1 = NULL;
    -> 1, NULL, NULL
```
 - IS boolean_value: 根据布尔值测试值，其中boolean_value可以为TRUE，FALSE或UNKNOWN。    
 ```sql
SELECT 1 IS TRUE, 0 IS FALSE, NULL IS UNKNOWN;
    -> 1, 1, 1
 ```
 - IS NOT boolean_value: 根据布尔值测试值，其中boolean_value可以为TRUE，FALSE或UNKNOWN。  
 ```sql
SELECT 1 IS NOT UNKNOWN, 0 IS NOT UNKNOWN, NULL IS NOT UNKNOWN;
    -> 1, 1, 0
 ```
 - COALESCE(value,...): 返回列表中的第一个非NULL值，如果没有非NULL值，则返回NULL。  
 ```sql
SELECT COALESCE(NULL,1);
    -> 1
SELECT COALESCE(NULL,NULL,NULL);
    -> NULL
 ```
 - GREATEST(value1,value2,...): 返回多个参数的最大（最大值）参数，比较规则与LEAST()相同。    
 ```sql
 SELECT GREATEST(2,0);
    -> 2
 SELECT GREATEST(34.0,3.0,5.0,767.0);
    -> 767.0
 SELECT GREATEST('B','A','C');
    -> 'C'
 ```
 只要任一参数为NULL，GREATEST（）将返回NULL。  
 - ISNULL(expr)：If expr is NULL, ISNULL() returns 1, otherwise it returns 0。与IS NULL作用相同。  
 ```sql
 SELECT ISNULL(1+1);
    -> 0
 SELECT ISNULL(1/0);
    -> 1
 ```
 - INTERVAL(N,N1,N2,N3,...)：如果N<N1，则返回0;如果N<N2，则返回1,N<N3,则返回2，以此类推;如果N为NULL，则返回-1。所有参数都被视为整数。要求N1 <N2 <N3 <... <Nn，此函数才能正常工作。这是因为使用了二分搜索。  
 ```sql
 SELECT INTERVAL(23, 1, 15, 17, 30, 44, 200);
    -> 3
 SELECT INTERVAL(10, 1, 10, 100, 1000);
    -> 2
 SELECT INTERVAL(22, 23, 30, 44, 200);
    -> 0
 ```
 - LEAST(value1,value2,...)：使用两个或更多参数，返回最小（最小值）参数。使用以下规则比较参数：  
 > - 如果任一参数为NULL，则结果为NULL。不需要比较。  
 > - 如果所有参数都是整数值，则将它们作为整数进行比较。  
 > - 如果至少有一个参数是双精度（double），则它们将作为双精度值(double)进行比较。否则，如果至少有一个参数是DECIMAL值，则将它们作为DECIMAL值进行比较。  
 > - 如果参数包含数字和字符串的混合，则将其作为数字进行比较。  
 > - 如果任一参数是非二进制（字符）字符串，则将参数作为非二进制字符串进行比较。  
 > - 在所有其他情况下，参数将作为二进制字符串进行比较。  
 ```sql
 SELECT LEAST(2,0);
        -> 0
 SELECT LEAST(34.0,3.0,5.0,767.0);
        -> 3.0
 SELECT LEAST('B','A','C');
        -> 'A'
 ```

### 3.3 逻辑操作符
AND, &&；NOT, !；||, OR；XOR都很熟悉，不再详述。  

### 3.4 赋值操作符

