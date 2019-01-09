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
#include "AppletCommUtils.h"


using namespace std;
using namespace rapidjson;
using namespace tars;
using namespace HardwareApplet;
using namespace base_utils;

class AppletContext : public TC_HandleBase
{
public:
    friend class ProtocolHandler;
public:
    AppletContext(tars::TarsCurrentPtr current)
    {
        _current = current;
        _beginTime = TNOW;
		_clienIp = current->getIp();
        _document.SetObject();
    }
	virtual ~AppletContext()
    {

    };
    
public:
    // 这个Current类是个核心，里边很多信息。
    tars::TarsCurrentPtr _current;
	ProxyReqHead _reqHead;
    SecurityTicket _st;
    Document _document;
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
    inline int CreateRspHead(rapidjson::Document &document);
    int ParseGetNewTicketReq(vector<char>& reqData);
    int PackGetNewTicketRsp(string& rspData);


    int ParseGetCategoryListReq(vector<char>& reqData);
    int PackGetCategoryListRsp(string &rspData);

    int ParseAddCategoryInfoReq(vector<char>& reqData);
    int PackAddCategoryInfoRsp(string &rspData);

    int ParseGetAttributeListReq(vector<char>& reqData);
    int PackGetAttributeListRsp(string &rspData);

    int ParseAddAttributeInfoReq(vector<char>& reqData);
    int PackAddAttributeInfoRsp(string &rspData);

    int ParseGetAttributeValueListReq(vector<char>& reqData);
    int PackGetAttributeValueListRsp(string &rspData);

    int ParseAddAttributeValueInfoReq(vector<char>& reqData);
    int PackAddAttributeValueInfoRsp(string &rspData);

    int ParseGetBrandListReq(vector<char>& reqData);
    int PackGetBrandListRsp(string &rspData);

    int ParseAddBrandInfoReq(vector<char>& reqData);
    int PackAddBrandInfoRsp(string &rspData);

    int ParseGetMakerListReq(vector<char>& reqData);
    int PackGetMakerListRsp(string &rspData);

    int ParseAddMakerInfoReq(vector<char>& reqData);
    int PackAddMakerInfoRsp(string &rspData);

    int ParseGetGoodsSPUListReq(vector<char>& reqData);
    int PackGetGoodsSPUListRsp(string &rspData);

    int ParseAddGoodsSPUInfoReq(vector<char>& reqData);
    int PackAddGoodsSPUInfoRsp(string &rspData);

    int ParseAddGoodsSKUInfoReq(vector<char>& reqData);
    int PackAddGoodsSKUInfoRsp(string &rspData);

    int ParseGetGoodsSKUListReq(vector<char>& reqData);
    int PackGetGoodsSKUListRsp(string &rspData);

    int ParseAddMyAddressInfoReq(vector<char>& reqData);
    int PackAddMyAddressInfoRsp(string &rspData);

    int ParseGetMyAddressListReq(vector<char>& reqData);
    int PackGetMyAddressListRsp(string &rspData);

    int ParseGetProvinceListReq(vector<char>& reqData);
    int PackGetProvinceListRsp(string &rspData);

    int ParseGetCityListByProvinceReq(vector<char>& reqData);
    int PackGetCityListByProvinceRsp(string &rspData);

    int ParseGetCountyListByCityReq(vector<char>& reqData);
    int PackGetCountyListByCityRsp(string &rspData);

    int ParseAddGoodsToShopCartReq(vector<char>& reqData);
    int PackAddGoodsToShopCartRsp(string &rspData);

    int ParseGetMyShopCartInfoReq(vector<char>& reqData);
    int PackGetMyShopCartInfoRsp(string &rspData);

    int ParseSubmitOrderReq(vector<char>& reqData);
    int PackSubmitOrderRsp(string &rspData);

    int ParseConfirmOrderReq(vector<char>& reqData);
    int PackConfirmOrderRsp(string &rspData);

private:
    AppletContextPtr _ctx;
};

typedef TC_AutoPtr<ProtocolHandler> ProtocolHandlerPtr;
