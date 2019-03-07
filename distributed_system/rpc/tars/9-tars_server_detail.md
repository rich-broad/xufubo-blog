---
title: tars 服务端部分详解
---

# 1. 概述
前面在[tars术语](./6-tars_terminology.md)部分约定了tars对象，明确了tars对象的一个实例就是tars服务，同时说明了服务端和客户端的概念，明确指出tars服务（对象实例）包含了客户端和服务器两个部分。本节学习tars的服务端部分的设计和实现。  

# 2. tars服务结构
tars服务本质上就是一个cpp程序。通过[tars快速开始](./2-tars_quick_start.md)可以看到，我们需要定义一个XXXServer类（tars的脚本也可以自动生成），该类继承自Application，我们需要定义XXXServer g_app。然后在main函数中如下调用就可以了：  
```C++
g_app.main(argc, argv);
g_app.waitForShutdown();
```
这时tars就可以正常服务了。  

研究tars服务之前，先说明几个tars服务在实现时重要的类。  
1. Application
一个tars服务就是一个Application，前面说过，一个tars服务同时会有服务端（被调用）和客户端（为了完成某件事而调用别的服务）两种角色。Application也包含了这两个部分。Application会读取配置文件，初始化服务端（包括监听端口、队列长度、业务线程数、网络线程数、超时时间等等），同时也会初始化客户端部分（包括要调用的对象的代理、异步回调线程、超时时间、队列长度等）。  

2. TC_EpollServer
Application包含TC_EpollServer的一个实例，该类是真正的服务端部分。包含各种服务端资源。tars服务端也采用了网络模块与业务模块分离的设计思路。该类主要是这两个模块的实现。其中用到了队列（也是将网络模块与业务模块分开的必备数据结构）。接下来会介绍这两个模块相关的类。   

3. NetThread--网络模块
代表tars服务端部分的网络模块。NetThread也是一个线程。用于处理网络事件。核心包括一个epoll，通过epoll的事件，来监听socket连接、数据发送等事件。同时还包含了连接的管理，总之任何与服务端网络相关的时间（连接管理、数据收发等）都与NetThread有关。TC_EpollServer持有的NetThread的个数是可配的。  

4. HandleGroup、Handle与ServantHandle--业务模块
代表着业务模块。Handle是一个用于执行RPC请求的一个线程。多个Handle组成的HandleGroup就是RPC服务的一组业务线程。业务线程用于执行用户实现的业务逻辑，并将处理结果放到发送缓存中等待网络模块处理。业务线数也是可配的。在配置文件中，利用/tars/application/server/xxxAdapter 下的threads配置一个HandleGroup中的Handle（业务线程）的个数。其中，ServantHandle继承自Handle，实际上tars服务不会直接创建Handle，而是创建的ServantHandle，ServantHandle包含了Servant名字与Servant类实例的映射。HandleGroup就是多个ServantHandle的集合。通过这个就可以调用到业务代码。后边会详细分析。  

5. BindAdapter
代表一个服务实体。在配置文件中的/tars/application/server下面的xxxAdapter就是对BindAdapter的配置，一个BindAdapter代表一个服务实体，看其配置就知道BindAdapter的作用是什么了，其代表一个RPC服务对外的监听套接字，还声明了连接的最大数量，接收队列的大小，业务线程数，RPC服务名，所使用的协议等。几个Servant就有几个xxxAdapter。这也说明一个tars服务可以监听多个端口(因为一个tars服务包含多个xxxAdapter).    

BindAdapter本身可以认为是一个服务的实例，能建立真实存在的监听socket并对外服务，与网络模块NetThread以及业务模块HandleGroup都有关联，例如，多个NetThread的第一个线程负责对BindAdapter的listen socket进行监听，有客户连接到BindAdapter的listen socket就随机在多个NetThread中选取一个，将连接放进被选中的NetThread的ConnectionList中。BindAdapter则通常会与一组HandleGroup进行关联，该HandleGroup里面的业务线程就执行BindAdapter对应的服务。可见，BindAdapter与网络模块以及业务模块都有所关联。  

了解了服务端重要的类之后，接下来看看服务端是如何初始化的。

# 3. tars服务端初始化
前面我们知道，使tars服务正常提供服务需要如下两步：  
```C++
g_app.main(argc, argv);
g_app.waitForShutdown();
```
我们就从Application的main函数开始研究。  

在[tars术语](./6-tars_terminology.md)中，我们约定了tars服务的两部分：客户端部分、服务端部分。本文研究服务端部分。  

由上分析，我们知道服务端主要包含网络模块和业务模块。这里我们也将初始化分为网络模块初始化和业务模块初始化。初始化的代码在Application的成员函数main()以及waitForQuit()中。初始化包含了基本的将一个进程初始化为daemon的通用代码，屏蔽pipe信号，读取配置文件等，这些比较简单，通过代码可以很明显的看出。这里主要研究服务端如何通过epoll、网络线程、队列等构建网络模块。同时又是如何初始化业务模块，如何调用到业务开发人员编写的业务代码的。  


初始化网络模块和业务模块之前，需要先创建者两个模块的载体：TC_EpollServer。TC_EpollServer的初始化主要在：  
```C++
void Application::main(int argc, char *argv[])
{
    ......
    // 初始化Server部分
    initializeServer();
    .....
}
```
在**initializeServer**中会设置ServerConfig（服务基本信息）结构中各个静态成员的值，以便需要时取用。例如：应用名称、服务名称、应用程序路径、应用程序数据路、本机IP、log路径、log日志级别、log大小、log个数、本机node地址、日志中心地址、配置中心地址等等等。具体可以看源代码。也可以看看这些值分别取自哪里。  

接着创建TC_EpollServer对象：  
```C++
_epollServer = new TC_EpollServer(iNetThreadNum);
```
iNetThreadNum为网络模块NetThread的个数，从代码看到最小为1，最大为15。从代码可以看到：TC_EpollServer被创建出来，而且网络线程NetThread也被建立出来了：  
```C++
TC_EpollServer::TC_EpollServer(unsigned int iNetThreadNum)
: _pReportRspQueue(NULL), _netThreadNum(iNetThreadNum), _bTerminate(false), _handleStarted(false), _pLocalLogger(NULL)
{
    if(_netThreadNum < 1)
    {
        _netThreadNum = 1;
    }

    if(_netThreadNum > 15)
    {
        _netThreadNum = 15;
    }

    for (size_t i = 0; i < _netThreadNum; ++i)
    {
        TC_EpollServer::NetThread* netThreads = new TC_EpollServer::NetThread(this);
        _netThreads.push_back(netThreads);
    }
}
```
创建TC_EpollServer和NetThread之后，其实还会创建一个用于监听tars管理端命令的AdminAdapter来实现tars管理端对业务服务的管理。**这在每个服务中都是通用的**。可以看到，用于接收各种管理端命令的Servant是AdminServant类的实例：  
```C++
ServantHelperManager::getInstance()->addServant<AdminServant>("AdminObj");
ServantHelperManager::getInstance()->setAdapterServant("AdminAdapter", "AdminObj");
```
这个不是我这里研究的重点，后边会单独研究框架运营管理端的架构和代码。TC_EpollServer被创建之后接着就初始化网络模块和业务模块了。  


## 3.1 网络模块初始化
网络模块的初始化无非就是创建RPC服务的监听socket，对于linux下，通常会通过epoll来监听网络事件，然后等待连接到来或者收发数据。  

对应的代码如下：  
```C++
void Application::main(int argc, char *argv[])
{
    ......
    vector<TC_EpollServer::BindAdapterPtr> adapters;
    //绑定对象和端口
    bindAdapter(adapters);
    ......
    _epollServer->createEpoll();
    .....
}
```
