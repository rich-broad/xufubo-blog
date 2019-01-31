/*********************************************************************************
 FileName: GoodsCommand.cpp
 Author: xufubo
 Date:  2019-01-01
 Description:
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/

#include "GoodsCommand.h"
#include "AppletGoodsManageServer.h"

//using namespace HardwareApplet;

GoodsCommand::GoodsCommand(ConfigurationFactory* configurationFactory, DependencedServiceFactory* serviceFactory, TC_Mysql * mysql)
	:Command(configurationFactory, serviceFactory), _mysql(mysql)
{

}

GoodsCommand::~GoodsCommand()
{

}

int GoodsCommand::getGoodsSPUList(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::GetGoodsSPUListReq& stReq, HardwareApplet::GetGoodsSPUListRsp& stRsp, tars::TarsCurrentPtr current)
{
    ostringstream ossStr;
    string funcName("getGoodsSPUList");
    DEBUGLOG_WITHHEAD(stHead, ossStr.str() << endl);
    int ret = -1;
    // ÓÃÓÚ»Ø°ü
    HardwareApplet::GetGoodsSPUListRsp response;
    try
    {

        response.ret = 0;
        sendReponse(response, stHead, funcName, ret, current);
        ret = 0;
    }
    __CATCH_EXCEPTION_WITH__(funcName);
    if (ret != 0)
    {
        //Å×³öÒì³£Ò²µÃ»Ø°ü
        sendReponse(response, stHead, funcName, ret, current);
    }

    return ret;
}

int GoodsCommand::addGoodsSPUInfo(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::AddGoodsSPUInfoReq& stReq, HardwareApplet::AddGoodsSPUInfoRsp& stRsp, tars::TarsCurrentPtr current)
{
    ostringstream ossStr;
    string funcName("addGoodsSPUInfo");
    DEBUGLOG_WITHHEAD(stHead, ossStr.str() << endl);
    int ret = -1;
    // ÓÃÓÚ»Ø°ü
    HardwareApplet::AddGoodsSPUInfoRsp response;
    try
    {
        ret = insertSPUCategoryMediaInfo__(stHead, stReq);
        if (ret)
        {
            ERRORLOG("insertSPUCategoryMediaInfo__ error|" << endl);
        }
        response.ret = ret;
        sendReponse(response, stHead, funcName, ret, current);
        ret = 0;
    }
    __CATCH_EXCEPTION_WITH__(funcName);
    if (ret != 0)
    {
        //Å×³öÒì³£Ò²µÃ»Ø°ü
        sendReponse(response, stHead, funcName, ret, current);
    }

    return ret;
}

int GoodsCommand::getGoodsSKUList(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::GetGoodsSKUListReq& stReq, HardwareApplet::GetGoodsSKUListRsp& stRsp, tars::TarsCurrentPtr current)
{
    ostringstream ossStr;
    string funcName("getGoodsSKUList");
    DEBUGLOG_WITHHEAD(stHead, ossStr.str() << endl);
    int ret = -1;
    // ÓÃÓÚ»Ø°ü
    HardwareApplet::GetGoodsSKUListRsp response;
    try
    {

        response.ret = 0;
        sendReponse(response, stHead, funcName, ret, current);
        ret = 0;
    }
    __CATCH_EXCEPTION_WITH__(funcName);
    if (ret != 0)
    {
        //Å×³öÒì³£Ò²µÃ»Ø°ü
        sendReponse(response, stHead, funcName, ret, current);
    }

    return ret;
}

int GoodsCommand::addGoodsSKUInfo(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::AddGoodsSKUInfoReq& stReq, HardwareApplet::AddGoodsSKUInfoRsp& stRsp, tars::TarsCurrentPtr current)
{
    ostringstream ossStr;
    string funcName("addGoodsSKUInfo");
    DEBUGLOG_WITHHEAD(stHead, ossStr.str() << endl);
    int ret = -1;
    // ÓÃÓÚ»Ø°ü
    HardwareApplet::AddGoodsSKUInfoRsp response;
    try
    {
        ret = insertSKUAndSKUValueInfo__(stHead, stReq);
        if (ret)
        {
            ERRORLOG("insertSKUAndSKUValueInfo__ error|" << endl);
        }
        response.ret = ret;
        sendReponse(response, stHead, funcName, ret, current);
        ret = 0;
    }
    __CATCH_EXCEPTION_WITH__(funcName);
    if (ret != 0)
    {
        //Å×³öÒì³£Ò²µÃ»Ø°ü
        sendReponse(response, stHead, funcName, ret, current);
    }

    return ret;
}

int GoodsCommand::transferGoodsStock(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::TransferGoodsStockReq& stReq, HardwareApplet::TransferGoodsStockRsp& stRsp, tars::TarsCurrentPtr current)
{
    ostringstream ossStr;
    string funcName("transferGoodsStock");
    DEBUGLOG_WITHHEAD(stHead, ossStr.str() << endl);
    int ret = -1;
    // ÓÃÓÚ»Ø°ü
    HardwareApplet::TransferGoodsStockRsp response;
    try
    {
        ret = transferGoodsStock__(stHead, stReq);
        if (ret)
        {
            ERRORLOG("transferGoodsStock error|" << endl);
        }
        response.ret = ret;
        sendReponse(response, stHead, funcName, ret, current);
        ret = 0;
    }
    __CATCH_EXCEPTION_WITH__(funcName);
    if (ret != 0)
    {
        //Å×³öÒì³£Ò²µÃ»Ø°ü
        sendReponse(response, stHead, funcName, ret, current);
    }

    return ret;
}

//========================================Ë½ÓÐº¯Êý start==============================================
//////////////////////////////////////////////////////////////////////////
//----------------------------------insertSPUCategoryMediaInfo__ start--------------------------------
int GoodsCommand::insertSPUCategoryMediaInfo__(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::AddGoodsSPUInfoReq& stReq)
{
    int ret = -1;
    // ÔÚÁ¬½Ó_db_InfoÉÏÖ´ÐÐÊÂÎñ
    AppletTransaction transaction(_mysql);
    try
    {
        if ((ret = transaction.Start()) != 0)
        {
            ERRORLOG("transaction.Start error|" << endl);
            return ret;
        }
        int spuid = 0;
        ret = insertGoodsSPUInfo__(stHead, stReq, spuid);
        if (ret != 0)
        {
            ERRORLOG("insertGoodsSPUInfo__ error|" << endl);
            transaction.Rollback();
            return ret;
        }

        ret = insertGoodsCategoryInfo__(stHead, stReq, spuid);
        if (ret != 0)
        {
            ERRORLOG("insertGoodsCategoryInfo__ error|" << endl);
            transaction.Rollback();
            return ret;
        }
        ret = insertGoodsMediaInfo__(stHead, stReq, spuid);
        if (ret != 0)
        {
            ERRORLOG("insertGoodsMediaInfo__ error|" << endl);
            transaction.Rollback();
            return ret;
        }
        transaction.Commit();
    }
    catch (...)
    {
        ERRORLOG("exception unknow|" << "insertSPUCategoryMediaInfo__" << endl);
        transaction.Rollback();
        ret = -1;
    }

    return ret;
}

int GoodsCommand::insertGoodsSPUInfo__(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::AddGoodsSPUInfoReq& stReq, int &spuid)
{
    int ret = -1;
    try
    {
        map<string, pair<TC_Mysql::FT, string> > record;
        record["create_time"] = make_pair(TC_Mysql::DB_INT, TNOW);
        record["name"] = make_pair(TC_Mysql::DB_STR, stReq.detail.name);
        record["brand_id"] = make_pair(TC_Mysql::DB_INT, stReq.detail.brandId);
        record["maker_id"] = make_pair(TC_Mysql::DB_INT, stReq.detail.makerId);
        record["s_desc"] = make_pair(TC_Mysql::DB_STR, stReq.detail.sDesc);
        record["desc"] = make_pair(TC_Mysql::DB_STR, stReq.detail.desc);
        record["model"] = make_pair(TC_Mysql::DB_STR, stReq.detail.model);
        record["status"] = make_pair(TC_Mysql::DB_INT, 0);  // ×´Ì¬ 0=>ÐÂÔö,1=>ÉÏ¼Ü,-1=>ÏÂ¼Ü

        size_t affected_rows = _mysql->insertRecord("t_goods_spu_info", record);
        if (affected_rows != 1)
        {
            ERRORLOG_WITHHEAD(stHead, "insert error. affected_rows|" << affected_rows << endl);
            return -1;
        }
        ret = 0;
    }
    __CATCH_EXCEPTION_WITH__("MySQL_Exception");

    if (ret != 0)
    {
        ERRORLOG_WITHHEAD(stHead, "exec sql error|" << endl);
        return ret;
    }
    ostringstream sqlStr;
    sqlStr << "select `spu_id` from t_goods_spu_info where `brand_id` = " << stReq.detail.brandId
        << " and `name` = '" << _mysql->escapeString(stReq.detail.name) << "'";

    TC_Mysql::MysqlData data = _mysql->queryRecord(sqlStr.str());
    if (data.size() != 1)
    {
        ERRORLOG("data.size() != 1 error|" << sqlStr.str() << endl);
        return -1;
    }
    TC_Mysql::MysqlRecord record = data[0];
    spuid = TC_Common::strto<int>(record["spu_id"]);
    return ret;
}

// ÕâÀïÊÇ¸öÅúÁ¿²åÈë
int GoodsCommand::insertGoodsCategoryInfo__(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::AddGoodsSPUInfoReq& stReq, const int spuid)
{
    int ret = -1;
    ostringstream ossStr;
    if (stReq.detail.categoryInfo.empty())
    {
        ERRORLOG_WITHHEAD(stHead, "stReq.detail.categoryInfo.empty() error" << endl);
        return -1;
    }
    int64_t now_time_ts = TNOW;
    try
    {
        ossStr << "insert into t_goods_category_info (`create_time`, `spu_id`, `category_id`) values ";
        for (size_t i = 0; i < stReq.detail.categoryInfo.size(); ++i)
        {
            ossStr << "( " << now_time_ts << "," << spuid << "," << stReq.detail.categoryInfo[i].categoryId << " )";
            if (i !=  stReq.detail.categoryInfo.size() - 1)
            {
                ossStr << ",";
            }
        }
        DEBUGLOG_WITHHEAD(stHead, "sql = " << ossStr.str() << endl);

        _mysql->execute(ossStr.str());

        size_t affected_rows = _mysql->getAffectedRows();

        if (affected_rows != stReq.detail.categoryInfo.size())
        {
            ERRORLOG_WITHHEAD(stHead, "insert error. affected_rows|" << affected_rows << endl);
            return -1;
        }
        ret = 0;
    }
    __CATCH_EXCEPTION_WITH__("MySQL_Exception");

    if (ret != 0)
    {
        ERRORLOG_WITHHEAD(stHead, "exec sql error|" << endl);
    }
    return ret;
}

// ÕâÀïÒ²ÊÇÅúÁ¿²åÈë
int GoodsCommand::insertGoodsMediaInfo__(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::AddGoodsSPUInfoReq& stReq, const int spuid)
{
    int ret = -1;
    ostringstream ossStr;
    if (stReq.detail.mediaInfo.empty())
    {
        ERRORLOG_WITHHEAD(stHead, "stReq.detail.mediaInfo.empty() error" << endl);
        return -1;
    }
    int64_t now_time_ts = TNOW;
    try
    {
        ossStr << "insert into t_goods_media_info (`create_time`, `spu_id`, `type`, `position`, `media_url`) values ";
        for (size_t i = 0; i < stReq.detail.mediaInfo.size(); ++i)
        {
            ossStr << "( " << now_time_ts << "," << spuid << "," << stReq.detail.mediaInfo[i].type << ","
                << stReq.detail.mediaInfo[i].position << "," << "'" << _mysql->escapeString(stReq.detail.mediaInfo[i].mediaUrl) << "'" << " )";
            if (i !=  stReq.detail.mediaInfo.size() - 1)
            {
                ossStr << ",";
            }
        }
        DEBUGLOG_WITHHEAD(stHead, "sql = " << ossStr.str() << endl);

        _mysql->execute(ossStr.str());

        size_t affected_rows = _mysql->getAffectedRows();

        if (affected_rows != stReq.detail.mediaInfo.size())
        {
            ERRORLOG_WITHHEAD(stHead, "insert error. affected_rows|" << affected_rows << endl);
            return -1;
        }
        ret = 0;
    }
    __CATCH_EXCEPTION_WITH__("MySQL_Exception");

    if (ret != 0)
    {
        ERRORLOG_WITHHEAD(stHead, "exec sql error|" << endl);
    }
    return ret;
}


//----------------------------------insertSPUCategoryMediaInfo__ end--------------------------------

//----------------------------------insertSKUAndSKUValueInfo__ start--------------------------------
int GoodsCommand::insertSKUAndSKUValueInfo__(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::AddGoodsSKUInfoReq& stReq)
{
    int ret = -1;
    // ÔÚÁ¬½Ó_db_InfoÉÏÖ´ÐÐÊÂÎñ
    AppletTransaction transaction(_mysql);
    try
    {
        if ((ret = transaction.Start()) != 0)
        {
            ERRORLOG("transaction.Start error|" << endl);
            return ret;
        }
        int skuid = 0;
        ret = insertGoodsSKUInfo__(stHead, stReq, skuid);
        if (ret != 0)
        {
            ERRORLOG("insertGoodsSKUInfo__ error|" << endl);
            transaction.Rollback();
            return ret;
        }

        ret = insertSKUValueInfo__(stHead, stReq, skuid);
        if (ret != 0)
        {
            ERRORLOG("insertSKUValueInfo__ error|" << endl);
            transaction.Rollback();
            return ret;
        }

        ret = insertSKUStockInfo__(stHead, stReq, skuid);
        if (ret != 0)
        {
            ERRORLOG("insertSKUStockInfo__ error|" << endl);
            transaction.Rollback();
            return ret;
        }

        transaction.Commit();
    }
    catch (...)
    {
        ERRORLOG("exception unknow|" << "insertSKUAndSKUValueInfo__" << endl);
        transaction.Rollback();
        ret = -1;
    }

    return ret;
}

int GoodsCommand::insertGoodsSKUInfo__(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::AddGoodsSKUInfoReq& stReq, int &skuid)
{
    int ret = -1;
    try
    {
        map<string, pair<TC_Mysql::FT, string> > record;
        record["create_time"] = make_pair(TC_Mysql::DB_INT, TNOW);
        record["name"] = make_pair(TC_Mysql::DB_STR, stReq.skuInfo.name);
//         record["stock"] = make_pair(TC_Mysql::DB_INT, stReq.skuInfo.stock);
//         record["warning_stock"] = make_pair(TC_Mysql::DB_INT, stReq.skuInfo.warnStock);
        record["price"] = make_pair(TC_Mysql::DB_INT, stReq.skuInfo.price);
        record["img_url"] = make_pair(TC_Mysql::DB_STR, stReq.skuInfo.imgUrl);
        record["is_default"] = make_pair(TC_Mysql::DB_INT, stReq.skuInfo.isDefault);
        record["min_count"] = make_pair(TC_Mysql::DB_INT, stReq.skuInfo.minCount);
        record["spu_id"] = make_pair(TC_Mysql::DB_INT, stReq.skuInfo.spuId);

        size_t affected_rows = _mysql->insertRecord("t_goods_sku_info", record);
        if (affected_rows != 1)
        {
            ERRORLOG_WITHHEAD(stHead, "insert error. affected_rows|" << affected_rows << endl);
            return -1;
        }
        ret = 0;
    }
    __CATCH_EXCEPTION_WITH__("MySQL_Exception");

    if (ret != 0)
    {
        ERRORLOG_WITHHEAD(stHead, "exec sql error|" << endl);
        return ret;
    }
    ostringstream sqlStr;
    sqlStr << "select `sku_id` from t_goods_sku_info where `spu_id` = " << stReq.skuInfo.spuId
        << " and `name` = '" << _mysql->escapeString(stReq.skuInfo.name) << "'";

    TC_Mysql::MysqlData data = _mysql->queryRecord(sqlStr.str());
    if (data.size() != 1)
    {
        ERRORLOG("data.size() != 1 error|" << sqlStr.str() << endl);
        return -1;
    }
    TC_Mysql::MysqlRecord record = data[0];
    skuid = TC_Common::strto<int>(record["sku_id"]);
    return ret;
}

int GoodsCommand::insertSKUValueInfo__(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::AddGoodsSKUInfoReq& stReq, const int skuid)
{
    int ret = -1;
    ostringstream ossStr;
    if (stReq.skuInfo.attrList.empty())
    {
        ERRORLOG_WITHHEAD(stHead, "stReq.skuInfo.attrList.empty() error" << endl);
        return -1;
    }
    int64_t now_time_ts = TNOW;
    try
    {
        ossStr << "insert into t_goods_sku_value_info (`create_time`, `sku_id`, `attr_id`, `attr_value_id`) values ";
        for (size_t i = 0; i < stReq.skuInfo.attrList.size(); ++i)
        {
            ossStr << "( " << now_time_ts << "," << skuid << "," << stReq.skuInfo.attrList[i].attrId << "," << stReq.skuInfo.attrList[i].attrValueId << " )";
            if (i !=  stReq.skuInfo.attrList.size() - 1)
            {
                ossStr << ",";
            }
        }
        DEBUGLOG_WITHHEAD(stHead, "sql = " << ossStr.str() << endl);

        _mysql->execute(ossStr.str());

        size_t affected_rows = _mysql->getAffectedRows();

        if (affected_rows != stReq.skuInfo.attrList.size())
        {
            ERRORLOG_WITHHEAD(stHead, "insert error. affected_rows|" << affected_rows << endl);
            return -1;
        }
        ret = 0;
    }
    __CATCH_EXCEPTION_WITH__("MySQL_Exception");

    if (ret != 0)
    {
        ERRORLOG_WITHHEAD(stHead, "exec sql error|" << endl);
    }
    return ret;
}

int GoodsCommand::insertSKUStockInfo__(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::AddGoodsSKUInfoReq& stReq, const int skuid)
{
    int ret = -1;
    ostringstream ossStr;
    if (stReq.skuInfo.stockList.empty())
    {
        ERRORLOG_WITHHEAD(stHead, "stReq.skuInfo.stockList.empty() error" << endl);
        return -1;
    }
    int64_t now_time_ts = TNOW;
    try
    {
        ossStr << "insert into t_goods_sku_stock_info (`create_time`, `sku_id`, `warehouse_id`, `stock`, `warning_stock`) values ";
        for (size_t i = 0; i < stReq.skuInfo.stockList.size(); ++i)
        {
            ossStr << "( " << now_time_ts << "," << skuid << "," << stReq.skuInfo.stockList[i].warehouseId << "," 
                << stReq.skuInfo.stockList[i].stock << "," << stReq.skuInfo.stockList[i].warningStock << " )";
            if (i !=  stReq.skuInfo.stockList.size() - 1)
            {
                ossStr << ",";
            }
        }
        DEBUGLOG_WITHHEAD(stHead, "sql = " << ossStr.str() << endl);

        _mysql->execute(ossStr.str());

        size_t affected_rows = _mysql->getAffectedRows();

        if (affected_rows != stReq.skuInfo.stockList.size())
        {
            ERRORLOG_WITHHEAD(stHead, "insert error. affected_rows|" << affected_rows << endl);
            return -1;
        }
        ret = 0;
    }
    __CATCH_EXCEPTION_WITH__("MySQL_Exception");

    if (ret != 0)
    {
        ERRORLOG_WITHHEAD(stHead, "exec sql error|" << endl);
    }
    return ret;
}

//----------------------------------insertSKUAndSKUValueInfo__ end--------------------------------


int GoodsCommand::transferGoodsStock__(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::TransferGoodsStockReq& stReq)
{
    int ret = -1;
    // ÔÚÁ¬½Ó_db_InfoÉÏÖ´ÐÐÊÂÎñ
    AppletTransaction transaction(_mysql);
    try
    {
        if ((ret = transaction.Start()) != 0)
        {
            ERRORLOG("transaction.Start error|" << endl);
            return ret;
        }
        ret = reduceOldStock__(stHead, stReq);
        if (ret != 0)
        {
            ERRORLOG("reduceOldStock__ error|" << endl);
            transaction.Rollback();
            return ret;
        }

        ret = increaseNewStock__(stHead, stReq);
        if (ret != 0)
        {
            ERRORLOG("increaseNewStock__ error|" << endl);
            transaction.Rollback();
            return ret;
        }
        transaction.Commit();
    }
    catch (...)
    {
        ERRORLOG("exception unknow|" << "transferGoodsStock__" << endl);
        transaction.Rollback();
        ret = -1;
    }

    return ret;
}

int GoodsCommand::reduceOldStock__(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::TransferGoodsStockReq& stReq)
{
    int ret = -1;
    ostringstream ossStr;
    try
    {
        ossStr << "update t_goods_sku_stock_info set `stock` = `stock` - " << stReq.num
            << " where `sku_id` = " << stReq.skuId << " and `warehouse_id` = " << stReq.oldWarehouseId << " and `stock` - " << stReq.num << " >= 0";
        DEBUGLOG_WITHHEAD(stHead, "sql = " << ossStr.str() << endl);
        _mysql->execute(ossStr.str());

        size_t affected_rows = _mysql->getAffectedRows();

        if (affected_rows != 1)
        {
            ERRORLOG_WITHHEAD(stHead, "update error. affected_rows|" << affected_rows << endl);
            return -1;
        }
 
        ret = 0;
    }
    __CATCH_EXCEPTION_WITH__("MySQL_Exception");

    if (ret != 0)
    {
        ERRORLOG_WITHHEAD(stHead, "exec sql error|" << endl);
    }
    return ret;
}

int GoodsCommand::increaseNewStock__(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::TransferGoodsStockReq& stReq)
{
    int ret = -1;
    ostringstream ossStr;
    int64_t now_time_ts = TNOW;
    try
    {
        int recordNum = 0;
        {
            ossStr << "select id from t_goods_sku_stock_info  where `sku_id` = " << stReq.skuId << " and `warehouse_id` = " << stReq.newWarehouseId;
            TC_Mysql::MysqlData data = _mysql->queryRecord(ossStr.str());
            recordNum = data.size();
        }
        {
            ossStr.str(""); ossStr.clear();
            if (recordNum > 0)
            {
                ossStr << "update t_goods_sku_stock_info set `stock` = `stock` + " << stReq.num
                    << " where `sku_id` = " << stReq.skuId << " and `warehouse_id` = " << stReq.newWarehouseId;
            }
            else
            {
                ossStr << "insert into t_goods_sku_stock_info (`create_time`, `sku_id`, `warehouse_id`, `stock`, `warning_stock`) values "
                    << " (" << now_time_ts << "," << stReq.skuId << "," << stReq.newWarehouseId << "," 
                    << stReq.num << "," << 0 << " )";
            }
        }
        DEBUGLOG_WITHHEAD(stHead, "sql = " << ossStr.str() << endl);
        _mysql->execute(ossStr.str());

        size_t affected_rows = _mysql->getAffectedRows();

        if (affected_rows != 1)
        {
            ERRORLOG_WITHHEAD(stHead, "increaseNewStock__ error. affected_rows|" << affected_rows << endl);
            return -1;
        }

        ret = 0;
    }
    __CATCH_EXCEPTION_WITH__("MySQL_Exception");

    if (ret != 0)
    {
        ERRORLOG_WITHHEAD(stHead, "exec sql error|" << endl);
    }
    return ret;
}

//////////////////////////////////////////////////////////////////////////