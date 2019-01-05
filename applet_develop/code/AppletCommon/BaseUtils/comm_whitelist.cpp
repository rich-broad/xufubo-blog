/*********************************************************************************
 FileName: 
 Author: xufubo
 Date:  2018-12-26
 Description: 
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/
#include "comm_whitelist.h"

using namespace std;
namespace base_utils
{
CommWhiteList::CommWhiteList()
{
    _cache_set = NULL;
    _cache_vec = NULL;
}

CommWhiteList::~CommWhiteList()
{
}

int CommWhiteList::LoadFile(const string &file_path)
{
    _cache_set = NULL;
    _cache_vec = NULL;

    map<string,SWhiteListUnit>::iterator itr;
    itr = _whitelist.find(file_path);
    if(itr == _whitelist.end())
    {
        pair< map<string,SWhiteListUnit>::iterator,bool> insert_ret;
        SWhiteListUnit whitelist_unit;

        whitelist_unit.last_modified_time = 0;
        whitelist_unit.last_time = 0;
        insert_ret = _whitelist.insert(make_pair(file_path,whitelist_unit));
        itr = insert_ret.first;
    }

    _cache_set = &itr->second.whitelist_set;
    _cache_vec = &itr->second.whitelist_vec;

    //check interv
    time_t now = time(NULL);

    if(now - itr->second.last_time < _interv)
    {
        return ECODE_OK;
    }

    //check file modified time
    struct stat file_stat;
    int ret = stat(file_path.c_str(),&file_stat);
    if(0 != ret)
    {
        //printf( "get file stat fail,ret=%d path[%s] errno[%s]\n",ret, file_path.c_str(), strerror(errno) );
        return ECODE_GET_FILE_STAT_FAIL;
    }

    if(file_stat.st_mtime == itr->second.last_modified_time)
    {
        return ECODE_OK;
    }

    itr->second.last_modified_time = file_stat.st_mtime;
    itr->second.last_time = now;

    return ReloadFile(file_path.c_str(), itr->second);
}


int CommWhiteList::ReloadFile(const std::string &file_path, SWhiteListUnit& whitelist_unit)
{
    int ret = 0;
    FILE *fp = fopen(file_path.c_str(),"r");

    if(fp != NULL)
    {
        //清空老的数据
        whitelist_unit.whitelist_set.clear();
        whitelist_unit.whitelist_vec.clear();
        static char value[INPUT_BUF_SIZE]={0};
        while(fscanf(fp,"%s",value) == 1)
        {
            whitelist_unit.whitelist_set.insert(string(value));
            whitelist_unit.whitelist_vec.push_back(string(value));
        }
        printf("whitelist_set size = %zu",whitelist_unit.whitelist_set.size());
        printf("whitelist_vec size = %zu",whitelist_unit.whitelist_vec.size());

        fclose(fp);
    }
    else
    {
        printf("can not open %s",file_path.c_str());
        ret = ECODE_OPEN_FILE_FAIL;
    }

    return ret;
}

bool CommWhiteList::InWhiteList(const std::string &file_path,const std::string &value)
{
    if(!LoadData(file_path))
    {
        return false;
    }
    return (_cache_set->find(value) != _cache_set->end());
}

bool CommWhiteList::GetWhiteList(const std::string &file_path, vector<string>& whitelist)
{
    if(!LoadData(file_path))
    {
        return false;
    }

    whitelist = *_cache_vec;

    return true;
}

bool CommWhiteList::LoadData(const string &file_path)
{
    int ret = LoadFile(file_path);
    if(0 != ret)
    {
        //在load file 中打error log，这里不打了
        return false;
    }

    if(_cache_set == NULL || _cache_vec == NULL)
    {
        printf("_cache is NULL");
        return false;
    }
    return true;
}

}