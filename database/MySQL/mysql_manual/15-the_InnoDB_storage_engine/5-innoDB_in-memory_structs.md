---
title: InnoDB内存结构
---
# 5、InnoDB内存结构
本节介绍InnoDB存储引擎体系结构中内存中的主要组件。  

## 5.1 缓冲池(buffer pool)
缓冲池是内存中的一个缓冲区， InnoDB将表和索引的热点数据缓冲在这里，这样，当访问数据库数据时，就可以直接访问内存中的数据，而无需访问磁盘，这就减少了IO，提高了速度。通常，在专用的数据库服务器上，通常将多达80％的物理内存分配给InnoDB缓冲池。为了实现大容量内存操作的效率，缓冲池被组织为可以容纳多行数据的页面的链表，链表使用改进的LRU算法维护。了解如何利用缓冲池将频繁访问的数据保存在内存中是MySQL调优的一个重要方面。  

#### 缓冲池LRU算法
使用最近最少使用（LRU）算法的变体将缓冲池作为页面列表进行管理。当需要空间将新页面添加到缓冲池时，最近最少使用的页面被逐出（如果为脏页，则需要刷新到磁盘），将新页面添加到列表的中间。这种中点插入策略将列表视为两个子列表：  
 - 在头部，是最近访问过的新（“年轻”）页面子列表。  
 - 在尾部，是最近访问的旧页面子列表（替换的对象）。  

如下：  
![buffer pool list](../image/innodb-buffer-pool-list.png)  

该算法将查询中大量使用的页面维护在新页面子列表中。旧页面子列表包含较少使用的页面;这些页面将来是被逐出的对象。默认情况下，算法如下运行：  
 - 3/8的缓冲池专用于旧页面子列表。  
 - 列表的中点是新子列表的尾部与旧子列表的头部相交的边界。  
 - 当InnoDB将页面读入缓冲池时，它最初将其插入中点（旧子列表的头部，其实就是访问一次并不代表一定就是热数据，放在这里其实就是说明可能是热数据，如果接下来又有几次访问，那无疑是热数据了）。
 - 访问旧子列表中的页面会使其变“年轻”，会将其移动到缓冲池的头部（新子列表的头部）。如果因为需要而读入页面，则会立即进行第一次访问，并使页面变得年轻.如果由于预读而读入了页面，则第一次访问不会立即发生（并且在页面被逐出之前可能根本不会发生）。  
 - 当数据库运行时，缓冲池中的页面通过移动到列表的尾部将不会被“访问”。  

默认情况下，查询读取的页面会立即移动到新的子列表中，这意味着它们会在缓冲池中停留更长时间。表扫描（例如为mysqldump操作执行，或者没有WHERE子句的SELECT语句）可以将大量数据带入缓冲池并驱逐等量的旧数据，即使这些新数据再也不会使用。由预读后台线程加载然后仅访问一次的页面移动到新列表的头部与此类似。这些情况反而将经常使用的页面推送到了旧的子列表中，在那里它们会被驱逐，缓存命中率会降低，性能会降低。有关优化此行为的信息，请参阅：[Section 15.8.3.3, “Making the Buffer Pool Scan Resistant”](https://dev.mysql.com/doc/refman/8.0/en/innodb-performance-midpoint_insertion.html), [Section 15.8.3.4, “Configuring InnoDB Buffer Pool Prefetching (Read-Ahead)”](https://dev.mysql.com/doc/refman/8.0/en/innodb-performance-read_ahead.html)。  

InnoDB标准监视器输出包含BUFFER POOL和MEMORY部分中有关缓冲池LRU算法操作的几个字段。有关详情，请参阅[](https://dev.mysql.com/doc/refman/8.0/en/innodb-buffer-pool.html#innodb-buffer-pool-monitoring)。  

#### 缓冲池的配置
您可以配置缓冲池的各个方面以提高性能。  
 - 理想情况下，缓冲池越大越好。详见[Section 15.8.3.1, “Configuring InnoDB Buffer Pool Size”](https://dev.mysql.com/doc/refman/8.0/en/innodb-buffer-pool-resize.html)    
 - 在具有足够内存的64位系统上，建议配置多个缓冲池实例，以最大限度地减少并发操作中内存结构的争用。详见[15.8.3.2 Configuring Multiple Buffer Pool Instances](https://dev.mysql.com/doc/refman/8.0/en/innodb-multiple-buffer-pools.html)  
 - 可以通过配置缓冲池，避免一次突然的访问高峰将现有的热点数据逐出（例如表扫描，可能很多数据只访问一次再也不会访问，这种数据就不应该保留在缓冲池中），更多信息见[Section 15.8.3.3, “Making the Buffer Pool Scan Resistant”](https://dev.mysql.com/doc/refman/8.0/en/innodb-performance-midpoint_insertion.html)  
 - 可以控制何时以及如何执行预读请求，以异步方式将预期很快就会访问的页面预取到缓冲池中。更多信息见：[Section 15.8.3.4, “Configuring InnoDB Buffer Pool Prefetching (Read-Ahead)”](https://dev.mysql.com/doc/refman/8.0/en/innodb-performance-read_ahead.html)  
 - 可以控制何时发生后台刷新以及是否根据工作负载动态调整刷新频率。更多信息见：[Section 15.8.3.5, “Configuring InnoDB Buffer Pool Flushing”](https://dev.mysql.com/doc/refman/8.0/en/innodb-performance-adaptive_flushing.html)  
 - 可以微调缓冲池刷新行为的各个方面以提高性能。更多信息见：[Section 15.8.3.6, “Fine-tuning InnoDB Buffer Pool Flushing”](https://dev.mysql.com/doc/refman/8.0/en/innodb-lru-background-flushing.html)  
 - 可以配置InnoDB如何保留当前缓冲池状态，以避免服务器重新启动后的长时间预热。更多信息见：[Section 15.8.3.7, “Saving and Restoring the Buffer Pool State”](https://dev.mysql.com/doc/refman/8.0/en/innodb-preload-buffer-pool.html)  

#### 使用InnoDB标准监视器监视缓冲池
可以使用SHOW ENGINE INNODB STATUS访问InnoDB标准监视器的输出，输出会提供有关缓冲池操作的指标。缓冲池指标位于InnoDB标准监视器输出的BUFFER POOL AND MEMORY部分，显示类似于以下内容：  
```sql
mysql> SHOW ENGINE INNODB STATUS \G;
*************************** 1. row ***************************
  Type: InnoDB
  Name: 
Status: 
=====================================
2018-11-20 19:45:10 0x7f112238a700 INNODB MONITOR OUTPUT
=====================================
Per second averages calculated from the last 41 seconds
-----------------
BACKGROUND THREAD
-----------------
srv_master_thread loops: 677997 srv_active, 0 srv_shutdown, 1857554 srv_idle
......
6770455 log i/o's done, 3.51 log i/o's/second
----------------------
BUFFER POOL AND MEMORY
----------------------
Total large memory allocated 137428992
Dictionary memory allocated 25331364
Buffer pool size   8192
Free buffers       1024
Database pages     7106
Old database pages 2603
Modified db pages  9
Pending reads      0
Pending writes: LRU 0, flush list 0, single page 0
Pages made young 2539, not young 69298
0.00 youngs/s, 0.00 non-youngs/s
Pages read 2918, created 39878, written 9717963
0.00 reads/s, 0.02 creates/s, 5.15 writes/s
Buffer pool hit rate 1000 / 1000, young-making rate 0 / 1000 not 0 / 1000
Pages read ahead 0.00/s, evicted without access 0.00/s, Random read ahead 0.00/s
LRU len: 7106, unzip_LRU len: 0
I/O sum[231]:cur[0], unzip sum[0]:cur[0]
--------------
ROW OPERATIONS
--------------
0 queries inside InnoDB, 0 queries in queue
......
----------------------------
END OF INNODB MONITOR OUTPUT
============================
```
**注意：InnoDB标准监视器输出中提供的每秒平均值基于自上次打印InnoDB标准监视器输出以来经过的时间。**   
下表描述了InnoDB Standard Monitor报告的缓冲池指标。  
*Table 15.2 InnoDB Buffer Pool Metrics(度量)*  

|名字|描述|  
|-|-|  
|Total memory allocated|为缓冲池分配的总内存（以字节为单位）|  
|Dictionary memory allocated|为InnoDB数据字典分配的总内存（以字节为单位）|  
|Buffer pool size|分配给缓冲池的页面的总大小（页面数）|  
|Free buffers|缓冲池空闲列表的页面总数|  
|Database pages|缓冲池LRU列表的页面总数|  
|Old database pages|缓冲池旧LRU子列表的页面总数|  
|Modified db pages|缓冲池中当前修改的页数（脏页，刷新之后就是0）|  
|Pending reads|等待读入缓冲池的缓冲池页数。|  
|Pending writes LRU|要从LRU列表底部写入磁盘的缓冲池中的旧脏页数|  
|Pending writes flush list|在检查点期间要刷新的缓冲池页数|  
|Pending writes single page|缓冲池中挂起的独立页面写入次数|  
|Pages made young|缓冲池LRU列表中年轻的总页数（位于新子列表）|  
|Pages made not young|缓冲池LRU列表中老的总页数（位于旧子列表）|  
|youngs/s|缓冲池的LRU列表中导致页面变年轻的每秒访问旧页面的平均值。有关详细信息，请参阅此表后面的注释。|  
|non-youngs/s|缓冲池的LRU列表中不会导致页面变年轻的每秒访问旧页面的平均值。有关详细信息，请参阅此表后面的注释。|  
|Pages read|从缓冲池中读取的总页数|  
|Pages created|缓冲池中创建的总页数|  
|Pages written|从缓冲池写入磁盘的总页数。|  
|reads/s|缓冲池页面每秒读取次数的平均值|  
|creates/s|每秒创建的缓冲池页面的平均数|  
|writes/s|缓冲池页面每秒被写入磁盘次数的平均值|  
|Buffer pool hit rate|从缓冲池内存中读取的页面与从磁盘存储中读取的页面的缓冲池页面命中率|  
|young-making rate|导致页面变年轻的页面访问的平均命中率。有关详细信息，请参阅此表后面的注释|  
|not (young-making rate)|没有导致页面变年轻的页面访问的平均命中率。有关详细信息，请参阅此表后面的注释|  
|Pages read ahead|每秒预读操作的平均值|  
|Pages evicted without access|在没有从缓冲池访问的情况下被逐出的页面的每秒平均值|  
|Random read ahead|随机预读操作的每秒平均值。|  
|LRU len|缓冲池LRU列表的页面总数|  
|unzip_LRU len|缓冲池unzip_LRU列表的页面总数|  
|I/O sum|最近50秒访问的缓冲池LRU列表页面总数|  
|I/O cur|当前访问的缓冲池LRU列表页面的总数|  
|I/O unzip sum|最近50秒访问的缓冲池unzip_LRU列表页面总数|  
|I/O unzip cur|当前访问的缓冲池unzip_LRU列表页面的总数|  
  
**注释：**  
 - youngs/s仅适用于旧页面。它基于页面访问次数而不是页数。可以对给定页面进行多次访问，所有这些访问都被计算在内。如果在没有发生大型扫描时看到非常低的youngs / s值，则可能需要减少延迟时间或增加用于旧子列表的缓冲池的百分比。因为增加百分比使旧的子列表变大，因此该子列表中的页面需要更长的时间才能移动到尾部，这增加了再次访问这些页面并使其变得年轻的可能性。  
 - non-youngs/s仅适用于旧页面。它基于页面访问次数而不是页数。可以对给定页面进行多次访问，所有这些访问都被计算在内。如果在执行大型表扫描时没有看到更高的non-youngs/s值（以及更高的youngs/s值），请增加延迟时间。
 - young-making rate 统计对所有的缓冲池页面的访问（新和旧），young-making rate和not (young-making rate)加起来通常小于缓冲池整体命中率（Buffer pool hit rate）。因为旧子列表中的页面命中会导致页面移动到新的子列表。但是，新子列表中的页面命中会导致页面仅在距离头部一定距离时才移动到列表的头部。  
 - not (young-making rate) 是由于innodb_old_blocks_time定义的延迟未被满足而导致页面访问未导致页面变年轻的平均命中率，或者由于新子列表中页面的访问不会导致页面移动到头部的命中率。该指标也统计对所有的缓冲池页面的访问（新和旧）。  

 缓冲池服务器状态变量和INNODB_BUFFER_POOL_STATS表也提供InnoDB Standard Monitor输出的很多指标，详见[Example 15.10, “Querying the INNODB_BUFFER_POOL_STATS Table”](https://dev.mysql.com/doc/refman/8.0/en/innodb-information-schema-buffer-pool-tables.html#innodb-information-schema-buffer-pool-stats-example)。  

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
