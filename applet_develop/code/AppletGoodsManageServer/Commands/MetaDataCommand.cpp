/*********************************************************************************
 FileName: MetaDataCommand.cpp
 Author: xufubo
 Date:  2019-01-01
 Description:
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/

#include "MetaDataCommand.h"

MetaDataCommand::MetaDataCommand(ConfigurationFactory* configurationFactory, DependencedServiceFactory* serviceFactory, TC_Mysql * mysql)
	:Command(configurationFactory, serviceFactory), _mysql(mysql)
{
    /*
    TC_DBConf conf;
    conf.loadFromMap(DEF_CFG_SINGLETON->_dbConfAndroidApp);
    try
    {
        _pMysql.init(conf);
        _pMysql.connect();
    }
    __CATCH_EXCEPTION_WITH__("MetaDataCommand condition mysql");
    */
}

MetaDataCommand::~MetaDataCommand()
{
    //_pMysql.disconnect();
}

int MetaDataCommand::getCategoryList(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::GetCategoryListReq& stReq, HardwareApplet::GetCategoryListRsp& stRsp, tars::TarsCurrentPtr current)
{
    ostringstream ossStr;
    ossStr << stReq.level;
    string funcName("getCategoryList");
    DEBUGLOG(COMM_HEAD_ALL_INFO(stHead) << "|" << ossStr.str() << endl;);
    int ret = -1;
    // 用于回包
    HardwareApplet::GetCategoryListRsp response;
    try
    {
        ret = CategoryMetaManagerSingleton->getCategoryList(stReq.level, response.mpCategory);
        if (ret != 0)
        {
            ERRORLOG("CategoryMetaManagerSingleton->getCategoryList error|" << ret << "|" << ossStr.str());
            response.ret = -1;
            sendReponse(response, stHead, funcName, ret, current);
            return -1;
        }
        response.ret = 0;
        sendReponse(response, stHead, funcName, ret, current);
        ret = 0;
    }
    __CATCH_EXCEPTION_WITH__(funcName);
    if (ret != 0)
    {
        //抛出异常也得回包
        sendReponse(response, stHead, funcName, ret, current);
    }
    
    return ret;
}

int MetaDataCommand::addCategoryInfo(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::AddCategoryInfoReq& stReq, HardwareApplet::AddCategoryInfoRsp& stRsp, tars::TarsCurrentPtr current)
{
    ostringstream ossStr;
    string funcName("addCategoryInfo");
    DEBUGLOG(COMM_HEAD_ALL_INFO(stHead) << "|" << ossStr.str() << endl);
    int ret = -1;
    HardwareApplet::AddCategoryInfoRsp response;
    try
    {
        map<string, pair<TC_Mysql::FT, string> > record;
        record["create_time"] = make_pair(TC_Mysql::DB_INT, TNOW);
        record["name"] = make_pair(TC_Mysql::DB_STR, stReq.item.categoryName);
        record["desc"] = make_pair(TC_Mysql::DB_STR, stReq.item.desc);
        record["parent_id"] = make_pair(TC_Mysql::DB_INT, stReq.item.parentId);
        record["level"] = make_pair(TC_Mysql::DB_INT, stReq.item.level);
        size_t affected_rows = _mysql->insertRecord("t_category_meta_info", record);
        if (affected_rows != 1)
        {
            ERRORLOG("inert error. affected_rows|" << affected_rows << endl);
            response.ret = -1;
            response.errmsg = "insert into db failed";
            sendReponse(response, stHead, funcName, ret, current);
            return -1;
        }
        response.ret = 0;
        sendReponse(response, stHead, funcName, ret, current);
        ret = 0;
    }
    __CATCH_EXCEPTION_WITH__(funcName);
    if (ret != 0)
    {
        //抛出异常
        sendReponse(response, stHead, funcName, ret, current);
    }
    return ret;
}
int MetaDataCommand::getAttributeList(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::GetAttributeListReq& stReq, HardwareApplet::GetAttributeListRsp& stRsp, tars::TarsCurrentPtr current)
{
    ostringstream ossStr;
    string funcName("getAttributeList");
    DEBUGLOG(COMM_HEAD_ALL_INFO(stHead) << "|" << ossStr.str() << endl);
    int ret = -1;
    HardwareApplet::GetAttributeListRsp response;
    try
    {
        ret = AttributeMetaManagerSingleton->getAttributeList(response.itemList);
        if (ret != 0)
        {
            ERRORLOG("AttributeMetaManagerSingleton->getAttributeList error|" << ret << "|" << ossStr.str());
            response.ret = -1;
            sendReponse(response, stHead, funcName, ret, current);
            return -1;
        }
        response.ret = 0;
        sendReponse(response, stHead, funcName, ret, current);
        ret = 0;
    }
    __CATCH_EXCEPTION_WITH__(funcName);
    if (ret != 0)
    {
        //抛出异常也得回包
        sendReponse(response, stHead, funcName, ret, current);
    }
    return ret;
}
int MetaDataCommand::addAttributeInfo(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::AddAttributeInfoReq& stReq, HardwareApplet::AddAttributeInfoRsp& stRsp, tars::TarsCurrentPtr current)
{
    ostringstream ossStr;
    string funcName("addAttributeInfo");
    DEBUGLOG(COMM_HEAD_ALL_INFO(stHead) << "|" << ossStr.str() << endl);
    int ret = -1;
    HardwareApplet::AddAttributeInfoRsp response;
    try
    {
        map<string, pair<TC_Mysql::FT, string> > record;
        record["create_time"] = make_pair(TC_Mysql::DB_INT, TNOW);
        record["name"] = make_pair(TC_Mysql::DB_STR, stReq.item.attrName);
        record["desc"] = make_pair(TC_Mysql::DB_STR, stReq.item.attrDesc);
        size_t affected_rows = _mysql->insertRecord("t_attribute_meta_info", record);
        if (affected_rows != 1)
        {
            ERRORLOG("inert error. affected_rows|" << affected_rows << endl);
            response.ret = -1;
            response.errmsg = "insert into db failed";
            sendReponse(response, stHead, funcName, ret, current);
            return -1;
        }
        response.ret = 0;
        sendReponse(response, stHead, funcName, ret, current);
        ret = 0;
    }
    __CATCH_EXCEPTION_WITH__(funcName);
    if (ret != 0)
    {
        //抛出异常也得回包
        sendReponse(response, stHead, funcName, ret, current);
    }
    return ret;
}

int MetaDataCommand::getAttributeValueList(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::GetAttributeValueListReq& stReq, HardwareApplet::GetAttributeValueListRsp& stRsp, tars::TarsCurrentPtr current)
{
    ostringstream ossStr;
    string funcName("getAttributeValueList");
    DEBUGLOG(COMM_HEAD_ALL_INFO(stHead) << "|" << ossStr.str() << endl);
    int ret = -1;
    HardwareApplet::GetAttributeValueListRsp response;
    try
    {
        ret = AttrValueMetaManagerSingleton->getAttrValueList(stReq.attrId, response.mpAttrValue);
        if (ret != 0)
        {
            ERRORLOG("AttrValueMetaManagerSingleton->getAttrValueList error|" << ret << "|" << ossStr.str());
            response.ret = -1;
            sendReponse(response, stHead, funcName, ret, current);
            return -1;
        }
        response.ret = 0;
        sendReponse(response, stHead, funcName, ret, current);
        ret = 0;
    }
    __CATCH_EXCEPTION_WITH__(funcName);
    if (ret != 0)
    {
        sendReponse(response, stHead, funcName, ret, current);
    }
    
    return ret;
}

int MetaDataCommand::addAttributeValueInfo(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::AddAttributeValueInfoReq& stReq, HardwareApplet::AddAttributeValueInfoRsp& stRsp, tars::TarsCurrentPtr current)
{
    ostringstream ossStr;
    string funcName("addAttributeValueInfo");
    DEBUGLOG(COMM_HEAD_ALL_INFO(stHead) << "|" << ossStr.str() << endl);
    int ret = -1;
    HardwareApplet::AddAttributeValueInfoRsp response;
    try
    {
        map<string, pair<TC_Mysql::FT, string> > record;
        record["create_time"] = make_pair(TC_Mysql::DB_INT, TNOW);
        record["name"] = make_pair(TC_Mysql::DB_STR, stReq.item.name);
        record["attr_id"] = make_pair(TC_Mysql::DB_INT, stReq.item.attrId);
        size_t affected_rows = _mysql->insertRecord("t_attribute_value_meta_info", record);
        if (affected_rows != 1)
        {
            ERRORLOG("inert error. affected_rows|" << affected_rows << endl);
            response.ret = -1;
            response.errmsg = "insert into db failed";
            sendReponse(response, stHead, funcName, ret, current);
            return -1;
        }
        response.ret = 0;
        sendReponse(response, stHead, funcName, ret, current);
        ret = 0;
    }
    __CATCH_EXCEPTION_WITH__(funcName);
    if (ret != 0)
    {
        sendReponse(response, stHead, funcName, ret, current);
    }
    
    return ret;
}
///
int MetaDataCommand::getBrandList(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::GetBrandListReq& stReq, HardwareApplet::GetBrandListRsp& stRsp, tars::TarsCurrentPtr current)
{
    ostringstream ossStr;
    string funcName("getBrandList");
    DEBUGLOG(COMM_HEAD_ALL_INFO(stHead) << "|" << ossStr.str() << endl);
    int ret = -1;
    HardwareApplet::GetBrandListRsp response;
    try
    {
        ret = BrandMetaManagerSingleton->getBrandList(response.itemList);
        if (ret != 0)
        {
            ERRORLOG("BrandMetaManagerSingleton->getBrandList error|" << ret << "|" << ossStr.str());
            response.ret = -1;
            sendReponse(response, stHead, funcName, ret, current);
            return -1;
        }
        response.ret = 0;
        sendReponse(response, stHead, funcName, ret, current);
        ret = 0;
    }
    __CATCH_EXCEPTION_WITH__(funcName);
    if (ret != 0)
    {
        sendReponse(response, stHead, funcName, ret, current);
    }
    
    return ret;
}

int MetaDataCommand::addBrandInfo(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::AddBrandInfoReq& stReq, HardwareApplet::AddBrandInfoRsp& stRsp, tars::TarsCurrentPtr current)
{
    ostringstream ossStr;
    string funcName("addBrandInfo");
    DEBUGLOG(COMM_HEAD_ALL_INFO(stHead) << "|" << ossStr.str() << endl);
    int ret = -1;
    HardwareApplet::AddBrandInfoRsp response;
    try
    {
        map<string, pair<TC_Mysql::FT, string> > record;
        record["create_time"] = make_pair(TC_Mysql::DB_INT, TNOW);
        record["name"] = make_pair(TC_Mysql::DB_STR, stReq.item.brandName);
        record["desc"] = make_pair(TC_Mysql::DB_STR, stReq.item.brandDesc);
        size_t affected_rows = _mysql->insertRecord("t_brand_info", record);
        if (affected_rows != 1)
        {
            ERRORLOG("inert error. affected_rows|" << affected_rows << endl);
            response.ret = -1;
            response.errmsg = "insert into db failed";
            sendReponse(response, stHead, funcName, ret, current);
            return -1;
        }
        response.ret = 0;
        sendReponse(response, stHead, funcName, ret, current);
        ret = 0;
    }
    __CATCH_EXCEPTION_WITH__(funcName);
    if (ret != 0)
    {
        sendReponse(response, stHead, funcName, ret, current);
    }
    
    return ret;
}

int MetaDataCommand::getMakerList(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::GetMakerListReq& stReq, HardwareApplet::GetMakerListRsp& stRsp, tars::TarsCurrentPtr current)
{
    ostringstream ossStr;
    string funcName("getMakerList");
    DEBUGLOG(COMM_HEAD_ALL_INFO(stHead) << "|" << ossStr.str() << endl);
    int ret = -1;
    HardwareApplet::GetMakerListRsp response;
    try
    {
        ret = MakerMetaManagerSingleton->getMakerList(response.itemList);
        if (ret != 0)
        {
            ERRORLOG("MakerMetaManagerSingleton->getMakerList error|" << ret << "|" << ossStr.str());
            response.ret = -1;
            sendReponse(response, stHead, funcName, ret, current);
            return -1;
        }
        response.ret = 0;
        sendReponse(response, stHead, funcName, ret, current);
        ret = 0;
    }
    __CATCH_EXCEPTION_WITH__(funcName);
    if (ret != 0)
    {
        sendReponse(response, stHead, funcName, ret, current);
    }
    
    return ret;
}

int MetaDataCommand::addMakerInfo(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::AddMakerInfoReq& stReq, HardwareApplet::AddMakerInfoRsp& stRsp, tars::TarsCurrentPtr current)
{
    ostringstream ossStr;
    string funcName("addMakerInfo");
    DEBUGLOG(COMM_HEAD_ALL_INFO(stHead) << "|" << ossStr.str() << endl);
    int ret = -1;
    HardwareApplet::AddMakerInfoRsp response;
    try
    {
        map<string, pair<TC_Mysql::FT, string> > record;
        record["create_time"] = make_pair(TC_Mysql::DB_INT, TNOW);
        record["name"] = make_pair(TC_Mysql::DB_STR, stReq.item.makerName);
        record["desc"] = make_pair(TC_Mysql::DB_STR, stReq.item.makerDesc);
        size_t affected_rows = _mysql->insertRecord("t_maker_info", record);
        if (affected_rows != 1)
        {
            ERRORLOG("inert error. affected_rows|" << affected_rows << endl);
            response.ret = -1;
            response.errmsg = "insert into db failed";
            sendReponse(response, stHead, funcName, ret, current);
            return -1;
        }
        response.ret = 0;
        sendReponse(response, stHead, funcName, ret, current);
        ret = 0;
    }
    __CATCH_EXCEPTION_WITH__(funcName);
    if (ret != 0)
    {
        sendReponse(response, stHead, funcName, ret, current);
    }
    
    return ret;
}


/*

void OpCommandCallback::callback_getAppList(taf::Int32 ret, const vector<taf::Char>& vtOut)
{
	HardwareApplet::GetAppListRsp response;
	HardwareApplet::GetAppListResponse stRsp;
	int decodeRet = JceDecode<HardwareApplet::GetAppListResponse>(vtOut, stRsp);
	if (decodeRet != 0)
	{
		ret = DECODE_ERR;
	}
	else if (ret == 0)
	{
		for (uint32_t i = 0; i < stRsp.appList.size(); ++i)
		{
			HardwareApplet::CardItem & ci = stRsp.appList[i];
			HardwareApplet::ListItem li;
			CardItem2MAAccessListItem(ci, li, _stHead.phoneGuid);
			response.appList.push_back(li);
		}
		response.contextData = stRsp.pageContext;
		response.hasNext = stRsp.hasNext;
		response.ret = stRsp.ret;
	}

	vector<char> vtResponse;
	if (ret == 0 && JceEncode<HardwareApplet::GetAppListRsp>(response, vtResponse) != 0)
	{
		ret = ENCODE_ERR;
	}
	CommonExternal::async_response_getAppList(_current, ret, vtResponse);
	TRLOG << "Res|" << _stHead.qua << "|" << _stHead.phoneGuid << "|" << ret << "|" << _stGetAppListReq.listType << "|" << _stGetAppListReq.pageSize << "|" << response.hasNext << "|" << response.appList.size() << response.contextData.size() << endl;
	DAYLOG << "getAppList|Res|" << _stHead.qua << "|" << _stHead.phoneGuid << "|" << ret << "|" << _stGetAppListReq.listType << "|" << _stGetAppListReq.pageSize << "|" << response.hasNext << "|" << response.appList.size() << response.contextData.size() << endl;
}

void OpCommandCallback::callback_getAppList_exception(taf::Int32 ret)
{
	vector<char> vtOut;
	CommonExternal::async_response_getAppList(_current, ret, vtOut);
	TRLOG << "Res|exception|" << ret << "|" << _stHead.qua << "|" << _stHead.phoneGuid << "|" << ret << "|" << _stGetAppListReq.listType << "|" << _stGetAppListReq.pageSize << endl;
	TERRLOG("getAppList|Res|exception|" << ret << "|" << _stHead.qua << "|" << _stHead.phoneGuid << "|" << ret << "|" << _stGetAppListReq.listType << "|" << _stGetAppListReq.pageSize) << endl;
}
*/