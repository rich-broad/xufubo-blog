/*********************************************************************************
 FileName: AddressCommand.cpp
 Author: xufubo
 Date:  2019-01-01
 Description:
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/

#include "AddressCommand.h"
#include "AppletMallCommonServer.h"

AddressCommand::AddressCommand(ConfigurationFactory* configurationFactory, DependencedServiceFactory* serviceFactory, TC_Mysql * mysql)
	:Command(configurationFactory, serviceFactory),_mysql(mysql)
{

}

AddressCommand::~AddressCommand()
{

}

int AddressCommand::addMyAddressInfo(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::AddMyAddressInfoReq& stReq, HardwareApplet::AddMyAddressInfoRsp& stRsp, tars::TarsCurrentPtr current)
{
    ostringstream ossStr;
    string funcName("addMyAddressInfo");
    DEBUGLOG_WITHHEAD(stHead, ossStr.str() << endl);
    int ret = -1;
    HardwareApplet::AddMyAddressInfoRsp response;
    if (stHead.sessionInfo.unionid.empty() || stHead.sessionInfo.uid <= 0 || stHead.sessionInfo.openid.empty())
    {
        ERRORLOG_WITHHEAD(stHead, "check sessionInfo error|");
        response.ret = -1;
        sendReponse(response, stHead, funcName, ret, current);
        return -1;
    }
    try
    {
        map<string, pair<TC_Mysql::FT, string> > record;
        record["create_time"] = make_pair(TC_Mysql::DB_INT, TNOW);
        record["uid"] = make_pair(TC_Mysql::DB_INT, stHead.sessionInfo.uid);
        record["phone_num"] = make_pair(TC_Mysql::DB_STR, stReq.addressInfo.phoneNum);
        record["province"] = make_pair(TC_Mysql::DB_STR, stReq.addressInfo.province);
        record["city"] = make_pair(TC_Mysql::DB_STR, stReq.addressInfo.city);
        record["county"] = make_pair(TC_Mysql::DB_STR, stReq.addressInfo.county);
        record["address_detail"] = make_pair(TC_Mysql::DB_STR, stReq.addressInfo.addressDetail);
        record["receiver"] = make_pair(TC_Mysql::DB_STR, stReq.addressInfo.receiver);
        record["union_id"] = make_pair(TC_Mysql::DB_STR, stHead.sessionInfo.unionid);
        size_t affected_rows = _mysql->insertRecord("t_user_address_info", record);
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

int AddressCommand::getMyAddressList(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::GetMyAddressListReq& stReq, HardwareApplet::GetMyAddressListRsp& stRsp, tars::TarsCurrentPtr current)
{
    ostringstream ossStr;
    ossStr << stReq.reserved_field;
    string funcName("getMyAddressList");
    DEBUGLOG_WITHHEAD(stHead, ossStr.str() << endl);
    int ret = -1;
    // 用于回包
    HardwareApplet::GetMyAddressListRsp response;
    if (stHead.sessionInfo.unionid.empty() || stHead.sessionInfo.uid <= 0 || stHead.sessionInfo.openid.empty())
    {
        ERRORLOG_WITHHEAD(stHead, "check sessionInfo error|");
        response.ret = -1;
        sendReponse(response, stHead, funcName, ret, current);
        return -1;
    }
    try
    {
        ostringstream sqlStr;
        sqlStr << "select address_id, uid, phone_num, province, city, county, address_detail, receiver from t_user_address_info "
            << " where union_id = " << "'" << _mysql->escapeString(stHead.sessionInfo.unionid) << "'" << " and uid = " << stHead.sessionInfo.uid;
        TC_Mysql::MysqlData data = _mysql->queryRecord(sqlStr.str());
        for (size_t i = 0; i < data.size(); ++i)
        {
            HardwareApplet::AddressInfo addressInfo;
            TC_Mysql::MysqlRecord record = data[i];
            addressInfo.addressId = TC_Common::strto<int>(record["address_id"]);
            addressInfo.phoneNum = record["phone_num"];
            addressInfo.province = record["province"];
            addressInfo.city = record["city"];
            addressInfo.county = record["county"];
            addressInfo.addressDetail = record["address_detail"];
            addressInfo.receiver = record["receiver"];
            addressInfo.uid = TC_Common::strto<int>(record["uid"]);
            response.infoList.push_back(addressInfo);
        }
        response.ret = response.infoList.empty() ? E_AMC_NEED_ADD_ADDRESS_INFO : 0;
        sendReponse(response, stHead, funcName, response.ret, current);
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

int AddressCommand::getProvinceList(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::GetProvinceListReq& stReq, HardwareApplet::GetProvinceListRsp& stRsp, tars::TarsCurrentPtr current)
{
    ostringstream ossStr;
    string funcName("getProvinceList");
    DEBUGLOG_WITHHEAD(stHead, ossStr.str() << endl);
    int ret = -1;
    // 用于回包
    HardwareApplet::GetProvinceListRsp response;
    try
    {
        
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

int AddressCommand::getCityListByProvince(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::GetCityListByProvinceReq& stReq, HardwareApplet::GetCityListByProvinceRsp& stRsp, tars::TarsCurrentPtr current)
{
    ostringstream ossStr;
    string funcName("getCityListByProvince");
    DEBUGLOG_WITHHEAD(stHead, ossStr.str() << endl;);
    int ret = -1;
    // 用于回包
    HardwareApplet::GetCityListByProvinceRsp response;
    try
    {

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

int AddressCommand::getCountyListByCity(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::GetCountyListByCityReq& stReq, HardwareApplet::GetCountyListByCityRsp& stRsp, tars::TarsCurrentPtr current)
{
    ostringstream ossStr;
    string funcName("getCountyListByCity");
    DEBUGLOG_WITHHEAD(stHead, ossStr.str() << endl;);
    int ret = -1;
    // 用于回包
    HardwareApplet::GetCountyListByCityRsp response;
    try
    {

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
