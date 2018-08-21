# 引言
在学习protobuf的时候，通过google的官方文档，我们看到，protobuf的编译器生成的代码不会绑定任何特定的RPC系统，相反的，只是生成了抽象的接口，不同的RPC系统差异可能很大。不过具体到某一个RPC系统，都必须继承google抽象出来的基类并实现必要的方法。当然了，RPC系统的网络通信（自己的RPC通信协议、解包等）要自己去实现。从文档[service.md](../../protobuf/service.md)我们看到，谷歌抽象了如下三个类：
```C++
class Service;  // 用于基于protobuf的RPC服务的抽象基础接口。
class RpcController;  // RpcController控制单个方法调用。
class RpcChannel;   // RPC通道的抽象接口。
```
每一个具体的RPC实现都必须继承这三个类并实现其中的方法。brpc也有对应的实现，分别是：
```C++
class Controller : public google::protobuf::RpcController;

// Base of all brpc channels.
class ChannelBase : public google::protobuf::RpcChannel/*non-copyable*/,
                    public Describable;
```
对于Service，则是由每一个业务服务自己去实现，例如：
```protobuf
syntax="proto2";
package example;

option cc_generic_services = true;

message EchoRequest {
      required string message = 1;
};

message EchoResponse {
      required string message = 1;
};

service EchoService {
      rpc Echo(EchoRequest) returns (EchoResponse);
};
```
生成的代码：
```C++
class EchoService : public ::google::protobuf::Service {
 protected:
  // This class should be treated as an abstract interface.
  inline EchoService() {};
 public:
  virtual ~EchoService();

  typedef EchoService_Stub Stub;

  static const ::google::protobuf::ServiceDescriptor* descriptor();

  virtual void Echo(::google::protobuf::RpcController* controller,
                       const ::example::EchoRequest* request,
                       ::example::EchoResponse* response,
                       ::google::protobuf::Closure* done);

  // implements Service ----------------------------------------------

  const ::google::protobuf::ServiceDescriptor* GetDescriptor();
  void CallMethod(const ::google::protobuf::MethodDescriptor* method,
                  ::google::protobuf::RpcController* controller,
                  const ::google::protobuf::Message* request,
                  ::google::protobuf::Message* response,
                  ::google::protobuf::Closure* done);
  const ::google::protobuf::Message& GetRequestPrototype(
    const ::google::protobuf::MethodDescriptor* method) const;
  const ::google::protobuf::Message& GetResponsePrototype(
    const ::google::protobuf::MethodDescriptor* method) const;

 private:
  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(EchoService);
};

class EchoService_Stub : public EchoService;
......
......
```
我们的实现：
```C++
class EchoServiceImpl : public EchoService {
public:
    EchoServiceImpl() {};
    virtual ~EchoServiceImpl() {};
    virtual void Echo(google::protobuf::RpcController* cntl_base,
                      const EchoRequest* request,
                      EchoResponse* response,
                      google::protobuf::Closure* done) {
        // This object helps you to call done->Run() in RAII style. If you need
        // to process the request asynchronously, pass done_guard.release().
        brpc::ClosureGuard done_guard(done);

        brpc::Controller* cntl =
            static_cast<brpc::Controller*>(cntl_base);

        // The purpose of following logs is to help you to understand
        // how clients interact with servers more intuitively. You should 
        // remove these logs in performance-sensitive servers.
        LOG(INFO) << "Received request[log_id=" << cntl->log_id() 
                  << "] from " << cntl->remote_side() 
                  << " to " << cntl->local_side()
                  << ": " << request->message()
                  << " (attached=" << cntl->request_attachment() << ")";

        // Fill response.
        response->set_message(request->message());

        // You can compress the response by setting Controller, but be aware
        // that compression may be costly, evaluate before turning on.
        // cntl->set_response_compress_type(brpc::COMPRESS_TYPE_GZIP);

        if (FLAGS_echo_attachment) {
            // Set attachment which is wired to network directly instead of
            // being serialized into protobuf messages.
            cntl->response_attachment().append(cntl->request_attachment());
        }
    }
};
```