
## 3、InnoDB多版本
InnoDB是一个[多版本存储引擎](https://dev.mysql.com/doc/refman/8.0/en/glossary.html#glos_mvcc),通过多版本技术，可以提高数据库的并发能力。为了支持事务功能，如并发和回滚，它会保存有关已更改行的旧版本。这些信息被存储在表空间中被称为[回滚段(rollback segment)](https://dev.mysql.com/doc/refman/8.0/en/glossary.html#glos_rollback_segment)的数据结构中。InnoDB 使用回滚段中的信息执行事务回滚中所需的撤消操作。它还使用这些信息构建一个行的早期版本以保证[一致性读取(consistent read)](https://dev.mysql.com/doc/refman/8.0/en/glossary.html#glos_consistent_read)。
在内部，InnoDB为存储在数据库中的每一行添加三个字段：
 - DB_TRX_ID：6字节，表示插入或更新行的最后一个事务的事务标识符。
 - DB_ROLL_PTR：
 - DB_ROW_ID：
在回滚段中的undo logs(存储在回滚段这种数据结构中)被分为插入和更新undo logs。


#
## 5 InnoDB锁和事务模型
### 5.1 InnoDB锁


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
