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

一个 
[典型的安装](2-Installing and Upgrading MySQL.md) 
后，包含的程序如下：  
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
