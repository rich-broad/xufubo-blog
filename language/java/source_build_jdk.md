# 1 下载openjdk12
http://hg.openjdk.java.net/jdk/jdk12/  

# 2 编译器前准备
## 2.1 建议内存：大于等于4GB，磁盘大于10GB.  

## 2.2 得到源码之后解压，看看官方的文档：（这个文档很有必要看,里边很多细节需要注意）  
 ```
[root@localhost jdk12-06222165c35f]# cd ./doc/
[root@localhost doc]# ll | grep building.
-rw-r--r--. 1 root root 101797 Mar 20  2019 building.html
-rw-r--r--. 1 root root  79116 Mar 20  2019 building.md
```

## 2.3 需要一个老的KDK
 ```
yum install java-11-openjdk-devel.x86_64
[root@localhost jdk12-06222165c35f]# javac --version
javac 11.0.6
```
## 2.4 安装依赖库，生辰构建需要的文件
```
yum -y install freetype freetype-devel

yum install cups
yum install cups-devel

yum install libffi libffi-devel
yum install fontconfig-devel
yum install alsa-lib-devel

yum install autoconf
```
安装完这些库之后开始构建，如果还有依赖没有安装，则会出错
```
bash configure --enable-debug --with-jvm-variants=server

报错：

checking if static link of stdc++ is possible... no
checking how to link with libstdc++... dynamic
checking for X... no
configure: error: **Could not find X11 libraries.** You might be able to fix this by running **'sudo yum install libXtst-devel libXt-devel libXrender-devel libXrandr-devel libXi-devel'**.
configure exiting with result code 1
[root@localhost jdk12-06222165c35f]# 
```

提示非常明确，缺少X11相关的库，这时就安装库
```
yum install libXtst-devel libXt-devel libXrender-devel libXrandr-devel libXi-devel

```
安装之后继续执行（该命令编译fastdebug版，仅含server模式的hotspot虚拟机）
```
bash configure --enable-debug --with-jvm-variants=server
```
如果还是因为依赖的库没有安装就按提示安装，直至最终成功。

想看看支持哪些编译选项可以：
```
bash configure --help
```

安装完这些库之后，如果成功会输出:
```
......
config.status: creating /home/xufubo/study_code/jdk12-06222165c35f/build/linux-x86_64-server-fastdebug/compare.sh
config.status: creating /home/xufubo/study_code/jdk12-06222165c35f/build/linux-x86_64-server-fastdebug/Makefile

====================================================
A new configuration has been successfully created in
/home/xufubo/study_code/jdk12-06222165c35f/build/linux-x86_64-server-fastdebug
using configure arguments '--enable-debug --with-jvm-variants=server'.

Configuration summary:
* Debug level:    fastdebug
* HS debug level: fastdebug
* JVM variants:   server
* JVM features:   server: 'aot cds cmsgc compiler1 compiler2 epsilongc g1gc graal jfr jni-check jvmci jvmti management nmt parallelgc serialgc services shenandoahgc vm-structs zgc' 
* OpenJDK target: OS: linux, CPU architecture: x86, address length: 64
* Version string: 12-internal+0-adhoc.xufubo.jdk12-06222165c35f (12-internal)

Tools summary:
* Boot JDK:       openjdk version "11.0.6" 2020-01-14 LTS OpenJDK Runtime Environment 18.9 (build 11.0.6+10-LTS) OpenJDK 64-Bit Server VM 18.9 (build 11.0.6+10-LTS, mixed mode, sharing)  (at /usr/lib/jvm/java-11-openjdk-11.0.6.10-1.el7_7.x86_64)
* Toolchain:      gcc (GNU Compiler Collection)
* C Compiler:     Version 4.8.5 (at /usr/bin/gcc)
* C++ Compiler:   Version 4.8.5 (at /usr/bin/g++)

Build performance summary:
* Cores to use:   4
* Memory limit:   7806 MB

[root@localhost jdk12-06222165c35f]# 
```

## 2.5 编译
```
make JOBS=3
```
成功之后，输出：
```
Creating support/modules_libs/jdk.security.auth/libjaas.so from 1 file(s)
Compiling 4 files for BUILD_JIGSAW_TOOLS
Stopping sjavac server
Finished building target 'default (exploded-image)' in configuration 'linux-x86_64-server-fastdebug'
[root@localhost jdk12-06222165c35f]# 
```
看看我们的编译成果：
```
[root@localhost jdk12-06222165c35f]# ll
total 48
-rw-r--r--.  1 root root  2114 Mar 20  2019 ADDITIONAL_LICENSE_INFO
-rw-r--r--.  1 root root  1522 Mar 20  2019 ASSEMBLY_EXCEPTION
drwxr-xr-x.  3 root root   158 Feb 29 05:52 bin
drwxr-xr-x.  4 root root    69 Feb 29 06:15 build   -- 里边有编译结果
-rw-r--r--.  1 root root  1649 Mar 20  2019 configure
drwxr-xr-x.  3 root root    99 Feb 29 05:52 doc
-rw-r--r--.  1 root root 19274 Mar 20  2019 LICENSE
drwxr-xr-x. 23 root root  4096 Feb 29 05:52 make
-rw-r--r--.  1 root root  2785 Mar 20  2019 Makefile
-rw-r--r--.  1 root root   341 Mar 20  2019 README
drwxr-xr-x. 78 root root  4096 Feb 29 05:52 src
drwxr-xr-x. 14 root root   217 Feb 29 05:53 test
[root@localhost jdk12-06222165c35f]# cd ./build/
[root@localhost build]# ll
total 0
drwxr-xr-x. 8 root root 249 Feb 29 06:21 linux-x86_64-server-fastdebug
[root@localhost build]# cd ./
.configure-support/            linux-x86_64-server-fastdebug/ 
[root@localhost build]# cd ./
.configure-support/            linux-x86_64-server-fastdebug/ 
[root@localhost build]# cd ./linux-x86_64-server-fastdebug/
buildtools/        configure-support/ hotspot/           jdk/               make-support/      support/           
[root@localhost build]# cd ./linux-x86_64-server-fastdebug/
[root@localhost linux-x86_64-server-fastdebug]# ll
total 128
-rw-r--r--.  1 root root  2695 Feb 29 06:18 bootcycle-spec.gmk
-rw-r--r--.  1 root root  7745 Feb 29 06:18 buildjdk-spec.gmk
-rw-r--r--.  1 root root 12374 Feb 29 06:40 build.log
drwxr-xr-x. 18 root root  4096 Feb 29 06:40 buildtools
-rwxr-xr-x.  1 root root  3706 Feb 29 06:18 compare.sh
-rw-r--r--.  1 root root 50279 Feb 29 06:18 configure.log
drwxr-xr-x.  2 root root    64 Feb 29 06:18 configure-support
drwxr-xr-x.  3 root root    28 Feb 29 06:21 hotspot
drwxr-xr-x.  8 root root   204 Feb 29 06:40 jdk
-rw-r--r--.  1 root root  1291 Feb 29 06:18 Makefile
drwxr-xr-x.  6 root root   155 Feb 29 06:21 make-support
-rw-r--r--.  1 root root 32926 Feb 29 06:18 spec.gmk
drwxr-xr-x. 13 root root   199 Feb 29 06:39 support
[root@localhost linux-x86_64-server-fastdebug]# cd ./jdk/
[root@localhost jdk]# ll
total 20
drwxr-xr-x.  2 root root 4096 Feb 29 06:39 bin
drwxr-xr-x.  5 root root  123 Feb 29 06:21 conf
drwxr-xr-x.  3 root root  117 Feb 29 06:21 include
drwxr-xr-x.  5 root root 4096 Feb 29 06:40 lib
drwxr-xr-x.  3 root root   18 Feb 29 06:39 man
drwxr-xr-x. 72 root root 4096 Feb 29 06:30 modules
-rw-r--r--.  1 root root    0 Feb 29 06:40 _packages_attribute.done
-rw-r--r--.  1 root root  486 Feb 29 06:40 _packages_attribute.done.cmdline
-rw-r--r--.  1 root root    0 Feb 29 06:40 _packages_attribute.done.log
-rw-r--r--.  1 root root   92 Feb 29 06:21 release
[root@localhost jdk]# ./bin/j
jaotc                 java                  javap.debuginfo       jdb.debuginfo         jfr.debuginfo         jinfo.debuginfo       jmap.debuginfo        jrunscript.debuginfo  jstatd
jaotc.debuginfo       javac                 jcmd                  jdeprscan             jhsdb                 jjs                   jmod                  jshell                jstatd.debuginfo
jar                   javac.debuginfo       jcmd.debuginfo        jdeprscan.debuginfo   jhsdb.debuginfo       jjs.debuginfo         jmod.debuginfo        jshell.debuginfo      jstat.debuginfo
jar.debuginfo         javadoc               jconsole              jdeps                 jimage                jlink                 jps                   jstack                
jarsigner             javadoc.debuginfo     jconsole.debuginfo    jdeps.debuginfo       jimage.debuginfo      jlink.debuginfo       jps.debuginfo         jstack.debuginfo      
jarsigner.debuginfo   javap                 jdb                   jfr                   jinfo                 jmap                  jrunscript            jstat                 
[root@localhost jdk]# ./bin/java --version
openjdk 12-internal 2019-03-19
OpenJDK Runtime Environment (fastdebug build 12-internal+0-adhoc.xufubo.jdk12-06222165c35f)
OpenJDK 64-Bit Server VM (fastdebug build 12-internal+0-adhoc.xufubo.jdk12-06222165c35f, mixed mode)
[root@localhost jdk]# ./bin/ja
jaotc                jar                  jarsigner            java                 javac.debuginfo      javadoc.debuginfo    javap.debuginfo      
jaotc.debuginfo      jar.debuginfo        jarsigner.debuginfo  javac                javadoc              javap                
[root@localhost jdk]# ./bin/java
java               javac              javac.debuginfo    javadoc            javadoc.debuginfo  javap              javap.debuginfo    
[root@localhost jdk]# ./bin/javac --version
javac 12-internal   -- 编译出来的版本

[root@localhost jdk]# javac --version
javac 11.0.6    -- 系统的版本
[root@localhost jdk]# 
```