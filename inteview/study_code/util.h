#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <cassert>
#include <cstdio>
#include <string>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <stack>
#include <vector>
#include <set>

//g++ test.cpp -std=c++11


using namespace std;

class util
{
public:
static void display(const vector<string>& src)
{
	cout << "[ " << endl;
	for(size_t i = 0; i < src.size(); ++i)
	{
		cout << "    " << src[i] << endl;
	}

	cout << " ]" << endl;
	return;
}

template <typename Iterator> static void display(Iterator begin, Iterator end)
{
	cout << "[ " << endl;
	for(Iterator it = begin; it != end; ++it)
	{
		cout << "    " << *it << endl;
	}

	cout << " ]" << endl;
	return;
}

template <typename Iterator> static void displayMap(Iterator begin, Iterator end)
{
	cout << "[ " << endl;
	for(Iterator it = begin; it != end; ++it)
	{
		cout << "    " << it->first << "=" << it->second << endl;
	}

	cout << " ]" << endl;
	return;
}

//一行一行的读取数据，遇到文件结束符结束  ctrl+D
static int getAllLinesByEOF(vector<string>& res)
{
	res.clear();
	string str;
	while (getline(cin, str))
	{
		res.push_back(str);
	}
	return 0;
}

static int getAllLinesByEOF2Map(map<string, int>& res)
{
	res.clear();
	string str;
	while (getline(cin, str))
	{
		res[str] = 0;
	}
	return 0;
}

static int getAllLinesByEOF2Set(set<string>& res)
{
	res.clear();
	string str;
	while (getline(cin, str))
	{
		res.insert(str);
	}
	return 0;
}


static int getLinesByNum(vector<string>& res)
{
	res.clear();
	size_t num = 0;
	cout << "input lines num:";
	cin >> num;
	string str;
	cout << "num = " << num << endl;
	for (size_t i = 0; i <= num; i++)
	{
		getline(cin, str);
		res.push_back(str);
	}
	
	return 0;
}

static int getLinesByNum2Map(map<string, int>& res)
{
	res.clear();
	size_t num = 0;
	cout << "input lines num:";
	cin >> num;
	string str;
	cout << "num = " << num << endl;
	for (size_t i = 0; i <= num; i++)
	{
		getline(cin, str);
		res[str] = 0;
	}
	
	return 0;
}

/*
这种得到num个字符串但是我们的循环确进行了num+1次，这是因为第一次得到的为一个换行符
*/
static int getLinesByNum2MapCount(map<string, int>& res)
{
	res.clear();
	size_t num = 0;
	cin >> num;
	string str;
	for (size_t i = 0; i <= num; i++)
	{
		getline(cin, str);
		(res.find(str) == res.end()) ? (res[str] = 1) : (res[str]++);
	}
	
	return 0;
}

template <typename Iterator> static void displayMapByCount(Iterator begin, Iterator end)
{
    
	for (Iterator it = begin; it != end; ++it)
    {	if (it->second > 1)
		{
			for(int i = 0; i < it->second; i++)
			{
				cout << it->first << endl;
			}
				
		}
		else
		{
			cout << it->first << endl;
		}	
    }	
	return;
}

static int getLinesByNum2Set(set<string>& res)
{
	res.clear();
	size_t num = 0;
	cout << "input lines num:";
	cin >> num;
	string str;
	cout << "num = " << num << endl;
	for (size_t i = 0; i <= num; i++)
	{
		getline(cin, str);
		res.insert(str);
	}
	
	return 0;
}



static int getLinesByEnter(vector<string>& res)
{
	res.clear();
	string str;
	while (getline(cin, str))
	{
		if(str.empty())
		{
			break;
		}
		res.push_back(str);
	}
	return 0;
}

static int getLinesByEnter2Map(map<string, int>& res)
{
	res.clear();
	string str;
	while (getline(cin, str))
	{
		if(str.empty())
		{
			break;
		}
		res[str] = 0;
	}
	return 0;
}

static int getLinesByEnter2Set(set<string>& res)
{
	res.clear();
	string str;
	while (getline(cin, str))
	{
		if(str.empty())
		{
			break;
		}
		res.insert(str);
	}
	return 0;
}

static string getOneLine()
{
	string str;
	getline(cin, str);
	return str;
}


static void getOneLine(string &str)
{
	getline(cin, str);
	return;
}

static string trim(const string &sStr, const string &s =  " \r\n\t", bool bChar = true)
{
    if(sStr.empty())
    {
        return sStr;
    }

    /**
    * 将完全与s相同的字符串去掉
    */
    if(!bChar)
    {
        return trimright(trimleft(sStr, s, false), s, false);
    }

    return trimright(trimleft(sStr, s, true), s, true);
}

static string trimleft(const string &sStr, const string &s =  " \r\n\t", bool bChar = true)
{
    if(sStr.empty())
    {
        return sStr;
    }

    /**
    * 去掉sStr左边的字符串s
    */
    if(!bChar)
    {
        if(sStr.length() < s.length())
        {
            return sStr;
        }

        if(sStr.compare(0, s.length(), s) == 0)
        {
            return sStr.substr(s.length());
        }

        return sStr;
    }

    /**
    * 去掉sStr左边的 字符串s中的字符
    */
    string::size_type pos = 0;
    while(pos < sStr.length())
    {
        if(s.find_first_of(sStr[pos]) == string::npos)
        {
            break;
        }

        pos++;
    }

    if(pos == 0) return sStr;

    return sStr.substr(pos);
}

static string trimright(const string &sStr, const string &s =  " \r\n\t", bool bChar = true)
{
    if(sStr.empty())
    {
        return sStr;
    }

    /**
    * 去掉sStr右边的字符串s
    */
    if(!bChar)
    {
        if(sStr.length() < s.length())
        {
            return sStr;
        }

        if(sStr.compare(sStr.length() - s.length(), s.length(), s) == 0)
        {
            return sStr.substr(0, sStr.length() - s.length());
        }

        return sStr;
    }

    /**
    * 去掉sStr右边的 字符串s中的字符
    */
    string::size_type pos = sStr.length();
    while(pos != 0)
    {
        if(s.find_first_of(sStr[pos-1]) == string::npos)
        {
            break;
        }

        pos--;
    }

    if(pos == sStr.length()) return sStr;

    return sStr.substr(0, pos);
}

static string lower(const string &s)
{
    string sString = s;
    for (string::iterator iter = sString.begin(); iter != sString.end(); ++iter)
    {
        *iter = tolower(*iter);
    }

    return sString;
}

static string upper(const string &s)
{
    string sString = s;

    for (string::iterator iter = sString.begin(); iter != sString.end(); ++iter)
    {
        *iter = toupper(*iter);
    }

    return sString;
}

static bool isdigit(const string &sInput)
{
    string::const_iterator iter = sInput.begin();

    if(sInput.empty())
    {
        return false;
    }

    while(iter != sInput.end())
    {
        if (!::isdigit(*iter))
        {
            return false;
        }
        ++iter;
    }
    return true;
}

/**
* @brief  替换字符串.
*
* @param sString  输入字符串
* @param sSrc     原字符串
* @param sDest    目的字符串
* @return string  替换后的字符串
*/
static string replace(const string &sString, const string &sSrc, const string &sDest)
{
    if(sSrc.empty())
    {
        return sString;
    }

    string sBuf = sString;

    string::size_type pos = 0;

    while( (pos = sBuf.find(sSrc, pos)) != string::npos)
    {
        sBuf.replace(pos, sSrc.length(), sDest);
        pos += sDest.length();
    }

    return sBuf;
}

/**
* @brief  批量替换字符串.
*
* @param sString  输入字符串
* @param mSrcDest  map<原字符串,目的字符串>
* @return string  替换后的字符串
*/
static string replace(const string &sString, const map<string,string>& mSrcDest)
{
    if(sString.empty())
    {
        return sString;
    }

    string tmp = sString;
    map<string,string>::const_iterator it = mSrcDest.begin();

    while(it != mSrcDest.end())
    {

        string::size_type pos = 0;
        while((pos = tmp.find(it->first, pos)) != string::npos)
        {
            tmp.replace(pos, it->first.length(), it->second);
            pos += it->second.length();
        }

        ++it;
    }

    return tmp;
}

template<typename T>
static T strto(const string &sStr);

/**
* @brief  字符串转化成T型.
*
* @param sStr      要转换的字符串
* @param sDefault  缺省值
* @return T        转换后的T类型
*/
template<typename T>
static T strto(const string &sStr, const string &sDefault);

static vector<string> sepstr(const string &sStr, const string &sSep, bool withEmpty)
{
    vector<string> vt;

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

        if(withEmpty)
        {
            vt.push_back(s);
        }
        else
        {
            if(!s.empty())
            {
            vt.push_back(s);
                }
        }
        pos_tmp = -1;

        if(pos1 == string::npos)
        {
            break;
        }

        pos++;
    }

    return vt;
}
};

namespace p
{
    template<typename D>
    struct strto1
    {
        D operator()(const string &sStr)
        {
            string s = "0";

            if(!sStr.empty())
            {
                s = sStr;
            }

            istringstream sBuffer(s);

            D t;
            sBuffer >> t;

            return t;
        }
    };

    template<>
    struct strto1<char>
    {
        char operator()(const string &sStr)
        {
            if(!sStr.empty())
            {
                return sStr[0];
            }
            return 0;
        }
    };

    template<>
    struct strto1<short>
    {
        short operator()(const string &sStr)
        {
            if(!sStr.empty())
            {
                return atoi(sStr.c_str());
            }
            return 0;
        }
    };

    template<>
    struct strto1<unsigned short>
    {
        unsigned short operator()(const string &sStr)
        {
            if(!sStr.empty())
            {
                return strtoul(sStr.c_str(), NULL, 10);
            }
            return 0;
        }
    };

    template<>
    struct strto1<int>
    {
        int operator()(const string &sStr)
        {
            if(!sStr.empty())
            {
                return atoi(sStr.c_str());
            }
            return 0;
        }
    };

    template<>
    struct strto1<unsigned int>
    {
        unsigned int operator()(const string &sStr)
        {
            if(!sStr.empty())
            {
                return strtoul(sStr.c_str(), NULL, 10);
            }
            return 0;
        }
    };

    template<>
    struct strto1<long>
    {
        long operator()(const string &sStr)
        {
            if(!sStr.empty())
            {
                return atol(sStr.c_str());
            }
            return 0;
        }
    };

    template<>
    struct strto1<long long>
    {
        long long operator()(const string &sStr)
        {
            if(!sStr.empty())
            {
                return atoll(sStr.c_str());
            }
            return 0;
        }
    };

    template<>
    struct strto1<unsigned long>
    {
        unsigned long operator()(const string &sStr)
        {
            if(!sStr.empty())
            {
                return strtoul(sStr.c_str(), NULL, 10);
            }
            return 0;
        }
    };

    template<>
    struct strto1<float>
    {
        float operator()(const string &sStr)
        {
            if(!sStr.empty())
            {
                return atof(sStr.c_str());
            }
            return 0;
        }
    };

    template<>
    struct strto1<double>
    {
        double operator()(const string &sStr)
        {
            if(!sStr.empty())
            {
                return atof(sStr.c_str());
            }
            return 0;
        }
    };

    template<typename D>
    struct strto2
    {
        D operator()(const string &sStr)
        {
            istringstream sBuffer(sStr);

            D t;
            sBuffer >> t;

            return t;
        }
    };

    template<>
    struct strto2<string>
    {
        string operator()(const string &sStr)
        {
            return sStr;
        }
    };

}

template<typename T>
T util::strto(const string &sStr)
{
    using strto_type = typename std::conditional<std::is_arithmetic<T>::value, p::strto1<T>, p::strto2<T>>::type;

    return strto_type()(sStr);
}

template<typename T>
T util::strto(const string &sStr, const string &sDefault)
{
    string s;

    if(!sStr.empty())
    {
        s = sStr;
    }
    else
    {
        s = sDefault;
    }

    return strto<T>(s);
}


/*

struct myComp{
	bool operator() (const int &a,const int &b){
		if(a!=b) return a>b;
		else return a>b;
	}
};
 
int main(){
	map<int,char,myComp> m;
	m[12] = 'm';
	m[1] = 'd';
	m[21] = 'f';
	m[32] = 'a';
	map<int,char,myComp>::iterator it;
	for(it = m.begin();it!=m.end();it++){
		cout<<(*it).first<<":"<<(*it).second<<endl;
	}
	return 0;
}
*/