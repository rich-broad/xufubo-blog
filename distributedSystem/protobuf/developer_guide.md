# 开发者指南
## 什么是protobuf
我们知道，RPC是构建分布式系统的基石，而构建RPC的时候，很重要的组件就是消息的传递格式。protobuf是google用于在网络中交换数据、存储数据的一种语言无关、平台无关可扩展的序列化结构化数据的方式。类似的已有的序列化方式有JSON，XML等，但是protobuf更加节省空间、更快。
## protobuf如何工作
你可以通过在.proto文件中定义protobuf消息的类型来指定如何序列化结构化的数据。每一个protobuf消息是一个小的逻辑信息记录。包含一系列的name-value对。如下是一个非常简单的.proto文件的例子:  
```
message Person {
  required string name = 1;
  required int32 id = 2;
  optional string email = 3;

  enum PhoneType {
    MOBILE = 0;
    HOME = 1;
    WORK = 2;
  }

  message PhoneNumber {
    required string number = 1;
    optional PhoneType type = 2 [default = HOME];
  }

  repeated PhoneNumber phone = 4;
}
```
如上，消息格式很简单 - 每个消息类型都有一个或多个唯一编号的字段，每个字段都有一个名称和一个类型，其中类型可以是数字（整数或浮点数），布尔值，字符串，原始字节，以及其他的protobuf消息类型，这就允许你可以分层构建你的数据。同时你还可以指定字段为：可选的（optional），必填的（required）和重复的（repeated）。  
一旦编写好.proto文件，就能够运行protobuf编译器，生成你构建应用所用语言的代码，这些代码包含，序列化、反序列化、获取成员、设置成员等。如果你使用的语言为c++，那么以上文件将会生成一个Person类。你可能会以如下方式来使用：  
序列化：将结构化的消息序列化成字节流并存储：  
```C++
Person person;
person.set_name("John Doe");
person.set_id(1234);
person.set_email("jdoe@example.com");
fstream output("myfile", ios::out | ios::binary);
person.SerializeToOstream(&output);
```
反序列化：将字节流恢复成结构化的数据：  
```C++
fstream input("myfile", ios::in | ios::binary);
Person person;
person.ParseFromIstream(&input);
cout << "Name: " << person.name() << endl;
cout << "E-mail: " << person.email() << endl;
```
您可以添加新的字段到你的消息格式而不破坏向后兼容性;旧的二进制可执行程序在分析时会忽略新的字段。所以如果你有一个使用protobuf作为数据格式的通信协议，你可以扩展你的协议，而不用担心破坏现有的代码。

## 为什么不使用XML
protobuf与XML相比有如下优点：  
- 序列化后的消息更小，这就很有利于在网络中传输
- 序列化、反序列化更快
- 更加简单易用
当然了，protobuf也不是万能的，例如XML就是可编辑的，要依据具体的场景合理选择。  

## proto3 简介
proto3可以认为是proto2的升级版，支持更多的编程语言，也有一些新的特点。

接下来介绍proto语言，包含proto2和proto3.更加详细的信息，参考[官网](https://developers.google.com/protocol-buffers/docs/overview)






































