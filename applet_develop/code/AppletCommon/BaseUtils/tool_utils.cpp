/*********************************************************************************
 FileName: 
 Author: xufubo
 Date:  2018-12-26
 Description: 随机数
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/

#include "tool_utils.h"
#include <vector>
#include <errno.h>

using namespace std;

namespace base_utils
{

int Tool::CreateFolder( const char* pcFolderPath )
{
    if( pcFolderPath == NULL )
    {
        return -1;
    }

    vector<string> vElement;
    Tool::Token( pcFolderPath, "/", vElement );

    vector<string> vFilePath;
    string strTmpPath;
    for( unsigned int i = 0; i < vElement.size(); i++ )
    {
        if( i > 0 ) strTmpPath += "/";

        strTmpPath += vElement[ i ];
        vFilePath.push_back( strTmpPath );
    }

    struct stat stStat;
    int index = 0;
    for( index = vFilePath.size() - 1 ; index >= 0; index-- )
    {
        if( stat( vFilePath[index].c_str(), &stStat ) == 0 )
        {
            if( !S_ISDIR( stStat.st_mode ) )
            {
                //当前路径是一个已存在的文件
                return -2;
            }
            else
            {
                break;
            }
        }
    }

    for( index++; index < (int)vFilePath.size(); index++ )
    {
        if( mkdir( vFilePath[index].c_str(), S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH ) != 0 && errno != EEXIST )
        {
            //创建文件夹失败
            return -3;
        }
    }

    return 0;
}


unsigned g_uiSub( unsigned a, unsigned b )
{
    if( a > b )
    {
        return ( a - b );
    }
    else
    {
        return 0;
    }

    return 0;
}

unsigned g_uiSub( unsigned a, int b )
{
    if(( b >= 0 &&  a >= (unsigned)b) || b < 0 )
    {
        return ( a - b );
    }
    else
    {
        return 0;
    }
    return 0;
}

unsigned g_uiSub( int a, unsigned b )
{
    if( a > 0 && (unsigned)a > b )
    {
        return ( a - b );
    }
    else
    {
        return 0;
    }
    return 0;
}

unsigned g_uiSub( int a, int b )
{
    if( a > b )
    {
        return ( a - b );
    }
    else
    {
        return 0;
    }

    return 0;
}




//容器内存放string的map, 当input为NULL时，表示对剩余的所有数据进行解析
void Tool::Token( const char* input, const char* seq ,std::list<std::string> &out )
{
    out.clear();

    if( seq == NULL || input == NULL )
    {
        return ;
    }

    std::string strInput = input ;
    int iSepLen = strlen( seq );
    size_t pos=0;

    while( ( pos = strInput.find(seq) ) != std::string::npos )
    {
        out.push_back( strInput.substr(0,pos) );
        strInput=strInput.substr( pos + iSepLen );
    }

    if( strInput.size() > 0 )
    {
        out.push_back( strInput );
    }
    return;
}

//容器内存放string的vector, 当input为NULL时，表示对剩余的所有数据进行解析
void Tool::Token( const char* input, const char* seq ,std::vector<std::string> &out )
{
    out.clear();

    if( seq == NULL || input == NULL )
    {
        return ;
    }

    std::string strInput = input ;
    int iSepLen = strlen( seq );
    size_t pos=0;

    while( ( pos = strInput.find(seq) ) != std::string::npos )
    {
        out.push_back( strInput.substr( 0, pos ) );
        strInput=strInput.substr( pos + iSepLen );
    }

    if( strInput.size() > 0 )
    {
        out.push_back( strInput );
    }
    return;
}

//容器内存放string的map, 当input为NULL时，表示对剩余的所有数据进行解析,解析后数据存在string字段
void Tool::Token( const char* input, const char* seq ,std::map<std::string,int> &out )
{
    out.clear();

    if( seq == NULL || input == NULL )
    {
        return ;
    }

    std::string strInput = input ;
    int iSepLen = strlen( seq );
    size_t pos=0;

    while( ( pos = strInput.find(seq) ) != std::string::npos )
    {
        out[ strInput.substr(0,pos) ] = 1;
        strInput=strInput.substr( pos + iSepLen );
    }

    if( strInput.size() > 0 )
    {
        out[ strInput ] = 1;
    }
    return;
}

//容器内存放string的map, 当input为NULL时，表示对剩余的所有数据进行解析,解析后数据存在string字段
void Tool::Token( const char* input, const char* seq ,std::map<int,std::string> &out )
{
    out.clear();

    if( seq == NULL || input == NULL )
    {
        return ;
    }

    std::string strInput = input ;
    int iSepLen = strlen( seq );
    int iIndex = 0;
    size_t pos=0;

    while( ( pos = strInput.find(seq) ) != std::string::npos )
    {
        out[ iIndex ] = strInput.substr(0,pos);
        strInput=strInput.substr( pos + iSepLen );
        iIndex ++;
    }

    if( strInput.size() > 0 )
    {
        out[ iIndex ] = strInput;
    }
    return;
}

void Tool::Token( const char* input, const char* seq ,std::vector<INT64> &out )
{
    out.clear();

    if( seq == NULL || input == NULL )
    {
        return ;
    }

    std::string strInput = input;
    int iSepLen = strlen( seq );
    size_t pos=0;
    INT64 value = 0;

    while( ( pos = strInput.find(seq) ) != std::string::npos )
    {
        if( pos > 0 )
        {
            value = strtoull(strInput.substr(0,pos).c_str(), NULL, 10 );
            out.push_back( value );
        }
        strInput=strInput.substr( pos + iSepLen );
    }

    if( strInput.size() > 0 )
    {
        value = strtoull( strInput.c_str(), NULL, 10 );
        out.push_back( value );
    }
    return;
}

}
