/*********************************************************************************
 FileName: TradeCommand.h
 Author: xufubo
 Date:  2019-01-01
 Description:
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/

#ifndef TRADE_COMMAND_H_
#define TRADE_COMMAND_H_

#include "Command.h"
#include "Global.h"
#include "AppletTrade.h"

class TradeCommand : public Command
{
public:
	TradeCommand(ConfigurationFactory* configurationFactory, DependencedServiceFactory* serviceFactory, TC_Mysql* mysql);
	virtual ~TradeCommand();

	int addGoodsToShopCart(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::AddGoodsToShopCartReq& stReq, HardwareApplet::AddGoodsToShopCartRsp& stRsp, tars::TarsCurrentPtr current);
	int getMyShopCartInfo(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::GetMyShopCartInfoReq& stReq, HardwareApplet::GetMyShopCartInfoRsp& stRsp, tars::TarsCurrentPtr current);
    int submitOrder(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::SubmitOrderReq& stReq, HardwareApplet::SubmitOrderRsp& stRsp, tars::TarsCurrentPtr current);
    int confirmOrder(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::ConfirmOrderReq& stReq, HardwareApplet::ConfirmOrderRsp& stRsp, tars::TarsCurrentPtr current);
    int getMyOrderList(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::GetMyOrderListReq& stReq, HardwareApplet::GetMyOrderListRsp& stRsp, tars::TarsCurrentPtr current);
    int getOrderList(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::GetOrderListReq& stReq, HardwareApplet::GetOrderListRsp& stRsp, tars::TarsCurrentPtr current);
    int getOrderDetail(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::GetOrderDetailReq& stReq, HardwareApplet::GetOrderDetailRsp& stRsp, tars::TarsCurrentPtr current);

private:
    int queryShopCartAndSKUInfo__(const HardwareApplet::AppletCommHead& stHead, vector<HardwareApplet::ShopCartItem> &itemList);
    int querySKUValueInfo__();

private:
    TC_Mysql* _mysql;
};

#endif /*TRADE_COMMAND_H_*/
