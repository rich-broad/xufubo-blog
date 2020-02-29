
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
/*
class util
{
public:

static void getOneLine(string &str)
{
	getline(cin, str);
	return;
}

static long sumArray(const vector<int>& src)
{
	long sum = 0;
	for(size_t i = 0; i < src.size(); ++i)
	{
		sum += src[i];
	}
	return sum;
}

static void sepstr(const string &sStr, const string &sSep, vector<int>& vt)
{
    string::size_type pos = 0;
    string::size_type pos1 = 0;
    int pos_tmp = -1;

    while(true)
    {
        string s;
        string s1;
        pos1 = sStr.find_first_of(sSep, pos);
        if(pos1 == string::npos)
        {
            if(pos + 1 <= sStr.length())
            {
                s = sStr.substr(-1 != pos_tmp ? pos_tmp : pos);
                s1 = "";
            }
        }
        else if(pos1 == pos && (pos1 + 1 == sStr.length()))
        {
            s = "";
            s1 = "";
        }
        else
        {
            s = sStr.substr(-1 != pos_tmp ? pos_tmp : pos, pos1 - (-1 != pos_tmp ? pos_tmp : pos));
            s1 = sStr.substr(pos1 + 1);
            if (-1 == pos_tmp)
                pos_tmp = pos;
            pos = pos1;
        }

        if(!s.empty())
        {
            vt.push_back(atoi(s.c_str()) );
        }
        pos_tmp = -1;

        if(pos1 == string::npos)
        {
            break;
        }

        pos++;
    }

    return;
}

static void swapBlance(vector<int> & a, vector<int> & b)
{
	long suma = sumArray(a);
	long sumb = sumArray(b);

	long min = suma > sumb ? suma : sumb;

	int minus = 0;

	for(size_t i = 0; i < a.size(); ++i)
	{
		for(size_t j = 0; j < b.size(); ++j)
		{
			int temp = a[i];
			a[i] = b[j];
			b[j] = temp;

			minus = abs(sumArray(a) - sumArray(b));
			if (minus < min)
			{
				min = minus;
				continue;
			}
			else
			{
				int temp = a[i];
				a[i] = b[j];
				b[j] = temp;
			}
		}
	}
	return;
}

};
*/
 
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
    for(size_t i = 0; i < 300000000; ++i)
    {
        user_info_hash_mp[i] = user_info;
    }
    cout << time(NULL) << endl;
    cout << "===========put 30000000 end====================" << endl;


    cout << "===========get 30000000 start====================" << endl;
    cout << time(NULL) << endl;
    for(size_t i = 0; i < 300000000; ++i)
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

    sleep(1000);
    return 0;
    
}

/*

*/


