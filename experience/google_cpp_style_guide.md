# 引言
后台开发三年来一直使用C++作为主要开发语言，其强大的功能和性能自不必说，但是C++也是一门非常复杂的编程语言。掌握良好的编码风格，可以使代码更加清晰，易于维护。接下来将对C++编码风格进行学习。学习的材料主要基于google的C++编码风格。  

# 背景
C++是 Google大部分开源项目的主要编程语言。正如每个C++程序员都知道的, C++ 有很多强大的特性, 但这种强大不可避免的导致它走向复杂，使代码更容易产生 bug, 难以阅读和维护。  
本指南的目的是通过详细阐述 C++ 注意事项来驾驭其复杂性. 这些规则在保证代码易于管理的同时, 也能高效使用 C++ 的语言特性。  
风格, 亦被称作可读性, 也就是指导 C++ 编程的约定。使用术语 “风格” 有些用词不当, 因为这些习惯远不止源代码文件格式化这么简单。  
使代码易于管理的方法之一是加强代码一致性. 让任何程序员都可以快速读懂你的代码这点非常重要。保持统一编程风格并遵守约定意味着可以很容易根据 “模式匹配” 规则来推断各种标识符的含义。创建通用, 必需的习惯用语和模式可以使代码更容易理解。在一些情况下可能有充分的理由改变某些编程风格, 但我们还是应该遵循一致性原则，尽量不这么做。  
本指南的另一个观点是 C++ 特性的臃肿。C++ 是一门包含大量高级特性的庞大语言。某些情况下, 我们会限制甚至禁止使用某些特性。这么做是为了保持代码清爽,避免这些特性可能导致的各种问题。指南中列举了这类特性, 并解释为什么这些特性被限制使用。  
Google 主导的开源项目均符合本指南的规定。  
**注意**: 本指南并非 C++ 教程, 我们假定读者已经对 C++ 非常熟悉。  

# 1、头文件
通常每一个.cc 文件都有一个对应的.h文件。也有一些常见例外, 如单元测试代码和只包含 main() 函数的.cc 文件。  
正确使用头文件可令代码在可读性、文件大小和性能上大为改观。  
下面的规则将引导你规避使用头文件时的各种陷阱。  

## 1.1 Self-contained 头文件
头文件应该能够自给自足（self-contained,也就是可以作为第一个头文件被引入），以 .h 结尾。至于用来插入文本的文件，说到底它们并不是头文件，所以应以 .inc 结尾。不允许分离出 -inl.h 头文件的做法.  

所有头文件要能够自给自足。换言之，用户和重构工具不需要为特别场合而包含额外的头文件。详言之，一个头文件要有 :ref:`define-guard`，统统包含它所需要的其它头文件，也不要求定义任何特别 symbols.  

不过有一个例外，即一个文件并不是 self-contained 的，而是作为文本插入到代码某处。或者，文件内容实际上是其它头文件的特定平台（platform-specific）扩展部分。这些文件就要用 .inc 文件扩展名。  

如果 .h 文件声明了一个模板或内联函数，同时也在该文件加以定义。凡是有用到这些的 .cc 文件，就得统统包含该头文件，否则程序可能会在构建中链接失败。不要把这些定义放到分离的 -inl.h 文件里（译者注：过去该规范曾提倡把定义放到 -inl.h 里过）。  

有个例外：如果某函数模板为所有相关模板参数显式实例化，或本身就是某类的一个私有成员，那么它就只能定义在实例化该模板的 .cc 文件里。  

## 1.2 #define 保护
所有头文件都应该使用 #define 来防止头文件被多重包含, 命名格式当是: <PROJECT>_<PATH>_<FILE>_H_ .  

为保证唯一性, 头文件的命名应该基于所在项目源代码树的全路径. 例如, 项目 foo 中的头文件 foo/src/bar/baz.h 可按如下方式保护:  
```C++
#ifndef FOO_BAR_BAZ_H_
#define FOO_BAR_BAZ_H_
...
#endif // FOO_BAR_BAZ_H_
```

## 1.3 前置声明
尽可能地避免使用前置声明。使用 #include 包含需要的头文件即可。  
### 定义 
所谓「前置声明」（forward declaration）是类、函数和模板的纯粹声明，没伴随着其定义.
### 优点
1. 前置声明能够节省编译时间，多余的 #include 会迫使编译器展开更多的文件，处理更多的输入。
2. 前置声明能够节省不必要的重新编译的时间。 #include 使代码因为头文件中无关的改动而被重新编译多次。  
### 缺点
1. 前置声明隐藏了依赖关系，头文件改动时，用户的代码会跳过必要的重新编译过程。  
2. 前置声明可能会被库的后续更改所破坏。前置声明函数或模板有时会妨碍头文件开发者变动其 API. 例如扩大形参类型，加个自带默认参数的模板形参等等。
3. 前置声明来自命名空间 std:: 的 symbol 时，其行为未定义。
4. 很难判断什么时候该用前置声明，什么时候该用 #include 。极端情况下，用前置声明代替 includes 甚至都会暗暗地改变代码的含义：  
 ```C++
 // b.h:
struct B {};
struct D : B {}

// good_user.cc:
#include "b.h"
void f(B*);
void f(void*);
void test(D* x) { f(x); }  // calls f(B*)
// 如果 #include 被 B 和 D 的前置声明替代， test() 就会调用 f(void*) .
 ```
5. 前置声明了不少来自头文件的 symbol 时，就会比单单一行的 include 冗长。  
6. 仅仅为了能前置声明而重构代码（比如用指针成员代替对象成员）会使代码变得更慢更复杂.
### 结论
1. 尽量避免前置声明那些定义在其他项目中的实体.
2. 函数：总是使用 #include.
3. 类模板：优先使用 #include.

## 1.4 内联函数
只有当函数只有 10 行甚至更少时才将其定义为内联函数.
### 定义
当函数被声明为内联函数之后, 编译器会将其内联展开, 而不是按通常的函数调用机制进行调用.  
### 优点
只要内联的函数体较小, 内联该函数可以令目标代码更加高效. 对于存取函数以及其它函数体比较短, 性能关键的函数, 鼓励使用内联.  
### 缺点
滥用内联将导致程序变得更慢. 内联可能使目标代码量或增或减, 这取决于内联函数的大小. 内联非常短小的存取函数通常会减少代码大小, 但内联一个相当大的函数将戏剧性的增加代码大小. 现代处理器由于更好的利用了指令缓存, 小巧的代码往往执行更快。  
### 结论
1. 一个较为合理的经验准则是, 不要内联超过 10 行的函数. 谨慎对待析构函数, 析构函数往往比其表面看起来要更长, 因为有隐含的成员和基类析构函数被调用!  
2. 另一个实用的经验准则: 内联那些包含循环或 switch 语句的函数常常是得不偿失 (除非在大多数情况下, 这些循环或 switch 语句从不被执行).   
3. 有些函数即使声明为内联的也不一定会被编译器内联, 这点很重要; **比如虚函数和递归函数就不会被正常内联**. 通常, 递归函数不应该声明成内联函数. 虚函数内联的主要原因则是想把它的函数体放在类定义内, 为了图个方便, 抑或是当作文档描述其行为, 比如精短的存取函数.  

## 1.5 #include 的路径及顺序
使用标准的头文件包含顺序可增强可读性, 避免隐藏依赖: **相关头文件**, C 库, C++ 库, 其他库的 .h, 本项目内的 .h.  
项目内头文件应按照项目源代码目录树结构排列, 避免使用 UNIX 特殊的快捷目录 . (当前目录) 或 .. (上级目录). 例如，google-awesome-project/src/base/logging.h 应该按如下方式包含:  
```C++
#include "base/logging.h"
```
又如, dir/foo.cc 或 dir/foo_test.cc 的主要作用是实现或测试 dir2/foo2.h 的功能, foo.cc 中包含头文件的次序如下:  
1. dir2/foo2.h (优先位置, 详情如下)  
2. 空白行  
3. C 系统文件  
4. C++ 系统文件  
5. 空白行  
6. 其他库的 .h 文件  
7. 本项目内 .h 文件  
这种优先的顺序排序保证当 dir2/foo2.h 遗漏某些必要的库时， dir/foo.cc 或 dir/foo_test.cc 的构建会立刻中止。因此这一条规则保证维护这些文件的人们首先看到构建中止的消息而不是维护其他包的人们。  

dir/foo.cc 和 dir2/foo2.h 通常位于同一目录下 (如 base/basictypes_unittest.cc 和 base/basictypes.h), 但也可以放在不同目录下.  

按字母顺序分别对每种类型的头文件进行二次排序是不错的主意。注意较老的代码可不符合这条规则，要在方便的时候改正它们。  

您所依赖的符号 (symbols) 被哪些头文件所定义，您就应该包含（include）哪些头文件，前置声明 (forward declarations) 情况除外。比如您要用到 bar.h 中的某个符号, 哪怕您所包含的 foo.h 已经包含了 bar.h, 也照样得包含 bar.h, 除非 foo.h 有明确说明它会自动向您提供 bar.h 中的 symbol. 不过，凡是 cc 文件所对应的「相关头文件」已经包含的，就不用再重复包含进其 cc 文件里面了，就像 foo.cc 只包含 foo.h 就够了，不用再管后者所包含的其它内容。  

举例来说, google-awesome-project/src/foo/internal/fooserver.cc 的包含次序如下:  
```C++
#include "foo/public/fooserver.h" // 优先位置

#include <sys/types.h>
#include <unistd.h>

#include <hash_map>
#include <vector>

#include "base/basictypes.h"
#include "base/commandlineflags.h"
#include "foo/public/bar.h"
```
例外：  
有时，平台特定（system-specific）代码需要条件编译（conditional includes），这些代码可以放到其它 includes 之后。当然，您的平台特定代码也要够简练且独立，比如：  
```C++
#include "foo/public/fooserver.h"

#include "base/port.h"  // For LANG_CXX11.

#ifdef LANG_CXX11
#include <initializer_list>
#endif  // LANG_CXX11
```

# 2、作用域
## 2.1 命名空间
鼓励在 .cc 文件内使用匿名命名空间或 static 声明. 使用具名的命名空间时, 其名称可基于项目名或相对路径. 禁止使用 using 指示（using-directive）。**禁止使用内联命名空间（inline namespace）**。  
### 定义
命名空间将全局作用域细分为独立的, 具名的作用域, 可有效防止全局作用域的命名冲突.
### 优点
1. 虽然类已经提供了（可嵌套的）命名轴线 (将命名分割在不同类的作用域内), 命名空间在这基础上又封装了一层。举例来说, 两个不同项目的全局作用域都有一个类 Foo, 这样在编译或运行时造成冲突. 如果每个项目将代码置于不同命名空间中, project1::Foo 和 project2::Foo 作为不同符号自然不会冲突。  
2. 内联命名空间会自动把内部的标识符放到外层作用域，比如：  
```C++
namespace X {
inline namespace Y {
void foo();
}  // namespace Y
}  // namespace X
```
X::Y::foo() 与 X::foo() 彼此可代替。内联命名空间主要用来保持跨版本的 ABI 兼容性。  
### 缺点
1. 命名空间具有迷惑性, 因为它们使得区分两个相同命名所指代的定义更加困难。  
2. 内联命名空间很容易令人迷惑，毕竟其内部的成员不再受其声明所在命名空间的限制。内联命名空间只在大型版本控制里有用。  
3. 有时候不得不多次引用某个定义在许多嵌套命名空间里的实体，使用完整的命名空间会导致代码的冗长。  
4. 在头文件中使用匿名空间导致违背 C++ 的唯一定义原则 (One Definition Rule (ODR)).  
### 结论
1. 遵循[命名空间命名](https://google.github.io/styleguide/cppguide.html#Namespace_Names)中的规则。  
2. 像之前的几个例子中一样，在命名空间的最后注释出命名空间的名字。  
3. 用命名空间把文件包含, gflags 的声明/定义, 以及类的前置声明以外的整个源文件封装起来, 以区别于其它命名空间:  
```C++
// .h 文件
namespace mynamespace {

// 所有声明都置于命名空间中
// 注意不要使用缩进
class MyClass {
    public:
    ...
    void Foo();
};

} // namespace mynamespace
//================================================
// .cc 文件
namespace mynamespace {

// 函数定义都置于命名空间中
void MyClass::Foo() {
    ...
}

} // namespace mynamespace

// 更复杂的 .cc 文件包含更多, 更复杂的细节, 比如 gflags 或 using 声明。
//================================================
#include "a.h"

DEFINE_FLAG(bool, someflag, false, "dummy flag");

namespace a {

...code for a...                // 左对齐

} // namespace a
```
4. 不要在命名空间 std 内声明任何东西, 包括标准库的类前置声明. 在 std 命名空间声明实体是未定义的行为, 会导致如不可移植. 声明标准库下的实体, 需要包含对应的头文件.  
5. 不应该使用 using 指示 引入整个命名空间的标识符号。
```C++
// 禁止 —— 污染命名空间
using namespace foo;
```
6. 不要在头文件中使用 命名空间别名 除非显式标记内部命名空间使用。因为任何在头文件中引入的命名空间都会成为公开API的一部分。  
```C++
// 在 .cc 中使用别名缩短常用的命名空间
namespace baz = ::foo::bar::baz;  

//================================================
// 在 .h 中使用别名缩短常用的命名空间
namespace librarian {
namespace impl {  // 仅限内部使用
namespace sidetable = ::pipeline_diagnostics::sidetable;
}  // namespace impl

inline void my_inline_function() {
  // 限制在一个函数中的命名空间别名
  namespace baz = ::foo::bar::baz;
  ...
}
}  // namespace librarian
```
7. 禁止用内联命名空间.  

## 2.2 匿名命名空间和静态变量
在 .cc 文件中定义一个不需要被外部引用的变量时，可以将它们放在匿名命名空间或声明为 static 。但是不要在 .h 文件中这么做。  
### 定义
所有置于匿名命名空间的声明都具有内部链接性，函数和变量可以经由声明为 static 拥有内部链接性，这意味着你在这个文件中声明的这些标识符都不能在另一个文件中被访问。即使两个文件声明了完全一样名字的标识符，它们所指向的实体实际上是完全不同的。  
### 结论
推荐、鼓励在 .cc 中对于不需要在其他地方引用的标识符使用内部链接性声明，但是不要在 .h 中使用。  
匿名命名空间的声明和具名的格式相同，在最后注释上 namespace :
```C++
namespace {
...
}  // namespace
```
## 2.3 非成员函数、静态成员函数和全局函数
使用静态成员函数或命名空间内的非成员函数, 尽量不要用裸的全局函数. 将一系列函数直接置于命名空间中，**不要用类的静态方法模拟出命名空间的效果，类的静态方法应当和类的实例或静态数据紧密相关**.  
### 优点
某些情况下, 非成员函数和静态成员函数是非常有用的, 将非成员函数放在命名空间内可避免污染全局作用域.
### 缺点
将非成员函数和静态成员函数作为新类的成员或许更有意义, 当它们需要访问外部资源或具有重要的依赖关系时更是如此.
### 结论
有时, 把函数的定义同类的实例脱钩是有益的, 甚至是必要的. 这样的函数可以被定义成静态成员, 或是非成员函数. 非成员函数不应依赖于外部变量, 应尽量置于某个命名空间内. 相比单纯为了封装若干不共享任何静态数据的静态成员函数而创建类, 不如使用 2.1. 命名空间 。举例而言，对于头文件 myproject/foo_bar.h , 应当使用  
```C++
namespace myproject {
namespace foo_bar {
void Function1();
void Function2();
}  // namespace foo_bar
}  // namespace myproject
```
而非
```C++
namespace myproject {
class FooBar {
 public:
  static void Function1();
  static void Function2();
};
}  // namespace myproject
```
定义在同一编译单元的函数, 被其他编译单元直接调用可能会引入不必要的耦合和链接时依赖; 静态成员函数对此尤其敏感. 可以考虑提取到新类中, 或者将函数置于独立库的命名空间内.  
如果你必须定义非成员函数, 又只是在 .cc 文件中使用它, 可使用匿名 2.1. 命名空间 或 static 链接关键字 (如 static int Foo() {...}) 限定其作用域.  

## 2.4 局部变量
将函数变量尽可能置于最小作用域内, 并在变量声明时进行初始化.  
```C++
int i;
i = f(); // 坏——初始化和声明分离
int j = g(); // 好——初始化时声明

vector<int> v;
v.push_back(1); // 用花括号初始化更好
v.push_back(2);
vector<int> v = {1, 2}; // 好——v 一开始就初始化
```
属于 if, while 和 for 语句的变量应当在这些语句中正常地声明，这样子这些变量的作用域就被限制在这些语句中了，举例而言:
```C++
while (const char* p = strchr(str, '/')) str = p + 1;
```
### 警告
有一个例外, 如果变量是一个对象, 每次进入作用域都要调用其构造函数, 每次退出作用域都要调用其析构函数. 这会导致效率降低.  
```C++
// 低效的实现
for (int i = 0; i < 1000000; ++i) {
    Foo f;                  // 构造函数和析构函数分别调用 1000000 次!
    f.DoSomething(i);
}
```
在循环作用域外面声明这类变量要高效的多:
```C++
Foo f;                      // 构造函数和析构函数只调用 1 次
for (int i = 0; i < 1000000; ++i) {
    f.DoSomething(i);
}
```
## 2.5 静态和全局变量
禁止定义静态存储周期非POD变量；禁止使用含有副作用的函数初始化POD全局变量，因为多编译单元中的静态变量执行时的构造和析构顺序是未明确的，这将导致代码的不可移植。  
禁止使用类的 静态储存周期 变量：由于构造和析构函数调用顺序的不确定性，它们会导致难以发现的 bug 。不过 constexpr 变量除外，毕竟它们又不涉及动态初始化或析构。  
静态生存周期的对象，即包括了全局变量，静态变量，静态类成员变量和函数静态变量，都必须是原生数据类型 (POD : Plain Old Data): 即 int, char 和 float, 以及 POD 类型的指针、数组和结构体。  
静态变量的构造函数、析构函数和初始化的顺序在 C++ 中是只有部分明确的，甚至随着构建变化而变化，导致难以发现的 bug. 所以除了禁用类类型的全局变量，我们也不允许用函数返回值来初始化 POD 变量，除非该函数（比如 getenv() 或 getpid() ）不涉及任何全局变量。函数作用域里的静态变量除外，毕竟它的初始化顺序是有明确定义的，而且只会在指令执行到它的声明那里才会发生。  
**注释：同一个编译单元内是明确的，静态初始化优先于动态初始化，初始化顺序按照声明顺序进行，销毁则逆序。不同的编译单元之间初始化和销毁顺序属于未明确行为 (unspecified behaviour)。**  
同理，全局和静态变量在程序中断时会被析构，无论所谓中断是从 main() 返回还是对 exit() 的调用。析构顺序正好与构造函数调用的顺序相反。但既然构造顺序未定义，那么析构顺序当然也就不定了。比如，在程序结束时某静态变量已经被析构了，但代码还在跑——比如其它线程——并试图访问它且失败；再比如，一个静态 string 变量也许会在一个引用了前者的其它变量析构之前被析构掉。  
改善以上析构问题的办法之一是用 quick_exit() 来代替 exit() 并中断程序。它们的不同之处是前者不会执行任何析构，也不会执行 atexit() 所绑定的任何 handlers. 如果您想在执行 quick_exit() 来中断时执行某 handler（比如刷新 log），您可以把它绑定到 _at_quick_exit(). 如果您想在 exit() 和 quick_exit() 都用上该 handler, 都绑定上去。  
综上所述，我们只允许 POD 类型的静态变量，即完全禁用 vector (使用 C 数组替代) 和 string (使用 const char [])。  
如果您确实需要一个 class 类型的静态或全局变量，可以考虑在 main() 函数或 pthread_once() 内初始化一个指针且永不回收。注意只能用 raw 指针，别用智能指针，毕竟后者的析构函数涉及到上文指出的不定顺序问题。  
**注释：上文提及的静态变量泛指静态生存周期的对象, 包括: 全局变量, 静态变量, 静态类成员变量, 以及函数静态变量.**  

## 2.7 自己小结
1. cc 中的匿名命名空间可避免命名冲突, 限定作用域, 避免直接使用 using 关键字污染命名空间;  
2. 嵌套类符合局部使用原则, 只是不能在其他头文件中前置声明;  
3. 尽量不用全局函数和全局变量, 考虑作用域和命名空间限制;  
4. 多线程中的全局变量 (含静态成员变量) 不要使用 class 类型 (含 STL 容器), 避免不明确的构造和析构行为导致的 bug；  
5. 注意「using 指示（using-directive）」和「using 声明（using-declaration）」的区别；  
6. 匿名命名空间说白了就是文件作用域，就像 C static 声明的作用域一样；  
7. 局部变量在声明的同时进行显式值初始化，比起隐式初始化再赋值的两步过程要高效；  
8. 注意别在循环犯大量构造和析构的低级错误，即：要考虑最小作用域原则，但是也要注重性能。  

# 3 、类
类是 C++ 中代码的基本单元. 显然, 它们被广泛使用. 本节列举了在写一个类时的主要注意事项。  

