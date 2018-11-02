# 4 mysql服务器日志
对于后台开发而言，日志是非常重要的东西，学习mysql也是一样，掌握好它的日志也是非常关键的技能，本节开始学习它的日志。  
mysql 服务器层（而非存储引擎）有如下几个日志：  
  
|日志类型|写入日志的信息|  
|错误日志|启动，运行或停止mysqld时遇到的问题|  
|一般查询日志|客户端连接信息和从客户端收到的语句信息|  
|二进制日志|更改数据的语句（用于复制）|  
|中继日志|从机上复制主机上对数据的更改日志(用于复制)|  
|查询日志慢|sql执行时间超过long_query_time秒的日志，经常用于监控|  
|DDL日志（元数据日志）|由DDL语句执行的元数据操作|  

默认情况下，除Windows上的错误日志外，不启用任何日志（DDL日志始终在需要时创建，并且没有用户可配置的选项）。  

默认情况下，mysql服务器将日志写入数据目录的日志文件中。你可以通过刷新日志来强制服务器关闭并重新打开日志文件（或在某些情况下切换到新的日志文件）。刷新日志的语句是：FLUSH LOGS。此外，当二进制日志的大小达到max_binlog_size系统变量的值时，将刷新二进制日志 。  

你可以在服务器运行时控制常规查询日志和慢查询日志。也可以启用或者禁用日志，还可以更改日志文件名。你也可以控制服务器将一般查询日志和慢查询日志写入日志文件还是表，还是二者都写入。后边会学习怎么做。  

中继日志仅用于从机的复制。中继日志的详细信息，我们在复制一章里会介绍。  

## 4.1 选择常规查询日志和慢查询日志输出目的地
这两种日志记录了客户端在服务器上的活动，如果开启了这两种日志，那么你可以配置mysql服务器将这些日志写入哪里，可以写到日志文件中，也可以写到系统数据库mysql的general_log和 slow_log表中，也可以选择两者都写入。  

### 服务器启动时的日志控制
log_output系统变量指定日志输出目的地。设置此变量本身不会启用日志; 必须单独启用它们。  
 - 如果log_output 在启动时未设置，则日志将写入文件
 - 如果log_output 在启动时设置了，则其值是一个由逗号分隔的单词列表，这些值可以是：TABLE（记录到表）， FILE（记录到文件）或 NONE（不记录到表或文件）。NONE如果存在，则优先于任何其他说明符。  
 - **general_log** 系统变量用于控制常规查询日志是否开启。服务器启动时，可以设置 general_log=0/1
 - general_log_file 用于设置常规查询日志的文件名。  
 - **slow_query_log** 系统变量用于控制慢查询日志是否开启。服务器启动时，可以设置 slow_query_log=0/1
 - slow_query_log_file 用于设置慢查询日志的文件名。  

例如：  
 - 要将常规查询日志条目写入日志表和日志文件，请使用--log_output = TABLE，FILE选择日志目标和--general_log以启用常规查询日志。  

### 运行时的日志控制
与日志表和文件关联的系统变量使得我们可以在运行时控制日志记录：  
 - log_output变量指示当前的日志记录目标。可以在运行时修改它以更改目标。  
 - general_log和slow_query_log变量指示是启用（ON）还是禁用（OFF）常规查询日志和慢速查询日志。可以在运行时设置这些变量以控制是否启用日志。  
 - general_log_file和slow_query_log_file变量指示常规查询日志和慢查询日志文件的名称。可以在服务器启动时或在运行时设置这些变量以更改日志文件的名称。  
 - 要禁用或启用当前会话的常规查询日志，请将会话变量sql_log_off设置为ON或OFF。 （这假定已启用了常规查询日志本身。）  

### 日指标的优点
使用表进行日志输出具有以下优点：  
 - 日志条目具有标准格式。要显示日志表的当前结构，请使用以下语句：  
 ```sql
SHOW CREATE TABLE mysql.general_log;
SHOW CREATE TABLE mysql.slow_log;
 ```
 - 可以通过SQL语句访问日志内容。例如可以选择与特定的客户端相关联的日志。  
 - 如果客户端具有相应的日志表权限，可以通过任何可以连接到服务器并发出查询的客户端远程访问日志，这就不需要登录到机器上并直接访问文件系统了。  

日志表实现具有以下特征：  
 - 通常，日志表的主要目的是为用户提供一个接口，以观察服务器的运行时执行，而不是干扰其运行时执行。  
 - CREATE TABLE，ALTER TABLE和DROP TABLE是日志表上的有效操作。但是对于ALTER TABLE和DROP TABLE，不能对日志表使用，必须禁用，后续会描述原因。  
 - 默认情况下，日志表使用CSV存储引擎，可以更改日志表以使用 MyISAM存储引擎。但是不能用 ALTER TABLE更改正在使用的日志表，更改之前必须先禁用日志。除了CSV或MyISAM之外，没有任何引擎对于日志表是合法的。  
 - 为了更改（或删除）日志表，需要禁用日志记录，你可以使用以下策略。以下示例改变常规查询日志。慢查询日志的过程与此类似。  
 ```sql
SET @old_log_state = @@global.general_log;
SET GLOBAL general_log = 'OFF';
ALTER TABLE mysql.general_log ENGINE = MyISAM;
SET GLOBAL general_log = @old_log_state;
 ```
 - TRUNCATE TABLE是日志表上的有效操作.  
 - RENAME TABLE是日志表上的有效操作。您可以使用以下策略以原子方式重命名日志表:  
 ```sql
USE mysql;
DROP TABLE IF EXISTS general_log2;
CREATE TABLE general_log2 LIKE general_log;
RENAME TABLE general_log TO general_log_backup, general_log2 TO general_log;
 ```
 - CHECK TABLE是日志表上的有效操作。  
 - LOCK TABLES不能用于日志表。  
 - INSERT，DELETE和UPDATE不能用于日志表。这些操作仅允许在服务器本身内部进行。  
 - FLUSH TABLES WITH READ LOCK和read_only系统变量的状态对日志表没有影响。服务器始终可以写入日志表。  
 - 写入日志表的条目不会写入二进制日志，因此不会复制到从属服务器。  
 - 要刷新日志表或日志文件，请分别使用FLUSH TABLES或FLUSH LOGS。 
 - 不允许对日志表进行分区。  
 - mysqldump 不转储日志表。  

## 4.2 错误日志
错误日志主要记录服务器本身的错误信息，例如：启动失败，关闭失败，异常退出，需要进行表修复等，这些错误消息都会写入错误日志。  

### 4.2.1 错误日志组件配置




