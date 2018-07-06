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
如果InnoDB不是默认存储引擎，可以通过--default-storage-engine=InnoDB命令行选项，或者在配置文件的配置组[mysqld]下配置default-storage-engine=innodb来设置默认的存储引擎。更改默认存储引擎只会影响创建的新表。  
如果对使用哪个存储引擎没有考量清楚，并且想要预览使用InnoDB时表的工作方式，那么，为每个表使用ALTER TABLE table_name ENGINE = InnoDB；或者为了不干扰原表可以如下：  
```sql
CREATE TABLE InnoDB_Table (...) ENGINE=InnoDB AS SELECT * FROM other_engine_table;
```
要在真实工作负载下完整的评估应用程序的性能，请安装最新的MySQL服务器并运行基准测试。测试完整的应用程序生命周期，从安装到大量使用和服务器重新启动。在数据库繁忙期间kill服务器进程以模拟电源故障，并在重新启动服务器时验证数据是否已成功恢复。测试任何复制配置，特别是如果您在主服务器和从服务器上使用不同的MySQL版本和选项（***没有非常充足的理由千万不要这么干，除非你对各个mysql版本的特性了解的非常清楚，深入，甚至对内核代码都要非常熟悉***）。  
  
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


## 4、InnoDB体系结构
本节介绍InnoDB存储引擎体系结构的主要组件。  
### 4.1 缓冲池(buffer pool)
缓冲池是内存中的一个缓冲区， InnoDB将表和索引的热点数据缓冲在这里，这样，当访问数据库数据时，就可以直接访问内存中的数据，而无需访问磁盘，这就减少了IO，提高了速度。通常，在专用的数据库服务器上，通常将多达80％的物理内存分配给InnoDB缓冲池。为了实现大容量内存操作的效率，缓冲池被组织为页面的链表，链表使用改进的LRU算法维护。  
### 4.2 改变缓冲(Change Buffer)
存储引擎设计中的一个挑战是写入操作期间的随机I/O。在InnoDB中，一个表将具有一个聚集索引和零个或多个辅助索引。这些索引中的每一个都是一个B+树。将记录插入到表中时，首先将该记录插入到聚簇索引中，然后再插入到每个辅助索引中。因此，所产生的I/O操作将随机分布在磁盘上。对于更新和删除操作，I/O模式也是随机的。为了减轻这个问题，InnoDB存储引擎使用一种称为Change Buffer的特殊数据结构（因为以前称为插入缓冲区，因此你将看到内部在很多场景下用ibuf和IBUF表示Change Buffer）。Change Buffer是另一个B+树，用于缓冲对辅助索引以及辅助索引页面相关的改变。InnoDB中只有一个Change Buffer，并且保留在系统表空间中。Change Buffer树的根页面在系统表空间（空间id为0）中，固定为FSP_IBUF_TREE_ROOT_PAGE_NO（等于4）。当服务器启动时，通过使用此固定页码来加载Change Buffer树。你可以参考函数ibuf_init_at_db_start() 了解更多详细信息。Change Buffer的总大小是可配置的，旨在确保完整的Change Buffer树可以驻留在内存中。使用innodb_change_buffer_max_size系统变量配置Change Buffer的大小。
当更改的记录所在的页面不在buff pool（缓冲池）中时，这时按道理来说就要读写磁盘了，但是正如前面分析的，如果涉及到辅助索引页的修改（INSERT, UPDATE, DELETE），将会引起随机I/O操作，为此，InnoDB将这些更改缓冲到Change Buffer中，积累一段时间之后，将这些I/O操作进行合并，这样，将避免从磁盘读入辅助索引页面所需的大量随机访问I / O以及将辅助索引页面写入磁盘的随机写I/O。
在内存中，更改缓冲区占用InnoDB缓冲池的一部分 。在磁盘上，更改缓冲区是系统表空间的一部分，因此索引更改在数据库重新启动之间保持缓冲。缓存在更改缓冲区中的数据类型由 innodb_change_buffering 配置选项控制。  
**监控Change Buffer：**  
 - InnoDB的标准监视器的输出包括Change Buffer的状态信息。要查看监控数据，请发出SHOW ENGINE INNODB STATUS命令。Change Buffer的状态信息位于INSERT BUFFER AND ADAPTIVE HASH INDEX 标题下方。输出的具体含义后边解释。内容与下边类似：  
 ```
 -------------------------------------
INSERT BUFFER AND ADAPTIVE HASH INDEX
-------------------------------------
Ibuf: size 1, free list len 0, seg size 2, 0 merges
merged operations:
 insert 0, delete mark 0, delete 0
discarded operations:
 insert 0, delete mark 0, delete 0
Hash table size 4425293, used cells 32, node heap has 1 buffer(s)
13577.57 hash searches/s, 202.47 non-hash searches/s
 ```
 - INFORMATION_SCHEMA.INNODB_METRICS 表也提供了在InnoDB的标准监视器输出中的大部分数据 ，以及其他数据。要查看Change Buffer，请发出以下查询：
 ```SQL
SELECT NAME, COMMENT FROM INFORMATION_SCHEMA.INNODB_METRICS WHERE NAME LIKE '%ibuf%'\G
 ```
 -  INFORMATION_SCHEMA.INNODB_BUFFER_PAGE 表提供了InnoDB缓冲池中每个页面的元数据，Change Buffer页面包括 change buffer index和change buffer bitmap两种页面。Change Buffer页面由 PAGE_TYPE的值决定，PAGE_TYPE值为IBUF_INDEX 表示change buffer index页面，值为IBUF_BITMAP表示change buffer bitmap页面。  
 **警告: 查询 INNODB_BUFFER_PAGE表可能会引起显着的性能开销。为了避免影响性能，请在测试环境的实例上重现问题并解决问题**  
例如，可以通过查询 INNODB_BUFFER_PAGE表以确定 IBUF_INDEX和 IBUF_BITMAP页面占总缓冲池页面的百分比。  
```sql
SELECT 
(
SELECT COUNT(*) FROM INFORMATION_SCHEMA.INNODB_BUFFER_PAGE 
WHERE PAGE_TYPE LIKE 'IBUF%' 
) AS change_buffer_pages, 
( 
SELECT COUNT(*) FROM INFORMATION_SCHEMA.INNODB_BUFFER_PAGE 
) AS total_pages, 
( 
SELECT ((change_buffer_pages/total_pages)*100) 
) AS change_buffer_page_percentage;
+---------------------+-------------+-------------------------------+
| change_buffer_pages | total_pages | change_buffer_page_percentage |
+---------------------+-------------+-------------------------------+
|                  25 |        8192 |                        0.3052 |
+---------------------+-------------+-------------------------------+
```
 - Performance Schema提供了更高级的性能监控，其中包括Change Buffer 的mutex等待的指示，可以如下查询：  
```sql
SELECT * FROM performance_schema.setup_instruments
       WHERE NAME LIKE '%wait/synch/mutex/innodb/ibuf%';
+-------------------------------------------------------+---------+-------+
| NAME                                                  | ENABLED | TIMED |
+-------------------------------------------------------+---------+-------+
| wait/synch/mutex/innodb/ibuf_bitmap_mutex             | YES     | YES   |
| wait/synch/mutex/innodb/ibuf_mutex                    | YES     | YES   |
| wait/synch/mutex/innodb/ibuf_pessimistic_insert_mutex | YES     | YES   |
+-------------------------------------------------------+---------+-------+
```
### 4.3 自适应哈希索引(Adaptive Hash Index)
自适应哈希索引是InnoDB基于对查询的统计分析，在内存中基于表上现有的B树索引构建的哈希索引 （自适应哈希索引始终基于表上现有的B树索引构建）。 InnoDB可以根据InnoDB针对B树索引的搜索模式，为B树键的任意长度的前缀创建哈希索引。哈希索引可以仅覆盖经常访问的索引对应的那些页面。如果一个表刚好完全可以载入内存，那么自适应哈希索引通常是有益的。不过自适应哈希索引页的维护也要消耗资源，而且对于范围查找、LIKE运算，自适应哈希索引常常是无效的，而且，在多并发的情况下，对自适应哈希索引相关的锁的争用也比较严重，此时，关闭自适应哈希索引通常可以减少很多性能开销。由于难以预先确定此功能是否应该开启，因此请使用实际的工作负载进行基准测试来决定是启用还是禁用。由innodb_adaptive_hash_index 选项启用 ，--skip-innodb_adaptive_hash_index禁用。默认为启用。  
```sql
MySQL [(none)]> SHOW variables like '%innodb_adaptive_hash_index%';
+----------------------------------+-------+
| Variable_name                    | Value |
+----------------------------------+-------+
| innodb_adaptive_hash_index       | ON    |
| innodb_adaptive_hash_index_parts | 8     |
+----------------------------------+-------+
```
在早期，自适应哈希索引由单个锁保护，这就会导致并发性不足，为此，从5.7开始，将自适应哈希索引分为多个区，每个区一个锁，通过将锁粒度变细来增加并发性。分区由innodb_adaptive_hash_index_parts 配置选项控制，默认为8，最大为512。如上语句输出。在SHOW ENGINE INNODB STATUS命令的SEMAPHORES输出部分可以监视自适应哈希索引的使用和锁的争用，如果看到很多线程正在等待锁，那么禁用自适应哈希索引通常是有益的。  
### 4.4 重做日志缓冲区(Redo Log Buffer)
重做日志文件的主要目的是，万一实例或者介质失败（media failure），重做日志文件就能派上用场。如数据库由于所在主机掉电导致实例失败，InnoDB存储引擎会使用重做日志恢复到掉电前的时刻，以此来保证数据的完整性。默认情况下会有两个文件，名称分别为ib_logfile0和ib_logfile1。MySQL官方手册中将其称为InnoDB存储引擎的日志文件，不过更准确的定义应该是重做日志文件（redo log file）。   
重做日志缓冲区是用来保存要写入[重做日志(redo log)](https://dev.mysql.com/doc/refman/8.0/en/glossary.html#glos_redo_log)的数据的内存区域。重做日志缓冲区大小由innodb_log_buffer_size 配置选项定控制。重做日志缓冲区会定期刷新到磁盘上的重做日志文件。大型重做日志缓冲区可以实现大型事务的运行，无需在事务提交之前不断的将重做日志写入磁盘。因此，如果有更新，插入或删除许多行的事务，使日志缓冲区更大，可以节省磁盘I/O。  
innodb_flush_log_at_trx_commit 选项控制如何将重做日志缓冲区的内容写入日志文件。innodb_flush_log_at_timeout 选项控制重做日志缓冲区刷新频率。
### 4.5 系统表空间(System Tablespace)
InnoDB系统表空间包含InnoDB的数据字典（InnoDB数据对象的元数据），并且也是doublewrite buffer, change buffer, and undo logs的存储区域，系统表空间还包含用户创建的表和索引的数据，因此系统表空间是一个共享表空间，因为它被多个表（包括不同数据库中的表）共享。  
系统表空间由一个或多个数据文件表示。默认情况下，MySQL 在data目录中创建一个名为ibdata1的系统数据文件。系统数据文件的大小和数量由innodb_data_file_path启动选项控制 。  
### 4.6 双写缓冲(Doublewrite Buffer)
InnoDB中，在将缓冲池中的数据刷新到磁盘时是以页面（InnoDB的页面，通常为16KB）为单位的，这时可能会出现部分页面写入的问题。所谓部分页面写入是指向操作系统提交的页面写入请求仅部分完成。例如，在16K 的Innodb页面中，只有第一个4KB（文件系统的块通常为4KB）的块被写入磁盘，其他部分保持原来的状态。最常见的部分页面写入一般在发生电源故障时发生。也可能发生在操作系统崩溃时。另外，如果使用软件RAID，页面可能会出现在需要多个IO请求的条带边界上。如果硬件RAID没有电池备份，电源故障时也会发生这种情况。如果对磁盘本身发出单个写入，即使电源掉电，在理论上也应完成写入，因为驱动器内部应该有足够的电源来完成它。但是真的很难检查是否总是这样，因为它不是部分页面写入的唯一原因。在Innodb Doublewrite Buffer实施之前，确实会有数据损坏。  
有的人会问，数据损坏可以使用重做日志来恢复呀，但是，请注意，InnoDB并不会将整个页面的内容写入重做日志，而是记录的对页面的操作，例如将某个偏移量处的值加2，使用重做日志进行恢复的基础是表空间中的实际数据页面在内部是完整的一致的，它是哪个页面版本无关紧要 ，但是如果页面不一致，则无法继续恢复，因为你的基础数据就是不一致的。为此引入了Doublewrite Buffer来解决问题。  
理解了为什么需要Doublewrite Buffer，也就不难理解Doublewrite Buffer如何工作了。具体来说就是：你可以将Doublewrite Buffer视为系统表空间中的一个短期日志文件，它包含100个页的空间。当Innodb从Innodb缓冲池中刷新页面时，InnoDB首先会将页面写入双写缓冲区（顺序），然后调用fsync（）以确保它们保存到磁盘，然后将页面写入真正的数据文件并第二次调用fsync（））。现在Innodb恢复的时候会检查表空间中数据页面的内容和Doublewrite Buffer中页面的内容。如果在双写缓冲区中的页面不一致，则简单地丢弃它，如果表空间中的数据页面不一致，则从双写缓冲区中恢复。那么会不会出现都不一致的情况呢？这个不会，以内是先写Doublewrite Buffer，后写表空间中真实的数据页面，这样，当Doublewrite Buffer中不一致时表示系统崩溃了，也就无法继续执行了，就不会收到Doublewrite Buffer是否写成功的响应，也就不会发出真实的数据页面的写操作，这样的话必然不会出现二者都损坏的情况。  
虽然Doublewrite Buffer的加入会使每次刷新数据时写两次磁盘，但是性能不会大幅下降，因为Doublewrite Buffer的写入是顺序的。所以一般来说，由于使用Doublewrite而不会超过5-10％的性能损失。但是数据是无价之宝，比起这个，这点损失可以接受。  
那么Doublewrite是否可以禁用的？默认是开启的，要禁用Doublewrite，可以设置innodb_doublewrite=0。当然了前提是你可以忍受数据丢失。  
如果系统表空间文件（“ ibdata文件 ”）位于支持原子写入的Fusion-io设备上，则自动禁用Doublewrite Buffer，并将Fusion-io原子写入用于所有数据文件。因为双写缓冲区设置是全局的，因此对非Fusion-io硬件上的数据文件也将禁用Doublewrite Buffer。此功能仅在Fusion-io硬件上并且仅在Linux上启用Fusion-io NVMFS下受支持。要充分利用此功能，建议使用innodb_flush_method设置 O_DIRECT。  
因此最好不要禁用Doublewrite Buffer。除非可以忍受数据丢失。  
### 4.7 撤销日志(Undo Logs)
撤消日志是与单个事务关联的撤消日志记录的集合，撤销日志记录包含有关如何撤消事务对聚集索引记录的最新更改的信息。如果另一个事务需要查看原始数据（为了实现一致性读），未修改的数据将从撤消日志记录中检索，撤销日志存在于撤消日志段中，撤消日志段包含在回滚段中，回滚段位于撤消表空间和临时表空间中。有关撤消表空间的更多信息，见：[15.7.8 Configuring Undo Tablespaces](https://dev.mysql.com/doc/refman/8.0/en/innodb-undo-tablespace.html)。有关多版本的信息，请参阅[15.3 InnoDB Multi-Versioning](https://dev.mysql.com/doc/refman/8.0/en/innodb-multi-versioning.html)。临时表空间和每个撤消表空间分别支持最多128个回滚段。innodb_rollback_segments配置选项定义了回滚段的数量。每个回滚段最多支持1023个并发的数据修改事务。  

### 4.8 File-Per-Table 表空间
File-Per-Table 表空间的意思就是mysql为每一个表创建一个表空间，并使用单独的文件存储表的数据和索引，而不是将这些数据存储在系统表空间中。这个功能由innodb_file_per_table选项控制。如果innodb_file_per_table选项没有启用，InnoDB表将在系统表空间中创建。File-Per-Table 表空间由一个.ibd数据文件表示，默认情况下该数据文件在数据库目录中创建。File-Per-Table 表空间支持DYNAMIC和COMPRESSED行格式。有关File-Per-Table 表空间的优点，见[15.7.4 InnoDB File-Per-Table Tablespaces](https://dev.mysql.com/doc/refman/8.0/en/innodb-multiple-tablespaces.html)。  

### 4.9 一般表空间
使用CREATE TABLESPACE语法创建InnoDB的共享表空间。一般表空间可以在MySQL数据目录之外创建，一般表空间能够容纳多个表格，并支持所有行格式。使用CREATE TABLE tbl_name ... TABLESPACE [=] tablespace_name或ALTER TABLE tbl_name TABLESPACE [=] tablespace_name语法将表添加到一般表空间中。更多信息，请参阅[15.7.10 InnoDB General Tablespaces](https://dev.mysql.com/doc/refman/8.0/en/general-tablespaces.html)。  

### 4.10 撤销(undo)表空间
撤消表空间包含一个或多个包含撤消日志的文件。innodb_undo_tablespaces(已弃用，将在未来版本中删除)选项定义了InnoDB撤销表空间的数量。更多信息，见：[15.7.8 Configuring Undo Tablespaces](https://dev.mysql.com/doc/refman/8.0/en/innodb-undo-tablespace.html)。  

### 4.11 临时表空间
用户创建的临时表和磁盘内部临时表在共享的临时表空间中创建。innodb_temp_data_file_path配置选项定义临时表空间数据文件的相对路径，名称，大小和其他属性。如果没有为innodb_temp_data_file_path指定值，默认情况下将在innodb_data_home_dir目录中创建一个名为ibtmp1的自动扩展数据文件。临时表空间在正常关闭或者被中断的初始化(aborted initialization)的情况下将被删除，并在每次启动服务器时重新创建。临时表空间在创建时接收动态生成的表空间ID。如果无法创建临时表空间，将无法启动。如果服务器意外停止，则不会删除临时表空间。在这种情况下，数据库管理员可以手动删除临时表空间或重新启动服务器。这会自动删除并重新创建临时表空间。  
临时表空间不能驻留在原始(raw)设备上。  
INFORMATION_SCHEMA.FILES提供有关InnoDB临时表空间的元数据。  
```sql
mysql> SELECT * FROM INFORMATION_SCHEMA.FILES WHERE TABLESPACE_NAME='innodb_temporary'\G
```
INFORMATION_SCHEMA.INNODB_TEMP_TABLE_INFO提供有关在InnoDB实例中当前处于活动状态的用户创建的临时表的元数据。更多信息，见[15.14.7 InnoDB INFORMATION_SCHEMA Temporary Table Info Table](https://dev.mysql.com/doc/refman/8.0/en/innodb-information-schema-temp-table-info.html)。  
**管理临时表空间数据文件大小：**  
默认情况下，临时表空间数据文件是自动扩展的，并根据需要增加大小以存储磁盘上的临时表。例如，如果某个操作创建了一个大小为20 MB的临时表，这就会导致临时表空间数据文件的自动增长。当临时表被删除时，释放的空间可以重新用于新的临时表，但是临时表空间的数据文件不会缩小。  
通过上边的描述，可以知道：在使用大型临时表或广泛使用临时表的环境中，自动扩展使得临时表空间数据文件可能会变的很大（例如同时创建了100个1GB的临时表，那么临时表空间的数据文件将达到100GB，而且不其大小不会缩小，除非mysql的实例重启）。  
要确定临时表空间数据文件是否自动扩展，请检查innodb_temp_data_file_path设置：  
```sql
mysql> SELECT @@innodb_temp_data_file_path;
+------------------------------+
| @@innodb_temp_data_file_path |
+------------------------------+
| ibtmp1:12M:autoextend        |
+------------------------------+
```
要检查临时表空间数据文件的大小，请使用与此类似的查询来查询INFORMATION_SCHEMA.FILES表：  
```sql
mysql> SELECT FILE_NAME, TABLESPACE_NAME, ENGINE, INITIAL_SIZE, TOTAL_EXTENTS*EXTENT_SIZE 
    ->        AS TotalSizeBytes, DATA_FREE, MAXIMUM_SIZE FROM INFORMATION_SCHEMA.FILES 
    ->        WHERE TABLESPACE_NAME = 'innodb_temporary';
+-----------+------------------+--------+--------------+----------------+-----------+--------------+
| FILE_NAME | TABLESPACE_NAME  | ENGINE | INITIAL_SIZE | TotalSizeBytes | DATA_FREE | MAXIMUM_SIZE |
+-----------+------------------+--------+--------------+----------------+-----------+--------------+
| ./ibtmp1  | innodb_temporary | InnoDB |     12582912 |       12582912 |   6291456 |         NULL |
+-----------+------------------+--------+--------------+----------------+-----------+--------------+
1 row in set (0.02 sec)
```
TotalSizeBytes值报告临时表空间数据文件的当前大小。有关其他字段值的信息，见[24.9 The INFORMATION_SCHEMA FILES Table](https://dev.mysql.com/doc/refman/8.0/en/files-table.html)。  
要回收临时表空间数据文件占用的磁盘空间，可以重新启动MySQL服务器。重新启动服务器时将根据innodb_temp_data_file_path定义的属性删除并重新创建临时表空间数据文件。  
为防止临时表空间数据文件变得过大，你可以配置innodb_temp_data_file_path选项以指定最大文件大小。例如：  
```
[mysqld]
innodb_temp_data_file_path=ibtmp1:12M:autoextend:max:500M
```
当数据文件达到最大时，查询将失败，并显示表已满的错误。配置innodb_temp_data_file_path需要重新启动服务器。  
也可以配置default_tmp_storage_engine和internal_tmp_disk_storage_engine选项，它们分别定义了用户创建临时表和磁盘内部临时表的存储引擎。这两个选项默认设置为InnoDB。MyISAM存储引擎为每个临时表使用单独的数据文件，临时表被删除时将删除该文件。  
**临时表撤消日志(Temporary Table Undo Logs)**  
在[15.4.7 Undo Logs](https://dev.mysql.com/doc/refman/8.0/en/innodb-undo-logs.html)中，我们知道，**撤销日志存在于撤消日志段中，撤消日志段包含在回滚段中，回滚段位于撤消表空间和临时表空间中。**你可能会问，为什么undo log还会存在于临时表空间呢？因为临时表的撤消日志驻留在临时表空间中，用于临时表和相关对象。***临时表撤消日志不会被记录到redo log中，因为它们不是崩溃恢复所必需的，它们仅用于服务器运行时的回滚。这种特殊类型的撤消日志通过避免重做日志I/O来提高性能***。innodb_rollback_segments配置选项定义临时表空间使用的回滚段的数量(撤销表空间的回滚段数量也由这个定义，从名字就看得出这是一个通用选项，并不专用与临时表空间的回滚段)。  

### 4.12 重做日志(Redo Log)
重做日志是在**崩溃恢复**期间使用的基于磁盘的数据结构，用于纠正由不完整事务写入的数据。在正常操作中，重做日志对由SQL语句或低级API调用引起InnoDB表更改的数据进行编码。在服务器初始化过程中，将会做崩溃恢复操作，在这完成之前，不能接受数据库连接。有关重做日志在崩溃恢复中的角色的信息，参见[15.17.2 InnoDB Recovery](https://dev.mysql.com/doc/refman/8.0/en/innodb-recovery.html)。  
默认情况下，重做日志在物理上表现为一组名为**ib_logfile0**和**ib_logfile1**的文件。MySQL以循环方式写入重做日志文件。重做日志中的数据按受影响的记录进行编码;这些数据统称为重做。数据通过重做日志的过程由不断增加的[LSN(log sequence number)](https://dev.mysql.com/doc/refman/8.0/en/glossary.html#glos_lsn)值表示。  
可见：redo log对于事务的实现非常重要，相关的信息，见：  
 - [Section 15.6.1, InnoDB Startup Configuration](https://dev.mysql.com/doc/refman/8.0/en/innodb-init-startup-configuration.html)。  
 - [Section 8.5.4, Optimizing InnoDB Redo Logging](https://dev.mysql.com/doc/refman/8.0/en/optimizing-innodb-logging.html)。  
 - [Section 15.7.2, Changing the Number or Size of InnoDB Redo Log Files](https://dev.mysql.com/doc/refman/8.0/en/innodb-data-log-reconfiguration.html)。  
 - [InnoDB Crash Recovery](https://dev.mysql.com/doc/refman/8.0/en/innodb-recovery.html#innodb-crash-recovery)。  

#### 4.12.1 重做日志刷新的组提交
与任何其他ACID兼容的数据库引擎一样，InnoDB在提交之前刷新事务的重做日志。InnoDB使用组提交功能将多个这样的刷新请求组合在一起，以避免每次提交刷新一次磁盘。这可以大量减少IO操作，从而增加吞吐量。有关COMMIT和其他事务操作性能的更多信息，请参阅：[8.5.2 Optimizing InnoDB Transaction Management](https://dev.mysql.com/doc/refman/8.0/en/optimizing-innodb-transaction-management.html)。  
  
## 5 InnoDB锁和事务模型
### 5.1 InnoDB锁


### 5.2 InnoDB事务模型
在InnoDB事务中，目标是将多版本技术和传统的两阶段锁结合起来，实现的事务系统。默认情况下，InnoDB为行级锁，并且通过多版本技术提供了非锁定情况下的一致性读。InnoDB中的锁信息以节省空间的方式存储，因此不需要锁升级(***这句话有些没理解***)。通常，允许多个用户锁定InnoDB表中的每一行，或任何行的随机子集，而不会导致InnoDB内存耗尽。  

#### 5.2.1 事务隔离级别



## 7 InnoDB表空间
### 7.1 调整InnoDB系统表空间的大小
InnoDB的系统表空间


## 8 InnoDB表和索引
### 8.1 InnoDB表
#### 8.1.1 创建InnoDB表
```sql
CREATE TABLE t1 (a INT, b CHAR (20), PRIMARY KEY (a)) ENGINE=InnoDB;
```
即：创建表时指定ENGINE=InnoDB，如果默认的存储引擎就是InnoDB，那么可以不用指定。确定默认存储引擎：  
```sql
mysql> SELECT @@default_storage_engine;
+--------------------------+
| @@default_storage_engine |
+--------------------------+
| InnoDB                   |
+--------------------------+
```
InnoDB表及其索引可以创建在 system tablespace,file-per-table tablespace 或者 general tablespace中。默认情况下，innodb_file_per_table是启用的。InnoDB表被创建在file-per-table tablespace中。相反，当innodb_file_per_table被禁用时，InnoDB表被创建在系统表空间中。要在常规表空间中创建表，请使用 CREATE TABLE ... TABLESPACE语法。更多信息参见[15.7.10 InnoDB General Tablespaces](https://dev.mysql.com/doc/refman/8.0/en/general-tablespaces.html)。  

默认情况下，当在file-per-table tablespace模式下创建一个表时，MySQL会在MySQL数据目录下的数据库目录中创建一个.ibd表空间文件。在InnoDB系统表空间中创建的表是在现有的ibdata文件中创建的，该文件位于MySQL数据目录中。在general tablespace中创建的表被放在general tablespace的.ibd文件中。可以在MySQL数据目录的内部或外部创建general tablespace文件。更多信息参见[15.7.10 InnoDB General Tablespaces](https://dev.mysql.com/doc/refman/8.0/en/general-tablespaces.html)。  

在内部实现层面，InnoDB将每个表的条目添加到数据字典中。该条目包括数据库名称。例如，如果在test数据库中创建了表t1，则数据库名称的数据字典条目为'test/t1'，这意味着在InnoDB内部，你可以在不同的数据库中创建相同的表，而不会冲突。  

**InnoDB表格和行格式**  
InnoDB表的默认行格式由innodb_default_row_format配置选项定义，其默认值为DYNAMIC。动态(Dynamic)和压缩(Compressed)行格式时你可以充分利用InnoDB的一系列优良特性，例如：表压缩和长列值的高效页外存储。要使用这些行格式，必须启用innodb_file_per_table（默认值）。  
```sql
SET GLOBAL innodb_file_per_table=1;
CREATE TABLE t3 (a INT, b CHAR (20), PRIMARY KEY (a)) ROW_FORMAT=DYNAMIC;
CREATE TABLE t4 (a INT, b CHAR (20), PRIMARY KEY (a)) ROW_FORMAT=COMPRESSED;
```
或者，可以使用CREATE TABLE ... TABLESPACE语法在通用表空间中(general tablespace)创建InnoDB表。通用表空间中(general tablespace)支持所有行格式。
```sql
CREATE TABLE t1 (c1 INT PRIMARY KEY) TABLESPACE ts1 ROW_FORMAT=DYNAMIC;
```
CREATE TABLE ... TABLESPACE语法还可用于在系统表空间中创建具有动态行格式的InnoDB表，以及具有Compact或Redundant行格式的表。  
```sql
CREATE TABLE t1 (c1 INT PRIMARY KEY) TABLESPACE = innodb_system ROW_FORMAT=DYNAMIC;
```
有关InnoDB行格式的更多信息，请参阅[15.10 InnoDB Row Storage and Row Formats](https://dev.mysql.com/doc/refman/8.0/en/innodb-row-format.html)。有关如何确定InnoDB表的行格式和InnoDB行格式的物理特征，请参阅[15.8.1.2 The Physical Row Structure of an InnoDB Table](https://dev.mysql.com/doc/refman/8.0/en/innodb-physical-record.html)。  

**InnoDB表和主键**  
始终为InnoDB表定义主键，特别是指定以下特点的列：  
 - 被最重要的查询引用。
 - 永远不会留空。
 - 永远不会有重复的值。
 - 一旦插入很少改变的值。  


