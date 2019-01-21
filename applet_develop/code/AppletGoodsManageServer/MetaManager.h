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
#include "AppletGoodsManage.h"
#include "Configuration.h"
#include "DependencedServiceFactory.h"

//=====================================分类元数据=====================================
class CategoryMetaManager
{
public:
	CategoryMetaManager();
	virtual ~CategoryMetaManager();

	static CategoryMetaManager* GetInstance();
	int initialize();
	int upCategoryList();
	int getCategoryList(const int level, map<int, vector<HardwareApplet::CategoryItem> > &mpResult);

private:
	CategoryMetaManager(const CategoryMetaManager&) {};
	void operator=(const CategoryMetaManager&) {};

	static CategoryMetaManager* volatile _pInstance;
	static TC_ThreadMutex _instanceMutex;
	static tars::TC_ThreadRWLocker _rwLockCategory;
    static tars::TC_ThreadMutex _mutexCategory;

	//data k=levet
	map<int, vector<HardwareApplet::CategoryItem> > _mpCategory[2];
	int32_t _curMap;

};

class AsyncUpdateCategoryMetaInfoTask
{
public:
	AsyncUpdateCategoryMetaInfoTask() {}
	void operator()();

};

#define CategoryMetaManagerSingleton CategoryMetaManager::GetInstance()


//=====================================属性元数据=====================================
class AttributeMetaManager
{
public:
    AttributeMetaManager();
    virtual ~AttributeMetaManager();

    static AttributeMetaManager* GetInstance();
    int initialize();
    int upAttributeList();
    int getAttributeInfo(long id, string &name);

private:
    AttributeMetaManager(const AttributeMetaManager&) {};
    void operator=(const AttributeMetaManager&) {};

    static AttributeMetaManager* volatile _pInstance;
    static TC_ThreadMutex _instanceMutex;
    static tars::TC_ThreadRWLocker _rwLockAttribute;
    static tars::TC_ThreadMutex _mutexAttribute;

    //data
    map<int64_t, string> _mpIdName[2];
    int32_t _curMap;

};

class AsyncUpdateAttributeMetaInfoTask
{
public:
    AsyncUpdateAttributeMetaInfoTask() {}
    void operator()();

};

#define AttributeMetaManagerSingleton AttributeMetaManager::GetInstance()


//=====================================属性值元数据=====================================
class AttrValueMetaManager
{
public:
    AttrValueMetaManager();
    virtual ~AttrValueMetaManager();

    static AttrValueMetaManager* GetInstance();
    int initialize();
    int upAttrValueList();
    int getAttrValueInfo(long id, string &name);

private:
    AttrValueMetaManager(const AttrValueMetaManager&) {};
    void operator=(const AttrValueMetaManager&) {};

    static AttrValueMetaManager* volatile _pInstance;
    static TC_ThreadMutex _instanceMutex;
    static tars::TC_ThreadRWLocker _rwLockAttrValue;
    static tars::TC_ThreadMutex _mutexAttrValue;

    //data
    map<int64_t, string> _mpIdName[2];
    int32_t _curMap;

};

class AsyncUpdateAttrValueMetaInfoTask
{
public:
    AsyncUpdateAttrValueMetaInfoTask() {}
    void operator()();

};

#define AttrValueMetaManagerSingleton AttrValueMetaManager::GetInstance()


//=====================================品牌元数据=====================================
class BrandMetaManager
{
public:
    BrandMetaManager();
    virtual ~BrandMetaManager();

    static BrandMetaManager* GetInstance();
    int initialize();
    int upBrandList();
    int getBrandInfo(long id, string &name);

private:
    BrandMetaManager(const BrandMetaManager&) {};
    void operator=(const BrandMetaManager&) {};

    static BrandMetaManager* volatile _pInstance;
    static TC_ThreadMutex _instanceMutex;
    static tars::TC_ThreadRWLocker _rwLockBrand;
    static tars::TC_ThreadMutex _mutexBrand;

    //data
    map<int64_t, string> _mpIdName[2];
    int32_t _curMap;

};

class AsyncUpdateBrandMetaInfoTask
{
public:
    AsyncUpdateBrandMetaInfoTask() {}
    void operator()();

};

#define BrandMetaManagerSingleton BrandMetaManager::GetInstance()


//====================================厂商元数据=====================================
class MakerMetaManager
{
public:
    MakerMetaManager();
    virtual ~MakerMetaManager();

    static MakerMetaManager* GetInstance();
    int initialize();
    int upMakerList();
    int getMakerInfo(long id, string &name);

private:
    MakerMetaManager(const MakerMetaManager&) {};
    void operator=(const MakerMetaManager&) {};

    static MakerMetaManager* volatile _pInstance;
    static TC_ThreadMutex _instanceMutex;
    static tars::TC_ThreadRWLocker _rwLockMaker;
    static tars::TC_ThreadMutex _mutexMaker;

    //data
    map<int64_t, string> _mpIdName[2];
    int32_t _curMap;

};

class AsyncUpdateMakerMetaInfoTask
{
public:
    AsyncUpdateMakerMetaInfoTask() {}
    void operator()();

};

#define MakerMetaManagerSingleton MakerMetaManager::GetInstance()

#endif	/* APP_MANAGER_H */
