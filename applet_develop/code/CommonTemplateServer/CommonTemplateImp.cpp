/*********************************************************************************
 FileName: 
 Author: xufubo
 Date:  2019-01-01
 Description:
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/

#include "CommonTemplateImp.h"

using namespace std;
using namespace rapidjson;
using namespace tars;
using namespace HardwareApplet;

void CommonTemplateImp::initialize()
{
    _pServiceFactory->Initialize(_pConfigurationFactory);
	_pAppCommand = new AppCommand(_pConfigurationFactory, _pServiceFactory);
	_pOpCommand =new OpCommand(_pConfigurationFactory, _pServiceFactory);
}

void CommonTemplateImp::destroy()
{
	if (_pAppCommand != NULL)
	{
		delete _pAppCommand;
		_pAppCommand = NULL;
	}
	if (_pOpCommand != NULL)
	{
		delete _pOpCommand;
		_pOpCommand = NULL;
	}
}

/*
int CommonTemplateImp::getAppList(const HardwareApplet::AppletCommHead & stHead,const vector<tars::Char> & vtIn,vector<tars::Char> &vtOut,tars::TarsCurrentPtr current)
{
	HardwareApplet::GetAppListReq stReq;
	HardwareApplet::GetAppListRsp stRsp;
	int ret = 0;

	if (HardwareApplet::TarsDecode<HardwareApplet::GetAppListReq>(vtIn, stReq) != 0)
	{
		ERRORLOG(stHead.phoneGuid << "|" << vtIn.size()) << endl;
		stRsp.ret = -1;
		return -1;
	}
	current->setResponse(false);
	ret = _pOpCommand->getAppList(stHead, stReq, stRsp, current);

	return ret;
}

tars::Int32 CommonTemplateImp::getGameAppList(const HardwareApplet::AppletCommHead & stHead, const vector<tars::Char> & vtIn, vector<tars::Char> &vtOut, tars::TarsCurrentPtr current)
{
	HardwareApplet::GetGameAppListReq stReq;
	HardwareApplet::GetGameAppListRsp stRsp;
	int ret = 0;

	if (HardwareApplet::TarsDecode<HardwareApplet::GetGameAppListReq>(vtIn, stReq) != SUCCESS)
	{
		ERRORLOG(stHead.phoneGuid << "|" << vtIn.size()) << endl;
		stRsp.ret = DECODE_ERR;
		return DECODE_ERR;
	}
	current->setResponse(false);
	FDLOG("DEBUG") <<"|adposId:" << stReq.adposId <<", sence:" << stReq.sence << endl;

	ret = _pAppCommand->getGameAppList(stHead, stReq, stRsp, current);
    return ret;
}
*/
