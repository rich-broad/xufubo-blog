/*********************************************************************************
 FileName: AppCommand.h
 Author: xufubo
 Date:  2019-01-01
 Description:
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/

#ifndef APP_COMMAND_H_
#define APP_COMMAND_H_

#include "Command.h"
#include "Global.h"
#include "CommonTemplate.h"

class AppCommand : public Command
{
public:
	AppCommand(ConfigurationFactory* configurationFactory, DependencedServiceFactory* serviceFactory);
	virtual ~AppCommand();

	//int getAppDetailBatch(const AppletCommHead& stHead, const HardwareApplet::GetAppDetailBatchReq& stReq, HardwareApplet::GetAppDetailBatchRsp& stRsp, tars::TarsCurrentPtr current);
	
private:
};

#endif /*APP_COMMAND_H_*/
