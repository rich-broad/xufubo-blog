/*********************************************************************************
 FileName: TradeCommand.cpp
 Author: xufubo
 Date:  2019-01-01
 Description:
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/

#include "TradeCommand.h"
#include "AppletTradeServer.h"
using namespace HardwareApplet;

TradeCommand::TradeCommand(ConfigurationFactory* configurationFactory, DependencedServiceFactory* serviceFactory, TC_Mysql* mysql)
	:Command(configurationFactory, serviceFactory), _mysql(mysql)
{

}

TradeCommand::~TradeCommand()
{

}

int TradeCommand::addGoodsToShopCart(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::AddGoodsToShopCartReq& stReq, HardwareApplet::AddGoodsToShopCartRsp& stRsp, tars::TarsCurrentPtr current)
{
    ostringstream ossStr;
    string funcName("addGoodsToShopCart");
    DEBUGLOG_WITHHEAD(stHead, ossStr.str() << endl);
    int ret = -1;
    // 用于回包
    HardwareApplet::AddGoodsToShopCartRsp response;
    try
    {
        map<string, pair<TC_Mysql::FT, string> > record;
        record["create_time"] = make_pair(TC_Mysql::DB_INT, TNOW);
        record["uid"] = make_pair(TC_Mysql::DB_INT, stHead.sessionInfo.uid);
        record["union_id"] = make_pair(TC_Mysql::DB_STR, stHead.sessionInfo.unionid);
        record["spu_id"] = make_pair(TC_Mysql::DB_STR, stReq.spuId);
        record["sku_id"] = make_pair(TC_Mysql::DB_STR, stReq.skuId);
        record["price"] = make_pair(TC_Mysql::DB_INT, stReq.price);
        record["num"] = make_pair(TC_Mysql::DB_INT, stReq.num);
        record["status"] = make_pair(TC_Mysql::DB_INT, 0);
        
        size_t affected_rows = _mysql->insertRecord("t_goods_shop_cart_info", record);
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

int TradeCommand::getMyShopCartInfo(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::GetMyShopCartInfoReq& stReq, HardwareApplet::GetMyShopCartInfoRsp& stRsp, tars::TarsCurrentPtr current)
{
    ostringstream ossStr;
    string funcName("getMyShopCartInfo");
    DEBUGLOG_WITHHEAD(stHead, ossStr.str() << endl);
    int ret = -1;
    // 用于回包
    HardwareApplet::GetMyShopCartInfoRsp response;
    try
    {
        
        response.ret = 0;
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

int TradeCommand::submitOrder(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::SubmitOrderReq& stReq, HardwareApplet::SubmitOrderRsp& stRsp, tars::TarsCurrentPtr current)
{
    ostringstream ossStr;
    string funcName("submitOrder");
    DEBUGLOG_WITHHEAD(stHead, ossStr.str() << endl);
    int ret = -1;
    // 用于回包
    HardwareApplet::SubmitOrderRsp response;
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

int TradeCommand::confirmOrder(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::ConfirmOrderReq& stReq, HardwareApplet::ConfirmOrderRsp& stRsp, tars::TarsCurrentPtr current)
{
    ostringstream ossStr;
    string funcName("confirmOrder");
    DEBUGLOG_WITHHEAD(stHead, ossStr.str() << endl);
    int ret = -1;
    // 用于回包
    HardwareApplet::ConfirmOrderRsp response;
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

int TradeCommand::getMyOrderList(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::GetMyOrderListReq& stReq, HardwareApplet::GetMyOrderListRsp& stRsp, tars::TarsCurrentPtr current)
{
    ostringstream ossStr;
    string funcName("getMyOrderList");
    DEBUGLOG_WITHHEAD(stHead, ossStr.str() << endl);
    int ret = -1;
    // 用于回包
    HardwareApplet::GetMyOrderListRsp response;
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

int TradeCommand::getOrderList(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::GetOrderListReq& stReq, HardwareApplet::GetOrderListRsp& stRsp, tars::TarsCurrentPtr current)
{
    ostringstream ossStr;
    string funcName("getOrderList");
    DEBUGLOG_WITHHEAD(stHead, ossStr.str() << endl);
    int ret = -1;
    // 用于回包
    HardwareApplet::GetOrderListRsp response;
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

int TradeCommand::getOrderDetail(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::GetOrderDetailReq& stReq, HardwareApplet::GetOrderDetailRsp& stRsp, tars::TarsCurrentPtr current)
{
    ostringstream ossStr;
    string funcName("getOrderDetail");
    DEBUGLOG_WITHHEAD(stHead, ossStr.str() << endl);
    int ret = -1;
    // 用于回包
    HardwareApplet::GetOrderDetailRsp response;
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

//=========================================类private函数 start=======================================================================//
//////////////////////////////////////////////////////////////////////////
int TradeCommand::queryShopCartAndSKUInfo__(const HardwareApplet::AppletCommHead& stHead, vector<HardwareApplet::ShopCartItem> &itemList)
{
    int ret = -1;
    try
    {
        /*
        ostringstream sqlStr;
        sqlStr << "select cart_id, spu_id, phone_num, province, city, county, address_detail, receiver from t_goods_shop_cart_info "
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
        */
        ret = 0;
    }
    __CATCH_EXCEPTION_WITH__(__func__);
    return ret;
}

int TradeCommand::querySKUValueInfo__()
{
    int ret = -1;
    try
    {
        ret = 0;
    }
    __CATCH_EXCEPTION_WITH__(__func__);
    return ret;
}