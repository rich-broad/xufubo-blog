
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <list>
#include <vector>
#include <map>
#include <set>
#include <string>

#ifndef SWEET_TOOL_20120401_H_
#define SWEET_TOOL_20120401_H_

using namespace std;

unsigned g_uiSub( unsigned a, unsigned b );
unsigned g_uiSub( unsigned a, int b );
unsigned g_uiSub( int a, unsigned b );
unsigned g_uiSub( int a, int b );

typedef int64_t INT64;
namespace base_utils
{

class Tool
{
public:

    static int CreateFolder( const char* pcFolderPath );

    //注意:
    //以下各函数均适用
    //1. 当需要将字符串解析为数值时，如果字段为空，则丢弃该值
    //   如 "123,,12,",解析后得到的是123和12两个数据，而不是123,0,12
    //2. 当存储为字符串时，即使是空值，也会存储一个空字符串
    //3. 使用map时，请注意这里不是multimap，所以如果存在相同的数值，有可能互相覆盖

    //容器内存放基本数据类型(short, int, long,unsigned)的list
    template< class T >
    static void Token( const char* input, const char* seq ,std::list<T> &out );

    //容器内存放基本数据类型(short, int, long,unsigned)的vector
    template< class T >
    static void Token( const char* input, const char* seq ,std::vector<T> &out );

    static void Token( const char* input, const char* seq ,std::vector<INT64> &out );


    //容器内存放基本数据类型的map, 解析后数据存在T字段
    template< class T >
    static void Token( const char* input, const char* seq ,std::map<T,int> &out );

    //容器内存放基本数据类型的set, 解析后数据存在T字段
    template< class T >
    static void Token( const char* input, const char* seq ,std::set<T> &out );

    //容器内存放string的list
    static void Token( const char* input, const char* seq ,std::list<std::string> &out );

    //容器内存放string的vector
    static void Token( const char* input, const char* seq ,std::vector<std::string> &out );

    //容器内存放string的map,解析后数据存在string字段
    static void Token( const char* input, const char* seq ,std::map<std::string,int> &out );

    //容器内存放string的map,解析后数据存在string字段
    static void Token( const char* input, const char* seq ,std::map<int,std::string> &out );
};

//容器内存放基本数据类型(short, int, long,unsigned)的list
template< class T >
void Tool::Token( const char* input, const char* seq ,std::list<T> &out )
{
    out.clear();

    if( seq == NULL || input == NULL )
    {
        return ;
    }

    std::string strInput = input;
    int iSepLen = strlen( seq );
    size_t pos=0;
    T value = 0;

    while( ( pos = strInput.find(seq) ) != std::string::npos )
    {
        if( pos > 0 )
        {
            value = (T)atoll(strInput.substr(0,pos).c_str() );
            out.push_back( value );
        }
        strInput=strInput.substr( pos + iSepLen );
    }

    if( strInput.size() > 0 )
    {
        value = (T)atoll( strInput.c_str() );
        out.push_back( value );
    }
    return;
}

template< class T >
void Tool::Token( const char* input, const char* seq ,std::vector<T> &out )
{
    out.clear();

    if( seq == NULL || input == NULL )
    {
        return ;
    }

    std::string strInput = input;
    int iSepLen = strlen( seq );
    size_t pos=0;
    T value = 0;

    while( ( pos = strInput.find(seq) ) != std::string::npos )
    {
        if( pos > 0 )
        {
            value = (T)atoll(strInput.substr(0,pos).c_str() );
            out.push_back( value );
        }
        strInput=strInput.substr( pos + iSepLen );
    }

    if( strInput.size() > 0 )
    {
        value = (T)atoll( strInput.c_str() );
        out.push_back( value );
    }
    return;
}

//容器内存放基本数据类型的set, 解析后数据存在T字段
template< class T >
void Tool::Token( const char* input, const char* seq ,std::set<T> &out )
{
    out.clear();

    if( seq == NULL || input == NULL )
    {
        return ;
    }

    std::string strInput = input;
    int iSepLen = strlen( seq );
    size_t pos=0;
    T value = 0;

    while( ( pos = strInput.find(seq) ) != std::string::npos )
    {
        if( pos > 0 )
        {
            value = (T)atoll(strInput.substr(0,pos).c_str() );
            out.insert( value );
        }
        strInput=strInput.substr( pos + iSepLen );
    }

    if( strInput.size() > 0 )
    {
        value = (T)atoll( strInput.c_str() );
        out.insert( value );
    }
    return;
}


template< class T >
void Tool::Token( const char* input, const char* seq ,std::map<T,int> &out )
{
    out.clear();

    if( seq == NULL || input == NULL )
    {
        return ;
    }

    std::string strInput = input;
    int iSepLen = strlen( seq );
    size_t pos=0;

    while( ( pos = strInput.find(seq) ) != std::string::npos )
    {
        if( pos > 0 )
        {
            T iIndex = atoll( strInput.substr(0,pos).c_str() );
            out[ iIndex ] = 1;
        }
        strInput=strInput.substr( pos + iSepLen );
    }

    if( strInput.size() > 0 )
    {
        T iIndex = atoll( strInput.c_str() );
        out[ iIndex ] = 1;
    }

    return;
}


};

#endif

