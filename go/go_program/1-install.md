---
title: Linux下安装Go
---
# Linux下安装Go开发环境
## 下载安装包并解压
```
[xufubo@localhost tmp]$ sudo tar -C /usr/local -xzf go1.10.linux-amd64.tar.gz 
[xufubo@localhost tmp]$ cd /usr/local/go/
[xufubo@localhost go]$ ll
total 188
drwxr-xr-x.  2 root root  4096 Feb 17 01:12 api
-rw-r--r--.  1 root root 47028 Feb 17 01:12 AUTHORS
drwxr-xr-x.  2 root root  4096 Feb 17 01:18 bin
drwxr-xr-x.  4 root root  4096 Feb 17 01:18 blog
-rw-r--r--.  1 root root  1339 Feb 17 01:12 CONTRIBUTING.md
-rw-r--r--.  1 root root 61940 Feb 17 01:12 CONTRIBUTORS
drwxr-xr-x.  8 root root  4096 Feb 17 01:12 doc
-rw-r--r--.  1 root root  5686 Feb 17 01:12 favicon.ico
drwxr-xr-x.  3 root root  4096 Feb 17 01:12 lib
-rw-r--r--.  1 root root  1479 Feb 17 01:12 LICENSE
drwxr-xr-x. 14 root root  4096 Feb 17 01:18 misc
-rw-r--r--.  1 root root  1303 Feb 17 01:12 PATENTS
drwxr-xr-x.  9 root root  4096 Feb 17 01:18 pkg
-rw-r--r--.  1 root root  1555 Feb 17 01:12 README.md
-rw-r--r--.  1 root root    26 Feb 17 01:12 robots.txt
drwxr-xr-x. 46 root root  4096 Feb 17 01:12 src
drwxr-xr-x. 20 root root 12288 Feb 17 01:18 test
-rw-r--r--.  1 root root     6 Feb 17 01:12 VERSION
[xufubo@localhost go]$ 
```
如上,我们将go安装在了/usr/local下。当然你也可以指定别的安装目录。
## 配置环境变量
如上，解压之后需要配置相关的环境变量，Linux下有连个文件可以配置环境变量，分别为：  
- /etc/profile：针对所有用户都有效的；
- $HOME/.bash_profile：是针对当前用户有效的.
针对所有用户的需要重启电脑才可以生效；针对当前用户的，在终端里使用source命令加载这个$HOME/.bash_profile即可生效。
```
[xufubo@localhost tmp]$ source /home/xufubo/.bash_profile
```
对于go语言，需要设置的最重要的三个环境变量如下：
- GOROOT：GO的安装目录
- GOPATH：工作目录，就是我们用来存放开发的源代码的地方，这个环境变量指定之后，我们编译源代码等生成的文件都会放到这个目录下  
- PATH：主要是为了我们在命令行下直接执行go，而不需要输入：/usr/local/bin/go
我们可以如下配置：
在$HOME/.bash_profile中添加：
```
export GOROOT=/usr/local/go
PATH=$PATH:$HOME/.local/bin:$HOME/bin:$GOROOT/bin
export PATH
export GOPATH=$HOME/code/go
[xufubo@localhost ~]$ source ./.bash_profile
```
GOPATH允许多个目录，当有多个目录时，请注意分隔符，Linux系统是冒号，当有多个GOPATH时，默认会将go get的内容放在第一个目录下。  
GOPATH 目录约定有三个子目录：  
- src 存放源代码（比如：.go .c .h .s等）  
- pkg 编译后生成的文件（比如：.a）  
- bin 编译后生成的可执行文件（为了方便，可以把此目录加入到 $PATH 变量中，如果有多个gopath，那么使用${GOPATH//://bin:}/bin添加所有的bin目录）
## 删除安装的go
- 删除go的安装目录,通常为/usr/local/go
- 删除添加的环境变量

如上就是Go语言开发环境的搭建。很简单吧！！！

