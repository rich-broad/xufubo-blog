/*********************************************************************************
 FileName: 
 Author: xufubo
 Date:  2019-01-01
 Description:
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/

#ifndef META_MANAGER_H
#define	META_MANAGER_H

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
    int getAttributeList(vector<HardwareApplet::AttributeItem> &vtResult);

private:
    AttributeMetaManager(const AttributeMetaManager&) {};
    void operator=(const AttributeMetaManager&) {};

    static AttributeMetaManager* volatile _pInstance;
    static TC_ThreadMutex _instanceMutex;
    static tars::TC_ThreadRWLocker _rwLockAttribute;
    static tars::TC_ThreadMutex _mutexAttribute;

    //data
    vector<HardwareApplet::AttributeItem> _vtAttr[2];
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
    int getAttrValueList(const int id, map<int, vector<HardwareApplet::AttributeValueItem> > &mpResult);

private:
    AttrValueMetaManager(const AttrValueMetaManager&) {};
    void operator=(const AttrValueMetaManager&) {};

    static AttrValueMetaManager* volatile _pInstance;
    static TC_ThreadMutex _instanceMutex;
    static tars::TC_ThreadRWLocker _rwLockAttrValue;
    static tars::TC_ThreadMutex _mutexAttrValue;

    //data
    map<int, vector<HardwareApplet::AttributeValueItem> > _mpAttrValue[2];
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
    int getBrandList(vector<HardwareApplet::BrandItem> &vtResult);

private:
    BrandMetaManager(const BrandMetaManager&) {};
    void operator=(const BrandMetaManager&) {};

    static BrandMetaManager* volatile _pInstance;
    static TC_ThreadMutex _instanceMutex;
    static tars::TC_ThreadRWLocker _rwLockBrand;
    static tars::TC_ThreadMutex _mutexBrand;

    //data
    vector<HardwareApplet::BrandItem> _vtBrand[2];
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
    int getMakerList(vector<HardwareApplet::MakerItem> &vtResult);

private:
    MakerMetaManager(const MakerMetaManager&) {};
    void operator=(const MakerMetaManager&) {};

    static MakerMetaManager* volatile _pInstance;
    static TC_ThreadMutex _instanceMutex;
    static tars::TC_ThreadRWLocker _rwLockMaker;
    static tars::TC_ThreadMutex _mutexMaker;

    //data
    vector<HardwareApplet::MakerItem> _vtMaker[2];
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
