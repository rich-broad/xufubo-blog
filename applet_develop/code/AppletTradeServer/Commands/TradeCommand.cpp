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

/**
 * 1、先加入购物车、
 * 2、再提交订单（操作两个表，生成订单号，需要计算展示价格，成交价格默认与此相等），
 * 3、一一确认订单中商品的成交价格，
 * 4、确认订单（计算成交价格）。
 * 5、设置发货（3-已发货代付款，4-已发货已付款）
 */

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
        record["tran_price"] = make_pair(TC_Mysql::DB_INT, stReq.price);  // 成交价格暂时等于展示价格，等待确认订单的过程中处理。
        record["num"] = make_pair(TC_Mysql::DB_INT, stReq.num);
        record["status"] = make_pair(TC_Mysql::DB_INT, 0);   // 0-新加入购物车
        
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
        ret = queryShopCartAndSKUInfo__(stHead, response.itemList);
        if (ret != 0)
        {
            ERRORLOG_WITHHEAD(stHead, "call queryShopCartAndSKUInfo__ error|" << ret << "|" << endl);
            response.ret = ret;
            sendReponse(response, stHead, funcName, ret, current);
            return ret;
        }
        ret = querySKUValueInfo__(stHead, response.itemList);
        if (ret != 0)
        {
            ERRORLOG_WITHHEAD(stHead, "call querySKUValueInfo__ error|" << ret << "|" << endl);
            response.ret = ret;
            sendReponse(response, stHead, funcName, ret, current);
            return ret;
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
        string billno;
        ret = base_utils::StringUtils::GetUniKey(billno);
        if (ret != 0)
        {
            ERRORLOG_WITHHEAD(stHead, "GetUniKey error|" << ret << "|" << ossStr.str() << endl);
            response.ret = 0;
            sendReponse(response, stHead, funcName, ret, current);
            return ret;
        }

        ret = saveOrderAndUpShopCart__(stHead, stReq, billno);
        if (ret)
        {
            ERRORLOG("saveOrderAndUpShopCart__ error|" << endl);
        }
        
        response.ret = ret;
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
// 卖家确认交易价格
int TradeCommand::confirmGoodsTranPrice(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::ConfirmGoodsTranPriceReq& stReq, HardwareApplet::ConfirmGoodsTranPriceRsp& stRsp, tars::TarsCurrentPtr current)
{
    ostringstream ossStr;
    string funcName("confirmTranPrice");
    DEBUGLOG_WITHHEAD(stHead, ossStr.str() << endl);
    int ret = -1;
    // 用于回包
    HardwareApplet::ConfirmGoodsTranPriceRsp response;
    try
    {
        ostringstream conStr;
        conStr << " where `cart_id` = " << stReq.item.cartId << "," << " and `sku_id` = " << stReq.item.skuId << ","
            << " and `union_id` = " << "'" << stHead.sessionInfo.unionid << "', " << " and `bill_no` = " << "'" << stReq.item.billNo << "'";
        map<string, pair<TC_Mysql::FT, string> > record;
        record["tran_price"] = make_pair(TC_Mysql::DB_INT, stReq.item.tranPrice);

        size_t affected_rows = _mysql->updateRecord("t_goods_shop_cart_info", record, conStr.str());
        if (affected_rows != 1)
        {
            ERRORLOG("update error. affected_rows|" << affected_rows << endl);
            response.ret = -1;
            response.errmsg = "update db failed";
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
    return 0;
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
        ret = confirmOrderTable__(stHead, stReq);
        if (ret)
        {
            ERRORLOG("confirmOrderTable__ error|" << endl);
        }

        response.ret = ret;
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

int TradeCommand::deliverGoods(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::DeliverGoodsReq& stReq, HardwareApplet::DeliverGoodsRsp& stRsp, tars::TarsCurrentPtr current)
{
    ostringstream ossStr;
    string funcName("deliverGoods");
    DEBUGLOG_WITHHEAD(stHead, ossStr.str() << endl);
    int ret = -1;
    // 用于回包
    HardwareApplet::DeliverGoodsRsp response;
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
        ostringstream sqlStr;
        sqlStr << "select t1.cart_id, t1.spu_id, t1.sku_id, t1.price, t1.num, t2.name, t2.img_url from "
            << " t_goods_shop_cart_info as t1 INNER JOIN t_goods_sku_info as t2 on t1.sku_id = t2.sku_id "
            << " where t1.union_id = " << "'" << _mysql->escapeString(stHead.sessionInfo.unionid) << "'" << " and t1.uid = " << stHead.sessionInfo.uid
            << " and t1.status = " << 0;
        TC_Mysql::MysqlData data = _mysql->queryRecord(sqlStr.str());
        DEBUGLOG_WITHHEAD(stHead, "sql = " << sqlStr.str() << endl);
        for (size_t i = 0; i < data.size(); ++i)
        {
            HardwareApplet::ShopCartItem item;
            TC_Mysql::MysqlRecord record = data[i];
            item.cartId = TC_Common::strto<int>(record["cart_id"]);
            item.spuId = TC_Common::strto<int>(record["spu_id"]);
            item.skuId = TC_Common::strto<int>(record["sku_id"]);
            item.uid = stHead.sessionInfo.uid;
            item.price = TC_Common::strto<int>(record["price"]);
            item.num = TC_Common::strto<int>(record["num"]);
            item.status = 0;

            item.skuInfo.skuId = item.skuId;
            item.skuInfo.name = record["name"];
            item.skuInfo.imgUrl = record["img_url"];
            itemList.push_back(item);
        }
        ret = 0;
    }
    __CATCH_EXCEPTION_WITH__(__func__);
    return ret;
}

int TradeCommand::querySKUValueInfo__(const HardwareApplet::AppletCommHead& stHead, vector<HardwareApplet::ShopCartItem> &itemList)
{
    if (itemList.empty())
    {
        return 0;
    }
    ostringstream skuIdStr;
    for (size_t i = 0; i < itemList.size(); ++i)
    {
        skuIdStr << itemList[i].skuId;
        if (i != itemList.size() - 1)
        {
            skuIdStr << ",";
        }
    }
    int ret = -1;
    try
    {
        ostringstream sqlStr;
        sqlStr << "select sku_id, attr_id, attr_value_id from t_goods_sku_value_info where sku_id in "
            << " (" << skuIdStr.str() << ")";
        DEBUGLOG_WITHHEAD(stHead, "sql = " << sqlStr.str() << endl);
        TC_Mysql::MysqlData data = _mysql->queryRecord(sqlStr.str());
        map<int, HardwareApplet::GoodsSKUAttrInfo> mpInfo;
        map<int, HardwareApplet::AttributeItem> mpAttr;
        map<int, HardwareApplet::AttributeValueItem> mpAttrValue;
        AttributeMetaManagerSingleton->getAttributeMap(mpAttr);
        AttrValueMetaManagerSingleton->getAttrValueMap(mpAttrValue);
        for (size_t i = 0; i < data.size(); ++i)
        {
            HardwareApplet::GoodsSKUAttrInfo info;
            TC_Mysql::MysqlRecord record = data[i];
            info.attrId = TC_Common::strto<int>(record["attr_id"]);
            info.attrName = mpAttr[info.attrId].attrName;
            info.attrValueId = TC_Common::strto<int>(record["attr_value_id"]);
            info.attrValueName = mpAttrValue[info.attrValueId].name;
            mpInfo[TC_Common::strto<int>(record["sku_id"])] = info;
        }
        for (size_t i = 0; i < itemList.size(); ++i)
        {
            HardwareApplet::ShopCartItem & item = itemList[i];
            item.skuInfo.attrList.push_back(mpInfo[item.skuInfo.skuId]);
        }
        ret = 0;
    }
    __CATCH_EXCEPTION_WITH__(__func__);
    return ret;
}
// -------------------------------------saveOrderAndUpShopCart__ start------------------------------------------------
int TradeCommand::saveOrderAndUpShopCart__(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::SubmitOrderReq& stReq, const string &billno)
{
    int ret = -1;
    // 在连接_db_Info上执行事务
    AppletTransaction transaction(_mysql);
    try
    {
        if ((ret = transaction.Start()) != 0)
        {
            ERRORLOG("transaction.Start error|" << endl);
            return ret;
        }

        ret = insertOrderInfo__(stHead, stReq, billno);
        if (ret != 0)
        {
            ERRORLOG("insertOrderInfo__ error|" << endl);
            transaction.Rollback();
            return ret;
        }

        ret = submitShopCartInfo__(stHead, stReq, billno);
        if (ret != 0)
        {
            ERRORLOG("submitShopCartInfo__ error|" << endl);
            transaction.Rollback();
            return ret;
        }
        transaction.Commit();
    }
    catch (...)
    {
        ERRORLOG("exception unknow|" << "saveOrderAndUpShopCart__" << endl);
        transaction.Rollback();
        ret = -1;
    }

    return ret;
}

int TradeCommand::insertOrderInfo__(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::SubmitOrderReq& stReq, const string &billno)
{
    int ret = -1;
    try
    {
        int64_t tranMoney = 0;
        int64_t money = 0;
        {// 计算价格，不相信前端的。
            ostringstream sqlStr;
            sqlStr << "select `price`, `tran_price`, `num` from t_goods_shop_cart_info where bill_no = '" << stReq.item.billNo << "'" << " for update";
            DEBUGLOG_WITHHEAD(stHead, "sql = " << sqlStr.str() << endl);
            TC_Mysql::MysqlData data = _mysql->queryRecord(sqlStr.str());
            for (size_t i = 0; i < data.size(); ++i)
            {
                tranMoney += S2I(data[i]["tran_price"]) * S2I(data[i]["num"]);
                money += S2I(data[i]["price"]) * S2I(data[i]["num"]);
            }
        }
        if (money != stReq.item.money)  //校验一下前端计算的金额和我的是否一致
        {
            ERRORLOG_WITHHEAD(stHead, "logic error. money != stReq.item.money|" << endl);
            return -1;
        }
        
        map<string, pair<TC_Mysql::FT, string> > record;
        record["create_time"] = make_pair(TC_Mysql::DB_INT, TNOW);
        record["bill_no"] = make_pair(TC_Mysql::DB_STR, billno);
        record["uid"] = make_pair(TC_Mysql::DB_INT, stHead.sessionInfo.uid);
        record["pay_way"] = make_pair(TC_Mysql::DB_INT, stReq.item.payWay);
        record["pay_time_ts"] = make_pair(TC_Mysql::DB_INT, 0);
        record["status"] = make_pair(TC_Mysql::DB_INT, 1);   // 订单状态 1-新提交待确认
        record["money"] = make_pair(TC_Mysql::DB_INT, money);
        record["tran_money"] = make_pair(TC_Mysql::DB_INT, tranMoney);  // 这个值在提交订单的时候默认与展示金额一致。
        record["freight"] = make_pair(TC_Mysql::DB_INT, 0);
        record["address_id"] = make_pair(TC_Mysql::DB_INT, stReq.item.addressInfo.addressId);
        record["union_id"] = make_pair(TC_Mysql::DB_STR, stHead.sessionInfo.unionid);

        size_t affected_rows = _mysql->insertRecord("t_goods_order_info", record);
        if (affected_rows != 1)
        {
            ERRORLOG_WITHHEAD(stHead, "inert error. affected_rows|" << affected_rows << endl);
            return -1;
        }
        ret = 0;
    }
    __CATCH_EXCEPTION_WITH__("MySQL_Exception");

    if (ret != 0)
    {
        ERRORLOG_WITHHEAD(stHead, "exec sql error bill_no|" << billno << endl);
    }
    return ret;
}


int TradeCommand::submitShopCartInfo__(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::SubmitOrderReq& stReq, const string &billno)
{
    int ret = -1;
    try
    {
        ostringstream conStr(" where `cart_id`  in (");
        for (size_t i = 0; i < stReq.item.itemList.size(); ++i)
        {
            conStr << stReq.item.itemList[i].cartId;
            if (i != stReq.item.itemList.size())
            {
                conStr << ", ";
            }
        }
        conStr << ") ";
        map<string, pair<TC_Mysql::FT, string> > record;
        record["status"] = make_pair(TC_Mysql::DB_INT, 1);  // 1-已提交订单
        record["bill_no"] = make_pair(TC_Mysql::DB_STR, billno);

        size_t affected_rows = _mysql->updateRecord("t_goods_shop_cart_info", record, conStr.str());
        if (affected_rows != 1)
        {
            ERRORLOG("inert error. affected_rows|" << affected_rows << endl);
            return -1;
        }
        ret = 0;
    }
    __CATCH_EXCEPTION_WITH__("MySQL_Exception");

    if (ret != 0)
    {
        ERRORLOG_WITHHEAD(stHead, "exec sql error bill_no|" << billno << endl);
    }
    return ret;
}
// -------------------------------------saveOrderAndUpShopCart__ end------------------------------------------------


int TradeCommand::confirmOrderTable__(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::ConfirmOrderReq& stReq)
{
    int ret = -1;
    // 在连接_db_Info上执行事务
    AppletTransaction transaction(_mysql);
    try
    {
        if ((ret = transaction.Start()) != 0)
        {
            ERRORLOG("transaction.Start error|" << endl);
            return ret;
        }
        int64_t tranMoney = 0;
        {
            ostringstream sqlStr;
            sqlStr << "select `tran_price`, `num` from t_goods_shop_cart_info where bill_no = '" << stReq.item.billNo << "'" << " for update";
            DEBUGLOG_WITHHEAD(stHead, "sql = " << sqlStr.str() << endl);
            TC_Mysql::MysqlData data = _mysql->queryRecord(sqlStr.str());
            for (size_t i = 0; i < data.size(); ++i)
            {
                tranMoney += S2I(data[i]["tran_price"]) * S2I(data[i]["num"]);
            }
        }
        {
            ostringstream conStr;
            conStr << " where `bill_no` = '" << stReq.item.billNo << "'";

            map<string, pair<TC_Mysql::FT, string> > record;
            record["tran_price"] = make_pair(TC_Mysql::DB_INT, tranMoney);
            size_t affected_rows = _mysql->updateRecord("t_goods_order_info", record, conStr.str());
            if (affected_rows != 1)
            {
                ERRORLOG("update error. affected_rows|" << affected_rows << endl);
                transaction.Rollback();
                return -1;
            }

        }
        transaction.Commit();
        ret = 0;
    }
    catch (TC_Mysql_Exception& e)
    {
        ERRORLOG_WITHHEAD(stHead, "database exception: " << e.what() << "|" << e.getErrCode() << "|" << stReq.item.billNo << endl);
        transaction.Rollback();
        return -1;
    }
    catch (...)
    {
        ERRORLOG("exception unknow|" << "confirmOrderTable__" << endl);
        transaction.Rollback();
        ret = -1;
    }

    return ret;
}
