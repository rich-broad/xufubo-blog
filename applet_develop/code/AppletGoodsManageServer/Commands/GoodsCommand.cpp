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

        ret = insertGoodsSPUInfo__(stHead, stReq);
        if (ret != 0)
        {
            ERRORLOG("insertGoodsSPUInfo__ error|" << endl);
            transaction.Rollback();
            return ret;
        }

        ret = insertGoodsCategoryInfo__(stHead, stReq);
        if (ret != 0)
        {
            ERRORLOG("insertGoodsCategoryInfo__ error|" << endl);
            transaction.Rollback();
            return ret;
        }
        ret = insertGoodsMediaInfo__(stHead, stReq);
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
        ERRORLOG("exception unknow|" << "saveOrderAndUpShopCart__" << endl);
        transaction.Rollback();
        ret = -1;
    }

    return ret;
}

int GoodsCommand::insertGoodsSPUInfo__(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::AddGoodsSPUInfoReq& stReq)
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
    }
    return ret;
}

// ÕâÀïÊÇ¸öÅúÁ¿²åÈë
int GoodsCommand::insertGoodsCategoryInfo__(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::AddGoodsSPUInfoReq& stReq)
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
            ossStr << "( " << now_time_ts << "," << stReq.detail.spuId << "," << stReq.detail.categoryInfo[i].categoryId << " )";
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
int GoodsCommand::insertGoodsMediaInfo__(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::AddGoodsSPUInfoReq& stReq)
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
            ossStr << "( " << now_time_ts << "," << stReq.detail.spuId << "," << stReq.detail.mediaInfo[i].type << ","
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

//////////////////////////////////////////////////////////////////////////