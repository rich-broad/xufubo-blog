/*********************************************************************************
 FileName: 
 Author: xufubo
 Date:  2018-12-26
 Description: 
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/


#ifndef _COMM_WHITELIST_H_
#define _COMM_WHITELIST_H_

#include <set>
#include <map>
#include <vector>
#include <utility>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <string>
#include <stdio.h>
#include <sys/stat.h>
#include "singleton.h"

#define VAS_WHITELIST (base_utils::HSingleton<base_utils::CommWhiteList>::Instance())


namespace base_utils 
{

    enum EWhiteLogicErrCode 
    {
        ECODE_OK       = 0,
        ECODE_OPEN_FILE_FAIL    =  100001,
        ECODE_GET_FILE_STAT_FAIL = 100002,
    };

struct SWhiteListUnit
{
    time_t last_modified_time; //上一次修改时间
    time_t last_time;          //上一次读取时间
    std::set<std::string> whitelist_set;
    std::vector<std::string> whitelist_vec;
};

class CommWhiteList 
{
public:
    CommWhiteList();
    ~CommWhiteList();

    bool InWhiteList(const std::string &file_path,const std::string &value);

    //@brief 获得白名单列表
    //@param[in] file_path,白名单的文件
    //@param[out] whitelist,白名单列表
    //@retval 文件存在则返回true，否则为false，不关心也可以，不存在默认为空
    bool GetWhiteList(const std::string &file_path, std::vector<std::string>& whitelist);

private:

    bool LoadData(const std::string &file_path);

    int LoadFile(const std::string &file_path);

    int ReloadFile(const std::string &file_path, SWhiteListUnit& whitelist_unit);

    std::map<std::string,SWhiteListUnit>  _whitelist;

private:

    std::set<std::string> *_cache_set;
    std::vector<std::string> *_cache_vec;

    static const time_t _interv = 10;

    static const int INPUT_BUF_SIZE = 4096;

};

}

#endif
