# MySQL 程序
mysql包括很多程序，本章主要对这些程序进行学习。

---
## 1、MySQL程序概述
安装mysql之后，有很多程序。mysql程序分为如下几类：  
 - mysql服务器相关的程序  
 - 安装或升级mysql时执行设置操作的程序  
 - 连接到MySQL服务器的MySQL客户端程序  
 - MySQL管理和实用程序  
 - MySQL程序开发实用程序  
 - 杂项工具   
 典型的安装后，包含的程序如下：  
```shell
[root@localhost bin]# pwd
/usr/local/mysql/bin
[root@localhost bin]# ll
total 1072992
-rwxr-xr-x. 1 root root  12429832 May 21 00:26 ibd2sdi
-rwxr-xr-x. 1 root root  10825744 May 21 00:26 innochecksum
-rwxr-xr-x. 1 root root   8761544 May 21 00:26 lz4_decompress
-rwxr-xr-x. 1 root root  13106848 May 21 00:21 myisamchk
-rwxr-xr-x. 1 root root  12400984 May 21 00:21 myisam_ftdump
-rwxr-xr-x. 1 root root  11792640 May 21 00:21 myisamlog
-rwxr-xr-x. 1 root root  12485768 May 21 00:21 myisampack
-rwxr-xr-x. 1 root root   9300984 May 21 00:26 my_print_defaults
-rwxr-xr-x. 1 root root  12371216 May 21 00:25 mysql
-rwxr-xr-x. 1 root root  11091320 May 21 00:25 mysqladmin
-rwxr-xr-x. 1 root root  15880056 May 21 00:25 mysqlbinlog
-rwxr-xr-x. 1 root root  11318128 May 21 00:25 mysqlcheck
-rwxr-xr-x. 1 root root  12517920 May 21 00:26 mysql_client_test
-rwxr-xr-x. 1 root root      5021 May 20 23:18 mysql_config
-rwxr-xr-x. 1 root root   9446032 May 21 00:25 mysql_config_editor
-rwxr-xr-x. 1 root root 736692504 May 21 00:31 mysqld
-rwxr-xr-x. 1 root root     26859 May 20 23:18 mysqld_multi
-rwxr-xr-x. 1 root root     29815 May 20 23:18 mysqld_safe
-rwxr-xr-x. 1 root root  11423968 May 21 00:25 mysqldump
-rwxr-xr-x. 1 root root      7855 May 20 23:18 mysqldumpslow
-rwxr-xr-x. 1 root root  11083224 May 21 00:25 mysqlimport
-rwxr-xr-x. 1 root root  20870888 May 21 00:26 mysqlpump
-rwxr-xr-x. 1 root root  11040832 May 21 00:25 mysql_secure_installation
-rwxr-xr-x. 1 root root  11052016 May 21 00:25 mysqlshow
-rwxr-xr-x. 1 root root  11151304 May 21 00:25 mysqlslap
-rwxr-xr-x. 1 root root   9681536 May 21 00:25 mysql_ssl_rsa_setup
-rwxr-xr-x. 1 root root  12555976 May 21 00:26 mysqltest
-rwxr-xr-x. 1 root root   9110056 May 21 00:26 mysql_tzinfo_to_sql
-rwxr-xr-x. 1 root root  19914976 May 21 00:25 mysql_upgrade
-rwxr-xr-x. 1 root root  33098920 May 21 00:25 mysqlxtest
-rwxr-xr-x. 1 root root   9888312 May 21 00:26 perror
-rwxr-xr-x. 1 root root   9296536 May 21 00:26 resolveip
-rwxr-xr-x. 1 root root   9358544 May 21 00:26 resolve_stack_dump
-rwxr-xr-x. 1 root root   8643184 May 21 00:26 zlib_decompress
[root@localhost bin]# 
```
接下来对这些程序简要的描述，后边的章节会有更加详细的描述。  
***mysql服务器相关的程序：***  
 - mysqld：mysql服务器  
 - mysqld_safe：服务器启动脚本  
 - mysql.server：调用mysqld_safe的服务器启动脚本  
 - mysqld_multi：服务器启动脚本，可以启动或停止系统上安装的多个服务器  
***安装或升级mysql时执行设置操作的程序：***  
 - comp_err：该程序在MySQL构建/安装过程中使用。它从错误源文件编译错误消息文件。  
 - mysql_secure_installation：该程序可以提高MySQL安装的安全性。  
 - mysql_ssl_rsa_setup：生成支持ssl相关的文件和密钥。  
 - mysql_tzinfo_to_sql：将主机系统时区数据库的信息加在到mysql数据库中。  
 - mysql_upgrade：该程序在MySQL升级操作之后使用。以检查新旧版本的兼容性问题，并修复它们。  

***连接到MySQL服务器的MySQL客户端程序：***  
 - mysql：交互式命令行工具。  
 - mysqladmin：执行管理操作的客户端，如创建或删除数据库，重新加载授权表，将表flush到磁盘，重新打开日志文件等。  
 - mysqlcheck：一个表格维护客户端，用于检查，修复，分析和优化表格。  
 - mysqldump：将MySQL数据库转储为SQL，文本或XML文件的客户端。  
 - mysqlimport：使用LOAD DATA INFILE将文本文件导入各自表的客户端。  
 - mysqlpump：一个将数据库转储为SQL文件的客户端。  
 - mysqlshow：显示有关数据库，表，列和索引信息的客户端。  
 - mysqlslap：load模拟客户端。  

***MySQL管理和实用程序：***  
 - innochecksum：InnoDB脱机文件校验和实用程序。  
 - myisam_ftdump：显示MyISAM表中有关全文索引信息的实用程序。  
 - myisamchk：用于描述，检查，优化和修复MyISAM表的实用程序。  
 - myisamlog：处理MyISAM日志文件内容的实用程序。  
 - myisampack：压缩MyISAM表以生成更小的只读表的实用程序。  
 - mysql_config_editor：一种实用程序，使您可以将身份验证凭据存储在名为.mylogin.cnf的安全加密登录路径文件中。  
 - mysqlbinlog：用于从二进制日志中读取语句的实用程序。可用于帮助从崩溃中恢复。  
 - mysqldumpslow：用于阅读和总结慢查询日志内容的实用程序。  

***MySQL程序开发实用程序：***  
 - mysql_config：一个用于生成在编译MySQL程序时所需的选项值的shell脚本。  
 - my_print_defaults：显示选项文件的选项组中存在哪些选项的实用程序。  
 - resolve_stack_dump：将数值堆栈跟踪转储解析为符号的实用程序。  

***杂项工具：***     
 - lz4_decompress：解压缩使用LZ4压缩创建的mysqlpump输出的实用程序。  
 - perror：显示系统或MySQL错误代码含义的实用程序。  
 - resolveip：将主机名解析为IP地址的实用程序，反之亦然。  
 - zlib_decompress：解压缩使用ZLIB压缩创建的mysqlpump输出的实用程序。  

以上都是命令行工具，另外，Oracle Corporation还提供[MySQL Workbench GUI](https://dev.mysql.com/doc/refman/8.0/en/workbench.html)工具，该工具用于管理MySQL服务器和数据库，以创建，执行和评估查询，以及从其他关系数据库管理系统迁移schemas和数据以供MySQL使用。额外的GUI工具包括[MySQL Notifier](https://dev.mysql.com/doc/refman/8.0/en/windows-notifier.html)和[MySQL for Excel](https://dev.mysql.com/doc/mysql-for-excel/en/)。  
使用 MySQL client/server library与服务器通信的MySQL客户端程序使用如下环境变量：  
  
|环境变量|含义|  
|-|-|  
|MYSQL_UNIX_PORT|默认的Unix套接字文件;用于连接到本地主机|  
|MYSQL_TCP_PORT|默认端口号;用于TCP / IP连接|  
|MYSQL_PWD|默认密码，使用这个环境变量是不安全的，千万不要这么用|  
|MYSQL_DEBUG|调试时调试跟踪选项|  
|TMPDIR|临时表和文件的创建目录|  
有关MySQL程序使用的完整环境变量列表，请参阅[4.9 MySQL Program Environment Variables](https://dev.mysql.com/doc/refman/8.0/en/environment-variables.html)。  

本节简略介绍了mysql的程序集，接下来会对其中重要的、常用的程序进行详细的介绍，其他的程序请查看mysql的官方手册[Chapter 4 MySQL Programs](https://dev.mysql.com/doc/refman/8.0/en/programs.html)。  

---
## 2、重要的mysql程序
