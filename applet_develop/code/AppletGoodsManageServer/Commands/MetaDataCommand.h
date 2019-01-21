/*********************************************************************************
 FileName: MetaDataCommand.h
 Author: xufubo
 Date:  2019-01-01
 Description:
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/

#ifndef METADATA_COMMAND_H_
#define METADATA_COMMAND_H_

#include "Command.h"
#include "Global.h"
#include "AppletGoodsManage.h"

class MetaDataCommand : public Command
{
public:
	MetaDataCommand(ConfigurationFactory* configurationFactory, DependencedServiceFactory* serviceFactory, TC_Mysql * mysql);
	virtual ~MetaDataCommand();
	int getCategoryList(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::GetCategoryListReq& stReq, HardwareApplet::GetCategoryListRsp& stRsp, tars::TarsCurrentPtr current);
    int addCategoryInfo(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::AddCategoryInfoReq& stReq, HardwareApplet::AddCategoryInfoRsp& stRsp, tars::TarsCurrentPtr current);

    int getAttributeList(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::GetAttributeListReq& stReq, HardwareApplet::GetAttributeListRsp& stRsp, tars::TarsCurrentPtr current);
    int addAttributeInfo(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::AddAttributeInfoReq& stReq, HardwareApplet::AddAttributeInfoRsp& stRsp, tars::TarsCurrentPtr current);

    int getAttributeValueList(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::GetAttributeValueListReq& stReq, HardwareApplet::GetAttributeValueListRsp& stRsp, tars::TarsCurrentPtr current);
    int addAttributeValueInfo(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::AddAttributeValueInfoReq& stReq, HardwareApplet::AddAttributeValueInfoRsp& stRsp, tars::TarsCurrentPtr current);

    int getBrandList(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::GetBrandListReq& stReq, HardwareApplet::GetBrandListRsp& stRsp, tars::TarsCurrentPtr current);
    int addBrandInfo(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::AddBrandInfoReq& stReq, HardwareApplet::AddBrandInfoRsp& stRsp, tars::TarsCurrentPtr current);

    int getMakerList(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::GetMakerListReq& stReq, HardwareApplet::GetMakerListRsp& stRsp, tars::TarsCurrentPtr current);
    int addMakerInfo(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::AddMakerInfoReq& stReq, HardwareApplet::AddMakerInfoRsp& stRsp, tars::TarsCurrentPtr current);

private:
	
    TC_Mysql * _mysql;
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
