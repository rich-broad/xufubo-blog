# SQL Statement Syntax
这里主要学习mysql的SQL语句的语法。SQL语句包含如下几类：
 - 数据定义语句
 - 数据操纵语句
 - 事务和锁语句
 - 复制相关语句
 - 预处理SQL语句
 - 复合语句
 - 数据库管理语句
 - 其他实用语句
接下来一一学习

## 1、数据定义语句


## 2、数据操纵语句

## 3、事务和锁语句
### 3.1 START TRANSACTION, COMMIT, 和 ROLLBACK的语法
```SQL
START TRANSACTION
    [transaction_characteristic [, transaction_characteristic] ...]

transaction_characteristic:
    WITH CONSISTENT SNAPSHOT
  | READ WRITE
  | READ ONLY

BEGIN [WORK]
COMMIT [WORK] [AND [NO] CHAIN] [[NO] RELEASE]
ROLLBACK [WORK] [AND [NO] CHAIN] [[NO] RELEASE]
SET autocommit = {0 | 1}
```
以上这些语句提供了对事务的控制：  
 - START TRANSACTION或BEGIN开始新的事务。
 - COMMIT 提交当前事务，使其更改持久化。
 - ROLLBACK 回滚当前事务，取消其更改。
 - SET autocommit 禁用或启用**当前会话**的默认自动提交模式。
默认情况下，MySQL在启用自动提交模式的情况下运行 。这意味着只要执行更新（修改）表的语句，MySQL就会将更新存储在磁盘上以使其永久保存。更改无法回滚。  
要为单个sql语句**隐式**禁用自动提交，请使用以下START TRANSACTION 语句：  
```sql
START TRANSACTION;
SELECT @A:=SUM(salary) FROM table1 WHERE type=1;
UPDATE table2 SET summary=@A WHERE type=1;
COMMIT;
```
mysql看到START TRANSACTION，将会禁用自动提交，直到用COMMIT或者ROLLBACK结束事务，结束之后，自动提交模式将恢复到之前的状态。  
START TRANSACTION语句允许使用一些控制事务的修饰符，多个修饰符之间用逗号隔开。  
- WITH CONSISTENT SNAPSHOT: 开始[一致性读取(consistent read)](https://dev.mysql.com/doc/refman/8.0/en/glossary.html#glos_consistent_read)，该修饰仅仅适用于InnoDB存储引擎。效果与START TRANSACTION后跟SELECT效果一样。该标识不改变当前的事务隔离级别。因此只有存储引擎当前的隔离级别允许一致性读取时它才能够提供一致性快照。唯一允许一致性读取的隔离级别是：REPEATABLE READ。对于所有其他隔离级别，WITH CONSISTENT SNAPSHOT标识将被忽略，WITH CONSISTENT SNAPSHOT子句被忽略时mysql会生成警告。
- READ WRITE和READ ONLY修饰符设置事务访问模式。他们允许或禁止更改事务中使用的表格。READ ONLY限制可防止当前事务修改或锁定对其他事务可见的事务性和非事务性表。不过该事务仍然可以修改或锁定临时表。  
当已知事务为只读时，MySQL将为InnoDB表上的查询启用额外的优化。指定READ ONLY标识可确保在只读无法自动确定的情况下应用这些额外的优化。  
如果未指定访问模式，则应用默认模式。除非默认值已更改，否则它是可读写的。不允许在同一个语句中同时指定READ WRITE和READ ONLY。  
在只读模式下，仍然可以使用DML语句更改使用TEMPORARY关键字创建的表。正如永久表（与临时表相对）一样，不允许使用DDL语句进行更改。  
其他有关事务访问模式的信息下一节详述。  
如果启用read_only系统变量，则使用START TRANSACTION READ WRITE显式启动事务需要CONNECTION_ADMIN或SUPER权限。  
*重要提示：用于编写MySQL客户端应用程序（例如JDBC）的许多API提供了自己的方法来启动和控制事务，而不是从客户端发送START TRANSACTION语句。*  
为了要显式禁用自动提交模式，请使用以下语句：
```sql
SET autocommit=0;
```
通过将autocommit变量设置为零禁用自动提交模式后 ，对事务安全型表（例如InnoDB或NDB表）的更改不会立即生效。你必须使用COMMIT将更改存储到磁盘或ROLLBACK忽略更改。autocommit是一个**会话**变量，必须为每个会话设置。要为每个新连接禁用自动提交模式，需要修改系统变量。具体见[服务器系统变量](https://dev.mysql.com/doc/refman/8.0/en/server-system-variables.html#sysvar_autocommit)。  
BEGIN和BEGIN WORK作为START TRANSACTION启动事务的别名，START TRANSACTION是标准的SQL。推荐使用START TRANSACTION，而且START TRANSACTION还支持BEGIN不支持的标识符。  
这里的BEGIN关键字，与使用BEGIN ... END启动复合语句的关键字不一样，后者不会启动事务。  
*注意：在所有存储的程序（存储过程、函数、触发器和事件）中，解析器将BEGIN [WORK]视为BEGIN ... END块的开始 。在这种情况下必须使用START TRANSACTION开始一个事务*   
CHAIN和RELEASE可用于控制事务完成后的行为。completion_type系统变量的值决定了默认的事务完成行为。AND CHAIN子句在当前事务结束时立即开始新的事务，新事务与刚刚结束的事务具有相同的隔离级别。新事务也使用与刚刚终止的事务相同的访问模式（READ WRITE或READ ONLY）。RELEASE子句会控制**服务器**在终止当前事务后断开当前客户端会话。如果将completion_type系统变量设置为默认情况下会chaining or release，那么NO关键字对这种默认行为起抑制作用。  
开始一个事务会导致任何未决的事务被提交。见[导致隐式提交的语句](https://dev.mysql.com/doc/refman/8.0/en/implicit-commit.html)。  
开始事务也会导致使用LOCK TABLES获取的表锁被释放，就好像你已经执行了UNLOCK TABLES一样。开始事务不会释放用FLUSH TABLES WITH READ LOCK获取的全局读锁。  
为获得最佳结果，一个事务，应该只使用由单个事务安全型存储引擎管理的表。否则，可能会出现以下问题：  
 - 如果在一个事务中使用了来自多个事务安全存储引擎（如InnoDB）的表，并且事务隔离级别不是 SERIALIZABLE，则可能当一个事务提交时，另一个使用相同表的正在进行的事务将仅看到第一个事务所做的部分更改。也就是说，混合引擎(即使都是事务型)无法保证事务的原子性，并且可能导致不一致。（如果混合引擎事务不频繁，您可以根据需要使用SET TRANSACTION ISOLATION LEVEL将隔离级别设置为SERIALIZABLE）。
 - 如果在事务中使用非事务安全的表，无论自动提交模式的状态如何，对这些表的更改都会立即存储。
 - 在一个事务中，如果在更新非事务表之后发出ROLLBACK语句，会发生ER_WARNING_NOT_COMPLETE_ROLLBACK警告。对事务安全表的更改会回滚，但对非事务安全表将不会回滚。
在COMMIT时，每笔事务都存储在binlog中的一个块中，回滚的事务不被记录。(例外：对非事务表的修改无法回滚。如果回滚的事务包括对非事务表的修改，则mysql最后使用ROLLBACK语句记录整个事务，以确保对非事务表的修改被复制，保证主从完全一致)。  
你可以使用SET TRANSACTION语句更改事务的隔离级别或访问模式。  
回滚可能是一种缓慢的操作，可能会在用户没有明确要求的情况下隐式发生（例如，发生错误时）。因此，SHOW PROCESSLIST在会话的State列中显示Rolling back，不仅代表使用ROLLBACK语句执行的显式回滚，还包含隐式回滚。  
*注意：在MySQL 5.5，5.6，5.7，8.0中，BEGIN，COMMIT和ROLLBACK不受--replicate-do-db或--replicate-ignore-db规则的影响。*


