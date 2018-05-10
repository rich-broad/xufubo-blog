# InnoDB存储引擎
这里主要学习InnoDB的各种特性和功能，它的实现有几十万行代码，也涉及到很多技术，因此先学习、使用其特性，全面了解和使用特性之后要去会学会优化，进一步的研究其实现。
## 1、InnoDB简介
InnoDB是一种平衡高可靠性和高性能的通用存储引擎。在MySQL 5.5之后，InnoDB是默认的MySQL存储引擎。这里要说明一下：MySQL中存储引擎是以表为单位的。
InnoDB的主要优点包括：
 - DML操作遵循 ACID模型，具有提交，回滚和崩溃恢复功能的事务以保护用户数据。
 - 行级锁和Oracle风格一致性读增加了并发量和性能。
 - InnoDB表将数据存放在磁盘上，根据主键优化查询 。每个 InnoDB表都有一个主键索引，称为聚集索引（即：索引和行数据存储在一块，行数据按照主键排序），用于组织数据以使主键查找时的I/O最小化。
 - 为了保证数据完整性， InnoDB支持 FOREIGN KEY约束。使用外键时，将检查插入，更新和删除操作，以确保它们不会导致不同表之间的不一致。
 **InnoDB存储引擎的主要功能如下：**
 |特征|是否支持|
 |-|-|
 |B树索引|支持|
 |备份/时间点恢复(在服务器层实现,而非存储引擎层)|支持|
 |集群数据库支持|否|
 |聚集索引|是|
 |数据压缩|是|
 |数据缓存|是|
 |数据加密(通过加密函数在服务器中实现。静态数据表空间加密在MySQL 5.7及更高版本中可用。)|是|
 |外键支持|是|
 |全文索引|是的（InnoDB对全文索引的支持在MySQL 5.6和更高版本中可用。）|
 |地理空间数据类型支持|是|
 |地理空间索引支持|是的（InnoDB对地理空间索引的支持在MySQL 5.7和更高版本中可用。）|
 |哈希索引|否（InnoDB在内部使用自适应哈希索引）|
 |索引缓存|是|
 |锁粒度|行级锁|
 |MVCC|是|
 |复制（在服务器层实现,而非存储引擎层）|是|
 |存储限制|64TB|
 |T-tree索引|否|
 |事务|是|
 |更新数据字典的统计信息|是|
### 1.1 使用InnoDB表的好处
 - 如果您的服务器因硬件或软件问题而崩溃，无论当时数据库中发生了什么情况，重新启动数据库后无需执行任何特殊操作。InnoDB 崩溃恢复机制会自动完成崩溃前提交的任何更改，并撤消所有正在处理但未提交的更改。你只需要简单的从你断开的地方继续。
 - InnoDB存储引擎维护它自己的缓冲池，这通常可以加快数据访问。
 - 支持外键，从而支持参照完整性约束。更新或删除主表数据时，其他辅助表中的相关数据会自动更新或删除。尝试将数据插入辅助表中，而主表中没有相应的数据时，会将错误的数据会自动剔除。
 - 如果数据在磁盘或内存中损坏，校验和机制会在你使用数据前告诉你将会提供虚假数据。
 - 当你为每个表设计适当的主键时 ，涉及主键这些列的操作会自动进行优化。引用主键列的WHERE子句，ORDER BY子句， GROUP BY子句和通过主键列的连接操作是非常快的。
 - 通过change buffer机制，可以自动优化更新，删除，插入，InnoDB不仅可以对同一个表进行并发读取和写入访问，还可以缓存更改的数据以简化磁盘I/O。
 - 统计查询热点，建立自适应哈希索引以加快查询。
 - 可以压缩表和关联的索引。
 - 可以在对性能和可用性影响很小的情况下创建和删除索引。
 - Truncating [file-per-table](https://dev.mysql.com/doc/refman/8.0/en/glossary.html#glos_file_per_table) 表表空间是非常快的，这可以释放空间供操作系统使用。
 - 使用[DYNAMIC](https://dev.mysql.com/doc/refman/8.0/en/glossary.html#glos_dynamic_row_format)行格式，对BLOB和long text更加有效。
 - 可以通过查询[INFORMATION_SCHEMA](https://dev.mysql.com/doc/refman/8.0/en/glossary.html#glos_information_schema) 表来监视存储引擎的内部运作情况。
 - 可以通过查询[Performance Schema](https://dev.mysql.com/doc/refman/8.0/en/glossary.html#glos_performance_schema)表来监控存储引擎的性能细节。
 - 即使在相同的语句中，也可以自由地将InnoDB表与来自其他MySQL存储引擎的表组合。
 - InnoDB被设计用于处理大量数据时的CPU效率和最高性能。
 - 即使在文件大小限制为2GB的操作系统上，InnoDB也可处理大量数据。
### 1.2 InnoDB表最好的实践
 - 将最常查询的列（也可以为多列）指定为表的主键， 如果没有明显的主键，则指定自增值。
 - 根据这些表中相同的ID值， 使用连接从多个表中提取数据。对于快速连接性能，请在连接列上定义 外键，并在每个表中声明具有相同数据类型的列。添加外键确保引用的列被索引，这可以提高性能。如果父表中不存在相应的ID，外键还会将删除或更新传播到所有受影响的表，并阻止在子表中插入数据。
 - 关闭自动提交，每秒提交数百次会限制性能（受存储设备写入速度的限制）。
 - 将一组相关的DML操作封装到事务中，即：将他们用START TRANSACTION和 COMMIT包起来。虽然你不想过于频繁地提交，但是你肯定也不想发出大量的在几个小时也运行不完的巨大 INSERT， UPDATE或者 DELETE操作，因此要平衡一个事务的执行时间，不然巨大的查询（广义的查询）占用了所有时间，导致其他小的查询也无法执行。因此最好将巨大的查询分解一下，这属于通用的实践原则。 
 - 不使用LOCK TABLES 语句。因为InnoDB可以在不牺牲可靠性或高性能的前提下处理多个会话同时读取和写入同一张表的情况。要获取对一组行的独占写入访问权限，请使用 SELECT ... FOR UPDATE语法锁定你要更新的行。
 - 启用 innodb_file_per_table选项，将各个表的数据和索引放入单独的文件中，而不是在单个巨大的 系统表空间中文件中。这个选项对于一些其他的特性也是必须的，例如表 压缩和快速 截断，要使用这两种特性，这个选项必须打开。
 - 评估你的数据和访问模式是否受益于InnoDB表和页面的压缩功能，你可以压缩InnoDB表而不牺牲读/写能力。
 - 在--sql_mode=NO_ENGINE_SUBSTITUTION 模式下运行服务器，一避免创建的表的存储引擎选择有误。
 ### 1.3 验证InnoDB是默认存储引擎
 ```sql
SHOW ENGINES;
 ```
 ```sql
SELECT * FROM INFORMATION_SCHEMA.ENGINES;
 ```
 ```
+--------------------+---------+----------------------------------------------------------------+--------------+------+------------+
| ENGINE             | SUPPORT | COMMENT                                                        | TRANSACTIONS | XA   | SAVEPOINTS |
+--------------------+---------+----------------------------------------------------------------+--------------+------+------------+
| FEDERATED          | NO      | Federated MySQL storage engine                                 | NULL         | NULL | NULL       |
| MRG_MYISAM         | YES     | Collection of identical MyISAM tables                          | NO           | NO   | NO         |
| CSV                | YES     | CSV storage engine                                             | NO           | NO   | NO         |
| BLACKHOLE          | YES     | /dev/null storage engine (anything you write to it disappears) | NO           | NO   | NO         |
| MEMORY             | YES     | Hash based, stored in memory, useful for temporary tables      | NO           | NO   | NO         |
| InnoDB             | DEFAULT | Supports transactions, row-level locking, and foreign keys     | YES          | YES  | YES        |
| ARCHIVE            | YES     | Archive storage engine                                         | NO           | NO   | NO         |
| MyISAM             | YES     | MyISAM storage engine                                          | NO           | NO   | NO         |
| PERFORMANCE_SCHEMA | YES     | Performance Schema                                             | NO           | NO   | NO         |
+--------------------+---------+----------------------------------------------------------------+--------------+------+------------+
 ```

 ### 1.4 使用InnoDB进行测试和基准测试


## 2、InnoDB和ACID模型
ACID模型是非常重要的，需要深入学习和理解，这里先简单的列出与ACID相关的MySQL特性。
 - A：原子性
 - C：一致性
 - I：隔离性
 - D：持久性  
**原子性**  
原子性方面主要涉及InnoDB事务。相关的MySQL功能包括：
 - 自动提交设置。
 - COMMIT语句。
 - ROLLBACK语句。
 - 来自INFORMATION_SCHEMA 表格的操作数据  
**一致性**  
一致性主要涉及InnoDB为崩溃而设计的保护机制，相关的MySQL功能包括：  
 - InnoDB 双写缓冲区。
 - InnoDB 崩溃恢复。  
**隔离性**  
隔离性主要涉及InnoDB事务，特别是适用于每个事务的隔离级别。相关的MySQL功能包括：  
 - 自动提交的设置。
 - SET ISOLATION LEVEL语句。
 - InnoDB锁的底层细节。在性能调优期间，可以通过INFORMATION_SCHEMA表格查看这些细节。  
**持久性**  
持久性方面涉及MySQL软件功能与你的特定硬件配置进行交互。由于硬件多种多样，因此，这方面是最为复杂的，提供具体的指导方针也是最复杂的。（这些方针可能是购买“新硬件”）。相关的MySQL功能包括：
 - InnoDB 双写缓冲区，由innodb_doublewrite 配置选项打开和关闭 。
 - 配置选项 innodb_flush_log_at_trx_commit。
 - 配置选项 sync_binlog。
 - 配置选项 innodb_file_per_table。
 - 存储设备中（如磁盘驱动器，SSD或RAID阵列）的写缓冲。
 - 存储设备中的电池备份缓存。
 - 保护运行MySQL服务器和存储MySQL数据的所有计算机服务器和存储设备的不间断电源（UPS）。
 - 你的备份策略，例如备份频率和类型以及备份保留期。
 - 对于分布式或托管数据应用程序，MySQL服务器硬件所在数据中心的特定特征以及数据中心之间的网络连接。  

## 3、InnoDB多版本
InnoDB是一个[多版本存储引擎](https://dev.mysql.com/doc/refman/8.0/en/glossary.html#glos_mvcc),通过多版本技术，可以提高数据库的并发能力。为了支持事务功能，如并发和回滚，它会保存有关已更改行的旧版本。这些信息被存储在表空间中被称为[回滚段(rollback segment)](https://dev.mysql.com/doc/refman/8.0/en/glossary.html#glos_rollback_segment)的数据结构中。InnoDB 使用回滚段中的信息执行事务回滚中所需的撤消操作。它还使用这些信息构建一个行的早期版本以保证[一致性读取(consistent read)](https://dev.mysql.com/doc/refman/8.0/en/glossary.html#glos_consistent_read)。
在内部，InnoDB为存储在数据库中的每一行添加三个字段：
 - DB_TRX_ID：6字节，表示插入或更新行的最后一个事务的事务标识符。
 - DB_ROLL_PTR：
 - DB_ROW_ID：
在回滚段中的undo logs(存储在回滚段这种数据结构中)被分为插入和更新undo logs。
