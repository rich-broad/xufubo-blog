/*********************************************************************************
 FileName: OpCommand.h
 Author: xufubo
 Date:  2019-01-01
 Description:
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/

#ifndef OP_COMMAND_H_
#define OP_COMMAND_H_

#include "Command.h"
#include "Global.h"
#include "AppletMallCommon.h"

class OpCommand : public Command
{
public:
	OpCommand(ConfigurationFactory* configurationFactory, DependencedServiceFactory* serviceFactory);
	virtual ~OpCommand();
	//int getAppList(const AppletCommHead& stHead, const HardwareApplet::GetAppListReq& stReq, HardwareApplet::GetAppListRsp& stRsp, tars::TarsCurrentPtr current);

private:
	

private:
	TC_Mysql _pMysql;

};

/*
class OpCommandCallback : public HardwareApplet::OpPrxCallback, public HardwareApplet::CFTBusiAppPrxCallback
{
public:
	OpCommandCallback(tars::TarsCurrentPtr current, const HardwareApplet::AppletCommHead & stHead):_current(current),_stHead(stHead){}

	OpCommandCallback(tars::TarsCurrentPtr current,const HardwareApplet::AppletCommHead & stHead,
		const HardwareApplet::GetAppListReq & stGetAppListReq, DependencedServiceFactory* pServiceFactory)
		:_current(current),_stHead(stHead),_stGetAppListReq(stGetAppListReq),_pServiceFactory(pServiceFactory){}
	~OpCommandCallback(){}

	virtual void callback_getAppList(taf::Int32 ret, const vector<taf::Char>& vtOut);
	virtual void callback_getAppList_exception(taf::Int32 ret);

private:
	tars::TarsCurrentPtr _current;
	HardwareApplet::AppletCommHead _stHead;
	HardwareApplet::GetAppListReq _stGetAppListReq;
	DependencedServiceFactory* _pServiceFactory;
};
typedef TC_AutoPtr<OpCommandCallback> OpCommandCallbackPtr;
*/
#endif
