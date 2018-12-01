# Language Guide（proto2）
本文是proto2语言的描述，具体包含如下内容：  
- 页内跳转如何实现，

## 定义消息类型
先看一个最简单的定义：  
```
message SearchRequest {
  required string query = 1;
  optional int32 page_number = 2;
  optional int32 result_per_page = 3;
}
```
如上，定义一个消息时，需要做如下几件事情。  

### 指定字段类型
在上面的例子中，所有的字段都是标量类型：两个整数（page_number和result_per_page）和一个字符串（query）。但是，您也可以为字段指定复合类型，包括枚举和其他消息类型。

### 分配字段标签编号
如你所见，消息定义中的每个字段都有一个唯一的编号标签。这些标签用于标识消息二进制格式的字段，一旦消息类型正在使用，不应该更改。请注意，值为1到15的变量需要一个字节进行编码，包括标识号和字段类型。16到2047中的标签占用两个字节。所以，对于在网络中频繁出现的消息，你应该将其标签范围控制在1到15之间，这样可以减少网络流量。请记住，为将来可能添加的频繁出现的元素留出一些空间。  
可以指定的最小标记号码是1，最大的标记号码是2的29次方减1或536870911。也不能使用数字19000到19999。因为它们是为实现protobuf而保留的。类似的，也不能使用任何以前保留的标签编号。  

### 指定字段解析规则
你能够将消息的字段类型指定为如下任意一种：  
 - required：一个正确的消息，必须有且仅有一个该字段的值。  
 - optional：一个正确的消息，必须包含0个或者一个该字段的值(不可超过1个)。  
 - repeated：一个正确的消息，可以包含该字段值的任意次数（包括0次）。重复值的顺序将被保留。可以理解为C++中的vector。  
由于历史原因，标量数字类型的repeated字段的编码效率不尽如人意。新代码应该使用特殊选项[packed = true]来获得更高效的编码。例如：
```
repeated int32 samples = 4 [packed=true];
```

*你应该非常小心地把字段标记为required。如果你希望在某个时候停止在消息中发送required字段，则将字段更改为optional会有问题 - 因为之间的代码会认为没有此字段的消息时不完整的，可能会将消息丢弃。谷歌的一些工程师得出结论：使用required弊大于利; 他们更喜欢只用optional和repeated。但是，这个观点并不普遍。也就是说没有绝对的啦*

### 增加更多的消息类型
同一proto文件中可以包含多个消息类型：  
```
message SearchRequest {
  required string query = 1;
  optional int32 page_number = 2;
  optional int32 result_per_page = 3;
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
  required string query = 1;
  optional int32 page_number = 2;  // Which page number do we want?
  optional int32 result_per_page = 3;  // Number of results to return per page.
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

|proto类型|说明|C++类型|Java类型|Python类型[2]|Go类型|
|-|-|-|-|-|-|
|double|无|double|double|float|float64|
|float|无|float|float|float|float32|
|int32|使用可变长度编码。对于负数是无效的 - 如果您的字段可能具有负值，请改用sint32类型|int32|int|int|int32|
|int64|使用可变长度编码。对于负数是无效的 - 如果您的字段可能具有负值，请改用sint64类型|int64|long|int/long[3]|int64|
|uint32|使用可变长度编码。|uint32|int[1]|int/long[3]|unit32|
|uint64|使用可变长度编码。|uint64|long[1]|int/long[3]|unit64|
|sint32|使用可变长度编码。带符号的int值。可以有效地编码负数。|int32|int|int|int32|
|sint64|使用可变长度编码。带符号的int值。可以有效地编码负数。|int64|long|int/long[3]|int64|
|fixed32|使用定长编码，总是四个字节。如果值通常大于2的28次方，则比uint32效率更高。|uint32|int[1]|int|uint32|
|fixed64|使用定长编码，总是八个字节。如果值通常大于2的56次方，则比uint64效率更高。|uint64|long[1]|int/long[3]|uint64|
|sfixed32|使用定长编码，总是四个字节。|int32|int|int|int32|
|sfixed64|使用定长编码，总是八个字节。|int64|long|int/long[3]|int64|
|bool|无|bool|boolean|bool|bool|
|string|字符串必须始终包含UTF-8编码或7位ASCII文本。|string|String|str/unicode[4]|string|
|bytes|字节序列|string|ByteString|str|[]byte|

[1]在Java中，无符号的32位和64位整数用符号对应表示，最高位仅存储在符号位中。在使用时需要注意这一点.  
[2]在任何情况下，将值设置到一个字段的时候将执行类型检查，以确保它是有效的.  
[3]64位或无符号32位整数在解码时总是表示为long，但如果在设置字段时为整型，则可以是int。在所有情况下，该值必须符合所设置的类型。见[2].  
[4]Python字符串在解码时表示为unicode，但如果给出ASCII字符串，则可以是str（这是可变的）。  

## 可选字段和默认值
如上所述，消息描述中的字段可以标记为optional。格式正确的消息可能包含或不包含optional字段。当消息被解析时，如果它不包含optional字段，则解析对象中相应的字段将被设置为该字段的默认值。默认值可以在消息类型定义时被指定。例如，假设您要为SearchRequest的result_per_page提供默认值10。   
```
optional int32 result_per_page = 3 [default = 10];
```
如果没有为optional字段指定默认值，则将​​使用特定于类型的默认值：对于字符串，默认值是空字符串。对于bool，默认值是false。对于数字类型，默认值为零。对于枚举，默认值是枚举的类型定义中列出的第一个值。这意味着在向枚举值列表的开始处添加一个值时必须小心。  

## 枚举
proto可以允许用户定义枚举类型，在以下示例中，我们添加了一个名为Corpus的枚举，其中包含所有可能的值以及一个类型为Corpus的字段：  
```
message SearchRequest {
  required string query = 1;
  optional int32 page_number = 2;
  optional int32 result_per_page = 3 [default = 10];
  enum Corpus {
    UNIVERSAL = 0;
    WEB = 1;
    IMAGES = 2;
    LOCAL = 3;
    NEWS = 4;
    PRODUCTS = 5;
    VIDEO = 6;
  }
  optional Corpus corpus = 4 [default = UNIVERSAL];
}
```
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
  repeated Result result = 1;
}

message Result {
  required string url = 1;
  optional string title = 2;
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

### 使用proto3消息类型
可以导入proto3消息类型并在proto2消息中使用它们，反之亦然。但是，proto2枚举不能用在proto3语法中。  

## 类型嵌套
可以在其他消息类型中定义和使用消息类型，如以下示例 - 此处的Result消息在SearchResponse消息中定义：  
```
message SearchResponse {
  message Result {
    required string url = 1;
    optional string title = 2;
    repeated string snippets = 3;
  }
  repeated Result result = 1;
}
```
在其他消息中使用Result：  
```
message SomeOtherMessage {
  optional SearchResponse.Result result = 1;
}
```
更多的例子：  
```
message Outer {                  // Level 0
  message MiddleAA {  // Level 1
    message Inner {   // Level 2
      required int64 ival = 1;
      optional bool  booly = 2;
    }
  }
  message MiddleBB {  // Level 1
    message Inner {   // Level 2
      required int32 ival = 1;
      optional bool  booly = 2;
    }
  }
}
```

### 组
- 请注意，此功能已弃用，不应在创建新消息类型时使用 - 请改为使用嵌套消息类型。  
指定包含多个Result的SearchResponse的方法如下：  
```
message SearchResponse {
  repeated group Result = 1 {
    required string url = 2;
    optional string title = 3;
    repeated string snippets = 4;
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
 - optional与repeated是兼容的。对于给定的序列化的repeated字段的数据，在转换为optional时，如果为标量原生类型，则取最后一个值，如果为消息类型，则合并所有输入。
 - 改变字段的默认值通常是OK的。Changing a default value is generally OK, as long as you remember that default values are never sent over the wire. Thus, if a program receives a message in which a particular field isn't set, the program will see the default value as it was defined in that program's version of the protocol. It will NOT see the default value that was defined in the sender's code.
 - enum is compatible with int32, uint32, int64, and uint64 in terms of wire format (note that values will be truncated if they don't fit), but be aware that client code may treat them differently when the message is deserialized. Notably, unrecognized enum values are discarded when the message is deserialized, which makes the field's has.. accessor return false and its getter return the first value listed in the enum definition, or the default value if one is specified. In the case of repeated enum fields, any unrecognized values are stripped out of the list. However, an integer field will always preserve its value. Because of this, you need to be very careful when upgrading an integer to an enum in terms of receiving out of bounds enum values on the wire.
 - In the current Java and C++ implementations, when unrecognized enum values are stripped out, they are stored along with other unknown fields. Note that this can result in strange behavior if this data is serialized and then reparsed by a client that recognizes these values. In the case of optional fields, even if a new value was written after the original message was deserialized, the old value will be still read by clients that recognize it. In the case of repeated fields, the old values will appear after any recognized and newly-added values, which means that order will not be preserved.

## 扩展
通过扩展这一语言特性，你可以声明消息中的一系列字段号可用于第三方扩展。扩展是一个占位符，其类型不是由原始.proto文件定义的。这允许其他.proto文件通过使用这些数字标签定义部分或全部字段的类型来添加到你的消息定义中，看一个例子：  
```
message Foo {
  // ...
  extensions 100 to 199;
}
```
这表示Foo中的字段编号[100，199]的范围是为扩展保留的。其他用户现在可以在他们自己的.proto文件中添加新的字段到Foo中，不过要使用已经指定的范围的标签编号，例如：  
```
extend Foo {
  optional int32 bar = 126;
}
```
这会在Foo的原始定义中添加一个字段名称为bar,标签编号为126的optional字段。  
编码格式与在Foo中定义的完全相同，只不过在访问方式上有点差异，例如，对于C++，会使用以下方式访问extensions字段。  
```
Foo foo;
foo.SetExtension(bar, 15);
```
类似地，Foo类会定义模板访问器HasExtension（），ClearExtension（），GetExtension（），MutableExtension（）和AddExtension（）。有关使用扩展的更多信息，请参阅所选语言的生成代码参考。  
 - 请注意，扩展可以是任何字段类型，包括消息类型，但不能是oneofs或maps。

### 嵌套扩展
您可以在另一个类型的范围中声明扩展：  
```
message Baz {
  extend Foo {
    optional int32 bar = 126;
  }
  ...
}
```
在这种情况下，访问这个扩展的C ++代码是：  
```
Foo foo;
foo.SetExtension(Baz::bar, 15);
```
 - 注意，对于嵌套扩展，外部类型与内部类型没有任何关系，千万不可以理解成为继承。对于上边的例子，仅仅是符号Foo定义在Baz的内部，除此之外，没有任何关系。  
一个常见的模式是在扩展的字段类型范围内定义扩展 - 例如，这里是对Baz类型的Foo的扩展，其中扩展被定义为Baz的一部分：  
```
message Baz {
  extend Foo {
    optional Baz foo_ext = 127;
  }
  ...
}
```
当然了，这不是必须的，你也可以如下定义：  
```
message Baz {
  ...
}

// This can even be in a different file.
extend Foo {
  optional Baz foo_baz_ext = 127;
}
```
事实上，这个语法是首选的，以避免混淆。如上所述，嵌套的语法经常被不熟悉扩展的用户误认为是继承。  

### 选择扩展号码
确保两个用户不使用相同的标签编号添加扩展名到相同的消息类型非常重要 - 如果扩展名被意外地解释为错误类型，则可能导致数据损坏。你可能要考虑为你的项目定义一个扩展编号约定来防止这种情况的发生。如果你的标签编号约定可能包含具有非常大的数字，则可以使用max关键字指定你的扩展范围可能上升到最大的字段编号：  
```
message Foo {
  extensions 1000 to max;
}
```
max是2的29次方减1, 或者536870911.  
就像通常选择标签号一样，编号约定也需要避免19000到19999的标签编号，因为它们是为Protobuf实现保留的。你可以定义包含此范围的扩展范围，但协议编译器将不允许您使用这些编号为消息定义实际扩展。  

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
然后你可以增加oneof字段到 oneof 定义中. 你可以增加任意类型的字段, 但是不能使用 required, optional, repeated 关键字.在产生的代码中, oneof字段拥有同样的 getters 和setters， 就像正常的可选字段一样. 也有一个特殊的方法来检查到底那个字段被设置. 你可以在相应的语言API中找到oneof API介绍.  

### Oneof的特点
 - 设置oneof会自动清楚其它oneof字段的值. 所以设置多次后，只有最后一次设置的字段有值.  
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
当增加或者删除oneof字段时一定要小心. 如果检查oneof的值返回None/NOT_SET, 它意味着oneof字段没有被赋值或者在一个不同的版本中赋值了。 你不会知道是哪种情况。  

 *标签编号重用问题* 
 - 将可选字段移入或移出oneof：在消息序列化和解析后，可能会丢失一些信息（某些字段将被清除）。
 - 删除一个字段或者加入一个字段：在消息被序列化和解析之后，这可以清除当前设置的字段。
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
生成的map API目前可用于所有proto2支持的语言。可以在相关的API参考中找到更多关于你所选语言的map API的信息。  
### Map的特点
 - map不支持扩展。
 - map不能被repeated, optional或者required标记。
 - 二进制格式中，map键的顺序与map迭代器遍历的顺序之间没有关系，因此，不要做这样的假设。
 - 为.proto生成文本格式时，map按键排序。数字键按数字排序。
 - 从二进制格式解析map时，如果有重复的键，则使用最后一个，从文本格式解析时，如果有重复的键，则可能失败。

### 向后兼容性
map语法等同于以下内容，因此不支持map的protobuf实现仍然可以处理您的数据：  
```
message MapFieldEntry {
  optional key_type key = 1;
  optional value_type value = 2;
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
  required foo.bar.Open open = 1;
  ...
}
```
包说明符影响生成的代码的方式取决于选择的语言：  
 - 在C++中，生成的类被封装在C++名字空间中。例如，Open将位于名字空间foo::bar中。
 - 在Java中，除非你在.proto文件中明确提供了选项java_package，否则该包将用作Java包。
 - 在Python中，package指令被忽略，因为Python模块是根据它们在文件系统中的位置来组织的。

- 请注意，即使包指令不直接影响生成的代码，例如在Python中，仍然强烈建议为.proto文件指定包，否则可能导致描述符中的命名冲突，并使原始语言不能移植到其他语言。  

### 包和名字解析
protobuf语言中的类型名称解析与C++类似，这里不做过多叙述。

## 定义服务
如前所属，序列化消息通常用于在网络中传递或者用于存储，传递的方式通常是RPC，如果你想将你的消息在RPC系统中使用，你可以在.proto文件中定义一个RPC服务接口，并且protobuf编译器将以你选择的语言生成服务接口代码和stub。例如，如果你想用一个接受你的SearchRequest为请求并返回一个SearchResponse作为响应的方法来定义一个RPC服务，你可以在你的.proto文件中定义它，如下所示：  
```C++
service SearchService {
  rpc Search (SearchRequest) returns (SearchResponse);
}
```
默认情况下，编译器将生成一个名为SearchService的抽象接口和一个相应的“stub”实现,stub会转发所有调用到RpcChannel，而RpcChannel又是一个抽象接口，你必须根据你自己的RPC系统实现RpcChannel，例如，你可能会实现一个RpcChannel，将消息序列化并通过HTTP将其发送到服务器。即RpcChannel是客户端的设施，用于与服务端通信。换句话说，生成的stub提供了一个类型安全的接口，用于进行基于protobuf的RPC调用，而不会将你锁定到任何特定的RPC实现中。所以，在C++ client部分中，你可能会得到这样的代码：  
```C++
using google::protobuf;

protobuf::RpcChannel* channel;
protobuf::RpcController* controller;
SearchService* service;
SearchRequest request;
SearchResponse response;

void DoSearch() {
  // You provide classes MyRpcChannel and MyRpcController, which implement
  // the abstract interfaces protobuf::RpcChannel and protobuf::RpcController.
  channel = new MyRpcChannel("somehost.example.com:1234");
  controller = new MyRpcController;

  // The protocol compiler generates the SearchService class based on the
  // definition given above.
  service = new SearchService::Stub(channel);

  // Set up the request.
  request.set_query("protocol buffers");

  // Execute the RPC.
  service->Search(controller, request, response, protobuf::NewCallback(&Done));
}

void Done() {
  delete service;
  delete channel;
  delete controller;
}
```
所有的服务类都实现了Service接口，它提供了一种调用特定函数的方法，而不需要在编译时知道方法名或者输入输出类型。在服务器端，这可以用来实现一个RPC服务器，你可以注册服务。  
```C++
using google::protobuf;

class ExampleSearchService : public SearchService {
 public:
  void Search(protobuf::RpcController* controller,
              const SearchRequest* request,
              SearchResponse* response,
              protobuf::Closure* done) {
    if (request->query() == "google") {
      response->add_result()->set_url("http://www.google.com");
    } else if (request->query() == "protocol buffers") {
      response->add_result()->set_url("http://protobuf.googlecode.com");
    }
    done->Run();
  }
};

int main() {
  // You provide class MyRpcServer.  It does not have to implement any
  // particular interface; this is just an example.
  MyRpcServer server;

  protobuf::Service* service = new ExampleSearchService;
  server.ExportOnPort(1234, service);
  server.Run();

  delete service;
  return 0;
}
```
如果你不想实现你自己的RPC系统，你可以使用[gRPC](https://grpc.io/docs/): gRPC与protobuf工作的很好，但是，由于在使用proto2和proto3生成的客户端和服务器之间存在潜在的兼容性问题，我们建议您使用proto3来定义gRPC服务，你可以在Proto3语言指南中找到有关proto3语法的更多信息。如果你想使用proto2和gRPC，你需要使用版本3.0.0或更高版本的ptotobuf编译器和库。除了gRPC之外，还有一些正在进行的第三方项目来开发Protocol Buffers的RPC实现。有关我们了解的项目的链接列表，请参阅[third-party add-ons wiki page](https://github.com/google/protobuf/blob/master/docs/third_party.md).  

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
 - optimize_for (file option): 可以设置为SPEED，CODE_SIZE或LITE_RUNTIME。这会以下列方式影响C ++和Java代码生成器（可能还有第三方生成器）：  
  - SPEED (default): 编译器将生成用于序列化，反序列化和用于消息类型的其他常见操作的代码。这个代码是高度优化的。
  - CODE_SIZE: 编译器将生成最小的类，并将依靠共享的基于反射的代码来实现序列化，反序列化和各种其他操作。生成的代码比SPEED要小得多，但是操作会比较慢。类仍将实现与SPEED模式中完全相同的公共API。这种模式在包含非常大数量字段的.proto文件，并且不需要所有这些文件都是非常快速的应用程序中非常有用。
  - LITE_RUNTIME: 编译器将生成仅依赖于“lite”运行时库（libprotobuf-lite而不是libprotobuf）的类。lite运行时比整个库小得多（大约小一个数量级），但省略了描述符和反射等特定功能。这对于在移动电话等受限平台上运行的应用程序特别有用。编译器仍然会像在SPEED模式下一样生成所有方法的高效实现。生成的类将仅实现每种语言的MessageLite接口，该接口只提供完整的Message接口的一部分方法。
```
option optimize_for = CODE_SIZE;
```
 - cc_generic_services, java_generic_services, py_generic_services (file options): 指示编译器是否应该根据C++，Java和Python中的服务定义生成抽象的服务代码。由于历史原因，默认值为true。然而，从版本2.3.0（2010年1月）开始，认为RPC实现提供[代码生成器插件](https://developers.google.com/protocol-buffers/docs/reference/cpp/google.protobuf.compiler.plugin.pb)来生成特定于每个实现系统的代码是更加合适的，而不是依赖于“抽象”服务。  
```
// This file relies on plugins to generate service code.
option cc_generic_services = false;
option java_generic_services = false;
option py_generic_services = false;
```
 - cc_enable_arenas (file option): 为C++生成的代码启用[arena allocation](https://developers.google.com/protocol-buffers/docs/reference/arenas).
 - message_set_wire_format (message option): 如果设置为true，则该消息使用不同的二进制编码格式，旨在与Google中称为MessageSet的旧格式兼容。Google以外的用户可能永远不需要使用此选项。该消息必须完全如下所示：
```
message Foo {
  option message_set_wire_format = true;
  extensions 4 to max;
}
```
 - packed (field option): 如果在基本数字类型的repeated字段上设置为true，则使用更紧凑的编码。使用此选项没有缺点。但是，请注意，在版本2.3.0之前，解析器将忽略收到未预期打包的数据。因此，不可能将现有字段更改为packed格式，而不破坏编码的兼容性。在2.3.0和更高版本中，这种变化是安全的，因为可压缩字段的序列化、反序列化解析器将始终接受这两种格式，但要小心使用旧的protobuf版本处理旧程序。
```
repeated int32 samples = 4 [packed=true];
```
 - deprecated (field option): 如果设置为true，则表示该字段已被弃用，不应被新代码使用。在大多数语言中，这没有实际的效果。在Java中，这变成了@Deprecated注释。将来，其他特定于语言的代码生成器可能会在字段的访问器上生成弃用注释，这会在编译试图使用该字段的代码时发出警告。如果该字段没有被任何人使用，并且你想阻止新用户使用该字段，请考虑用reserved语句替换该字段声明。  
```
optional int32 old_field = 6 [deprecated=true];
```

### 自定义选项
protobuf甚至允许你定义和使用你自己的选项。请注意，这是大多数人不需要的高级功能。由于选项是由google/protobuf/descriptor.proto（如FileOptions或FieldOptions）中的消息定义的，因此定义自己的选项只是扩展这些消息的问题。例如：  
```
import "google/protobuf/descriptor.proto";

extend google.protobuf.MessageOptions {
  optional string my_option = 51234;
}

message MyMessage {
  option (my_option) = "Hello world!";
}
```
这里我们通过扩展MessageOptions来定义一个新的消息级选项。当我们使用该选项时，选项名称必须用括号括起来表示它是一个扩展名。现在在C++中我们可以读取my_option的值了，如下所示：  
```
string value = MyMessage::descriptor()->options().GetExtension(my_option);
```
在Java中：  
```
String value = MyProtoFile.MyMessage.getDescriptor().getOptions().getExtension(MyProtoFile.myOption);
```
在Python中：  
```
value = my_proto_file_pb2.MyMessage.DESCRIPTOR.GetOptions().Extensions[my_proto_file_pb2.my_option]
```
可以为Protocol Buffers语言中的每种元素定义自定义选项。这是一个使用各种选项的例子：  
```C++
import "google/protobuf/descriptor.proto";

extend google.protobuf.FileOptions {
  optional string my_file_option = 50000;
}
extend google.protobuf.MessageOptions {
  optional int32 my_message_option = 50001;
}
extend google.protobuf.FieldOptions {
  optional float my_field_option = 50002;
}
extend google.protobuf.EnumOptions {
  optional bool my_enum_option = 50003;
}
extend google.protobuf.EnumValueOptions {
  optional uint32 my_enum_value_option = 50004;
}
extend google.protobuf.ServiceOptions {
  optional MyEnum my_service_option = 50005;
}
extend google.protobuf.MethodOptions {
  optional MyMessage my_method_option = 50006;
}

option (my_file_option) = "Hello world!";

message MyMessage {
  option (my_message_option) = 1234;

  optional int32 foo = 1 [(my_field_option) = 4.5];
  optional string bar = 2;
}

enum MyEnum {
  option (my_enum_option) = true;

  FOO = 1 [(my_enum_value_option) = 321];
  BAR = 2;
}

message RequestType {}
message ResponseType {}

service MyService {
  option (my_service_option) = FOO;

  rpc MyMethod(RequestType) returns(ResponseType) {
    // Note:  my_method_option has type MyMessage.  We can set each field
    //   within it using a separate "option" line.
    option (my_method_option).foo = 567;
    option (my_method_option).bar = "Some string";
  }
}
```
请注意，如果要在定义选项的包以外使用自定义选项，则必须在选项名称前加上包的名称，就像类型名称一样。  
```
// foo.proto
import "google/protobuf/descriptor.proto";
package foo;
extend google.protobuf.MessageOptions {
  optional string my_option = 51234;
}
// bar.proto
import "foo.proto";
package bar;
message MyMessage {
  option (foo.my_option) = "Hello world!";
}
```
 *最后一件事* ： 由于自定义选项是通过扩展实现的，因此必须像其他字段或扩展名一样指定字段编号。在上面的例子中，我们使用了50000-99999范围内的字段编号。这个范围保留给内部使用，因此你可以自由使用此范围内的数字作为内部应用程序。但是，如果你打算在公共应用程序中使用自定义选项，那么确保你的字段号码是全球唯一的是非常重要的。要获取全球唯一的字段号码，请发送请求至protobuf-global-extension-registry@google.com。只需提供你的项目名称（例如Object-C插件）和你的项目站点（如果可用）。通常你只需要一个扩展号码。然后如下进行扩展：  
```
message FooOptions {
  optional int32 opt1 = 1;
  optional string opt2 = 2;
}

extend google.protobuf.FieldOptions {
  optional FooOptions foo_options = 1234;
}

// usage:
message Bar {
  optional int32 a = 1 [(foo_options).opt1 = 123, (foo_options).opt2 = "baz"];
  // alternative aggregate syntax (uses TextFormat):
  optional int32 b = 2 [(foo_options) = { opt1: 123 opt2: "baz" }];
}
```
另外，请注意，每个选项类型（文件级别，消息级别，字段级别等）具有其自己的数字空间，例如，你可以用相同的数字来声明FieldOptions和MessageOptions的扩展。

## 代码生成
要生成Java，Python或C++代码，你需要使用.proto文件中定义的消息类型，在.proto文件上运行protobuf编译器protoc。编译器如下调用：  
```
protoc --proto_path=IMPORT_PATH --cpp_out=DST_DIR --java_out=DST_DIR --python_out=DST_DIR path/to/file.proto
```
其中：
 - IMPORT_PATH: 指定解析import指令时要查找.proto文件的目录，如果省略，则使用当前目录。可以通过多次传递--proto_path选项来指定多个导入目录，将按指定的顺序搜索。-I=IMPORT_PATH可以用作--proto_path的一个简短形式。
 - 你可以提供一个或多个输出指令：  
  - --cpp_out: 在DST_DIR中生成C++代码。
  - --java_out: 在DST_DIR中生成Java代码。
  - --python_out: 在DST_DIR中生成Python代码。
为方便起见，如果DST_DIR以.zip或.jar结尾，则编译器会将输出写入一个具有给定名称的ZIP格式存档文件。根据Java JAR规范的要求，.jar输出也会生成一个清单文件在里边。请注意，如果输出文件已经存在，它将被覆盖；编译器不够智能，无法将文件添加到现有的存档文件中。
 - 你必须提供一个或多个.proto文件作为输入。可以一次性指定多个.proto文件。虽然这些文件是相对于当前目录命名的，每个文件必须驻留在其中一个IMPORT_PATH中，以便编译器可以确定其规范名称。
