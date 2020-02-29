
#include <unistd.h>
#include <time.h>

#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <stdlib.h>

#include <unordered_map>

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
    unordered_map<int64_t, user_info_info> user_info_hash_mp;
    cout << "===========put 30000000 start====================" << endl;
    cout << time(NULL) << endl;
    for(size_t i = 0; i < 30000000; ++i)
    {
        user_info_hash_mp[i] = user_info;
    }
    cout << time(NULL) << endl;
    cout << "===========put 30000000 end====================" << endl;


    cout << "===========get 30000000 start====================" << endl;
    cout << time(NULL) << endl;
    for(size_t i = 0; i < 30000000; ++i)
    {
        user_info_info tttt = user_info_hash_mp[i];
    }
    cout << time(NULL) << endl;
    cout << "===========get 30000000 end====================" << endl;

    cout << "================loop 100 put start===============" << endl;
    cout << time(NULL) << endl;
    for(size_t i = 0; i < 100; ++i)
    {
        user_info_hash_mp[i] = user_info;
    }
    cout << time(NULL) << endl;
    cout << "=================loop 100 put end==============" << endl;

    cout << "================loop 100 get start===============" << endl;
    cout << time(NULL) << endl;
    for(size_t i = 0; i < 100; ++i)
    {
        user_info_info tttt = user_info_hash_mp[i];
    }
    cout << time(NULL) << endl;
    cout << "=================loop 100 get end==============" << endl;

    cout << "================get start===============" << endl;
    cout << time(NULL) << endl;
    cout << user_info_hash_mp[1].up_time << endl;
    cout << time(NULL) << endl;
    cout << "=================get end==============" << endl;

    cout << "================put start===============" << endl;
    cout << time(NULL) << endl;
    user_info_hash_mp[1] = user_info;
    cout << time(NULL) << endl;
    cout << "=================put end==============" << endl;

    cout << "user_info_hash_mp.size=" << user_info_hash_mp.size() << endl;

    cout << "================clear start===============" << endl;
    cout << time(NULL) << endl;
    user_info_hash_mp.clear();
    cout << time(NULL) << endl;
    cout << "=================clear end==============" << endl;

    cout << "user_info_hash_mp.size=" << user_info_hash_mp.size() << endl;

    sleep(1000);
    return 0;
    
}

/*
[xufubo@localhost inteview]$ g++ ordered_map.cpp -std=c++11
[xufubo@localhost study_code]$ ./a.out 
sizeof(user_info):8
===========put 30000000 start====================
1582971528
1582971538
===========put 30000000 end====================
===========get 30000000 start====================
1582971538
1582971540
===========get 30000000 end====================
================loop 100 put start===============
1582971540
1582971540
=================loop 100 put end==============
================loop 100 get start===============
1582971540
1582971540
=================loop 100 get end==============
================get start===============
1582971540
6311384
1582971540
=================get end==============
================put start===============
1582971540
1582971540
=================put end==============
user_info_hash_mp.size=30000000
================clear start===============
1582971540
1582971541
=================clear end==============
user_info_hash_mp.size=0


看看资源占用情况：
[xufubo@localhost ~]$ ps aux | grep out
xufubo    11909 48.7 23.6 1235124 1223616 pts/1 S+   22:21   0:18 ./a.out
xufubo    11919  0.0  0.0 112712   956 pts/2    S+   22:21   0:00 grep --color=auto out

[xufubo@localhost ~]$ 
[xufubo@localhost ~]$ top -p 11909
top - 22:23:15 up  9:20,  4 users,  load average: 0.05, 0.06, 0.05
Tasks:   1 total,   0 running,   1 sleeping,   0 stopped,   0 zombie
%Cpu(s):  0.0 us,  0.1 sy,  0.0 ni, 99.8 id,  0.1 wa,  0.0 hi,  0.0 si,  0.0 st
KiB Mem :  5182416 total,  1477804 free,  1870608 used,  1834004 buff/cache
KiB Swap:  5374972 total,  5374972 free,        0 used.  2984528 avail Mem 

   PID USER      PR  NI    VIRT    RES    SHR S  %CPU %MEM     TIME+ COMMAND                                                                                                                                    
 11909 xufubo    20   0 1235124 1.167g   1016 S   0.0 23.6   0:18.03 a.out   

unordered_map,3000万，key为int64_t,value大小也是8字节，总共占了1.167GB内存。

*/


