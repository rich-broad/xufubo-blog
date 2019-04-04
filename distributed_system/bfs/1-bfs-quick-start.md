# 1、概述
为了快速感受一下BFS，我们先进行安装。  
安装步骤也很简单。  
```shell
[root@localhost bfs]# git clone https://github.com/baidu/bfs.git
[root@localhost bfs]# cd ./bfs/
[root@localhost bfs]# ll
-rwxrwxrwx. 1 root root  5793 Apr  2 09:49 build.sh
-rwxrwxrwx. 1 root root  1877 Apr  2 09:49 build_version.sh
drwxrwxrwx. 1 root root  4096 Apr  2 09:49 cluster
-rwxrwxrwx. 1 root root   548 Apr  2 09:49 dockerfile
drwxrwxrwx. 1 root root     0 Apr  2 09:49 docs
-rwxrwxrwx. 1 root root   324 Apr  2 09:49 ft_test.sh
drwxrwxrwx. 1 root root     0 Apr  2 09:49 fuse
drwxrwxrwx. 1 root root     0 Apr  2 09:49 fuse_lowlevel
-rwxrwxrwx. 1 root root  5406 Apr  2 09:49 internal_build.sh
-rwxrwxrwx. 1 root root  1470 Apr  2 09:49 LICENSE
-rwxrwxrwx. 1 root root 10105 Apr  2 09:49 Makefile
-rwxrwxrwx. 1 root root  3256 Apr  2 09:49 README.md
drwxrwxrwx. 1 root root     0 Apr  2 09:49 resources
drwxrwxrwx. 1 root root     0 Apr  2 09:49 sandbox
drwxrwxrwx. 1 root root  4096 Apr  2 09:49 src
drwxrwxrwx. 1 root root     0 Apr  2 09:49 tera
drwxrwxrwx. 1 root root  4096 Apr  2 09:49 test
[root@localhost bfs]# ./build.sh
[root@localhost bfs]# ll
-rwxr-xr-x.  1 root root 14797496 Apr  2 19:28 bfs_client   # 编译产生的
drwxr-xr-x.  2 root root     4096 Apr  2 19:27 .build   # 编译产生的 里边有很多依赖的第三方库
-rwxr-xr-x.  1 root root     5798 Apr  2 19:22 build.sh
-rwxr-xr-x.  1 root root     1877 Apr  2 19:22 build_version.sh
-rwxr-xr-x.  1 root root 18890392 Apr  2 19:28 chunkserver   # 编译产生的
drwxr-xr-x.  2 root root     4096 Apr  2 19:22 cluster
-rw-r--r--.  1 root root      285 Apr  2 19:28 depends.mk
-rwxr-xr-x.  1 root root      548 Apr  2 19:22 dockerfile
drwxr-xr-x.  4 root root     4096 Apr  2 19:22 docs
-rwxr-xr-x.  1 root root      324 Apr  2 19:22 ft_test.sh
drwxr-xr-x.  2 root root     4096 Apr  2 19:22 fuse
drwxr-xr-x.  2 root root     4096 Apr  2 19:22 fuse_lowlevel
-rwxr-xr-x.  1 root root     5406 Apr  2 19:22 internal_build.sh
-rwxr-xr-x.  1 root root 14114008 Apr  2 19:28 kv_client   # 编译产生的
-rw-r--r--.  1 root root 13131666 Apr  2 19:28 libbfs.a   # 编译产生的
-rwxr-xr-x.  1 root root 15486192 Apr  2 19:28 libbfs_c.so   # 编译产生的
-rwxr-xr-x.  1 root root     1470 Apr  2 19:22 LICENSE
-rwxr-xr-x.  1 root root    10105 Apr  2 19:22 Makefile
-rwxr-xr-x.  1 root root 22547368 Apr  2 19:28 nameserver   # 编译产生的
-rwxr-xr-x.  1 root root 17411760 Apr  2 19:28 raft_kv   # 编译产生的
-rwxr-xr-x.  1 root root     3256 Apr  2 19:22 README.md
drwxr-xr-x.  3 root root     4096 Apr  2 19:22 resources
drwxr-xr-x.  2 root root     4096 Apr  2 19:22 sandbox
drwxr-xr-x. 11 root root     4096 Apr  2 19:28 src
drwxr-xr-x.  2 root root     4096 Apr  2 19:22 tera
drwxr-xr-x.  2 root root     4096 Apr  2 19:22 test
drwxr-xr-x.  7 root root     4096 Apr  2 19:23 thirdparty   # ./build.sh 创建的存放依赖的库的安装目录
drwxr-xr-x. 14 root root     4096 Apr  2 19:28 thirdsrc   # ./build.sh 创建的存放依赖的源码的目录
[root@localhost bfs]# 
# 部署并启动集群
[root@localhost thirdparty]# cd ../sandbox/
[root@localhost sandbox]# ll
total 20
-rwxr-xr-x. 1 root root  207 Apr  2 19:22 clear.sh
-rwxr-xr-x. 1 root root 1656 Apr  2 19:22 deploy.sh
-rwxr-xr-x. 1 root root 1332 Apr  2 19:22 README.md
-rwxr-xr-x. 1 root root 2042 Apr  2 19:22 small_test.sh
-rwxr-xr-x. 1 root root  705 Apr  2 19:22 start_bfs.sh
[root@localhost sandbox]# 
[root@localhost sandbox]# 
[root@localhost sandbox]# ./deploy.sh 
nameserver: no process found
chunkserver: no process found
bfs_client: no process found
[root@localhost sandbox]# 
[root@localhost sandbox]# ./start_bfs.sh 
/root/bfs/bfs/sandbox
/root/bfs/bfs/sandbox
/root/bfs/bfs/sandbox
/root/bfs/bfs/sandbox
/root/bfs/bfs/sandbox
[root@localhost sandbox]# 
[root@localhost sandbox]# ll
total 14496
-rwxr-xr-x. 1 root root 14797496 Apr  2 19:33 bfs_client
-rw-r--r--. 1 root root      603 Apr  2 19:33 bfs.flag
drwxr-xr-x. 6 root root     4096 Apr  2 19:34 chunkserver0
drwxr-xr-x. 6 root root     4096 Apr  2 19:34 chunkserver1
drwxr-xr-x. 6 root root     4096 Apr  2 19:34 chunkserver2
drwxr-xr-x. 6 root root     4096 Apr  2 19:34 chunkserver3
-rwxr-xr-x. 1 root root      207 Apr  2 19:22 clear.sh
-rwxr-xr-x. 1 root root     1656 Apr  2 19:22 deploy.sh
drwxr-xr-x. 5 root root     4096 Apr  2 19:34 nameserver0
-rwxr-xr-x. 1 root root     1332 Apr  2 19:22 README.md
-rwxr-xr-x. 1 root root     2042 Apr  2 19:22 small_test.sh
-rwxr-xr-x. 1 root root      705 Apr  2 19:22 start_bfs.sh
[root@localhost sandbox]# 
# 进行一系列的操作
[root@localhost sandbox]# ./bfs_client touchz /empty_file
[root@localhost sandbox]# ./bfs_client ls /
Found 1 items
-rw-r--r-- 0.000     2019-04-02 19:43 /empty_file
[root@localhost sandbox]#  ./bfs_client put bfs_client /
[AutoTimer] StartWrite /bfs_client use 5.156 ms
Put file to bfs /bfs_client 14797496 bytes
[AutoTimer] BfsPut /bfs_client use 139.565 ms
[root@localhost sandbox]# 
[root@localhost sandbox]#  ./bfs_client put bfs_client /
Put file to bfs /bfs_client 14797496 bytes
[AutoTimer] BfsPut /bfs_client use 132.725 ms
[root@localhost sandbox]# ./bfs_client ls /
Found 2 items
-rwxr-xr-x 14.112 M  2019-04-02 19:46 /bfs_client
-rw-r--r-- 0.000     2019-04-02 19:43 /empty_file
[root@localhost sandbox]# ll
total 14496
-rwxr-xr-x. 1 root root 14797496 Apr  2 19:33 bfs_client
-rw-r--r--. 1 root root      603 Apr  2 19:33 bfs.flag
drwxr-xr-x. 6 root root     4096 Apr  2 19:34 chunkserver0
drwxr-xr-x. 6 root root     4096 Apr  2 19:34 chunkserver1
drwxr-xr-x. 6 root root     4096 Apr  2 19:34 chunkserver2
drwxr-xr-x. 6 root root     4096 Apr  2 19:34 chunkserver3
-rwxr-xr-x. 1 root root      207 Apr  2 19:22 clear.sh
-rwxr-xr-x. 1 root root     1656 Apr  2 19:22 deploy.sh
drwxr-xr-x. 5 root root     4096 Apr  2 19:34 nameserver0
-rwxr-xr-x. 1 root root     1332 Apr  2 19:22 README.md
-rwxr-xr-x. 1 root root     2042 Apr  2 19:22 small_test.sh
-rwxr-xr-x. 1 root root      705 Apr  2 19:22 start_bfs.sh
[root@localhost sandbox]# ll -h
total 15M
-rwxr-xr-x. 1 root root  15M Apr  2 19:33 bfs_client
-rw-r--r--. 1 root root  603 Apr  2 19:33 bfs.flag
drwxr-xr-x. 6 root root 4.0K Apr  2 19:34 chunkserver0
drwxr-xr-x. 6 root root 4.0K Apr  2 19:34 chunkserver1
drwxr-xr-x. 6 root root 4.0K Apr  2 19:34 chunkserver2
drwxr-xr-x. 6 root root 4.0K Apr  2 19:34 chunkserver3
-rwxr-xr-x. 1 root root  207 Apr  2 19:22 clear.sh
-rwxr-xr-x. 1 root root 1.7K Apr  2 19:22 deploy.sh
drwxr-xr-x. 5 root root 4.0K Apr  2 19:34 nameserver0
-rwxr-xr-x. 1 root root 1.4K Apr  2 19:22 README.md
-rwxr-xr-x. 1 root root 2.0K Apr  2 19:22 small_test.sh
-rwxr-xr-x. 1 root root  705 Apr  2 19:22 start_bfs.sh
[root@localhost sandbox]# ./bfs_client get /bfs_client ./bfs_client.bak
Read 14797496 bytes from /bfs_client
[AutoTimer] BfsGet /bfs_client use 96.233 ms
[root@localhost sandbox]# ll -h
total 29M
-rwxr-xr-x. 1 root root  15M Apr  2 19:33 bfs_client
-rw-r--r--. 1 root root  15M Apr  2 19:47 bfs_client.bak
-rw-r--r--. 1 root root  603 Apr  2 19:33 bfs.flag
drwxr-xr-x. 6 root root 4.0K Apr  2 19:34 chunkserver0
drwxr-xr-x. 6 root root 4.0K Apr  2 19:34 chunkserver1
drwxr-xr-x. 6 root root 4.0K Apr  2 19:34 chunkserver2
drwxr-xr-x. 6 root root 4.0K Apr  2 19:34 chunkserver3
-rwxr-xr-x. 1 root root  207 Apr  2 19:22 clear.sh
-rwxr-xr-x. 1 root root 1.7K Apr  2 19:22 deploy.sh
drwxr-xr-x. 5 root root 4.0K Apr  2 19:34 nameserver0
-rwxr-xr-x. 1 root root 1.4K Apr  2 19:22 README.md
-rwxr-xr-x. 1 root root 2.0K Apr  2 19:22 small_test.sh
-rwxr-xr-x. 1 root root  705 Apr  2 19:22 start_bfs.sh
[root@localhost sandbox]# ll
total 28948
-rwxr-xr-x. 1 root root 14797496 Apr  2 19:33 bfs_client
-rw-r--r--. 1 root root 14797496 Apr  2 19:47 bfs_client.bak
-rw-r--r--. 1 root root      603 Apr  2 19:33 bfs.flag
drwxr-xr-x. 6 root root     4096 Apr  2 19:34 chunkserver0
drwxr-xr-x. 6 root root     4096 Apr  2 19:34 chunkserver1
drwxr-xr-x. 6 root root     4096 Apr  2 19:34 chunkserver2
drwxr-xr-x. 6 root root     4096 Apr  2 19:34 chunkserver3
-rwxr-xr-x. 1 root root      207 Apr  2 19:22 clear.sh
-rwxr-xr-x. 1 root root     1656 Apr  2 19:22 deploy.sh
drwxr-xr-x. 5 root root     4096 Apr  2 19:34 nameserver0
-rwxr-xr-x. 1 root root     1332 Apr  2 19:22 README.md
-rwxr-xr-x. 1 root root     2042 Apr  2 19:22 small_test.sh
-rwxr-xr-x. 1 root root      705 Apr  2 19:22 start_bfs.sh
```

# 2、构建脚本build.sh详解
bfs本质上是基于RPC框架的分布式系统。bfs源代码依赖很多组件，除了最基本的GCC编译器和其他最基础的组件外还有如下组件：  
 - CMake-3.2.1  
 - boost
 - common  
 - leveldb：google开源的K-V数据库  
 - libunwind-0.99：用于获取程序的调用栈和异常处理和跳转需要的库    
 - protobuf-2.6.1：google开源的IDL    
 - gflags-2.1.1：google开源的处理命令行参数的库    
 - gperftools-2.2.1：google开源的非常实用的性能分析工具集，包括：tcmalloc，Heap-profiler，Heap-checker，Cpu-profiler   
 - gtest-1.7.0：google开源的跨平台C++单元测试框架   
 - snappy-1.1.1：google开源的压缩解压缩库    
 - sofa-pbrpc：百度开源的轻量级rpc框架    

接下来详细分析编译脚本，分析之前，先补充点shell脚本的基础知识，由于下边使用了很多的if语句，在此先回顾下：  
---
**shell中if语句中相关的-e，-d，-f等的含义如下：**    
1. 文件表达式  
 - -e filename 如果 filename存在，则为真  
 - -d filename 如果 filename为目录，则为真  
 - -f filename 如果 filename为常规文件，则为真  
 - -L filename 如果 filename为符号链接，则为真  
 - -r filename 如果 filename可读，则为真  
 - -w filename 如果 filename可写，则为真  
 - -x filename 如果 filename可执行，则为真  
 - -s filename 如果文件长度不为0，则为真  
 - -h filename 如果文件是软链接，则为真  
 - filename1 -nt filename2 如果 filename1比 filename2新，则为真。  
 - filename1 -ot filename2 如果 filename1比 filename2旧，则为真。
2. 整数变量表达式  
 - -eq 等于  
 - -ne 不等于  
 - -gt 大于  
 - -ge 大于等于  
 - -lt 小于  
 - -le 小于等于  
3. 字符串变量表达式  
 - if  [ $a = $b ]                 如果string1等于string2，则为真字符串允许使用赋值号做等号   
 - if  [ $string1 !=  $string2 ]   如果string1不等于string2，则为真  
 - if  [ -n $string  ]             如果string 非空(非0），返回0(true)  
 - if  [ -z $string  ]             如果string 为空，则为真  
 - if  [ $sting ]                  如果string 非空，返回0 (和-n类似)  
4. 逻辑非 !  条件表达式的相反  
 - if [ ! 表达式 ]  
 - if [ ! -d $num ]  如果不存在目录$num
5. 逻辑与 –a                   条件表达式的并列  
 - if [ 表达式1  –a  表达式2 ]
6. 逻辑或 -o                   条件表达式的或
 - if [ 表达式1  –o 表达式2 ]
---

整个脚本先是依赖的这些软件包的安装。安装完之后，配置bfs相关变量值的依赖，接下来就是bfs的编译。下边详细分段分析安装的过程。  
```shell
#!/usr/bin/env bash
set -x -e  # -e 若指令传回值不等于0，则立即退出shell   -x 主要用来告诉你脚本做了什么事，跟sh -x 执行脚本名的效果一样

########################################
# download & build depend software
########################################

WORK_DIR=`pwd`  # bfs克隆目录
DEPS_SOURCE=`pwd`/thirdsrc   # 依赖的第三方库的源码目录
DEPS_PREFIX=`pwd`/thirdparty  # 依赖的第三方库的安装目录
DEPS_CONFIG="--prefix=${DEPS_PREFIX} --disable-shared --with-pic"   # 安装第三方库时执行 configure 时的参数，用来将这些库安装在DEPS_PREFIX中，configure 的参数--disable-shared表示禁止构建共享库（默认启用）
FLAG_DIR=`pwd`/.build   # 标识目录

export PATH=${DEPS_PREFIX}/bin:$PATH   # 导出PATH，因为涉及到protoc的使用，需要将pb文件编译成h和cc文件
mkdir -p ${DEPS_SOURCE} ${DEPS_PREFIX} ${FLAG_DIR}   # 创建这些所需的目录。即：thirdsrc  thirdparty  .build

if [ ! -f "${FLAG_DIR}/dl_third" ] || [ ! -d "${DEPS_SOURCE}/.git" ]; then
    rm -rf ${DEPS_SOURCE}
    mkdir ${DEPS_SOURCE}
    git clone https://github.com/yvxiang/thirdparty.git thirdsrc
    touch "${FLAG_DIR}/dl_third"
fi

cd ${DEPS_SOURCE}   # 切换到三方库源码目录thirdsrc

# boost 如果boost不存在则下载并解压boost，然后拷贝到thirdparty目录中，并在.build中创建一个标签空文件，表示boost库安装好了。
if [ ! -f "${FLAG_DIR}/boost_1_57_0" ] \
    || [ ! -d "${DEPS_PREFIX}/boost_1_57_0/boost" ]; then
    if [ -e boost_1_57_0.tar.gz  ]; then
        rm boost_1_57_0.tar.gz 
    fi 
    wget --no-check-certificate https://raw.githubusercontent.com/lylei9/boost_1_57_0/master/boost_1_57_0.tar.gz
    tar zxf boost_1_57_0.tar.gz
    rm -rf ${DEPS_PREFIX}/boost_1_57_0
    mv boost_1_57_0 ${DEPS_PREFIX}/boost_1_57_0
    touch "${FLAG_DIR}/boost_1_57_0"
fi

# protobuf 在git clone https://github.com/yvxiang/thirdparty.git thirdsrc中克隆了下来
if [ ! -f "${FLAG_DIR}/protobuf_2_6_1" ] \
    || [ ! -f "${DEPS_PREFIX}/lib/libprotobuf.a" ] \
    || [ ! -d "${DEPS_PREFIX}/include/google/protobuf" ]; then
    cd protobuf-2.6.1
    autoreconf -ivf
    ./configure ${DEPS_CONFIG}  # 安装在thirdparty目录中
    make -j4
    make install
    cd -
    touch "${FLAG_DIR}/protobuf_2_6_1"
fi

#leveldb
if [ ! -f "${FLAG_DIR}/leveldb" ] \
    || [ ! -f "${DEPS_PREFIX}/lib/libleveldb.a" ] \
    || [ ! -d "${DEPS_PREFIX}/include/leveldb" ]; then
    rm -rf leveldb
    git clone https://github.com/lylei/leveldb.git leveldb
    cd leveldb
    echo "PREFIX=${DEPS_PREFIX}" > config.mk  # 安装在thirdparty目录中
    make -j4
    make install
    cd -
    touch "${FLAG_DIR}/leveldb"
fi

# snappy
if [ ! -f "${FLAG_DIR}/snappy_1_1_1" ] \
    || [ ! -f "${DEPS_PREFIX}/lib/libsnappy.a" ] \
    || [ ! -f "${DEPS_PREFIX}/include/snappy.h" ]; then
    cd snappy-1.1.1
    autoreconf -ivf
    ./configure ${DEPS_CONFIG}   # 安装在thirdparty目录中
    make -j4
    make install
    cd -
    touch "${FLAG_DIR}/snappy_1_1_1"
fi

# sofa-pbrpc
if [ ! -f "${FLAG_DIR}/sofa-pbrpc" ] \
    || [ ! -f "${DEPS_PREFIX}/lib/libsofa-pbrpc.a" ] \
    || [ ! -d "${DEPS_PREFIX}/include/sofa/pbrpc" ]; then
    rm -rf sofa-pbrpc

    git clone --depth=1 https://github.com/baidu/sofa-pbrpc.git sofa-pbrpc
    cd sofa-pbrpc
    sed -i '/BOOST_HEADER_DIR=/ d' depends.mk
    sed -i '/PROTOBUF_DIR=/ d' depends.mk
    sed -i '/SNAPPY_DIR=/ d' depends.mk
    echo "BOOST_HEADER_DIR=${DEPS_PREFIX}/boost_1_57_0" >> depends.mk
    echo "PROTOBUF_DIR=${DEPS_PREFIX}" >> depends.mk
    echo "SNAPPY_DIR=${DEPS_PREFIX}" >> depends.mk
    echo "PREFIX=${DEPS_PREFIX}" >> depends.mk   # 安装在thirdparty目录中
    make -j4
    make install
    cd -
    touch "${FLAG_DIR}/sofa-pbrpc"
fi

# cmake for gflags
if ! which cmake ; then
    cd CMake-3.2.1
    ./configure --prefix=${DEPS_PREFIX}   # 安装在thirdparty目录中
    make -j4
    make install
    cd -
fi

# gflags
if [ ! -f "${FLAG_DIR}/gflags_2_1_1" ] \
    || [ ! -f "${DEPS_PREFIX}/lib/libgflags.a" ] \
    || [ ! -d "${DEPS_PREFIX}/include/gflags" ]; then
    cd gflags-2.1.1
    cmake -DCMAKE_INSTALL_PREFIX=${DEPS_PREFIX} -DGFLAGS_NAMESPACE=google -DCMAKE_CXX_FLAGS=-fPIC  # 安装在thirdparty目录中
    make -j4
    make install
    cd -
    touch "${FLAG_DIR}/gflags_2_1_1"
fi

# gtest
if [ ! -f "${FLAG_DIR}/gtest_1_7_0" ] \
    || [ ! -f "${DEPS_PREFIX}/lib/libgtest.a" ] \
    || [ ! -d "${DEPS_PREFIX}/include/gtest" ]; then
    cd gtest-1.7.0
    ./configure ${DEPS_CONFIG}   # 安装在thirdparty目录中
    make
    cp -a lib/.libs/* ${DEPS_PREFIX}/lib
    cp -a include/gtest ${DEPS_PREFIX}/include
    cd -
    touch "${FLAG_DIR}/gtest_1_7_0"
fi

# libunwind for gperftools
if [ ! -f "${FLAG_DIR}/libunwind_0_99" ] \
    || [ ! -f "${DEPS_PREFIX}/lib/libunwind.a" ] \
    || [ ! -f "${DEPS_PREFIX}/include/libunwind.h" ]; then
    cd libunwind-0.99
    ./configure ${DEPS_CONFIG}   # 安装在thirdparty目录中
    make CFLAGS=-fPIC -j4
    make CFLAGS=-fPIC install
    cd -
    touch "${FLAG_DIR}/libunwind_0_99"
fi

# gperftools (tcmalloc)
if [ ! -f "${FLAG_DIR}/gperftools_2_2_1" ] \
    || [ ! -f "${DEPS_PREFIX}/lib/libtcmalloc_minimal.a" ]; then
    cd gperftools-2.2.1
    ./configure ${DEPS_CONFIG} CPPFLAGS=-I${DEPS_PREFIX}/include LDFLAGS=-L${DEPS_PREFIX}/lib   # 安装在thirdparty目录中
    make -j4
    make install
    cd -
    touch "${FLAG_DIR}/gperftools_2_2_1"
fi

# test c++11
set +e
if test -z "$CXX"; then
    CXX=g++
fi

$CXX --std=c++11 -x c++ - -o teststd.out 2>/dev/null <<EOF
int main() {}
EOF

if [ "$?" = 0 ]; then
    STD_FLAG=c++11
fi
rm -rf teststd.out
set -e

# common
if [ ! -f "${FLAG_DIR}/common" ] \
    || [ ! -f "${DEPS_PREFIX}/lib/libcommon.a" ]; then
    rm -rf common
    git clone -b cpp11 https://github.com/baidu/common
    cd common
    sed -i 's/^PREFIX=.*/PREFIX=..\/..\/thirdparty/' config.mk   # 安装在thirdparty目录中
    if test -z "$STD_FLAG"; then
        sed -i 's/-std=c++11/-std=c++0x/g' Makefile
    fi
    make -j4
    make install
    cd -
    touch "${FLAG_DIR}/common"
fi


cd ${WORK_DIR}

########################################
# config depengs.mk 配置依赖，bfs的Makefile会include文件depends.mk，进而使用这些变量的值
########################################

echo "PBRPC_PATH=./thirdparty" > depends.mk
echo "PROTOBUF_PATH=./thirdparty" >> depends.mk
echo "PROTOC_PATH=./thirdparty/bin/" >> depends.mk
echo 'PROTOC=$(PROTOC_PATH)protoc' >> depends.mk
echo "PBRPC_PATH=./thirdparty" >> depends.mk
echo "BOOST_PATH=./thirdparty/boost_1_57_0" >> depends.mk
echo "GFLAG_PATH=./thirdparty" >> depends.mk
echo "GTEST_PATH=./thirdparty" >> depends.mk
echo "COMMON_PATH=./thirdparty" >> depends.mk
echo "TCMALLOC_PATH=./thirdparty" >> depends.mk

if test -z "$STD_FLAG"; then
    echo "STD_FLAG=c++0x" >> depends.mk
else
    echo "STD_FLAG=c++11" >> depends.mk
fi

########################################
# build
########################################

make clean
make -j4  # 编译。
```

# 3 部署 deploy.sh详解
编译完成之后，部署我们的集群服务。这里我们通过单机模拟一个集群。deploy.sh的内容如下：  
```sh
#!/usr/bin/env bash

bash ./clear.sh # 清理之前的部署

ns_num=2  # nameserver的进程数
strategy=$1  # 高可用策略，因为nameserver是整个系统的大脑，因此nameserver的高可用就非常重要了

if [ "$1"x = "x" ]; then    # 如果shell脚本没有参数，高可用策略就是none，即nameserver只有一个实例，显然不好。
    strategy="none";
    ns_num=1;
elif [ "$1x" == "raftx" ]; then  # 参数为raft，表示使用raft协议做为高可用策略，nameserver为三副本
    ns_num=3
elif [ "$1x" == "master_slave" ]; then # 参数为master_slave，表示使用传统的主从复制做为高可用策略，nameserver为主从架构
    ns_num=2
fi

# 以下产生的bfs.flag文件为bfs 各个server要用到的，源码中会使用gflags来解析到程序的变量中以供使用。main函数中都有：FLAGS_flagfile = "./bfs.flag" 语句
echo '--default_replica_num=3' >> bfs.flag
echo '--chunkserver_log_level=2' >> bfs.flag
echo '--blockreport_interval=2' >> bfs.flag
echo '--bfs_log=./log/bfs.log' >> bfs.flag
echo '--nameserver_log_level=2' >> bfs.flag
echo '--keepalive_timeout=10' >> bfs.flag
echo '--nameserver_start_recover_timeout=15' >> bfs.flag
echo '--block_store_path=./data1,./data2' >> bfs.flag
echo '--bfs_bug_tolerant=false' >> bfs.flag
echo '--select_chunkserver_local_factor=0' >> bfs.flag
echo '--bfs_web_kick_enable=true' >> bfs.flag
echo "--ha_strategy=$strategy" >> bfs.flag  # 高可用策略，决定了nameserver启动是按照主从还是raft模式。
echo '--nameserver_nodes=127.0.0.1:8827,127.0.0.1:8828,127.0.0.1:8829' >> bfs.flag  # nameserver的部署节点和监听端口
echo '--sdk_wirte_mode=fanout' >> bfs.flag
echo '--chunkserver_multi_path_on_one_disk=true' >> bfs.flag
echo '--master_slave_log_limit=300' >> bfs.flag
echo '--master_log_gc_interval=10' >> bfs.flag
echo '--logdb_log_size=1' >> bfs.flag
echo '--log_replicate_timeout=3' >> bfs.flag
echo '--snapshot_step=10000' >> bfs.flag
echo '--log_batch_size=1000' >> bfs.flag

# 依据高可用策略，为nameserver创建目录
for((i=0;i<$ns_num;i++));
do
    mkdir -p nameserver$i/bin
    mkdir -p nameserver$i/log
    cp -f ../nameserver nameserver$i/bin/
    cp -f bfs.flag nameserver$i/
done

# 为chunkserver创建目录
for i in `seq 0 3`;
do
    mkdir -p chunkserver$i/bin
    mkdir -p chunkserver$i/data1
    mkdir -p chunkserver$i/data2
    mkdir -p chunkserver$i/log
    cp -f ../chunkserver chunkserver$i/bin/
    cp bfs.flag chunkserver$i/
done

cp -f ../bfs_client ./
```

# 4 启动 start_bfs.sh详解

经过git clone，build，deploy，我们已经为启动bfs做好了准备。接下来启动相关服务（start_bfs.sh的命令行参数要和deploy的命令行参数一致）。  

start_bfs.sh 的内容如下：  
```shell
#!/usr/bin/env bash

ns_num=1   # nameserver 进程的个数
if [ "$1"x = "raft"x ]; then
    ns_num=3;
elif [ "$1"x == "master_slave"x ]; then
    ns_num=0;
fi

# 启动nameserver
for((i=0;i<$ns_num;i++))
do
    cd nameserver$i;
    ./bin/nameserver --node_index=$i 1>nlog 2>&1 &
    echo $! > pid
    cd -
done;

if [ "$1"x == "master_slave"x ]; then
    cd nameserver0;
    ./bin/nameserver --master_slave_role=slave --node_index=0 1>nlog 2>&1 &
    echo $! > pid
    cd -
    sleep 1
    cd nameserver1;
    ./bin/nameserver --master_slave_role=master --node_index=1 1>nlog 2>&1 &
    echo $! > pid
    cd -
fi

# 启动chunkserver
for i in `seq 0 3`;
do
    cd chunkserver$i;
    ./bin/chunkserver --chunkserver_port=802$i 1>clog1 2>&1 &
    echo $! > pid
    cd -
done

```

如此，bfs的编译、部署、启动就完成了。  

如何在程序中使用，见：[bfs_client](https://github.com/xufubo/bfs/blob/master/src/client/bfs_client.cc)  

接下来研究bfs满足的需求和设计上的思考。  
