/*********************************************************************************
 FileName: 
 Author: xufubo
 Date:  2019-01-01
 Description:
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/

#include "CategoryMetaManager.h"
#include "Configuration.h"

CategoryMetaManager* volatile CategoryMetaManager::_pInstance = NULL;
tars::TC_ThreadMutex CategoryMetaManager::_instanceMutex;
tars::TC_ThreadRWLocker CategoryMetaManager::_rwLockCategory;
tars::TC_ThreadMutex CategoryMetaManager::_mutexCategory;

CategoryMetaManager* CategoryMetaManager::GetInstance()
{
	if (_pInstance == NULL)
	{
		MutexGuard mg(_instanceMutex);
		if (_pInstance == NULL)
		{
			CategoryMetaManager* volatile tmp = new CategoryMetaManager();
			_pInstance = tmp;
		}
	}
	return _pInstance;
}

CategoryMetaManager::CategoryMetaManager()
{
	_curMap = 0;
}

CategoryMetaManager::~CategoryMetaManager()
{

}

int CategoryMetaManager::initialize()
{
	upCategoryMetaList();
	return 0;
}

int CategoryMetaManager::getCategoryInfo(long id, string &name)
{
    int curNum = 0;
    {
        ReadLock rl(_rwLockCategory);
        curNum = _curMap;
    }
    if (_mpIdName[curNum].find(id) != _mpIdName[curNum].end())
    {
        name = _mpIdName[curNum][id];
    }
    else
    {
        ERRORLOG("appId|" << id << "|not find|" << endl);
        return -1;
    }
    return 0;
}

int CategoryMetaManager::upCategoryMetaList()
{
    MutexGuard mg(_mutexCategory);
    int curNum = 0,nextNum = 0;
    {
        ReadLock rl(_rwLockCategory);
        curNum = _curMap;
    }
    nextNum = (curNum + 1) % 2;
    TC_Mysql mysql;
	try
	{
        _mpIdName[nextNum].clear();
        {
            TC_DBConf conf;
            conf.loadFromMap(DEF_CFG_SINGLETON->_dbConfAndroidApp);
            mysql.init(conf);
            mysql.connect();
            map<int64_t, string> mpTemp;
            TC_Mysql::MysqlData data = mysql.queryRecord("select category_id, name from t_category_meta_info");
            for (size_t i = 0; i < data.size(); ++i)
            {
                TC_Mysql::MysqlRecord record = data[i];
                _mpIdName[nextNum][TC_Common::strto<int64_t>(record["category_id"])] = record["name"];;
            }

            DEBUGLOG("load t_category_meta_info suc|" << data.size() << "|" << mpTemp.size() << "|" << endl);
        }

		{
			WriteLock wl(_rwLockCategory);
			_curMap = nextNum;
		}
	}
	catch (TC_Mysql_Exception& e)
	{
		ERRORLOG("Query database exception: " << e.what() << "|" << e.getErrCode() << endl);
		return -1;
	}
	catch (...)
	{
		ERRORLOG("Query database exception, unknown error occured. " << endl);
		return -1;
	}

	mysql.disconnect();

	return 0;
}

void AsyncUpdateCategoryMetaInfoTask::operator()()
{
	while (1)
	{
		CategoryMetaManagerSingleton->upCategoryMetaList();
		sleep(rand()%DEF_CFG_SINGLETON->_cacheUpTime);
	}
}
