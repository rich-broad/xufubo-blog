# INFORMATION_SCHEMA的表
INFORMATION_SCHEMA提供对数据库元数据的访问 ，这些元数据又称为数据字典或者系统目录。INFORMATION_SCHEMA是每个MySQL实例中的一个数据库，它存储有关MySQL服务器维护的所有其他数据库对象的信息。INFORMATION_SCHEMA数据库包含多个只读表。它们实际上是视图，而不是基表，因此没有与它们关联的文件，并且也无法在它们上设置触发器。此外，没有该名称的数据库目录。INFORMATION_SCHEMA中的表（视图）只能select，而不能insert、update、delete语句。  
INFORMATION_SCHEMA也是show语句的替代方案，使用INFORMATION_SCHEMA替代show具有如下优点：  
 - 它符合Codd的规则，因为所有的访问都是在表上完成的。  
 - 你可以使用熟悉的SELECT语句语法，只需要学习一些表名和列名。  
 - 实现者无需担心添加关键字。（这个比较好，只需要学习表和列的定义就可以了）  
 - 你可以在select时对结果进行筛选、排序等，通过你想要的形式查询。  
 - 这种技术与其他数据库系统更具互操作性。  
每个MySQL用户都有权访问这些表，但只能看到表中与用户具有适当访问权限的对象相对应的行。  
INFORMATION_SCHEMA从多个数据库搜索信息的查询可能需要很长时间并影响性能。这一点要注意。  
现在我们看看mysql8.0.11的INFORMATION_SCHEMA数据库中都有哪些表。  
```sql
use information_schema;
show tables;
+---------------------------------------+
| Tables_in_information_schema          |
+---------------------------------------+
| CHARACTER_SETS                        |
| COLLATIONS                            |
| COLLATION_CHARACTER_SET_APPLICABILITY |
| COLUMNS                               |
| COLUMN_PRIVILEGES                     |
| COLUMN_STATISTICS                     |
| ENGINES                               |
| EVENTS                                |
| FILES                                 |
| INNODB_BUFFER_PAGE                    |
| INNODB_BUFFER_PAGE_LRU                |
| INNODB_BUFFER_POOL_STATS              |
| INNODB_CACHED_INDEXES                 |
| INNODB_CMP                            |
| INNODB_CMPMEM                         |
| INNODB_CMPMEM_RESET                   |
| INNODB_CMP_PER_INDEX                  |
| INNODB_CMP_PER_INDEX_RESET            |
| INNODB_CMP_RESET                      |
| INNODB_COLUMNS                        |
| INNODB_DATAFILES                      |
| INNODB_FIELDS                         |
| INNODB_FOREIGN                        |
| INNODB_FOREIGN_COLS                   |
| INNODB_FT_BEING_DELETED               |
| INNODB_FT_CONFIG                      |
| INNODB_FT_DEFAULT_STOPWORD            |
| INNODB_FT_DELETED                     |
| INNODB_FT_INDEX_CACHE                 |
| INNODB_FT_INDEX_TABLE                 |
| INNODB_INDEXES                        |
| INNODB_METRICS                        |
| INNODB_TABLES                         |
| INNODB_TABLESPACES                    |
| INNODB_TABLESPACES_BRIEF              |
| INNODB_TABLESTATS                     |
| INNODB_TEMP_TABLE_INFO                |
| INNODB_TRX                            |
| INNODB_VIRTUAL                        |
| KEYWORDS                              |
| KEY_COLUMN_USAGE                      |
| OPTIMIZER_TRACE                       |
| PARAMETERS                            |
| PARTITIONS                            |
| PLUGINS                               |
| PROCESSLIST                           |
| PROFILING                             |
| REFERENTIAL_CONSTRAINTS               |
| RESOURCE_GROUPS                       |
| ROUTINES                              |
| SCHEMATA                              |
| SCHEMA_PRIVILEGES                     |
| STATISTICS                            |
| ST_GEOMETRY_COLUMNS                   |
| ST_SPATIAL_REFERENCE_SYSTEMS          |
| TABLES                                |
| TABLESPACES                           |
| TABLE_CONSTRAINTS                     |
| TABLE_PRIVILEGES                      |
| TRIGGERS                              |
| USER_PRIVILEGES                       |
| VIEWS                                 |
+---------------------------------------+
62 rows in set (0.08 sec)
```

## 1、CHARACTER_SETS
CHARACTER_SETS表提供可用字符集的信息。以下语句是等价的：  
```sql
SELECT * FROM INFORMATION_SCHEMA.CHARACTER_SETS
  [WHERE CHARACTER_SET_NAME LIKE 'wild']

SHOW CHARACTER SET
  [LIKE 'wild']
```

## 2、COLLATIONS
COLLATIONS表提供了有关每个字符集的排序规则的信息。以下语句是等价的：  
```sql
SELECT COLLATION_NAME FROM INFORMATION_SCHEMA.COLLATIONS
  [WHERE COLLATION_NAME LIKE 'wild']

SHOW COLLATION
  [LIKE 'wild']
```

## 3、COLLATION_CHARACTER_SET_APPLICABILITY
COLLATION_CHARACTER_SET_APPLICABILITY表指示什么字符集适用于什么排序规则。这些列等同于我们从SHOW COLLATION中获得的前两个字段。  

## 4、COLUMNS
COLUMNS表提供有关表中列的信息。如下语句基本等同：  
```sql
SELECT COLUMN_NAME, DATA_TYPE, IS_NULLABLE, COLUMN_DEFAULT
  FROM INFORMATION_SCHEMA.COLUMNS
  WHERE table_name = 'tbl_name'
  [AND table_schema = 'db_name']
  [AND column_name LIKE 'wild']

SHOW COLUMNS
  FROM tbl_name
  [FROM db_name]
  [LIKE 'wild']  
查询一下哪些表中有id列：  
mysql> select TABLE_NAME, TABLE_SCHEMA  from columns where column_name='id';
+----------------------+--------------------+
| TABLE_NAME           | TABLE_SCHEMA       |
+----------------------+--------------------+
| COLLATIONS           | information_schema |
| INNODB_FOREIGN       | information_schema |
| INNODB_FOREIGN_COLS  | information_schema |
| PROCESSLIST          | information_schema |
| slave_relay_log_info | mysql              |
| slave_worker_info    | mysql              |
| t2                   | test               |
+----------------------+--------------------+
7 rows in set (0.00 sec)
```
## 5、COLUMN_PRIVILEGES
COLUMN_PRIVILEGES表提供了有关列权限的信息。这些信息来自mysql.columns_priv授权表。  

## 6、COLUMN_STATISTICS
COLUMN_STATISTICS表是一个视图，用于访问column_statistics数据字典表中的信息，该表存储有关列值的直方图统计信息。  

## 7、ENGINES
ENGINES表提供了关于存储引擎的信息。  
```sql
mysql> select * from ENGINES;
+--------------------+---------+----------------------------------------------------------------+--------------+------+------------+
| ENGINE             | SUPPORT | COMMENT                                                        | TRANSACTIONS | XA   | SAVEPOINTS |
+--------------------+---------+----------------------------------------------------------------+--------------+------+------------+
| ARCHIVE            | YES     | Archive storage engine                                         | NO           | NO   | NO         |
| CSV                | YES     | CSV storage engine                                             | NO           | NO   | NO         |
| PERFORMANCE_SCHEMA | YES     | Performance Schema                                             | NO           | NO   | NO         |
| InnoDB             | DEFAULT | Supports transactions, row-level locking, and foreign keys     | YES          | YES  | YES        |
| MEMORY             | YES     | Hash based, stored in memory, useful for temporary tables      | NO           | NO   | NO         |
| MyISAM             | YES     | MyISAM storage engine                                          | NO           | NO   | NO         |
| FEDERATED          | NO      | Federated MySQL storage engine                                 | NULL         | NULL | NULL       |
| MRG_MYISAM         | YES     | Collection of identical MyISAM tables                          | NO           | NO   | NO         |
| BLACKHOLE          | YES     | /dev/null storage engine (anything you write to it disappears) | NO           | NO   | NO         |
+--------------------+---------+----------------------------------------------------------------+--------------+------+------------+
9 rows in set (0.02 sec)
```

## 8、EVENTS
EVENTS表提供了关于预定事件的信息。例如：   
用户jon@ghidora创建一个名为e_daily的事件，并在几分钟后使用ALTER EVENT语句修改它:  
```sql
use test;
DELIMITER |

CREATE EVENT e_daily
    ON SCHEDULE
      EVERY 1 DAY
    COMMENT 'Saves total number of sessions then clears the table each day'
    DO
      BEGIN
        INSERT INTO site_activity.totals (time, total)
          SELECT CURRENT_TIMESTAMP, COUNT(*)
            FROM site_activity.sessions;
        DELETE FROM site_activity.sessions;
      END |

DELIMITER ;

ALTER EVENT e_daily
    ENABLE;

mysql> SELECT * FROM INFORMATION_SCHEMA.EVENTS WHERE EVENT_NAME = 'e_daily' AND EVENT_SCHEMA = 'test'\G;
*************************** 1. row ***************************
       EVENT_CATALOG: def
        EVENT_SCHEMA: test
          EVENT_NAME: e_daily
             DEFINER: root@localhost
           TIME_ZONE: SYSTEM
          EVENT_BODY: SQL
    EVENT_DEFINITION: BEGIN
        INSERT INTO site_activity.totals (time, total)
          SELECT CURRENT_TIMESTAMP, COUNT(*)
            FROM site_activity.sessions;
        DELETE FROM site_activity.sessions;
      END
          EVENT_TYPE: RECURRING
          EXECUTE_AT: NULL
      INTERVAL_VALUE: 1
      INTERVAL_FIELD: DAY
            SQL_MODE: ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION
              STARTS: 2018-06-03 00:14:07
                ENDS: NULL
              STATUS: ENABLED
       ON_COMPLETION: NOT PRESERVE
             CREATED: 2018-06-03 00:14:07
        LAST_ALTERED: 2018-06-03 00:14:07
       LAST_EXECUTED: 2018-06-02 16:14:07
       EVENT_COMMENT: Saves total number of sessions then clears the table each day
          ORIGINATOR: 1
CHARACTER_SET_CLIENT: utf8mb4
COLLATION_CONNECTION: utf8mb4_0900_ai_ci
  DATABASE_COLLATION: utf8_general_ci
1 row in set (0.01 sec)

```

## 9、FILES
FILES表提供有关存储MySQL表空间数据的文件信息。  
```sql
mysql>SELECT FILE_ID, FILE_NAME, FILE_TYPE, TABLESPACE_NAME, FREE_EXTENTS, TOTAL_EXTENTS,
EXTENT_SIZE, INITIAL_SIZE, MAXIMUM_SIZE, AUTOEXTEND_SIZE, DATA_FREE, STATUS ENGINE
FROM INFORMATION_SCHEMA.FILES \G
```

## 10、KEY_COLUMN_USAGE
KEY_COLUMN_USAGE表描述了哪些键列有约束条件。即这个列上有索引，而且还有其他约束。例如：  
```sql
use test;
CREATE TABLE t1
(
    s1 INT,
    s2 INT,
    s3 INT,
    PRIMARY KEY(s3)
) ENGINE=InnoDB;

CREATE TABLE t3
(
    s1 INT,
    s2 INT,
    s3 INT,
    KEY(s1),
    CONSTRAINT CO FOREIGN KEY (s2) REFERENCES t1(s3)
) ENGINE=InnoDB;
mysql> select * from KEY_COLUMN_USAGE where TABLE_SCHEMA='test' \G;
*************************** 1. row ***************************
           CONSTRAINT_CATALOG: def
            CONSTRAINT_SCHEMA: test
              CONSTRAINT_NAME: PRIMARY
                TABLE_CATALOG: def
                 TABLE_SCHEMA: test
                   TABLE_NAME: t1
                  COLUMN_NAME: s3
             ORDINAL_POSITION: 1
POSITION_IN_UNIQUE_CONSTRAINT: NULL
      REFERENCED_TABLE_SCHEMA: NULL
        REFERENCED_TABLE_NAME: NULL
       REFERENCED_COLUMN_NAME: NULL
*************************** 2. row ***************************
           CONSTRAINT_CATALOG: def
            CONSTRAINT_SCHEMA: test
              CONSTRAINT_NAME: CO
                TABLE_CATALOG: def
                 TABLE_SCHEMA: test
                   TABLE_NAME: t3
                  COLUMN_NAME: s2
             ORDINAL_POSITION: 1
POSITION_IN_UNIQUE_CONSTRAINT: 1
      REFERENCED_TABLE_SCHEMA: test
        REFERENCED_TABLE_NAME: t1
       REFERENCED_COLUMN_NAME: s3
2 rows in set, 2 warnings (0.01 sec)
```
如果约束是外键，那么将显示外键列，而不是外键引用的列。  

## 11、KEYWORDS
KEYWORDS表列出了MySQL考虑的关键字。并且有一个表示该关键字是否保留的字段。  
```sql
SELECT * FROM INFORMATION_SCHEMA.KEYWORDS;
SELECT WORD FROM INFORMATION_SCHEMA.KEYWORDS WHERE RESERVED = 1;
SELECT WORD FROM INFORMATION_SCHEMA.KEYWORDS WHERE RESERVED = 0;

SELECT WORD FROM INFORMATION_SCHEMA.KEYWORDS WHERE RESERVED;
SELECT WORD FROM INFORMATION_SCHEMA.KEYWORDS WHERE NOT RESERVED;
```

## 12、OPTIMIZER_TRACE
OPTIMIZER_TRACE表提供了优化器跟踪功能产生的信息。要启用跟踪，请使用 optimizer_trace系统变量。  

## 13、PARAMETERS
PARAMETERS表提供有关存储过程和函数参数的信息，以及有关存储函数的返回值的信息。  

## 14、PARTITIONS
PARTITIONS表提供有关表分区的信息。有关分区表的信息，见[分区](https://dev.mysql.com/doc/refman/8.0/en/partitioning.html)。  
例如：
```sql
CREATE TABLE tp (
    c1 INT,
    c2 INT,
    c3 VARCHAR(25)
)
PARTITION BY HASH(c1 + c2)
PARTITIONS 4;

mysql> SELECT DISTINCT PARTITION_EXPRESSION
     >     FROM INFORMATION_SCHEMA.PARTITIONS
     >     WHERE TABLE_NAME='tp' AND TABLE_SCHEMA='test';
+----------------------+
| PARTITION_EXPRESSION |
+----------------------+
| c1 + c2              |
+----------------------+
1 row in set (0.09 sec)
```

## 15、PLUGINS
PLUGINS表提供了有关服务器插件的信息。例如如下语句是等价的：   
```sql
SELECT
  PLUGIN_NAME, PLUGIN_STATUS, PLUGIN_TYPE,
  PLUGIN_LIBRARY, PLUGIN_LICENSE
FROM INFORMATION_SCHEMA.PLUGINS;

SHOW PLUGINS;
```
## 16、PROCESSLIST
PROCESSLIST表提供有关哪些线程正在运行的信息。例如，如下语句是等价的：  
```sql
SELECT * FROM INFORMATION_SCHEMA.PROCESSLIST

SHOW FULL PROCESSLIST
```

## 17、PROFILING
PROFILING表提供语句分析信息。将来会在mysql中删除，改用[Chapter 25, MySQL Performance](https://dev.mysql.com/doc/refman/8.0/en/performance-schema.html)。  

## 18、REFERENTIAL_CONSTRAINTS
REFERENTIAL_CONSTRAINTS表提供有关外键的信息。  

## 19、RESOURCE_GROUPS
RESOURCE_GROUPS表是一个视图，用于访问resource_groups定义资源组的数据字典表中的信息。  

## 20、ROUTINES
ROUTINES表提供有关存储例程的信息（包括存储过程和函数）。ROUTINES表不包含用户定义的函数（UDF）。  

## 21、SCHEMATA
schema是数据库，因此SCHEMATA表提供有关数据库的信息。例如下列语句是等价的：  
```sql
SELECT SCHEMA_NAME AS `Database`
  FROM INFORMATION_SCHEMA.SCHEMATA
  [WHERE SCHEMA_NAME LIKE 'wild']

SHOW DATABASES
  [LIKE 'wild']
```

## 22、SCHEMA_PRIVILEGES
SCHEMA_PRIVILEGES表提供有关schema（数据库）特权的信息。这些信息来自mysql.db授权表。例如：   
```sql
select * from SCHEMA_PRIVILEGES;
+-----------------------------+---------------+--------------------+-------------------------+--------------+
| GRANTEE                     | TABLE_CATALOG | TABLE_SCHEMA       | PRIVILEGE_TYPE          | IS_GRANTABLE |
+-----------------------------+---------------+--------------------+-------------------------+--------------+
| 'mysql.sys'@'localhost'     | def           | sys                | TRIGGER                 | NO           |
| 'mysql.session'@'localhost' | def           | performance_schema | SELECT                  | NO           |
| 'test'@'%'                  | def           | test               | SELECT                  | YES          |
| 'test'@'%'                  | def           | test               | INSERT                  | YES          |
| 'test'@'%'                  | def           | test               | UPDATE                  | YES          |
| 'test'@'%'                  | def           | test               | DELETE                  | YES          |
| 'test'@'%'                  | def           | test               | CREATE                  | YES          |
| 'test'@'%'                  | def           | test               | DROP                    | YES          |
| 'test'@'%'                  | def           | test               | REFERENCES              | YES          |
| 'test'@'%'                  | def           | test               | INDEX                   | YES          |
| 'test'@'%'                  | def           | test               | ALTER                   | YES          |
| 'test'@'%'                  | def           | test               | CREATE TEMPORARY TABLES | YES          |
| 'test'@'%'                  | def           | test               | LOCK TABLES             | YES          |
| 'test'@'%'                  | def           | test               | EXECUTE                 | YES          |
| 'test'@'%'                  | def           | test               | CREATE VIEW             | YES          |
| 'test'@'%'                  | def           | test               | SHOW VIEW               | YES          |
| 'test'@'%'                  | def           | test               | CREATE ROUTINE          | YES          |
| 'test'@'%'                  | def           | test               | ALTER ROUTINE           | YES          |
| 'test'@'%'                  | def           | test               | EVENT                   | YES          |
| 'test'@'%'                  | def           | test               | TRIGGER                 | YES          |
+-----------------------------+---------------+--------------------+-------------------------+--------------+
```

## 23、STATISTICS
STATISTICS表提供有关表索引的信息。如下语句是等价的：  
```sql
SELECT * FROM INFORMATION_SCHEMA.STATISTICS
  WHERE table_name = 'tbl_name'
  AND table_schema = 'db_name'

SHOW INDEX
  FROM tbl_name
  FROM db_name
```

## 24、ST_GEOMETRY_COLUMNS
ST_GEOMETRY_COLUMNS表提供了有关存储空间数据的表列的信息。

## 25、ST_SPATIAL_REFERENCE_SYSTEMS
ST_SPATIAL_REFERENCE_SYSTEMS表提供了有关空间数据的可用空间参照系的信息。  

## 26、TABLES
TABLES表提供有关数据库中表的信息。例如：  
```sql
SELECT table_name FROM INFORMATION_SCHEMA.TABLES
  WHERE table_schema = 'db_name'
  [AND table_name LIKE 'wild']

SHOW TABLES
  FROM db_name
  [LIKE 'wild']
```

## 27、TABLESPACES
TABLESPACES表提供有关活动表空间的信息。  
***说明：TABLESPACES表不提供有关InnoDB表空间的信息。对于InnoDB表空间的元数据，请参阅INNODB_TABLESPACES和 INNODB_DATAFILES。INFORMATION_SCHEMA.FILES表也提供InnoDB表空间的元数据。***  

## 28、TABLE_CONSTRAINTS
TABLE_CONSTRAINTS表描述哪些表有约束。  

## 29、TABLE_PRIVILEGES
TABLE_PRIVILEGES表提供有关表特权的信息。这些信息来自mysql.tables_priv授权表。  
```sql
select * from TABLE_PRIVILEGES;
+-----------------------------+---------------+--------------+------------+----------------+--------------+
| GRANTEE                     | TABLE_CATALOG | TABLE_SCHEMA | TABLE_NAME | PRIVILEGE_TYPE | IS_GRANTABLE |
+-----------------------------+---------------+--------------+------------+----------------+--------------+
| 'mysql.sys'@'localhost'     | def           | sys          | sys_config | SELECT         | NO           |
| 'mysql.session'@'localhost' | def           | mysql        | user       | SELECT         | NO           |
+-----------------------------+---------------+--------------+------------+----------------+--------------+
```

## 30、TRIGGERS
TRIGGERS表提供了有关触发器的信息。要查看有关表格触发器的信息，您必须具有该表格的TRIGGER特权。  
```sql
mysql> SELECT * FROM INFORMATION_SCHEMA.TRIGGERS
       WHERE TRIGGER_SCHEMA='test' AND TRIGGER_NAME='ins_sum'\G
*************************** 1. row ***************************
           TRIGGER_CATALOG: def
            TRIGGER_SCHEMA: test
              TRIGGER_NAME: ins_sum
        EVENT_MANIPULATION: INSERT
      EVENT_OBJECT_CATALOG: def
       EVENT_OBJECT_SCHEMA: test
        EVENT_OBJECT_TABLE: account
              ACTION_ORDER: 1
          ACTION_CONDITION: NULL
          ACTION_STATEMENT: SET @sum = @sum + NEW.amount
        ACTION_ORIENTATION: ROW
             ACTION_TIMING: BEFORE
ACTION_REFERENCE_OLD_TABLE: NULL
ACTION_REFERENCE_NEW_TABLE: NULL
  ACTION_REFERENCE_OLD_ROW: OLD
  ACTION_REFERENCE_NEW_ROW: NEW
                   CREATED: 2017-03-30 20:29:39.06
                  SQL_MODE: ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION
                   DEFINER: me@localhost
      CHARACTER_SET_CLIENT: utf8
      COLLATION_CONNECTION: utf8_general_ci
        DATABASE_COLLATION: utf8mb4_0900_ai_ci
```

## 31、USER_PRIVILEGES
USER_PRIVILEGES表提供有关全局权限的信息。这些信息来自mysql.user授权表。  
```sql
select * from USER_PRIVILEGES;
+--------------------------------+---------------+----------------------------+--------------+
| GRANTEE                        | TABLE_CATALOG | PRIVILEGE_TYPE             | IS_GRANTABLE |
+--------------------------------+---------------+----------------------------+--------------+
| 'mysql.infoschema'@'localhost' | def           | SELECT                     | NO           |
| 'mysql.session'@'localhost'    | def           | SUPER                      | NO           |
| 'mysql.sys'@'localhost'        | def           | USAGE                      | NO           |
| 'root'@'localhost'             | def           | SELECT                     | YES          |
| 'root'@'localhost'             | def           | INSERT                     | YES          |
| 'root'@'localhost'             | def           | UPDATE                     | YES          |
| 'root'@'localhost'             | def           | DELETE                     | YES          |
| 'root'@'localhost'             | def           | CREATE                     | YES          |
| 'root'@'localhost'             | def           | DROP                       | YES          |
| 'root'@'localhost'             | def           | RELOAD                     | YES          |
| 'root'@'localhost'             | def           | SHUTDOWN                   | YES          |
| 'root'@'localhost'             | def           | PROCESS                    | YES          |
| 'root'@'localhost'             | def           | FILE                       | YES          |
| 'root'@'localhost'             | def           | REFERENCES                 | YES          |
| 'root'@'localhost'             | def           | INDEX                      | YES          |
| 'root'@'localhost'             | def           | ALTER                      | YES          |
| 'root'@'localhost'             | def           | SHOW DATABASES             | YES          |
| 'root'@'localhost'             | def           | SUPER                      | YES          |
| 'root'@'localhost'             | def           | CREATE TEMPORARY TABLES    | YES          |
| 'root'@'localhost'             | def           | LOCK TABLES                | YES          |
| 'root'@'localhost'             | def           | EXECUTE                    | YES          |
| 'root'@'localhost'             | def           | REPLICATION SLAVE          | YES          |
| 'root'@'localhost'             | def           | REPLICATION CLIENT         | YES          |
| 'root'@'localhost'             | def           | CREATE VIEW                | YES          |
| 'root'@'localhost'             | def           | SHOW VIEW                  | YES          |
| 'root'@'localhost'             | def           | CREATE ROUTINE             | YES          |
| 'root'@'localhost'             | def           | ALTER ROUTINE              | YES          |
| 'root'@'localhost'             | def           | CREATE USER                | YES          |
| 'root'@'localhost'             | def           | EVENT                      | YES          |
| 'root'@'localhost'             | def           | TRIGGER                    | YES          |
| 'root'@'localhost'             | def           | CREATE TABLESPACE          | YES          |
| 'root'@'localhost'             | def           | CREATE ROLE                | YES          |
| 'root'@'localhost'             | def           | DROP ROLE                  | YES          |
| 'root'@'localhost'             | def           | XA_RECOVER_ADMIN           | YES          |
| 'root'@'localhost'             | def           | SYSTEM_VARIABLES_ADMIN     | YES          |
| 'root'@'localhost'             | def           | SET_USER_ID                | YES          |
| 'root'@'localhost'             | def           | ROLE_ADMIN                 | YES          |
| 'root'@'localhost'             | def           | RESOURCE_GROUP_USER        | YES          |
| 'root'@'localhost'             | def           | RESOURCE_GROUP_ADMIN       | YES          |
| 'root'@'localhost'             | def           | REPLICATION_SLAVE_ADMIN    | YES          |
| 'root'@'localhost'             | def           | PERSIST_RO_VARIABLES_ADMIN | YES          |
| 'root'@'localhost'             | def           | GROUP_REPLICATION_ADMIN    | YES          |
| 'root'@'localhost'             | def           | ENCRYPTION_KEY_ADMIN       | YES          |
| 'root'@'localhost'             | def           | CONNECTION_ADMIN           | YES          |
| 'root'@'localhost'             | def           | BINLOG_ADMIN               | YES          |
| 'root'@'localhost'             | def           | BACKUP_ADMIN               | YES          |
| 'test'@'%'                     | def           | USAGE                      | NO           |
+--------------------------------+---------------+----------------------------+--------------+
47 rows in set (0.01 sec)
```

## 32、VIEWS
VIEWS表提供关于数据库中视图的信息。你必须具有SHOW VIEW权限才能访问此表。  

## 33、InnoDB相关的INFORMATION_SCHEMA表
InnoDB的表如下：  
```sql
select TABLE_NAME from TABLES where TABLE_NAME like 'INNODB_%';
+----------------------------+
| TABLE_NAME                 |
+----------------------------+
| INNODB_BUFFER_PAGE         |
| INNODB_BUFFER_PAGE_LRU     |
| INNODB_BUFFER_POOL_STATS   |
| INNODB_CACHED_INDEXES      |
| INNODB_CMP                 |
| INNODB_CMPMEM              |
| INNODB_CMPMEM_RESET        |
| INNODB_CMP_PER_INDEX       |
| INNODB_CMP_PER_INDEX_RESET |
| INNODB_CMP_RESET           |
| INNODB_COLUMNS             |
| INNODB_DATAFILES           |
| INNODB_FIELDS              |
| INNODB_FOREIGN             |
| INNODB_FOREIGN_COLS        |
| INNODB_FT_BEING_DELETED    |
| INNODB_FT_CONFIG           |
| INNODB_FT_DEFAULT_STOPWORD |
| INNODB_FT_DELETED          |
| INNODB_FT_INDEX_CACHE      |
| INNODB_FT_INDEX_TABLE      |
| INNODB_INDEXES             |
| INNODB_METRICS             |
| INNODB_TABLES              |
| INNODB_TABLESPACES         |
| INNODB_TABLESPACES_BRIEF   |
| INNODB_TABLESTATS          |
| INNODB_TEMP_TABLE_INFO     |
| INNODB_TRX                 |
| INNODB_VIRTUAL             |
+----------------------------+
```
接下来将一一学习。  

### 33.1 INNODB_BUFFER_PAGE
INNODB_BUFFER_PAGE表保存有关InnoDB缓冲池中每个页面的信息。  
***警告：查询INNODB_BUFFER_PAGE表可能会导致显着的性能开销。不要在生产系统上查询此表，除非你知道你的查询可能具有的性能影响，并确定它可以接受。***  
我们先看看这个表的定义：  
```sql
mysql> show create table INNODB_BUFFER_PAGE;
CREATE TEMPORARY TABLE `INNODB_BUFFER_PAGE` (
  `POOL_ID` bigint(21) unsigned NOT NULL DEFAULT '0',
  `BLOCK_ID` bigint(21) unsigned NOT NULL DEFAULT '0',
  `SPACE` bigint(21) unsigned NOT NULL DEFAULT '0',
  `PAGE_NUMBER` bigint(21) unsigned NOT NULL DEFAULT '0',
  `PAGE_TYPE` varchar(64) DEFAULT NULL,
  `FLUSH_TYPE` bigint(21) unsigned NOT NULL DEFAULT '0',
  `FIX_COUNT` bigint(21) unsigned NOT NULL DEFAULT '0',
  `IS_HASHED` varchar(3) DEFAULT NULL,
  `NEWEST_MODIFICATION` bigint(21) unsigned NOT NULL DEFAULT '0',
  `OLDEST_MODIFICATION` bigint(21) unsigned NOT NULL DEFAULT '0',
  `ACCESS_TIME` bigint(21) unsigned NOT NULL DEFAULT '0',
  `TABLE_NAME` varchar(1024) DEFAULT NULL,
  `INDEX_NAME` varchar(1024) DEFAULT NULL,
  `NUMBER_RECORDS` bigint(21) unsigned NOT NULL DEFAULT '0',
  `DATA_SIZE` bigint(21) unsigned NOT NULL DEFAULT '0',
  `COMPRESSED_SIZE` bigint(21) unsigned NOT NULL DEFAULT '0',
  `PAGE_STATE` varchar(64) DEFAULT NULL,
  `IO_FIX` varchar(64) DEFAULT NULL,
  `IS_OLD` varchar(3) DEFAULT NULL,
  `FREE_PAGE_CLOCK` bigint(21) unsigned NOT NULL DEFAULT '0'
) ENGINE=MEMORY DEFAULT CHARSET=utf8;
```
可以看到，它是个临时表。   
该表中的列的解释如下：  
  
|列名|描述|  
|-|-|  
|POOL_ID|缓冲池ID。用于区分多个缓冲池实例的标识符。|  
|BLOCK_ID|缓冲池块ID。|  
|SPACE|表空间ID，使用与INNODB_TABLES.SPACE中相同的值|  
|PAGE_NUMBER|页号|  
|PAGE_TYPE|页面类型。允许的值为：|  
|FLUSH_TYPE|Flush 类型|  
|FIX_COUNT|在缓冲池中使用此块的线程数。当为零时，该区块可以被释放。|  
|IS_HASHED|哈希索引是否构建在此页面上。|  
|NEWEST_MODIFICATION||  
|OLDEST_MODIFICATION||  
|ACCESS_TIME||  
|TABLE_NAME||  
|INDEX_NAME||  
|NUMBER_RECORDS||  
|DATA_SIZE||  
|COMPRESSED_SIZE||  
|PAGE_STATE||  
|IO_FIX||  
|IS_OLD||  
|FREE_PAGE_CLOCK||  












