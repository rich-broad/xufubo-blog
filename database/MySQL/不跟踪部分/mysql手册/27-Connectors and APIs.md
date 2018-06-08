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

### 7.6 C API函数概述
C API函数很多，关于它们的描述这里并不详细列出来，主要是说明使用上遵循的基本大纲：  
 - 1、调用mysql_library_init（）来初始化MySQL客户端库。  
 - 2、调用mysql_init（）来初始化连接处理程序，并通过调用mysql_real_connect（）连接到服务器。  
 - 3、发出SQL语句并处理它们的结果。（下边会详细解释）。  
 - 4、调用mysql_close（）关闭与MySQL服务器的连接。  
 - 5、调用mysql_library_end（）。
调用mysql_library_init（）和mysql_library_end（）的目的是为了正确初始化和销毁MySQL客户端库。在非多线程环境中，可以省略对mysql_library_init（）的调用，因为mysql_init（）将根据需要自动调用它。但是，在多线程环境中，mysql_library_init（）不是线程安全的，因此你必须在创建任何线程之前调用该函数，或者在mutex的保护下来调用该函数（不管是直接还是间接的）。这应该在任何其他客户端库调用之前完成。  
要连接到服务器，请调用mysql_init（）来初始化连接处理程序（MYSQL），然后使用该处理程序调用mysql_real_connect（）。连接时，mysql_real_connect（）在5.0.3以前的API版本中将重连标志（MYSQL结构的一部分）设置为1，在较新版本中设置为0。重连标志的值为1表示如果由于连接丢失而无法执行语句时，mysql客户端会在放弃之前尝试重新连接到服务器。也可以使用mysql_options（）的MYSQL_OPT_RECONNECT选项来控制重新连接行为。当你使用完成后调用mysql_close（）来终止连接。  
当连接处于活动状态时，客户端可以使用mysql_query（）或mysql_real_query（）向服务器发送SQL语句。两者之间的区别是：mysql_query接收的参数为C风格字符串，mysql_real_query接收的参数为带有计数的字符串。如果字符串包含二进制数据（可能包含空字节），则必须使用 mysql_real_query()。**在此，建议只用mysql_real_query()函数**。  
对于每个非SELECT查询（例如，INSERT，UPDATE，DELETE），通过调用mysql_affected_rows（）可以找出有多少行被更改（受影响）。  
客户端有两种处理结果集的方法。一种方法是通过调用mysql_store_result（）来一次性接收整个结果集。该函数从服务器获取查询返回的所有行并将它们存储在客户端中。第二种方法是让客户端通过调用mysql_use_result（）来启动逐行获取结果集，该函数初始化获取必要的操作，但实际上并未从服务器获取任何行。在这两种情况下，通过调用mysql_fetch_row（）来获取行。 对于mysql_store_result（）而言，mysql_fetch_row（）将访问之前从服务器获取的并存储在客户端的结果集(不在需要网络通信，mysql服务器的通信缓冲区可以释放并处理更多的查询，最为常用)。对于mysql_use_result（），mysql_fetch_row（）将从服务器获取行(将进行网络通信，并占用服务器资源，不常用)。不过具体使用哪种行为，还得看具体情况。有关每行数据大小的信息可通过调用mysql_fetch_lengths（）获得。  
处理完结果集之后，调用mysql_free_result（）来释放它占用的内存。  
mysql_store_result（）的一个优点是，因为所有的行都被存储到客户端，所以你不仅可以顺序访问行，也可以使用mysql_data_seek（）或mysql_row_seek（）在结果集中来回移动以更改结果集中的当前行位置来访问行。你也可以通过调用mysql_num_rows（）来查看一共有多少行。另一方面，对于大型结果集，mysql_store_result（）的内存要求可能非常高，你更可能遇到内存不足的情况。  
mysql_use_result（）的一个优点是客户端对结果集所需的内存较少，因为它一次只维护一行。缺点是你必须快速处理每一行以避免客户端捆绑服务器，来占用服务器资源，对于繁忙的数据库服务器通常不这么做。你也不能随机访问结果集中的行（只能按顺序访问行），并且结果集中的行数是未知的，直到全部检索完为止。此外，即使你在检索过程中确定找到了你要查找的信息，也必须检索所有行，因为如果你不全部从服务器检索完，服务器不知道你是否需要剩下的行，服务器无法释放资源，会一直保留，并且返回给下一次查询，这会导致错误发生，因此必须检索完毕。  
API可以判断出我们通过 mysql_query()或mysql_real_query()执行的是否是一个SELECT语句。你可以通过在每个mysql_query（）（或mysql_real_query（））之后调用mysql_store_result（）来完成此操作。如果结果集调用成功，则该语句是一个SELECT，你可以读取这些行。如果结果集调用失败，请调用mysql_field_count（）来确定结果是否确实是预期的。如果mysql_field_count（）返回零，则语句不返回任何数据（表示它是INSERT，UPDATE，DELETE等等这些非SELECT语句，注意：像 SHOW, DESCRIBE, EXPLAIN都是SELECT类语句，因为它们都返回结果集），并不希望返回数据行。如果mysql_field_count（）不为零，则表示该语句应该返回数据行，却没有返回。这表明该语句是一个失败的SELECT类语句，这个过程如下：  
```C
MYSQL_RES *result;
unsigned int num_fields;
unsigned int num_rows;

if (mysql_query(&mysql,query_string))
{
    // error
}
else // query succeeded, process any data returned by it
{
    result = mysql_store_result(&mysql);
    if (result)  // there are rows
    {
        num_fields = mysql_num_fields(result);
        // retrieve rows, then call mysql_free_result(result)
    }
    else  // mysql_store_result() returned nothing; should it have?
    {
        if(mysql_field_count(&mysql) == 0)
        {
            // query does not return data
            // (it was not a SELECT)
            num_rows = mysql_affected_rows(&mysql);
        }
        else // mysql_store_result() should have returned data
        {
            fprintf(stderr, "Error: %s\n", mysql_error(&mysql));
        }
    }
}
```
更加详细的说明，见：[27.7.7.22 mysql_field_count()](https://dev.mysql.com/doc/refman/8.0/en/mysql-field-count.html)。  
如上，我们看到，当mysql_store_result返回NULL时会调用mysql_field_count来判断是否真的发生了错误，可见mysql_store_result返回为NULL不一定表示失败，具体的见：[27.7.25.1 Why mysql_store_result() Sometimes Returns NULL After mysql_query() Returns Success](https://dev.mysql.com/doc/refman/8.0/en/null-mysql-store-result.html)。  
mysql_store_result（）和mysql_use_result（）都使你可以获得有关组成结果集的字段（字段数，它们的名称和类型等）的信息。你可以通过重复调用mysql_fetch_field（）来依次访问行内的字段信息，也可以通过行的字段号调用mysql_fetch_field_direct()来获取字段信息。通过调用mysql_field_seek（）可以更改当前字段位置。设置字段位置会影响对mysql_fetch_field（）的后续调用（可见，mysql_fetch_field（）依赖于结果集中字段的游标位置，只是每次简单的往前移动）。你也可以通过调用mysql_fetch_fields（）一次获得所有字段的信息。这些例子如下：  
```C
MYSQL_FIELD *field;

while((field = mysql_fetch_field(result)))
{
    printf("field name %s\n", field->name);
}
```

```C
unsigned int num_fields;
unsigned int i;
MYSQL_FIELD *field;

num_fields = mysql_num_fields(result);
for(i = 0; i < num_fields; i++)
{
    field = mysql_fetch_field_direct(result, i);
    printf("Field %u is %s\n", i, field->name);
}
```
```C
unsigned int num_fields;
unsigned int i;
MYSQL_FIELD *fields;

num_fields = mysql_num_fields(result);
fields = mysql_fetch_fields(result);
for(i = 0; i < num_fields; i++)
{
   printf("Field %u is %s\n", i, fields[i].name);
}
```

为了检测和报告错误，MySQL通过mysql_errno（）和mysql_error（）函数提供对错误信息的访问.这些将返回最近调用的函数的错误代码或错误消息。从而使你能够确定何时发生错误以及它是什么。  

以上是C API使用的基本大纲，至于每一个函数详细的描述这里不在啰嗦，现用现查。后边会从源码的角度分析一下一些非常重要的函数的实现。  
  
### 7.7 








