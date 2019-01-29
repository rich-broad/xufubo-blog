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
        sendReponse(response, stHead, "getNewTicket", ret, current);
        return ret;
    }
    
    DEBUGLOG("SendHttpReqSuccess|" << COMM_HEAD_ALL_INFO(stHead) << "|" << sUrl << "|" << endl);
    return ret;
}

void AsyncHttpCallback::onException(const string &ex)
{
	DEBUGLOG("onException|Because|" << ex << "|" << _sUrl << "|" << COMM_HEAD_ALL_INFO(_stHead) << "|" << endl);
	ERRORLOG("onException|Because|" << ex << "|" << _sUrl << "|" << COMM_HEAD_ALL_INFO(_stHead) << "|" << endl);
    HardwareApplet::GetNewTicketRsp response;
    sendReponse(response, _stHead, _funcName, -1, _current);
}

void AsyncHttpCallback::onResponse(bool bClose, TC_HttpResponse &stHttpResponse)
{
	int status = stHttpResponse.getStatus();
	string content = stHttpResponse.getContent();
	DEBUGLOG("onResponse|status|" << status << "|" << _sUrl << "|" << COMM_HEAD_ALL_INFO(_stHead) << "|" << content << "|" << endl);
    HardwareApplet::GetNewTicketRsp response;
    if (status != 200)
    {
        ERRORLOG("onResponse|status|" << status << "|" << _sUrl << "|" << content << "|" << COMM_HEAD_ALL_INFO(_stHead) << "|" << endl);
        sendReponse(response, _stHead, _funcName, -1, _current);
        return;
    }
    
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
        ret = makeST(wxrsp, response.st, sST);
        if (ret != 0)
        {
            response.ret = ret;
            ERRORLOG_WITHHEAD(_stHead, "|onResponse|makeST error|" << status << "|" << _sUrl << "|" << content << "|" << endl);
            sendReponse(response, _stHead, _funcName, ret, _current);
            return;
        }
        
		// 该函数具有事务性
        ret = updateUserInfo(wxrsp, sST);
        if (ret)
        {
            ERRORLOG("updateUserLoginInfo error|" << endl);
        }
        response.ret = ret;
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
    HardwareApplet::GetNewTicketRsp response;
    sendReponse(response, _stHead, _funcName, -1, _current);
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
    if (errCodeiter == document.MemberEnd())
    {
        return -1;
    }
    wxrsp.errcode = errCodeiter->value.GetInt();
    if (wxrsp.errcode != 0)
    {
        return wxrsp.errcode;
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
        wxrsp.errmsg = msgCodeiter->value.GetString();
    }

    if (wxrsp.openid.empty() || wxrsp.session_key.empty() || wxrsp.unionid.empty())
    {
        ERRORLOG(wxrsp.openid << "|" << wxrsp.session_key << "|" << wxrsp.unionid << "|");
        return -1;
    }
    return 0;
}


int AsyncHttpCallback::updateUserInfo(const HardwareApplet::WXJSCodeToSessionRsp &wxrsp, const HardwareApplet::SecurityTicket &sST)
{
    int ret = -1;
    int uid = 0;
    // 在连接_db_Info上执行事务
    AppletTransaction transaction(_db_Info);
    try
    {
        if ((ret = transaction.Start()) != 0)
        {
            ERRORLOG("transaction.Start error|" << endl);
            return ret;
        }

        ret = updateUserBaseInfo(wxrsp, sST, uid);
        if (ret != 0)
        {
            ERRORLOG("updateUserBaseData error|" << endl);
            transaction.Rollback();
            return ret;
        }

        ret = updateUserLoginInfo(wxrsp, sST, uid);
        if (ret != 0)
        {
            ERRORLOG("updateUserLoginInfo error|" << endl);
            transaction.Rollback();
            return ret;
        }
        transaction.Commit();
    }
    catch (...)
    {
        ERRORLOG("exception unknow|" << "updateUserInfo" << endl);
        transaction.Rollback();
        ret = -1;
    }

    return ret;
}


int AsyncHttpCallback::updateUserBaseInfo(const HardwareApplet::WXJSCodeToSessionRsp &wxrsp, const HardwareApplet::SecurityTicket &sST, int & uid)
{
    int ret = -1;
    ostringstream sqlStr;
    try
    {
        sqlStr << "replace into t_user_base_data " << "(" << USER_BASE_DATA_COLUMN_NOT_ID << ") values "
            << "("
            << "'" << _db_Info->escapeString(wxrsp.unionid) << "'" << ","
            << TNOW
            << ")";
        DEBUGLOG("sql = " << sqlStr.str() << endl);
        // execute在mysql执行不成功的时候会抛出异常因此只要抛出了异常说明execute不成功
        _db_Info->execute(sqlStr.str());
        size_t affected_rows = _db_Info->getAffectedRows();

        if (affected_rows != 1)
        {
            ERRORLOG("insert error. affected_rows|" << affected_rows << endl);
            return -1;
        }

        ret = 0;
    }
    __CATCH_EXCEPTION_WITH__("MySQL_Exception");
    
    if (ret != 0)
    {
        ERRORLOG("exec sql error|" << sqlStr.str() << endl);
        return ret;
    }
    sqlStr.str(""); sqlStr.clear();
    sqlStr << "select `uid` from t_user_base_data where `union_id` = " << "'" << _db_Info->escapeString(wxrsp.unionid) << "'";

    TC_Mysql::MysqlData data = _db_Info->queryRecord(sqlStr.str());
    if (data.size() != 1)
    {
        ERRORLOG("data.size() != 1 error|" << sqlStr.str() << endl);
        return -1;
    }
    TC_Mysql::MysqlRecord record = data[0];
    uid = TC_Common::strto<int>(record["uid"]);
    return 0;
}

//#define USER_LOGIN_DATA_ALL_COLUMN  "`uid`, `union_id`, `custom_session_key`, `open_id`, `session_key`, `session_key_time`"
int AsyncHttpCallback::updateUserLoginInfo(const HardwareApplet::WXJSCodeToSessionRsp &wxrsp, const HardwareApplet::SecurityTicket &sST, const int uid)
{
    int ret = -1;
    ostringstream sqlStr;

    try
    {
        sqlStr << "replace into t_user_login_data " << "(" << USER_LOGIN_DATA_ALL_COLUMN << ") values "
            << "("
            << uid << ","
            << "'" << _db_Info->escapeString(wxrsp.unionid) << "'" << ","
            << "'" << _db_Info->escapeString(sST.sessionKey) << "'" << ","
            << "'" << _db_Info->escapeString(wxrsp.openid) << "'" << ","
            << "'" << _db_Info->escapeString(wxrsp.session_key) << "'" << ","
            << sST.timets
            << ")";
        DEBUGLOG("sql = " << sqlStr.str() << endl);
        // execute在mysql执行不成功的时候会抛出异常因此只要抛出了异常说明execute不成功
        _db_Info->execute(sqlStr.str());
        size_t affected_rows = _db_Info->getAffectedRows();

        if (affected_rows != 1)
        {
            ERRORLOG("insert error. affected_rows|" << affected_rows << endl);
            return -1;
        }
        ret = 0;
    }
    __CATCH_EXCEPTION_WITH__("MySQL_Exception");

    if (ret)
    {
        ERRORLOG("exec sql error|" << sqlStr.str() << endl);
    }
    return ret;
}



int AuthCommand::getNewCookie(const HardwareApplet::AppletCommHead &stHead, const HardwareApplet::GetNewCookieReq &stReq, HardwareApplet::GetNewCookieRsp &stRsp, tars::TarsCurrentPtr current)
{
    ostringstream ossStr;
    ossStr << stReq.userName << "|" << stReq.password << "";
    string funcName("getNewCookie");
    DEBUGLOG_WITHHEAD(stHead, ossStr.str() << endl);
    string passwordsha = TC_SHA::sha256str(stReq.password.data(), stReq.password.length());
    int ret = -1;
    // 用于回包
    HardwareApplet::GetNewCookieRsp response;
    try
    {
        ostringstream ossql;
        ossql << "select `uid` from t_seller_user_data where `user_name` = '" << _db_Info->escapeString(stReq.userName) << "'"
            << " and `password` = '" <<  _db_Info->escapeString(passwordsha) << "'";
        DEBUGLOG_WITHHEAD(stHead, "sql = " << ossql.str() << endl);
        _db_Info->execute(ossql.str());
        size_t affected_rows = _db_Info->getAffectedRows();
        if (affected_rows != 1)
        {
            ERRORLOG_WITHHEAD(stHead, "select error|" << ossStr.str() << "|" << ossql.str() << endl);
            response.ret = -1;
            sendReponse(response, stHead, funcName, ret, current);
            return -1;
        }

        HardwareApplet::SecurityTicket sST;
        ret = makeCookie(stReq.userName, passwordsha, response.st, sST);
        if (ret != 0)
        {
            ERRORLOG_WITHHEAD(stHead, "|makeCookie error|" << ossStr.str() << endl);
            response.ret = ret;
            sendReponse(response, stHead, funcName, ret, current);
            return ret;
        }
        
        ret = upSellerUserInfo(stReq.userName, passwordsha, sST);
        if (ret)
        {
            ERRORLOG("upSellerUserInfo error|" << endl);
        }
        response.ret = ret;
        sendReponse(response, stHead, funcName, ret, current);
        ret = 0;
    }
    __CATCH_EXCEPTION_WITH__(funcName);
    if (ret != 0)
    {
        //抛出异常也得回包
        sendReponse(response, stHead, funcName, ret, current);
    }

    return ret;
}

int AuthCommand::makeCookie(const string &userName, const string & passwordsha, string &st, HardwareApplet::SecurityTicket &sST)
{

    sST.timets = TNOW;
    string sessionKey = userName + passwordsha + L2S(sST.timets);
    sST.sessionKey = TC_SHA::sha256str(sessionKey.data(), sessionKey.length());
    sST.signature = TC_MD5::md5str(L2S(sST.timets) + sST.sessionKey + DEF_CFG_SINGLETON->_SVRMD5SIGKEY);

    string bsst;
    int ret = TarsEncode(sST, bsst);
    if (ret)
    {
        ERRORLOG("TarsEncode error|" << endl);
        return ret;
    }

    vector<char> vtST = TC_Tea::encrypt2(DEF_CFG_SINGLETON->_SVRKEY.c_str(), bsst.c_str(), bsst.length());
    string tempBase64In(vtST.begin(), vtST.end());
    st = TC_Base64::encode(tempBase64In);
    return 0;
}

int AuthCommand::upSellerUserInfo(const string &userName, const string & passwordsha, const HardwareApplet::SecurityTicket &sST)
{
    int ret = -1;
    try
    {
        map<string, pair<TC_Mysql::FT, string> > record;
        record["session_key"] = make_pair(TC_Mysql::DB_STR, sST.sessionKey);
        record["session_key_time"] = make_pair(TC_Mysql::DB_INT, sST.timets);
        ostringstream conStr;
        conStr << " where `user_name` = '" << _db_Info->escapeString(userName) << "' and `password` = '" <<  _db_Info->escapeString(passwordsha) << "'";

        size_t affected_rows = _db_Info->updateRecord("t_seller_user_data", record, conStr.str());
        if (affected_rows != 1)
        {
            ERRORLOG("upSellerUserInfo error. affected_rows|" << affected_rows << endl);
            return -1;
        }
        ret = 0;
    }
    __CATCH_EXCEPTION_WITH__("MySQL_Exception");

    if (ret != 0)
    {
        ERRORLOG("exec sql error|" << endl);
    }
    return ret;
}

