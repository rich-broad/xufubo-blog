---
title: InnoDB磁盘结构
---
# 6、InnoDB磁盘结构
本节介绍InnoDB存储引擎体系结构中磁盘中的主要组件。   

## 6.1 表(Tables)
本节介绍与InnoDB表相关的主题。  

### 6.1.1 创建InnoDB表
要创建InnoDB表，请使用CREATE TABLE语句：  
```sql
CREATE TABLE t1 (a INT, b CHAR (20), PRIMARY KEY (a)) ENGINE=InnoDB;
```
如果将InnoDB定义为默认存储引擎（默认情况下为默认存储引擎），则无需指定ENGINE = InnoDB子句。检查默认存储引擎：  
```sql
mysql> SELECT @@default_storage_engine;
+--------------------------+
| @@default_storage_engine |
+--------------------------+
| InnoDB                   |
+--------------------------+
```
如果你计划使用mysqldump或复制来重放默认存储引擎不是InnoDB的服务器上的CREATE TABLE语句，你仍可以使用ENGINE = InnoDB子句。  
可以在system tablespace（系统表空间），file-per-table tablespace（每表一文件表空间）或general tablespace（通用表空间）中创建InnoDB表及其索引。当启用innodb_file_per_table（默认值）时，会在file-per-table tablespace中创建InnoDB表，如果innodb_file_per_table被禁用，则在system tablespace中创建InnoDB表。要在general tablespace中创建表，请使用CREATE TABLE ... TABLESPACE语法。更多信息见[Section 15.6.3.3, “General Tablespaces”](https://dev.mysql.com/doc/refman/8.0/en/general-tablespaces.html).  

默认情况下，在file-per-table tablespace中创建表时，InnoDB会在mysql数据目录中的数据库目录下（一个数据库在mysql数据目录下对应一个目录）创建一个表对应的 .ibd 表空间文件。在InnoDB系统表空间中创建的表在现有的ibdata文件中创建，系统表空间文件驻留在MySQL数据目录中。在通用表空间中创建的表在现有的通用表空间.ibd文件中创建，通用表空间文件可以在MySQL数据目录的内部或外部创建。
例如：  
```shell
[root@localhost xufubo]# ll -h  /usr/local/mysql/data/
total 1.3G
-rw-r-----. 1 mysql mysql   56 Oct 14 15:29 auto.cnf
-rw-r-----. 1 mysql mysql  178 Oct 14 15:32 binlog.000001
-rw-r-----. 1 mysql mysql 1.5K Oct 14 15:45 binlog.000002
-rw-r-----. 1 mysql mysql   32 Oct 14 15:32 binlog.index
-rw-------. 1 mysql mysql 1.7K Oct 14 15:29 ca-key.pem
-rw-r--r--. 1 mysql mysql 1.1K Oct 14 15:29 ca.pem
-rw-r--r--. 1 mysql mysql 1.1K Oct 14 15:29 client-cert.pem
-rw-------. 1 mysql mysql 1.7K Oct 14 15:29 client-key.pem
drwxr-x---. 2 mysql mysql 4.0K Oct 17 15:35 db_tars
drwxr-x---. 2 mysql mysql 4.0K Oct 17 17:16 db_tars_web
-rw-r-----. 1 mysql mysql 3.2K Oct 14 15:58 ib_buffer_pool
-rw-r-----. 1 mysql mysql  12M Nov 27 17:38 ibdata1  # 系统表空间
-rw-r-----. 1 mysql mysql  48M Nov 27 17:38 ib_logfile0
-rw-r-----. 1 mysql mysql  48M Nov 27 12:57 ib_logfile1
-rw-r-----. 1 mysql mysql  12M Oct 30 20:44 ibtmp1
-rw-r-----. 1 mysql mysql  34K Nov 17 12:15 localhost.localdomain.err
-rw-r-----. 1 mysql mysql    7 Oct 14 15:58 localhost.localdomain.pid
drwxr-x---. 2 mysql mysql 4.0K Oct 14 15:29 mysql
-rw-r-----. 1 mysql mysql 1.1G Nov 27 12:09 mysql-bin.000002
-rw-r-----. 1 mysql mysql 6.9M Nov 27 17:38 mysql-bin.000003
-rw-r-----. 1 mysql mysql   38 Nov 27 12:09 mysql-bin.index
-rw-r-----. 1 mysql mysql  88M Nov 27 17:38 mysql.ibd
drwxr-x---. 2 mysql mysql 4.0K Oct 14 15:29 performance_schema
-rw-------. 1 mysql mysql 1.7K Oct 14 15:29 private_key.pem
-rw-r--r--. 1 mysql mysql  452 Oct 14 15:29 public_key.pem
-rw-r--r--. 1 mysql mysql 1.1K Oct 14 15:29 server-cert.pem
-rw-------. 1 mysql mysql 1.7K Oct 14 15:29 server-key.pem
drwxr-x---. 2 mysql mysql 4.0K Oct 14 15:29 sys
drwxr-x---. 2 mysql mysql  52K Nov 27 00:05 tars_property
drwxr-x---. 2 mysql mysql  44K Nov 27 00:05 tars_stat
drwxr-x---. 2 mysql mysql 4.0K Oct 19 14:43 test
-rw-r-----. 1 mysql mysql  12M Nov 27 17:38 undo_001
-rw-r-----. 1 mysql mysql  12M Nov 27 17:38 undo_002
[root@localhost xufubo]# ll -h  /usr/local/mysql/data/db_tars  # 数据库中每个表一个idb文件
total 43M
-rw-r-----. 1 mysql mysql 160K Oct 24 16:33 t_adapter_conf.ibd
-rw-r-----. 1 mysql mysql 112K Oct 17 15:32 t_ats_cases.ibd
-rw-r-----. 1 mysql mysql 128K Oct 17 15:32 t_ats_interfaces.ibd
-rw-r-----. 1 mysql mysql 128K Oct 17 15:32 t_config_files.ibd
-rw-r-----. 1 mysql mysql 112K Oct 17 15:32 t_config_history_files.ibd
-rw-r-----. 1 mysql mysql 128K Oct 17 15:32 t_config_references.ibd
-rw-r-----. 1 mysql mysql 112K Oct 17 15:32 t_group_priority.ibd
-rw-r-----. 1 mysql mysql 144K Oct 17 15:32 t_machine_tars_info.ibd
-rw-r-----. 1 mysql mysql 144K Nov 27 20:47 t_node_info.ibd
-rw-r-----. 1 mysql mysql 128K Oct 17 15:32 t_profile_template.ibd
-rw-r-----. 1 mysql mysql 128K Nov 27 20:47 t_registry_info.ibd
-rw-r-----. 1 mysql mysql 176K Nov 27 20:47 t_server_conf.ibd
-rw-r-----. 1 mysql mysql 128K Oct 17 15:32 t_server_group_relation.ibd
-rw-r-----. 1 mysql mysql 128K Oct 17 15:32 t_server_group_rule.ibd
-rw-r-----. 1 mysql mysql  40M Nov 27 20:47 t_server_notifys.ibd
-rw-r-----. 1 mysql mysql 160K Oct 19 15:26 t_server_patchs.ibd
-rw-r-----. 1 mysql mysql 128K Nov 14 11:57 t_task.ibd
-rw-r-----. 1 mysql mysql 160K Nov 14 11:57 t_task_item.ibd
-rw-r-----. 1 mysql mysql 144K Oct 17 15:32 t_web_release_conf.ibd
[root@localhost xufubo]# 

```
在内部，InnoDB将每个表的条目添加到数据字典中。该条目包括数据库名称。例如，如果在test数据库中创建了表t1，则数据字典条目为“test/t1”，这意味着，在InnoDB中你可以在不同的数据库中创建同名表（t1）而不会发生冲突。  

### 6.1.2 innoDB 表的限制

|InnoDB页大小   | 最大表空间大小  |  
| ------------ | ------------ |  
| 4KB  | 16TB  |  
| 8KB  | 32TB  |  
|16KB   | 64TB  |  
| 32KB  | 128TB  |  
| 64KB  | 256TB  |  
  
最大表空间大小也是表的最大大小。默认页面大小InnoDB为16KB。 

## 6.2 索引(Indexs)
索引是数据库中非常非常重要的概念，它是存储引擎能够快速定位记录的秘密武器，对于提升数据库的性能、减轻数据库服务器的负担有着非常重要的作用；索引优化是对查询性能优化的最有效手段，它能够轻松地将查询的性能提高几个数量级。    

InnoDB 存储引擎中的表都是使用索引组织的，也就是按照primary key或者unique key（not null）的顺序存放；聚集索引就是按照表中主键的顺序构建一颗 B+ 树，并在叶节点中存放表中的**行记录数据**。即：索引组织表。  

InnoDB会首先找到primary key列，如果找到了，会以该列的值排序，以聚集索引的形式维护一颗B+树，如果没找到，会接着找unique key（not null）列，并以该列维护B+树，如果二者都没找到，InnoDB会为每行数据自动分配行ID，并以此构建B+树。我们最好为InnoDB表指定primary key，同时primary key最好是依据插入的顺序递增的，而且很少改变（这会使得InnoDB以很小的代价维护聚集索引，因为插入是按照主键的顺序进行的，而且主键很少更新，这就不会造成B+树的经常调整）。  

InnoDB除了聚集索引外也包含辅助索引，辅助索引也是通过 B+ 树实现的，但是它的叶节点并不包含行记录的全部数据，仅包含索引中的所有键和一个用于查找对应行记录的指针，在 InnoDB 中这个指针就是当前记录的主键。再通过辅助索引查询时，会先查找到主键，再通过主键从聚集索引中查询行记录，即：通过辅助索引查询数据，需要查询两次B+树，这一点与很多存储引擎不同。  

## 6.3 表空间(Tablespaces)

### 6.3.1 系统表空间
InnoDB系统表空间包含InnoDB的数据字典（InnoDB数据对象的元数据），并且也是doublewrite buffer, change buffer, and undo logs的存储区域，系统表空间还包含用户创建的表和索引的数据，因此系统表空间是一个共享表空间，因为它被多个表（包括不同数据库中的表）共享。  
系统表空间由一个或多个数据文件表示。默认情况下，MySQL 在data目录中创建一个名为ibdata1的系统数据文件。系统数据文件的大小和数量由innodb_data_file_path启动选项控制。  

### 6.3.2  File-Per-Table 表空间
File-Per-Table 表空间的意思就是mysql为每一个表创建一个表空间，并使用单独的文件存储表的数据和索引，而不是将这些数据存储在系统表空间中。这个功能由innodb_file_per_table选项控制。如果innodb_file_per_table选项没有启用，InnoDB表将在系统表空间中创建。File-Per-Table 表空间由一个.ibd数据文件表示，默认情况下该数据文件在数据库目录中创建。File-Per-Table 表空间支持DYNAMIC和COMPRESSED行格式。有关File-Per-Table 表空间的优点，见[15.7.4 InnoDB File-Per-Table Tablespaces](https://dev.mysql.com/doc/refman/8.0/en/innodb-multiple-tablespaces.html)。  

### 6.3.3 一般表空间
使用CREATE TABLESPACE语法创建InnoDB的共享表空间。一般表空间可以在MySQL数据目录之外创建，一般表空间能够容纳多个表格，并支持所有行格式。使用CREATE TABLE tbl_name ... TABLESPACE [=] tablespace_name或ALTER TABLE tbl_name TABLESPACE [=] tablespace_name语法将表添加到一般表空间中。更多信息，请参阅[15.7.10 InnoDB General Tablespaces](https://dev.mysql.com/doc/refman/8.0/en/general-tablespaces.html)。  

## 6.3.4 撤销(undo)表空间
撤消表空间包含一个或多个包含撤消日志的文件。innodb_undo_tablespaces(已弃用，将在未来版本中删除)选项定义了InnoDB撤销表空间的数量。更多信息，见：[15.7.8 Configuring Undo Tablespaces](https://dev.mysql.com/doc/refman/8.0/en/innodb-undo-tablespace.html)。  

## 6.3.5 临时表空间
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

## 6.4 双写缓冲(Doublewrite Buffer)
InnoDB中，在将缓冲池中的数据刷新到磁盘时是以页面（InnoDB的页面，通常为16KB）为单位的，这时可能会出现部分页面写入的问题。所谓部分页面写入是指向操作系统提交的页面写入请求仅部分完成。例如，在16K 的Innodb页面中，只有第一个4KB（文件系统的块通常为4KB）的块被写入磁盘，其他部分保持原来的状态。最常见的部分页面写入一般在发生电源故障时发生。也可能发生在操作系统崩溃时。另外，如果使用软件RAID，页面可能会出现在需要多个IO请求的条带边界上。如果硬件RAID没有电池备份，电源故障时也会发生这种情况。如果对磁盘本身发出单个写入，即使电源掉电，在理论上也应完成写入，因为驱动器内部应该有足够的电源来完成它。但是真的很难检查是否总是这样，因为它不是部分页面写入的唯一原因。在Innodb Doublewrite Buffer实施之前，确实会有数据损坏。  
有的人会问，数据损坏可以使用重做日志来恢复呀，但是，请注意，InnoDB并不会将整个页面的内容写入重做日志，而是记录的对页面的操作，例如将某个偏移量处的值加2，使用重做日志进行恢复的基础是表空间中的实际数据页面在内部是完整的一致的，它是哪个页面版本无关紧要 ，但是如果页面不一致，则无法继续恢复，因为你的基础数据就是不一致的。为此引入了Doublewrite Buffer来解决问题。  
理解了为什么需要Doublewrite Buffer，也就不难理解Doublewrite Buffer如何工作了。具体来说就是：你可以将Doublewrite Buffer视为系统表空间中的一个短期日志文件，它包含100个页的空间。当Innodb从Innodb缓冲池中刷新页面时，InnoDB首先会将页面写入双写缓冲区（顺序），然后调用fsync（）以确保它们保存到磁盘，然后将页面写入真正的数据文件并第二次调用fsync（））。现在Innodb恢复的时候会检查表空间中数据页面的内容和Doublewrite Buffer中页面的内容。如果在双写缓冲区中的页面不一致，则简单地丢弃它，如果表空间中的数据页面不一致，则从双写缓冲区中恢复。那么会不会出现都不一致的情况呢？这个不会，以内是先写Doublewrite Buffer，后写表空间中真实的数据页面，这样，当Doublewrite Buffer中不一致时表示系统崩溃了，也就无法继续执行了，就不会收到Doublewrite Buffer是否写成功的响应，也就不会发出真实的数据页面的写操作，这样的话必然不会出现二者都损坏的情况。  
虽然Doublewrite Buffer的加入会使每次刷新数据时写两次磁盘，但是性能不会大幅下降，因为Doublewrite Buffer的写入是顺序的。所以一般来说，由于使用Doublewrite而不会超过5-10％的性能损失。但是数据是无价之宝，比起这个，这点损失可以接受。  
那么Doublewrite是否可以禁用的？默认是开启的，要禁用Doublewrite，可以设置innodb_doublewrite=0。当然了前提是你可以忍受数据丢失。  
如果系统表空间文件（“ ibdata文件 ”）位于支持原子写入的Fusion-io设备上，则自动禁用Doublewrite Buffer，并将Fusion-io原子写入用于所有数据文件。因为双写缓冲区设置是全局的，因此对非Fusion-io硬件上的数据文件也将禁用Doublewrite Buffer。此功能仅在Fusion-io硬件上并且仅在Linux上启用Fusion-io NVMFS下受支持。要充分利用此功能，建议使用innodb_flush_method设置 O_DIRECT。  
因此最好不要禁用Doublewrite Buffer。除非可以忍受数据丢失。   

## 6.5 重做日志(Redo Log)
重做日志是在**崩溃恢复**期间使用的基于磁盘的数据结构，用于纠正由不完整事务写入的数据。在正常操作中，重做日志对由SQL语句或低级API调用引起InnoDB表更改的数据进行编码。在服务器初始化过程中，将会做崩溃恢复操作，在这完成之前，不能接受数据库连接。有关重做日志在崩溃恢复中的角色的信息，参见[15.17.2 InnoDB Recovery](https://dev.mysql.com/doc/refman/8.0/en/innodb-recovery.html)。  
默认情况下，重做日志在物理上表现为一组名为**ib_logfile0**和**ib_logfile1**的文件。MySQL以循环方式写入重做日志文件。重做日志中的数据按受影响的记录进行编码;这些数据统称为重做。数据通过重做日志的过程由不断增加的[LSN(log sequence number)](https://dev.mysql.com/doc/refman/8.0/en/glossary.html#glos_lsn)值表示。  
可见：redo log对于事务的实现非常重要，相关的信息，见：  
 - [Section 15.6.1, InnoDB Startup Configuration](https://dev.mysql.com/doc/refman/8.0/en/innodb-init-startup-configuration.html)。  
 - [Section 8.5.4, Optimizing InnoDB Redo Logging](https://dev.mysql.com/doc/refman/8.0/en/optimizing-innodb-logging.html)。  
 - [Section 15.7.2, Changing the Number or Size of InnoDB Redo Log Files](https://dev.mysql.com/doc/refman/8.0/en/innodb-data-log-reconfiguration.html)。  
 - [InnoDB Crash Recovery](https://dev.mysql.com/doc/refman/8.0/en/innodb-recovery.html#innodb-crash-recovery)。  

### 6.5.1 重做日志刷新的组提交
与任何其他ACID兼容的数据库引擎一样，InnoDB在提交之前刷新事务的重做日志。InnoDB使用组提交功能将多个这样的刷新请求组合在一起，以避免每次提交刷新一次磁盘。这可以大量减少IO操作，从而增加吞吐量。有关COMMIT和其他事务操作性能的更多信息，请参阅：[8.5.2 Optimizing InnoDB Transaction Management](https://dev.mysql.com/doc/refman/8.0/en/optimizing-innodb-transaction-management.html)。  
  

## 6.6 撤销日志(Undo Logs)
撤消日志是与单个事务关联的撤消日志记录的集合，撤销日志记录包含有关如何撤消事务对聚集索引记录的最新更改的信息。如果另一个事务需要查看原始数据（为了实现一致性读），未修改的数据将从撤消日志记录中检索，撤销日志存在于撤消日志段中，撤消日志段包含在回滚段中，回滚段位于撤消表空间和临时表空间中。有关撤消表空间的更多信息，见：[15.7.8 Configuring Undo Tablespaces](https://dev.mysql.com/doc/refman/8.0/en/innodb-undo-tablespace.html)。有关多版本的信息，请参阅[15.3 InnoDB Multi-Versioning](https://dev.mysql.com/doc/refman/8.0/en/innodb-multi-versioning.html)。临时表空间和每个撤消表空间分别支持最多128个回滚段。innodb_rollback_segments配置选项定义了回滚段的数量。每个回滚段最多支持1023个并发的数据修改事务。 