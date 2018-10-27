mysql使用了一系列的内存缓存策略来提高性能。本节对mysql的buffer和cache与优化的关系进行研究学习。  

# 1、InnoDB缓冲池(buffer pool)优化
InnoDB维护一个称为缓冲池的存储区域，用于在内存中缓存数据和索引。了解InnoDB缓冲池如何工作，并利用它来将频繁访问的数据保存在内存中，是MySQL调优的一个重要方面（核心思想是将热点数据全都cache在内存中来增快访问速度）。有关InnoDB缓冲池内部工作原理的说明，见[Section 15.9, “InnoDB Buffer Pool”](https://dev.mysql.com/doc/refman/8.0/en/innodb-buffer-pool.html)。   
更多的bufer pool的调优，可以参考如下章节：  
 - [Section 15.6.3.4, “Configuring InnoDB Buffer Pool Prefetching (Read-Ahead)”](https://dev.mysql.com/doc/refman/8.0/en/innodb-performance-read_ahead.html)  
 - [Section 15.6.3.5, “Configuring InnoDB Buffer Pool Flushing”](https://dev.mysql.com/doc/refman/8.0/en/innodb-performance-adaptive_flushing.html)  
 - [Section 15.6.3.3, “Making the Buffer Pool Scan Resistant”](https://dev.mysql.com/doc/refman/8.0/en/innodb-performance-midpoint_insertion.html)  
 - [Section 15.6.3.2, “Configuring Multiple Buffer Pool Instances”](https://dev.mysql.com/doc/refman/8.0/en/innodb-multiple-buffer-pools.html)  
 - [Section 15.6.3.7, “Saving and Restoring the Buffer Pool State”](https://dev.mysql.com/doc/refman/8.0/en/innodb-preload-buffer-pool.html)  
 - [Section 15.6.3.6, “Fine-tuning InnoDB Buffer Pool Flushing”](https://dev.mysql.com/doc/refman/8.0/en/innodb-lru-background-flushing.html)  
 - [Section 15.6.3.1, “Configuring InnoDB Buffer Pool Size”](https://dev.mysql.com/doc/refman/8.0/en/innodb-buffer-pool-resize.html)  

# 2、MyISAM键缓存
为了最小化磁盘I/O，MyISAM存储引擎也使用许多数据库管理系统使用的策略。它采用缓存机制将最常访问的数据保存在内存中：  
 - 对于索引块，mysql 服务器层会维护称为键缓存的特殊内存结构，该结构包含许多**块缓冲区**，其中存放了最常使用的索引数据块。  
 - 对于数据块，mysql 服务器层不使用特殊缓存。相反，它依赖于本机操作系统文件系统缓存。  

本节先介绍MyISAM键缓存的基本操作。 然后讨论可以提高键缓存性能的操作：  
 - 多个会话可以*同时访问键缓存*。  
 - 可以设置多个密​键缓存并将表索引分配给特定键缓存。  

key_buffer_size系统变量用于控制键缓存的大小，如果改值为0，将不会分配键缓存。如果key_buffer_size值太小而无法分配最小数量（8）的**块缓冲区**，也不使用键缓存。如果键缓存被禁用时，对于MyISAM，mysql仅使用操作系统文件系统的缓存机制来访问索引（和表数据块的机制一样）。*索引块*是访问MyISAM索引文件的连续单元。通常，索引块的大小等于索引B树的节点大小。键缓存结构中的所有**块缓冲区**大小相同。该大小可以等于，大于或小于索引块的大小，通常这两个值中的一个是另一个的数倍。  
当需要访问来自任何表索引块的数据时，服务器首先会检查它在键缓存中是否可用，如果可用，则不需要读写磁盘，否则，服务器需要读写磁盘，并用新的索引块数据替换服务器选择的键缓存中的块缓冲区中的内容。

如果被服务器选中的块缓冲区已被修改，则该块被认为是“脏的”，在这种情况下，需要先将块缓冲区刷新到磁盘对应的索引块中。通常服务器遵循LRU（最近最少使用）策略：当选择要替换的块时，它选择最近最少使用的索引块。InnoDB也使用LRU算法来管理它的buffer pool。  

## 2.1 共享键缓冲区访问
线程可以同时访问 键缓冲区，但是要满足一下条件：  
- 多个会话可以访问未更新的缓冲区。  
- 正在更新的缓冲区会导致需要使用它的会话等待更新完成。  
- 多个会话可以发起导致高速缓存块替换的请求，只要它们不相互干扰。  

对键缓存的共享访问使服务器能够显着提高吞吐量。   

## 2.2 多个键缓存
对键缓存的共享访问可以提高性能，但不会完全消除会话之间的争用。为了进一步减少键缓存的争用mysql提供了多个键缓存。此功能使你可以将不同的表索引分配给不同的键缓存。在多个键缓存的情况下，mysql服务器层需要知道在处理给定MyISAM表的查询时要使用哪个缓存 。默认情况下，所有MyISAM表的索引都缓存在默认的键缓存中。要将表索引分配给特定的键缓存，请使用CACHE INDEX 语句，例如，以下语句将t1，t2以及t3的索引分配给名为hot_cache的键缓存：  
```sql
mysql> CACHE INDEX t1, t2, t3 IN hot_cache;
+---------+--------------------+----------+----------+
| Table   | Op                 | Msg_type | Msg_text |
+---------+--------------------+----------+----------+
| test.t1 | assign_to_keycache | status   | OK       |
| test.t2 | assign_to_keycache | status   | OK       |
| test.t3 | assign_to_keycache | status   | OK       |
+---------+--------------------+----------+----------+
```
CACHE INDEX语句中引用的键缓存可以通过使用SET GLOBAL语句或启动选项设置其大小来。例如：  
```sql
mysql> SET GLOBAL keycache1.key_buffer_size=128*1024;
```
要销毁键缓存，请将其大小设置为零：  
```sql
mysql> SET GLOBAL keycache1.key_buffer_size=0;
```
无法销毁默认键缓存。这样的尝试会被mysql忽略：  
```sql
mysql> SET GLOBAL key_buffer_size = 0;

mysql> SHOW VARIABLES LIKE 'key_buffer_size';
+-----------------+---------+
| Variable_name   | Value   |
+-----------------+---------+
| key_buffer_size | 8384512 |
+-----------------+---------+
```

默认情况下，表索引分配给服务器在启动时创建的主（默认）键缓存，销毁键缓存时，mysql将分配给它的所有表索引重新分配给默认键缓存。对于繁忙的服务器，你可以使用涉及三个键缓存的策略：  
 - 将所有键缓存空间的20％用于热键缓存，这些缓存用于大量查询但是不更新的表。  
 - 将所有键缓存空间的20％用于冷键缓存，将此缓存用于中型，密集修改的表，例如临时表。  
 - 将所有键缓存空间的60％用于温(warm)键缓存，将此作为默认键缓存，默认情况下用于所有其他表。  

使用三个键缓存有益的一个原因是对一个键缓存结构的访问不会阻止对其他键缓存的访问。  

CACHE INDEX语句在表和键缓存之间建立关联，**但每次服务器重新启动时关联都会丢失**。如果你希望每次服务器启动时关联生效，可以使用选项文件：这些配置项包含 *键缓存的配置* 和 一个用于指定要执行CACHE INDEX语句文件的init-file选项。例如：  
```conf
key_buffer_size = 4G
hot_cache.key_buffer_size = 2G
cold_cache.key_buffer_size = 2G
init_file=/path/to/data-directory/mysqld_init.sql
```
这样，每次服务器启动时都会执行mysqld_init.sql中的语句。mysqld_init.sql文件内容大概如下：  
```sql
CACHE INDEX db1.t1, db1.t2, db2.t3 IN hot_cache
CACHE INDEX db1.t4, db2.t5, db2.t6 IN cold_cache
```

## 2.3 中点插入策略
默认情况下，键缓存使用简单的LRU策略来选择则要替换的块缓冲区，除此之外，mysql还支持称为*中点插入策略*的复杂方法。故名思议：中点插入策略将LRU链非为两部分：hot list和warm list。这两部分的划分占比是可配的。不过，键缓存管理系统保证warm不会太短，**始终至少**包含键缓存块的百分之key_cache_division_limit。key_cache_division_limit是结构化键缓存变量的一个组件，因此其值是可以为每个缓存设置的参数。例如：  
```conf
key_cache_division_limit = 50
hot_cache.key_cache_division_limit = 40
```

当索引块从表中读入键缓存时，它被放在warm list的末尾，经过一定次数的访问，它被提升到hot list。现在对于所有的索引块这个命中次数是相同的（3次）。升级到hot list的块被放在列表的末尾。然后该块在这个hot list中循环。如果块在hot list的头部停留足够长的时间，这个块会被降级到warm list的头部。接下来可能被替换掉。时间由键缓存配置项key_cache_age_threshold的值确定。具体算法如下：  
对于包含N块的键缓存，如果最近的 N * key_cache_age_threshold / 100 次都没访问到hot list开头的块，那么这个块将被移到warm list的头部。**然后它成为第一个被替换的候选者，因为替换的块总是从warm list的开头获取。**如果你觉得不需要使用该策略，请将key_cache_division_limit值设置为其默认值100。  

这个可以称其为改进的LRU，因为单纯的LRU仅仅是最近最少使用，这个不仅考虑了时间上是否使用，还考虑了使用是否频繁。即：最近使用的块不一定就是热点，有可能就是最近用了一次，然后再也不用了，那么，单纯的LRU就认为这个块是热数据，而中点插入策略就不会这么认为，显然是合理的。对于索引扫描而言，中点插入策略是好的，这时要设置key_cache_division_limit远小于100，这时，hot list会比较大，在索引扫描操作期间，有价值的频繁命中的索引块将保留在热子列表中。  

## 2.4 索引预加载
如果键缓存中有足够的块来保存整个索引的块，或者至少是与其非叶节点对应的块，则在使用之前可以使用索引预加载。预加载使你能够以最有效的方式将表索引块放入键缓存缓冲区，即：通过顺序从磁盘读取索引块。  
在没有预加载的情况下，索引块仍然根据查询的需要放入键缓存中。虽然这些块将保留在缓存中，但是它们是以随机顺序从磁盘中提取的，而不是按顺序提取的。  
要将索引预加载到缓存中，请使用LOAD INDEX INTO CACHE语句：  
```sql
mysql> LOAD INDEX INTO CACHE t1, t2 IGNORE LEAVES;
+---------+--------------+----------+----------+
| Table   | Op           | Msg_type | Msg_text |
+---------+--------------+----------+----------+
| test.t1 | preload_keys | status   | OK       |
| test.t2 | preload_keys | status   | OK       |
+---------+--------------+----------+----------+
```
IGNORE LEAVES修饰符表示仅预加载索引的非叶节点的块。因此，t1将加载所有索引块，t2仅加载非叶节点块。如果已使用CACHE INDEX语句将索引分配给特定的键缓存，那么，预加载应该将索引块放入该缓存中。否则，索引将加载到默认键缓存中。

## 2.5 键缓存块大小
可以使用key_cache_block_size变量为单个键缓存指定块缓冲区的大小。这种手段可以用来调整索引文件的I/O操作的性能。当读缓冲区的大小等于本机操作系统I/O缓冲区的大小时，可以实现I/O操作的最佳性能。但是，将索引节点的大小设置为I/O缓冲区的大小并不总能确保最佳的整体性能。在读取大叶子节点时，服务器会读取大量不必要的数据，有效地阻止了读取其他叶子节点。要控制MyISAM表的.MYI索引文件中块的大小，请在服务器启动时使用--myisam-block-size选项。  
例如：   
```sql
key_cache_block_size = 4K
```
## 2.6 重构键缓存
可以通过更新键缓存的参数值来随时重建键缓存。例如：  
```sql
mysql> SET GLOBAL cold_cache.key_buffer_size=4*1024*1024;
```
如果为key_buffer_size或 key_cache_block_size设置的值与当前值不同，服务器会销毁现有键缓存并进行重建。如果键缓存包含任何脏块，则服务器会在销毁和重建键缓存之前将它们刷新到磁盘。如果更改其他键缓存参数，则不会进行重建。  

# 3 缓存Prepared Statements 和 Stored Programs