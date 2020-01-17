/*********************************************************************************
 FileName: AppletContext.h
 Author: xufubo
 Date:  2018-12-26
 Description:
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/
#include "Global.h"
#include "Configuration.h"


using namespace std;
using namespace tars;
using namespace HardwareApplet;
using namespace base_utils;
using namespace rapidjson;

class AppletContext : public TC_HandleBase
{
#define SELLER 2
#define BUYER 1
public:
    friend class ProtocolHandler;
public:
    AppletContext(TC_Mysql * dbInfo,tars::TarsCurrentPtr current)
    {
        _dbInfo = dbInfo;
        _current = current;
        _beginTime = TNOW;
		_clienIp = current->getIp();
    }
	virtual ~AppletContext()
    {
        
    };
    
public:
    // 这个Current类是个核心，里边很多信息。
    tars::TarsCurrentPtr _current;
	ProxyReqHead _reqHead;
    SecurityTicket _st;
    SessionInfo _sessionInfo;
    string _reqBodyStr;
	ProxyRspHead _rspHead;
    vector<char> _vtRsp;
    time_t _beginTime;
    string _clienIp;
	string _funcName;
    // 后端服务调用异常时exception回调里的错误码，框架层的返回值，RPC调用出现了异常
    int32_t _tarsRet;
    // 后端服务调用成功，响应包体的tag 0 值，约定为ret错误码
    int32_t _busiRet;
    // 后端服务调用成功，rpc方法的返回值
	int32_t _rpcRet;

public:
    int32_t getTimeInterval()
    {
        return (TNOW - _beginTime);
    }

    //解析HTTP请求包体中的数据
	int32_t parseRequestData(const vector<char> & httpReqData);
    //回包
	int32_t respond();
//以下三个为日志即监控
	void logRequset();
	void logResponse();
	void reportMonitorMsg(const string & funcName, tars::StatReport::StatResult statResult, int32_t iRet);

private:
	int32_t parseHttpReq(const vector<char>& httpReqData);
	int32_t parseST();
	int32_t parseRequest();
    int32_t parseHttpBody(const string & content);
    int32_t getSessionInfo();
    int32_t checkLoginInfo();

private:
    TC_Mysql * _dbInfo;
};
typedef TC_AutoPtr<AppletContext> AppletContextPtr;

class ProtocolHandler : public TC_HandleBase
{
public:
    ProtocolHandler(AppletContextPtr ctx):_ctx(ctx){}
    virtual ~ProtocolHandler(){};

    int CreateAppletCommHead(HardwareApplet::AppletCommHead& commHead);
    int Json2Tars(vector<char>& reqData);
    int Tars2Json(string &json_body);

private:
    template<typename ReqType>
    int ParseJson2Request(vector<char> &reqData)
    {
        int ret = 0;
        ReqType request;
        try
        {
            request.readFromJsonString(_ctx->_reqBodyStr);
            ret = TarsEncode<ReqType>(request, reqData);
            if (ret)
            {
                ERRORLOG("TarsEncode err|"<< endl);
            }
        }
        catch(const std::exception& e)
        {
            ERRORLOG("exception err|" << e.what() << endl);
            ret = -1;
        }
        return ret;
    }

    template<typename ProxyRspType, typename RspType>
    int PackJsonFromResponse(string &rspData)
    {
        int ret = 0;
        ProxyRspType response;

        try
        {
            ret = TarsDecode<RspType>(_ctx->_vtRsp, response.body);
            if (ret)
            {
                ERRORLOG("TarsDecode err|"<< endl);
            }
            response.head = _ctx->_rspHead;

            rspData = response.writeToJsonString();
        }
        catch(const std::exception& e)
        {
            ERRORLOG("exception err|" << e.what() << endl);
            ret = -1;
        }
        return ret;
    }

private:
    AppletContextPtr _ctx;
};

typedef TC_AutoPtr<ProtocolHandler> ProtocolHandlerPtr;
