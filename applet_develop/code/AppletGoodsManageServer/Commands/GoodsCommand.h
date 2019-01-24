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
	
private:
    TC_Mysql * _mysql
};

#endif /*GOODS_COMMAND_H_*/
