# 连接mysql的API
mysql连接器为客户端程序提供到MySQL服务器的连接，mysql支持的连接器很多，API提供对MySQL协议和MySQL资源的低级访问。Oracle开发了许多连接器，很多连接器都依赖mysql C API，它提供对MySQL客户端/服务器协议的低级访问。这是用于连接到mysql服务器实例的主要方法，并且由mysql命令行客户端(这个最常用了)以及许多mysql连接器和第三方API使用。即：其他的就是对这些API的调用和封装。要从C应用程序访问mysql服务器或者为新的语言构建访问mysql服务器的API，那么详细的研究和学习C API将是必不可少的。本章就详细的研究这些C API。  
如上我们知道，很多第三方连接器API都是借助mysql C API实现的，不过有些第三方连接器API不借助mysql C API，而是自己实现自己的驱动程序，例如：Connector/J、Connector/Python等，更多信息见：[Table 27.1 MySQL APIs and Interfaces](https://dev.mysql.com/doc/refman/8.0/en/connectors-apis.html#connectors-apis-summary)。  
接下来我们详细的学习mysql C API。  

## 7、mysql C API
C API提供对MySQL客户端/服务器协议的底层访问，并使C程序能够访问数据库内容。C API代码随MySQL分发并在libmysqlclient库中实现。大多数的其他客户端API使用libmysqlclient库与MySQL服务器进行通信。（例外是Connector/J和Connector/Net）。  
 > - **注意：**
  如果在升级之后遇到了编译问题，例如*Commands out of sync*或者coredump，程序可能是使用了旧的头文件或库文件编译的。在这种情况下，请检查用于编译的mysql.h文件和libmysqlclient.a库的日期，以验证它们来自新的MySQL分发版。如果没有，请用新的头文件和库重新编译程序。如果库的主要版本号已更改，则对于针对共享客户端库编译的程序也可能需要重新编译（例如，从libmysqlclient.so.17到libmysqlclient.so.18）。有关其他兼容性信息，请参阅[27.7.4.4 Running C API Client Programs](https://dev.mysql.com/doc/refman/8.0/en/c-api-running-clients.html)。  
客户端具有最大的通信缓冲区大小。最初分配的缓冲区大小（16KB）会自动增大到最大值（默认情况下为16MB）。由于缓冲区大小仅在需要时才增加，因此仅增加最大限制本身并不会导致使用更多资源。通信缓冲区必须足够大以包含单个SQL语句（用于客户端到服务器的流量）和一行返回的数据（用于服务器到客户端的流量）。每个会话的通信缓冲区都会动态增加，以处理任何查询或达到最大的限制。例如，如果BLOB值包含高达16MB的数据，则必须具有至少16MB的通信缓冲区限制（在服务器和客户端中）。客户端库中内置的默认最大值为1GB，但服务器中的默认最大值为1MB，可以通过在服务器启动时更改max_allowed_pa​​cket参数的值来增加此值。每次查询后，MySQL服务器将每个通信缓冲区缩小为net_buffer_length个字节。对于客户端来说，与连接关联的缓冲区的大小在连接关闭之前不会减少，连接关闭时客户端内存将被回收。  

### 7.1 MySQL C API实现
MySQL C API是一个基于C的API，在Linux/UNIX上，静态库是libmysqlclient.a，动态库是libmysqlclient.so。例如：
```shell
[xufubo@localhost ~]$ cd /usr/local/mysql/lib/
[xufubo@localhost lib]$ ll
total 34656
-rw-r--r--. 1 root root 24460452 May 20 23:22 libmysqlclient.a
lrwxrwxrwx. 1 root root       20 May 21 00:31 libmysqlclient.so -> libmysqlclient.so.21
lrwxrwxrwx. 1 root root       25 May 21 00:31 libmysqlclient.so.21 -> libmysqlclient.so.21.0.11
-rwxr-xr-x. 1 root root 10969144 May 20 23:22 libmysqlclient.so.21.0.11
... ...
```

### 7.2 同时安装MySQL服务器和Connector/C
具体见[27.7.2 Simultaneous MySQL Server and Connector/C Installations](https://dev.mysql.com/doc/refman/8.0/en/c-api-multiple-installations.html)。  

### 7.3 C API 客户端程序举例
MySQL源代码发行版中的许多客户端都是用C编写的，比如[mysql](https://dev.mysql.com/doc/refman/8.0/en/mysql.html)，[mysqladmin](https://dev.mysql.com/doc/refman/8.0/en/mysqladmin.html)和[mysqlshow](https://dev.mysql.com/doc/refman/8.0/en/mysqlshow.html)。如果你正想学习如何使用C API，请查看这些客户端的代码，那么获取其源代码，并在client 目录中查看。例如：  
```shell
fuboxu@fuboxu-PC3 MINGW64 /f/code/mysql/mysql-boost-8.0.11/mysql-8.0.11/client
$ ll
total 1156
drwxr-xr-x 1 fuboxu 1049089      0 四月  8 15:01 base/
drwxr-xr-x 1 fuboxu 1049089      0 四月  8 15:01 check/
-rw-r--r-- 1 fuboxu 1049089   5592 四月  8 14:44 client_priv.h
-rw-r--r-- 1 fuboxu 1049089   4619 四月  8 14:44 CMakeLists.txt
-rw-r--r-- 1 fuboxu 1049089   5433 四月  8 14:44 completion_hash.cc
-rw-r--r-- 1 fuboxu 1049089   2463 四月  8 14:44 completion_hash.h
drwxr-xr-x 1 fuboxu 1049089      0 四月  8 15:01 dump/
-rw-r--r-- 1 fuboxu 1049089   1853 四月  8 14:44 echo.cc
-rw-r--r-- 1 fuboxu 1049089   6148 四月  8 14:44 get_password.cc
-rw-r--r-- 1 fuboxu 1049089   2004 四月  8 14:44 infix_ostream_it.h
-rw-r--r-- 1 fuboxu 1049089   1990 四月  8 14:44 logger.cc
-rw-r--r-- 1 fuboxu 1049089   2375 四月  8 14:44 logger.h
-rw-r--r-- 1 fuboxu 1049089   1984 四月  8 14:44 my_readline.h
-rw-r--r-- 1 fuboxu 1049089 167522 四月  8 14:44 mysql.cc
-rw-r--r-- 1 fuboxu 1049089  37725 四月  8 14:44 mysql_config_editor.cc
-rw-r--r-- 1 fuboxu 1049089  31271 四月  8 14:44 mysql_secure_installation.cc
-rw-r--r-- 1 fuboxu 1049089  20248 四月  8 14:44 mysql_ssl_rsa_setup.cc
-rw-r--r-- 1 fuboxu 1049089  51509 四月  8 14:44 mysqladmin.cc
-rw-r--r-- 1 fuboxu 1049089 104889 四月  8 14:44 mysqlbinlog.cc
-rw-r--r-- 1 fuboxu 1049089   2051 四月  8 14:44 mysqlbinlog.h
-rw-r--r-- 1 fuboxu 1049089 200395 四月  8 14:44 mysqldump.cc
-rw-r--r-- 1 fuboxu 1049089  24617 四月  8 14:44 mysqlimport.cc
-rw-r--r-- 1 fuboxu 1049089  27599 四月  8 14:44 mysqlshow.cc
-rw-r--r-- 1 fuboxu 1049089  67907 四月  8 14:44 mysqlslap.cc
-rw-r--r-- 1 fuboxu 1049089 337033 四月  8 14:44 mysqltest.cc
-rw-r--r-- 1 fuboxu 1049089   4885 四月  8 14:44 path.cc
-rw-r--r-- 1 fuboxu 1049089   2339 四月  8 14:44 path.h
-rw-r--r-- 1 fuboxu 1049089   3329 四月  8 14:44 pattern_matcher.cc
-rw-r--r-- 1 fuboxu 1049089   2076 四月  8 14:44 pattern_matcher.h
-rw-r--r-- 1 fuboxu 1049089   8479 四月  8 14:44 readline.cc
drwxr-xr-x 1 fuboxu 1049089      0 四月  8 15:01 upgrade/
```
你可以看看上边的与mysql通信的客户端的实现。  

### 7.4 构建和运行C API客户端程序
#### 7.4.1 构建C API客户端程序
mysql主要作为数据库服务器在Linux/UNIX上使用，我们这里主要学习mysql客户端程序在linux系统上的编译，对于windows上的编译连接，请参考其手册。  
这里假设mysql安装的基础目录为/usr/local/mysql,而且其他的都使用默认值，编译器使用gcc，那么头文件将安装在/usr/local/mysql/include,库文件安装在/usr/local/mysql/lib。  
在编译时需要加上如下选项：  
```shell
-I/usr/local/mysql/include
```
连接时：  
```shell
-L/usr/local/mysql/lib -lmysqlclient
```
系统上的路径名称可能有所不同。根据需要调整 -I和-L选项。  
为了在Linux/Unix上更简单的编译MySQL程序，使用 [mysql_config](https://dev.mysql.com/doc/refman/8.0/en/mysql-config.html)脚本。mysql_config脚本显示编译或链接所需的选项：  
```shell
[xufubo@localhost include]$ mysql_config --cflags
-I/usr/local/mysql/include
[xufubo@localhost include]$ mysql_config --libs
-L/usr/local/mysql/lib -lmysqlclient -lpthread -lm -lrt -lssl -lcrypto -ldl
```
获取这些选项之后，将他们加到编译和连接选项中。或者：  
```shell
shell> gcc -c `mysql_config --cflags` progname.c
shell> gcc -o progname progname.o `mysql_config --libs`
```
在Unix上，链接默认使用动态库。要链接到静态客户端库，请将其路径名添加到链接命令中。例如，如果库位于 /usr/local/mysql/lib，链接如下所示：  
```shell
shell> gcc -o progname progname.o /usr/local/mysql/lib/libmysqlclient.a
shell> gcc -o progname progname.o `mysql_config --variable=pkglibdir`/libmysqlclient.a
```
可以使用pkg-config作为mysql_config的替代方法来获取信息。例如，以下几对命令是等价的：  
```shell
mysql_config --cflags
pkg-config --cflags mysqlclient

mysql_config --libs
pkg-config --libs mysqlclient
```
使用静态链接，请使用以下命令：  
```shell
pkg-config --static --libs mysqlclient
```
更多信息见[27.7.4.2 Building C API Client Programs Using pkg-config](https://dev.mysql.com/doc/refman/8.0/en/c-api-building-clients-pkg-config.html)。  

#### 7.4.2 使用pkg-config构建C API客户端程序
MySQL发行版包含一个mysqlclient.pc文件，该文件提供有关MySQL配置的信息以供pkg-config命令使用。这使得 pkg-config可以用来替代mysql_config来获得编译MySQL应用程序所需的编译器选项或链接库等信息。具体的见：
[27.7.4.2 Building C API Client Programs Using pkg-config](https://dev.mysql.com/doc/refman/8.0/en/c-api-building-clients-pkg-config.html)。  

#### 7.4.3 编写C API多线程客户端程序
客户端库***几乎***都是线程安全的。
**********************这个要好好了解下**********************

#### 7.4.4 运行C API客户端程序
这个很简单，不在叙述，见[27.7.4.4 Running C API Client Programs](https://dev.mysql.com/doc/refman/8.0/en/c-api-running-clients.html)。  

#### 7.4.5 C API 服务器和客户端库版本
mysql服务器版本的值在编译时由MYSQL_SERVER_VERSION和 MYSQL_VERSION_ID宏的值指定。在运行时通过mysql_get_server_info()和 mysql_get_server_version() 函数获取。  
mysql客户端库的版本取决于库的分发类型：  
 - 对于mysql发行版，客户端的库版本在编译时也是由MYSQL_SERVER_VERSION和 MYSQL_VERSION_ID宏指定，在运行时通过mysql_get_client_info()和 mysql_get_client_version() 函数获取。LIBMYSQL_VERSION和LIBMYSQL_VERSION_ID宏具有与MYSQL_SERVER_VERSION和MYSQL_VERSION_ID相同的值，并且这两组宏可以互换使用。因为在mysql的源码中，有如下文件：  
  mysql-8.0.11\include\mysql_version.h.in
  ```
  /* Version numbers for protocol & mysqld */
  
  #ifndef _mysql_version_h
  #define _mysql_version_h
  
  #define PROTOCOL_VERSION            @PROTOCOL_VERSION@
  #define MYSQL_SERVER_VERSION       "@VERSION@"
  #define MYSQL_BASE_VERSION         "mysqld-@MYSQL_BASE_VERSION@"
  #define MYSQL_SERVER_SUFFIX_DEF    "@MYSQL_SERVER_SUFFIX@"
  #define MYSQL_VERSION_ID            @MYSQL_VERSION_ID@
  #define MYSQL_PORT                  @MYSQL_TCP_PORT@
  #define MYSQL_PORT_DEFAULT          @MYSQL_TCP_PORT_DEFAULT@
  #define MYSQL_UNIX_ADDR            "@MYSQL_UNIX_ADDR@"
  #define MYSQL_CONFIG_NAME          "my"
  #define MYSQL_PERSIST_CONFIG_NAME  "mysqld-auto"
  #define MYSQL_COMPILATION_COMMENT  "@COMPILATION_COMMENT@"
  #define LIBMYSQL_VERSION           "@VERSION@"
  #define LIBMYSQL_VERSION_ID         @MYSQL_VERSION_ID@
  #define SYS_SCHEMA_VERSION         "@SYS_SCHEMA_VERSION@"
  
  #ifndef LICENSE
  #define LICENSE                     GPL
  #endif /* LICENSE */
  
  #endif /* _mysql_version_h */
  
  ```
 - 对于Connector/C发行版，客户端库版本是Connector/C版本。在编译时由 LIBMYSQL_VERSION和LIBMYSQL_VERSION_ID宏指定，在运行时通过mysql_get_client_info()和 mysql_get_client_version() 函数获取。其实我们上边也说了，LIBMYSQL_VERSION和LIBMYSQL_VERSION_ID宏具有与MYSQL_SERVER_VERSION和MYSQL_VERSION_ID相同的值。  

### 7.5 C API 数据结构
本节介绍C API数据结构，而不是用于预处理语句（prepared statements）或复制流接口的那些数据结构。主要包含如下结构：  
 - MYSQL：代表一个mysql连接及其处理程序。不要复制它。  
 - MYSQL_RES：此结构表示返回行（SELECT，SHOW，DESCRIBE，EXPLAIN）的查询的结果。  
 - MYSQL_ROW：一行数据。它目前作为一个字节字符串数组来实现。  
 - MYSQL_FIELD：这个结构包含元数据：有关字段的信息，例如字段的名称，类型和大小。可以通过重复调用mysql_fetch_field（）来获取每个字段的MYSQL_FIELD结构。字段的值不是这个结构的一部分，这些值被包含在MYSQL_ROW结构中。  
 - MYSQL_FIELD_OFFSET：字段列表中的偏移量。偏移量是一行中的字段号，从零开始。  
 - my_ulonglong：很多函数(mysql_affected_rows（），mysql_num_rows（）和mysql_insert_id（）)返回该类型，其底层实现为：typedef unsigned long long my_ulonglong。此类型提供0到1.84e19范围的值。一些返回该类型表示行数的函数通过返回-1来表示错误的发生，因此你可以将返回值同(my_ulonglong)-1做比较，以检查函数返回是否出错。  
 - my_bool：bool类型，0为false，非0为true。my_bool类型在MySQL 8.0之前使用。从MySQL 8.0开始，改为使用bool或int C类型。  
这些结构体的定义包含在mysql.h中。这里不详细列出。  




