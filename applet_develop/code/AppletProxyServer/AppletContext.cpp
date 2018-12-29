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


using namespace std;
using namespace rapidjson;
using namespace tars;
using namespace HardwareApplet;

int32_t AppletContext::parseRequestData(const vector<char>& httpReqData)
{
	int32_t ret = parseHttpReq(httpReqData);
	if (ret != 0)
	{
		return -1;
	}
    if (_reqHead.st.empty())
    {
        if ( _funcName != "getNewTicket")
        {
            return -4;
        }
        return 0;
    }
    
	ret = parseST();
	_rspHead.csTicketState = ret;
	if (ret == -104)
	{
		_rspHead.ret = E_TICKET_EXPIRED;		//票据过期
		return -2;
	}
	else if (ret < 0)
	{
		_rspHead.ret = E_TICKET_INVALID;		//票据非法
		return -3;
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
    int ret = 0;
    Document document;
    document.Parse(content.c_str());
    const Value& head = document["head"];
    _reqJsonBody.CopyFrom(document["body"], document.GetAllocator());
    _reqHead.requestId = head["requestId"].GetInt();
    _reqHead.cmd = (HardwareApplet::CMD)head["cmd"].GetInt();
    _funcName = DEF_CFG_SINGLETON->cmdToFuncName(_reqHead.cmd);
    if (_funcName.empty())
    {
        ERRORLOG("cmdToFuncName err" << endl);
        return -1;
    }
    _reqHead.st = head["st"].GetString();
    _reqHead.clientTimestamp = head["clientTimestamp"].GetInt64();
    _reqHead.svrTimestamp = head["svrTimestamp"].GetInt64();

    const Value& deviceInfo = head["deviceInfo"];
    const Value& romInfo = head["romInfo"];
    const Value& netInfo = head["netInfo"];

    _reqHead.deviceInfo.imei1 = deviceInfo["imei1"].GetString();
    _reqHead.deviceInfo.imei2 = deviceInfo["imei2"].GetString();
    _reqHead.deviceInfo.macAddr = deviceInfo["macAddr"].GetString();
    _reqHead.deviceInfo.brand = deviceInfo["brand"].GetString();
    _reqHead.deviceInfo.mode = deviceInfo["mode"].GetString();

    _reqHead.romInfo.sysId = romInfo["sysId"].GetString();
    _reqHead.romInfo.sysVersionName = romInfo["sysVersionName"].GetString();
    _reqHead.romInfo.sysVersionCode = romInfo["sysVersionCode"].GetString();
    _reqHead.romInfo.rootFlag = romInfo["rootFlag"].GetInt();

    _reqHead.netInfo.netType = netInfo["netType"].GetInt();
    _reqHead.netInfo.wifiSsid = netInfo["wifiSsid"].GetString();
    _reqHead.netInfo.wifiBssid = netInfo["wifiBssid"].GetString();
    return ret;
}

int32_t AppletContext::parseST()
{
	int ret = -100;
    if (_reqHead.st.empty())
    {
        ERRORLOG("parseST error st empty" << endl);
        return E_ST_EMPTY_ERROR;
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
		ret = -101;
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
		ret = -102;
		return ret;
	}

    string reqSig = TC_MD5::md5str(L2S(_st.timets) + _st.sessionKey + DEF_CFG_SINGLETON->_SVRMD5SIGKEY);
    if (reqSig != _st.signature)
    {
        ERRORLOG("forgery attack err|" << _clienIp << "|" << reqSig << "|" << _st.signature << endl);
        ret = -105;
        return ret;
    }
    

	int64_t timeInterval = TNOW - _st.timets;
	if (timeInterval < -60)
	{
		ERRORLOG("ticket time err|" << _clienIp << "|" << timeInterval << "|" << _st.timets << endl);
		ret = -103;
	}
	else if (timeInterval > DEF_CFG_SINGLETON->_outdateTime)
	{
		ERRORLOG("ticket outdate|" << _clienIp << "|" << timeInterval << "|" << _st.timets << endl);
		ret = -104;
	}
	else if (timeInterval > DEF_CFG_SINGLETON->_preOutedateTime)	//快要过期
	{
		ret = 1;
		DEBUGLOG("ticket need update|" << _clienIp << "|" << timeInterval << "|" << _st.timets << endl);
	}
	else
	{
		ret = 0;
	}

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
        << AppletCommUtils::Value2Str(_reqJsonBody);
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
        << AppletCommUtils::Value2Str(_reqJsonBody) << "|"
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

