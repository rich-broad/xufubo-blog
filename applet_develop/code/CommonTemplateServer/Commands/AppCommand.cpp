/*********************************************************************************
 FileName: AppCommand.cpp
 Author: xufubo
 Date:  2019-01-01
 Description:
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/

#include "AppCommand.h"
#include "CommonTemplateServer.h"

AppCommand::AppCommand(ConfigurationFactory* configurationFactory, DependencedServiceFactory* serviceFactory)
	:Command(configurationFactory, serviceFactory)
{

}

AppCommand::~AppCommand()
{

}

/*
int AppCommand::getAppDetailBatch(const AppletCommHead& stHead, const HardwareApplet::GetAppDetailBatchReq& stReq, HardwareApplet::GetAppDetailBatchRsp& stRsp, tars::TarsCurrentPtr current)
{
    int ret = 0;
    vector<char> vtOut;
    HardwareApplet::GetAppDetailBatchRsp response;
    response.ret = 0;
    sendReponse(response, stHead, "getAppDetailBatch", ret, current)

    return ret;
}
*/
