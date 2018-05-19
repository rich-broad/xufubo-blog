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
该方法比md5更加可靠，但是需要更多的工作。GnuPG通常都会安装在linux上，如果没有安装，参见：http://www.gnupg.org/。具体如何验证，请参考官方网址[2.1.3.2使用GnuPG进行签名检查](https://dev.mysql.com/doc/refman/8.0/en/checking-gpg-signature.html)。通常情况下，我们使用md5来校验即可。  
***使用RPM进行签名检查：***   
对于RPM软件包，没有单独的签名。RPM软件包具有内置的GPG签名和MD5校验和。您可以通过运行以下命令来验证软件包：  
```
shell> rpm --checksig package_name.rpm
shell> rpm --checksig MySQL-server-8.0.13-0.linux_glibc2.5.i386.rpm
MySQL-server-8.0.13-0.linux_glibc2.5.i386.rpm: md5 gpg OK
```
更多的校验方法，可从mysql手册上获得。  

### 1.4 安装布局
不同的安装类型，不同的系统，布局不同，具体见[安装布局](https://dev.mysql.com/doc/refman/8.0/en/installation-layouts.html)。我们后边会详细学习源码的安装。并且学习源码安装的布局。

## 8、源码安装mysql