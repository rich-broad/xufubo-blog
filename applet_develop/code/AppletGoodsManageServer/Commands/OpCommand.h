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
#include "AppletGoodsManage.h"

class OpCommand : public Command
{
public:
	OpCommand(ConfigurationFactory* configurationFactory, DependencedServiceFactory* serviceFactory);
	virtual ~OpCommand();
	//int getCategoryList(const AppletCommHead& stHead, const HardwareApplet::GetCategoryListReq& stReq, HardwareApplet::GetCategoryListRsp& stRsp, tars::TarsCurrentPtr current);

private:

};

/*
class OpCommandCallback : public HardwareApplet::OpPrxCallback, public HardwareApplet::CFTBusiAppPrxCallback
{
public:
	OpCommandCallback(tars::TarsCurrentPtr current, const HardwareApplet::AppletCommHead & stHead):_current(current),_stHead(stHead){}

	OpCommandCallback(tars::TarsCurrentPtr current,const HardwareApplet::AppletCommHead & stHead,
		const MAAccessExt::GetAppListReq & stGetAppListReq, DependencedServiceFactory* pServiceFactory)
		:_current(current),_stHead(stHead),_stGetAppListReq(stGetAppListReq),_pServiceFactory(pServiceFactory){}
	~OpCommandCallback(){}

	virtual void callback_getAppList(taf::Int32 ret, const vector<taf::Char>& vtOut);
	virtual void callback_getAppList_exception(taf::Int32 ret);

private:
	tars::TarsCurrentPtr _current;
	HardwareApplet::AppletCommHead _stHead;
	MAAccessExt::GetAppListReq _stGetAppListReq;
	DependencedServiceFactory* _pServiceFactory;
};
typedef TC_AutoPtr<OpCommandCallback> OpCommandCallbackPtr;
*/
#endif
