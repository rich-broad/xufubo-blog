# 1. 概述
本文主要学习bfs中客户端与bfs的交互。具体包括如下命令的实现[bfs_client.cc]()：  
```sh
 - ls <path> : list the directory  
 - cat <path>... : cat the file
 - mkdir <path>... : make directory  
 - mv <srcpath> <destpath> : rename directory or file  
 - touchz <path>... : create a new file  
 - rm <path>... : remove a file  
 - get <bfsfile> <localfile> : copy file to local  
 - put <localfile> <bfsfile> : copy file from local to bfs  
 - rmdir <path>... : remove empty directory  
 - rmr <path>... : remove directory recursively  
 - du <path>... : count disk usage for path  
 - stat : list current stat of the file system  
 - ln <src> <dst> : create symlink  
 - chmod <mode> <path> : change file mode bits  
``` 

bfs_client.cc实现的为与bfs集群交互的命令行客户端，其main函数如下：  
```C++
int main(int argc, char* argv[]) {
    FLAGS_flagfile = "./bfs.flag";
    int gflags_argc = 1;
    ::google::ParseCommandLineFlags(&gflags_argc, &argv, false);

    if (argc < 2) {
        print_usage();
        return 0;
    }

    baidu::bfs::FS* fs;
    std::string ns_address = FLAGS_nameserver_nodes;
    //打开失败直接返回
    if (!baidu::bfs::FS::OpenFileSystem(ns_address.c_str(), &fs, baidu::bfs::FSOptions())) {
        fprintf(stderr, "Open filesytem %s fail\n", ns_address.c_str());
        return 1;
    }

    int ret = 1;
    if (strcmp(argv[1], "touchz") == 0) {
        ret = BfsTouchz(fs, argc - 2, argv + 2);
    } else if (strcmp(argv[1], "rm") == 0) {
        ret = BfsRm(fs, argc - 2, argv + 2);
    } else if (strcmp(argv[1], "mkdir") == 0) {
        ret = BfsMkdir(fs, argc - 2, argv + 2);
    } 
    ... ... ... ...
    else {
        fprintf(stderr, "Unknown command: %s\n", argv[1]);
    }
    return ret;
}
```
 - 函数首先是解析./bfs.flag文件，该文件主要是集群运行的一些配置，比如nameserver部署的地址、文件块副本的数目、高可用策略等。我们这个client主要用到了nameserver的部署地址。  
 - 然后是打开文件系统，然后依据命令行参数argv[1]的值调用对应的函数。  

这里的每个命令的实现函数的第一个参数都是：baidu::bfs::FS* fs，一个已经打开的文件系统，我们先分析下打开文件系统的实现：  
```C++
// nameserver为nameserver的部署地址，格式为：127.0.0.1:8827,127.0.0.1:8828,127.0.0.1:8829，FS：需要该函数创建，FSOptions：表示文件系统的username和passwd
static bool OpenFileSystem(const char* nameserver, baidu::bfs::FS** fs, const FSOptions&);  // 函数原型
```
```C++
bool FS::OpenFileSystem(const char* nameserver, FS** fs, const FSOptions&) {
    FSImpl* impl = new FSImpl;
    if (!impl->ConnectNameServer(nameserver)) {
        *fs = NULL;
        return false;
    }
    *fs = impl;
    return true;
}
```
该函数直接是new FSImpl，然后连接到nameserver。其实打开文件系统的语义很明确，就是连接到nameserver。可见nameserver具有文件系统的元数据。  
baidu::bfs::FS：是一个抽象类，里边定义了一堆虚函数。  
baidu::bfs::FSImpl：继承自FS，代表一个文件系统，实现了文件系统
这两个类位于目录sdk中。  
FSImpl包含如下成员：  
```C++
    RpcClient* rpc_client_;
    NameServerClient* nameserver_client_;  // nameserver
    //NameServer_Stub* nameserver_;
    std::vector<std::string> nameserver_addresses_;  // 多个nameserver的地址
    int32_t leader_nameserver_idx_; // 哪一个是leader。nameserver为了高可用，有多个副本
    //std::string nameserver_address_;
    std::string local_host_name_;   // 本机主机名
    ThreadPool* thread_pool_;       // 线程池
```
可见：nameserver的客户端代理就是一个文件系统。  
```C++
// 构造，默认情况下，会认为第0个nameserver为leader(127.0.0.1:8827)，其他的为备份。
FSImpl::FSImpl() : rpc_client_(NULL), nameserver_client_(NULL), leader_nameserver_idx_(0) {
    local_host_name_ = common::util::GetLocalHostName();
    thread_pool_ = new ThreadPool(FLAGS_sdk_thread_num);
}
// 连接到nameserver，单机默认情况下，模拟集群下：nameserver_nodes=="127.0.0.1:8827,127.0.0.1:8828,127.0.0.1:8829"
bool FSImpl::ConnectNameServer(const char* nameserver) {
    std::string nameserver_nodes = FLAGS_nameserver_nodes;
    if (nameserver != NULL) {
        nameserver_nodes = std::string(nameserver);
    }
    rpc_client_ = new RpcClient();  // new一个RPC Client，创建一个new sofa::pbrpc::RpcClient(options)，这个RPC框架不在这里学习。
    nameserver_client_ = new NameServerClient(rpc_client_, nameserver_nodes);  // 创建NameServerClient
    return true;
}
```
可见本质上就是构造个NameServerClient。  
```C++
NameServerClient::NameServerClient(RpcClient* rpc_client, const std::string& nameserver_nodes)
    : rpc_client_(rpc_client), leader_id_(0) {
    common::SplitString(nameserver_nodes, ",", &nameserver_nodes_);
    stubs_.resize(nameserver_nodes_.size());
    //为每个nameserver创建一个Stub
    for (uint32_t i = 0; i < nameserver_nodes_.size(); i++) {
        rpc_client_->GetStub(nameserver_nodes_[i], &stubs_[i]);   // 通过ip:port得到一个stub
    }
}
```
**小结一下：操作文件系统之前，首先打开文件系统，打开文件系统，实质上就是创建多个nameserver的Stub，为以后的RPC调用打基础。默认情况下，leader nameserver为第0个Stub**  

接下来分析各个命令的实现。  

我们首先看最重要的几个命令的实现来交接客户端与集群的核心交互逻辑：  
 - touchz：文件的创建  
 - cat: 文件的读  
 - put: 文件的写  
 - get: 文件的读  

# 1. touchz 文件的创建
touchz创建多个文件的实现如下：  
```C++
int BfsTouchz(baidu::bfs::FS* fs, int argc, char* argv[]) {
    if (argc < 1) {
        print_usage();
        return 1;
    }
    int32_t ret = 0;
    baidu::bfs::File* file;
    for (int i = 0; i < argc; i++) {
        ret = fs->OpenFile(argv[i], O_WRONLY, 0644, &file, baidu::bfs::WriteOptions());
        if (ret != 0) {
            fprintf(stderr, "Open %s fail\n", argv[i]);
            return 1;
        }
    }
    return 0;
}
```
这个函数已经是假设我们拿到一个已经打开的文件系统fs了(多个nameserver的)。touchz文件就是调用文件系统的OpenFile函数。  
```C++
int32_t FSImpl::OpenFile(const char* path, int32_t flags, int32_t mode,
                         File** file, const WriteOptions& options) {
    *file = NULL;
    ... ... ...
    CreateFileRequest request;
    CreateFileResponse response;
    request.set_file_name(path);
    request.set_sequence_id(0);
    request.set_flags(flags);
    request.set_mode(mode&0777);
    request.set_replica_num(write_option.replica);
    bool rpc_ret = nameserver_client_->SendRequest(&NameServer_Stub::CreateFile, &request, &response, 15, 1); // 核心就是调用nameserver的CreateFile
    if (!rpc_ret || response.status() != kOK) {
        // 错误处理
    } else {
        *file = new FileImplWrapper(this, rpc_client_, path, flags, write_option);
    }
    return OK;
}
```
接下来看看nameserver如何创建一个文件：  

