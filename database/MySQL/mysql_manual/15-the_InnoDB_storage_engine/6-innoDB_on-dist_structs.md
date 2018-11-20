---
title: InnoDB磁盘结构
---
# 4、InnoDB体系结构
本节介绍InnoDB存储引擎体系结构的主要组件。  

## 4.1 缓冲池(buffer pool)
缓冲池是内存中的一个缓冲区， InnoDB将表和索引的热点数据缓冲在这里，这样，当访问数据库数据时，就可以直接访问内存中的数据，而无需访问磁盘，这就减少了IO，提高了速度。通常，在专用的数据库服务器上，通常将多达80％的物理内存分配给InnoDB缓冲池。为了实现大容量内存操作的效率，缓冲池被组织为页面的链表，链表使用改进的LRU算法维护。  

## 4.2 改变缓冲(Change Buffer)
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

## 4.3 自适应哈希索引(Adaptive Hash Index)
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

## 4.4 重做日志缓冲区(Redo Log Buffer)
重做日志文件的主要目的是，万一实例或者介质失败（media failure），重做日志文件就能派上用场。如数据库由于所在主机掉电导致实例失败，InnoDB存储引擎会使用重做日志恢复到掉电前的时刻，以此来保证数据的完整性。默认情况下会有两个文件，名称分别为ib_logfile0和ib_logfile1。MySQL官方手册中将其称为InnoDB存储引擎的日志文件，不过更准确的定义应该是重做日志文件（redo log file）。   
重做日志缓冲区是用来保存要写入[重做日志(redo log)](https://dev.mysql.com/doc/refman/8.0/en/glossary.html#glos_redo_log)的数据的内存区域。重做日志缓冲区大小由innodb_log_buffer_size 配置选项定控制。重做日志缓冲区会定期刷新到磁盘上的重做日志文件。大型重做日志缓冲区可以实现大型事务的运行，无需在事务提交之前不断的将重做日志写入磁盘。因此，如果有更新，插入或删除许多行的事务，使日志缓冲区更大，可以节省磁盘I/O。  
innodb_flush_log_at_trx_commit 选项控制如何将重做日志缓冲区的内容写入日志文件。innodb_flush_log_at_timeout 选项控制重做日志缓冲区刷新频率。  

## 4.5 系统表空间(System Tablespace)
InnoDB系统表空间包含InnoDB的数据字典（InnoDB数据对象的元数据），并且也是doublewrite buffer, change buffer, and undo logs的存储区域，系统表空间还包含用户创建的表和索引的数据，因此系统表空间是一个共享表空间，因为它被多个表（包括不同数据库中的表）共享。  
系统表空间由一个或多个数据文件表示。默认情况下，MySQL 在data目录中创建一个名为ibdata1的系统数据文件。系统数据文件的大小和数量由innodb_data_file_path启动选项控制 。   

## 4.6 双写缓冲(Doublewrite Buffer)
InnoDB中，在将缓冲池中的数据刷新到磁盘时是以页面（InnoDB的页面，通常为16KB）为单位的，这时可能会出现部分页面写入的问题。所谓部分页面写入是指向操作系统提交的页面写入请求仅部分完成。例如，在16K 的Innodb页面中，只有第一个4KB（文件系统的块通常为4KB）的块被写入磁盘，其他部分保持原来的状态。最常见的部分页面写入一般在发生电源故障时发生。也可能发生在操作系统崩溃时。另外，如果使用软件RAID，页面可能会出现在需要多个IO请求的条带边界上。如果硬件RAID没有电池备份，电源故障时也会发生这种情况。如果对磁盘本身发出单个写入，即使电源掉电，在理论上也应完成写入，因为驱动器内部应该有足够的电源来完成它。但是真的很难检查是否总是这样，因为它不是部分页面写入的唯一原因。在Innodb Doublewrite Buffer实施之前，确实会有数据损坏。  
有的人会问，数据损坏可以使用重做日志来恢复呀，但是，请注意，InnoDB并不会将整个页面的内容写入重做日志，而是记录的对页面的操作，例如将某个偏移量处的值加2，使用重做日志进行恢复的基础是表空间中的实际数据页面在内部是完整的一致的，它是哪个页面版本无关紧要 ，但是如果页面不一致，则无法继续恢复，因为你的基础数据就是不一致的。为此引入了Doublewrite Buffer来解决问题。  
理解了为什么需要Doublewrite Buffer，也就不难理解Doublewrite Buffer如何工作了。具体来说就是：你可以将Doublewrite Buffer视为系统表空间中的一个短期日志文件，它包含100个页的空间。当Innodb从Innodb缓冲池中刷新页面时，InnoDB首先会将页面写入双写缓冲区（顺序），然后调用fsync（）以确保它们保存到磁盘，然后将页面写入真正的数据文件并第二次调用fsync（））。现在Innodb恢复的时候会检查表空间中数据页面的内容和Doublewrite Buffer中页面的内容。如果在双写缓冲区中的页面不一致，则简单地丢弃它，如果表空间中的数据页面不一致，则从双写缓冲区中恢复。那么会不会出现都不一致的情况呢？这个不会，以内是先写Doublewrite Buffer，后写表空间中真实的数据页面，这样，当Doublewrite Buffer中不一致时表示系统崩溃了，也就无法继续执行了，就不会收到Doublewrite Buffer是否写成功的响应，也就不会发出真实的数据页面的写操作，这样的话必然不会出现二者都损坏的情况。  
虽然Doublewrite Buffer的加入会使每次刷新数据时写两次磁盘，但是性能不会大幅下降，因为Doublewrite Buffer的写入是顺序的。所以一般来说，由于使用Doublewrite而不会超过5-10％的性能损失。但是数据是无价之宝，比起这个，这点损失可以接受。  
那么Doublewrite是否可以禁用的？默认是开启的，要禁用Doublewrite，可以设置innodb_doublewrite=0。当然了前提是你可以忍受数据丢失。  
如果系统表空间文件（“ ibdata文件 ”）位于支持原子写入的Fusion-io设备上，则自动禁用Doublewrite Buffer，并将Fusion-io原子写入用于所有数据文件。因为双写缓冲区设置是全局的，因此对非Fusion-io硬件上的数据文件也将禁用Doublewrite Buffer。此功能仅在Fusion-io硬件上并且仅在Linux上启用Fusion-io NVMFS下受支持。要充分利用此功能，建议使用innodb_flush_method设置 O_DIRECT。  
因此最好不要禁用Doublewrite Buffer。除非可以忍受数据丢失。  

## 4.7 撤销日志(Undo Logs)
撤消日志是与单个事务关联的撤消日志记录的集合，撤销日志记录包含有关如何撤消事务对聚集索引记录的最新更改的信息。如果另一个事务需要查看原始数据（为了实现一致性读），未修改的数据将从撤消日志记录中检索，撤销日志存在于撤消日志段中，撤消日志段包含在回滚段中，回滚段位于撤消表空间和临时表空间中。有关撤消表空间的更多信息，见：[15.7.8 Configuring Undo Tablespaces](https://dev.mysql.com/doc/refman/8.0/en/innodb-undo-tablespace.html)。有关多版本的信息，请参阅[15.3 InnoDB Multi-Versioning](https://dev.mysql.com/doc/refman/8.0/en/innodb-multi-versioning.html)。临时表空间和每个撤消表空间分别支持最多128个回滚段。innodb_rollback_segments配置选项定义了回滚段的数量。每个回滚段最多支持1023个并发的数据修改事务。  

## 4.8 File-Per-Table 表空间
File-Per-Table 表空间的意思就是mysql为每一个表创建一个表空间，并使用单独的文件存储表的数据和索引，而不是将这些数据存储在系统表空间中。这个功能由innodb_file_per_table选项控制。如果innodb_file_per_table选项没有启用，InnoDB表将在系统表空间中创建。File-Per-Table 表空间由一个.ibd数据文件表示，默认情况下该数据文件在数据库目录中创建。File-Per-Table 表空间支持DYNAMIC和COMPRESSED行格式。有关File-Per-Table 表空间的优点，见[15.7.4 InnoDB File-Per-Table Tablespaces](https://dev.mysql.com/doc/refman/8.0/en/innodb-multiple-tablespaces.html)。  

## 4.9 一般表空间
使用CREATE TABLESPACE语法创建InnoDB的共享表空间。一般表空间可以在MySQL数据目录之外创建，一般表空间能够容纳多个表格，并支持所有行格式。使用CREATE TABLE tbl_name ... TABLESPACE [=] tablespace_name或ALTER TABLE tbl_name TABLESPACE [=] tablespace_name语法将表添加到一般表空间中。更多信息，请参阅[15.7.10 InnoDB General Tablespaces](https://dev.mysql.com/doc/refman/8.0/en/general-tablespaces.html)。  

## 4.10 撤销(undo)表空间
撤消表空间包含一个或多个包含撤消日志的文件。innodb_undo_tablespaces(已弃用，将在未来版本中删除)选项定义了InnoDB撤销表空间的数量。更多信息，见：[15.7.8 Configuring Undo Tablespaces](https://dev.mysql.com/doc/refman/8.0/en/innodb-undo-tablespace.html)。  

## 4.11 临时表空间
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

## 4.12 重做日志(Redo Log)
重做日志是在**崩溃恢复**期间使用的基于磁盘的数据结构，用于纠正由不完整事务写入的数据。在正常操作中，重做日志对由SQL语句或低级API调用引起InnoDB表更改的数据进行编码。在服务器初始化过程中，将会做崩溃恢复操作，在这完成之前，不能接受数据库连接。有关重做日志在崩溃恢复中的角色的信息，参见[15.17.2 InnoDB Recovery](https://dev.mysql.com/doc/refman/8.0/en/innodb-recovery.html)。  
默认情况下，重做日志在物理上表现为一组名为**ib_logfile0**和**ib_logfile1**的文件。MySQL以循环方式写入重做日志文件。重做日志中的数据按受影响的记录进行编码;这些数据统称为重做。数据通过重做日志的过程由不断增加的[LSN(log sequence number)](https://dev.mysql.com/doc/refman/8.0/en/glossary.html#glos_lsn)值表示。  
可见：redo log对于事务的实现非常重要，相关的信息，见：  
 - [Section 15.6.1, InnoDB Startup Configuration](https://dev.mysql.com/doc/refman/8.0/en/innodb-init-startup-configuration.html)。  
 - [Section 8.5.4, Optimizing InnoDB Redo Logging](https://dev.mysql.com/doc/refman/8.0/en/optimizing-innodb-logging.html)。  
 - [Section 15.7.2, Changing the Number or Size of InnoDB Redo Log Files](https://dev.mysql.com/doc/refman/8.0/en/innodb-data-log-reconfiguration.html)。  
 - [InnoDB Crash Recovery](https://dev.mysql.com/doc/refman/8.0/en/innodb-recovery.html#innodb-crash-recovery)。  

### 4.12.1 重做日志刷新的组提交
与任何其他ACID兼容的数据库引擎一样，InnoDB在提交之前刷新事务的重做日志。InnoDB使用组提交功能将多个这样的刷新请求组合在一起，以避免每次提交刷新一次磁盘。这可以大量减少IO操作，从而增加吞吐量。有关COMMIT和其他事务操作性能的更多信息，请参阅：[8.5.2 Optimizing InnoDB Transaction Management](https://dev.mysql.com/doc/refman/8.0/en/optimizing-innodb-transaction-management.html)。  
  