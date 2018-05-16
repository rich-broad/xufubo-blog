# MySQL服务器管理
MySQL服务器（mysqld）是执行MySQL中大部分工作的主要程序。本章概述了MySQL服务器并涵盖了一般的服务器管理，具体如下：  
 - 服务器配置
 - 数据目录，特别是mysql 系统数据库
 - 服务器日志文件
 - 在一台机器上管理多台服务器
除了这些最基本的管理之外，其他的管理包括：安全、备份和恢复、复制：
 - [第6章 安全](https://dev.mysql.com/doc/refman/8.0/en/security.html)。
 - [第7章 备份和恢复](https://dev.mysql.com/doc/refman/8.0/en/backup-and-recovery.html)。
 - [第17章 复制](https://dev.mysql.com/doc/refman/8.0/en/replication.html)。  
 ---

## 1、MySQL服务器
mysqld就是MySQL服务器的可执行文件，本节将涵盖如下主题：  
 - 服务器支持的启动选项。可以在命令行上、配置文件或通过两者来指定这些选项。
 - 服务器系统变量。这些变量反映了启动选项的当前状态和值，其中一些可以在服务器运行时修改。
 - 服务器状态变量。这些变量包含运行时操作的计数器和统计信息。
 - 如何设置服务器SQL模式。此设置会修改SQL语法和语义的某些方面，例如与其他数据库系统的代码兼容性，或者控制特定情况下的错误处理。  
 - 配置和使用IPv6支持。
 - 配置和使用时区支持。
 - 服务器端帮助功能。
 - 服务器关机过程。根据表的类型（事务性或非事务性）以及是否使用复制，存在性能和可靠性方面的考虑。
有关已在MySQL 8.0中添加，弃用或删除的MySQL服务器变量和选项的列表，请参见[第1.5节 在MySQL 8.0中添加，弃用或删除服务器和状态变量和选项](https://dev.mysql.com/doc/refman/8.0/en/added-deprecated-removed.html)。  
并非所有的MySQL服务器二进制文件和配置都支持所有的存储引擎。了解如何确定MySQL服务器安装支持哪些存储引擎，见[13.7.6.16, SHOW ENGINES语法](https://dev.mysql.com/doc/refman/8.0/en/show-engines.html)。  

### 1.1 配置服务器
MySQL服务器mysqld具有许多命令选项和系统变量，可以在启动时设置它们以配置其操作。要确定服务器使用的命令选项和系统变量值，请执行以下命令：  
```shell
mysqld --verbose --help
```
该命令会生成所有mysqld选项和可配置系统变量的列表,***没有状态变量哈***。它的输出包含选项和默认值，如下所示：  
```sql
abort-slave-event-count           0
allow-suspicious-udfs             FALSE
archive                           ON
auto-increment-increment          1
auto-increment-offset             1
back-log                          80
basedir                           /home/jon/bin/mysql-8.0/
...
validate-user-plugins             TRUE
verbose                           TRUE
wait-timeout                      28800
```
要查看服务器运行时使用的当前系统变量值，请连接到它并执行以下语句：  
```sql
SHOW VARIABLES;
```
要查看正在运行的服务器的一些统计和状态指示器，请执行以下语句：  
```sql
SHOW STATUS;
```
系统变量和状态信息也可以通过 mysqladmin命令获得：  
```shell
shell> mysqladmin variables
shell> mysqladmin extended-status
```
有关所有命令行选项，系统变量和状态变量的完整说明，请参阅以下部分：
 - [第5.1.6节 “服务器命令选项”](https://dev.mysql.com/doc/refman/8.0/en/server-options.html)。
 - [第5.1.7节 “服务器系统变量”](https://dev.mysql.com/doc/refman/8.0/en/server-system-variables.html)。
 - [第5.1.9节 “服务器状态变量”](https://dev.mysql.com/doc/refman/8.0/en/server-status-variables.html)。  
除了状态变量，更详细的监控信息可从performance schema获得; 请参阅 [Chapter 25, MySQL Performance Schema](https://dev.mysql.com/doc/refman/8.0/en/performance-schema.html)。  
如果在命令行上为mysqld或mysqld_safe指定了一个选项 ，则它仅在启动服务器时才有效。要在每次运行服务器时使用该选项，请将其放入选项文件中。

---
### 1.2 服务器配置默认值
MySQL服务器有许多操作参数，你可以在服务器启动时使用命令行选项或配置文件（选项文件）更改这些参数的默认值。许多参数也可以在运行时进行更改。有关在启动或运行时设置参数的一般说明，请参见[第5.1.6节“服务器命令选项”](https://dev.mysql.com/doc/refman/8.0/en/server-options.html)和 [第5.1.7节“服务器系统变量”](https://dev.mysql.com/doc/refman/8.0/en/server-system-variables.html)。  
在windows上，MySQL安装程序与用户交互并在基本安装目录中创建一个名为my.ini的文件作为默认选项文件。  
安装完成后，你可以随时编辑默认选项文件以修改服务器使用的参数。选项文件的参数以#来注释。对于非windows平台，在服务器安装或数据目录初始化过程中不会创建默认选项文件。如果没有选项文件请参考[第4.2.6节“使用选项文件”](https://dev.mysql.com/doc/refman/8.0/en/option-files.html)来创建选项文件，服务器仅以其默认设置启动。  

---

### 1.3 服务器选项，系统变量和状态变量指引
下表列出了所有适用于mysqld的命令行选项，系统变量和状态变量。  
该表在一个统一列表中列出了命令行选项（Cmd-line），配置文件（Option文件）中有效的选项，服务器系统变量（System Var）和状态变量（Status var）。如果在命令行或选项文件中设置的服务器选项与相应服务器系统变量的名称不同，则会在相应选项的下方标记变量名称。对于系统和状态变量，变量的范围显示为（Var Scope）全局，会话或两者都支持。  

|Name|Cmd-Line|Option File|Sysytem Var|Status Var|Var Scope|Dynamic(动态的)|
|-|-|-|-|-|-|-|
|abort-slave-event-count|是|是|||||
|||||是|全局|否|

这里只是列出了几个例子，更加完整的列表，请参见MySQL官方手册:[5.1.3 服务器选项，系统变量和状态变量](https://dev.mysql.com/doc/refman/8.0/en/server-option-variable-reference.html)。  
***注意：***  
1、此选项是动态的，但只有服务器应设置此信息。你不应该手动设置这个变量的值。***这句话的意义不是很理解***  

---
### 1.4 服务器系统变量指引
也是一个表格，具体见：[服务器系统变量指引](https://dev.mysql.com/doc/refman/8.0/en/server-system-variable-reference.html)。

### 1.5 服务器状态变量
也是一个表格，具体见：[服务器状态变量指引](https://dev.mysql.com/doc/refman/8.0/en/server-status-variable-reference.html)。

### 1.6 服务器命令行选项详解


