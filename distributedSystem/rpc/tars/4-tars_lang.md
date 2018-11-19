---
title: tars IDL 语言
---
# 概述
RPC框架为了解决款平台跨语言问题，通常会提供IDL，tars也不例外。接下来对tars的IDL进行学习。  
 - tars语言是一种类似C++的接口描述语言，通过tars框架提供的工具，用于生成对应语言的源文件，从而实现跨语言开发。  
 - tars文件的扩展名为.tars。  
 - 对于结构的定义，tars支持扩展字段，即：可以增加字段的同时向前兼容，这点对于互联网业务而言是必须的。  
 - 大小写敏感。  

## 1 词法规则
### 1.1 注释
采用c++的注释规范。// 表示注释一行，/**/表示注释范围中的所有代码。  

### 1.2 关键字
```tars
void,struct,bool,byte,short,int,double,float,long,string,vector,map,key,routekey,module,interface,out,require,optional,false,true,enum,const,unsigned。
```
### 1.3 标识符
所有标识符不能带有'tars_’符号，且必须以字母开头，同时不能和关键字冲突。  

## 2 数据类型
### 2.1 基本类型
支持的基本类型包括以下：  
```tars
void: 只能在函数的返回值表示  
bool: 布尔类型，映射到tars::Bool
byte: 有符号字符，映射到 tars::Char
short: 有符号短整型，映射到 tars::Short
int: 有符号整型，映射到 tars::Int32
long: 有符号长整型，映射到 tars::Int64
float: 映射到tars::Float
double: 映射到tars::Double
string: 映射到 std::string，java:String
unsigned byte: 无符号字符,c++映射到 unsigend char 其它版本tars::Short
unsigned short: 无符号短整形c++映射到 unsigned short其它版本 tars::Int32
unsigned int: 无符号整形c++映射到 unsigned int其它版本 tars::Int64
```

### 2.2 复杂类型
#### 枚举
```
enum TE
{
    E1,
    E2,
    E3
};
```
**说明：**  
 - 枚举类型支持指定变量的值，例如：E1 = 1  
 - 默认情况下，第一个定义的枚举类型值为0，这里E1的值为0；  

#### 常量
Tars文件中可以定义常量，例如：
```tars
const int a = 0;
const string s = “abc”;
```

#### 结构
结构如下定义：  
```tars
struct Test
{
    0  require  string s;
    1  optional int  i = 23;
};

key[Test, s, i];
```
**说明：**  
 - 第一列数字表示该字段的标识（tag），无论结构增减字段，该字段得值都不变；
 - Tag的值必须要>=0且<=255；
 - require表示该字段必选；
 - optional表示该字段可选；
 - 对于optional字段，可以有一个缺省值，缺省值在编码时默认不打包；  
 - 关键字key用于生成结构的小于比较符号，默认情况下不生成。  
 - > key的使用格式为： key[Struct name, member…]
 - > 生成的小于比较操作符，按照key中成员变量定义的顺序进行优先<比较；  
 - > 生成小于比较操作符以后，该结构就可以作为map的key；  
 - 在Tars的c++语言中，对于结构而言，提供两个成员函数用于直接打印出结构的内容，这个后续在使用的时候，你可以查看生成的代码。  

#### 序列
```tars
vector<int> vi;
```

#### 字典
```tars
map<int, string> m;
```
**说明：**  
 - 对于struct，通常不能作为map的key，因为struct没有大小比较符号；
 - 如果需要struct能够作为map的key，需要用less定义struct中成员的比较顺序；

#### 数组
结构中可以定义数组类型，数组用[]来定义，如下：  
```tars
byte m[5];
```
数组很少使用，一般会定义成vector。  

#### 指针
结构中可以定义byte指针类型，指针用*来定义，如下：  
```tars
byte *m;
```
指针很少使用，因为rpc框架主要是在不同机器之间传递数据的。  

*任何的struct，map，vector都可以嵌套*  

## 3 接口
接口如下定义：  
```tars
interface Demo
{
    int get(out vector<map<int, string>> v);
    
    int set(vector<map<int, string>> v);
};
```
**说明：**  
 - out 表示输出参数,即：rpc调用后的response  
 - 接口定义后，通过自动代码生成工具（如：tars2cpp)会生成同步接口和异步接口等代码，这些代码需要在客户端和服务端部分引用。  

## 4 名字空间
通过module定义名字空间，所有的struct，interface必须在名字空间中，例如：  
```tars
module MemCache
{
    struct Key
    {
        0 require string s;
    };

    struct Value
    {
        0 require string s;
    };

    interface MemCacheI
    {
        int get(Key k, out Value v);
        int set(Key k, Value v);
    };
};
```
**说明：**  
 - module不能嵌套  
 - 可以引用其他名字空间,例如:Module1::Struct1  

### 以上内容主要来自于tencent开源的tars框架。  

