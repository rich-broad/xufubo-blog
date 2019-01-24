/*********************************************************************************
 FileName: 
 Author: xufubo
 Date:  2019-01-01
 Description:
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/

#ifndef APP_MANAGER_H
#define	APP_MANAGER_H

#include "Global.h"
#include "AppletMallCommon.h"
#include "Configuration.h"
#include "DependencedServiceFactory.h"


class CategoryMetaManager
{
public:
	CategoryMetaManager();
	virtual ~CategoryMetaManager();

	static CategoryMetaManager* GetInstance();
	int initialize();
	int upCategoryMetaList();
	int getCategoryInfo(long id, string &name);

private:
	CategoryMetaManager(const CategoryMetaManager&) {};
	void operator=(const CategoryMetaManager&) {};

	static CategoryMetaManager* volatile _pInstance;
	static TC_ThreadMutex _instanceMutex;
	static tars::TC_ThreadRWLocker _rwLockCategory;
    static tars::TC_ThreadMutex _mutexCategory;

	//data
	map<int64_t, string> _mpIdName[2];
	int32_t _curMap;

};

class AsyncUpdateCategoryMetaInfoTask
{
public:
	AsyncUpdateCategoryMetaInfoTask() {}
	void operator()();

};

#define CategoryMetaManagerSingleton CategoryMetaManager::GetInstance()

#endif	/* APP_MANAGER_H */
