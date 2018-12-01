# 概述
本文主要学习protobuf的service.h文件的内容，如果我们使用protobuf作为数据交换格式，并使用其生成的接口来实现RPC框架的话，必须要详细了解该文件的细节，在此详细学习。  
在该文件的开头会有如下说明：大意就是2.3.0之后不应该再使用这个功能生成抽象的，未绑定任何RPC实现的接口，而应该提供代码生成器插件，它们生成特定于RPC实现的代码，其目的是使生成的代码更加适合具体的实现，而且可以避免不必要的间接层。  
不过很多市场上的RPC实现还是基于google的这个功能来实现自己的rpc，而不会自己去通过protobuf的插件，生成自己的代码，因此我们学习它还是很有意义的，而且通过对它的学习，也能加深对RPC实现的理解，以及对代码结构，代码解耦等的理解。  
当你使用protobuf的编译器编译服务定义时，它会生成两个类：  
 - 1、service：代表一个服务，需要RPC的服务端去实现；  
 - 2、stub：一个用于调用服务端方法的包装器。  
例如：  
```
service MyService {
  rpc Foo(MyRequest) returns(MyResponse);
}
```
将生成抽象类MyService和MyService_Stub(就是MyService::Stub)。可以按如下方式实现MyService：  
```C++
class MyServiceImpl : public MyService {
 public:
  MyServiceImpl() {}
  ~MyServiceImpl() {}

  // implements MyService ---------------------------------------

  void Foo(google::protobuf::RpcController* controller,
           const MyRequest* request,
           MyResponse* response,
           Closure* done) {
    // ... read request and fill in response ...
    done->Run();
  }
};
```
然后，将使用RPC实现注册MyServiceImpl的实例（如何做到这一点取决于具体实现，**后边我们会研究brpc的实现**）。  
为了调用远程的MyServiceImpl，你必须通过RpcChannel连接到它(可见RpcChannel在实现RPC系统时是非常重要的，它负责很重要的网络通信这一块)。如何构建RpcChannel也取决于具体的RPC实现。这里我们用一个假设的“MyRpcChannel”作为例子：  
```C++
MyRpcChannel channel("rpc:hostname:1234/myservice");
MyRpcController controller;
MyServiceImpl::Stub stub(&channel);
FooRequest request;
FooResponse response;

// ... fill in request ...

stub.Foo(&controller, &request, &response, NewCallback(HandleResponse));
```
关于线程安全：  
不同的RPC实现会有不同的保护，这个与具体的实现又关了，与protobuf编译器生成的代码无关。  
在service.h中，有如下三个类：  
```C++
class Service;  // 用于基于protobuf的RPC服务的抽象基础接口。
class RpcController;  // RpcController控制单个方法调用。
class RpcChannel;   // RPC通道的抽象接口。
```

## class Service
类Service的抽象函数如下：  
```C++
class LIBPROTOBUF_EXPORT Service {
 public:
  inline Service() {}
  virtual ~Service();
  enum ChannelOwnership {
    STUB_OWNS_CHANNEL,
    STUB_DOESNT_OWN_CHANNEL
  };
  virtual const ServiceDescriptor* GetDescriptor() = 0;
  virtual void CallMethod(const MethodDescriptor* method,
                          RpcController* controller,
                          const Message* request,
                          Message* response,
                          Closure* done) = 0;

  virtual const Message& GetRequestPrototype(
    const MethodDescriptor* method) const = 0;
  virtual const Message& GetResponsePrototype(
    const MethodDescriptor* method) const = 0;

 private:
  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(Service);
};
```
这里着重描述一下Service::CallMethod：调用由MethodDescriptor指定的服务的方法。这通常作为一个简单的switch（）来实现。  
例如，对于上边我们的RPC定义，CallMethod的实现如下：  
```C++
void MyService::CallMethod(const ::google::protobuf::MethodDescriptor* method,
                             ::google::protobuf::RpcController* controller,
                             const ::google::protobuf::Message* request,
                             ::google::protobuf::Message* response,
                             ::google::protobuf::Closure* done) {
  GOOGLE_DCHECK_EQ(method->service(), MyService_descriptor_);
  switch(method->index()) {
    case 0:
      Foo(controller,
             ::google::protobuf::down_cast<const ::example::MyRequest*>(request),
             ::google::protobuf::down_cast< ::example::MyResponse*>(response),
             done);
      break;
    default:
      GOOGLE_LOG(FATAL) << "Bad method index; this should never happen.";
      break;
  }
}
```
如此，将会调用到我们实现的Foo函数。  
函数Foo完成之前，done->Run()需要被调用(**done由我们具体的RPC框架创建，传递给CallMethod用于服务回调，该函数的主要作用就是检查response正确性，序列化，打包，发送响应等逻辑，Run函数的实现取决于具体的RPC实现。为什么框架不自己调用done->Run()？这是为了允许用户把done保存下来，在服务回调之后的某事件发生时再调用，即实现异步Service**)，如果RPC成功，则response包含服务器的回包，如果RPC失败，则response的内容未定义。可以通过RpcController以确定是否发生了错误，并可获取有关该错误的更多信息。  

## class RpcController
RpcController的主要目的是提供一种特定于RPC实现的设置并找出RPC级错误的方法。protobuf提供的RpcController包含了一个RPC系统在实现时通常需要的最基本的方法，你当然可以实现自己更高级的特性。定义如下：  
```C++
class LIBPROTOBUF_EXPORT RpcController {
 public:
  inline RpcController() {}
  virtual ~RpcController();
  //---------------Client-side methods-------------------
  // 这些调用只能从客户端进行。他们的结果在服务器端是未定义的.

  // 将RpcController重置为初始状态，以便可以在新的调用中重用它。RPC正在进行时不能被调用。
  virtual void Reset() = 0;
  
  // 调用结束后，如果调用失败，则返回true。失败的原因取决于RPC实现，在调用完成之前，不能调用Failed，
  // 如果Failed返回true，则response消息的内容未定义。
  virtual bool Failed() const = 0;

  // 如果Failed为true，则返回人类可读的错误描述。
  virtual string ErrorText() const = 0;

  // 建议RPC系统调用者希望RPC调用被取消。RPC系统可能会立即取消它，可能会等待一段时间然后取消它，甚至可能根本不会取消该调用。  
  // 如果该调用被取消，则“done”回调将仍然被调用，并且RpcController将指示此时调用失败。
  virtual void StartCancel() = 0;

  //---------------Server-side methods-------------------
  // 这些调用只能从服务器端进行。他们的结果在客户端端是未定义的.

  // 使得Failed在客户端返回true。
  virtual void SetFailed(const string& reason) = 0;

  // 如果为true，则表示客户端取消了RPC，因此服务器可能放弃回复它。服务器仍然应该调用最终的“done”回调。
  virtual bool IsCanceled() const = 0;

  // 要求在取消RPC时调用给定的回调。回调将始终被调用一次。如果RPC完成而未被取消，则完成后将调用回调。
  // 如果在调用NotifyOnCancel时RPC已被取消，则立即调用回调。NotifyOnCancel（）每个请求只能调用一次。
  virtual void NotifyOnCancel(Closure* callback) = 0;

 private:
  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(RpcController);
};
```
## class RpcChannel
RpcChannel代表了RPC的通信。通常情况下，你不应该直接调用RpcChannel，而是构造一个包装它的stub Service。例如：  
```C++
RpcChannel* channel = new MyRpcChannel("remotehost.example.com:1234");
MyService* service = new MyService::Stub(channel);
service->MyMethod(request, &response, callback);
```
类定义如下：  
```C++
class LIBPROTOBUF_EXPORT RpcChannel {
 public:
  inline RpcChannel() {}
  virtual ~RpcChannel();
  
  // 调用远程服务的方法。此过程的签名看起来与Service::CallMethod()相同，其实并不严格要求。
  // 只要它们的描述符是method-> input_type（）和method-> output_type（），请求和响应对象就不需要具有任何特定的类。
  virtual void CallMethod(const MethodDescriptor* method,
                          RpcController* controller,
                          const Message* request,
                          Message* response,
                          Closure* done) = 0;

 private:
  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(RpcChannel);
};
```
如上我们较为详细的学习了protobuf生成的几个重要的类，了解了他们的作用，可以看到，我们如果自己在实现我们的RPC系统时需要实现这几个类。实现具体的通信层。后边将详细的学习brpc的实现。  
**小结一下：在实现我们自己的RPC系统时，服务端的done由框架注册给业务服务，done->Run由业务服务执行回调，客户端的done由业务客户端创建，注册给框架，done->Run由框架执行回调。后边详细学习brpc的实现**  

