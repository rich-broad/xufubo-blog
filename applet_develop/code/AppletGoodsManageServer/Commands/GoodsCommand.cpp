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

GoodsCommand::GoodsCommand(ConfigurationFactory* configurationFactory, DependencedServiceFactory* serviceFactory)
	:Command(configurationFactory, serviceFactory)
{

}

GoodsCommand::~GoodsCommand()
{

}

int GoodsCommand::getGoodsSPUList(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::GetGoodsSPUListReq& stReq, HardwareApplet::GetGoodsSPUListRsp& stRsp, tars::TarsCurrentPtr current)
{
    ostringstream ossStr;
    //ossStr << stReq.level;
    DEBUGLOG(COMM_HEAD_ALL_INFO(stHead) << "|" << ossStr.str() << endl);
    int ret = -1;
    try
    {
        HardwareApplet::GetGoodsSPUListRsp response;
        sendReponse(response, stHead, "getGoodsSPUList", ret, current);
    }
    __CATCH_EXCEPTION_WITH__("MetaDataCommand::getGoodsSPUList");
    return 0;
}

int GoodsCommand::addGoodsSPUInfo(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::AddGoodsSPUInfoReq& stReq, HardwareApplet::AddGoodsSPUInfoRsp& stRsp, tars::TarsCurrentPtr current)
{
    ostringstream ossStr;
    //ossStr << stReq.level;
    DEBUGLOG(COMM_HEAD_ALL_INFO(stHead) << "|" << ossStr.str() << endl);
    int ret = -1;
    try
    {
        HardwareApplet::AddGoodsSPUInfoRsp response;
        sendReponse(response, stHead, "addGoodsSPUInfo", ret, current);
    }
    __CATCH_EXCEPTION_WITH__("MetaDataCommand::addGoodsSPUInfo");
    return 0;
}
