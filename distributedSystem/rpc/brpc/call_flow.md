# 概述
本小节通过一个简单的例子来分析服务端调用流程

设置断点：
```shell
[xufubo@localhost echo_c++]$ ps aux | grep echo
xufubo    21297  0.0  1.2 905996 24048 pts/1    tl   Aug19   1:00 ./echo_server
[xufubo@localhost echo_c++]$ gdb -p 21297
(gdb) b example::EchoServiceImpl::Echo
Breakpoint 1 at 0x41d730: file server.cpp, line 37.
(gdb) c
Continuing.
```

```shell
在另一个终端执行客户端：
[xufubo@localhost echo_c++]$ ./echo_client
```

回到设置断点的终端我们看到：
```shell
[Switching to Thread 0x7fa130d26700 (LWP 21301)]

Breakpoint 1, example::EchoServiceImpl::Echo (this=0x7ffe94cf15c0, cntl_base=0x7fa11c025170, request=0x7fa11c0250e0, response=0x7fa11c0253f0, done=0x7fa11c02e6d0) at server.cpp:37
37	    virtual void Echo(google::protobuf::RpcController* cntl_base,
(gdb) bt
#0  example::EchoServiceImpl::Echo (this=0x7ffe94cf15c0, cntl_base=0x7fa11c025170, request=0x7fa11c0250e0, response=0x7fa11c0253f0, done=0x7fa11c02e6d0) at server.cpp:37
#1  0x000000000041bfb5 in example::EchoService::CallMethod (this=<optimized out>, method=<optimized out>, controller=<optimized out>, request=<optimized out>,  response=<optimized out>, done=<optimized out>) at echo.pb.cc:610
#2  0x00000000004bc41d in brpc::policy::ProcessRpcRequest (msg_base=<optimized out>) at src/brpc/policy/baidu_rpc_protocol.cpp:499
#3  0x00000000005eaf2a in brpc::ProcessInputMessage (void_arg=void_arg@entry=0x7fa11c01d090) at src/brpc/input_messenger.cpp:133
#4  0x00000000005ebf74 in operator() (this=<optimized out>, last_msg=0x7fa11c01d090) at src/brpc/input_messenger.cpp:139
#5  brpc::InputMessenger::OnNewMessages (m=0x7fa10801ac00) at /usr/include/c++/4.8.2/bits/unique_ptr.h:184
#6  0x00000000005d51cd in brpc::Socket::ProcessEvent (arg=0x7fa10801ac00) at src/brpc/socket.cpp:1061
#7  0x0000000000542b7a in bthread::TaskGroup::task_runner (skip_remained=<optimized out>) at src/bthread/task_group.cpp:297
#8  0x0000000000545e71 in bthread_make_fcontext ()
#9  0x0000000000000000 in ?? ()
```

由此可以看到服务端的调用流程。