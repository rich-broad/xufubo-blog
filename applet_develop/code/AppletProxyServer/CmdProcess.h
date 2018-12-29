
/*********************************************************************************
 FileName: CmdProcess.h
 Author: xufubo
 Date:  2018-12-26
 Description:
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/
#include "AppletContext.h"


class CmdProcess
{
public:
	CmdProcess()
	{

	}

	~CmdProcess()
	{

	}
	int32_t excute(AppletContextPtr ctx);
};

class CmdProcessCallback: public tars::ServantProxyCallback
{
public:
	CmdProcessCallback(AppletContextPtr ctx):_ctx(ctx)
	{

	}
	~CmdProcessCallback()
	{
	
	}
	int32_t onDispatch(ReqMessagePtr msg);
	void callback_excuteCmd(int ret, const string& funcName, const vector<char>& cmdRsp);
	void callback_excuteCmd_exception(int ret, const string& funcName);

private:
	AppletContextPtr _ctx;
};

typedef tars::TC_AutoPtr<CmdProcessCallback> CmdProcessCallbackPtr;