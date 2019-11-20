# 1. 概述
本节是go语言开发者使用protobuf的简单介绍，通过简单的例子学习如下知识:  
 - 在.proto文件中定义消息。  
 - 使用PROTOBUF编译器。  
 - 使用Go protobuf API来write和read消息。  

可以在[例子](https://github.com/protocolbuffers/protobuf/tree/master/examples)中发现一些例子。  

# 2. 定义协议格式
为了学习protobuf的基本使用，我们这里准备定义一个用户信息管理的协议，包含用户的name、id、email、phonenumber，你当然也可以使用json、xml等，不过我们这里使用protobuf。协议文件名是：addressbook.proto.  
.proto文件以包声明开头，这有助于防止不同项目之间的命名冲突。  
```proto
syntax = "proto3";
package tutorial;

import "google/protobuf/timestamp.proto";
```
在Go中，包名称用作Go包，除非您指定了go_package.  
接下来定义你需要序列化存储的消息格式：  
```proto
message Person {
  string name = 1;
  int32 id = 2;  // Unique ID number for this person.
  string email = 3;

  enum PhoneType {
    MOBILE = 0;
    HOME = 1;
    WORK = 2;
  }

  message PhoneNumber {
    string number = 1;
    PhoneType type = 2;
  }

  repeated PhoneNumber phones = 4;

  google.protobuf.Timestamp last_updated = 5;
}

// Our address book file is just one of these.
message AddressBook {
  repeated Person people = 1;
}
```
# 3. 编译
为了生成序列化和反序列化的类，你需要对proto文件进行编译：  
1. 如果没有安装protobuf，先安装它；  
2. 接下来安装go prorobuf插件；  
```
go get -u github.com/golang/protobuf/protoc-gen-go
```
编译器插件protoc-gen-go将安装在$GOBIN中，默认为$GOPATH/bin。要将这个路径放在你的PATH中。  
3. 现在运行编译器，指定源目录（应用程序的源代码所在的位置-如果不提​​供值，则使用当前目录），目标目录（希望将生成的代码放在那里;通常与$SRC_DIR相同，以及与以及.proto的路径：  
```
protoc -I=$SRC_DIR --go_out=$DST_DIR $SRC_DIR/addressbook.proto
```
因为您需要Go类，所以使用--go_out选项.  

