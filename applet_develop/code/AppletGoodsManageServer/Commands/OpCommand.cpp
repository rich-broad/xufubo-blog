/*********************************************************************************
 FileName: OpCommand.cpp
 Author: xufubo
 Date:  2019-01-01
 Description:
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/

#include "OpCommand.h"

OpCommand::OpCommand(ConfigurationFactory* configurationFactory, DependencedServiceFactory* serviceFactory)
	:Command(configurationFactory, serviceFactory)
{
}

OpCommand::~OpCommand()
{
}

/*
int OpCommand::getCategoryList(const AppletCommHead& stHead, const HardwareApplet::GetCategoryListReq& stReq, HardwareApplet::GetCategoryListRsp& stRsp, tars::TarsCurrentPtr current)
{
    ostringstream ossStr;
    ossStr << stReq.level;
    DEBUGLOG(COMM_HEAD_ALL_INFO(stHead) << "|" << ossStr.str()) << endl;
    return 0;
}
*/

/*

void OpCommandCallback::callback_getAppList(taf::Int32 ret, const vector<taf::Char>& vtOut)
{
	HardwareApplet::GetAppListRsp response;
	HardwareApplet::GetAppListResponse stRsp;
	int decodeRet = JceDecode<HardwareApplet::GetAppListResponse>(vtOut, stRsp);
	if (decodeRet != 0)
	{
		ret = DECODE_ERR;
	}
	else if (ret == 0)
	{
		for (uint32_t i = 0; i < stRsp.appList.size(); ++i)
		{
			HardwareApplet::CardItem & ci = stRsp.appList[i];
			HardwareApplet::ListItem li;
			CardItem2MAAccessListItem(ci, li, _stHead.phoneGuid);
			response.appList.push_back(li);
		}
		response.contextData = stRsp.pageContext;
		response.hasNext = stRsp.hasNext;
		response.ret = stRsp.ret;
	}

	vector<char> vtResponse;
	if (ret == 0 && JceEncode<HardwareApplet::GetAppListRsp>(response, vtResponse) != 0)
	{
		ret = ENCODE_ERR;
	}
	CommonExternal::async_response_getAppList(_current, ret, vtResponse);
	TRLOG << "Res|" << _stHead.qua << "|" << _stHead.phoneGuid << "|" << ret << "|" << _stGetAppListReq.listType << "|" << _stGetAppListReq.pageSize << "|" << response.hasNext << "|" << response.appList.size() << response.contextData.size() << endl;
	DAYLOG << "getAppList|Res|" << _stHead.qua << "|" << _stHead.phoneGuid << "|" << ret << "|" << _stGetAppListReq.listType << "|" << _stGetAppListReq.pageSize << "|" << response.hasNext << "|" << response.appList.size() << response.contextData.size() << endl;
}

void OpCommandCallback::callback_getAppList_exception(taf::Int32 ret)
{
	vector<char> vtOut;
	CommonExternal::async_response_getAppList(_current, ret, vtOut);
	TRLOG << "Res|exception|" << ret << "|" << _stHead.qua << "|" << _stHead.phoneGuid << "|" << ret << "|" << _stGetAppListReq.listType << "|" << _stGetAppListReq.pageSize << endl;
	TERRLOG("getAppList|Res|exception|" << ret << "|" << _stHead.qua << "|" << _stHead.phoneGuid << "|" << ret << "|" << _stGetAppListReq.listType << "|" << _stGetAppListReq.pageSize) << endl;
}
*/