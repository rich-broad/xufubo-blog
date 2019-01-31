/*********************************************************************************
 FileName: GoodsCommand.h
 Author: xufubo
 Date:  2019-01-01
 Description:
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/

#ifndef GOODS_COMMAND_H_
#define GOODS_COMMAND_H_

#include "Command.h"
#include "Global.h"
#include "AppletGoodsManage.h"

using namespace HardwareApplet;

class GoodsCommand : public Command
{
public:
	GoodsCommand(ConfigurationFactory* configurationFactory, DependencedServiceFactory* serviceFactory, TC_Mysql * mysql);
	virtual ~GoodsCommand();

	int getGoodsSPUList(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::GetGoodsSPUListReq& stReq, HardwareApplet::GetGoodsSPUListRsp& stRsp, tars::TarsCurrentPtr current);
    int addGoodsSPUInfo(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::AddGoodsSPUInfoReq& stReq, HardwareApplet::AddGoodsSPUInfoRsp& stRsp, tars::TarsCurrentPtr current);

    int getGoodsSKUList(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::GetGoodsSKUListReq& stReq, HardwareApplet::GetGoodsSKUListRsp& stRsp, tars::TarsCurrentPtr current);
    int addGoodsSKUInfo(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::AddGoodsSKUInfoReq& stReq, HardwareApplet::AddGoodsSKUInfoRsp& stRsp, tars::TarsCurrentPtr current);
	
    int transferGoodsStock(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::TransferGoodsStockReq& stReq, HardwareApplet::TransferGoodsStockRsp& stRsp, tars::TarsCurrentPtr current);
private:
    int insertSPUCategoryMediaInfo__(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::AddGoodsSPUInfoReq& stReq);
    int insertGoodsSPUInfo__(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::AddGoodsSPUInfoReq& stReq, int &spuid);
    int insertGoodsCategoryInfo__(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::AddGoodsSPUInfoReq& stReq, const int spuid);
    int insertGoodsMediaInfo__(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::AddGoodsSPUInfoReq& stReq, const int spuid);


    int insertSKUAndSKUValueInfo__(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::AddGoodsSKUInfoReq& stReq);
    int insertGoodsSKUInfo__(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::AddGoodsSKUInfoReq& stReq, int &skuid);
    int insertSKUValueInfo__(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::AddGoodsSKUInfoReq& stReq, const int skuid);
    int insertSKUStockInfo__(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::AddGoodsSKUInfoReq& stReq, const int skuid);

    int transferGoodsStock__(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::TransferGoodsStockReq& stReq);
    int reduceOldStock__(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::TransferGoodsStockReq& stReq);
    int increaseNewStock__(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::TransferGoodsStockReq& stReq);
private:
    TC_Mysql * _mysql;
};

#endif /*GOODS_COMMAND_H_*/
