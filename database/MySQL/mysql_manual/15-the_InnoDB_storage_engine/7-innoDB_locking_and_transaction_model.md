# 5 InnoDB锁和事务模型
## 5.1 InnoDB锁
本节学习InnoDB使用的锁。  
 - Shared and Exclusive Locks: 共享和独占锁；  
 - Intention Locks: 意向锁；  
 - Record Locks: 记录锁；  
 - Gap Locks: 间隙锁;  
 - Next-Key Locks
 - Insert Intention Locks: 插入意向锁；  
 - AUTO-INC Locks: 自增锁；  
 - Predicate Locks for Spatial Indexes: 空间索引的谓词锁。  

### Shared and Exclusive Locks
InnoDB实现标准的行级锁定，有两种类型的锁，共享（S）锁和独占（X）锁。  
 - 共享（S）锁允许持有锁的事务读取行。  
 - 独占（X）锁允许持有锁的事务更新或删除行。  
如果事务T1在行r上保持共享（S）锁，则来自不同事务T2的对行r的锁请求按如下方式处理：  
 - 以立即授予T2对S锁的请求。结果，T1和T2都在r上保持S锁定。  
 - T2的X锁定请求不能立即授予。  
如果事务T1在行r上保持独占（X）锁，则来自某个不同事务T2的对r上任一类型的锁的请求将被阻塞。相反，事务T2必须等待事务T1释放其对行r的锁定。  

### Intention Locks
InnoDB支持多种粒度的锁定，允许行锁和表锁共存。例如，LOCK TABLES ... WRITE等语句在指定的表上采用独占锁（X锁）。为了实现多个粒度级别的锁定，InnoDB使用意向锁。意向锁是表级锁，它指示事务稍后对表中的行所需的锁（共享或独占）类型。意向锁有两种类型：  
 - 


### Record Locks
记录锁是对索引记录的锁定。例如，SELECT c1 FROM t WHERE c1 = 10 FOR UPDATE;防止任何其他事务插入，更新或删除t.c1的值为10的行。  
即使定义了没有索引的表，记录锁也始终锁定索引记录。对于这种情况，InnoDB创建一个隐藏的聚簇索引并使用此索引进行记录锁定。
记录锁的事务数据在SHOW ENGINE INNODB STATUS和InnoDB监视器输出中显示的内容大致如下：  
```
RECORD LOCKS space id 58 page no 3 n bits 72 index `PRIMARY` of table `test`.`t` 
trx id 10078 lock_mode X locks rec but not gap
Record lock, heap no 2 PHYSICAL RECORD: n_fields 3; compact format; info bits 0
 0: len 4; hex 8000000a; asc     ;;
 1: len 6; hex 00000000274f; asc     'O;;
 2: len 7; hex b60000019d0110; asc        ;;
```

### Gap Locks
间隙锁用于锁定索引记录之间的间隙，或在第一个或最后一个索引记录之前锁定间隙。例如：SELECT c1 FROM t WHERE c1 BETWEEN 10 and 20 FOR UPDATE。将会阻止其他事务将c1为15的值插入表中。  

间隙可能跨越单个索引值，多个索引值，甚至可能为空。间隙锁是在性能和并发之间的一种权衡，用于某些特定的事务隔离级别而不是所有的隔离级别。  

使用唯一索引锁定行以搜索唯一行的语句不需要间隙锁定（这不包括搜索条件仅包括**多列唯一索引**的一些列的情况;在这种情况下，确实会发生间隙锁定，因为唯一索引为多列，你查询的条件包含比如一列，可能存在多条记录，也就可能有间隙锁）。例如，如果id列具有唯一索引，以下语句仅对id值为100的行使用索引记录锁定，并且其他会话是否在前一个间隙中插入行无关紧要：  
```sql
SELECT * FROM child WHERE id = 100;
```
如果id未编入索引或具有非唯一索引，则该语句会锁定前一个间隙。   

### AUTO-INC Locks
AUTO-INC锁是由插入到具有AUTO_INCREMENT列的表中的事务所采用的特殊表级锁。在最简单的情况下，如果一个事务将值插入表中，任何其他事务插入数据到该表时必须等待，以便保证主键连续唯一。  

innodb_autoinc_lock_mode配置用来项控制用于auto-increment locking的算法


### 5.2 InnoDB事务模型
在InnoDB事务中，目标是将多版本技术和传统的两阶段锁结合起来，实现的事务系统。默认情况下，InnoDB为行级锁，并且通过多版本技术提供了非锁定情况下的一致性读。InnoDB中的锁信息以节省空间的方式存储，因此不需要锁升级(***这句话有些没理解***)。通常，允许多个用户锁定InnoDB表中的每一行，或任何行的随机子集，而不会导致InnoDB内存耗尽。  

#### 5.2.1 事务隔离级别



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
 - 一旦插入很少改变的值（这是因为InnoDB将数组组织成索引组织表，即：主键索引树中也包含行记录）。  

**查看InnoDB表属性**  
要查看InnoDB表的属性，请使用SHOW TABLE STATUS 语句：  
```sql
mysql> SHOW TABLE STATUS FROM test LIKE 't%' \G;
*************************** 1. row ***************************
           Name: t1
         Engine: InnoDB
        Version: 10
     Row_format: Compact
           Rows: 0
 Avg_row_length: 0
    Data_length: 16384
Max_data_length: 0
   Index_length: 0
      Data_free: 0
 Auto_increment: NULL
    Create_time: 2015-03-16 15:13:31
    Update_time: NULL
     Check_time: NULL
      Collation: utf8mb4_0900_ai_ci
       Checksum: NULL
 Create_options:
        Comment:
```
也可以使用Information Schema系统表查询表属性：  
```sql
mysql> SELECT * FROM INFORMATION_SCHEMA.INNODB_TABLES WHERE NAME='test/t1' \G
*************************** 1. row ***************************
     TABLE_ID: 45
         NAME: test/t1
         FLAG: 1
       N_COLS: 5
        SPACE: 35
   ROW_FORMAT: Compact
ZIP_PAGE_SIZE: 0
   SPACE_TYPE: Single
```

#### 8.1.2 InnoDB表的物理行结构
InnoDB表的物理行结构取决于创建表时指定的物理行格式。如果未指定，使用默认行格式，默认行格式由innodb_default_row_format 配置选项定义 ，其默认值为 DYNAMIC。  
接下来描述InnoDB各种行格式的信息：  
 - 确定InnoDB表的行格式  
 - Redundant行格式的特征
 - COMPACT行格式的特征
 - DYNAMIC 和 COMPRESSED 行格式的特征
有关InnoDB行格式的更多信息，见 [Section 15.10 InnoDB Row Storage and Row Formats](https://dev.mysql.com/doc/refman/8.0/en/innodb-row-format.html)。   

**确定InnoDB表的行格式**   

``sql
mysql> SHOW TABLE STATUS IN test1\G;
-- 或者
SELECT NAME, ROW_FORMAT FROM INFORMATION_SCHEMA.INNODB_TABLES WHERE NAME='test1/t1';
```

**Redundant行格式的特点**    
REDUNDANT格式可用于保持与旧版MySQL的兼容性。使用REDUNDANT行格式的表具有如下特点：  
 - 每个索引记录包含一个6字节的标头。标头用于将连续记录链接在一起，也用于行级锁定。  
 - 聚簇索引中的记录包含用户定义的所有列。此外，还有一个6字节的事务ID字段和一个7字节的滚动指针字段。  
 - 如果没有为表定义主键，则每个聚簇索引记录还包含一个6字节的行ID字段。  
 - 

**COMPACT行格式的特征**   
与REDUNDANT格式相比，COMPACT行格式减少了大约20％的行存储空间，但代价是增加了某些操作的CPU使用。如果你的工作负载是一个受缓存命中率和磁盘速度限制的典型工作负载（受限于内存大小和IO），COMPACT格式可能更快（因为COMPACT格式会增加CPU消耗）。如果工作负载受限于CPU，则COMPACT格式会更慢。COMPACT行格式的特点如下：  
 - 

**DYNAMIC 和 COMPRESSED 行格式的特征**  
DYNAMIC和COMPRESSED行格式是COMPACT行格式的变体。有关这些行格式的信息，请参阅[15.10.3 DYNAMIC and COMPRESSED Row Formats](https://dev.mysql.com/doc/refman/8.0/en/innodb-row-format-dynamic.html)。  

#### 8.1.3 移动或复制InnoDB表
