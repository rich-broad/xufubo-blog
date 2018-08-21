# python3升级

## 引言
默认情况下，centos7安装的是python2.7，不过python3毕竟是主流，因此我们学习python3.首先先升级centos7下的python2.7至python3。  
但是，由于很多基本的命令、软件包都依赖旧版本，比如：yum。所以，在更新 Python 时，建议不要删除旧版本（新旧版本可以共存）。  

## 查看 Python 版本号
当 Linux 上安装 Python 后（默认安装），只需要输入简单的命令，就可以查看 Python 的版本号：  
```shell
python -V
# 或者
python --version
```

## 下载新版本
进入 Python下载页面，选择需要的版本。这里，我选择的版本是 3.7.0 。进入 /usr/local/src 目录:  
```shell
cd /usr/local/src/
wget https://www.python.org/ftp/python/3.7.0/Python-3.7.0.tgz
```
## 安装
```shell
tar -zxvf Python-3.7.0.tgz
#安装openssl-devel（pip需要）
yum install openssl-devel
cd Python-3.7.0/
./configure
# 编译
make
# 安装
make install
# 安装成功以后，就可以查看 Python 的版本了：
python -V
Python 2.7.5
python3 -V
Python 3.7.0
# 注意：在 /usr/local/bin/ 下有一个 python3 的链接，指向 bin 目录下的 python 3.7.
```
## 设置3.x 为默认版本
查看 Python 的路径，在 /usr/bin 下面。可以看到 python 链接的是 python 2.7，所以，执行 python 就相当于执行 python 2.7。  
```shell
ll /usr/bin | grep python
-rwxr-xr-x.   1 root root       11232 Aug 10  2017 abrt-action-analyze-python
-rwxr-xr-x.   1 root root        7208 Aug  5  2017 pmpython
lrwxrwxrwx.   1 root root           7 Jan 25  2018 python -> python2
lrwxrwxrwx.   1 root root           9 Jan 25  2018 python2 -> python2.7
-rwxr-xr-x.   1 root root        7136 Aug  4  2017 python2.7
-rwxr-xr-x.   1 root root        1835 Aug  4  2017 python2.7-config
lrwxrwxrwx.   1 root root          16 Jan 25  2018 python2-config -> python2.7-config
lrwxrwxrwx.   1 root root          14 Jan 25  2018 python-config -> python2-config
```
将原来 python 的软链接重命名：  
```shell
mv /usr/bin/python /usr/bin/python.bak
```
将 python 链接至 python3：  
```shell
ln -s /usr/local/bin/python3 /usr/bin/python
```
再查看 Python 的版本：  

```shell
python -V
Python 3.7.0
```
## 配置 yum
升级 Python 之后，由于将默认的 python 指向了 python3，yum 不能正常使用，需要编辑 yum 的配置文件：  
```shell
vi /usr/bin/yum
```
将 #!/usr/bin/python 改为 #!/usr/bin/python2，保存退出即可。  

## centos7的问题
如上修改yum配置文件后，执行yum任然有问题：  
```shell
[root@localhost Python-3.7.0]# yum install openssl-devel
......
......
Total download size: 3.2 M
Is this ok [y/d/N]: y
Downloading packages:
No Presto metadata available for base
  File "/usr/libexec/urlgrabber-ext-down", line 28
    except OSError, e:
                  ^
SyntaxError: invalid syntax
  File "/usr/libexec/urlgrabber-ext-down", line 28
    except OSError, e:
                  ^
SyntaxError: invalid syntax
  File "/usr/libexec/urlgrabber-ext-down", line 28
    except OSError, e:
                  ^
SyntaxError: invalid syntax


Exiting on user cancel
[root@localhost Python-3.7.0]#
```
以前CentOS6没这个问题的。

打开/usr/libexec/urlgrabber-ext-down看了下，发下它也使用了/usr/bin/python，于是跟前面一样，改为2，完成。  
升级后 还有一个问题，gnome-tweak-tool 也就是优化工具打不开  
```shell
[root@localhost applications]# find / -name gnome-tweak-tool
/usr/bin/gnome-tweak-tool
/usr/share/gnome-tweak-tool
[root@localhost applications]# vim /usr/bin/gnome-tweak-tool 
```
解决办法： gnome-tweak-tool 文件 #!/usr/bin/python换成 #!/usr/bin/python2 就可以了.  

yum-config-manager 文件同样出错  
```shell
[root@localhost kwplayer-master]# whereis yum-config-manager
yum-config-manager: /usr/bin/yum-config-manager /usr/share/man/man1/yum-config-manager.1.gz
[root@localhost kwplayer-master]# vim /usr/bin/yum-config-manager 
```
解决办法同上： #!/usr/bin/python换成 #!/usr/bin/python2 就可以了。

