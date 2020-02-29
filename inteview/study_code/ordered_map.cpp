
#include <unistd.h>
#include <time.h>

#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <stdlib.h>

#include <map>

//g++ test.cpp -std=c++11

using namespace std;
 
struct user_info_info
{
	unsigned int up_time;
    short sum;
    short se;
};


int main(int argc, char** argv)
{
    user_info_info user_info;
    cout << "sizeof(user_info):" << sizeof(user_info) << endl;
    map<int64_t, user_info_info> user_info_tree_mp;
    cout << "===========put 30000000 start====================" << endl;
    cout << time(NULL) << endl;
    for(size_t i = 0; i < 30000000; ++i)
    {
        user_info_tree_mp[i] = user_info;
    }
    cout << time(NULL) << endl;
    cout << "===========put 30000000 end====================" << endl;


    cout << "===========get 30000000 start====================" << endl;
    cout << time(NULL) << endl;
    for(size_t i = 0; i < 30000000; ++i)
    {
        user_info_info tttt = user_info_tree_mp[i];
    }
    cout << time(NULL) << endl;
    cout << "===========get 30000000 end====================" << endl;

    cout << "================loop 100 put start===============" << endl;
    cout << time(NULL) << endl;
    for(size_t i = 0; i < 100; ++i)
    {
        user_info_tree_mp[i] = user_info;
    }
    cout << time(NULL) << endl;
    cout << "=================loop 100 put end==============" << endl;

    cout << "================loop 100 get start===============" << endl;
    cout << time(NULL) << endl;
    for(size_t i = 0; i < 100; ++i)
    {
        user_info_info tttt = user_info_tree_mp[i];
    }
    cout << time(NULL) << endl;
    cout << "=================loop 100 get end==============" << endl;

    cout << "================get start===============" << endl;
    cout << time(NULL) << endl;
    cout << user_info_tree_mp[1].up_time << endl;
    cout << time(NULL) << endl;
    cout << "=================get end==============" << endl;

    cout << "================put start===============" << endl;
    cout << time(NULL) << endl;
    user_info_tree_mp[1] = user_info;
    cout << time(NULL) << endl;
    cout << "=================put end==============" << endl;

    cout << "user_info_tree_mp.size=" << user_info_tree_mp.size() << endl;

    cout << "================clear start===============" << endl;
    cout << time(NULL) << endl;
    user_info_tree_mp.clear();
    cout << time(NULL) << endl;
    cout << "=================clear end==============" << endl;

    cout << "user_info_tree_mp.size=" << user_info_tree_mp.size() << endl;

    sleep(1000);
    return 0;
    
}

/*
[xufubo@localhost study_code]$ g++ ordered_map.cpp -std=c++11
[xufubo@localhost study_code]$ ./a.out 
sizeof(user_info):8
===========put 30000000 start====================
1582971614
1582971642
===========put 30000000 end====================
===========get 30000000 start====================
1582971642
1582971656
===========get 30000000 end====================
================loop 100 put start===============
1582971656
1582971656
=================loop 100 put end==============
================loop 100 get start===============
1582971656
1582971656
=================loop 100 get end==============
================get start===============
1582971656
6311384
1582971656
=================get end==============
================put start===============
1582971656
1582971656
=================put end==============
user_info_tree_mp.size=30000000
================clear start===============
1582971656
1582971657
=================clear end==============
user_info_tree_mp.size=0



看看资源占用情况：
[xufubo@localhost ~]$ ps aux | grep out
xufubo    12303  100 11.0 581872 570248 pts/1   R+   22:48   0:08 ./a.out
xufubo    12305  0.0  0.0 112712   952 pts/2    S+   22:49   0:00 grep --color=auto out
[xufubo@localhost ~]$ 
[xufubo@localhost ~]$ top -p 12303
top - 22:50:08 up  9:47,  4 users,  load average: 0.27, 0.22, 0.14
Tasks:   1 total,   0 running,   1 sleeping,   0 stopped,   0 zombie
%Cpu(s):  0.0 us,  0.1 sy,  0.0 ni, 99.9 id,  0.0 wa,  0.0 hi,  0.0 si,  0.0 st
KiB Mem :  5182416 total,   773040 free,  2529636 used,  1879740 buff/cache
KiB Swap:  5374972 total,  5374972 free,        0 used.  2323808 avail Mem 

   PID USER      PR  NI    VIRT    RES    SHR S  %CPU %MEM     TIME+ COMMAND                                                                                                                                    
 12303 xufubo    20   0 1887616 1.789g   1008 S   0.0 36.2   0:41.97 a.out  

map,3000万，key为int64_t,value大小也是8字节，总共占了1.789GB内存。

*/


