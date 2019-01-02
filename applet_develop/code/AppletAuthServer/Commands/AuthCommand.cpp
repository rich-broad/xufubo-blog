/*********************************************************************************
 FileName: AuthCommand.cpp
 Author: xufubo
 Date:  2018-12-29
 Description:
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/
#include "AuthCommand.h"
#include "AppletAuthServer.h"

using namespace std;
using namespace rapidjson;
using namespace tars;
using namespace HardwareApplet;

AuthCommand::AuthCommand(ConfigurationFactory* configurationFactory, TC_Mysql * db_Info)
	:Command(configurationFactory), _db_Info(db_Info)
{

}

AuthCommand::~AuthCommand()
{

}

int AuthCommand::getNewTicket(const HardwareApplet::AppletCommHead &stHead, const HardwareApplet::GetNewTicketReq &request, HardwareApplet::GetNewTicketRsp &response, tars::TarsCurrentPtr current)
{
    DEBUGLOG("getNewTicket|" << COMM_HEAD_ALL_INFO(stHead) << "|" << request.wx_code << endl);

    TC_HttpRequest stHttpReq;

    string sUrl( DEF_CFG_SINGLETON->_wxProxyIpPort + "/sns/jscode2session?appid=" + DEF_CFG_SINGLETON->_appid + "&secret=" + DEF_CFG_SINGLETON->_appSecret + "&js_code=" + request.wx_code + "&grant_type=authorization_code");
    
    stHttpReq.setGetRequest(sUrl);
    DEBUGLOG(COMM_HEAD_ALL_INFO(stHead) << "|" << "url|" << sUrl<< endl);

    int ret = -100;
    try
    {
        TC_HttpAsync::RequestCallbackPtr callbackPtr = new AsyncHttpCallback(sUrl, stHead, request, "getNewTicket", _db_Info, current);
        ret = g_app.httpAsync.doAsyncRequest(stHttpReq, callbackPtr);
    }
    __CATCH_EXCEPTION_WITH__("getNewTicket exception|" << COMM_HEAD_ALL_INFO(stHead));
    if (ret)
    {
        ERRORLOG("SendHttpReqError|" << COMM_HEAD_ALL_INFO(stHead) << "|" << sUrl << "|" << endl);
        DEBUGLOG("SendHttpReqError|" << COMM_HEAD_ALL_INFO(stHead) << "|" << sUrl << "|" << endl);
        return ret;
    }
    
    DEBUGLOG("SendHttpReqSuccess|" << COMM_HEAD_ALL_INFO(stHead) << "|" << sUrl << "|" << endl);
    return ret;
}

void AsyncHttpCallback::onException(const string &ex)
{
	DEBUGLOG("onException|Because|" << ex << "|" << _sUrl << "|" << COMM_HEAD_ALL_INFO(_stHead) << "|" << endl);
	ERRORLOG("onException|Because|" << ex << "|" << _sUrl << "|" << COMM_HEAD_ALL_INFO(_stHead) << "|" << endl);
}

void AsyncHttpCallback::onResponse(bool bClose, TC_HttpResponse &stHttpResponse)
{
	int status = stHttpResponse.getStatus();
	string content = stHttpResponse.getContent();
	DEBUGLOG("onResponse|status|" << status << "|" << _sUrl << "|" << COMM_HEAD_ALL_INFO(_stHead) << "|" << content << "|" << endl);
    if (status != 200)
    {
        ERRORLOG("onResponse|status|" << status << "|" << _sUrl << "|" << content << "|" << COMM_HEAD_ALL_INFO(_stHead) << "|" << endl);
        DEBUGLOG("onResponse|status|" << status << "|" << _sUrl << "|" << content << "|" << COMM_HEAD_ALL_INFO(_stHead) << "|" << endl);
        return;
    }
    HardwareApplet::GetNewTicketRsp response;
    WXJSCodeToSessionRsp wxrsp;
    int ret = -99;
    try
    {
        ret = parseCode2SessionRsp(content, wxrsp);
        if (ret || wxrsp.errcode)
        {
            ERRORLOG("onResponse|code2session error|" << status << "|" << _sUrl << "|" << content << "|" << COMM_HEAD_ALL_INFO(_stHead) << "|" << endl);
            response.ret = ret;
            response.errmsg = wxrsp.errmsg;

            DEBUGLOG(ret << "|" << response.errmsg << "|" << wxrsp.errmsg << "|================" << endl);
            sendReponse(response, _stHead, _funcName, ret, _current);
            return;
        }
        HardwareApplet::SecurityTicket sST;
        response.ret = makeST(wxrsp, response.st, sST);
        //后续支持事务，现在看没必要
        ret = updateUserInfo(wxrsp, sST);
        if (ret)
        {
            ERRORLOG("updateUserInfo error|" << endl);
        }
        sendReponse(response, _stHead, _funcName, ret, _current);
        return;
    }
    __CATCH_EXCEPTION_WITH__("HttpResponseBody|ParseError");

    if (0 != ret)
    {
        ERRORLOG("onResponse|HttpResponseBody|ParseError|" << status << "|" << content << "|" << _sUrl << "|" << COMM_HEAD_ALL_INFO(_stHead) << "|" << endl);
        DEBUGLOG("onResponse|HttpResponseBody|ParseError|" << status << "|" << content << "|" << _sUrl << "|" << COMM_HEAD_ALL_INFO(_stHead) << "|" << endl);
        response.ret = ret;
        response.errmsg = "parse wx respone error";
        sendReponse(response, _stHead, _funcName, ret, _current);
    }
}

int AsyncHttpCallback::makeST(const WXJSCodeToSessionRsp &wxrsp, string &st, HardwareApplet::SecurityTicket &sST)
{
    
    sST.timets = TNOW;
    string openidSessionKey = wxrsp.openid + wxrsp.session_key + L2S(sST.timets);
    sST.sessionKey = TC_SHA::sha256str(openidSessionKey.data(), openidSessionKey.length());
    sST.signature = TC_MD5::md5str(L2S(sST.timets) + sST.sessionKey + DEF_CFG_SINGLETON->_SVRMD5SIGKEY);

    string bsst;
    int ret = TarsEncode(sST, bsst);
    if (ret)
    {
        ERRORLOG("TarsEncode error|" << _sUrl << "|" << COMM_HEAD_ALL_INFO(_stHead) << "|" << endl);
        return ret;
    }

    vector<char> vtST = TC_Tea::encrypt2(DEF_CFG_SINGLETON->_SVRKEY.c_str(), bsst.c_str(), bsst.length());
    string tempBase64In(vtST.begin(), vtST.end());
    st = TC_Base64::encode(tempBase64In);
    return 0;
}

void AsyncHttpCallback::onTimeout()
{
	DEBUGLOG("onTimeout|" << _sUrl << "|" << COMM_HEAD_ALL_INFO(_stHead) << "|" << endl);
	ERRORLOG("onTimeout|" << _sUrl << "|" << COMM_HEAD_ALL_INFO(_stHead) << "|" << endl);
}

void AsyncHttpCallback::onClose()
{
	DEBUGLOG("onClose|" << _sUrl << "|" << COMM_HEAD_ALL_INFO(_stHead) << "|" << endl);
}

int AsyncHttpCallback::parseCode2SessionRsp(const string & content, HardwareApplet::WXJSCodeToSessionRsp & wxrsp)
{
    Document document;
    document.Parse(content.c_str());

    Value::ConstMemberIterator errCodeiter = document.FindMember("errcode");
    if (errCodeiter != document.MemberEnd())
    {
        wxrsp.errcode = errCodeiter->value.GetInt();
    }
    
    Value::ConstMemberIterator openidCodeiter = document.FindMember("openid");
    if (openidCodeiter != document.MemberEnd())
    {
        wxrsp.openid = openidCodeiter->value.GetString();
    }

    Value::ConstMemberIterator sessionKeyCodeiter = document.FindMember("session_key");
    if (sessionKeyCodeiter != document.MemberEnd())
    {
        wxrsp.session_key = sessionKeyCodeiter->value.GetString();
    }

    Value::ConstMemberIterator unionidCodeiter = document.FindMember("unionid");
    if (unionidCodeiter != document.MemberEnd())
    {
        wxrsp.unionid = unionidCodeiter->value.GetString();
    }

    Value::ConstMemberIterator msgCodeiter = document.FindMember("errmsg");
    if (msgCodeiter != document.MemberEnd())
    {
        DEBUGLOG(msgCodeiter->value.GetString() << endl);
        wxrsp.errmsg = msgCodeiter->value.GetString();
        DEBUGLOG(wxrsp.errmsg << endl;);
        DEBUGLOG(msgCodeiter->value.GetString() << endl);
    }

    if (errCodeiter == document.MemberEnd())
    {
        return -1;
    }
    return wxrsp.errcode;
}

int AsyncHttpCallback::updateUserInfo(const HardwareApplet::WXJSCodeToSessionRsp &wxrsp, const HardwareApplet::SecurityTicket &sST)
{
    int ret = 0;
    //更新用户信息先封装成两个函数，因为将来用户登录信息可能优化为nosql
    ret = updateUserBaseInfo(wxrsp, sST);
    if (ret)
    {
        ERRORLOG("updateUserBaseInfo error|" << endl);
        return ret;
    }
    ret = updateUserLoginInfo(wxrsp, sST);
    if (ret)
    {
        ERRORLOG("updateUserLoginInfo error|" << endl);
        return ret;
    }
    
    return 0;
}
int AsyncHttpCallback::updateUserBaseInfo(const HardwareApplet::WXJSCodeToSessionRsp &wxrsp, const HardwareApplet::SecurityTicket &sST)
{
    int ret = -1;
    ostringstream sqlStr;
    
    try
    {
        sqlStr << "replace into t_user_base_data " << "(" << USER_BASE_DATA_COLUMN_NOT_ID << ") values "
            << "("
            << "'" << _db_Info->escapeString(wxrsp.openid) << "'" << ","
            << TNOW
            << ")";
        DEBUGLOG("sql = " << sqlStr.str() << endl);
        // execute在mysql执行不成功的时候会抛出异常因此只要抛出了异常说明execute不成功
        _db_Info->execute(sqlStr.str());
        ret = 0;
    }
    __CATCH_EXCEPTION_WITH__("MySQL_Exception");
    
    if (ret)
    {
        ERRORLOG("exec sql error|" << sqlStr.str() << endl);
    }
    return ret;
}

int AsyncHttpCallback::updateUserLoginInfo(const HardwareApplet::WXJSCodeToSessionRsp &wxrsp, const HardwareApplet::SecurityTicket &sST)
{
    int ret = 0;
    ostringstream sqlStr;

    try
    {
        sqlStr << "replace into t_user_login_data " << "(" << USER_LOGIN_DATA_ALL_COLUMN << ") values "
            << "("
            << "'" << _db_Info->escapeString(sST.sessionKey) << "'" << ","
            << "'" << _db_Info->escapeString(wxrsp.openid) << "'" << ","
            << "'" << _db_Info->escapeString(wxrsp.session_key) << "'" << ","
            << sST.timets
            << ")";
        DEBUGLOG("sql = " << sqlStr.str() << endl);
        // execute在mysql执行不成功的时候会抛出异常因此只要抛出了异常说明execute不成功
        _db_Info->execute(sqlStr.str());
        ret = 0;
    }
    __CATCH_EXCEPTION_WITH__("MySQL_Exception");

    if (ret)
    {
        ERRORLOG("exec sql error|" << sqlStr.str() << endl);
    }
    return ret;
}

