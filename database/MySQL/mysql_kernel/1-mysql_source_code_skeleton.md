---
title: 1 MySQL服务器源码骨架分析
comments: true #是否可评论 
toc: true #是否显示文章目录 
categories: "MySQL Kernel" #分类 
tags:
 - MySQL内核
 - 数据库
---

mysql服务器本质上也是一个C++程序，本节的主要目的是了解其主程序的基本结构，为此，先了解mysql源码的结构。

# 1 MySQL 源码目录介绍
下载解压mysql源码，源码中核心目录如下：  
## 1.1 BUILD 目录
MySQL 使用cmake 的方式去构建的，cmake 有两种编译方式：内部编译和外部编译，外部编译通常需要一个BUILD目录，用于存放cmake 在构建过程中生成的一些中间文件。  
## 1.2 client 目录
该目录主要是MySQL 的一些客户端程序，包括mysql.cc(mysql 可执行文件的源代码)和其他实用程序。MySQL参考手册中提到了大多数的客户端实用程序。通常这些是独立的C 程序，它们以“客户端模式”运行，即：它们调用MySQL 服务器。
该目录中的核心C 程序文件如下：
 - get_password.c ---从终端控制台获取一个密码(就是那个passwd 提示符)
 - mysql.cc ---MySQL 命令行工具
 - mysqladmin.cc ---维护MySQL 数据库的工具
 - mysqlcheck.c ---检查所有数据库，检查连接等。
 - mysqldump.c ---备份MySQL 数据库相关的东西
 - mysqlimport.c ---将不同格式的文本文件导入到MySQL 表中
 - mysqlmanager-pwgen.c --- pwgen 代表“密码生成”(目前没有维护)
 - mysqlmanagerc.c --- mysql manager 的入口点(目前没有维护)
 - mysqlshow.c ---显示数据库，表或列的信息
 - mysqltest.c ---由mysql 测试套件使用的测试程序，mysql-test-run
## 1.3 cmake 目录
还是与MySQL 的构建有关，该目录存放MySQL 构建相关的cmake 自定义模块。  
## 1.4 cmd-line-utils 目录
这里的所有文件都是“非MySQL”文件，在这个意义上，MySQL 没有生成它们，它只是使用它们。除非你正在为MySQL 编写或调试类似tty 的客户端，否则不必研究这些文件中的程序，例如mysql.exe。就是一些构建MySQL 客户端时使用的更加基础的一些命令行的通用的库，这些库非MySQL 专有，其他的程序也可以使用。因此在研究MySQL 时，没必要研究他们。
## 1.5 dbug 目录
Fred Fish 的dbug 库。这不是MySQL 的真正部分。相反，它是一组公共库，可用于调试MySQL 程序。MySQL服务器，所有的.c 和.cc 程序都支持使用这个包。  
它的使用方式：在一个常规的MySQL 程序中插入一个以DBUG_ *开头的函数调用。例如，在get_password.c 中，在一个函数的开始你会发现这一行：  
DBUG_ENTER("get_tty_password");  
在例行结束时你会发现这一行：  
DBUG_RETURN(my_strdup(to,MYF(MY_FAE)));  
这些行不影响生产代码。dbug 库的特性包括广泛的报告和分析(后者尚未被MySQL 使用)。  
## 1.6 Docs 目录
文档目录，因为MySQL 的文档都是以另外的形式在官网发布，因此，这个目录通常为空。  
## 1.7 extra 目录
一些次要的独立的实用程序。这些程序都是独立的实用程序，也就是说，它们有一个main()函数，它们的主要作用是显示MySQL 服务器需要或生成的信息。大多数都不重要。确实需要的话再进行研究。  
## 1.8 include 目录
MySQL 的头文件所在的地方。  
## 1.9 libevent 目录
就是libevent 目录。  
## 1.10 libmysql 目录
该目录中为MySQL 的库(例如windows 下为ddl，Linux 下为so 文件)，其中最为重要的就是：libmysql.c ---实现MySQL API 的代码，即：要连接到MySQL 的客户端程序必须链接到该库才能与MySQL 服务器通信。之前的老版本会有对应的libmysql_r 线程安全版本。现在已经不存在了，默认libmysql 就是线程安
全的。  
## 1.11 libmysqld 目录
嵌入式MySQL 服务器库。 libmysqld 不是一个客户/服务器，而是一个库。也不是关注和研究的重点。  
## 1.12 man 目录
man 手册页  
## 1.13 mysql-test 目录
一个MySQL测试套件。该目录有一个README 文件，说明如何运行测试，如何进行新的测试，以及如何报告错误等。  
## 1.14 mysys 目录
MySQL 系统的基础库，主要是对底层系统调用的封装，例如字符集处理、字符编码(例如base64 算法)文件处理，线程相关的锁、POSIX 线程处理、基础数据结构(例如hash 表，链表，动态数组、树等)、全局错误信息、内存管理、压缩/解压缩。这些是构建MySQL 其他代码的基础。  
## 1.15 mysys_ssl 目录
主要是用于SSL 的一些算法，例如MD5、sha1、sha2、aes 等。  
## 1.16 packaging 目录
打包等的目录  
## 1.17 plugin 目录
我们知道，MySQL 很多功能都是以插件的形式提供的，例如：UDF、一些企业级插件等，MySQL 实现架构中，该目录主要就是用于实现其插件。  
## 1.18 regex 目录
BSD Unix 中正则表达式处理的默认实现。Henry Spencer 的正则表达式库。  
## 1.19 scripts 目录
sql 批处理相关的脚本，是一个入门学习的好地方。  
## 1.20 sql 目录
我们知道，MySQL 的分成架构中包含服务器层、存储引擎层，该目录是MySQL 服务器层的实现，具体包括：服务器的启动，与客户端连接的处理，网络socket 的监听，客户端的鉴权，SQL 的词法分析、语法分析、查询的优化、连表的实现，服务器的管理(包括暂停、重启等等)、以及与存储引擎借口的交互处理，查询缓存的处理，执行计划的生成(将一个SQL 查询转换成一系列的存储引擎API 操作)，MySQL 函数的实现、主从复制、binlog等日志的处理等等。注意，MySQL 的事务是在存储引擎层做的哈，当然了，为了理解MySQL 的事务，也要理解MySQL 服务器层对事务处理到了何种程度，如何与存储引擎层进行交互等。该目录的文件非常多。不再一一列出，后续的学习中慢慢研究。  
## 1.21 sql-bench 目录
MySQL 用于基准测试的目录，里边包含一些用于基准测试的工具。  
## 1.22 sql-common 目录
存放部分服务器端和客户端都会用到的代码。  
## 1.23storage 目录
MySQL 各种存储引擎的实现目录，MySQL 源码中最为重要的就是sql 目录和该目录了。  
## 1.24 strings 目录
字符编码的实现(例如：big5、cp932、gbk、gb2312、utf8 等)，字符串函数的实现，一个程序中字符串的处理会非常频繁，因此高效的处理字符串是非常重要的，因此MySQL 实现了通用的字符串库，这些字符串处理函数的含义与C 标准库的很多都一样。只是做了一些特殊的优化。  
## 1.25 support-files
用于支持在不同系统上构建MySQL 的一些文件。这里的文件会被用于构建MySQL 的包管理器，编译器，链接器和其他构建工具使用。例如：my.cnf 文件，可以用作MySQL 的默认设置。  
## 1.26 test 目录
测试脚本所在目录。  
## 1.27 unittest 目录
用于MySQL 的单元测试的测试工具。  
## 1.28 vio 目录
虚拟I / O 库。我的理解就是对上层提供一个统一接口，然而底层的IO 可以使TCP/IP 协议的网络IO，也可以是unix 的domain socket 或者共享内存(windows 下)，命名管道等。即：上层代码只需调用这些函数即可。真正的实现会对应不同的IO，我觉得思想和Linux 的VFS 很像。   
## 1.29 win 目录
用于windows 平台编译支持的一些文件。    
## 1.30 zlib 目录
数据压缩库，用于Windows。在Unix/Linux 上，MySQL 使用libgz.a 库。它不是一个MySQL 包。它是由GNU Zip(gzip.org)人员开发的公共使用的库。  
## 1.31 libservices 目录
5.5.x 中新加的目录，实现了打印功能，非核心。  
小结：以上是5.6.36 版本中MySQL 源码目录的总结，当然了，MySQL 源码的目录会随着版本的升级会做适当的调整，但是最为核心的目录基本会保持不变的。  
对于5.7.18 又增加了如下几个目录：  
 - 1、没有了test，取而代之的是testclients。  
 - 2、增加了rapid：组复制等一些新特性，非核心功能。  
 - 3、增加了libbinlogevents：名字就是实现的功能，即：与binlog 事件相关的东西。  
 - 4、增加了libbinlogstandalone：也是与binlog 相关的东西。  

# 2 MySQL 服务器源码骨架分析
为了看到MySQL 的整体程序架构，我们先分析其main 函数，从这个函数，可以看到MySQL 的启动流程，以及启动后的状态。  
MySQL 的源码目录中，核心为sql 目录下的源代码，该目录为MySQL 的核心部分。main 函数也在该层实现，位于文件main.cc 中。函数非常简单，只是简单的调用mysqld_main 函数。因此分析mysqld_main 函数即可。首先通过分析mysqld_main 函数来了解MySQL 的程序骨架。该函数位于mysqld.cc 文件中。  
```C++
int mysqld_main(int argc, char **argv)
{
//一系列的初始化，最为重要的是各种基础资源的初始化，如线程资源的相关初始化，配置文件的装载，sql语句名字的初始化，一些全局变量的初始化，插件的初始化，日志组件的初始化，线程栈保护区大小的设置，ddl日志的恢复(因为MySQL 启动的时候从main 函数开始执行，因此可能是崩溃后的重启，因此会进行相关的恢复)，网络的初始化(主要是对套接字进行监听，简单来说，就是linux 网络编程服务器端的那几步，只不过加了一些错误处理而已)。做好这些初始化工作之后，就可以处理来自客户端的网络连接了。这时进入作为一个服务器最为核心的骨架代码了：handle_connections_sockets(); 该函数就是MySQL 主程序的核心。后边就是MySQL退出时做的一些事情了。下边详细分析函数handle_connections_sockets()。
}
```
handle_connections_sockets 函数也位于mysqld.cc 中  
handle_connections_sockets()的程序骨架就是一个大的while 循环，在这里你能够看到典型的服务端程序骨架：一个主线程负责处理监听套接字上的新连接，然后接受链接，并创建链接相关的数据结构THD，然后创建一个新的线程来处理客户端的链接，主线程继续监听套接字上的新连接。这就是MySQL 的核心主程序。在这里我们只是列出了其最为核心的部分，略去了一些错误处理。  
```C++
void handle_connections_sockets()
{
    。。。。。。。。
    while (!abort_loop))
    {
        retval= select((int) max_used_connection,&readFDs,0,0,0);
        sock = ip_sock; //sock为监听套接字
        flags= ip_flags;
        //循环尝试接受新的连接，只要连接成功返回，则结束循环，MAX_ACCEPT_RETRY被定义为10.最多尝试10次。
        for (uint retry=0; retry < MAX_ACCEPT_RETRY; retry++)
        {
        size_socket length= sizeof(struct sockaddr_storage);
        new_sock= mysql_socket_accept(key_socket_client_connection, sock,
        (struct sockaddr *)(&cAddr), &length);
        if (mysql_socket_getfd(new_sock) != INVALID_SOCKET ||
            (socket_errno != SOCKET_EINTR && socket_errno != SOCKET_EAGAIN))
            break;
        }
        //创建一个针对该链接socket的THD，同时使用各种结构初始化THD的网络部分，
        if (!(thd= new THD))
        {//如果创建THD失败，则做些错误处理(关闭socket等)继续进行循环处理，等待下一个连接到来 }
        bool is_unix_sock= (mysql_socket_getfd(sock) == mysql_socket_getfd(unix_sock));
        enum_vio_type vio_type= (is_unix_sock ? VIO_TYPE_SOCKET : VIO_TYPE_TCPIP);
        uint vio_flags= (is_unix_sock ? VIO_LOCALHOST : 0);
        vio_tmp= mysql_socket_vio_new(new_sock, vio_type, vio_flags);
        if (!vio_tmp || my_net_init(&thd->net, vio_tmp))
        {//也是一样，如果初始化网络部分失败了，则做些资源清理，然后继续下一次循环，等待链接到来。
            continue;
        }
        init_net_server_extension(thd);
        create_new_thread(thd);
    }
}
```
接着就是创建一个新的线程来处理这个链接了，create_new_thread(thd);函数位于文件mysqld.cc 中。该函数的主要逻辑为：检查当前连接数是否已经达到最大，如果是，则关闭链接，否则，允许链接，并进行下一步的处理。最核心的代码为：  
```C++
static void create_new_thread(THD *thd)
{
    mysql_mutex_lock(&LOCK_connection_count);
    if (connection_count >= max_connections + 1 || abort_loop)
    {
        //关闭链接，做些统计。
    }
    ++connection_count;
    if (connection_count > max_used_connections)
        max_used_connections= connection_count;
    mysql_mutex_unlock(&LOCK_connection_count);
    /* Start a new thread to handle connection. */
    mysql_mutex_lock(&LOCK_thread_count);
    thd->thread_id= thd->variables.pseudo_thread_id= thread_id++;
    MYSQL_CALLBACK(thread_scheduler, add_connection, (thd));
}
```
可见，最核心的是；MYSQL_CALLBACK(thread_scheduler, add_connection, (thd))。这一句是什么意思呢？    
MYSQL_CALLBACK 是个宏定义，该宏有三个参数，分别是：obj，func，params，即对象实例，对象实例中的函数指针成员，函数的参数。因此，该语句就是调用thread_scheduler 的add_connection 函数，参数为thd 即：thread_scheduler->add_connection(thd)。接着要分析函数add_connection。  
thread_scheduler 的定义如下:(位于sql/scheduler.cc)
```C++
scheduler_functions *thread_scheduler= NULL;
```
scheduler_functions 的定义如下：(位于sql/scheduler.h)  
```C++
/* Functions used when manipulating threads */
struct scheduler_functions
{
    uint max_threads;
    bool (*init)(void);
    bool (*init_new_connection_thread)(void);
    void (*add_connection)(THD *thd);
    void (*thd_wait_begin)(THD *thd, int wait_type);
    void (*thd_wait_end)(THD *thd);
    void (*post_kill_notification)(THD *thd);
    bool (*end_thread)(THD *thd, bool cache_thread);
    void (*end)(void);
};
```
该结构体的成员主要是各种为了支持线程的函数指针。该结构体的初始化在sql/scheduler.cc 中，如下：  
```C++
static scheduler_functions one_thread_per_connection_scheduler_functions=
{
    0,                                  // max_threads
    NULL,                               // init
    init_new_connection_handler_thread, // init_new_connection_thread
    create_thread_to_handle_connection, // add_connection
    NULL,                               // thd_wait_begin
    NULL,                               // thd_wait_end
    NULL,                               // post_kill_notification
    one_thread_per_connection_end,      // end_thread
    NULL,                               // end
};
```
因此对add_connection 的调用将调用到函数：create_thread_to_handle_connection，该函数位于myqld.cc 中。该函数如下：   
```C++
void create_thread_to_handle_connection(THD *thd)
{
    mysql_mutex_assert_owner(&LOCK_thread_count);
    if (blocked_pthread_count > wake_pthread)
    {
        /* 唤醒空闲的缓存的线程 */
        waiting_thd_list->push_back(thd);
        wake_pthread++;
        mysql_cond_signal(&COND_thread_cache);
    }
    else
    {
        char error_message_buff[MYSQL_ERRMSG_SIZE];
        /* Create new thread to handle connection */
        int error;
        inc_thread_created();
        thd->prior_thr_create_utime= thd->start_utime= my_micro_time();
        if ((error= mysql_thread_create(key_thread_one_connection,
                                          &thd->real_id, &connection_attrib,
                                          handle_one_connection,
                                          (void*) thd)))
        {
            /* 创建线程失败了，则进行一系列的错误处理，然后返回*/
            return;
        }
        add_global_thread(thd);
    }
}

``` 

该函数逻辑也很清晰，如果存在已经空闲的线程，则唤醒空闲的线程来处理这个链接，否则创建一个新的线程来处理这个链接，至此，主线程逻辑已经结束，它又回去监听新的网络链接去了。  

新线程将执行函数handle_one_connection。该函数位于文件sql/sql_connect.cc 中，什么也不做，只是简单的调用函数do_handle_one_connection。do_handle_one_connection 位于文件sql/sql_connect.cc 中。该函数做如下几件事情：  
 - 初始化线程；
 - 初始化THD 结构(这是一个非常重要的结构，一个链接、对应一个线程，对应一个THD)；
 - 用户授权；
 - 处理所有的在这个链接上的查询；
 - 结束线程(会将线程缓存下来用于下一个链接的处理)。

接下来结合代码具体分析，该函数代码如下：为了体现核心的逻辑，我们略去了一些代码。  
```C++
void do_handle_one_connection(THD *thd_arg)
{
    THD *thd= thd_arg;
    //初始化线程，主要是将线程pthread_detach，同时进行一些其他的初始化。
    MYSQL_CALLBACK_ELSE(thread_scheduler, init_new_connection_thread, (), 0);
    //外层循环为线程的主体逻辑，以后可用于放在线程缓存中。
    for (;;)
    {
        bool rc;
        NET *net= &thd->net;
        mysql_socket_set_thread_owner(net->vio->mysql_socket);
        //这个函数会进行用户授权，通过这个函数的授权，用户才能接着继续执行。他会调用以下函数；
        thd_prepare_connection()
            |-login_connection()
                |-check_connection()
                    |-acl_authenticate()
        rc= thd_prepare_connection(thd);
        if (rc)
            goto end_thread;
        //这个循环是该线程处理该客户端连接的核心。就是循环调用函数do_command
        while (thd_is_connection_alive(thd))
        {
            mysql_audit_release(thd);
            if (do_command(thd))
                break;
        }
        end_connection(thd);
end_thread:
        close_connection(thd);
        if (MYSQL_CALLBACK_ELSE(thread_scheduler, end_thread, (thd, 1), 0))
            return;                     // Probably no-threads
        /*
        If end_thread() returns, we are either running with
        thread-handler=no-threads or this thread has been schedule to
        handle the next connection.
        */
        thd= current_thd;
        thd->thread_stack= (char*) &thd;
    }
}
```
由上边的分析，可以看到，链接建立之后，核心的就是循环调用函数do_command 了。该函数位于文件
sql/sql_parse.cc 中，该函数读取TCP 连接上的数据包，根据MySQL 客户端和服务器之间的通信协议解析出数据包的内容，执行里边的命令。它的核心代码如下：  
```C++
bool do_command(THD *thd)
{
    thd->lex->current_select= 0;
    my_net_set_read_timeout(net, thd->variables.net_wait_timeout);
    thd->clear_error();               // Clear error message
    thd->get_stmt_da()->reset_diagnostics_area();
    net_new_transaction(net);
    thd->m_server_idle= true;
    packet_length= my_net_read(net);
    thd->m_server_idle= false;
    if (packet_length == packet_error)
    {
        //一系列的错误处理，然后返回。篇幅原因，这里省略了。
        return_value= FALSE;
        goto out;
    }
    packet= (char*) net->read_pos;
    packet[packet_length]= '\0';                /* safety */
    command= (enum enum_server_command) (uchar) packet[0];
    if (command >= COM_END)
        command= COM_END; // Wrong command
    /* Restore read timeout value */
    my_net_set_read_timeout(net, thd->variables.net_read_timeout);
    return_value= dispatch_command(command, thd, packet+1, (uint) (packet_length-1));
out:
    /* The statement instrumentation must be closed in all cases. */
    DBUG_ASSERT(thd->m_digest == NULL);
    DBUG_ASSERT(thd->m_statement_psi == NULL);
    DBUG_RETURN(return_value);
}
```
该函数最核心的就是两步：my_net_read 和dsipatch_command 函数。  

前一个函数从客户端获取完整的数据包(如果包太大，客户端分开传送的，那么它会把包读取完整的)，解压缩它，并去掉一些头部字段。一旦完成，我们得到字节数组，其中包含客户端发送的内容。第一个字节很重要，因为它标识了消息的类型。我们将把它和其余的数据包传递给dispatch_command 函数。例如我们的一个查询:  
```sql
select * from test1;
```
那么将会收到如下参数，当然了，不同的体系结构thd，packet+1 会有所不同。如下：  
```C++
dispatch_command (COM_QUERY, 0x33b5940, packet=0x338e9e1 "select * from test1", packet_length=19)
```
该函数位于sql/sql_parse.cc 中，该函数有700 多行，其主要功能是：对每个客户端传过来的命令进行处理。虽然很长，但是代码结构非常清晰，具体如下：  
```C++
bool dispatch_command(enum enum_server_command command, THD *thd,
char* packet, uint packet_length)
{
    NET *net= &thd->net;
    ...............
    thd->profiling.start_new_query();
    thd->set_command(command);
    .............
    thd->set_query_id(next_query_id());
    inc_thread_running();
    ...............
    switch (command) 
    {
        case COM_INIT_DB:
        case COM_REGISTER_SLAVE:
        case COM_CHANGE_USER:
        case COM_STMT_EXECUTE:
        。。。。。。。。。。
        case COM_QUERY:
        case COM_FIELD_LIST: // This isn't actually needed
        case COM_QUIT:
        case COM_BINLOG_DUMP_GTID:
        case COM_BINLOG_DUMP:
        case COM_REFRESH:
        case COM_SHUTDOWN:
        case COM_STATISTICS:
        case COM_PING:
        case COM_PROCESS_INFO:
        case COM_PROCESS_KILL:
        case COM_SET_OPTION:
        {
            status_var_increment(thd->status_var.com_stat[SQLCOM_SET_OPTION]);
            uint opt_command= uint2korr(packet);
            switch (opt_command) 
            {
                case (int) MYSQL_OPTION_MULTI_STATEMENTS_ON:
                case (int) MYSQL_OPTION_MULTI_STATEMENTS_OFF:
                default:
            }
            break;
        }
        case COM_DEBUG:
        case COM_SLEEP:
        case COM_CONNECT: // Impossible here
        case COM_TIME: // Impossible from client
        case COM_DELAYED_INSERT:
        case COM_END:
        default:
            break;
    }
done:
    thd->update_server_status();
    thd->protocol->end_statement();
    query_cache_end_of_result(thd);
    thd->set_command(COM_SLEEP);
    MYSQL_END_STATEMENT(thd->m_statement_psi, thd->get_stmt_da());
    ......................
    dec_thread_running();
    thd->packet.shrink(thd->variables.net_buffer_length); // Reclaim some memory
    free_root(thd->mem_root,MYF(MY_KEEP_PREALLOC));
    ......................
    thd->profiling.finish_current_query();
}
```
就是一系列的case，一共27 个，针对每一种情况执行对应的case。例如：客户端执行use test，则表示要改变当前的数据库，然后就命中COM_INIT_DB 这个case 了。如果客户端要执行预编译好的SQL 语句，则命中COM_STMT_EXECUTE 这个case 了，再比如，客户端执行select * from test1，则命中COM_QUERY(通用查询，MySQL 中不是select 才叫查询。MySQL 中很多操作，比如update，insert 等都叫一次查询。)这个case 了。我们知道，MySQL 是插件式存储引擎，那么接下来我们以一种场景为例来说明MySQL Server 层如何与存储引擎层进行交互。显然，必须MySQL Server 层定义好API，然后存储引擎层去实现这些API，那么MySQL Server 层肯定需要将语句的执行计划对应到对API 的调用上。这只是我们的分析，具体看看是不是这样呢？现在以插入一条数据为例进行分析：  

如上分析，也将命中COM_QUERY 这个case，这个case 下，主要是做一些sql 语句处理前的准备，然后调用函数mysql_parse 来处理sql 语句。其核心就是mysql_parse。该函数位于sql/sql_parse.cc 中，负责具体的sql 处理，其核心逻辑有两个：因为sql 的词法分析、语法分析、查询重写、查询优化、物理执行计划生成、执行物理计划需要很长时间，因此，数据库系统通常会对查询进行缓存，当一个查询(广义的查询)来了之后首先会查看缓存，如果命中，则直接返回，如果没有命中查询缓存，则进行下一步的处理即：sql 的分析、查询计划的生成、执行等。   

下边针对没有命中查询缓存的情况进行分析：  
首先调用parse_sql，该函数只是简单的调用函数MYSQLparse，MYSQLparse 其实会在预处理阶段被替换为yyparse，yyparse 就是gnu 的Bison 生成的语法解析器的入口，该函数会生成语法树，MySQL 的词法分析是自己做的，该函数执行之后，SQL 语句的语法分析结果会被放在TDH 的LEX 这个对象中，该对象包含了sql 语句的信息，例如：什么样的sql 语句，查询的那些表，where 条件信息，选择那些列等等。   


对sql 分析完成之后，如果分析成功，则会进行查询重写，注意：查询缓存只处理SELECT，我们也不会对SELECT进行查询重写。**而且一定要区分开查询重写与查询优化完全是两码事哈，查询优化还早着呢**。   
经过sql 分析和查询重写，然后就是对其进行执行了。这一步由函数mysql_execute_command 来完成，位于sql/sql_parse.cc 中，需要详细分析，该函数非常长，大约2800 多行，虽然很长，但是逻辑也非常清晰，就是一堆case，这堆case 就是对应sql 语法分析器分析出来的要执行的sql 命令。由此可见，代码的处理结构还是非常的清晰的。例如：在语法分析中的一个代码片段(词法分析、语法分析的相关知识可以查阅相关书籍)：  
```C++
......
    break;
case 1748:
    {
        LEX *lex= Lex;
        lex->sql_command= SQLCOM_SHOW_TABLE_STATUS;
        lex->select_lex.db= (yyvsp[(3) - (4)].simple_string);
        if (prepare_schema_table(YYTHD, lex, 0, SCH_TABLES))
            MYSQL_YYABORT;
    }
    break;
case 1749:
    {
        LEX *lex= Lex;
        lex->sql_command= SQLCOM_SHOW_OPEN_TABLES;
        lex->select_lex.db= (yyvsp[(3) - (4)].simple_string);
        if (prepare_schema_table(YYTHD, lex, 0, SCH_OPEN_TABLES))
            MYSQL_YYABORT;
    }
    break;
case 1750:
......
```
如上：例如在语法分析中，语法分析器发现要执行的SQL 语句是SQLCOM_SHOW_OPEN_TABLES，那么在函数mysql_execute_command 中也会有对应的case 来处理这个命令的。例如在函mysql_execute_command 中就有对应的如下代码段：  
```C++
.................
case SQLCOM_SHOW_TABLE_STATUS:
case SQLCOM_SHOW_OPEN_TABLES:
.........
case SQLCOM_SELECT:
{
    thd->status_var.last_query_cost= 0.0;
    thd->status_var.last_query_partial_plans= 0;
    if ((res= select_precheck(thd, lex, all_tables, first_table)))
        break;
    res= execute_sqlcom_select(thd, all_tables);
    break;
}
.......
```

为了逻辑清晰，在此总结一下MySQL 服务器的骨架代码(后续还会分析如何与存储引擎的API 进行交互)。  
```C++
//MySQL服务器主线程
int main()
{
    mysqld_main()
    {
        my_init();
        udf_init();
        .....
        init_slave();
        network_init();
        .....
        handle_connections_sockets()
        {
            while()
            {//循环监听socket上的连接，对于每个连接，创建一个线程去处理该链接上的查询。
                select();
                accept();
                create_new_thread()
                {
                    create_thread_to_handle_connection()
                    {
                        mysql_thread_create(,, ,handle_one_connection,(void*) thd);
                        //在此，主线程又进入对socket的监听，等待新连接的到来，而新线程处理该链接。
                    }
                } //end of create_new_thread
            } //end of while
        } // end of handle_connections_sockets
    } //end of mysqld_main
}// end of main

//MySQL服务器处理链接的线程开始执行
//如上，MySQL开始处理新的链接，执行函数handle_one_connection
handle_one_connection()
{
    do_handle_one_connection()
    {
        ......
        thd_prepare_connection(); //检查用户权限，看看是否允许链接
        while () //循环处理该连接上的各种命令(各种服务器管理命令、SQL命令等)
        {
            do_command(thd)
            {
                my_net_read(); //读取完整的客户端命令
                ...... //解析客户端的命令
                dispatch_command(enum enum_server_command)//命令分发
                {//注意：客户端和服务端之间的命令分为两层，该函数只识别第一层命令。
                    switch (command) 
                    {
                        ........
                        case COM_STMT_EXECUTE:
                        ..........
                        case COM_QUERY:
                        ..........
                    }
                }
            }
        }
    }
}
/*
* dispatch_command中处理第一层命令，即：mysql客户端和服务端通信协议中粒度比较粗的命令，
* 即: enum_server_command中定义的命令，这些命令是针对整个MySQL服务器而言的，例如：
* COM_REGISTER_SLAVE: 注册从属服务器
* COM_CHANGE_USER: 改变当前的链接用户
* COM_QUERY: 让服务器处理一个查询(注意: MySQL中的查询是广义的，包含：DDL和DML等语句)
* COM_SHUTDOWN: 服务器的关闭
* COM_PROCESS_KILL: kill的处理
* 以上只是举了几个简单的例子，enum_server_command的完整定义在mysql_comm.h中。
*
* 对于COM_QUERY，MySQL会对各种SQL语句进行解析。解析出SQL相关的命令(enum_sql_command)
* 来进行执行，这个是由函数mysql_execute_command来完成的。enum_sql_command枚举的定义
* 在sql_cmd.h中。可见MySQL从代码上就是逐渐分层，sql的处理虽然重要，但只是一个MySQL服务
* 器的一个模块。具体enum_server_command和enum_sql_command的定义可以查看对应的.h文件。
*/

//接着分析链接线程中dispatch_command函数最为重要的一个case分支,case COM_QUERY:
case COM_QUERY:
{
    MYSQL_QUERY_START();
    mysql_parse()
    {
        if (query_cache_send_result_to_client(thd, rawbuf, length) <= 0)
        {   //没有命中查询缓存
            //对sql语句进行词法分析,语法分析,上边说的enum_sql_command就在这里解析出来的。
            parse_sql();
            mysql_rewrite_query(); //必要时进行查询重写,注意: select不会重写。
            .......
            mysql_execute_command(THD *thd)
            {
                switch (lex->sql_command)
                {
                    case SQLCOM_SHOW_STATUS_PROC:
                    case SQLCOM_SHOW_PROFILE:
                    ........
                    case SQLCOM_SELECT:
                    .........
                }
            }
        }
        else
        {
            //查询命中,做点处理,写日志
        }
    }
}
```

至此，服务器的大体骨架就清楚了，接下来研究MySQL 服务器如何同存储引擎曾进行交互。  

