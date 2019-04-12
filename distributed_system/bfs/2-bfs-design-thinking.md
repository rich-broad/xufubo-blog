
# 1. 协议设计
分布式文件系统是为了解决海量的小文件，或者单个的超大文件的问题，因此需要将文件的定义抽象出来：  
```proto
message FileInfo {
    optional int64 entry_id = 1;
    optional int64 version = 2;
    optional int32 type = 3 [default = 0755];
    repeated int64 blocks = 4;      // 文件包含哪些块
    optional uint32 ctime = 5;
    optional string name = 6;        // 文件名字
    optional int64 size = 7;        // 文件大小
    optional int32 replicas = 8;
    optional int64 parent_entry_id = 9;
    optional int32 owner = 10;
    repeated string cs_addrs = 11;      // chunckserer的地址
    optional string sym_link = 12;
}
```

分布式文件系统在存储文件数据时也类比了Linux文件系统的设计思路，将文件存储在多个数据块（位于多个不同的服务器中）中。我们需要定义一个块的元数据结构：  
```proto
message BlockMeta {
    optional int64 block_id = 1;    // 块id
    optional int64 block_size = 2;  // 块大小
    optional int64 checksum = 3;    // 校验和
    optional int64 version = 4 [default = -1];          // 版本号
    optional string store_path = 5;                 // 块在磁盘上的存储路径
}
```

设计分布式文件系统的时候，也借鉴了GFS的设计思路，nameserver就是GFS中的master。下边我们先看看nameserver协议的定义：  
```proto


```


