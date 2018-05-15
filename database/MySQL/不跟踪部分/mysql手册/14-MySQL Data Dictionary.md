# MySQL数据字典
MySQL8.0相比于5.7、5.6、5.5、5.1等版本最为中最重要的特性之一就是数据字典。MySQL服务器包含一个存储有关数据库对象信息的**事务**数据字典。在之前的MySQL版本中，字典数据存储在元数据文件，非事务表和存储引擎特定的数据字典中。可以看到，之前的数据库元数据被分散的存储在多个地方，现在被统一存储的数据字典具有如下好处：
 - 统一存储的数据字典模式简单易于管理。
 - 删除基于文件的元数据存储。
 - 字典数据的事务性，使得MySQL更加可靠。
 - 可以对字典对象进行统一和集中式的缓存。
 - 一些INFORMATION_SCHEMA表的更简单和改进的实现。
 - 原子DDL。
 ***重要：与没有数据字典的服务器相比，启用数据字典的服务器在常规的操作上存在一些差异，此外，对于升级到MySQL 8.0，升级过程与以前的MySQL版本有所不同，并要求你通过检查特定先决条件来验证安装的升级准备情况。更多信息见：[Section 2.10.1, “Upgrading MySQL](https://dev.mysql.com/doc/refman/8.0/en/upgrading.html)***
 
 ---
 ## 1、数据字典Schema
 数据字典表受到保护，只能在MySQL的调试版本中访问。但是，MySQL支持通过INFORMATION_SCHEMA表和 SHOW语句访问存储在数据字典表中的数据 。有关构成数据字典的表的概述，请参阅[数据字典表](https://dev.mysql.com/doc/refman/8.0/en/system-database.html#system-database-data-dictionary-tables)。  
MySQL系统表仍然存在于MySQL 8.0中，可以通过如下方式查看：
```sql
use mysql;
show tables;
```
通常，MySQL系统表和数据字典表之间的区别在于系统表包含辅助数据，如时区和帮助信息，而数据字典表包含执行SQL查询所需的数据。MySQL系统表和数据字典表的升级方式也不同。升级MySQL系统表需要运行 mysql_upgrade。数据字典升级由MySQL服务器管理。
***数据字典如何升级***

## 2、删除基于文件的元数据存储


## 3、数据字典存储在事务表中
数据字典存储在事务表(InnoDB表)中。数据字典表与非数据字典系统表一起位于mysql数据库中。数据字典表存储在名为mysql.ibd的单个InnoDB表空间中，它位于MySQL数据目录中。mysql.ibd表空间文件必须位于MySQL数据目录中，并且其名称不能被其他表空间修改或使用。如下：
```
[root@localhost data]# pwd
/usr/local/mysql/data
[root@localhost data]# ll
total 168028
-rw-r-----. 1 mysql mysql       56 Jan 30 06:19 auto.cnf
-rw-r-----. 1 mysql mysql      155 May 14 01:26 binlog.000006
-rw-r-----. 1 mysql mysql      155 May 14 01:26 binlog.000007
-rw-r-----. 1 mysql mysql       32 May 14 01:26 binlog.index
-rw-------. 1 root  root      1675 Jan 30 06:20 ca-key.pem
-rw-r--r--. 1 root  root      1111 Jan 30 06:20 ca.pem
-rw-r--r--. 1 root  root      1111 Jan 30 06:20 client-cert.pem
-rw-------. 1 root  root      1675 Jan 30 06:20 client-key.pem
-rw-r-----. 1 mysql mysql     3496 Mar 11 13:37 ib_buffer_pool
-rw-r-----. 1 mysql mysql 12582912 May 14 01:26 ibdata1
-rw-r-----. 1 mysql mysql 50331648 May 14 01:26 ib_logfile0
-rw-r-----. 1 mysql mysql 50331648 Jan 30 06:19 ib_logfile1
-rw-r-----. 1 mysql mysql 12582912 May 14 22:58 ibtmp1
-rw-r-----. 1 mysql mysql    19337 May 14 01:26 localhost.localdomain.err
-rw-r-----. 1 mysql mysql        5 May 14 01:26 localhost.localdomain.pid
drwxr-x---. 2 mysql mysql     4096 Jan 30 06:19 mysql
-rw-r-----. 1 mysql mysql 25165824 May 14 01:26 mysql.ibd
drwxr-x---. 2 mysql mysql     4096 Jan 30 06:19 performance_schema
-rw-------. 1 root  root      1679 Jan 30 06:20 private_key.pem
-rw-r--r--. 1 root  root       451 Jan 30 06:20 public_key.pem
-rw-r--r--. 1 root  root      1111 Jan 30 06:20 server-cert.pem
-rw-------. 1 root  root      1679 Jan 30 06:20 server-key.pem
drwxr-x---. 2 mysql mysql     4096 Jan 30 06:19 sys
-rw-r-----. 1 mysql mysql 10485760 May 14 01:26 undo_001
-rw-r-----. 1 mysql mysql 10485760 May 14 01:26 undo_002
[root@localhost data]# 
```
字典数据像其他存储在InnoDB中的其他用户数据一样，受 commit, rollback, 和 crash-recovery功能的保护。  

---
## 4、字典对象cache
通常，cache是计算机系统中提高性能的最为实用的技术，mysql也不例外，为了提升数据库系统的性能，mysql对数据字典数据进行了缓存。数据字典对象高速缓存是一个**共享的全局高速缓存**，用于将先前访问的数据字典对象存储在内存中，以启用对象重用并最小化磁盘I/O。类似于MySQL使用的其他缓存机制，字典对象缓存使用基于LRU的淘汰策略从内存中淘汰最少使用的对象。  
数据字典对象的高速缓存依据对象类型，分为多个分区；一些分区的大小是可配的，一些分区的大小是硬编码的。
 - ***表空间定义缓存分区(tablespace definition cache partition)：*** 存储表空间定义对象。tablespace_definition_cache选项表示可存储在字典对象高速缓存中的表空间定义对象的数量限制。默认值是256。
 - ***schema定义缓存分区(schema definition cache partition)：*** 存储schema定义对象。schema_definition_cache选项表示可以存储在字典对象高速缓存中的achema定义对象的数量限制。默认值是256。
 - ***表定义缓存分区(table definition cache partition)：*** 存储表定义对象。对象限制设置为max_connections的值，默认值为151。  
 表定义缓存的分区与使用table_definition_cache配置选项配置的表定义缓存并行存在。这两个缓存都存储表定义，但服务于MySQL服务器的不同部分。一个缓存中的对象不依赖于另一个缓存中对象的存在。
 - ***存储的程序定义缓存分区(stored program definition cache partition)：*** 存储 存储的程序定义对象。stored_program_definition_cache选项设置可存储在字典对象高速缓存中的存储程序定义对象的数量限制。默认值是256。  
 存储的程序定义的高速缓存分区与使用选项stored_program_cache配置的存储过程和函数高速缓存并行存在。stored_program_cache 选项为每个连接缓存的存储过程或函数的数量设置一个软上限，并且连接每次执行存储过程或函数时都会检查该限制。另一方面，存储的程序定义缓存分区是一个共享缓存，用于其他目的。存储的程序定义缓存分区(被stored_program_definition_cache设定)中对象的存在不依赖于存储过程高速缓存或函数高速缓存(被stored_program_cache设定)中对象的存在，反之亦然。
 - ***字符集定义缓存分区(character set definition cache partition)：*** 存储字符集定义对象，具有256的硬编码限制。
 - ***排序规则定义缓存分区(collation definition cache partition)：*** 存储排序规则定义对象，具有256的硬编码限制。  
有关数据字典对象高速缓存配置选项的更多信息，见[服务器系统变量](https://dev.mysql.com/doc/refman/8.0/en/server-system-variables.html)。

---
## 5、INFORMATION_SCHEMA和Data Dictionary的集成
随着数据字典的引入，以下INFORMATION_SCHEMA表被实现为数据字典表上的视图：
 - CHARACTER_SETS
 - COLLATIONS
 - COLLATION_CHARACTER_SET_APPLICABILITY
 - COLUMNS
 - COLUMN_STATISTICS
 - EVENTS
 - FILES
 - INNODB_COLUMNS
 - INNODB_DATAFILES
 - INNODB_FIELDS
 - INNODB_FOREIGN
 - INNODB_FOREIGN_COLS
 - INNODB_INDEXES
 - INNODB_TABLES
 - INNODB_TABLESPACES
 - INNODB_TABLESPACES_BRIEF
 - INNODB_TABLESTATS
 - KEY_COLUMN_USAGE
 - KEYWORDS
 - PARAMETERS
 - PARTITIONS
 - REFERENTIAL_CONSTRAINTS
 - RESOURCE_GROUPS
 - ROUTINES
 - SCHEMATA
 - STATISTICS
 - ST_GEOMETRY_COLUMNS
 - ST_SPATIAL_REFERENCE_SYSTEMS
 - TABLES
 - TABLE_CONSTRAINTS
 - TRIGGERS
 - VIEWS
 有关这些表/视图的详细说明见：[INFORMATION_SCHEMA Tables](https://dev.mysql.com/doc/refman/8.0/en/information-schema.html)。  
现在对这些表的查询效率更高，因为它们从数据字典表中而不是其他较慢的方法获取信息。特别的，他们是数据字典表的视图，具有如下好处：
 - 服务器不再必须为INFORMATION_SCHEMA表的每个查询创建一个临时表。
 - 当底层数据字典表存储以前通过目录扫描获得的值（例如，枚举数据库实例中的数据库名称或表名称）或文件打开操作（例如，从.frm文件读取信息）时，现在这些信息通过查询INFORMATION_SCHEMA表即可（另外，即使对于非视图INFORMATION_SCHEMA表，也可以通过对数据字典的查找来检索数据库和表名等信息，而不需要目录或文件扫描）。
 - 底层数据字典表上的索引允许优化器构造高效的查询执行计划，对于之前的实现来说，对于INFORMATION_SCHEMA表的每个查询使用临时表将无法进行查询优化。
上述优化也适用于通过SHOW语句展示INFORMATION_SCHEMA中属于数据字典表视图的表信息。例如，SHOW DATABASES显示与SCHEMATA表相同的信息。  
除了引入数据字典表的视图之外，现在包含表统计信息的STATISTICS和TABLES也会被缓存，以提高INFORMATION_SCHEMA的查询性能。*information_schema_stats_expiry*系统变量定义了缓存表统计信息的过期时间，默认值为86400秒。要随时更新给定表格的缓存值，请使用ANALYZE TABLE。可以将*information_schema_stats_expiry*设置为0，以使INFORMATION_SCHEMA查询直接从存储引擎中检索最新的统计信息，这不如检索缓存的统计信息那么快。  
更多信息见[Optimizing INFORMATION_SCHEMA Queries](https://dev.mysql.com/doc/refman/8.0/en/information-schema-optimization.html)。

---
## 6、序列化字典信息（SDI）
除了在数据字典中存储关于数据库对象的元数据之外，MySQL还以序列化的形式存储它们。这些数据被称为序列化字典信息（SDI）。 InnoDB将SDI数据存储在其表空间文件中。其他存储引擎将SDI数据存储在schema目录中创建的.sdi文件中。SDI数据以紧凑的JSON格式生成。  
序列化字典信息（SDI）存在于InnoDB除临时表空间和撤消表空间文件之外的所有 表空间文件中。InnoDB表空间文件中的SDI记录 仅描述表空间中包含的表和表空间对象。InnoDB表空间文件中的 SDI数据只能通过表空间内的表上的DDL操作来更新。  
SDI数据的存在提供了元数据的冗余。例如，如果数据字典变得不可用，则可以使用[ibd2sdi](https://dev.mysql.com/doc/refman/8.0/en/ibd2sdi.html)工具直接从InnoDB表空间文件中提取数据库对象的元数据。  
对于InnoDB，SDI记录需要一个索引页面，默认大小为16k。但是，SDI数据被压缩以减少存储空间。  
对于由多个表空间组成的分区InnoDB表，SDI数据存储在第一个分区的表空间文件中。  
MySQL服务器使用在DDL操作期间访问的内部API来创建和维护SDI记录。  
IMPORT TABLE语句根据.sdi文件中包含的信息导入MyISAM表。见[IMPORT TABLE Syntax](https://dev.mysql.com/doc/refman/8.0/en/import-table.html)。

---
## 7、数据字典使用差异
与没有数据字典的服务器相比，使用启用数据字典的MySQL服务器会带来一些操作差异：

---
## 8、数据字典限制
本节介绍MySQL数据字典引入的临时限制。  
 - 在数据目录下手动创建数据库目录（例如，使用mkdir）不受支持。手动创建的数据库目录不被MySQL服务器识别。
 - 由于DDL操作需要写入存储，撤消日志和重做日志而不是.frm文件，因此DDL操作需要更长的时间。