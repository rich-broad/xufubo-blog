/*********************************************************************************
 FileName: ProtocolHandler.cpp
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

int ProtocolHandler::CreateAppletCommHead(AppletCommHead& commHead)
{
    const HardwareApplet::ProxyReqHead &head = _ctx->_reqHead;
    commHead.deviceInfo = head.deviceInfo;
    commHead.romInfo = head.romInfo;
    commHead.netInfo = head.netInfo;
    commHead.st = _ctx->_st;
    commHead.clientTimestamp = head.clientTimestamp;
    return 0;
}

int ProtocolHandler::CreateRspHead(rapidjson::Document &document)
{
    rapidjson::Value head(kObjectType);

    head.AddMember("ret", _ctx->_rspHead.ret, document.GetAllocator());
    head.AddMember("svrTimestamp", _ctx->_rspHead.svrTimestamp, document.GetAllocator());
    head.AddMember("csTicketState", _ctx->_rspHead.csTicketState, document.GetAllocator());
    head.AddMember("st", rapidjson::Value(_ctx->_rspHead.st.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
    head.AddMember("requestId", _ctx->_rspHead.requestId, document.GetAllocator());

    document.AddMember("head", head, document.GetAllocator());
    return 0;
}


int ProtocolHandler::Json2Tars(vector<char>& reqData)
{
    int ret = 0;
    if (_ctx->_funcName == "getNewTicket")
    {
        return ParseGetNewTicketReq(reqData);
    }
    else if (_ctx->_funcName == "getNewTicket")
    {
        return ParseGetNewTicketReq(reqData);
    }
    else
    {
        return -1;
    }
    return ret;
}

int ProtocolHandler::Tars2Json(string &rspData)
{
    int ret = 0;
    if (_ctx->_funcName == "getNewTicket")
    {
        return PackGetNewTicketRsp(rspData);
    }
    else if (_ctx->_funcName == "getNewTicket")
    {
        return PackGetNewTicketRsp(rspData);
    }
    else
    {
        return -1;
    }
    return ret;
}

int ProtocolHandler::ParseGetNewTicketReq(vector<char>& reqData)
{
    int ret = 0;
    const Value& jsonValue = _ctx->_reqJsonBody;
    DEBUGLOG("ParseGetNewTicketReq enter: _reqJsonBody = " << AppletCommUtils::Value2Str(_ctx->_reqJsonBody) << endl);
    GetNewTicketReq request;
    request.wx_code = jsonValue["wx_code"].GetString();
    ret = TarsEncode<GetNewTicketReq>(request, reqData);
    if (ret)
    {
        ERRORLOG("ParseGetNewTicketReq TarsEncode err|"<< endl) ;
    }
    DEBUGLOG("ParseGetNewTicketReq enter: reqData.size = " << reqData.size() << endl);
    return ret;
}

int ProtocolHandler::PackGetNewTicketRsp(string &rspData)
{
    int ret = 0;
    GetNewTicketRsp response;
    ret = TarsDecode<GetNewTicketRsp>(_ctx->_vtRsp, response);
    if (ret)
    {
        ERRORLOG("PackGetNewTicketReq TarsDecode err|"<< endl);
        return ret;
    }
     rapidjson::Document document;
     document.SetObject();
     rapidjson::Value body(rapidjson::kObjectType);
     body.AddMember("st", rapidjson::Value(response.st.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
     document.AddMember("body", body, document.GetAllocator());
     CreateRspHead(document);
     rspData = AppletCommUtils::Document2Str(document);
     return ret;

}