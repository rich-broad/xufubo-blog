# Language Guide（proto3）
本文是proto3语言的描述，具体包含如下内容：  
- 页内跳转如何实现，  

有关旧版本的描述，见[proto2](https://developers.google.com/protocol-buffers/docs/proto)  

## 定义消息类型
先看一个最简单的定义：  
```
syntax = "proto3";

message SearchRequest {
  string query = 1;
  int32 page_number = 2;
  int32 result_per_page = 3;
}
```
 - 该文件的第一行指定你正在使用proto3语法：如果你不这样做，protobuf编译器会认为你正在使用proto2。这必须是文件的第一个非空，非注释行。  

定义一些消息类型时需要如下几步。  
### 指定字段类型
在上面的例子中，所有的字段都是标量类型：两个整数（page_number和result_per_page）和一个字符串（query）。但是，您也可以为字段指定复合类型，包括枚举和其他消息类型。

### 分配字段标签编号
如你所见，消息定义中的每个字段都有一个唯一的编号标签。这些标签用于标识消息二进制格式的字段，一旦消息类型正在使用，不应该更改。请注意，值为1到15的变量需要一个字节进行编码，包括标识号和字段类型。16到2047中的标签占用两个字节。所以，对于在网络中频繁出现的消息，你应该将其标签范围控制在1到15之间，这样可以减少网络流量。请记住，为将来可能添加的频繁出现的元素留出一些空间。  
可以指定的最小标记号码是1，最大的标记号码是2的29次方减1或536870911。也不能使用数字19000到19999。因为它们是为实现protobuf而保留的。类似的，也不能使用任何以前保留的标签编号。  

### 指定字段解析规则
消息的字段类型可以是以下之一：  
 - singular(默认值)：一个正确的消息，必须包含0个或者一个该字段的值(不可超过1个)。  
 - repeated：一个正确的消息，可以包含该字段值的任意次数（包括0次）。重复值的顺序将被保留。可以理解为C++中的vector。  
在proto3中，默认情况下，标量数字类型的重复字段使用packed编码（一种高效的编码格式，注意这点与proto2不同）。  

### 增加更多的消息类型
同一proto文件中可以包含多个消息类型：  
```
message SearchRequest {
  string query = 1;
  int32 page_number = 2;
  int32 result_per_page = 3;
}

message SearchResponse {
 ...
}
```

### 增加注释
proto使用C/C++风格的注释。例如：  
```
/* SearchRequest represents a search query, with pagination options to
 * indicate which results to include in the response. */

message SearchRequest {
  string query = 1;
  int32 page_number = 2;  // Which page number do we want?
  int32 result_per_page = 3;  // Number of results to return per page.
}
```

### 保留字段
在业务的发展中，通常需要更新消息的定义，更新时，可以删除某些字段，那么未来其他用户可以重复使用被删除的标签号，这可能导致严重的问题。为了确保这种情况不会发生，你应该将删除的字段标记为reserved。这样，如果将来有用户使用这个标签号，编译器将会报错：  
```
message Foo {
  reserved 2, 15, 9 to 11;
  reserved "foo", "bar";
}
```
- 请注意，不能在同一个reserved语句中混合使用字段名称和标签号码

### .proto文件生成了什么
当你使用protobuf编译你的.proto文件时，编译器会使用你选择的语言生成对应的代码。包括获取和设置字段值，将消息序列化到输出流，以及从输入流中解析消息。详细的指南见[API Reference](https://developers.google.com/protocol-buffers/docs/reference/overview)

## 标量（内置）类型

|proto类型|说明|C++类型|Java类型|Python类型[2]|Go类型|Ruby类型|C#类型|PHP类型|
|-|-|-|-|-|-|-|-|-|
|double  |无|double|double|float|float64|Float|double|float|
|float   |无|float|float|float|float32|Float|float|float|
|int32   |使用可变长度编码。对于负数是无效的 - 如果您的字段可能具有负值，请改用sint32类型|int32|int|int|int32|Fixnum or Bignum (as required)|int|integer|
|int64   |使用可变长度编码。对于负数是无效的 - 如果您的字段可能具有负值，请改用sint64类型|int64|long|int/long[3]|int64|Bignum|long|integer/string[5]|
|uint32  |使用可变长度编码。|uint32|int[1]|int/long[3]|unit32|Fixnum or Bignum (as required)|uint|integer|
|uint64  |使用可变长度编码。|uint64|long[1]|int/long[3]|unit64|Bignum|ulong|integer/string[5]|
|sint32  |使用可变长度编码。带符号的int值。可以有效地编码负数。|int32|int|int|int32|Fixnum or Bignum (as required)|int|integer|
|sint64  |使用可变长度编码。带符号的int值。可以有效地编码负数。|int64|long|int/long[3]|int64|Bignum|long|integer/string[5]|
|fixed32 |使用定长编码，总是四个字节。如果值通常大于2的28次方，则比uint32效率更高。|uint32|int[1]|int|uint32|Fixnum or Bignum (as required)|unit|integer|
|fixed64 |使用定长编码，总是八个字节。如果值通常大于2的56次方，则比uint64效率更高。|uint64|long[1]|int/long[3]|uint64|Bignum|ulong|integer/string[5]|
|sfixed32|使用定长编码，总是四个字节。|int32|int|int|int32|Fixnum or Bignum (as required)|int|integer|
|sfixed64|使用定长编码，总是八个字节。|int64|long|int/long[3]|int64|Bignum|long|integer/string[5]|
|bool    |无|bool|boolean|bool|bool|TrueClass/FalseClass|bool|boolean|
|string  |字符串必须始终包含UTF-8编码或7位ASCII文本。|string|String|str/unicode[4]|string|String (UTF-8)|string|string|
|bytes   |字节序列|string|ByteString|str|[]byte|String (ASCII-8BIT)|ByteString|string|

[1]在Java中，无符号的32位和64位整数用符号对应表示，最高位仅存储在符号位中。在使用时需要注意这一点.  
[2]在任何情况下，将值设置到一个字段的时候将执行类型检查，以确保它是有效的.  
[3]64位或无符号32位整数在解码时总是表示为long，但如果在设置字段时为整型，则可以是int。在所有情况下，该值必须符合所设置的类型。见[2].  
[4]Python字符串在解码时表示为unicode，但如果给出ASCII字符串，则可以是str（这是可变的）。  
[5]Integer用于64位机器，string用于32位机器。  

## 默认值
解析消息时，如果序列化的消息不包含特定的singular元素，则解析对象中的相应字段将设置为该字段的默认值。这些默认值是类型相关的：  
 - 对于字符串，默认值是空字符串。
 - 对于bytes，默认值是空字节。
 - 对于bools，默认值是false。
 - 对于数字类型，默认值为零。
 - 对于枚举，默认值是第一个定义的枚举值，它必须是0。（proto2没有这个约定）
 - 对于消息字段，该字段未设置。它的确切值是语言相关的。有关详情，请参阅[generated code guide](https://developers.google.com/protocol-buffers/docs/reference/overview)。
 repeated字段的默认值为空（通常是相应语言的空列表）。  

## 枚举
proto可以允许用户定义枚举类型，在以下示例中，我们添加了一个名为Corpus的枚举，其中包含所有可能的值以及一个类型为Corpus的字段：  
```
message SearchRequest {
  string query = 1;
  int32 page_number = 2;
  int32 result_per_page = 3;
  enum Corpus {
    UNIVERSAL = 0;
    WEB = 1;
    IMAGES = 2;
    LOCAL = 3;
    NEWS = 4;
    PRODUCTS = 5;
    VIDEO = 6;
  }
  Corpus corpus = 4;
}
```
如你所见，枚举的第一个常量映射为零：每个枚举定义必须包含一个映射为零的常量作为其第一个元素。这是因为：  
 - 必须有一个零值，以便我们可以使用0作为数字默认值。
 - 零值必须是第一个元素，与第一个枚举值总是默认值的proto2语义兼容（因为proro的枚举值默认为0，proto2的第一个枚举值可以不为0）。  
可以通过将相同的值分配给不同的枚举常量来定义别名。为此，需要将allow_alias选项设置为true，否则当找到别名时，协议编译器将生成错误消息。实战上用的很少。  
```
enum EnumAllowingAlias {
  option allow_alias = true;
  UNKNOWN = 0;
  STARTED = 1;
  RUNNING = 1;
}
enum EnumNotAllowingAlias {
  UNKNOWN = 0;
  STARTED = 1;
  // RUNNING = 1;  // Uncommenting this line will cause a compile error inside Google and a warning message outside.
}
```
枚举常量必须在32位整数的范围内。由于枚举值使用varint编码，所以不推荐使用负值。你也可以在消息定义中定义枚举，如上例，这些枚举可以在.proto文件中的任何消息定义中重用。您还可以使用语法MessageType.EnumType将一个消息中声明的枚举类型用作不同消息类型中的字段类型。  
在使用枚举的.proto上运行protobuf编译器时，生成的代码将具有对应于Java或C++的枚举或用于Python的特殊EnumDescriptor类。  
在反序列化过程中，无法识别的枚举值将保留在消息中（并不会丢弃），但是当消息被反序列化时如何表示是依赖于语言的。  

### 保留值
在业务的发展中，通常需要更新枚举的定义，更新时，可以删除某些值，那么未来其他用户可以重复使用被删除的值，这可能导致严重的问题。为了确保这种情况不会发生，你应该将删除的值标记为reserved。这样，如果将来有用户使用这个值，编译器将会报错，也可以使用max关键字指定保留的数值范围可能上升到最大值。  
```
enum Foo {
  reserved 2, 15, 9 to 11, 40 to max;
  reserved "FOO", "BAR";
}
``` 
- 请注意，不能在同一个reserved语句中混合使用字段名称和标签号码  

## 使用其他消息类型
可以使用其他消息类型作为字段的类型。例如，假设你想要在每个SearchResponse消息中包含Result消息 - 为此，可以在同一个.proto中定义一个Result消息类型，然后在SearchResponse中指定一个Result类型的字段：  
```
message SearchResponse {
  repeated Result results = 1;
}

message Result {
  string url = 1;
  string title = 2;
  repeated string snippets = 3;
}
```
### 导入定义
在上面的例子中，Result消息类型在与SearchResponse相同的文件中定义 - 如果已经在另一个.proto文件中定义，该怎么办？ 
你可以通过导入来使用其他.proto文件中定义的类型。要导入另一个.proto文件的定义，可以在文件顶部添加一条导入语句：  
```
import "myproject/other_protos.proto";
```
默认情况下，只能使用直接导入的.proto文件中的定义，即：有a,b,c三个proto文件，其中，a导入了b，b导入了c，则a无法使用c中定义的类型，只能使用b中定义的类型。为了使a可以使用c中定义的类型，那么b在导入c时要使用import public。这是非常有用的，例如，你需要移动一个已经存在的proto文件，你的做法不是直接移动该存在的文件并更新所有的import，而是在旧位置放置一个虚拟文件，其中import public新文件，即：  
```
// new.proto
// All definitions are moved here

// old.proto
// This is the proto that all clients are importing.
import public "new.proto";
import "other.proto";

// client.proto
import "old.proto";
// You use definitions from old.proto and new.proto, but not other.proto
```
protobuf编译器在-I/--proto_path命令行选项中指定的目录中搜索proto文件，如果未指定该选项，则在调用编译器的目录中搜索。  

### 使用proto2消息类型
可以导入proto2消息类型并在proto3消息中使用它们，反之亦然。但是，proto2枚举不能用在proto3语法中(如果导入的proto2消息使用它们也没问题)。  

## 类型嵌套
可以在其他消息类型中定义和使用消息类型，如以下示例 - 此处的Result消息在SearchResponse消息中定义：  
```
message SearchResponse {
  message Result {
    string url = 1;
    string title = 2;
    repeated string snippets = 3;
  }
  repeated Result results = 1;
}
```
在其他消息中使用Result：  
```
message SomeOtherMessage {
  SearchResponse.Result result = 1;
}
```
更多的例子：  
```
message Outer {                  // Level 0
  message MiddleAA {  // Level 1
    message Inner {   // Level 2
      int64 ival = 1;
      bool  booly = 2;
    }
  }
  message MiddleBB {  // Level 1
    message Inner {   // Level 2
      int32 ival = 1;
      bool  booly = 2;
    }
  }
}
```

## 更新一个消息的类型
如果现有的消息类型不再满足你的所有需求 - 例如，你希望消息具有额外的字段 - 但是你仍然希望使用以旧格式创建的代码，请不要担心！更新消息类型非常简单，而不会破坏任何现有的代码。只要记住以下规则：  
 - 不要更改任何现有字段的数字标签。
 - 添加的任何新字段应该是optional 或者 repeated。这意味着任何使用“旧”消息格式的代码序列化的消息都可以被新生成的代码解析，因为它们不会丢失任何required元素。你应该为这些元素设置合理的默认值，以便新代码可以正确地与旧代码生成的消息进行交互。同样，新代码创建的消息也可以用旧代码解析：旧的二进制文件在解析时会忽略新的字段。但是，未知字段不会被丢弃，并且如果消息稍后被旧代码序列化，则未知字段将与其一起序列化 - 所以如果消息被传递给新代码，则新字段仍然可用。
 - 只要在更新的消息类型中不再使用标签号码，non-required字段可以被删除。你可能需要重命名该字段，可能会添加前缀“OBSOLETE_”，或者reserved标记，以便将来的.proto用户不会意外的重复使用该标签编号数值。
 - 只要类型和数量保持不变，non-required字段可以被转换为一个extension，反之亦然。
 - int32，uint32，int64，uint64，和bool都是相互兼容的-这意味着你可以将字段从这些类型之一更改为另一个类型，而不会破坏前向或后向兼容性。但是，如果从不符合相应类型的数据中解析出数字，则会得到与C++中将该数字转换为该类型相同的效果（例如，如果将64位数字读作int32，它将被截断为32位，也就是说，可以改变类型，改变之后代码也可以工作，但是不一定正确工作，这要看业务的值了）。
 - sint32和sint64相互兼容，但与其他整数类型不兼容。
 - 只要bytes是有效的UTF-8，string与bytes是兼容的。
 - 如果bytes包含消息的编码版本，嵌入式消息与bytes兼容。
 - fixed32与sfixed32兼容，而fixed64与sfixed64兼容。
 - 
 - 

## 未知的字段

## Any


## Oneof
如果有一个包含许多可选字段的消息，并且最多只能同时设置一个字段，则可以使用该功能强制执行此操作并节省内存。Oneof字段与optional字段是相似的，不同的是，oneof字段共享一块内存，在理解上可以借鉴C++中的union。最多只能同时设置一个字段。设置任何成员将会自动清除所有其他成员。你可以根据你选择的语言，使用特殊case（）或WhichOneof（）方法来检查哪个字段的值存在。  

### 使用oneof
为了在.proto定义Oneof字段， 你需要在名字前面加上oneof关键字, 比如下面例子的test_oneof:   
```
message SampleMessage {
  oneof test_oneof {
     string name = 4;
     SubMessage sub_message = 9;
  }
}
```
然后你可以增加oneof字段到 oneof 定义中. 你可以增加任意类型的字段, 但是不能使用repeated 关键字.在产生的代码中, oneof字段拥有同样的 getters 和setters， 就像正常的字段一样. 也有一个特殊的方法来检查到底那个字段被设置. 你可以在相应的语言API中找到oneof API介绍.  

### Oneof的特点
 - 设置oneof会自动清除其它oneof字段的值. 所以设置多次后，只有最后一次设置的字段有值.  
```C++
SampleMessage message;
message.set_name("name");
CHECK(message.has_name());
message.mutable_sub_message();   // Will clear name field.
CHECK(!message.has_name());
```
 - 如果解析器遇到oneof的多个成员，则仅仅使用最后那个填充消息。
 - oneof字段不支持扩展。
 - oneof字段不能repeated。
 - 反射API对oneof 字段有效。
 - 如果您使用C++，请确保您的代码不会导致memory crashes。下面的示例代码将会崩溃，因为通过调用set_name()方法已经删除了sub_message。  
```C++
SampleMessage message;
SubMessage* sub_message = message.mutable_sub_message();
message.set_name("name");      // Will delete sub_message
sub_message->set_...            // Crashes here
```
 - 在C++中，如果Swap()两个带有oneof字段的消息，则oneof字段也随之交换，例如：msg1将有一个sub_message并且msg2将有一个name：  
```C++
SampleMessage msg1;
msg1.set_name("name");
SampleMessage msg2;
msg2.mutable_sub_message();
msg1.swap(&msg2);
CHECK(msg1.has_sub_message());
CHECK(msg2.has_name());
```

### 向后兼容性问题
当增加或者删除oneof字段时一定要小心. 如果检查oneof的值返回None/NOT_SET, 它意味着oneof字段没有被赋值或者在一个不同的版本中赋值了。你不会知道是哪种情况。  

 *标签编号重用问题* 
 - 将可选字段移入或移出oneof：在消息序列化和解析后，可能会丢失一些信息（某些字段将被清除）。
 - 删除oneof字段并将其添加回来：在消息被序列化和解析之后，这可以清除当前设置的字段。
 - 拆分或者合并oneof：这与移动普通optional字段有同样的问题。

## Maps
如果你想创建一个关联映射作为数据定义的一部分，protobuf提供了一个方便快捷的语法：  
```
map<key_type, value_type> map_field = N;
```
其中key_type可以是任何整数或字符串类型（即：除float类型和bytes以外的任何标量类型）。请注意，枚举不是有效的key_type。 value_type可以是除map之外的任何类型。例如：  
```C++
map<string, Project> projects = 3;
```
### Map的特点
 - map不能被repeatedd标记。
 - 二进制格式中，map键的顺序与map迭代器遍历的顺序之间没有关系，因此，不要做这样的假设。
 - 为.proto生成文本格式时，map按键排序。数字键按数字排序。
 - 从二进制格式解析map时，如果有重复的键，则使用最后一个，从文本格式解析时，如果有重复的键，则可能失败。
生成的map API目前可用于所有proto3支持的语言。

### 向后兼容性
map语法等同于以下内容，因此不支持map的protobuf实现仍然可以处理您的数据：  
```
message MapFieldEntry {
  key_type key = 1;
  value_type value = 2;
}

repeated MapFieldEntry map_field = N;
```

## 包
你可以将一个可选的package说明符添加到.proto文件，以防止协议消息类型之间的名称冲突。可以理解为C++中的名字空间。  
```C++
package foo.bar;
message Open { ... }
```
在定义消息类型的字段时使用包说明符：  
```
message Foo {
  ...
  foo.bar.Open open = 1;
  ...
}
```
包说明符影响生成的代码的方式取决于选择的语言：  
 - 在C++中，生成的类被封装在C++名字空间中。例如，Open将位于名字空间foo::bar中。
 - 在Java中，除非你在.proto文件中明确提供了选项java_package，否则该包将用作Java包。
 - 在Python中，package指令被忽略，因为Python模块是根据它们在文件系统中的位置来组织的。
 - 在Go中，除非你在.proto文件中明确提供了选项go_package，否则该包将用作Go包名称。
 - 在Ruby中，生成的类被封装在嵌套的Ruby命名空间中，转换为所需的Ruby大写样式（第一个字母大写;如果第一个字符不是字母，则会预置PB_）。例如，Open将在名称空间Foo :: Bar中。
 - 在C#中，该包将用作命名空间，除非你在.proto文件中明确提供了一个选项csharp_namespace。例如，Open将位于名称空间Foo.Bar中。

### 包和名字解析
protobuf语言中的类型名称解析与C++类似，这里不做过多叙述。

## 定义服务
如前所属，序列化消息通常用于在网络中传递或者用于存储，传递的方式通常是RPC，如果你想将你的消息在RPC系统中使用，你可以在.proto文件中定义一个RPC服务接口，并且protobuf编译器将以你选择的语言生成服务接口代码和stub。例如，如果你想用一个接受你的SearchRequest为请求并返回一个SearchResponse作为响应的方法来定义一个RPC服务，你可以在你的.proto文件中定义它，如下所示：  
```C++
service SearchService {
  rpc Search (SearchRequest) returns (SearchResponse);
}
```
如果你不想实现你自己的RPC系统，你可以使用[gRPC](https://grpc.io/docs/): gRPC与protobuf工作的很好。除了gRPC之外，还有一些正在进行的第三方项目来开发Protocol Buffers的RPC实现。有关我们了解的项目的链接列表，请参阅[third-party add-ons wiki page](https://github.com/google/protobuf/blob/master/docs/third_party.md).你也可以实现自己的RPC系统，更多的信息可以参考[Proto2 Language Guide.](https://developers.google.com/protocol-buffers/docs/proto#services).  

## JSON映射


## 选项
.proto文件中的一些声明可以用多个选项标注。选项不会更改声明的整体含义，但可能会影响在特定上下文中的处理方式。可用选项的完整列表在google/protobuf/descriptor.proto中定义。  
一些选项是文件级的，这意味着它们应该写在顶层作用域中，而不是在任何消息，枚举或服务的定义中。一些选项是消息级选项，意味着它们应该写在消息定义中。有些选项是字段级别的，这意味着它们应该写在字段定义中。选项也可以写在枚举类型，枚举值，服务类型和服务方法上。  
以下是最常用的选项：  
 - java_package (file option): 指定java生成代码时的包名。如果在.proto文件中没有给出明确的java_package选项，那么默认情况下，为在.proto文件中使用“package”关键字指定的包。如果不生成Java代码，则此选项不起作用。  
```
option java_package = "com.example.foo";
```
 - java_outer_classname (file option): 要生成的最外层的Java类（因此也就是文件名）的类名。如果在.proto文件中没有指定显式的java_outer_classname，则通过将.proto文件名称转换为camel-case来构造类名（foo_bar.proto变成FooBar.java）。如果不生成Java代码，则此选项不起作用。  
```
option java_outer_classname = "Ponycopter";
```
 - java_multiple_files (file option): 为true将表示最外层消息，枚举和服务在包级别定义，而不是在以.proto文件命名的外部类中定义。
```
option java_multiple_files = true;
```
 - optimize_for (file option): 可以设置为SPEED，CODE_SIZE或LITE_RUNTIME。这会以下列方式影响C ++和Java代码生成器（可能还有第三方生成器）：  
  - SPEED (default): 编译器将生成用于序列化，反序列化和用于消息类型的其他常见操作的代码。这个代码是高度优化的。
  - CODE_SIZE: 编译器将生成最小的类，并将依靠共享的基于反射的代码来实现序列化，反序列化和各种其他操作。生成的代码比SPEED要小得多，但是操作会比较慢。类仍将实现与SPEED模式中完全相同的公共API。这种模式在包含非常大数量字段的.proto文件，并且不需要所有这些文件都是非常快速的应用程序中非常有用。
  - LITE_RUNTIME: 编译器将生成仅依赖于“lite”运行时库（libprotobuf-lite而不是libprotobuf）的类。lite运行时比整个库小得多（大约小一个数量级），但省略了描述符和反射等特定功能。这对于在移动电话等受限平台上运行的应用程序特别有用。编译器仍然会像在SPEED模式下一样生成所有方法的高效实现。生成的类将仅实现每种语言的MessageLite接口，该接口只提供完整的Message接口的一部分方法。
```
option optimize_for = CODE_SIZE;
```
 - cc_enable_arenas (file option): 为C++生成的代码启用[arena allocation](https://developers.google.com/protocol-buffers/docs/reference/arenas).
 - objc_class_prefix (file option): Objective-C相关，这里不多解释，具体的参考手册页。
 - deprecated (field option): 如果设置为true，则表示该字段已被弃用，不应被新代码使用。在大多数语言中，这没有实际的效果。在Java中，这变成了@Deprecated注释。将来，其他特定于语言的代码生成器可能会在字段的访问器上生成弃用注释，这会在编译试图使用该字段的代码时发出警告。如果该字段没有被任何人使用，并且你想阻止新用户使用该字段，请考虑用reserved语句替换该字段声明。  
```
int32 old_field = 6 [deprecated=true];
```

### 自定义选项
protobuf允许你定义和使用你自己的选项，这是大多数人不需要的高级功能。如果你认为需要创建自己的选项，请参阅[Proto2 Language Guide](https://developers.google.com/protocol-buffers/docs/proto.html#customoptions)以了解详细信息。请注意，创建自定义选项使用的扩展功能在proto3中只允许在自定义选项时使用，这一点与proto2不同。

## 代码生成
要生成Java，Python，C++，Go，Ruby，C#等代码，你需要使用.proto文件中定义的消息类型，在.proto文件上运行protobuf编译器protoc。编译器如下调用：  
```
protoc --proto_path=IMPORT_PATH --cpp_out=DST_DIR --java_out=DST_DIR --python_out=DST_DIR --go_out=DST_DIR --ruby_out=DST_DIR --javanano_out=DST_DIR --objc_out=DST_DIR --csharp_out=DST_DIR path/to/file.proto
```
其中：
 - IMPORT_PATH: 指定解析import指令时要查找.proto文件的目录，如果省略，则使用当前目录。可以通过多次传递--proto_path选项来指定多个导入目录，将按指定的顺序搜索。-I=IMPORT_PATH可以用作--proto_path的一个简短形式。
 - 你可以提供一个或多个输出指令：  
  - --cpp_out: 在DST_DIR中生成C++代码。
  - --java_out: 在DST_DIR中生成Java代码。
  - --python_out: 在DST_DIR中生成Python代码。
  - --go_out: 在DST_DIR中生成Go代码。
  - --ruby_out: 在DST_DIR中生成Ruby代码。
  - --javanano_out: 在DST_DIR中生成JavaNano代码。 JavaNano代码生成器有许多选项可用来自定义生成器输出
  - --objc_out: 在DST_DIR中生成Objective-C代码。
  - --csharp_out: 在DST_DIR中生成C#代码。
  - --php_out: 在DST_DIR中生成PHP代码。
为方便起见，如果DST_DIR以.zip或.jar结尾，则编译器会将输出写入一个具有给定名称的ZIP格式存档文件。根据Java JAR规范的要求，.jar输出也会生成一个清单文件在里边。请注意，如果输出文件已经存在，它将被覆盖；编译器不够智能，无法将文件添加到现有的存档文件中。
 - 你必须提供一个或多个.proto文件作为输入。可以一次性指定多个.proto文件。虽然这些文件是相对于当前目录命名的，每个文件必须驻留在其中一个IMPORT_PATH中，以便编译器可以确定其规范名称。


