# InnoDB配置
本节介绍InnoDB初始化，启动以及各种组件和功能的配置，关于InnoDB表的优化见[Section 8.5, “Optimizing for InnoDB Tables”](https://dev.mysql.com/doc/refman/8.0/en/optimizing-innodb.html)。  

## 7.1 InnoDB启动配置
有关InnoDB配置，首先考虑和涉及的是：数据文件、日志文件、页（InnoDB中可用于存储多行数据的块）大小、内存缓冲区。建议你在创建InnoDB实例之前配置好数据文件，日志文件和页面大小。在创建InnoDB实例之后修改数据文件或日志文件配置会比较复杂，同时：**页面大小只能在首次初始化InnoDB实例时定义。** 除了这些配置，本小节还包括如下配置：  
 - 在MySQL配置文件中指定选项  
 - 查看InnoDB初始化信息  
 - 主要的考量因素  
 - 系统表空间数据文件配置   
 - 重做日志文件配置  
 - 撤消表空间配置  
 - 全局临时表空间配置  
 - 会话临时表空间配置  
 - 页大小配置  
 - 内存配置  

### 7.1.1 在MySQL配置文件中指定选项
有关InnoDB的配置，在配置文件中，放在[mysqld] 之下。要确保mysqld仅从特定文件（和mysqld-auto.cnf）读取选项，请在启动服务器时使用--defaults-file选项作为命令行上的第一个选项：  
```shell
mysqld --defaults-file=path_to_configuration_file
```

### 7.1.2 查看InnoDB初始化信息
要在启动期间查看InnoDB初始化信息，请从命令提示符启动mysqld。从命令提示符启动mysqld时，初始化信息将打印到控制台。例如：  
```shell
shell> bin/mysqld --user=mysql &
```
如果未将服务器输出发送到控制台，可以查阅日志文件。  

#### 注意：
 > InnoDB在启动时不会打开所有用户表和关联的数据文件,但是，InnoDB会检查数据字典中引用的表空间文件（*.ibd文件）是否存在。如果找不到表空间文件，InnoDB会打印错误信息并继续按序启动。在崩溃恢复期间，很可能会打开重做日志中引用的表空间文件。  

### 7.1.3 主要的考量因素
在开始配置InnoDB之前，请了解与存储相关的注意事项。  
 - 某些情况下，数据不全部存储在同一物理磁盘上，会提高数据库性能。将日志文件放在与数据不同的磁盘上通常有利于提高性能。可以将系统表空间数据文件和日志文件放在不同的磁盘。还可以将原始磁盘分区（原始设备）用于InnoDB数据文件,这可能会加快I/O速度。详见[Using Raw Disk Partitions for the System Tablespace](https://dev.mysql.com/doc/refman/8.0/en/innodb-system-tablespace.html#innodb-raw-devices)  

 - InnoDB是一种适用于MySQL的事务安全（ACID兼容）存储引擎，具有提交，回滚和崩溃恢复功能，可保护用户数据。**但是**，如果底层操作系统或硬件无法正常工作，则无法保证这些操作。许多操作系统或磁盘子系统可能会延迟或重新排序写入操作以提高性能。在某些操作系统上，等到文件的所有数据都已刷新的fsync（）系统调用实际上可能在数据刷新到持久存储之前返回。因此，操作系统崩溃或断电可能会破坏最近提交的数据，或者在最坏的情况下，甚至因为写入操作已重新排序而损坏数据库。如果数据对您非常重要，在用于生产环境之前，请一定要进行插拔测试。在OS X 10.3及更高版本上，InnoDB使用特殊的fcntl（）文件刷新方法。在Linux下，**建议禁用回写缓存(disable the write-back cache)**。在ATA / SATA磁盘驱动器上，hdparm -W0 /dev/hda等命令可能会禁用回写缓存。请注意，某些驱动器或磁盘控制器可能无法禁用回写缓存。  

 - 为了保证数据在崩溃恢复下不受损坏，InnoDB使用文件刷新技术，被称为doublewrite缓冲区的结构（称为缓冲区，但是该缓冲区位于磁盘上），默认情况下启用（innodb_doublewrite = ON）。双写缓冲区可在崩溃或停电后为恢复增加安全性。为了保证数据安全性，最好启用该选项。

 - 在InnoDB与NFS一起使用之前，请看[NFS与MySQL一起使用的潜在问题](https://dev.mysql.com/doc/refman/8.0/en/disk-issues.html#disk-issues-nfs)。  

### 7.1.4 系统表空间数据文件配置
innodb_data_file_path配置项定义InnoDB系统表空间数据文件的名称，大小和属性。如果没有为innodb_data_file_path指定值，默认行为是创建一个略大于12MB的单个自动扩展数据文件，名为ibdata1。  
```sql
mysql> show variables like 'innodb_data_file_path';
+-----------------------+------------------------+
| Variable_name         | Value                  |
+-----------------------+------------------------+
| innodb_data_file_path | ibdata1:12M:autoextend |
+-----------------------+------------------------+
1 row in set (0.63 sec)
```
要指定多个数据文件，请用";" 分隔：  
```conf
innodb_data_file_path=datafile_spec1[;datafile_spec2]...
```
以下配置一个名为ibdata1的12MB数据文件，该文件是自动扩展的。没有给出文件的位置，因此默认情况下，InnoDB在MySQL数据目录中创建它：  
```conf
[mysqld]
innodb_data_file_path=ibdata1:12M:autoextend
```
文件大小指定时使用K,KB, M,MB, G,GB 这些后缀。
当系统表空间有多个文件时，为**第一个**系统表空间数据文件强制实施最小文件大小，以确保有足够的空间用于双写缓冲区页面：  
 - innodb_page_size值为16KB或更小时，文件最小为3MB。  
 - innodb_page_size值为32KB时，文件最小为6MB。  
 - innodb_page_size值为64KB时，文件最小为12MB。  
具有固定大小50MB文件名为ibdata1的系统表空间和一个名为ibdata2的50MB自动扩展文件可以像这样配置：  
```conf
[mysqld]
innodb_data_file_path=ibdata1:50M;ibdata2:50M:autoextend
```
完整的语法如下：  
```conf
file_name:file_size[:autoextend[:max:max_file_size]]
```
autoextend和max属性只能用于innodb_data_file_path设置中最后指定的数据文件。 如果为最后一个数据文件指定autoextend选项, 如果表空间中的可用空间不足，InnoDB会扩展数据文件。默认情况下，自动扩展增量一次为64MB。要修改增量，请更改innodb_autoextend_increment系统变量。如果磁盘已满，您可能希望在另一个磁盘上添加另一个数据文件。有关说明，请参阅[Resizing the System Tablespace](https://dev.mysql.com/doc/refman/8.0/en/innodb-system-tablespace.html#innodb-resize-system-tablespace)。  

单个文件的大小限制由您的操作系统决定。你可以在支持大文件的操作系统上将文件大小设置为大于4GB。还可以将[原始磁盘分区作为数据文件](https://dev.mysql.com/doc/refman/8.0/en/innodb-system-tablespace.html#innodb-raw-devices)。  

InnoDB不知道文件系统的最大文件大小，因此在最大文件大小较小的文件系统（如2GB）时要小心。要指定自动扩展数据文件的最大大小，请使用autoextend属性后面的max属性。因为超过最大大小会导致致命错误，可能导致服务器退出，所以仅在限制磁盘使用率至关重要的情况下才使用max属性。以下配置允许ibdata1增长到500MB：  
```conf
[mysqld]
innodb_data_file_path=ibdata1:12M:autoextend:max:500M
```
默认情况下，InnoDB在MySQL数据目录（datadir指定）中创建系统表空间文件。要明确指定位置，请使用innodb_data_home_dir选项。例如，要在名为myibdata的目录中创建名为ibdata1和ibdata2的两个文件，请按以下方式配置InnoDB：
```conf
[mysqld]
innodb_data_home_dir = /path/to/myibdata/
innodb_data_file_path=ibdata1:50M;ibdata2:50M:autoextend
```
#### 注意：
 > 在为innodb_data_home_dir指定值时，需要使用尾部斜杠。  
 > InnoDB不会创建目录，因此请确保在启动服务器之前myibdata目录已经存在。
 > 确保MySQL服务器具有在数据目录中创建文件的适当访问权限。更一般地说，服务器必须在需要创建数据文件的任何目录中具有访问权限。  

如果将innodb_data_home_dir指定为空字符串，则可以为innodb_data_file_path值中列出的数据文件指定绝对路径。以下示例等同于前一个示例：
```conf
[mysqld]
innodb_data_home_dir =
innodb_data_file_path=/path/to/myibdata/ibdata1:50M;/path/to/myibdata/ibdata2:50M:autoextend
```
### 7.1.5 重做日志文件配置
默认情况下，InnoDB会在数据目录中创建两个5MB的重做日志文件：ib_logfile0和ib_logfile1。以下选项可用于修改默认配置：  






