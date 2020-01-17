/*********************************************************************************
 FileName: AppletContext.cpp
 Author: xufubo
 Date:  2018-12-26
 Description:
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/
#include "AppletContext.h"
#include "AppletCommUtils.h"

using namespace std;
using namespace rapidjson;
using namespace tars;
using namespace HardwareApplet;

int32_t AppletContext::parseRequestData(const vector<char>& httpReqData)
{
	int32_t ret = parseHttpReq(httpReqData);
	if (ret != 0)
	{
        ERRORLOG("parseHttpReq error");
		return ret;
	}
    
	ret = parseST();
	_rspHead.csTicketState = ret;
    _rspHead.ret = (HardwareApplet::COMMRETCODE)ret;
    if (ret == E_TICKET_WILL_EXPIRED )
    {
        DEBUGLOG("E_TICKET_WILL_EXPIRED");
        ret = 0;
    }
    if (ret != 0)
    {
        ERRORLOG("parseST error. ret = " << ret << endl);
        return ret;
    }
    
    // 能到这一步，说明调的这个函数一定存在, 1：买家函数，2：卖家函数
    if (DEF_CFG_SINGLETON->_mpFunc[_funcName] == BUYER)
    {
        ret = getSessionInfo();
        if (ret != 0)
        {
            _rspHead.ret = E_GET_BUYER_SESSION_ERROR;
            ERRORLOG("getSessionInfo error. ret = " << ret << endl);
        }
        return ret;
    }
    if (DEF_CFG_SINGLETON->_mpFunc[_funcName] == SELLER)
    {
        ret = checkLoginInfo();
        if (ret != 0)
        {
            _rspHead.ret = E_CHECK_SELLER_LOGIN_ERROR;
            ERRORLOG("checkLoginInfo error. ret = " << ret << endl);
        }
        return ret;
    }
	return 0;
}

int32_t AppletContext::parseHttpReq(const vector<char>& httpReqData)
{
	int32_t ret = -100;
	TC_HttpRequest tcHttpRequest;
	try
	{
		if(tcHttpRequest.decode(httpReqData.data(), httpReqData.size()))
		{
			TC_Http::http_header_type headers = tcHttpRequest.getHeaders();
			const string & content = tcHttpRequest.getContent();
            //将HTTP请求包体进行反序列化，得到
			if(parseHttpBody(content) != 0)
			{
				ERRORLOG("request parseHttpBody err|" << _clienIp.c_str() << "|" << content.size() 
                    << "|" << httpReqData.size() << "|" << tcHttpRequest.getHeadLength() 
                    << "|" << tcHttpRequest.getContentLength() << endl);
				ret = -1;
			}
			else
			{
				ret = 0;
			}	
		}
		else
		{
			ERRORLOG("httpDecode err|" << _clienIp << "|" << httpReqData.size() << endl);
			ret = -2;
		}
	}
	__COMMON_EXCEPTION_CATCH_EXT__("httpDecdoe exception|" << httpReqData.size())

	return ret;
}

int32_t AppletContext::parseHttpBody(const string & content)
{
    int ret = -1;
    try
	{
		if(content.empty())
		{
            return -1;
		}
        Document document;
        document.Parse(content.c_str());
        if(document.FindMember("head") == document.MemberEnd() || document.FindMember("body") == document.MemberEnd())
        {
            ERRORLOG("find member head or body err" << endl);
            return -1;
        }
        if (!document["head"].IsObject() || document["body"].IsObject())
        {
            ERRORLOG("head or body is not object err" << endl);
            return -1;
        }
    
        _reqBodyStr = AppletCommUtils::Value2Str(document["body"]);
        _reqHead.readFromJsonString(AppletCommUtils::Value2Str(document["head"]));
        _funcName = _reqHead.cmd;
        if (!_funcName.empty())
        {
            ret = 0;
        }
        
	}
	__COMMON_EXCEPTION_CATCH_EXT__("json exception|")
    return ret;
}

// 这里管理登录态(卖家和买家是不一样的，这里只写了买家的登录态逻辑，卖家的怎么搞呢？？？？)
int32_t AppletContext::parseST()
{
	int ret = -100;
    if (_reqHead.st.empty())
    {
        if (_funcName != "getNewTicket" && _funcName != "getNewCookie")
        {
            ERRORLOG("parseST error st empty" << endl);
            return E_TICKET_INVALID;
        }
        return 0;
    }
    
	vector<char> vtST;
	try
	{
		string sST = TC_Base64::decode(_reqHead.st);
		vtST = TC_Tea::decrypt2(DEF_CFG_SINGLETON->_SVRKEY.c_str(), sST.c_str(), sST.size());
		ret = 0;
	}
	catch(TC_Tea_Exception& e)
	{
		ERRORLOG("parseST exception|tea|" << _clienIp << "|" << _reqHead.st.size() 
            << "|" << _reqHead.st << "|" << e.what() << endl);
		ret = E_TICKET_INVALID;
	}
	__COMMON_EXCEPTION_CATCH_EXT__("parseST exception|" << _clienIp << "|" << _reqHead.st.size() << "|" 
        << _reqHead.st)
	if (ret != 0)
	{
		return ret;
	}

	if (TarsDecode<HardwareApplet::SecurityTicket>(vtST, _st) != 0)
	{
		ERRORLOG("TarsDecode ST err|" << _clienIp << "|" << vtST.size() << endl);
		return E_TICKET_INVALID;
	}

    string reqSig = TC_MD5::md5str(L2S(_st.timets) + _st.sessionKey + DEF_CFG_SINGLETON->_SVRMD5SIGKEY);
    if (reqSig != _st.signature)
    {
        ERRORLOG("forgery attack err|" << _clienIp << "|" << reqSig << "|" << _st.signature << endl);
        return E_TICKET_INVALID;
    }
    

	int64_t timeInterval = TNOW - _st.timets;
	if (timeInterval < -60)
	{
		ERRORLOG("ticket time err|" << _clienIp << "|" << timeInterval << "|" << _st.timets << endl);
		ret = E_TICKET_INVALID;
	}
	else if (timeInterval > DEF_CFG_SINGLETON->_outdateTime)  // 是否过期，这个时间可以很长
	{
		ERRORLOG("ticket outdate|" << _clienIp << "|" << timeInterval << "|" << _st.timets << endl);
		ret = E_TICKET_EXPIRED;             // 票据过期
	}
	else if (timeInterval > DEF_CFG_SINGLETON->_preOutedateTime)
	{
		ret = E_TICKET_WILL_EXPIRED;  // 票据将要过期
		DEBUGLOG("ticket need update|" << _clienIp << "|" << timeInterval << "|" << _st.timets << endl);
	}
	else
	{
		ret = 0;
	}

	return ret;
}

int32_t AppletContext::getSessionInfo()
{
    // 这个函数不需要去换取登录态信息
    if (_funcName == "getNewTicket")
    {
        ERRORLOG("funcName == getNewTicket" << endl);
        return 0;
    }
    int ret = -1;
    try
    {
        ostringstream sqlStr;
        sqlStr << "select `uid`, `open_id`, `union_id` from t_user_login_data where `custom_session_key` = " << _dbInfo->escapeString(_st.sessionKey);
        DEBUGLOG("sql = " << sqlStr.str() << endl);
        TC_Mysql::MysqlData data = _dbInfo->queryRecord(sqlStr.str());
        if (data.size() != 1)
        {
            ERRORLOG("SELECT error|sql=" << sqlStr.str() << endl);
            return -1;
        }

        TC_Mysql::MysqlRecord record = data[0];
        _sessionInfo.uid = TC_Common::strto<int32_t>(record["uid"]);
        _sessionInfo.openid = record["open_id"];
        _sessionInfo.unionid = record["union_id"];
        if (_sessionInfo.uid <= 0 || _sessionInfo.openid.empty() || _sessionInfo.unionid.empty())
        {
            ERRORLOG("sessionInfo error|" << _sessionInfo.uid << "|" << _sessionInfo.openid << "|" << _sessionInfo.unionid << endl);
            return -1;
        }
        ret = 0;
    }
    __CATCH_EXCEPTION_WITH__("MySQL_Exception");
    return ret;
    
}

int32_t AppletContext::checkLoginInfo()
{
    // 这个函数不需要去换取登录态信息
    if (_funcName == "getNewCookie")
    {
        ERRORLOG("funcName == getNewCookie" << endl);
        return 0;
    }
    int ret = -1;
    try
    {
        ostringstream sqlStr;
        sqlStr << "select `uid` from t_seller_user_data where `session_key` = " << _dbInfo->escapeString(_st.sessionKey);
        DEBUGLOG("sql = " << sqlStr.str() << endl);
        TC_Mysql::MysqlData data = _dbInfo->queryRecord(sqlStr.str());
        if (data.size() != 1)
        {
            ERRORLOG("checkLoginInfo error|sql=" << sqlStr.str() << endl);
            return -1;
        }
        ret = 0;
    }
    __CATCH_EXCEPTION_WITH__("MySQL_Exception");

    return ret;
}

void AppletContext::logRequset()
{
    ostringstream buf;
    buf << "HttpRequest|"
        << REQ_HEAD_ALL_INFO(_reqHead) << "|"
        << _beginTime << "|"
        << _clienIp << "|"
        << _funcName << "|"
        << _reqBodyStr << "|";
    DEBUGLOG(buf.str() << endl);
}

int32_t AppletContext::respond()
{
    int ret = -1000;
    
    TC_HttpResponse httpRsp;
    httpRsp.setHeader("Content-Type", "text/javascript");	//防止XSS
    string jsonBody;
    _rspHead.svrTimestamp = TNOW;
    _rspHead.st = _reqHead.st;
    _rspHead.requestId = _reqHead.requestId;
    try
    {
        ProtocolHandlerPtr protocol_handler = new ProtocolHandler(this);
        ret = protocol_handler->Tars2Json(jsonBody);
        ret = 0;
    }
    __COMMON_EXCEPTION_CATCH_EXT__("response exception|" << DEVICE_INFO(_reqHead.deviceInfo) << "|" << _clienIp << "|" << jsonBody.size());

    if (ret)
    {
        ERRORLOG("respond Tars2Json err|" << DEVICE_INFO(_reqHead.deviceInfo) << "|" << _clienIp << jsonBody.size() << endl);
        httpRsp.setResponse(404, "server err");
    }
    else
    {
        httpRsp.setResponse(200, "OK", jsonBody);
    }

	
	string httpRspBuf = httpRsp.encode();
    ret = -100;
	try
	{
		_current->sendResponse(httpRspBuf.c_str(), httpRspBuf.size());
		ret = 0;
	}
	catch(exception& e)
	{
		ERRORLOG("sendRes exception|" 
			<< REQ_HEAD_ALL_INFO(_reqHead) << "|" << _clienIp << "|" << e.what() << endl);
		ret = -101;
	}
	catch(...)
	{
		ERRORLOG("sendRes unknown exception|" << REQ_HEAD_ALL_INFO(_reqHead) << endl);
		ret = -102;
	}

	return ret;
}

void AppletContext::logResponse()
{
    ostringstream buf;
    buf << "HttpRespond|"
        << REQ_HEAD_ALL_INFO(_reqHead) << "|"
        << _beginTime << "|"
        << _clienIp << "|"
        << _funcName << "|"
        << _reqBodyStr << "|"
        << RSP_HEAD_ALL_INFO(_rspHead) << "|"
        << _tarsRet << "|"
        << _busiRet << "|"
        << _rpcRet << "|"
        << _vtRsp.size() << "|";
    DEBUGLOG(buf.str() << endl);
}

void AppletContext::reportMonitorMsg(const string & funcName, tars::StatReport::StatResult statResult, int32_t iRet)
{
    Application::getCommunicator()->getStatReport()->report("",
        "",
        funcName,
        _current->getIp(),
        10001,
        statResult,
        getTimeInterval(),
        0,
        false);
}

