# 概述
近期在学习百度开源的BFS，其实现使用了LevelDB，对LevelDB产生了浓厚的兴趣，里边的很多技术点以及技术细节也一直是自己想了解的。本文章先从其官方使用文档开始，逐步分析LevelDB的实现。
本篇文章主要是对LevelDB官方文档的翻译。
leveldb
=======
_Jeff Dean, Sanjay Ghemawat_
leveldb支持key-value形式的持久性存储。Keys和Values可以是任意的字节数组。leveldb中的数据是根据用户提供的比较函数(comparator function)对key进行排序后将键值进行存储的。
# 打开数据库
leveldb数据库以一个文件目录作为名字。数据库中的所有数据均存放在该目录下。以下实例示范了如何打开一个leveldb数据库，并且在数据库不存在时新建一个。
```C++
#include <cassert>
#include "leveldb/db.h"

leveldb::DB* db;
leveldb::Options options;
options.create_if_missing = true;
leveldb::Status status = leveldb::DB::Open(options, "/tmp/testdb", &db);
assert(status.ok());
...
```
如果想要在数据库已存在时报错，则需要在调用leveldb::DB::Open之前调用：
```C++
options.error_if_exists = true;
```
# Status
你可能注意到，在上面打开数据的例子中，返回值得类型为leveldb::Status。leveldb中的大部分可能返回失败状态的函数都返回该类型的值。用户可以检查返回值是否为ok，并且打印其携带的相关错误信息：
```C++
leveldb::Status s = ...;
if (!s.ok()) cerr << s.ToString() << endl;
```
# 关闭数据库
当不在使用数据库时，只需要删除对应的实例即可关闭数据库。例如：
```C++
... open the db as described above ...
... do something with db ...
delete db;
```
# 读操作和写操作
数据库提供了Put，Delete和Get方法来修改/查询数据库。例如，以下代码将存储在key1下的值移至key2下：
```C++
std::string value;
leveldb::Status s = db->Get(leveldb::ReadOptions(), key1, &value);
if (s.ok()) s = db->Put(leveldb::WriteOptions(), key2, value);
if (s.ok()) s = db->Delete(leveldb::WriteOptions(), key1);
```
# 原子更新
上面的读写例子中，如果程序在删除key1前退出，那么数据库中将会存在两个拥有相同value的key。这类问题可以通过使用WriteBatch类将一组更新操作作为原子动作执行。如下所示：
```C++
#include "leveldb/write_batch.h"
...
std::string value;
leveldb::Status s = db->Get(leveldb::ReadOptions(), key1, &value);
if (s.ok()) {
  leveldb::WriteBatch batch;
  batch.Delete(key1);
  batch.Put(key2, value);
  s = db->Write(leveldb::WriteOptions(), &batch);
}
```
WriteBatch包含了对数据库进行的一系列操作。然后这些操作将会根据添加的顺序进行执行。注意该例中delete删除了key1放在了put操作之前，这样可以避免key1和key2相同的情况下，将key1和key2全部错误的删掉。
WriteBatch除了具有原子性的优势之外，你还可以将多个单个的操作放在同一个WriteBatch中以加快批量更新。

# 同步写
默认情况下，对数据库的写操作是异步的：将数据从程序中写入操作系统后立即返回。数据从操作系统保存到底层的持久性存储器是异步的操作。通过打开sync标志，可以让写操作阻塞直到写入的数据全部被保存到持久存储器中。(在Posix系统中，可以通过在写操作中调用fsync(…)、fdatasync(…)或者msync(…, MS_SYNC)来实现)。
```C++
leveldb::WriteOptions write_options;
write_options.sync = true;
db->Put(write_options, ...);
```
异步写常常比同步写快千倍以上。但是异步写的缺点是如果机器crash的话会导致最近的几次操作更新丢失。然而，如果仅仅是写操作进程(而不是机器重启)crash的话则不会导致更新操作丢失，因为即使sync标识未打开，当一个写操作返回前，更新操作已经将数据从进程的内存空间传给了操作系统内核空间，操作系统会将这些数据写到持久性存储器中。
异步写通常可以安全的使用。例如，在加载大量数据数据到数据库时，若发生了crash，可以通过重启批量加载来处理丢失的更新操作。若是混合操作的情况下，如每隔几个异步写就插入一个同步写操作，在这样的情况下发生了crash，那么将在上一次运行的最后一个同步写操作结束后，重新启动批量加载。(同步写会更新一个标识用于记录crash后从何处重启)。
WriteBatch提供了异步写之外的另一种选择。多个更新操作可以放置于同一个WriteBatch中，然后已同步写的方式一起执行(即write_options.sync设置为true)。同步写的额外开销将被平摊给批量操作中的所有写操作。

# 并发性
一个数据库同一时间只能被一个进程打开。为了防止误操作，leveldb会从操作系统获取一个锁。在一个进程中，同一个leveldb::DB的实例可以被多个线程并行共享使用，即不同的线程可以对同一个数据库同时执行写操作(write)，获取迭代器(iterators )，或者调用Get()方法，而不需要做任何额外的同步措施(leveldb内部实现会自动做同步)。然而其他类的实例(如Iterator和WriteBatch)则需要使用者在使用时进行同步。如果多个线程共享这样的对象，他们必须使用锁机制进行访问同步。更多的细节可以查看公共头文件。
# 迭代/遍历(Iteration)
以下例子示范了如何打印数据库中所有的键值对：
```C++
leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
for (it->SeekToFirst(); it->Valid(); it->Next()) {
  cout << it->key().ToString() << ": "  << it->value().ToString() << endl;
}
assert(it->status().ok());  // Check for any errors found during the scan
delete it;
```
在此基础上修改，示范怎么处理key在[start,limit)区间内的数据：
```C++
for (it->Seek(start);
   it->Valid() && it->key().ToString() < limit;
   it->Next()) {
  ...
}
```
也可以以相反的顺序处理数据：(警告：倒序迭代会比正序迭代稍微慢一些)
```C++
for (it->SeekToLast(); it->Valid(); it->Prev()) {
  ...
}
```

# 快照(Snapshots)
快照代表了数据库的所有键值数据在某一时间点的状态，提供了一致的读视图。ReadOptions::snapshot为非空时，表示读操作应该在一个特定版本的数据库状态上进行操作。如果ReadOptions::snapshot为空，读操作将隐式的在数据库的当前状态上进行操作。 
快照可以通过DB::GetSnapshot()方法构造：
```C++
leveldb::ReadOptions options;
options.snapshot = db->GetSnapshot();
... apply some updates to db ...
leveldb::Iterator* iter = db->NewIterator(options);
... read using iter to view the state when the snapshot was created ...
delete iter;
db->ReleaseSnapshot(options.snapshot);
```
当快照不再需要时，要通过DB::ReleaseSnapshot接口进行释放。这允许实现摆脱为维持读取快照而维护的状态。
# Slice
在上面的代码中，it->key() 和it->value()方法的返回值类型为leveldb::Slice。Slice是包含一个指向外部字节数组的指针以及字节数组长度的简单结构。相比于返回std::string，返回一个Slice的开销更小，因为不需要隐式的拷贝键值数据。另外，leveldb的方法不返回C语言形式的以’\0’结束的字符串，因为leveldb的key和value都允许包含’\0’。 
C++ string和C风格形式字符串可以很简单的转换为Slice类型：
```C++
leveldb::Slice s1 = "hello";

std::string str("world");
leveldb::Slice s2 = str;
```
Slice类型也可以很简单的转换为C++ string：
```C++
std::string str = s1.ToString();
assert(str == std::string("hello"));
```
必须要注意的是在使用Slice期间Slice中的指针所指向的字节数组必须是有效的。例如，以下场景就是错误的：
```C++
leveldb::Slice slice;
if (...) {
  std::string str = ...;
  slice = str;
}
Use(slice);
```
当离开if的作用域是，str将会被销毁，slice的指针将指向一个非法的地址。

# 比较器(Comparators)
之前的里例子中，使用了默认的按照字母顺序比较的方法对key进行顺序排序。Leveldb支持自定义的排序方法。例如，假设key是由2个数字组成的结构，排序时首先使用第一个数字做比较，若第一个数字相等，再使用第二个数字做比较。首先，需要继承leveldb::Comparator的实现一个子类：
```C++
class TwoPartComparator : public leveldb::Comparator {
 public:
  // Three-way comparison function:
  //   if a < b: negative result
  //   if a > b: positive result
  //   else: zero result
  int Compare(const leveldb::Slice& a, const leveldb::Slice& b) const {
    int a1, a2, b1, b2;
    ParseKey(a, &a1, &a2);
    ParseKey(b, &b1, &b2);
    if (a1 < b1) return -1;
    if (a1 > b1) return +1;
    if (a2 < b2) return -1;
    if (a2 > b2) return +1;
    return 0;
  }

  // Ignore the following methods for now:
  const char* Name() const { return "TwoPartComparator"; }
  void FindShortestSeparator(std::string*, const leveldb::Slice&) const {}
  void FindShortSuccessor(std::string*) const {}
};
```
然后创建一个使用该比较器的数据库：
```C++
TwoPartComparator cmp;
leveldb::DB* db;
leveldb::Options options;
options.create_if_missing = true;
options.comparator = &cmp;
leveldb::Status status = leveldb::DB::Open(options, "/tmp/testdb", &db);
...
```
# 向后兼容(Backwards compatibility)
数据库被创建时，指定的比较器(leveldb::Comparator)的Name方法的返回值将会被保存起来；之后每一次打开数据库时，都会检查该值是否与本次打开指定的比较器的Name方法的返回值匹配。如果名字变了，那么 leveldb::DB::Open方法就会返回失败。因此，只有在新的key格式和比较器无法与现有的数据库兼容是，可以使用新的名称；同时，现在有的数据库的所有数据都讲被丢弃。 
然而，通过提前制定计划也可以实现key格式的逐渐改变。例如，在每个key的末尾中保存一个版本号(一个字节通常够用了)，当需要修改key的格式的时，可以在不修改比较器名称的前提下，增加版本号的值，然后修改比较器的比较函数，使其通过区分key中的版本号，来做不同的处理。

# 性能
通过修改一些参数可以调整leveldb的性能，可以在include/leveldb/options.h中查看定义。
## 块大小

## 压缩

## cache

## 键布局(Key Layout)

## 过滤器(Filters)
过滤器
# 校验和

# 近似大小

# 环境(Environment)

# 可移植性(Porting)

# 其他信息
有关leveldb实现的细节可以在以下文档中找到：
1. [实现说明](impl.md)
2. [immutable Table文件格式](table_format.md)
3. [日志文件格式](log_format.md)











