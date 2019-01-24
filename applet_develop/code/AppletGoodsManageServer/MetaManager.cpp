/*********************************************************************************
 FileName: 
 Author: xufubo
 Date:  2019-01-01
 Description:
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/

#include "MetaManager.h"
#include "Configuration.h"

//=====================================分类元数据=====================================
//////////////////////////////////////////////////////////////////////////
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
	upCategoryList();
	return 0;
}

int CategoryMetaManager::getCategoryList(const int level, map<int, vector<HardwareApplet::CategoryItem> > &mpResult)
{
    int curNum = 0;
    {
        ReadLock rl(_rwLockCategory);
        curNum = _curMap;
    }

    if (0 == level)
    {
        mpResult = _mpCategory[curNum];
        return 0;
    }
    
    if (_mpCategory[curNum].find(level) != _mpCategory[curNum].end())
    {
        mpResult[level] = _mpCategory[curNum][level];
        return 0;
    }
    ERRORLOG("level|" << level << "|not find|" << endl);
    return -1;
}

int CategoryMetaManager::upCategoryList()
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
        _mpCategory[nextNum].clear();
        {
            TC_DBConf conf;
            conf.loadFromMap(DEF_CFG_SINGLETON->_dbInfoConf);
            mysql.init(conf);
            mysql.connect();
            TC_Mysql::MysqlData data = mysql.queryRecord("select category_id, name, parent_id, desc, level, img_url from t_category_meta_info");
            for (size_t i = 0; i < data.size(); ++i)
            {
                HardwareApplet::CategoryItem item;
                TC_Mysql::MysqlRecord record = data[i];
                item.categoryId = TC_Common::strto<int32_t>(record["category_id"]);
                item.categoryName = record["name"];
                item.parentId = TC_Common::strto<int32_t>(record["parent_id"]);
                item.level =  TC_Common::strto<int32_t>(record["level"]);
                item.desc = record["desc"];
                item.imgUrl = record["img_url"];
                _mpCategory[nextNum][item.level].push_back(item);
            }

            DEBUGLOG("load t_category_meta_info suc|" << data.size() << "|" << _mpCategory[nextNum].size() << "|" << endl);
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
		CategoryMetaManagerSingleton->upCategoryList();
		sleep(rand()%DEF_CFG_SINGLETON->_cacheUpTime);
	}
}


//=====================================属性元数据=====================================
//////////////////////////////////////////////////////////////////////////
AttributeMetaManager* volatile AttributeMetaManager::_pInstance = NULL;
tars::TC_ThreadMutex AttributeMetaManager::_instanceMutex;
tars::TC_ThreadRWLocker AttributeMetaManager::_rwLockAttribute;
tars::TC_ThreadMutex AttributeMetaManager::_mutexAttribute;

AttributeMetaManager* AttributeMetaManager::GetInstance()
{
    if (_pInstance == NULL)
    {
        MutexGuard mg(_instanceMutex);
        if (_pInstance == NULL)
        {
            AttributeMetaManager* volatile tmp = new AttributeMetaManager();
            _pInstance = tmp;
        }
    }
    return _pInstance;
}

AttributeMetaManager::AttributeMetaManager()
{
    _curMap = 0;
}

AttributeMetaManager::~AttributeMetaManager()
{

}

int AttributeMetaManager::initialize()
{
    upAttributeList();
    return 0;
}

int AttributeMetaManager::getAttributeList(vector<HardwareApplet::AttributeItem> &vtResult)
{
    int curNum = 0;
    {
        ReadLock rl(_rwLockAttribute);
        curNum = _curMap;
    }
    vtResult = _vtAttr[curNum];
    return 0;
}

int AttributeMetaManager::upAttributeList()
{
    MutexGuard mg(_mutexAttribute);
    int curNum = 0,nextNum = 0;
    {
        ReadLock rl(_rwLockAttribute);
        curNum = _curMap;
    }
    nextNum = (curNum + 1) % 2;
    TC_Mysql mysql;
    try
    {
        _vtAttr[nextNum].clear();
        {
            TC_DBConf conf;
            conf.loadFromMap(DEF_CFG_SINGLETON->_dbInfoConf);
            mysql.init(conf);
            mysql.connect();
            TC_Mysql::MysqlData data = mysql.queryRecord("select attr_id, name, desc from t_attribute_meta_info");
            for (size_t i = 0; i < data.size(); ++i)
            {
                HardwareApplet::AttributeItem item;
                TC_Mysql::MysqlRecord record = data[i];
                item.attrId = TC_Common::strto<int32_t>(record["attr_id"]);
                item.attrName = record["name"];
                item.attrDesc = record["desc"];
                _vtAttr[nextNum].push_back(item);
            }

            DEBUGLOG("load t_attribute_meta_info suc|" << data.size() << "|" << _vtAttr[nextNum].size() << "|" << endl);
        }

        {
            WriteLock wl(_rwLockAttribute);
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

void AsyncUpdateAttributeMetaInfoTask::operator()()
{
    while (1)
    {
        AttributeMetaManagerSingleton->upAttributeList();
        sleep(rand()%DEF_CFG_SINGLETON->_cacheUpTime);
    }
}


//=====================================属性值元数据=====================================
//////////////////////////////////////////////////////////////////////////
AttrValueMetaManager* volatile AttrValueMetaManager::_pInstance = NULL;
tars::TC_ThreadMutex AttrValueMetaManager::_instanceMutex;
tars::TC_ThreadRWLocker AttrValueMetaManager::_rwLockAttrValue;
tars::TC_ThreadMutex AttrValueMetaManager::_mutexAttrValue;

AttrValueMetaManager* AttrValueMetaManager::GetInstance()
{
    if (_pInstance == NULL)
    {
        MutexGuard mg(_instanceMutex);
        if (_pInstance == NULL)
        {
            AttrValueMetaManager* volatile tmp = new AttrValueMetaManager();
            _pInstance = tmp;
        }
    }
    return _pInstance;
}

AttrValueMetaManager::AttrValueMetaManager()
{
    _curMap = 0;
}

AttrValueMetaManager::~AttrValueMetaManager()
{

}

int AttrValueMetaManager::initialize()
{
    upAttrValueList();
    return 0;
}

int AttrValueMetaManager::getAttrValueList(const int id, map<int, vector<HardwareApplet::AttributeValueItem> > &mpResult)
{
    int curNum = 0;
    {
        ReadLock rl(_rwLockAttrValue);
        curNum = _curMap;
    }

    if (0 == id)
    {
        mpResult = _mpAttrValue[curNum];
        return 0;
    }

    if (_mpAttrValue[curNum].find(id) != _mpAttrValue[curNum].end())
    {
        mpResult[id] = _mpAttrValue[curNum][id];
        return 0;
    }
    ERRORLOG("id|" << id << "|not find|" << endl);
    return -1;
}

int AttrValueMetaManager::upAttrValueList()
{
    MutexGuard mg(_mutexAttrValue);
    int curNum = 0,nextNum = 0;
    {
        ReadLock rl(_rwLockAttrValue);
        curNum = _curMap;
    }
    nextNum = (curNum + 1) % 2;
    TC_Mysql mysql;
    try
    {
        _mpAttrValue[nextNum].clear();
        {
            TC_DBConf conf;
            conf.loadFromMap(DEF_CFG_SINGLETON->_dbInfoConf);
            mysql.init(conf);
            mysql.connect();
            map<int64_t, string> mpTemp;
            TC_Mysql::MysqlData data = mysql.queryRecord("select attr_value_id, name, attr_id from t_attribute_value_meta_info");
            for (size_t i = 0; i < data.size(); ++i)
            {
                HardwareApplet::AttributeValueItem item;
                TC_Mysql::MysqlRecord record = data[i];
                item.attrValueId = TC_Common::strto<int32_t>(record["attr_value_id"]);
                item.name = record["name"];
                item.attrId = TC_Common::strto<int32_t>(record["attr_id"]);
                _mpAttrValue[nextNum][item.attrId].push_back(item);
            }

            DEBUGLOG("load t_attribute_value_meta_info suc|" << data.size() << "|" << _mpAttrValue[nextNum].size() << "|" << endl);
        }

        {
            WriteLock wl(_rwLockAttrValue);
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

void AsyncUpdateAttrValueMetaInfoTask::operator()()
{
    while (1)
    {
        AttrValueMetaManagerSingleton->upAttrValueList();
        sleep(rand()%DEF_CFG_SINGLETON->_cacheUpTime);
    }
}


//=====================================品牌元数据=====================================
//////////////////////////////////////////////////////////////////////////
BrandMetaManager* volatile BrandMetaManager::_pInstance = NULL;
tars::TC_ThreadMutex BrandMetaManager::_instanceMutex;
tars::TC_ThreadRWLocker BrandMetaManager::_rwLockBrand;
tars::TC_ThreadMutex BrandMetaManager::_mutexBrand;

BrandMetaManager* BrandMetaManager::GetInstance()
{
    if (_pInstance == NULL)
    {
        MutexGuard mg(_instanceMutex);
        if (_pInstance == NULL)
        {
            BrandMetaManager* volatile tmp = new BrandMetaManager();
            _pInstance = tmp;
        }
    }
    return _pInstance;
}

BrandMetaManager::BrandMetaManager()
{
    _curMap = 0;
}

BrandMetaManager::~BrandMetaManager()
{

}

int BrandMetaManager::initialize()
{
    upBrandList();
    return 0;
}

int BrandMetaManager::getBrandList(vector<HardwareApplet::BrandItem> &vtResult)
{
    int curNum = 0;
    {
        ReadLock rl(_rwLockBrand);
        curNum = _curMap;
    }
    vtResult = _vtBrand[curNum];
    return 0;
}

int BrandMetaManager::upBrandList()
{
    MutexGuard mg(_mutexBrand);
    int curNum = 0,nextNum = 0;
    {
        ReadLock rl(_rwLockBrand);
        curNum = _curMap;
    }
    nextNum = (curNum + 1) % 2;
    TC_Mysql mysql;
    try
    {
        _vtBrand[nextNum].clear();
        {
            TC_DBConf conf;
            conf.loadFromMap(DEF_CFG_SINGLETON->_dbInfoConf);
            mysql.init(conf);
            mysql.connect();
            TC_Mysql::MysqlData data = mysql.queryRecord("select brand_id, name, desc from t_brand_info");
            for (size_t i = 0; i < data.size(); ++i)
            {
                HardwareApplet::BrandItem item;
                TC_Mysql::MysqlRecord record = data[i];
                item.brandId = TC_Common::strto<int32_t>(record["brand_id"]);
                item.brandName = record["name"];
                item.brandDesc = record["desc"];
                _vtBrand[nextNum].push_back(item);
            }

            DEBUGLOG("load t_brand_info suc|" << data.size() << "|" << _vtBrand[nextNum].size() << "|" << endl);
        }

        {
            WriteLock wl(_rwLockBrand);
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

void AsyncUpdateBrandMetaInfoTask::operator()()
{
    while (1)
    {
        BrandMetaManagerSingleton->upBrandList();
        sleep(rand()%DEF_CFG_SINGLETON->_cacheUpTime);
    }
}


//=====================================厂商元数据=====================================
//////////////////////////////////////////////////////////////////////////
MakerMetaManager* volatile MakerMetaManager::_pInstance = NULL;
tars::TC_ThreadMutex MakerMetaManager::_instanceMutex;
tars::TC_ThreadRWLocker MakerMetaManager::_rwLockMaker;
tars::TC_ThreadMutex MakerMetaManager::_mutexMaker;

MakerMetaManager* MakerMetaManager::GetInstance()
{
    if (_pInstance == NULL)
    {
        MutexGuard mg(_instanceMutex);
        if (_pInstance == NULL)
        {
            MakerMetaManager* volatile tmp = new MakerMetaManager();
            _pInstance = tmp;
        }
    }
    return _pInstance;
}

MakerMetaManager::MakerMetaManager()
{
    _curMap = 0;
}

MakerMetaManager::~MakerMetaManager()
{

}

int MakerMetaManager::initialize()
{
    upMakerList();
    return 0;
}

int MakerMetaManager::getMakerList(vector<HardwareApplet::MakerItem> &vtResult)
{
    int curNum = 0;
    {
        ReadLock rl(_rwLockMaker);
        curNum = _curMap;
    }
    vtResult = _vtMaker[curNum];
    return 0;
}

int MakerMetaManager::upMakerList()
{
    MutexGuard mg(_mutexMaker);
    int curNum = 0,nextNum = 0;
    {
        ReadLock rl(_rwLockMaker);
        curNum = _curMap;
    }
    nextNum = (curNum + 1) % 2;
    TC_Mysql mysql;
    try
    {
        _vtMaker[nextNum].clear();
        {
            TC_DBConf conf;
            conf.loadFromMap(DEF_CFG_SINGLETON->_dbInfoConf);
            mysql.init(conf);
            mysql.connect();
            TC_Mysql::MysqlData data = mysql.queryRecord("select maker_id, name, desc from t_maker_info");
            for (size_t i = 0; i < data.size(); ++i)
            {
                HardwareApplet::MakerItem item;
                TC_Mysql::MysqlRecord record = data[i];
                item.makerId = TC_Common::strto<int32_t>(record["maker_id"]);
                item.makerName = record["name"];
                item.makerDesc = record["desc"];
                _vtMaker[nextNum].push_back(item);
            }

            DEBUGLOG("load t_brand_info suc|" << data.size() << "|" << _vtMaker[nextNum].size() << "|" << endl);
        }

        {
            WriteLock wl(_rwLockMaker);
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

void AsyncUpdateMakerMetaInfoTask::operator()()
{
    while (1)
    {
        MakerMetaManagerSingleton->upMakerList();
        sleep(rand()%DEF_CFG_SINGLETON->_cacheUpTime);
    }
}

