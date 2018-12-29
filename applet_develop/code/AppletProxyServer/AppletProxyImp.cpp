
/*********************************************************************************
 FileName: AppletProxyImp.cpp
 Author: xufubo
 Date:  2018-12-26
 Description:
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/
#include "AppletProxyImp.h"
#include "servant/Application.h"
#include "AppletCommUtils.h"


using namespace std;
using namespace rapidjson;
using namespace tars;
using namespace HardwareApplet;

//////////////////////////////////////////////////////
void AppletProxyImp::initialize()
{
	_pCmdProcess = new CmdProcess();
}

void AppletProxyImp::destroy()
{
	delete _pCmdProcess;
}

int AppletProxyImp::doRequest(tars::TarsCurrentPtr current, vector<char>& response)
{
    DEBUGLOG("doRequest enter: " << endl);
	current->setResponse(false);
	const vector<char> &httpReqData = current->getRequestBuffer();
    DEBUGLOG("FirstEnter|" << current->getIp() << "|" << current->getPort() << "|" << httpReqData.size() << endl);
	AppletContextPtr ctx = new AppletContext(current);
    	int ret = ctx->parseRequestData(httpReqData);
	if (ret != 0)
	{
		ERRORLOG("illegal request|parse err|" << ret << "|" 
            << REQ_HEAD_ALL_INFO(ctx->_reqHead) << "|"
            << ctx->_beginTime << "|"
            << ctx->_clienIp << "|"
            << ctx->_funcName << "|"
            << AppletCommUtils::Value2Str(ctx->_reqJsonBody) << endl); 
	}

	ctx->logRequset();

	if (ret != 0)
	{
		ctx->respond();
		ctx->logResponse();
		ctx->reportMonitorMsg("mul_parseErr_" + ctx->_funcName, tars::StatReport::STAT_SUCC, ctx->_rspHead.ret);
		DEF_CFG_SINGLETON->_pParseErrPropery->report(1);
		return ret;
	}	
	ret = _pCmdProcess->excute(ctx);
	if (ret != 0)
	{
		ctx->respond();
		ctx->logResponse();
		ctx->reportMonitorMsg("mul_" + ctx->_funcName, tars::StatReport::STAT_EXCE, ctx->_rspHead.ret);
	}
	
	return ret;
}

