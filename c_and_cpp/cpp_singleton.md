# 概述
本文详细研究C++多线程环境下单利模式的实现。  

单例模式，可以说设计模式中最常应用的一种模式了。但是如果没有学过设计模式的人，可能不会想到要去应用单例模式，面对单例模式适用的情况，可能会优先考虑使用全局或者静态变量的方式，这样比较简单，也是没学过设计模式的人所能想到的最简单的方式了。

一般情况下，我们建立的一些类是属于工具性质的，基本不用存储太多的跟自身有关的数据，在这种情况下，每次都去new一个对象，即增加了开销，也使得代码更加臃肿。其实，我们只需要一个实例对象就可以。如果采用全局或者静态变量的方式，会影响封装性，特别对于大型项目，很容易照成名字冲突。基于这些原因，大佬们设计出了单例模式。  

在实现的时候，我们将类的构造函数设置为private，这样，外部就不会去构造该类的对象了，只能通过我们提供的函数来获取。  

在单线程环境下，我们会这么实现：  
```C++
class CSingleton  
{  
private:  
    CSingleton();
    CSingleton(const CSingleton &);
    CSingleton& operator = (const CSingleton &);
  
public:  
    static CSingleton *GetInstance();
    static CSingleton *_instance;
};  
```
最为核心的是GetInstance()函数，在单线程下，
实现是：
```C++
CSingleton* CSingleton::_instance = NULL;  
  
CSingleton* CSingleton::GetInstance()  
{  
    if(_instance == NULL)  
    {
        _instance = new CSingleton();  
    }  
    return _instance;  
} 
```

在多线程环境下，上述函数Getinstance在多个线程执行时会存在竞争，例如两个线程都判断_instance == NULL 为true，都会new一个新对象。这明显会有问题。因此需要加锁保护。对于多线程而言，Getinstance函数通常如下实现：
```C++
CSingleton* CSingleton::GetInstance()  
{
    lock();
    if(_instance == NULL)  
    {
        _instance = new CSingleton();  
    } 
    unlock(); 
    return _instance; 
} 
```
这个函数看起来解决了问题，但是每当线程GetInstance()时，都要先lock，其实我们只在一开始创建一个实例，因此后续每次_instance == NULL判断结果都是false，从而返回_instance，这其实会导致很多无效的lock，unlock(对于linux下的mutex来说，这两个动作还是很昂贵的，每次都lock，unlock都快失去单例模式的优势了)，基于这个问题，引出了double check机制。如下：  
```C++
CSingleton* CSingleton::GetInstance()  
{
    if(_instance == NULL)
    {
        lock();
        if(_instance == NULL)  
        {
            _instance = new CSingleton();  
        } 
        unlock(); 
    }
    
    return _instance; 
} 
```
这时，只是在首次调用的时候调用了lock，unlock后续由于每次判断_instance == NULL都为false，因此直接返回_instance。  
但是这样完美了吗？这段代码看起来没有问题，当函数返回时，_instance总是指向一个有效对象。为lock和unlock又解决了多线程竞争的问题。  
但是实际上这个代码也有问题。问题的来源是CPU的乱序执行。C++里，new包含了两个步骤：
 - 1. 分配内存  
 - 2. 调用构造函数。  

这时， _instance = new CSingleton()就包含了三步：  
 - 1. 分配内存  
 - 2. 调用构造函数  
 - 3. 将内存地址赋值给 _instance  

在这三步中，2和3步完全可以颠倒。也就会出现如下情况： _instance不为NULL，但是没有调用构造函数。这时如果另外一个线程直接调用GetInstance，就会直接得到一个没有构造完全的对象。这时候程序会如何执行就不得而知了。  
可见，CPU的乱序执行对于多线程保障有了很大挑战。因此，要保证多线程安全，在某些时候必须阻止CPU换序。通常会调用CPU的一条指令(比较遗憾，不同的平台，通常不同)，该指令通常被称为barrier。barrier会阻止CPU将barrier之前和之后的指令交换。由此得到线程安全的实现。:  
```C++
CSingleton* CSingleton::GetInstance()  
{
    if(_instance == NULL)
    {
        lock();
        if(_instance == NULL)  
        {
            CSingleton * temp = new CSingleton();
            barrier();   //不同的平台不一样。
            _instance = temp;
        } 
        unlock(); 
    }
    
    return _instance; 
} 
```

如此就实现了多线程环境下的单例模式。  

