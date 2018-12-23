---
title: protobuf 编码风格
---
# Style Guide
本文档提供了.proto文件的风格指南。通过遵循这些约定，可以使protobuf消息定义及其相应的类保持一致并易于阅读。

## 消息和字段名称
使用CamelCase（带有首字母大写）作为消息名称，例如SongServerRequest。使用underscore_separated_names作为字段名 - 例如，song_name。  
```
message SongServerRequest {
  required string song_name = 1;
}
```
对字段名称使用此命名约定可为您提供以下访问方式：  
```
C++:
  const string& song_name() { ... }
  void set_song_name(const string& x) { ... }

Java:
  public String getSongName() { ... }
  public Builder setSongName(String v) { ... }
```

## 枚举
使用CamelCase（使用首字母大写）作为枚举类型名称，使用CAPITALS_WITH_UNDERSCORES作为数值名称：  
```
enum Foo {
  FIRST_VALUE = 0;
  SECOND_VALUE = 1;
}
```
每个枚举值应以分号结束，而不是逗号。  

## 服务
如果你的.proto定义了一个RPC服务，你应该使用CamelCase（带有首字母大写）来表示服务名和任何RPC方法名：  
```
service FooService {
  rpc GetSomething(FooRequest) returns (FooResponse);
}
```

