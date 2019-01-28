/*********************************************************************************
 FileName: AuthCommand.h
 Author: xufubo
 Date:  2018-12-29
 Description:
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/
#ifndef _PUSH_COMMAND_H_
#define _PUSH_COMMAND_H_

#include "Command.h"
#include "Global.h"
#include "AppletAuth.h"
#include "Configuration.h"

class AuthCommand : public Command
{
public:
	AuthCommand(ConfigurationFactory* configurationFactory, TC_Mysql * db_Info);
	virtual ~AuthCommand();
    int getNewTicket(const HardwareApplet::AppletCommHead &stHead, const HardwareApplet::GetNewTicketReq &request, HardwareApplet::GetNewTicketRsp &response, tars::TarsCurrentPtr current);
    int getNewCookie(const HardwareApplet::AppletCommHead &stHead, const HardwareApplet::GetNewCookieReq &request, HardwareApplet::GetNewCookieRsp &response, tars::TarsCurrentPtr current);

private:
    TC_Mysql * _db_Info;
};

class AsyncHttpCallback : public TC_HttpAsync::RequestCallback
{
public:
	AsyncHttpCallback(const string &sUrl, const HardwareApplet::AppletCommHead &stHead, const HardwareApplet::GetNewTicketReq &request,
        const string &funcName, TC_Mysql * db_Info, tars::TarsCurrentPtr current) : 
		_sUrl(sUrl),_stHead(stHead),_request(request),_funcName(funcName), _db_Info(db_Info), _current(current){}

	virtual void onException(const string &ex);
	virtual void onResponse(bool bClose, TC_HttpResponse &stHttpResponse);
	virtual void onTimeout();
	virtual void onClose();

private:
    inline int parseCode2SessionRsp(const string & content, HardwareApplet::WXJSCodeToSessionRsp & rsp);
    int makeST(const HardwareApplet::WXJSCodeToSessionRsp &wxrsp, string &st, HardwareApplet::SecurityTicket &sST);
    int updateUserInfo(const HardwareApplet::WXJSCodeToSessionRsp &wxrsp, const HardwareApplet::SecurityTicket &sST);
    int updateUserLoginInfo(const HardwareApplet::WXJSCodeToSessionRsp &wxrsp, const HardwareApplet::SecurityTicket &sST, const int uid);
    int updateUserBaseInfo(const HardwareApplet::WXJSCodeToSessionRsp &wxrsp, const HardwareApplet::SecurityTicket &sST, int & uid);

protected:
	string _sUrl;
	HardwareApplet::AppletCommHead _stHead;
	HardwareApplet::GetNewTicketReq _request;
    string _funcName;
    TC_Mysql * _db_Info;
    tars::TarsCurrentPtr _current;
};

#endif /*_PUSH_COMMAND_H_*/
