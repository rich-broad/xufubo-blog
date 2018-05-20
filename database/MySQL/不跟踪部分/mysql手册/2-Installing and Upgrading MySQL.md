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
该方法比md5更加可靠，但是需要更多的工作。GnuPG通常都会安装在linux上，如果没有安装，参见：http://www.gnupg.org/。  
 - 具体如何验证，请参考官方网址[使用GnuPG进行签名检查](https://dev.mysql.com/doc/refman/8.0/en/checking-gpg-signature.html)。  
  
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
## 2.1 源码安装
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
如上边我们在执行***cmake .. -L***时，cmake提示我们boost版本不一致，Boost minor version found is 53 we need 66（mysql需要66，而安装的为53），因此在我们的安装脚本中要指定boost路径，而不能使用默认的路径。  
你也可以使用特定的环境变量来影响cmake，见[4.9 MySQL Program Environment Variables](https://dev.mysql.com/doc/refman/8.0/en/environment-variables.html)。  
下表将对cmake支持的选项进行说明。在Default列，PREFIX 表示了 CMAKE_INSTALL_PREFIX选项的值，它指定安装的基目录。此值用作几个安装子目录的父级位置。  
通过**cmake .. -LAH**也能查看这些编译选项。    
  
|Formats|Description|Default|Introduced|Removed|
|-|-|-|-||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||
||||||