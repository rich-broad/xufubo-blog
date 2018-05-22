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

本节简略介绍了mysql的程序集，接下来会对mysql程序如何使用进行通用的介绍，然后对其中重要的、常用的程序进行详细的介绍，其他的程序请查看mysql的官方手册[Chapter 4 MySQL Programs](https://dev.mysql.com/doc/refman/8.0/en/programs.html)。  

---
## 2、使用mysql程序
### 2.1 调用mysql程序
在linux命令行输入mysql程序名称，后边加程序的命令行参数即可，例如：  
```shell
shell> mysql --user=root test
shell> mysqladmin extended-status variables
shell> mysqlshow --help
shell> mysqldump -u root personnel
```
程序的命令行选项通常分为两种，单破折号的简写形式和双破折号的普通形式，例如：-uroot和--user=root均表示使用root账号链接mysql服务器。有时候你会发现如上形式输入,shell会提示你command not found，这通常是你的mysql安装目录没有添加到环境变量PATH中。这时你只要添加就可以了。一些选项对于许多MySQL程序是常见的。最常用的选项有： --host（或-h）， --user（或-u）和--password（或 -p）选项，它们指示运行MySQL服务器的主机，MySQL帐户的用户名和密码。所有MySQL客户端程序都支持这些选项；它们使你能够指定要连接的服务器和要在该服务器上使用的帐户。其他的连接选项如： --port（或-P）指定MySQL服务器监听的TCP/IP端口号， --socket（或-S）在Unix/Linux上指定Unix套接字文件（或Windows上命名管道的名称）。  

### 2.2 连接到mysql服务器
要连接到MySQL服务器，对于***所有的MySQL客户端***有些参数都是必须的。要连接哪台主机、哪个端口的MySQL服务器？要使用哪个账号连接？账号的密码是多少？连接上去之后要使用哪个数据库？如何进行安全加密的链接？有了这些问题，就知道MySQL客户端必须支持的选项了。这里以mysql这个最常用的客户端来说明，当然了，这些也适用于其他的客户端。针对以上问题MySQL客户端均支持如下选项：  
 - --default-auth=plugin：客户端使用的身份验证插件的提示  
 - --host=host_name或者-h host_name：要连接的服务器主机。默认值为 localhost。  
 - --password[=pass_val]或者 -p[pass_val]：MySQL帐户的密码。密码值是可选的，但如果给出，则密码和-p以及密码之中不可以有空格。最好使用-p，因为这会要求用户接下来输入密码，而且不可见，否则，Linux上的其他用户有可能会查看执行的命令，这就会增加MySQL账号密码泄露的风险。  
 - --pipe或者 –W：在Windows上，使用命名管道连接到服务器。必须启动服务器选项--enable-named-pipe，以启用命名管道连接 。  
 - --port=port_num或者 -P port_num：用于连接的端口号，用于使用TCP / IP进行连接。默认端口号为3306。  
 - --protocol={TCP|SOCKET|PIPE|MEMORY}：此选项显式指定用于连接到服务器的协议。例如：Unix上默认使用Unix套接字文件连接localhost。  
 要强制使用TCP/IP连接，请指定 --protocol选项：  
 ```shell
 shell> mysql --host=localhost --protocol=TCP
 ```
下表显示了允许的 --protocol选项值，并指出可以使用每个值的系统平台。值不区分大小写。  
  
|--protocol值|连接协议|操作系统平台|  
|-|-|-|  
|TCP|TCP/IP连接到本地或远程服务器|所有|  
|SOCKET|Unix套接字文件连接到本地服务器|仅UNIX/Linux|  
|PIPE|命名管道连接到本地或远程服务器|仅Windows|  
|MEMORY|共享内存连接到本地服务器|仅Windows|  

 - --shared-memory-base-name=name：在Windows上，要使用的共享内存名称，用于使用共享内存链接到本地服务器。默认值为MYSQL。共享内存名称区分大小写。必须以--shared-memory选项启动服务器以启用共享内存连接。  
 - --socket=file_name或者 -S file_name：在Unix上，使用Unix域套接字文件连接到本地服务器。默认的Unix套接字文件名是 /tmp/mysql.sock。在Windows上，需要指定命名管道的名称。默认的Windows管道名称是MySQL。管道名称不区分大小写。必须通过--enable-named-pipe启动服务器以启用命名管道连接方式。  
 - --ssl*：如果服务器配置了SSL支持，则该***系列***选项使客户端用SSL与服务器建立安全连接。有关详细信息，见[6.4.2 Command Options for Encrypted Connections](https://dev.mysql.com/doc/refman/8.0/en/encrypted-connection-options.html) 。  
 - --tls-version=protocol_list：客户端允许的加密连接协议。该值是一个包含一个或多个协议名称的逗号分隔列表。可以为此选项命名的协议取决于用于编译MySQL的SSL库。   
 - --user=user_name或者 -u user_name：要使用的MySQL帐户的用户名。Windows上的默认用户名为ODBC，Unix上为登录到Unix的登录名。  

为了在链接MySQL服务时少输入些参数，你可以为程序配置默认值，通常有如下方式：
 - 1、你可以在选项文件的[client]部分指定连接参数 。该文件的相关部分可能如下所示：  
 ```conf
 [client] 
 host=host_name 
 user=user_name 
 password=your_pass
 ```
 - 2、你可以使用环境变量指定一些连接参数。可以使用MYSQL_HOST为mysql指定要链接的主机 。可以使用USER（仅适用于Windows）指定MySQL用户名 。密码可以使用MYSQL_PWD指定（虽然这是不安全的）。 等等。  
### 2.3 指定mysql程序选项
有几种方法可以为MySQL程序指定选项：  
 - 在程序名称后列出命令行中的选项。  
 - 通过选项文件。  
 - 通过环境变量。  

选项按顺序处理，所以如果多次指定选项，则最后一个选项优先。以下命令会导致mysql连接到运行的服务器localhost：  
```shell
shell> mysql -h example.com -h localhost
```
如果给出冲突或相关选项，以后的选项优先于早期的选项。以下命令在“ 无列名 ”模式下运行 mysql：  
```shell
shell> mysql --column-names --skip-column-names
```
MySQL程序通过检查环境变量，然后处理选项文件，最后检查命令行来确定首先给出哪些选项。这意味着环境变量的优先级最低，命令行选项最高。对于服务器来说，有一个例外：***数据目录中的 mysqld-auto.cnf选项文件是最后处理的，所以它优先于命令行选项***。  
你可以合理的利用MySQL程序处理选项的方式，方法是在选项文件中指定程序的默认选项值。这使你可以避免在每次运行程序时输入它们，同时你也可以使用命令行选项覆盖默认值（如有必要）。  

### 2.4 在命令行上使用选项
本节简单的列举几个例子，一看就会用：   
```shell
shell> mysql -p test  --skip-grant-tables
# 和
shell> mysql -p test  --skip_grant_tables
# 等价

shell> mysqladmin --count=1K/M/G --sleep=10 ping

shell> mysql -u root -p --execute="SELECT User, Host FROM mysql.user"
Enter password: ******
+------+-----------+
| User | Host      |
+------+-----------+
|      | gigan     |
| root | gigan     |
|      | localhost |
| jon  | localhost |
| root | localhost |
+------+-----------+
shell>

shell> mysql -u root -p -e "SELECT VERSION();SELECT NOW()"
Enter password: ******
+---------------------+
| VERSION()           |
+---------------------+
| 5.8.0-m17-debug-log |
+---------------------+
+---------------------+
| NOW()               |
+---------------------+
| 2015-11-05 20:00:20 |
+---------------------+

```
### 2.5 程序选项修饰符

### 2.6 使用选项(配置)文件

### 2.7 
## 3、重要的mysql程序
