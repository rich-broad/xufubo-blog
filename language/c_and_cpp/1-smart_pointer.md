# 1. 概述
C/C++ 语言与其他的编程语言最大的不同之处就是内存完全由程序员管理。这是最为强大的特性，也是最为人所诟病的特性。这一特性带来的最大问题就是内存泄漏，因此后来的大多数语言都提供了内置内存分配与释放功能，有的甚至干脆对语言的使用者屏蔽了指针这一概念。这里不做评价，任何一种技术都有其两面性，任何事情都有两面性。这是两种不同的设计哲学。有人认为，内存如此重要的东西怎么能放心交给用户去管理呢？而另外一些人则认为，内存如此重要的东西怎么能放心交给系统去管理呢？在 C/C++ 语言中，内存泄露的问题一直困扰着广大的开发者，因此各类库和工具一直在努力尝试各种方法去检测和避免内存泄露，其中C/C++解决内存泄漏最为常用也最为有用的方法就是智能指针，本文详细探讨C++11标准里提供的几种智能指针。 
  
|指针类别|支持|特性|  
|-|-|-|   
|unique_ptr|C++ 11|拥有独有对象所有权语义的智能指针|  
|shared_ptr|C++ 11|拥有共享对象所有权语义的智能指针|  
|weak_ptr|C++ 11|到 std::shared_ptr 所管理对象的弱引用|  
|auto_ptr|C++ 17中移除|拥有严格对象所有权语义的智能指针|  


2020年还讨论在C++17中移除的智能指针std::auto_ptr是不是太过时了呢？我觉得不然，虽然std::auto_ptr被移除了（取而代之的是std::unique_ptr）,但是我们还是很有必要研究一下它。了解了std::auto_ptr 的用法以及它的设计不足之处才能更好的理解现在的设计不是吗？一项技术如果我们了解了它的发展历史，我们就能更好地掌握它，不是吗？其实任何事情都是这样的：**知史以明鉴，查古以至今**。  

# 2. 98/03的探索-auto_ptr
auto_ptr作为最早的智能指针，它设计的本意只是简单的利用C++对于栈区对象的自动析构管理堆区对象。但是它最反人性的设计是：  
 - 让一切赋值行为变成了资源转移。这一点不符合最小惊讶原则，你很难一上来就接受原件被“拷贝/赋值”以后会失效这件事。所以C++11使用unique_ptr代替auto_ptr，除非在自身即将消亡的时候，资源的转交只能通过另一成员release显式完成。总而言之，auto_ptr最大的弊端在于允许很多几乎没有实际用处又不符合常规认知的行为，这种反人性的设计也常常是出错的根源。  

用scoped_ptr和unique_ptr可以利用静态检查早早地指出这些无意义操作的存在。  

下边我们看看auto_ptr的使用：  

```C++
//初始化方式1
std::auto_ptr<int> sp1(new int(8));
//初始化方式2
std::auto_ptr<int> sp2;
sp2.reset(new int(8));
```
智能指针对象 sp1 和 sp2 均持有一个在堆上分配 int 对象，其值均是 8，这两块堆内存均可以在 sp1 和 sp2 释放时得到释放。这是 std::auto_ptr 的基本用法。  

如上所述：std::auto_ptr 真正让人容易误用的地方是其不常用的复制语义，即当复制一个 std::auto_ptr 对象时（拷贝复制或 operator = 复制），原对象所持有的堆内存对象也会转移给复制出来的对象。原有智能指针不可用。示例代码如下：  
```C++
#include <iostream>
#include <memory>

int main()
{
    //测试拷贝构造
    std::auto_ptr<int> sp1(new int(0));
    std::auto_ptr<int> sp2(sp1);
    std::cout << (sp1.get() != NULL ? "sp1 is not NULL." : "sp1 is NULL") :  << std::endl;
    std::cout << (sp2.get() != NULL ? "sp2 is not NULL." : "sp2 is NULL") :  << std::endl;

    //测试赋值构造
    std::auto_ptr<int> sp3(new int(0));
    std::auto_ptr<int> sp4 = sp3;
    std::cout << (sp3.get() != NULL ? "sp3 is not NULL." : "sp3 is NULL") :  << std::endl;
    std::cout << (sp4.get() != NULL ? "sp4 is not NULL." : "sp4 is NULL") :  << std::endl;

    return 0;
}
/*
输出如下：
sp1 is NULL
sp2 is not NULL.
sp3 is NULL
sp4 is not NULL.
*/
```
由于 std::auto_ptr 这种反人性的复制语义，我们应该避免在容器中使用 std::auto_ptr，例如我们绝不应该写出如下代码：  

std::vector<std::auto_ptr<int>> my_vt;
当用算法对容器操作的时候（如最常见的容器元素遍历），很难避免不对容器中的元素实现赋值传递，这样便会使容器中多个元素被置为空指针，这不是我们想看到的，会造成很多意想不到的错误。因此，作为 std::auto_ptr 的替代者 std::unique_ptr 吸取了这个经验教训。std::auto_ptr 已经被废弃/移除，你的代码不应该再使用它。  

C++11 标准在充分借鉴和吸收了 boost 库中智能指针的设计思想，引入了三种类型的智能指针，即 std::unique_ptr、std::shared_ptr 和 std::weak_ptr。所有的智能指针类均包含于头文件 <memory> 中。
下边我们详细介绍C++11引入的三类智能指针。  

# 3. std::unique_ptr

std::unique_ptr 对其持有的堆内存具有唯一拥有权，也就是说引用计数永远是 1，std::unique_ptr 对象销毁时会释放其持有的堆内存。默认情况下(不传入自定义析构器时)， std::unique_ptr 和原始指针大小一样，对于大多数的操作，它们执行相同的指令，也就是说在内存和计算资源吃紧的地方也可以用 std::unique_ptr。std::unique_ptr 呈现出来的是独占使用权语义，因此， std::unqiue_ptr 不能拷贝，只能**移动**，析构时非空的 std::unique_ptr 会销毁它的资源，默认情况下， std::unique_ptr 会对内部的原始指针使用 delete 来释放原始指针所指向的资源。   

可以使用以下方式初始化一个 std::unique_ptr 对象：  
```C++
//初始化方式1
std::unique_ptr<int> sp1(new int(123));
//初始化方式2
std::unique_ptr<int> sp2;
sp2.reset(new int(123));
//初始化方式3
std::unique_ptr<int> sp3 = std::make_unique<int>(123); //C++14才支持
// 可以自己实现一下make_unique
template<typename T, typename... Ts>
std::unique_ptr<T> make_unique(Ts&& ...params)
{
    return std::unique_ptr<T>(new T(std::forward<Ts>(params)...));
}
```
你应该尽量使用初始化方式 3 的方式去创建一个 std::unique_ptr 而不是方式 1 和 2，因为形式 3 更安全。鉴于 std::auto_ptr 的前车之鉴，std::unique_ptr 禁止复制语义，为了达到这个效果，std::unique_ptr 类的拷贝构造函数和赋值运算符（operator =）被标记为 delete：   
```C++
template <class T>
class unique_ptr
{
    //略去其他代码
    unique_ptr(const unique_ptr&) = delete;
    unique_ptr& operator=(const unique_ptr&) = delete;
};
```
因此，下列代码是无法通过编译的：  
```C++
std::unique_ptr<int> sp1(std::make_unique<int>(123));;
//以下代码无法通过编译
std::unique_ptr<int> sp2(sp1);
std::unique_ptr<int> sp3;
//以下代码无法通过编译
sp3 = sp1;
```
禁止复制语义也存在特例，即可以通过一个函数返回一个 std::unique_ptr：  
```C++
#include <memory>
std::unique_ptr<int> func(int val)
{
    std::unique_ptr<int> up(new int(val));
    return up;
}
int main(int argc, char* argv[])
{
    std::unique_ptr<int> sp1 = func(123);
    return 0;
}
```
上述代码从 func 函数中得到一个 std::unique_ptr 对象，然后返回给 sp1。既然 std::unique_ptr 不能复制，那么如何将一个 std::unique_ptr 对象持有的堆内存转移给另外一个呢？答案是使用移动构造，示例代码如下：  
```C++
int main(int argc, char* argv[])
{
    std::unique_ptr<int> sp1(std::make_unique<int>(123));
    std::unique_ptr<int> sp2(std::move(sp1));
    std::unique_ptr<int> sp3;
    sp3 = std::move(sp2);
    std::cout << sp1.get() << ", " << sp2.get() << ", " << sp3.get() << std::endl; 
    return 0;
}
/*
输出如下：（后边我们会看看std::shared_ptr的实例）
0, 0, 0xddc010
*/
```
以上代码利用 std::move 将 sp1 持有的堆内存（值为 123）转移给 sp2，再把 sp2 转移给 sp3。最后，sp1 和 sp2 不再持有堆内存的引用，变成一个空的智能指针对象。并不是所有的对象的 std::move 操作都有意义，只有实现了移动构造函数（Move Constructor）或移动赋值运算符（operator =）的类才行，而 std::unique_ptr 正好实现了这二者，以下是实现伪码：  
```C++
template<typename T, typename Deletor>
class unique_ptr
{
    //其他函数省略...
public:
    unique_ptr(unique_ptr&& rhs)
    {
        this->m_pT = rhs.m_pT;
        //源对象释放
        rhs.m_pT = nullptr;
    }
    unique_ptr& operator=(unique_ptr&& rhs)
    {
        this->m_pT = rhs.m_pT;
        //源对象释放
        rhs.m_pT = nullptr;
        return *this;
    }
private:
    T*    m_pT;
};
```
std::unique_ptr, std::shared_ptr 和 std::weak_ptr 都可以持有一组堆对象，这里不再翔述。  

前面我们说过：默认情况下， std::unique_ptr 会对内部的原始指针使用 delete 来释放原始指针所指向的资源。但是有的时候这是不够的，比如:  
 - 我们的对象持有fd；
 - 我们的对象持有数据库连接
等等，这时如果没有在析构函数中释放这些资源，这个时候就需要：**自定义智能指针对象持有的资源的释放函数了。** 假设现在有一个 MySQLConnection 类，对应着MySQL数据库连接，在回收时需要关闭该连接，不然数据库链接这种昂贵的资源会被使用完，我们可以如下自定义智能指针对象的资源析构函数，以 std::unique_ptr 为例：  

```C++
#include <iostream>
#include <memory>
class MySQLConnection
{
public:
    MySQLConnection(){}
    ~MySQLConnection(){}
    //关闭链接
    void close(){}
};
int main(int argc, char* argv[])
{
    auto deletor = [](MySQLConnection* pDBConn) {
        //关闭链接
        pDBConn->close();
        delete pDBConn;
    };

    std::unique_ptr<MySQLConnection, void(*)(MySQLConnection * pDBConn)> spDBConn(new MySQLConnection(), deletor);
    return 0;
}
```
自定义 std::unique_ptr 的资源释放函数其规则是：     
```C++
std::unique_ptr<T, DeletorFuncPtr>
```
其中 T 是你要释放的对象类型, DeletorFuncPtr 是一个自定义函数指针。上述代码有点负责，我们可以使用 decltype(deletor) 让编译器自己推导 deletor 的类型，因此可以将代码修改为：  
```C++
std::unique_ptr<Socket, decltype(deletor)> spSocket(new Socket(), deletor);
```

# 4. std::shared_ptr
std::unique_ptr 对其持有的资源具有独占性，而 std::shared_ptr 持有的资源可以在多个 std::shared_ptr 之间共享，每多一个 std::shared_ptr 对资源的引用，资源引用计数将增加 1，每一个指向该资源的 std::shared_ptr 对象析构时，资源引用计数减 1，最后一个 std::shared_ptr 对象析构时，发现资源计数为 0，将释放其持有的资源。多个线程之间，递增和减少资源的引用计数是安全的。（**注意：这不意味着多个线程同时操作 std::shared_ptr 引用的对象是安全的**）。std::shared_ptr 提供了一个 use_count() 方法来获取当前持有资源的引用计数。除了上面描述的，std::shared_ptr 用法和 std::unique_ptr 基本相同。  
下面是一个初始化 std::shared_ptr 的示例：  
```C++
//初始化方式1
std::shared_ptr<int> sp1(new int(123));
//初始化方式2
std::shared_ptr<int> sp2;
sp2.reset(new int(123));
//初始化方式3
std::shared_ptr<int> sp3;
sp3 = std::make_shared<int>(123);
```
和 std::unique_ptr 一样，你应该优先使用 std::make_shared 去初始化一个 std::shared_ptr 对象。std::shared_ptr在实现的时候使用的refcount技术，因此内部会有一个计数器（控制块，用来管理数据）和一个指针，指向数据。因此在执行std::shared_ptr<A> p2(new A) 的时候，首先会申请数据的内存，然后申请内控制块，因此是两次内存申请，而std::make_shared<A>()则是只执行一次内存申请，将数据和控制块的申请放到一起。    

再来看个例子:  
```C++
#include <iostream>
#include <memory>
class A
{
public:
    A(){    std::cout << "A constructor" << std::endl;  }
    ~A(){   std::cout << "A destructor" << std::endl;   }
};

int main(int argc, char* argv[])
{
    std::shared_ptr<A> sp1(new A());
    std::cout << "use count: " << sp1.use_count() << std::endl;  //1
    std::shared_ptr<A> sp2(sp1);
    std::cout << "use count: " << sp1.use_count() << std::endl;  //2
    sp2.reset();
    std::cout << "use count: " << sp1.use_count() << std::endl;  //1
    {
        std::shared_ptr<A> sp3 = sp1;
        std::cout << "use count: " << sp1.use_count() << std::endl;  //2
    }
    std::cout << "use count: " << sp1.use_count() << std::endl;  //1
    return 0;
}
/*
输出：
A constructor
use count: 1
use count: 2
use count: 1
use count: 2
use count: 1
A destructor
*/
int main(int argc, char* argv[])
{
    std::shared_ptr<int> sp1(std::make_shared<int>(123));
    std::shared_ptr<int> sp2(std::move(sp1));
    std::shared_ptr<int> sp3;
    sp3 = std::move(sp2);
    std::cout << sp1.get() << ", " << sp2.get() << ", " << sp3.get() << std::endl; 

    std::shared_ptr<int> sp4(std::make_shared<int>(123));
    std::shared_ptr<int> sp5(sp4);
    std::shared_ptr<int> sp6 = sp5;
    std::cout << sp4.get() << ", " << sp5.get() << ", " << sp6.get() << std::endl; 

    return 0;
}
/*
输出：
0, 0, 0x1d4e028
0x1d4e058, 0x1d4e058, 0x1d4e058
*/
```
## 4.1 std::enable_shared_from_this
实际开发中，有时候需要在类中返回包裹当前对象（this）的一个 std::shared_ptr 对象给外部使用，C++ 新标准也为我们考虑到了这一点，有如此需求的类只要继承自 std::enable_shared_from_this<T> 模板对象即可。用法如下：  
```C++
#include <iostream>
#include <memory>
class A : public std::enable_shared_from_this<A>
{
public:
    A(){    std::cout << "A constructor" << std::endl;  }
    ~A(){   std::cout << "A destructor" << std::endl;   }
    std::shared_ptr<A> getSelf(){   return shared_from_this();  }
};
int main(int argc, char* argv[])
{
    std::shared_ptr<A> sp1(new A());
    std::shared_ptr<A> sp2 = sp1->getSelf();
    std::cout << "use count: " << sp1.use_count() << std::endl;
    return 0;
}
/*
A constructor
use count: 2
A destructor
*/
```
enable_shared_from_this是好用，但是也有陷阱。
 - 不应该共享栈对象的 this 给智能指针对象
 - 避免 std::enable_shared_from_this 的循环引用问题

### 不应该共享栈对象的 this 给智能指针对象
```C++
int main(int argc, char* argv[])
{
    A a;
    std::shared_ptr<A> sp2 = a.getSelf();
    std::cout << "use count: " << sp2.use_count() << std::endl;
    return 0;
}
```

### 避免 std::enable_shared_from_this 的循环引用问题
```C++
#include <iostream>
#include <memory>
class A : public std::enable_shared_from_this<A>
{
public:
    //比较好的做法是在构造函数里面调用shared_from_this()给m_SelfPtr赋值
    //但是很遗憾不能这么做,如果写在构造函数里面程序会直接崩溃,自己都没构造完怎么能共享呢？
    A(){    std::cout << "A constructor" << std::endl;  }
    ~A(){    std::cout << "A destructor" << std::endl;   }
    void func(){    m_SelfPtr = shared_from_this();    }
public:
    int                 m_i;
    std::shared_ptr<A>  m_SelfPtr;
};

int main()
{
    {
        std::shared_ptr<A> spa(new A());
        spa->func();
    }
    return 0;
}
//输出：A constructor
```
我们发现在程序的整个生命周期内，只有 A 类构造函数的调用输出，没有 A 类析构函数的调用输出，这意味着 new 出来的 A 对象产生了内存泄漏了！  
我们来分析一下为什么 new 出来的 A 对象得不到释放。当程序执行到 42 行后，spa 出了其作用域准备析构，在析构时其发现仍然有另外的一个 std::shared_ptr 对象即 A::m_SelfPtr 引用了 A，因此 spa 只会将 A 的引用计数递减为 1，然后就销毁自身了。现在留下一个矛盾的处境：必须销毁 A 才能销毁其成员变量 m_SelfPtr，而销毁 m_SelfPtr 必须先销毁 A。这就是所谓的 std::enable_shared_from_this 的循环引用问题。我们在实际开发中应该避免做出这样的逻辑设计，这种情形下即使使用了智能指针也会造成内存泄漏。也就是说**一个资源的生命周期可以交给一个智能指针对象，但是该智能指针的生命周期不可以再交给这个资源来管理。智能指针之前的循环引用也是一样的**  

智能指针之间也不能循环引用，不然也会发生内存泄漏：   
```C++
#include <iostream>
#include <memory>

class B;
class A
{
public:
    shared_ptr<B> pb;
    A(){    cout << "constructor A" << endl;    }
    ~A(){    cout << "destructor A" << endl;    }
};
 
class B
{
public:
    shared_ptr<A> pa;
    B(){    cout << "constructor B" << endl;    }
    ~B(){    cout << "destructor B" << endl;    }
};
 
int main(int argc, char** argv)
{
    {
        shared_ptr<A> sa(new A());
        shared_ptr<B> sb(new B());
        if(sa && sb)
        {
            sa->pb=sb;
            sb->pa=sa;
        }
    }
    return 0;
}
/*
输出：
constructor A
constructor B
*/
```
可见，只有构造，没有析构，发生了内存泄漏。因此我们在使用智能指针时一定要注意循环引用的问题，智能指针虽好，你用的不好也不行。为此，我们接着学习C++11中提供的最后一个智能指针。  


## 5. std::weak_ptr
std::weak_ptr 是一个不控制资源生命周期的智能指针，是对对象的一种弱引用，只是提供了对其管理的资源的一个访问手段，引入它的目的为协助 std::shared_ptr 工作。std::weak_ptr 可以从一个 std::shared_ptr 或另一个 std::weak_ptr 对象构造，std::shared_ptr 可以直接赋值给 std::weak_ptr ，也可以通过 std::weak_ptr 的 lock() 函数来获得 std::shared_ptr。它的构造和析构不会引起引用计数的增加或减少。std::weak_ptr 可用来解决 std::shared_ptr 相互引用时的死锁问题（即两个std::shared_ptr 相互引用，那么这两个指针的引用计数永远不可能下降为 0， 资源永远不会释放）。  

举例如下：  
```C++
int main()
{
    //创建一个std::shared_ptr对象
    std::shared_ptr<int> sp1(new int(123));
    std::cout << "use count: " << sp1.use_count() << std::endl;

    //通过构造函数得到一个std::weak_ptr对象
    std::weak_ptr<int> sp2(sp1);
    std::cout << "use count: " << sp1.use_count() << std::endl;

    //通过赋值运算符得到一个std::weak_ptr对象
    std::weak_ptr<int> sp3 = sp1;
    std::cout << "use count: " << sp1.use_count() << std::endl;

    //通过一个std::weak_ptr对象得到另外一个std::weak_ptr对象
    std::weak_ptr<int> sp4 = sp2;
    std::cout << "use count: " << sp1.use_count() << std::endl;

    return 0;
}
// 输出均为：use count: 1
```
无论通过何种方式创建 std::weak_ptr 都不会增加资源的引用计数，因此每次输出引用计数的值都是 1。既然，std::weak_ptr 不管理对象的生命周期，那么其引用的对象可能在某个时刻被销毁了，如何得知呢？std::weak_ptr 提供了一个 expired() 方法来做这一项检测，返回 true，说明其引用的资源已经不存在了；返回 false，说明该资源仍然存在，这个时候可以使用 std::weak_ptr 的 lock() 方法得到一个 std::shared_ptr 对象然后继续操作资源，以下代码演示了该用法：  
```C++
//tmpConn_ 是一个 std::weak_ptr<MySQLConnection> 对象
//tmpConn_引用的MySQLConnection已经销毁，直接返回
if (tmpConn_.expired())
    return;

std::shared_ptr<MySQLConnection> conn = tmpConn_.lock();
if (conn)
{
    //对conn进行操作，省略...
}
```
有读者可能对上述代码产生疑问，既然使用了 std::weak_ptr 的 expired() 方法判断了对象是否存在，为什么不直接使用 std::weak_ptr 对象对引用资源进行操作呢？实际上这是行不通的，std::weak_ptr 类没有重写 operator-> 和 operator* 方法，因此不能像 std::shared_ptr 或 std::unique_ptr 一样直接操作对象，同时 std::weak_ptr 类也没有重写 operator! 操作，因此也不能通过 std::weak_ptr 对象直接判断其引用的资源是否存在：  
```C++
class A
{
public:
    void doSomething(){}
};
int main()
{    
    std::shared_ptr<A> sp1(new A());
    std::weak_ptr<A> sp2(sp1);
    //正确代码
    if (sp1)
    {
        //正确代码
        sp1->doSomething();
        (*sp1).doSomething();
    }

    //正确代码
    if (!sp1)
    {
    }

    //错误代码，无法编译通过
    //if (sp2)
    //{
    //    //错误代码，无法编译通过
    //    sp2->doSomething();
    //    (*sp2).doSomething();
    //}

    //错误代码，无法编译通过
    //if (!sp2)
    //{
    //}

    return 0;
}
```
之所以 std::weak_ptr 不增加引用资源的引用计数不管理资源的生命周期，是因为，即使它实现了以上说的几个方法，调用它们也是不安全的，因为在调用期间，引用的资源可能恰好被销毁了，这会造成棘手的错误和麻烦。因此，std::weak_ptr 的正确使用场景是那些资源如果可能就使用，如果不可使用则不用的场景，它不参与资源的生命周期管理。例如，网络分层结构中，Session 对象（会话对象）利用 Connection 对象（连接对象）提供的服务工作，但是 Session 对象不管理 Connection 对象的生命周期，Session 管理 Connection 的生命周期是不合理的，因为网络底层出错会导致 Connection 对象被销毁，此时 Session 对象如果强行持有 Connection 对象与事实矛盾。std::weak_ptr 的应用场景，经典的例子是订阅者模式或者观察者模式中。这里以订阅者为例来说明，消息发布器只有在某个订阅者存在的情况下才会向其发布消息，而不能管理订阅者的生命周期。  
```C++
class Subject;
class Observer
{
public:
    virtual ~Observer() {}
    virtual void Update(Subject* theChangedSubject) = 0;
protected:
    Observer() {}
private:
};

class Subject
{
public:
    virtual ~Subject() {}
    virtual void Notify()
    {
        for (auto& obiter : observers_)
        {
            if (!obiter.expired())
            {
                //TODO：给订阅者发送消息
                std::shared_ptr<Observer> iter = obiter.lock();
                if (iter)
                {
                    iter->Update(this);
                }
            }
        }
    }
protected:
    Subject() {}
private:
    std::vector<std::weak_ptr<Observer>> observers_;
};
```

前面有个例子，说明智能指针的循环引用带来的内存泄漏:  
```C++
#include <iostream>
#include <memory>
class B;
class A
{
public:
    shared_ptr<B> pb;
    A(){    cout << "constructor A" << endl;    }
    ~A(){    cout << "destructor A" << endl;    }
};
 
class B
{
public:
    shared_ptr<A> pa;
    B(){    cout << "constructor B" << endl;    }
    ~B(){    cout << "destructor B" << endl;    }
};
 
int main(int argc, char** argv)
{
    {
        shared_ptr<A> sa(new A());
        shared_ptr<B> sb(new B());
        if(sa && sb)
        {
            sa->pb=sb;
            sb->pa=sa;
        }
    }
    return 0;
}
/*
输出：
constructor A
constructor B
*/
```
现在就说明通过std::weak_ptr如何解决这个问题:  
```C++
#include <iostream>
#include <memory>
class B;
class A
{
public:
    std::weak_ptr<B> pb;
    A(){    cout << "constructor A" << endl;    }
    ~A(){    cout << "destructor A" << endl;    }
};
 
class B
{
public:
    std::shared_ptr<A> pa;
    B(){    cout << "constructor B" << endl;    }
    ~B(){    cout << "destructor B" << endl;    }
};
 
int main(int argc, char** argv)
{
    {
        shared_ptr<A> sa(new A());
        shared_ptr<B> sb(new B());
        if(sa && sb)
        {
            sa->pb=sb;
            sb->pa=sa;
        }
    }
    return 0;
}
/*
输出：
constructor A
constructor B
destructor B
destructor A
*/
```
可以看到这种循环引用被打破了。解决了内存泄漏问题。  

同时，也不要把一个原生指针给多个shared_ptr或者unique_ptr管理。我们知道，在使用原生指针对智能指针初始化的时候，智能指针对象都视原生指针为自己管理的资源。换句话意思就说：初始化多个智能指针之后，这些智能指针都担负起释放内存的作用。那么就会导致该原生指针会被释放多次！！举例如下:  
```C++
class A
{
public:
    A(){    cout << "constructor A" << endl;    }
    ~A(){    cout << "destructor A" << endl;    }
};
 
int main(int argc, char** argv)
{
    {
        A * pa = new A();
        shared_ptr<A> sa1(pa);
        shared_ptr<A> sa2(sa1);
        cout << sa1.use_count() << ", " << sa2.use_count() << endl;
    }
    cout << "===================" << endl;
    {
        A * pa = new A();
        shared_ptr<A> sa1(pa);
        shared_ptr<A> sa2(pa);
        cout << sa1.use_count() << ", " << sa2.use_count() << endl;
    }
    return 0;
}
/*
输出：
constructor A
2, 2
destructor A
===================
constructor A
1, 1
destructor A
destructor A
*/
```
C++ 新标准提倡的理念之一是不应该再手动调用 delete 或者 free 函数去释放内存了，而应该把它们交给新标准提供的各种智能指针对象。C++ 新标准中的各种智能指针是如此的实用与强大，在现代 C++ 项目开发中，读者应该尽量去使用它们。智能指针虽然好用，但稍不注意，也可能存在许多难以发现的 bug，常见的有：  

1. 尽量不要使用 get()：  
智能指针设计者之处提供get()接口是为了使得智能指针也能够适配原生指针使用的相关函数。这个设计可以说是个好的设计，也可以说是个失败的设计。因为根据封装的封闭原则，我们将原生指针交付给智能指针管理，我们就不应该也不能得到原生指针了；因为原生指针唯一的管理者就应该是智能指针。而不是客户逻辑区的其他什么代码。所以我们在使用get()的时候要额外小心，禁止使用get()返回的原生指针再去初始化其他智能指针或者释放[delete get返回的原生指针是绝对不允许的]。(**只能够被使用，不能够被管理**)。而下面这段代码就违反了这个规定：   
```C++
int main()
{
    shared_ptr<int> sp(new int(4));
    shared_ptr<int> pp(sp.get());
    return 0;
}
```
2. 使用delete无法完全清理的，一定要定义删除器:  
以下代码试图将malloc产生的动态内存交给shared_ptr管理；显然是有问题的，delete 和 malloc 牛头不对马嘴！！！所以我们需要自定义删除器[](int* p){ free(p); }传递给shared_ptr。  
```C++
int main()
{
    int* pi = (int*)malloc(4 * sizeof(int));
    shared_ptr<int> sp(pi);
    return 0;
}
```

3. 一旦一个对象使用智能指针管理后，就不该再使用原始裸指针去操作  


4. 认真考虑，避免操作某个引用资源已经释放的智能指针  
例如下面的代码：  
```C++
class T
{
public:
    void doSomething(){ std::cout << "T do something..." << m_i << std::endl;   }
private:
    int m_i;
};

int main()
{    
    std::shared_ptr<T> sp1(new T());
    const auto& sp2 = sp1; 
    sp1.reset();
    //由于sp2已经不再持有对象的引用，程序会在这里出现意外的行为(coredump)
    sp2->doSomething();
    return 0;
}
```
上述代码中，sp2 是 sp1 的引用，sp1 被置空后，sp2 也一同为空。这时候调用 sp2->doSomething()，sp2->（即 operator->）在内部会调用 get() 方法获取原始指针对象，这时会得到一个空指针（地址为 0），继续调用 doSomething() 导致程序崩溃。智能指针的引用一定要谨慎使用，反复推敲。    

总而言之，只能指针是管理内存的强大工具，使用得当可以避免内存泄漏，使用不当依然会内存泄漏，因此在使用智能指针时，要遵循良好的规范，仔细推敲代码的正确性，以避免内存泄漏。虽然如此，只要是程序就会存在内存泄漏，那么我们如何发现程序中的内存泄漏呢？  

## 6. 内存泄漏
在计算机科学中，内存泄漏指由于疏忽或错误造成程序未能释放已经不再使用的内存。内存泄漏并非指内存在物理上的消失，而是应用程序分配某段内存后，由于设计错误，导致在释放该段内存之前就失去了对该段内存的控制，从而造成了内存的浪费。在C++中出现内存泄露的主要原因就是程序猿在申请了内存后(malloc(), new)，没有及时释放没用的内存空间，甚至消灭了指针导致该区域内存空间根本无法释放。
内存泄漏可能会导致严重的后果：  
 - 程序运行后，随着时间占用了更多的内存，最后无内存可用而崩溃；  
 - 程序消耗了大量的内存，导致其他程序无法正常使用；  
 - 程序消耗了大量内存，导致消费者选用了别人的程序而不是你的.  

内存泄漏很容易被发现，那么发生了内存泄漏该如何定位到泄露点呢？  

内存泄漏的分析主要是三种思路:  
 - 静态分析：主要包括review自己的代码和静态代码分析工具（代码静态扫描和分析的工具比较多，比如 splint, PC-LINT, BEAM 等）。  
 - 动态运行检测：主要包括使用动态分析工具和增加最终代码（实时检测工具主要有 valgrind, Rational purify 等）。  
 - 内嵌代码：也有两种思路，氛围重载内存分配和释放函数 new 和 delete,然后编写程序定期统计内存的分配和释放，从中找出可能的内存泄漏。或者调用系统函数定期监视程序堆的大小，关键要确定堆的增长是泄漏而不是合理的内存使用。或者修改自己的应用程序，通过记录申请与释放的对象数目是否一致来判断。在类中追加一个静态变量 static int count;在构造函数中执行count++;在析构函数中执行count--;，通过在程序结束前将所有类析构，之后输出静态变量，看count的值是否为0，如果为0,则问题并非出现在该处，如果不为0，则是该类型对象没有完全释放。  

总而言之，我们在写程序时，应该使用现代C++提供的智能指针来避免内存泄漏，而不是发生内存泄漏之后去排查。  

