---
title: MySQL安装和升级
---
# 安装和更新MySQL
本章主要学习MySQL的下载、安装、升级。和其他软件的安装一样，mysql的安装通常也包括如下几个步骤：  
 - 1、确定mysql是否支持你的平台。[地址](https://www.mysql.com/support/supportedplatforms/database.html)。
 - 2、选择要安装的发行版和版本(二进制还是源码)。
 - 3、下载你计划安装的发行版。
 - 4、安装发行版。
 - 5、进行必要的安装后设置。
 - 6、如果要运行mysql基准测试脚本，系统必须安装perl，支持perl脚本的运行。
本章主要介绍在UNIX、Linux、BSD，windows，OS X上的安装，不过通常情况下，我们的服务器都是安装在Linux上，因此我们主要对mysql在Linux上的安装进行详细的学习。

---
## 1、通用安装指南
### 1.1 选定版本和发行版
本节学习mysql的通用安装指南。介绍安装mysql需要的通用信息。在安装mysql之前，先决定使用哪种版本和分发格式（二进制或源代码）。MySQL通常包含两种版本，开发版和GA版本，GA版本也称为稳定版本，生产环境一般不要安装开发版本，要安装GA版本。  
mysql8.0的命名方案使用由三个数字和一个可选的后缀组成的版本名称; 例如：mysql-8.0.1-dmr。发行版名称中的数字解释如下：  
 - 第一个数字（8）是主版本号。
 - 第二个数字（0）是次版本号。综合来看，主和次数字构成发布系列号码。系列号描述了稳定的功能集。
 - 第三个数字（1）是发布系列中的版本号。主要表示bug的修复，并不增加新的功能集。对于每个新的修补程序，这个数字都会增加。在大多数情况下，最新版本是最好的选择。
可选的后缀表示了该版本的稳定性。具体如下：  
 - dmr表示开发版本（DMR）。该版本会增加/删除功能。
 - rc表示预发布版（RC）。该版本通过了MySQL的所有内部测试。RC版本中仍可能引入新功能，但重点转移到修复bug以稳定本系列之前引入的功能。
 - 没有后缀表示GA或生产版本，可以用于生产环境。
即：DMR-->RC-->GA版。  
选择了要安装的mysql版本后，就需要决定安装的发行版了，对于大多数用户而言，二进制发行版通常是正确的选择。但是在某些情况下，需要从源代码进行安装。  
 - 你想在某个指定的位置安装mysql，而不是标准的位置。
 - 你希望配置mysqld，其中可能不包含标准二进制分发版中的功能。以下是常见的额外选项：  
 > - 通过-DWITH_LIBWRAP=1来支持TCP包装器
 > - -DWITH_ZLIB={system|bundled}，指定压缩特性
 > - -DWITH_DEBUG=1，支持debug
 > - 更多额外的信息，参见[MySQL Source-Configuration Options](https://dev.mysql.com/doc/refman/8.0/en/source-configuration-options.html).
 - 你希望配置mysqld在标准二进制分发版中不包含的某些功能。
 - 你想要定制自己的mysql，修改其内核代码。
 - 源代码分发包含比二进制分发更多的测试和示例。

### 1.2 获得mysql
 确定了要安装的mysql版本和发行版之后，就去获得它，有如下几种途径：  
 - 下载最近几个版本mysql：http://dev.mysql.com/downloads/
 - 完整的列表：http://dev.mysql.com/downloads/mirrors.html
 - 获取最新的开发源码：[2.8.3 Installing MySQL Using a Development Source Tree](https://dev.mysql.com/doc/refman/8.0/en/installing-development-tree.html)

### 1.3 校验包的完整性
下载完成之后，需要检查你安装包的完整性。主要有三种方法：  
 - MD5校验和
 - 使用GNU Privacy Guard的GnuPG加密签名
 - 对于RPM软件包，内置RPM完整性验证机制  
***验证MD5：***  
在你下载了一个MySQL包之后，你应该确保它的MD5校验和与MySQL下载页面上提供的校验和匹配。每个操作系统都带有自己的md5程序，通常为md5sum。如果你的系统没有计算md5的工具，可以从google上下载安装。例如在linux上：  
```shell
shell> md5sum mysql-standard-8.0.13-linux-i686.tar.gz
aaab65abbec64d5e907dcd41b8699945  mysql-standard-8.0.13-linux-i686.tar.gz
```
***使用GnuPG进行签名校验：***  
该方法比md5更加可靠，但是需要更多的工作。GnuPG通常都会安装在linux上，如果没有安装，参见：http://www.gnupg.org/ 。  
 - 具体如何验证，请参考官方网址[使用GnuPG进行签名检查](https://dev.mysql.com/doc/refman/8.0/en/checking-gpg-signature.html) 。  
  
通常情况下，我们使用md5来校验即可。  
***使用RPM进行签名检查：***   
对于RPM软件包，没有单独的签名。RPM软件包具有内置的GPG签名和MD5校验和。您可以通过运行以下命令来验证软件包：  
```
shell> rpm --checksig package_name.rpm
shell> rpm --checksig MySQL-server-8.0.13-0.linux_glibc2.5.i386.rpm
MySQL-server-8.0.13-0.linux_glibc2.5.i386.rpm: md5 gpg OK
```
更多的校验方法，可从mysql手册上获得。  

### 1.4 安装布局
不同的安装类型，不同的系统，布局不同，具体见官方手册[安装布局](https://dev.mysql.com/doc/refman/8.0/en/installation-layouts.html)。我们后边会详细学习源码的安装。并且学习源码安装的布局。

## 2、源码安装mysql
### 2.1 源码安装
在使用源码安装mysql之前，请先检查Oracle是否为你的平台生成了预编译的二进制分发版本，以及是否适合你的需求，因为Oracle官方花费了大量的精力来优化和构建mysql的二进制版本。有两种方式从源代码安装mysql：   
 - 使用标准的MySQL源码分发。即：mysql-VERSION.tar.gz, mysql-VERSION.zip这种文件，源代码文件不包含平台信息，二进制文件会包含平台信息，例如：pc-linux-i686、winx64
 - 使用MySQL开发树。
为了源码安装mysql，你需要自己从源码编译，从源代码安装MySQL需要多种工具。具体要求如下：  
 - CMake  
 - 好的make程序，建议使用GNU make 3.75或更高版本
 - mysql 8.0使用了C++ 11功能。对于常用的GCC而言，要求版本大于等于4.8
 - MySQL C API需要使用C++或C99编译器来编译
 - mysql依赖Boost，最好下载自带boost库的mysql源码版本。在使用CMake编译时，需要在调用CMake时通过WITH_BOOST选项来告诉构建系统Boost文件所在的位置：
 ```shell
 shell> cmake . -DWITH_BOOST=/usr/local/boost_version_number
 ```
 - ncurses 库
 - 足够的内存
 - 为了运行测试脚本，需要安装Perl。
 - 对于标准的分发版，需要解压工具。
 - 对于使用mysql开发树，需要Git，bison 2.1或者更高版本，对于Solaris，还需要m4。
***源代码安装mysql后的布局***  
***默认情况下***，从源代码编译安装完成后，位于*/usr/local/mysql*下。安装目录下的组件与二进制发行版相同。如下：  

|目录|目录的内容|
|-|-|
|bin|mysqld，client，其他工具程序|
|docs|MySQL手册的Info格式|
|man|Unix手册页|
|include|include头文件|
|lib|库|
|share|错误消息，数据字典和用于数据库安装的SQL|
|support-files|其他支持文件|

为了源码编译安装mysql，我们需要先讲已有的mysql卸载。但是不同的安装方式，卸载方式也不同。MySQL的安装主要有三种方式：二进制包安装（Using Generic Binaries）、操作系统的包安装工具的安装、源码安装。  
  
**操作系统的包安装工具的安装方式的MySQL卸载：**  
```shell
rpm -qa | grep -i mysql
service mysql status
service mysql stop
# 卸载各类组件
rpm -ev MySQL-devel-5.6.23-1.linux_glibc2.5
......
rpm -ev MySQL-client-5.6.23-1.linux_glibc2.5

# 删除mysql对应的文件夹
find / -name mysql
rm -rf ....
# 确认是否卸载干净
rpm -qa | grep -i mysql
```

**二进制包/源码安装方式的MySQL卸载：**   
```shell
# 如果是采用二进制包安装的MySQL，那么你用下面命令是找不到任何MySQL组件的。
rpm -qa | grep -i mysql

# 检查mysql进程状态
/etc/init.d/mysql.server status

# 停止mysql
/etc/init.d/mysql.server stop
  
# 删除mysql对应的文件夹
find / -name mysql
rm -rf ....

# 删除配置文件
/etc/my.cnf 或/etc/init.d/mysql.server等
```
可选的删除mysql用户和组。  
要配置与默认值不同的安装位置，参见[2.8.4 MySQL Source-Configuration Options](https://dev.mysql.com/doc/refman/8.0/en/source-configuration-options.html)。  
下边我们写了一个从标准的分发版安装mysql的脚本，具体更加的信息，可以参考mysql的官网：  
该脚本结合了mysql官方手册的2.8和2.9节的内容，主要做的工作是mysql源码的编译、安装和安装后的初始化工作。  
本脚本中选择的为标准的发行版：mysql-boost-8.0.11.tar.gz，下载之后，我们放在了/tmp下，在/tmp下的脚本为install_mysql.sh，其内容如下：  
```shell
#/bin/bash

######################################################################
#
#          该脚本用于在CentOS7上源码安装MySQL 8.0(包含boost)
#
######################################################################

# 测试必要的软件是否安装
if [ `rpm -qa | grep 'ncurses' | wc -l` -ne 0 ];
then
	echo  "ncurses is installed\n"
else
	echo "ncurses is not installed\n"
	exit 1
fi

if [ `rpm -qa | grep 'gcc' | wc -l` -ne 0 ];
then
	echo  "gcc is installed\n"
else
	echo "gcc is not installed\n"
	exit 1
fi

if [ `rpm -qa | grep 'make' | wc -l` -ne 0 ];
then
	echo  "make is installed\n"
else
	echo "make is not installed\n"
	exit 1
fi

if [ `rpm -qa | grep 'cmake' | wc -l` -ne 0 ];
then
	echo  "cmake is installed\n"
else
	echo "cmake is not installed\n"
	exit 1
fi

if [ `rpm -qa | grep 'perl' | wc -l` -ne 0 ];
then
	echo  "perl is installed\n"
else
	echo "perl is not installed\n"
	exit 1
fi

# 安装准备
groupadd mysql
useradd -r -g mysql -s /bin/false mysql
# 开始构建和安装
tar zxvf mysql-boost-8.0.11.tar.gz
cd mysql-8.0.11
mkdir bld
cd bld
cmake .. -DWITH_BOOST=../boost/boost_1_66_0
make
make install

# 初始化数据目录
## 将位置更改为MySQL安装目录的顶级目录，通常是 /usr/local/mysql
cd /usr/local/mysql

## 创建一个目录，该目录的位置可以提供给 secure_file_priv系统变量,这限制了对特定目录的导入/导出操作：
mkdir mysql-files

## 将目录的所有权授予 mysql用户和组所有权 mysql，并为该目录设置正确的权限：
chown mysql:mysql mysql-files
chmod 750 mysql-files

## 初始化数据目录，包括mysql包含初始MySQL授权表的 数据库，以确定用户如何连接到服务器。
bin/mysqld --initialize --user=mysql

## 如果希望服务器能够自动支持安全连接，请使用 mysql_ssl_rsa_setup实用程序创建默认的SSL和RSA文件：
bin/mysql_ssl_rsa_setup
bin/mysqld_safe --user=mysql &

# 支持Linux标准的service mysql start/systemctl start mysqld
cp support-files/mysql.server /etc/init.d/mysql.server

## 如上安装完成之后输出如下信息：
# -- Installing: /usr/local/mysql/support-files/mysql.server
# 2018-05-20T16:32:41.424291Z 0 [System] [MY-013169] [Server] /usr/local/mysql/bin/mysqld (mysqld 8.0.11) initializing of server in progress as process 127500
# 2018-05-20T16:32:44.343746Z 5 [Note] [MY-010454] [Server] A temporary password is generated for root@localhost: %Ktmpy7jtl5!
# 2018-05-20T16:32:45.358071Z 0 [System] [MY-013170] [Server] /usr/local/mysql/bin/mysqld (mysqld 8.0.11) initializing of server has completed
# Logging to '/usr/local/mysql/data/localhost.localdomain.err'.
# [root@localhost tmp]# 2018-05-20T16:32:47.981912Z mysqld_safe Starting mysqld daemon with databases from /usr/local/mysql/data

## 注意：其中为root用户生成了临时密码，%Ktmpy7jtl5!，在稍后的登陆中必须使用该密码，登录之后立刻为root分配新密码，然后才能使用。

# 其中，最后一步为可选的手动拷贝
# [root@localhost mysql]# cp support-files/mysql.server /etc/init.d/mysql.server
# 这个拷贝是为了支持service mysql.server start语句。

## 登录
#[root@localhost mysql]# mysql -uroot -p%Ktmpy7jtl5!
# mysql: [Warning] Using a password on the command line interface can be insecure.
# Welcome to the MySQL monitor.  Commands end with ; or \g.
# Your MySQL connection id is 8
# Server version: 8.0.11

# Copyright (c) 2000, 2018, Oracle and/or its affiliates. All rights reserved.

# Oracle is a registered trademark of Oracle Corporation and/or its
# affiliates. Other names may be trademarks of their respective
# owners.

# Type 'help;' or '\h' for help. Type '\c' to clear the current input statement.

# mysql> 
# 设置新密码：ALTER USER 'root'@'localhost' IDENTIFIED BY 'mysql';
# 首次安装登录之后，必须执行如上语句，重置root用户的密码，否则会出现如下错误：
# ERROR 1820 (HY000): You must reset your password using ALTER USER statement before executing this statement.
# 改变密码不过期：ALTER USER 'root'@'localhost' PASSWORD EXPIRE NEVER;

################################################################################
# 注意和小结：
# 在初始化数据目录时，如果使用--initialize，则会生成随机初始的root密码。在这种情
# 况下，密码被标记为过期，你将需要选择一个新的密码。
# 使用--initialize-insecure选项，不会为root生成密码;假设你在将服务器投入生产之前
# 将会及时为帐户分配密码。

# 如果使用--initialize，则如下登录：
# mysql -u root -p
# 如果你不知道此密码，请查看服务器错误日志。
# 登录之后必须设置新密码才可使用：ALTER USER 'root'@'localhost' IDENTIFIED BY 'mysql';
# FLUSH PRIVILEGES;
# 对于新版的MySQL 5.7.4及以后，设置密码不过期：
# ALTER USER 'root'@'localhost' PASSWORD EXPIRE NEVER;

# 如果用 --initialize-insecure，则如下登录：
# mysql -u root --skip-password
# 连接后，分配一个新的root 密码：
# ALTER USER 'root'@'%' IDENTIFIED BY 'new_password';
# ALTER USER 'root'@'%' PASSWORD EXPIRE NEVER;

################################################################################

## 看看mysql从哪里读取配置文件呢：
# [root@localhost bld]# mysql --verbose --help | grep -A 1 'Default options'
# Default options are read from the following files in the given order:
# /etc/my.cnf /etc/mysql/my.cnf /usr/local/mysql/etc/my.cnf ~/.my.cnf 
# [root@localhost tmp]# 
```
至此，MySQL安装完成，并且可以正常使用了，具体安装过程中的各种配置，可以参考[官方手册](https://dev.mysql.com/doc/refman/8.0/en/installing.html)

---

### 2.2 mysql源码配置选项
上边的脚本进行了mysql的源码安装，在编译的过程中，很多选项是可以指定的，接下对这些配置选项详细描述。   
CMake程序提供了对MySQL源代码分发安装的大量控制。为了查看提供了哪些编译方面的控制，你可以在mysql源码标准发布版解压后的目录下执行如下命令：  
```shell
shell> cmake .. -L   # overview(概观)
shell> cmake .. -LH  # overview with help text
shell> cmake .. -LAH # all params with help text（全部选项）
shell> ccmake ..     # interactive display
# 例如：cmake .. -L 的输出为：
-- Running cmake version 2.8.12.2
-- Configuring with MAX_INDEXES = 64U
-- CMAKE_GENERATOR: Unix Makefiles
-- SIZEOF_VOIDP 8
-- MySQL 8.0.11
-- Packaging as: mysql-8.0.11-Linux-x86_64
-- Found /usr/include/boost/version.hpp 
-- BOOST_VERSION_NUMBER is #define BOOST_VERSION 105300
CMake Warning at cmake/boost.cmake:285 (MESSAGE):
  Boost minor version found is 53 we need 66
Call Stack (most recent call first):
  CMakeLists.txt:671 (INCLUDE)


-- BOOST_INCLUDE_DIR /usr/include
-- LOCAL_BOOST_DIR 
-- LOCAL_BOOST_ZIP 
-- Could not find (the correct version of) boost.
-- MySQL currently requires boost_1_66_0

CMake Error at cmake/boost.cmake:100 (MESSAGE):
  You can download it with -DDOWNLOAD_BOOST=1 -DWITH_BOOST=<directory>

  This CMake script will look for boost in <directory>.  If it is not there,
  it will download and unpack it (in that directory) for you.

  If you are inside a firewall, you may need to use an https proxy:

  export https_proxy=http://example.com:80

Call Stack (most recent call first):
  cmake/boost.cmake:288 (COULD_NOT_FIND_BOOST)
  CMakeLists.txt:671 (INCLUDE)


-- Configuring incomplete, errors occurred!
See also "/tmp/mysql-8.0.11/CMakeFiles/CMakeOutput.log".
See also "/tmp/mysql-8.0.11/CMakeFiles/CMakeError.log".
-- Cache values
ALTERNATIVE_GCC:FILEPATH=ALTERNATIVE_GCC-NOTFOUND
ALTERNATIVE_GPP:FILEPATH=ALTERNATIVE_GPP-NOTFOUND
CMAKE_BUILD_TYPE:STRING=RelWithDebInfo
CMAKE_INSTALL_PREFIX:PATH=/usr/local/mysql
COMMUNITY_BUILD:BOOL=ON
DOWNLOAD_BOOST:BOOL=OFF
DOWNLOAD_BOOST_TIMEOUT:STRING=600
ENABLED_PROFILING:BOOL=ON
ENABLE_GCOV:BOOL=OFF
ENABLE_GPROF:BOOL=OFF
ENABLE_MEMCACHED_SASL:BOOL=OFF
ENABLE_MEMCACHED_SASL_PWDB:BOOL=OFF
FEATURE_SET:STRING=community
INSTALL_LAYOUT:STRING=STANDALONE
INSTALL_STATIC_LIBRARIES:BOOL=ON
LINK_RANDOMIZE:BOOL=OFF
LINK_RANDOMIZE_SEED:STRING=mysql
MYSQL_DATADIR:PATH=/usr/local/mysql/data
MYSQL_KEYRINGDIR:PATH=/usr/local/mysql/keyring
MY_GCC_COMPILER:FILEPATH=/usr/bin/gcc
MY_UNAME:FILEPATH=/usr/bin/uname
REPRODUCIBLE_BUILD:BOOL=OFF
TMPDIR:PATH=P_tmpdir
USE_LD_GOLD:BOOL=ON
WITH_ARCHIVE_STORAGE_ENGINE:BOOL=ON
WITH_ASAN:BOOL=OFF
WITH_AUTHENTICATION_LDAP:BOOL=OFF
WITH_BLACKHOLE_STORAGE_ENGINE:BOOL=ON
WITH_BOOST:PATH=
WITH_CLIENT_PROTOCOL_TRACING:BOOL=ON
WITH_DEBUG:BOOL=OFF
WITH_DEFAULT_COMPILER_OPTIONS:BOOL=ON
WITH_DEFAULT_FEATURE_SET:BOOL=ON
WITH_EDITLINE:STRING=bundled
WITH_FEDERATED_STORAGE_ENGINE:BOOL=ON
WITH_ICU:STRING=bundled
WITH_INNOBASE_STORAGE_ENGINE:BOOL=ON
WITH_INNODB_MEMCACHED:BOOL=OFF
WITH_LZ4:STRING=bundled
WITH_LZMA:STRING=bundled
WITH_MSAN:BOOL=OFF
WITH_RAPID:BOOL=OFF
WITH_RE2:STRING=bundled
WITH_SSL:STRING=system
WITH_TEST_TRACE_PLUGIN:BOOL=OFF
WITH_TSAN:BOOL=OFF
WITH_UBSAN:BOOL=OFF
WITH_UNIT_TESTS:BOOL=ON
WITH_VALGRIND:BOOL=OFF
WITH_ZLIB:STRING=bundled
```
在上边的编译过程中，如果cmake失败，那么你可能需要重新配置，通过使用不同选项再次运行它。如果你重新配置了，请注意以下事项：  
 - 如果在运行cmake之前已经运行过cmake，它可能会使用先前收集的信息。这些信息存储在CMakeCache.txt中。当CMake启动时，它会查找该文件并假设其中的信息仍然是正确的。但是，当你重新配置时，这个假设是错误的的。
 - 每次运行CMake时，都必须再次运行make来重新编译。但是，你可能希望先删除先前版本中的旧对象文件，因为它们是使用不同的配置选项编译的。
要防止使用旧对象文件或配置信息，请在重新运行CMake之前在Unix上的构建目录中运行这些命令：  
```shell
shell> make clean
shell> rm CMakeCache.txt
```
对于上边的脚本而言，我们的构建目录为：***bld***  
如上边我们在执行**cmake .. -L**时，cmake提示我们boost版本不一致，Boost minor version found is 53 we need 66（mysql需要66，而安装的为53），因此在我们的安装脚本中要指定boost路径，而不能使用默认的路径。  
你也可以使用特定的环境变量来影响cmake，见[4.9 MySQL Program Environment Variables](https://dev.mysql.com/doc/refman/8.0/en/environment-variables.html)。  
通过在构建目录（解压后创建的bld目录）下执行**cmake .. -LAH**可以查看所有这些编译选项及其默认值。   
下边将对CMake选项进行详细的描述。  
CMake选项主要包括儒如下几类：  
 - 常规选项  
 - 安装布局选项  
 - 存储引擎选项  
 - 功能特性选项
 - 编译器标志  
对于布尔类型的选项，可以将该值指定为1或ON以启用该选项，或将其指定为0或OFF以禁用该选项。  
***许多编译时选项可以在安装后服务器启动时通过选项文件或者命令行进行覆盖***。例如：默认安装基础目录位置选项：CMAKE_INSTALL_PREFIX，TCP/IP端口号选项MYSQL_TCP_PORT, 和Unix套接字文件选项MYSQL_UNIX_ADDR可以在服务器启动时通过 --basedir, --port, 和--socket来指定。下边我们对一些重要的CMake编译选项进行描述，完整的列表可以参考mysql的官方手册或者使用cmake .. -LAH列出来，篇幅原因不再完整列出。

#### 2.2.1 常规选项
 - -DBUILD_CONFIG=mysql_release  
 该选项配置一个源代码发行版，其中包含与Oracle使用相同的构建选项来生成官方MySQL版本的二进制发行版。  
 - -DCMAKE_BUILD_TYPE=type  
 type如下取值：  
 > - RelWithDebInfo: 启用优化并生成调试信息。这是默认的MySQL构建类型  
 > - Debug: 禁用优化并生成调试信息。如果启用了WITH_DEBUG选项，也会使用此构建类型。即，-DWITH_DEBUG=1与-DCMAKE_BUILD_TYPE=Debug具有相同的效果。
 - -DCPACK_MONOLITHIC_INSTALL=bool  
 影响make是生成一个安装文件还是多个文件。默认值为OFF，表示生成多个安装文件。

#### 2.2.2 安装布局选项
 - -DCMAKE_INSTALL_PREFIX=dir_name     
 安装基础目录。该值可以在服务器启动时使用--basedir选项进行设置。默认为/usr/local/mysql  
 - -DINSTALL_BINDIR=dir_name  
 安装用户程序的目录，一般使用默认值。DCMAKE_INSTALL_PREFIX/bin  
 - -DINSTALL_DOCDIR=dir_name  
 文档安装目录。  
 - -DINSTALL_DOCREADMEDIR=dir_name  
 README文件安装目录。  
 - -DINSTALL_INCLUDEDIR=dir_name  
 头文件安装目录。  
 - -DINSTALL_INFODIR=dir_name  
 Info文件安装目录  
 - -DINSTALL_LAYOUT=name  
 选择预定义的安装布局。name取值如下：  
 > - STANDALONE: 与.tar.gz和.zip软件包使用的布局相同。默认值。  
 > - RPM: 布局类似于RPM软件包。  
 > - SVR4: Solaris软件包布局。  
 > - DEB: DEB软件包布局。  
 你可以选择预定义的安装布局，同时可以通过指定其他选项来修改各个组件的安装位置。例如：  
 ```shell
 shell> cmake . -DINSTALL_LAYOUT=SVR4 -DMYSQL_DATADIR=/var/mysql/data
 ```
 INSTALL_LAYOUT的值决定了secure_file_priv，keyring_encrypted_file_data和keyring_file_data系统变量的默认值。  
 - -DINSTALL_LIBDIR=dir_name  
 库文件安装目录。  
 - -DINSTALL_MANDIR=dir_name  
 手册页安装目录。  
 - -DINSTALL_MYSQLKEYRINGDIR=dir_path  
 用作keyring_file插件数据文件位置的默认目录。默认值是平台相关的并且取决于INSTALL_LAYOUT CMake选项的值。  
 - -DINSTALL_MYSQLSHAREDIR=dir_name  
 share数据文件安装目录。  
 - -DINSTALL_MYSQLTESTDIR=dir_name  
 在哪里安装mysql-test目录。要禁止安装此目录，请将该选项显式设置为空值（-DINSTALL_MYSQLTESTDIR =）。  
 - -DINSTALL_PKGCONFIGDIR=dir_name  
 用于安装pkg-config使用的mysqlclient.pc文件的目录。默认值是INSTALL_LIBDIR/pkgconfig（除非INSTALL_LIBDIR以/mysql结尾，在这种情况下，首先删除它）。  
 - -DINSTALL_PLUGINDIR=dir_name  
 插件的位置。该值可以在服务器启动时使用--plugin_dir选项进行设置。  
 - -DINSTALL_SBINDIR=dir_name  
 安装mysqld的目录。  
 - -DINSTALL_SECURE_FILE_PRIVDIR=dir_name  
 secure_file_priv系统变量的默认值。默认值是平台特定的，取决于INSTALL_LAYOUT CMake选项的值。  
 - -DINSTALL_STATIC_LIBRARIES=bool  
 是否安装静态库。默认为ON。如果设置为OFF，则不安装这些库：libmysqlclient.a，libmysqlservices.a。  
 - -DMYSQL_DATADIR=dir_name  
 MySQL数据目录的位置。该值可以在服务器启动时使用--datadir选项进行设置。  
 - -DSYSCONFDIR=dir_name  
 默认的my.cnf选项文件位置。可以使用--defaults-file = file_name给定的选项文件启动服务器来进行修改。  
 其他的选项参考mysql的手册。  

#### 2.2.3 存储引擎选项
存储引擎作为插件来构建。可以将插件构建为静态模块（编译到服务器中）或者动态模块（构建为动态库时，为了使用它，必须使用INSTALL PLUGIN语句或--plugin-load选项将其安装到服务器中），一些插件不能支持静态或动态构建。  
InnoDB， MyISAM， MERGE， MEMORY，和 CSV存储引擎是强制性静态的（总是编译到服务器中）无需显式安装。
要将存储引擎静态编译到服务器中，请使用-DWITH_engine_STORAGE_ENGINE=1，engine的值可以是：ARCHIVE, BLACKHOLE, EXAMPLE, 和 FEDERATED，例如：  
```s
-DWITH_ARCHIVE_STORAGE_ENGINE=1 \
-DWITH_BLACKHOLE_STORAGE_ENGINE=1
```
要从构建中排除某个引擎，请使用-DWITH_engine_STORAGE_ENGINE=0或者-DWITHOUT_engine_STORAGE_ENGINE=1  
```s
-DWITH_ARCHIVE_STORAGE_ENGINE=0
-DWITH_EXAMPLE_STORAGE_ENGINE=0
-DWITH_FEDERATED_STORAGE_ENGINE=0

-DWITHOUT_ARCHIVE_STORAGE_ENGINE=1
-DWITHOUT_EXAMPLE_STORAGE_ENGINE=1
-DWITHOUT_FEDERATED_STORAGE_ENGINE=1
```

#### 2.2.4 特性选项
下边列出重要的选项。  
 - -DDEFAULT_CHARSET=charset_name  
 默认情况下，MySQL使用utf8mb4字符集。该值可以在服务器启动时使用--character_set_server选项进行设置。  
 - -DDEFAULT_COLLATION=collation_name  
 服务器排序规则。默认情况下，MySQL使用utf8mb4_0900_ai_ci。可以在服务器启动时使用--collat​​ion_server选项设置此值。  
 - -DMAX_INDEXES=num  
 每个表的最大索引数。默认值为64.最大值为255.小于64的值将被忽略，并使用默认值64。  
 - -DMYSQLX_TCP_PORT=port_num  
 X插件监听TCP/IP连接的端口号。默认值是33060。该值可在服务器启动时使用--mysqlx-port选项进行设置。  
 - -DMYSQLX_UNIX_ADDR=file_name  
 服务器侦听X插件套接字连接的Unix套接字文件路径。这必须是绝对路径名称。默认值是/tmp/mysqlx.sock。该值可以在服务器启动时使用--mysqlx-socket选项进行设置。  
 - -DMYSQL_TCP_PORT=port_num  
 服务器侦听TCP/IP连接的端口号。默认值是3306。该值可以在服务器启动时使用--port选项进行设置。  
 - -DMYSQL_UNIX_ADDR=file_name  
 服务器侦听套接字连接的Unix套接字文件路径。这必须是绝对路径名称。默认是/tmp/mysql.sock。 这个值可以在服务器启动时用--socket选项来设置。  
 我们在默认情况下安装的话，使用netstat -an可以看到：  
 ```
 [root@localhost tmp]# netstat -an | grep -E '3306|mysql'
 tcp6       0      0 :::33060                :::*                    LISTEN     
 tcp6       0      0 :::3306                 :::*                    LISTEN     
 unix  2      [ ACC ]     STREAM     LISTENING     2139620  /tmp/mysql.sock
 unix  2      [ ACC ]     STREAM     LISTENING     2139623  /tmp/mysqlx.sock
 unix  3      [ ]         STREAM     CONNECTED     3731765  /tmp/mysql.sock
 ```
 如上有个CONNECTED状态的unix套接字文件，因为我们本地有个连接。  
 - -DWITH_BOOST=path_name
 Boost库是构建MySQL所必需的。这些CMake选项可以控制库的位置，以及是否自动下载它：  
 > - -DWITH_BOOST=path_name，指定boost库的位置。也可以通过BOOST_ROOT 或者 WITH_BOOST环境变量指定。  
 > - -DWITH_BOOST=system，只是mysql使用系统安装的boost来构建，而不是使用mysql源代码发行版中包含的任何版本。  
 > - -DDOWNLOAD_BOOST=bool，是否下载Boost库。默认值为OFF。  
 > - -DDOWNLOAD_BOOST_TIMEOUT=seconds，下载Boost库的超时时间（秒）。默认为600秒。  
 例如自动构建mysql：   
 ```shell
 mkdir bld
 cd bld
 cmake .. -DDOWNLOAD_BOOST=ON -DWITH_BOOST=$HOME/my_boost
 ```
 这会使Boost下载到你的主目录下的my_boost目录中。如果所需的Boost版本已经存在，则不进行下载。如果所需的Boost版本更改，则会下载较新的版本。如果Boost已经在本地安装，并且编译器自己找到Boost头文件，则可能不需要指定前面的CMake选项。但是，如果MySQL所需的Boost版本已经升级但是本地安装的版本尚未升级，则可能会出现构建问题。通过上述方法将Boost下载到指定的位置，当所需的Boost版本发生更改时，你需要删除bld文件夹并重新创建它，然后再次执行cmake步骤。否则，新的Boost版本可能无法下载，编译可能会失败。  
 - -DWITH_DEBUG=bool  
 是否支持debug。
 - -DWITH_INNODB_MEMCACHED=bool  
 是否生成memcached共享库（libmemcached.so和 innodb_engine.so）。  
 - -DWITH_LZ4=lz4_type  
 使用哪个lz4库。  
 > - bundled：使用mysql发行版自带的lz4库。  
 > - system：使用系统lz4库。  
 - -DWITH_PROTOBUF=protobuf_type  
 protobuf_type可以是以下值之一：  
 > - bundled：使用与分发版捆绑在一起的软件包。这是默认设置。
 > - system：使用系统上安装的软件包。
 其他值被忽略，回退到 bundled。
 -DWITH_SSL={ssl_type|path_name}
 编译要使用的SSL库相关的位置信息。  
 - -DWITH_ZLIB=zlib_type
 与-DWITH_LZ4类似。  
如上我们学习了常用的选项，完整的选项，见[mysql手册](https://dev.mysql.com/doc/refman/8.0/en/source-configuration-options.html)。  

#### 2.2.5 编译器标志
 - -DCMAKE_C_FLAGS="flags"   
 C编译器的标志。  
 - -DCMAKE_CXX_FLAGS="flags"   
 C ++编译器的标志。  
 - -DWITH_DEFAULT_COMPILER_OPTIONS=bool  
 是否使用 cmake/build_configurations/compiler_options.cmake中的标志。  
 ***注意：所有优化标志都经过了MySQL编译团队的仔细挑选和测试。因此不要随意的修改，除非你自己经过了严格的测试***。  
 例如，要在64位Linux机器上创建32位版本，请执行以下操作：  
 ```
 shell> mkdir bld
 shell> cd bld
 shell> cmake .. -DCMAKE_C_FLAGS=-m32 \
         -DCMAKE_CXX_FLAGS=-m32 \
         -DCMAKE_BUILD_TYPE=RelWithDebInfo
 ```
如上我们学习了常用的选项，完整的选项，见[mysql手册](https://dev.mysql.com/doc/refman/8.0/en/source-configuration-options.html)。  

---

## 3 升级或降级mysql
软件升级是一种常见的过程。在生产环境升级之前，一定要先在测试环境执行升级过程，一切正常后才能在生产环境上进行。降级不常见。  
***注意：不支持从MySQL 8.0降级到MySQL 5.7（或从一个MySQL 8.0发行版降级到以前的MySQL 8.0发行版）。唯一支持的方法是恢复升级前的备份，因此，在开始升级过程之前备份数据至关重要***   
具体的见[mysql的手册](https://dev.mysql.com/doc/refman/8.0/en/upgrading-downgrading.html)。  


