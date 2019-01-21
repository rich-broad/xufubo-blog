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

MetaDataCommand::MetaDataCommand(ConfigurationFactory* configurationFactory, DependencedServiceFactory* serviceFactory)
	:Command(configurationFactory, serviceFactory)
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
        }
        sendReponse(response, stHead, "getCategoryList", ret, current);
    }
    __CATCH_EXCEPTION_WITH__("MetaDataCommand::getCategoryList");
    if (ret != 0)
    {
        //抛出异常也得回包
        sendReponse(response, stHead, "getCategoryList", ret, current);
    }
    
    return ret;
}

int MetaDataCommand::addCategoryInfo(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::AddCategoryInfoReq& stReq, HardwareApplet::AddCategoryInfoRsp& stRsp, tars::TarsCurrentPtr current)
{
    ostringstream ossStr;
    DEBUGLOG(COMM_HEAD_ALL_INFO(stHead) << "|" << ossStr.str() << endl);
    int ret = -1;
    HardwareApplet::AddCategoryInfoRsp response;
    try
    {
        
        sendReponse(response, stHead, "addCategoryInfo", ret, current);
    }
    __CATCH_EXCEPTION_WITH__("MetaDataCommand::addCategoryInfo");
    if (ret != 0)
    {
        //抛出异常也得回包
        sendReponse(response, stHead, "addCategoryInfo", ret, current);
    }
    return ret;
}
int MetaDataCommand::getAttributeList(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::GetAttributeListReq& stReq, HardwareApplet::GetAttributeListRsp& stRsp, tars::TarsCurrentPtr current)
{
    ostringstream ossStr;
    DEBUGLOG(COMM_HEAD_ALL_INFO(stHead) << "|" << ossStr.str() << endl);
    int ret = -1;
    HardwareApplet::GetAttributeListRsp response;
    try
    {
        sendReponse(response, stHead, "getAttributeList", ret, current);
    }
    __CATCH_EXCEPTION_WITH__("MetaDataCommand::getAttributeList");
    if (ret != 0)
    {
        //抛出异常也得回包
        sendReponse(response, stHead, "getAttributeList", ret, current);
    }
    return ret;
}
int MetaDataCommand::addAttributeInfo(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::AddAttributeInfoReq& stReq, HardwareApplet::AddAttributeInfoRsp& stRsp, tars::TarsCurrentPtr current)
{
    ostringstream ossStr;
    DEBUGLOG(COMM_HEAD_ALL_INFO(stHead) << "|" << ossStr.str() << endl);
    int ret = -1;
    HardwareApplet::AddAttributeInfoRsp response;
    try
    {
        sendReponse(response, stHead, "addAttributeInfo", ret, current);
    }
    __CATCH_EXCEPTION_WITH__("MetaDataCommand::addAttributeInfo");
    if (ret != 0)
    {
        //抛出异常也得回包
        sendReponse(response, stHead, "addAttributeInfo", ret, current);
    }
    return ret;
}

int MetaDataCommand::getAttributeValueList(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::GetAttributeValueListReq& stReq, HardwareApplet::GetAttributeValueListRsp& stRsp, tars::TarsCurrentPtr current)
{
    ostringstream ossStr;

    DEBUGLOG(COMM_HEAD_ALL_INFO(stHead) << "|" << ossStr.str() << endl);
    int ret = -1;
    HardwareApplet::GetAttributeValueListRsp response;
    try
    {
        sendReponse(response, stHead, "getAttributeValueList", ret, current);
    }
    __CATCH_EXCEPTION_WITH__("MetaDataCommand::getAttributeValueList");
    if (ret != 0)
    {
        sendReponse(response, stHead, "getAttributeValueList", ret, current);
    }
    
    return ret;
}

int MetaDataCommand::addAttributeValueInfo(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::AddAttributeValueInfoReq& stReq, HardwareApplet::AddAttributeValueInfoRsp& stRsp, tars::TarsCurrentPtr current)
{
    ostringstream ossStr;

    DEBUGLOG(COMM_HEAD_ALL_INFO(stHead) << "|" << ossStr.str() << endl);
    int ret = -1;
    HardwareApplet::AddAttributeValueInfoRsp response;
    try
    {
        sendReponse(response, stHead, "addAttributeValueInfo", ret, current);
    }
    __CATCH_EXCEPTION_WITH__("MetaDataCommand::addAttributeValueInfo");
    if (ret != 0)
    {
        sendReponse(response, stHead, "addAttributeValueInfo", ret, current);
    }
    
    return ret;
}

int MetaDataCommand::getBrandList(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::GetBrandListReq& stReq, HardwareApplet::GetBrandListRsp& stRsp, tars::TarsCurrentPtr current)
{
    ostringstream ossStr;
    DEBUGLOG(COMM_HEAD_ALL_INFO(stHead) << "|" << ossStr.str() << endl);
    int ret = -1;
    HardwareApplet::GetBrandListRsp response;
    try
    {
        sendReponse(response, stHead, "getBrandList", ret, current);
    }
    __CATCH_EXCEPTION_WITH__("MetaDataCommand::getBrandList");
    if (ret != 0)
    {
        sendReponse(response, stHead, "getBrandList", ret, current);
    }
    
    return ret;
}

int MetaDataCommand::addBrandInfo(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::AddBrandInfoReq& stReq, HardwareApplet::AddBrandInfoRsp& stRsp, tars::TarsCurrentPtr current)
{
    ostringstream ossStr;
    DEBUGLOG(COMM_HEAD_ALL_INFO(stHead) << "|" << ossStr.str() << endl);
    int ret = -1;
    HardwareApplet::AddBrandInfoRsp response;
    try
    {
        sendReponse(response, stHead, "addBrandInfo", ret, current);
    }
    __CATCH_EXCEPTION_WITH__("MetaDataCommand::addBrandInfo");
    if (ret != 0)
    {
        sendReponse(response, stHead, "addBrandInfo", ret, current);
    }
    
    return ret;
}

int MetaDataCommand::getMakerList(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::GetMakerListReq& stReq, HardwareApplet::GetMakerListRsp& stRsp, tars::TarsCurrentPtr current)
{
    ostringstream ossStr;
    DEBUGLOG(COMM_HEAD_ALL_INFO(stHead) << "|" << ossStr.str() << endl);
    int ret = -1;
    HardwareApplet::GetMakerListRsp response;
    try
    {
        sendReponse(response, stHead, "getMakerList", ret, current);
    }
    __CATCH_EXCEPTION_WITH__("MetaDataCommand::getMakerList");
    if (ret != 0)
    {
        sendReponse(response, stHead, "getMakerList", ret, current);
    }
    
    return ret;
}

int MetaDataCommand::addMakerInfo(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::AddMakerInfoReq& stReq, HardwareApplet::AddMakerInfoRsp& stRsp, tars::TarsCurrentPtr current)
{
    ostringstream ossStr;
    DEBUGLOG(COMM_HEAD_ALL_INFO(stHead) << "|" << ossStr.str() << endl);
    int ret = -1;
    HardwareApplet::AddMakerInfoRsp response;
    try
    {
        sendReponse(response, stHead, "addMakerInfo", ret, current);
    }
    __CATCH_EXCEPTION_WITH__("MetaDataCommand::addMakerInfo");
    if (ret != 0)
    {
        sendReponse(response, stHead, "addMakerInfo", ret, current);
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