/*********************************************************************************
 FileName: 
 Author: xufubo
 Date:  2019-01-01
 Description:
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/

#include "AppletTradeImp.h"

using namespace std;
using namespace rapidjson;
using namespace tars;
using namespace HardwareApplet;

void AppletTradeImp::initialize()
{
    _dbInfo = new TC_Mysql();
    TC_DBConf dbConf;
    dbConf.loadFromMap(DEF_CFG_SINGLETON->_dbInfoConf);
    _dbInfo->init(dbConf);
    _dbInfo->connect();

    _pServiceFactory->Initialize(_pConfigurationFactory);
	_pTradeCommand = new TradeCommand(_pConfigurationFactory, _pServiceFactory, _dbInfo);
	_pOpCommand =new OpCommand(_pConfigurationFactory, _pServiceFactory);
}

void AppletTradeImp::destroy()
{
	if (_pTradeCommand != NULL)
	{
		delete _pTradeCommand; _pTradeCommand = NULL;
	}
	if (_pOpCommand != NULL)
	{
		delete _pOpCommand; _pOpCommand = NULL;
	}
    if (_dbInfo != NULL)
    {
        delete _dbInfo; _dbInfo = NULL;
    }
}

tars::Int32 AppletTradeImp::addGoodsToShopCart(const HardwareApplet::AppletCommHead & stHead,const vector<tars::Char> & vtIn,vector<tars::Char> &vtOut,tars::TarsCurrentPtr current)
{
    int ret = 0;
    try
    {
        HardwareApplet::AddGoodsToShopCartReq stReq;
        HardwareApplet::AddGoodsToShopCartRsp stRsp;
        if (HardwareApplet::TarsDecode<HardwareApplet::AddGoodsToShopCartReq>(vtIn, stReq) != 0)
        {
            ERRORLOG_WITHHEAD(stHead, vtIn.size()) << endl;
            stRsp.ret = -1;
            return -1;
        }
        current->setResponse(false); // ÉèÖÃ·Ç×Ô¶¯»Ø°ü
        ret = _pTradeCommand->addGoodsToShopCart(stHead, stReq, stRsp, current);
    }
    __CATCH_EXCEPTION_WITH__("|AppletTradeImp::addGoodsToShopCart");
    return ret;
}

tars::Int32 AppletTradeImp::getMyShopCartInfo(const HardwareApplet::AppletCommHead & stHead,const vector<tars::Char> & vtIn,vector<tars::Char> &vtOut,tars::TarsCurrentPtr current)
{
    int ret = 0;
    try
    {
        HardwareApplet::GetMyShopCartInfoReq stReq;
        HardwareApplet::GetMyShopCartInfoRsp stRsp;
        if (HardwareApplet::TarsDecode<HardwareApplet::GetMyShopCartInfoReq>(vtIn, stReq) != 0)
        {
            ERRORLOG_WITHHEAD(stHead, vtIn.size()) << endl;
            stRsp.ret = -1;
            return -1;
        }
        current->setResponse(false); // ÉèÖÃ·Ç×Ô¶¯»Ø°ü
        ret = _pTradeCommand->getMyShopCartInfo(stHead, stReq, stRsp, current);
    }
    __CATCH_EXCEPTION_WITH__("|AppletTradeImp::getMyShopCartInfo");
    return ret;
}

tars::Int32 AppletTradeImp::submitOrder(const HardwareApplet::AppletCommHead & stHead,const vector<tars::Char> & vtIn,vector<tars::Char> &vtOut,tars::TarsCurrentPtr current)
{
    int ret = 0;
    try
    {
        HardwareApplet::SubmitOrderReq stReq;
        HardwareApplet::SubmitOrderRsp stRsp;
        if (HardwareApplet::TarsDecode<HardwareApplet::SubmitOrderReq>(vtIn, stReq) != 0)
        {
            ERRORLOG_WITHHEAD(stHead, vtIn.size()) << endl;
            stRsp.ret = -1;
            return -1;
        }
        current->setResponse(false); // ÉèÖÃ·Ç×Ô¶¯»Ø°ü
        ret = _pTradeCommand->submitOrder(stHead, stReq, stRsp, current);
    }
    __CATCH_EXCEPTION_WITH__("|AppletTradeImp::submitOrder");
    return ret;
}

tars::Int32 AppletTradeImp::confirmOrder(const HardwareApplet::AppletCommHead & stHead,const vector<tars::Char> & vtIn,vector<tars::Char> &vtOut,tars::TarsCurrentPtr current)
{
    int ret = 0;
    try
    {
        HardwareApplet::ConfirmOrderReq stReq;
        HardwareApplet::ConfirmOrderRsp stRsp;
        if (HardwareApplet::TarsDecode<HardwareApplet::ConfirmOrderReq>(vtIn, stReq) != 0)
        {
            ERRORLOG_WITHHEAD(stHead, vtIn.size()) << endl;
            stRsp.ret = -1;
            return -1;
        }
        current->setResponse(false); // ÉèÖÃ·Ç×Ô¶¯»Ø°ü
        ret = _pTradeCommand->confirmOrder(stHead, stReq, stRsp, current);
    }
    __CATCH_EXCEPTION_WITH__("|AppletTradeImp::confirmOrder");
    return ret;
}

tars::Int32 AppletTradeImp::getMyOrderList(const HardwareApplet::AppletCommHead & stHead,const vector<tars::Char> & vtIn,vector<tars::Char> &vtOut,tars::TarsCurrentPtr current)
{
    int ret = 0;
    try
    {
        HardwareApplet::GetMyOrderListReq stReq;
        HardwareApplet::GetMyOrderListRsp stRsp;
        if (HardwareApplet::TarsDecode<HardwareApplet::GetMyOrderListReq>(vtIn, stReq) != 0)
        {
            ERRORLOG_WITHHEAD(stHead, vtIn.size()) << endl;
            stRsp.ret = -1;
            return -1;
        }
        current->setResponse(false); // ÉèÖÃ·Ç×Ô¶¯»Ø°ü
        ret = _pTradeCommand->getMyOrderList(stHead, stReq, stRsp, current);
    }
    __CATCH_EXCEPTION_WITH__("|AppletTradeImp::getMyOrderList");
    return ret;
}

tars::Int32 AppletTradeImp::getOrderList(const HardwareApplet::AppletCommHead & stHead,const vector<tars::Char> & vtIn,vector<tars::Char> &vtOut,tars::TarsCurrentPtr current)
{
    int ret = 0;
    try
    {
        HardwareApplet::GetOrderListReq stReq;
        HardwareApplet::GetOrderListRsp stRsp;
        if (HardwareApplet::TarsDecode<HardwareApplet::GetOrderListReq>(vtIn, stReq) != 0)
        {
            ERRORLOG_WITHHEAD(stHead, vtIn.size()) << endl;
            stRsp.ret = -1;
            return -1;
        }
        current->setResponse(false); // ÉèÖÃ·Ç×Ô¶¯»Ø°ü
        ret = _pTradeCommand->getOrderList(stHead, stReq, stRsp, current);
    }
    __CATCH_EXCEPTION_WITH__("|AppletTradeImp::getOrderList");
    return ret;
}

tars::Int32 AppletTradeImp::getOrderDetail(const HardwareApplet::AppletCommHead & stHead,const vector<tars::Char> & vtIn,vector<tars::Char> &vtOut,tars::TarsCurrentPtr current)
{
    int ret = 0;
    try
    {
        HardwareApplet::GetOrderDetailReq stReq;
        HardwareApplet::GetOrderDetailRsp stRsp;
        if (HardwareApplet::TarsDecode<HardwareApplet::GetOrderDetailReq>(vtIn, stReq) != 0)
        {
            ERRORLOG_WITHHEAD(stHead, vtIn.size()) << endl;
            stRsp.ret = -1;
            return -1;
        }
        current->setResponse(false); // ÉèÖÃ·Ç×Ô¶¯»Ø°ü
        ret = _pTradeCommand->getOrderDetail(stHead, stReq, stRsp, current);
    }
    __CATCH_EXCEPTION_WITH__("|AppletTradeImp::getOrderDetail");
    return ret;
}



