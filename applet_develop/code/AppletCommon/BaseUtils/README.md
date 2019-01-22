# 基础工具库
这个目录是一个独立的基础工具库。

刚好学习下Makefile的编写。内容如下：  

```Makefile
CC := gcc
CXX := g++
CFLAGS := -g -O2 -std=c++11 -Wno-deprecated -Wall -fPIC

SOURCES=$(wildcard *.cpp *.c)

OBJS = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES)))
OUTLIB := libbase_utils.a

all: $(OBJS) $(OUTLIB)

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o:%.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

$(OUTLIB): $(OBJS)
	$(CC) -shared $(CFLAGS) -o $(OUTLIB) $(OBJS)

.PHONY: cleanall install

cleanall:
	rm *.o *.so *.a
```

以上文件将该目录下的.c和.cpp文件编译成一个静态库libbase_utils.a。  
以上文件用了变量，也用了函数，变量一幕了然，关键是函数，解释如下：  

1. wildcard 功能: 产生一个所有以“.c”结尾的文件的列表。
 ```
 SOURCES = $(wildcard *.c *.cpp) 
 ```
 这句代码表示产生一个所有以.c，.cpp结尾的文件的列表，然后存入变量SOURCES中。  
2. patsubst 功能: 匹配替换，有三个参数。第一个参数是一个需要匹配的式样，第二个表示用什么来替换它，第三个是一个需要被处理的由空格分隔的文件列表.  
 ```
 OBJS = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES)))
 ```
 这句代码表示把文件名列表中的所有的.c,.cpp字符变成.o，形成一个新的文件名列表，然后存入OBJS变量中。其中两次调用patsubst，先处理cpp文件名的替换，后处理c文件名的替换。最终就是所有的.c和cpp文件名被替换成了对应的.o文件名。形成一个列表。  

3. 然后看：  
 ```
%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o:%.cpp
	$(CXX) $(CFLAGS) -c $< -o $@
 ```
 这几句命令表示把所有的 .c , .cpp 文件编译成.o文件。 这里有几个比较有用的内部变量:  
  - $@	表示目标文件
  - $<	表示依赖文件列表中第一个依赖文件
  - $^	表示整个依赖文件列表
  - $?	表示比目标文件还要新的依赖文件列表

4. 清空目标文件的规则
每个Makefile都应该编写一个清空目标文件(.o和执行文件)的规则，这不仅便于重编译，也很利于保持文件的整洁。 一般的风格是:  
```Makefile
cleanall:
	rm *.o *.so *.a
```

但是更为稳健的方法是:  
```Makefile
.PHONY: cleanall
cleanall:
	-rm *.o *.so *.a
```
在这里”.PHONY”的意思表示clean是一个“伪目标”，而在rm命令前机上一个小减号”-“表示的意思是:也许莫邪文件出现了问题，但是不要管，继续做后面的事。当然，clean的规则不要放在文件的开头，不然会被make以为是默认的目标。所有有个不成文的规定是——”clean从来都是放在最后面的”。  


