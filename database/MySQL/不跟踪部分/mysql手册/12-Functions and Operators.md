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
|~|按位反转|  
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
