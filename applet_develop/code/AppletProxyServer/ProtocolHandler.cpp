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
    commHead.sessionInfo = _ctx->_sessionInfo;
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
    else if (_ctx->_funcName == "getCategoryList")
    {
        return ParseGetCategoryListReq(reqData);
    }
    else if (_ctx->_funcName == "addCategoryInfo")
    {
        return ParseAddCategoryInfoReq(reqData);
    }
    else if (_ctx->_funcName == "getAttributeList")
    {
        return ParseGetAttributeListReq(reqData);
    }
    else if (_ctx->_funcName == "addAttributeInfo")
    {
        return ParseAddAttributeInfoReq(reqData);
    }
    else if (_ctx->_funcName == "getAttributeValueList")
    {
        return ParseGetAttributeValueListReq(reqData);
    }
    else if (_ctx->_funcName == "addAttributeValueInfo")
    {
        return ParseAddAttributeValueInfoReq(reqData);
    }
    else if (_ctx->_funcName == "getBrandList")
    {
        return ParseGetBrandListReq(reqData);
    }
    else if (_ctx->_funcName == "addBrandInfo")
    {
        return ParseAddBrandInfoReq(reqData);
    }
    else if (_ctx->_funcName == "getMakerList")
    {
        return ParseGetMakerListReq(reqData);
    }
    else if (_ctx->_funcName == "addMakerInfo")
    {
        return ParseAddMakerInfoReq(reqData);
    }
    else if (_ctx->_funcName == "getGoodsSPUList")
    {
        return ParseGetGoodsSPUListReq(reqData);
    }
    else if (_ctx->_funcName == "addGoodsSPUInfo")
    {
        return ParseAddGoodsSPUInfoReq(reqData);
    }
    else if (_ctx->_funcName == "addGoodsSKUInfo")
    {
        return ParseAddGoodsSKUInfoReq(reqData);
    }
    else if (_ctx->_funcName == "getGoodsSKUList")
    {
        return ParseGetGoodsSKUListReq(reqData);
    }
    else if (_ctx->_funcName == "addMyAddressInfo")
    {
        return ParseAddMyAddressInfoReq(reqData);
    }
    else if (_ctx->_funcName == "getMyAddressList")
    {
        return ParseGetMyAddressListReq(reqData);
    }
    else if (_ctx->_funcName == "addGoodsToShopCart")
    {
        return ParseAddGoodsToShopCartReq(reqData);
    }
    else if (_ctx->_funcName == "getMyShopCartInfo")
    {
        return ParseGetMyShopCartInfoReq(reqData);
    }
    else if (_ctx->_funcName == "submitOrder")
    {
        return ParseSubmitOrderReq(reqData);
    }
    else if (_ctx->_funcName == "confirmOrder")
    {
        return ParseConfirmOrderReq(reqData);
    }
    else if (_ctx->_funcName == "getProvinceList")
    {
        return ParseGetProvinceListReq(reqData);
    }
    else if (_ctx->_funcName == "getCityListByProvince")
    {
        return ParseGetCityListByProvinceReq(reqData);
    }
    else if (_ctx->_funcName == "getCountyListByCity")
    {
        return ParseGetCountyListByCityReq(reqData);
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
    else if (_ctx->_funcName == "getCategoryList")
    {
        return PackGetCategoryListRsp(rspData);
    }
    else if (_ctx->_funcName == "addCategoryInfo")
    {
        return PackAddCategoryInfoRsp(rspData);
    }
    else if (_ctx->_funcName == "getAttributeList")
    {
        return PackGetAttributeListRsp(rspData);
    }
    else if (_ctx->_funcName == "addAttributeInfo")
    {
        return PackAddAttributeInfoRsp(rspData);
    }
    else if (_ctx->_funcName == "getAttributeValueList")
    {
        return PackGetAttributeValueListRsp(rspData);
    }
    else if (_ctx->_funcName == "addAttributeValueInfo")
    {
        return PackAddAttributeValueInfoRsp(rspData);
    }
    else if (_ctx->_funcName == "getBrandList")
    {
        return PackGetBrandListRsp(rspData);
    }
    else if (_ctx->_funcName == "addBrandInfo")
    {
        return PackAddBrandInfoRsp(rspData);
    }
    else if (_ctx->_funcName == "getMakerList")
    {
        return PackGetMakerListRsp(rspData);
    }
    else if (_ctx->_funcName == "addMakerInfo")
    {
        return PackAddMakerInfoRsp(rspData);
    }
    else if (_ctx->_funcName == "getGoodsSPUList")
    {
        return PackGetGoodsSPUListRsp(rspData);
    }
    else if (_ctx->_funcName == "addGoodsSPUInfo")
    {
        return PackAddGoodsSPUInfoRsp(rspData);
    }
    else if (_ctx->_funcName == "addGoodsSKUInfo")
    {
        return PackAddGoodsSKUInfoRsp(rspData);
    }
    else if (_ctx->_funcName == "getGoodsSKUList")
    {
        return PackGetGoodsSKUListRsp(rspData);
    }
    else if (_ctx->_funcName == "addMyAddressInfo")
    {
        return PackAddMyAddressInfoRsp(rspData);
    }
    else if (_ctx->_funcName == "getMyAddressList")
    {
        return PackGetMyAddressListRsp(rspData);
    }
    else if (_ctx->_funcName == "addGoodsToShopCart")
    {
        return PackAddGoodsToShopCartRsp(rspData);
    }
    else if (_ctx->_funcName == "getMyShopCartInfo")
    {
        return PackGetMyShopCartInfoRsp(rspData);
    }
    else if (_ctx->_funcName == "submitOrder")
    {
        return PackSubmitOrderRsp(rspData);
    }
    else if (_ctx->_funcName == "confirmOrder")
    {
        return PackConfirmOrderRsp(rspData);
    }
    else if (_ctx->_funcName == "getProvinceList")
    {
        return PackGetProvinceListRsp(rspData);
    }
    else if (_ctx->_funcName == "getCityListByProvince")
    {
        return PackGetCityListByProvinceRsp(rspData);
    }
    else if (_ctx->_funcName == "getCountyListByCity")
    {
        return PackGetCountyListByCityRsp(rspData);
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
    const Value& jsonValue = _ctx->_document["body"];
    //DEBUGLOG("ParseGetNewTicketReq enter: _reqJsonBody = " << AppletCommUtils::Value2Str(jsonValue) << endl);
    GetNewTicketReq request;
    request.wx_code = RapidJsonUtil::GetJsonString(jsonValue, "wx_code");
    DEBUGLOG("ParseGetNewTicketReq enter: request.wx_code = " << request.wx_code << endl);
    ret = TarsEncode<GetNewTicketReq>(request, reqData);
    if (ret)
    {
        ERRORLOG("TarsEncode err|"<< endl) ;
    }
    //DEBUGLOG("ParseGetNewTicketReq enter: reqData.size = " << reqData.size() << endl);
    return ret;
}

int ProtocolHandler::PackGetNewTicketRsp(string &rspData)
{
    int ret = 0;
    GetNewTicketRsp response;
    ret = TarsDecode<GetNewTicketRsp>(_ctx->_vtRsp, response);
    if (ret)
    {
        ERRORLOG("TarsDecode err|"<< endl);
        return ret;
    }
     rapidjson::Document document;
     document.SetObject();
     rapidjson::Value body(rapidjson::kObjectType);
     body.AddMember("st", rapidjson::Value(response.st.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
     body.AddMember("errmsg", rapidjson::Value(response.errmsg.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
     body.AddMember("ret", response.ret, document.GetAllocator());
     document.AddMember("body", body, document.GetAllocator());
     CreateRspHead(document);
     rspData = AppletCommUtils::Document2Str(document);
     return ret;

}


int ProtocolHandler::ParseGetCategoryListReq(vector<char>& reqData)
{
    int ret = 0;
    const Value& jsonValue = _ctx->_document["body"];
    GetCategoryListReq request;
    request.level = RapidJsonUtil::GetJsonInt(jsonValue, "level", 0);
    ret = TarsEncode<GetCategoryListReq>(request, reqData);
    if (ret)
    {
        ERRORLOG("TarsEncode err|"<< endl) ;
    }
    return ret;
}

int ProtocolHandler::PackGetCategoryListRsp(string &rspData)
{
    int ret = 0;
    GetCategoryListRsp response;
    ret = TarsDecode<GetCategoryListRsp>(_ctx->_vtRsp, response);
    if (ret)
    {
        ERRORLOG("TarsDecode err|"<< endl);
        return ret;
    }
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Value body(rapidjson::kObjectType);
    body.AddMember("errmsg", rapidjson::Value(response.errmsg.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
    body.AddMember("ret", response.ret, document.GetAllocator());

    rapidjson::Value mpCategory(rapidjson::kObjectType);
    for (map<int, vector<CategoryItem> >::const_iterator iter = response.mpCategory.begin(); iter != response.mpCategory.end(); ++iter)
    {

        rapidjson::Value categoryList(rapidjson::kArrayType);
        for (size_t i = 0; i < iter->second.size(); ++i)
        {
            rapidjson::Value item(rapidjson::kObjectType);
            AppletCommUtils::CategoryItem2Json(document, iter->second[i], item);
            categoryList.PushBack(item, document.GetAllocator());
        }
        // mpCategory.AddMember(I2S(iter->first).c_str(), categoryList, document.GetAllocator());
        mpCategory.AddMember(rapidjson::Value(I2S(iter->first).c_str(), document.GetAllocator()).Move(), categoryList, document.GetAllocator());
    }
    body.AddMember("mpCategory", mpCategory, document.GetAllocator());

    document.AddMember("body", body, document.GetAllocator());
    CreateRspHead(document);
    rspData = AppletCommUtils::Document2Str(document);
    return ret;
}

int ProtocolHandler::ParseAddCategoryInfoReq(vector<char>& reqData)
{
    int ret = 0;
    const Value& jsonValue = _ctx->_document["body"];
    if (jsonValue.FindMember("item") == jsonValue.MemberEnd())
    {
        return -1;
    }
    AddCategoryInfoReq request;
    AppletCommUtils::Json2CategoryItem(jsonValue["item"], request.item);
    
    ret = TarsEncode<AddCategoryInfoReq>(request, reqData);
    if (ret)
    {
        ERRORLOG("TarsEncode err|"<< endl) ;
    }
    return ret;
}

int ProtocolHandler::PackAddCategoryInfoRsp(string &rspData)
{
    int ret = 0;
    AddCategoryInfoRsp response;
    ret = TarsDecode<AddCategoryInfoRsp>(_ctx->_vtRsp, response);
    if (ret)
    {
        ERRORLOG("TarsDecode err|"<< endl);
        return ret;
    }
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Value body(rapidjson::kObjectType);
    body.AddMember("errmsg", rapidjson::Value(response.errmsg.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
    body.AddMember("ret", response.ret, document.GetAllocator());

    document.AddMember("body", body, document.GetAllocator());
    CreateRspHead(document);
    rspData = AppletCommUtils::Document2Str(document);
    return ret;
}

int ProtocolHandler::ParseGetAttributeListReq(vector<char>& reqData)
{
    int ret = 0;
    const Value& jsonValue = _ctx->_document["body"];
    GetAttributeListReq request;
    request.reserved_field = RapidJsonUtil::GetJsonString(jsonValue, "reserved_field");
    ret = TarsEncode<GetAttributeListReq>(request, reqData);
    if (ret)
    {
        ERRORLOG("TarsEncode err|"<< endl) ;
    }
    return ret;
}

int ProtocolHandler::PackGetAttributeListRsp(string &rspData)
{
    int ret = 0;
    GetAttributeListRsp response;
    ret = TarsDecode<GetAttributeListRsp>(_ctx->_vtRsp, response);
    if (ret)
    {
        ERRORLOG("TarsDecode err|"<< endl);
        return ret;
    }
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Value body(rapidjson::kObjectType);
    body.AddMember("errmsg", rapidjson::Value(response.errmsg.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
    body.AddMember("ret", response.ret, document.GetAllocator());

    rapidjson::Value itemList(rapidjson::kArrayType);
    for (size_t i = 0; i < response.itemList.size(); ++i)
    {
        rapidjson::Value item(rapidjson::kObjectType);
        AppletCommUtils::AttributeItem2Json(document, response.itemList[i], item);
        itemList.PushBack(item, document.GetAllocator());
    }
    body.AddMember("itemList", itemList, document.GetAllocator());

    document.AddMember("body", body, document.GetAllocator());
    CreateRspHead(document);
    rspData = AppletCommUtils::Document2Str(document);
    return ret;
}
int ProtocolHandler::ParseAddAttributeInfoReq(vector<char>& reqData)
{
    int ret = 0;
    const Value& jsonValue = _ctx->_document["body"];
    if (jsonValue.FindMember("item") == jsonValue.MemberEnd())
    {
        return -1;
    }
    AddAttributeInfoReq request;
    AppletCommUtils::Json2AttributeItem(jsonValue["item"], request.item);
    ret = TarsEncode<AddAttributeInfoReq>(request, reqData);
    if (ret)
    {
        ERRORLOG("TarsEncode err|"<< endl) ;
    }
    return ret;
}

int ProtocolHandler::PackAddAttributeInfoRsp(string &rspData)
{
    int ret = 0;
    AddAttributeInfoRsp response;
    ret = TarsDecode<AddAttributeInfoRsp>(_ctx->_vtRsp, response);
    if (ret)
    {
        ERRORLOG("TarsDecode err|"<< endl);
        return ret;
    }
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Value body(rapidjson::kObjectType);
    body.AddMember("errmsg", rapidjson::Value(response.errmsg.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
    body.AddMember("ret", response.ret, document.GetAllocator());

    document.AddMember("body", body, document.GetAllocator());
    CreateRspHead(document);
    rspData = AppletCommUtils::Document2Str(document);
    return ret;
}

int ProtocolHandler::ParseGetAttributeValueListReq(vector<char>& reqData)
{
    int ret = 0;
    const Value& jsonValue = _ctx->_document["body"];
    GetAttributeValueListReq request;
    request.attrId = RapidJsonUtil::GetJsonInt(jsonValue, "attrId");

    ret = TarsEncode<GetAttributeValueListReq>(request, reqData);
    if (ret)
    {
        ERRORLOG("TarsEncode err|"<< endl) ;
    }
    return ret;
}

int ProtocolHandler::PackGetAttributeValueListRsp(string &rspData)
{
    int ret = 0;
    GetAttributeValueListRsp response;
    ret = TarsDecode<GetAttributeValueListRsp>(_ctx->_vtRsp, response);
    if (ret)
    {
        ERRORLOG("TarsDecode err|"<< endl);
        return ret;
    }
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Value body(rapidjson::kObjectType);
    body.AddMember("errmsg", rapidjson::Value(response.errmsg.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
    body.AddMember("ret", response.ret, document.GetAllocator());

    rapidjson::Value mpAttrValue(rapidjson::kObjectType);
    for (map<int, vector<AttributeValueItem> >::const_iterator iter = response.mpAttrValue.begin(); iter != response.mpAttrValue.end(); ++iter)
    {
        rapidjson::Value itemList(kArrayType);
        for (size_t i = 0; i < iter->second.size(); ++i)
        {
            rapidjson::Value item(rapidjson::kObjectType);
            AppletCommUtils::AttributeValueItem2Json(document, iter->second[i], item);
            itemList.PushBack(item, document.GetAllocator());
        }
        mpAttrValue.AddMember(rapidjson::Value(I2S(iter->first).c_str(), document.GetAllocator()).Move(), itemList, document.GetAllocator());
    }
    body.AddMember("mpAttrValue", mpAttrValue, document.GetAllocator());

    document.AddMember("body", body, document.GetAllocator());
    CreateRspHead(document);
    rspData = AppletCommUtils::Document2Str(document);
    return ret;
}

int ProtocolHandler::ParseAddAttributeValueInfoReq(vector<char>& reqData)
{
    int ret = 0;
    const Value& jsonValue = _ctx->_document["body"];
    if (jsonValue.FindMember("item") == jsonValue.MemberEnd())
    {
        return -1;
    }
    AddAttributeValueInfoReq request;
    AppletCommUtils::Json2AttributeValueItem(jsonValue["item"], request.item);

    ret = TarsEncode<AddAttributeValueInfoReq>(request, reqData);
    if (ret)
    {
        ERRORLOG("TarsEncode err|"<< endl) ;
    }
    return ret;
}

int ProtocolHandler::PackAddAttributeValueInfoRsp(string &rspData)
{
    int ret = 0;
    AddAttributeValueInfoRsp response;
    ret = TarsDecode<AddAttributeValueInfoRsp>(_ctx->_vtRsp, response);
    if (ret)
    {
        ERRORLOG("TarsDecode err|"<< endl);
        return ret;
    }
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Value body(rapidjson::kObjectType);
    body.AddMember("errmsg", rapidjson::Value(response.errmsg.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
    body.AddMember("ret", response.ret, document.GetAllocator());

    document.AddMember("body", body, document.GetAllocator());
    CreateRspHead(document);
    rspData = AppletCommUtils::Document2Str(document);
    return ret;
}

int ProtocolHandler::ParseGetBrandListReq(vector<char>& reqData)
{
    int ret = 0;
    const Value& jsonValue = _ctx->_document["body"];
    GetBrandListReq request;
    request.reserved_field = RapidJsonUtil::GetJsonString(jsonValue, "reserved_field");
    ret = TarsEncode<GetBrandListReq>(request, reqData);
    if (ret)
    {
        ERRORLOG("TarsEncode err|"<< endl) ;
    }
    return ret;
}

int ProtocolHandler::PackGetBrandListRsp(string &rspData)
{
    int ret = 0;
    GetBrandListRsp response;
    ret = TarsDecode<GetBrandListRsp>(_ctx->_vtRsp, response);
    if (ret)
    {
        ERRORLOG("TarsDecode err|"<< endl);
        return ret;
    }
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Value body(rapidjson::kObjectType);
    body.AddMember("errmsg", rapidjson::Value(response.errmsg.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
    body.AddMember("ret", response.ret, document.GetAllocator());

    rapidjson::Value itemList(rapidjson::kArrayType);
    for (size_t i = 0; i < response.itemList.size(); ++i)
    {
        rapidjson::Value item(rapidjson::kObjectType);
        AppletCommUtils::BrandItem2Json(document, response.itemList[i], item);
        itemList.PushBack(item, document.GetAllocator());
    }
    body.AddMember("itemList", itemList, document.GetAllocator());

    document.AddMember("body", body, document.GetAllocator());
    CreateRspHead(document);
    rspData = AppletCommUtils::Document2Str(document);
    return ret;
}

int ProtocolHandler::ParseAddBrandInfoReq(vector<char>& reqData)
{
    int ret = 0;
    const Value& jsonValue = _ctx->_document["body"];
    if (jsonValue.FindMember("item") == jsonValue.MemberEnd())
    {
        return -1;
    }
    AddBrandInfoReq request;
    AppletCommUtils::Json2BrandItem(jsonValue["item"], request.item);
    ret = TarsEncode<AddBrandInfoReq>(request, reqData);
    if (ret)
    {
        ERRORLOG("TarsEncode err|"<< endl) ;
    }
    return ret;
}

int ProtocolHandler::PackAddBrandInfoRsp(string &rspData)
{
    int ret = 0;
    AddBrandInfoRsp response;
    ret = TarsDecode<AddBrandInfoRsp>(_ctx->_vtRsp, response);
    if (ret)
    {
        ERRORLOG("TarsDecode err|"<< endl);
        return ret;
    }
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Value body(rapidjson::kObjectType);
    body.AddMember("errmsg", rapidjson::Value(response.errmsg.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
    body.AddMember("ret", response.ret, document.GetAllocator());

    document.AddMember("body", body, document.GetAllocator());
    CreateRspHead(document);
    rspData = AppletCommUtils::Document2Str(document);
    return ret;
}

int ProtocolHandler::ParseGetMakerListReq(vector<char>& reqData)
{
    int ret = 0;
    const Value& jsonValue = _ctx->_document["body"];
    GetMakerListReq request;
    request.reserved_field = RapidJsonUtil::GetJsonString(jsonValue, "reserved_field");

    ret = TarsEncode<GetMakerListReq>(request, reqData);
    if (ret)
    {
        ERRORLOG("TarsEncode err|"<< endl) ;
    }
    return ret;
}

int ProtocolHandler::PackGetMakerListRsp(string &rspData)
{
    int ret = 0;
    GetMakerListRsp response;
    ret = TarsDecode<GetMakerListRsp>(_ctx->_vtRsp, response);
    if (ret)
    {
        ERRORLOG("TarsDecode err|"<< endl);
        return ret;
    }
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Value body(rapidjson::kObjectType);
    body.AddMember("errmsg", rapidjson::Value(response.errmsg.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
    body.AddMember("ret", response.ret, document.GetAllocator());

    rapidjson::Value itemList(rapidjson::kArrayType);
    for (size_t i = 0; i < response.itemList.size(); ++i)
    {
        rapidjson::Value item(rapidjson::kObjectType);
        AppletCommUtils::MakerItem2Json(document, response.itemList[i], item);
        itemList.PushBack(item, document.GetAllocator());
    }
    body.AddMember("itemList", itemList, document.GetAllocator());

    document.AddMember("body", body, document.GetAllocator());
    CreateRspHead(document);
    rspData = AppletCommUtils::Document2Str(document);
    return ret;
}

int ProtocolHandler::ParseAddMakerInfoReq(vector<char>& reqData)
{
    int ret = 0;
    const Value& jsonValue = _ctx->_document["body"];
    if (jsonValue.FindMember("item") == jsonValue.MemberEnd())
    {
        return -1;
    }
    AddMakerInfoReq request;
    AppletCommUtils::Json2MakerItem(jsonValue["item"], request.item);
    ret = TarsEncode<AddMakerInfoReq>(request, reqData);
    if (ret)
    {
        ERRORLOG("TarsEncode err|"<< endl) ;
    }
    return ret;
}

int ProtocolHandler::PackAddMakerInfoRsp(string &rspData)
{
    int ret = 0;
    AddMakerInfoRsp response;
    ret = TarsDecode<AddMakerInfoRsp>(_ctx->_vtRsp, response);
    if (ret)
    {
        ERRORLOG("TarsDecode err|"<< endl);
        return ret;
    }
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Value body(rapidjson::kObjectType);
    body.AddMember("errmsg", rapidjson::Value(response.errmsg.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
    body.AddMember("ret", response.ret, document.GetAllocator());

    document.AddMember("body", body, document.GetAllocator());
    CreateRspHead(document);
    rspData = AppletCommUtils::Document2Str(document);
    return ret;
}

int ProtocolHandler::ParseGetGoodsSPUListReq(vector<char>& reqData)
{
    int ret = 0;
    const Value& jsonValue = _ctx->_document["body"];
    GetGoodsSPUListReq request;
    request.name = RapidJsonUtil::GetJsonString(jsonValue, "name");
    ret = TarsEncode<GetGoodsSPUListReq>(request, reqData);
    if (ret)
    {
        ERRORLOG("TarsEncode err|"<< endl) ;
    }
    return ret;
}

int ProtocolHandler::PackGetGoodsSPUListRsp(string &rspData)
{
    int ret = 0;
    GetGoodsSPUListRsp response;
    ret = TarsDecode<GetGoodsSPUListRsp>(_ctx->_vtRsp, response);
    if (ret)
    {
        ERRORLOG("TarsDecode err|"<< endl);
        return ret;
    }
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Value body(rapidjson::kObjectType);
    body.AddMember("errmsg", rapidjson::Value(response.errmsg.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
    body.AddMember("ret", response.ret, document.GetAllocator());

    rapidjson::Value infoList(rapidjson::kArrayType);
    for (size_t i = 0; i < response.infoList.size(); ++i)
    {
        rapidjson::Value info(rapidjson::kObjectType);
        AppletCommUtils::GoodsSPUSimpleInfo2Json(document, response.infoList[i], info);
        infoList.PushBack(info, document.GetAllocator());
    }
    body.AddMember("infoList", infoList, document.GetAllocator());

    document.AddMember("body", body, document.GetAllocator());
    CreateRspHead(document);
    rspData = AppletCommUtils::Document2Str(document);
    return ret;
}

int ProtocolHandler::ParseAddGoodsSPUInfoReq(vector<char>& reqData)
{
    int ret = 0;
    const Value& jsonValue = _ctx->_document["body"];
    if (jsonValue.FindMember("detail") == jsonValue.MemberEnd() || jsonValue["detail"].FindMember("mediaInfo") == jsonValue["detail"].MemberEnd()
        || jsonValue["detail"].FindMember("categoryInfo") == jsonValue["detail"].MemberEnd() || !jsonValue["detail"]["categoryInfo"].IsArray())
    {
        return -1;
    }
    AddGoodsSPUInfoReq request;
    AppletCommUtils::Json2GoodsSPUDetail(jsonValue["detail"], request.detail);

    ret = TarsEncode<AddGoodsSPUInfoReq>(request, reqData);
    if (ret)
    {
        ERRORLOG("TarsEncode err|"<< endl) ;
    }
    return ret;
}

int ProtocolHandler::PackAddGoodsSPUInfoRsp(string &rspData)
{
    int ret = 0;
    AddGoodsSPUInfoRsp response;
    ret = TarsDecode<AddGoodsSPUInfoRsp>(_ctx->_vtRsp, response);
    if (ret)
    {
        ERRORLOG("TarsDecode err|"<< endl);
        return ret;
    }
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Value body(rapidjson::kObjectType);
    body.AddMember("errmsg", rapidjson::Value(response.errmsg.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
    body.AddMember("ret", response.ret, document.GetAllocator());

    document.AddMember("body", body, document.GetAllocator());
    CreateRspHead(document);
    rspData = AppletCommUtils::Document2Str(document);
    return ret;
}

int ProtocolHandler::ParseAddGoodsSKUInfoReq(vector<char>& reqData)
{
    int ret = 0;
    const Value& jsonValue = _ctx->_document["body"];
    if (jsonValue.FindMember("skuInfo") == jsonValue.MemberEnd() || jsonValue["skuInfo"].FindMember("attrList") == jsonValue["skuInfo"].MemberEnd()
        || !jsonValue["skuInfo"]["attrList"].IsArray())
    {
        return -1;
    }

    AddGoodsSKUInfoReq request;
    AppletCommUtils::Json2GoodsSKUInfo(jsonValue["skuInfo"], request.skuInfo);
    ret = TarsEncode<AddGoodsSKUInfoReq>(request, reqData);
    if (ret)
    {
        ERRORLOG("TarsEncode err|"<< endl) ;
    }
    return ret;
}

int ProtocolHandler::PackAddGoodsSKUInfoRsp(string &rspData)
{
    int ret = 0;
    AddGoodsSKUInfoRsp response;
    ret = TarsDecode<AddGoodsSKUInfoRsp>(_ctx->_vtRsp, response);
    if (ret)
    {
        ERRORLOG("TarsDecode err|"<< endl);
        return ret;
    }
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Value body(rapidjson::kObjectType);
    body.AddMember("errmsg", rapidjson::Value(response.errmsg.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
    body.AddMember("ret", response.ret, document.GetAllocator());

    document.AddMember("body", body, document.GetAllocator());
    CreateRspHead(document);
    rspData = AppletCommUtils::Document2Str(document);
    return ret;
}

int ProtocolHandler::ParseGetGoodsSKUListReq(vector<char>& reqData)
{
    int ret = 0;
    const Value& jsonValue = _ctx->_document["body"];
    GetGoodsSKUListReq request;
    if (jsonValue.FindMember("spuId") == jsonValue.MemberEnd())
    {
        return -1;
    }
    request.spuId = RapidJsonUtil::GetJsonInt(jsonValue, "spuId");

    if (jsonValue.FindMember("attrList") != jsonValue.MemberEnd() && jsonValue["attrList"].IsArray())
    {
        const Value& attrList = jsonValue["attrList"];
        for (Value::ConstValueIterator itr = attrList.Begin(); itr != attrList.End(); ++itr)
        {
            HardwareApplet::GoodsSKUAttrInfo item;
            AppletCommUtils::Json2GoodsSKUAttrInfo(*itr, item);
            request.attrList.push_back(item);
        }
    }

    ret = TarsEncode<GetGoodsSKUListReq>(request, reqData);
    if (ret)
    {
        ERRORLOG("TarsEncode err|"<< endl) ;
    }
    return ret;
}

int ProtocolHandler::PackGetGoodsSKUListRsp(string &rspData)
{
    int ret = 0;
    GetGoodsSKUListRsp response;
    ret = TarsDecode<GetGoodsSKUListRsp>(_ctx->_vtRsp, response);
    if (ret)
    {
        ERRORLOG("TarsDecode err|"<< endl);
        return ret;
    }
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Value body(rapidjson::kObjectType);
    body.AddMember("errmsg", rapidjson::Value(response.errmsg.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
    body.AddMember("ret", response.ret, document.GetAllocator());
    ////////////////////////////////////////////////////////////////
    rapidjson::Value infoList(rapidjson::kArrayType);
    for (size_t i = 0; i < response.infoList.size(); ++i)
    {
        rapidjson::Value info(rapidjson::kObjectType);
        AppletCommUtils::GoodsSKUInfo2Json(document, response.infoList[i], info);

        infoList.PushBack(info, document.GetAllocator());
    }
    body.AddMember("infoList", infoList, document.GetAllocator());
    ////////////////////////////////////////////////////////////////////
    document.AddMember("body", body, document.GetAllocator());
    CreateRspHead(document);
    rspData = AppletCommUtils::Document2Str(document);
    return ret;
}

int ProtocolHandler::ParseAddMyAddressInfoReq(vector<char>& reqData)
{
    int ret = 0;
    const Value& jsonValue = _ctx->_document["body"];
    AddMyAddressInfoReq request;
    if (jsonValue.FindMember("addressInfo") == jsonValue.MemberEnd())
    {
        return -1;
    }
    AppletCommUtils::Json2AddressInfo(jsonValue["addressInfo"], request.addressInfo);
    ret = TarsEncode<AddMyAddressInfoReq>(request, reqData);
    if (ret)
    {
        ERRORLOG("TarsEncode err|"<< endl) ;
    }
    return ret;
}

int ProtocolHandler::PackAddMyAddressInfoRsp(string &rspData)
{
    int ret = 0;
    AddMyAddressInfoRsp response;
    ret = TarsDecode<AddMyAddressInfoRsp>(_ctx->_vtRsp, response);
    if (ret)
    {
        ERRORLOG("TarsDecode err|"<< endl);
        return ret;
    }
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Value body(rapidjson::kObjectType);
    body.AddMember("errmsg", rapidjson::Value(response.errmsg.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
    body.AddMember("ret", response.ret, document.GetAllocator());

    document.AddMember("body", body, document.GetAllocator());
    CreateRspHead(document);
    rspData = AppletCommUtils::Document2Str(document);
    return ret;
}

int ProtocolHandler::ParseGetMyAddressListReq(vector<char>& reqData)
{
    int ret = 0;
    const Value& jsonValue = _ctx->_document["body"];
    GetMyAddressListReq request;
    if (jsonValue.FindMember("reserved_field") == jsonValue.MemberEnd())
    {
        return -1;
    }
    request.reserved_field = RapidJsonUtil::GetJsonString(jsonValue, "reserved_field");
    ret = TarsEncode<GetMyAddressListReq>(request, reqData);
    if (ret)
    {
        ERRORLOG("TarsEncode err|"<< endl) ;
    }
    return ret;
}

int ProtocolHandler::PackGetMyAddressListRsp(string &rspData)
{
    int ret = 0;
    GetMyAddressListRsp response;
    ret = TarsDecode<GetMyAddressListRsp>(_ctx->_vtRsp, response);
    if (ret)
    {
        ERRORLOG("TarsDecode err|"<< endl);
        return ret;
    }
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Value body(rapidjson::kObjectType);
    body.AddMember("errmsg", rapidjson::Value(response.errmsg.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
    body.AddMember("ret", response.ret, document.GetAllocator());
    ////////////////////////////////////////////////////////////////
    rapidjson::Value infoList(rapidjson::kArrayType);
    for (size_t i = 0; i < response.infoList.size(); ++i)
    {
        rapidjson::Value info(rapidjson::kObjectType);
        AppletCommUtils::AddressInfo2Json(document, response.infoList[i], info);
        infoList.PushBack(info, document.GetAllocator());
    }
    body.AddMember("infoList", infoList, document.GetAllocator());
    ////////////////////////////////////////////////////////////////
    document.AddMember("body", body, document.GetAllocator());
    CreateRspHead(document);
    rspData = AppletCommUtils::Document2Str(document);
    return ret;
}

int ProtocolHandler::ParseGetProvinceListReq(vector<char>& reqData)
{
    int ret = 0;
    const Value& jsonValue = _ctx->_document["body"];
    GetProvinceListReq request;
    if (jsonValue.FindMember("reserved_field") == jsonValue.MemberEnd())
    {
        return -1;
    }
    request.reserved_field = RapidJsonUtil::GetJsonString(jsonValue, "reserved_field");
    ret = TarsEncode<GetProvinceListReq>(request, reqData);
    if (ret)
    {
        ERRORLOG("TarsEncode err|"<< endl) ;
    }
    return ret;
}

int ProtocolHandler::PackGetProvinceListRsp(string &rspData)
{
    int ret = 0;
    GetProvinceListRsp response;
    ret = TarsDecode<GetProvinceListRsp>(_ctx->_vtRsp, response);
    if (ret)
    {
        ERRORLOG("TarsDecode err|"<< endl);
        return ret;
    }
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Value body(rapidjson::kObjectType);
    body.AddMember("errmsg", rapidjson::Value(response.errmsg.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
    body.AddMember("ret", response.ret, document.GetAllocator());
    /////////////////////////////////////////////////////////////////////
    rapidjson::Value nameList(rapidjson::kArrayType);
    for (size_t i = 0; i < response.nameList.size(); ++i)
    {
        nameList.PushBack(rapidjson::Value(response.nameList[i].c_str(), document.GetAllocator()).Move(), document.GetAllocator());
    }
    body.AddMember("nameList", nameList, document.GetAllocator());
    /////////////////////////////////////////////////////////////////////
    document.AddMember("body", body, document.GetAllocator());
    CreateRspHead(document);
    rspData = AppletCommUtils::Document2Str(document);
    return ret;
}

int ProtocolHandler::ParseGetCityListByProvinceReq(vector<char>& reqData)
{
    int ret = 0;
    const Value& jsonValue = _ctx->_document["body"];
    GetCityListByProvinceReq request;
    if (jsonValue.FindMember("provinceName") == jsonValue.MemberEnd())
    {
        return -1;
    }
    request.provinceName = RapidJsonUtil::GetJsonString(jsonValue, "provinceName");
    ret = TarsEncode<GetCityListByProvinceReq>(request, reqData);
    if (ret)
    {
        ERRORLOG("TarsEncode err|"<< endl) ;
    }
    return ret;
}

int ProtocolHandler::PackGetCityListByProvinceRsp(string &rspData)
{
    int ret = 0;
    GetCityListByProvinceRsp response;
    ret = TarsDecode<GetCityListByProvinceRsp>(_ctx->_vtRsp, response);
    if (ret)
    {
        ERRORLOG("TarsDecode err|"<< endl);
        return ret;
    }
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Value body(rapidjson::kObjectType);
    body.AddMember("errmsg", rapidjson::Value(response.errmsg.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
    body.AddMember("ret", response.ret, document.GetAllocator());
    /////////////////////////////////////////////////////////////////////
    rapidjson::Value nameList(rapidjson::kArrayType);
    for (size_t i = 0; i < response.nameList.size(); ++i)
    {
        nameList.PushBack(rapidjson::Value(response.nameList[i].c_str(), document.GetAllocator()).Move(), document.GetAllocator());
    }
    body.AddMember("nameList", nameList, document.GetAllocator());
    /////////////////////////////////////////////////////////////////////
    document.AddMember("body", body, document.GetAllocator());
    CreateRspHead(document);
    rspData = AppletCommUtils::Document2Str(document);
    return ret;
}

int ProtocolHandler::ParseGetCountyListByCityReq(vector<char>& reqData)
{
    int ret = 0;
    const Value& jsonValue = _ctx->_document["body"];
    GetCountyListByCityReq request;
    if (jsonValue.FindMember("cityName") == jsonValue.MemberEnd())
    {
        return -1;
    }
    request.cityName = RapidJsonUtil::GetJsonString(jsonValue, "cityName");
    ret = TarsEncode<GetCountyListByCityReq>(request, reqData);
    if (ret)
    {
        ERRORLOG("TarsEncode err|"<< endl) ;
    }
    return ret;
}

int ProtocolHandler::PackGetCountyListByCityRsp(string &rspData)
{
    int ret = 0;
    GetCountyListByCityRsp response;
    ret = TarsDecode<GetCountyListByCityRsp>(_ctx->_vtRsp, response);
    if (ret)
    {
        ERRORLOG("TarsDecode err|"<< endl);
        return ret;
    }
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Value body(rapidjson::kObjectType);
    body.AddMember("errmsg", rapidjson::Value(response.errmsg.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
    body.AddMember("ret", response.ret, document.GetAllocator());
    /////////////////////////////////////////////////////////////////////
    rapidjson::Value nameList(rapidjson::kArrayType);
    for (size_t i = 0; i < response.nameList.size(); ++i)
    {
        nameList.PushBack(rapidjson::Value(response.nameList[i].c_str(), document.GetAllocator()).Move(), document.GetAllocator());
    }
    body.AddMember("nameList", nameList, document.GetAllocator());
    /////////////////////////////////////////////////////////////////////
    document.AddMember("body", body, document.GetAllocator());
    CreateRspHead(document);
    rspData = AppletCommUtils::Document2Str(document);
    return ret;
}

int ProtocolHandler::ParseAddGoodsToShopCartReq(vector<char>& reqData)
{
    int ret = 0;
    const Value& jsonValue = _ctx->_document["body"];
    AddGoodsToShopCartReq request;
    
    request.spuId = RapidJsonUtil::GetJsonInt(jsonValue, "spuId");
    request.skuId = RapidJsonUtil::GetJsonInt(jsonValue, "skuId");
    request.price = RapidJsonUtil::GetJsonInt(jsonValue, "price");
    request.tranPrice = RapidJsonUtil::GetJsonInt(jsonValue, "tranPrice");
    request.num = RapidJsonUtil::GetJsonInt(jsonValue, "num");

    ret = TarsEncode<AddGoodsToShopCartReq>(request, reqData);
    if (ret)
    {
        ERRORLOG("TarsEncode err|"<< endl) ;
    }
    return ret;
}

int ProtocolHandler::PackAddGoodsToShopCartRsp(string &rspData)
{
    int ret = 0;
    AddGoodsToShopCartRsp response;
    ret = TarsDecode<AddGoodsToShopCartRsp>(_ctx->_vtRsp, response);
    if (ret)
    {
        ERRORLOG("TarsDecode err|"<< endl);
        return ret;
    }
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Value body(rapidjson::kObjectType);
    body.AddMember("errmsg", rapidjson::Value(response.errmsg.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
    body.AddMember("ret", response.ret, document.GetAllocator());

    document.AddMember("body", body, document.GetAllocator());
    CreateRspHead(document);
    rspData = AppletCommUtils::Document2Str(document);
    return ret;
}

int ProtocolHandler::ParseGetMyShopCartInfoReq(vector<char>& reqData)
{
    int ret = 0;
    const Value& jsonValue = _ctx->_document["body"];
    GetMyShopCartInfoReq request;
    if (jsonValue.FindMember("reserved_field") == jsonValue.MemberEnd())
    {
        return -1;
    }
    request.reserved_field = RapidJsonUtil::GetJsonString(jsonValue, "reserved_field");
    ret = TarsEncode<GetMyShopCartInfoReq>(request, reqData);
    if (ret)
    {
        ERRORLOG("TarsEncode err|"<< endl) ;
    }
    return ret;
}

int ProtocolHandler::PackGetMyShopCartInfoRsp(string &rspData)
{
    int ret = 0;
    GetMyShopCartInfoRsp response;
    ret = TarsDecode<GetMyShopCartInfoRsp>(_ctx->_vtRsp, response);
    if (ret)
    {
        ERRORLOG("TarsDecode err|"<< endl);
        return ret;
    }
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Value body(rapidjson::kObjectType);
    body.AddMember("errmsg", rapidjson::Value(response.errmsg.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
    body.AddMember("ret", response.ret, document.GetAllocator());
    //////////////////////////////////////////////////////////////////////
    rapidjson::Value itemList(rapidjson::kArrayType);
    for (size_t i = 0; i < response.itemList.size(); ++i)
    {
        rapidjson::Value item(rapidjson::kObjectType);
        AppletCommUtils::ShopCartItem2Json(document, response.itemList[i], item);
        itemList.PushBack(item, document.GetAllocator());
    }
    body.AddMember("itemList", itemList, document.GetAllocator());
    //////////////////////////////////////////////////////////////////////
    document.AddMember("body", body, document.GetAllocator());
    CreateRspHead(document);
    rspData = AppletCommUtils::Document2Str(document);
    return ret;
}

int ProtocolHandler::ParseSubmitOrderReq(vector<char>& reqData)
{
    int ret = 0;
    const Value& jsonValue = _ctx->_document["body"];
    
    if (jsonValue.FindMember("item") == jsonValue.MemberEnd() || jsonValue["item"].FindMember("addressInfo") == jsonValue["detail"].MemberEnd()
        || jsonValue["item"].FindMember("itemList") == jsonValue["detail"].MemberEnd() || !jsonValue["detail"]["itemList"].IsArray())
    {
        return -1;
    }
    SubmitOrderReq request;
    AppletCommUtils::Json2OrderItem(jsonValue["item"], request.item);

    ret = TarsEncode<SubmitOrderReq>(request, reqData);
    if (ret)
    {
        ERRORLOG("TarsEncode err|"<< endl) ;
    }
    return ret;
}

int ProtocolHandler::PackSubmitOrderRsp(string &rspData)
{
    int ret = 0;
    SubmitOrderRsp response;
    ret = TarsDecode<SubmitOrderRsp>(_ctx->_vtRsp, response);
    if (ret)
    {
        ERRORLOG("TarsDecode err|"<< endl);
        return ret;
    }
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Value body(rapidjson::kObjectType);
    body.AddMember("errmsg", rapidjson::Value(response.errmsg.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
    body.AddMember("ret", response.ret, document.GetAllocator());

    document.AddMember("body", body, document.GetAllocator());
    CreateRspHead(document);
    rspData = AppletCommUtils::Document2Str(document);
    return ret;
}

int ProtocolHandler::ParseConfirmOrderReq(vector<char>& reqData)
{
    int ret = 0;
    const Value& jsonValue = _ctx->_document["body"];
    
    if (jsonValue.FindMember("item") == jsonValue.MemberEnd() || jsonValue["item"].FindMember("addressInfo") == jsonValue["detail"].MemberEnd()
        || jsonValue["item"].FindMember("itemList") == jsonValue["detail"].MemberEnd() || !jsonValue["detail"]["itemList"].IsArray())
    {
        return -1;
    }
    ConfirmOrderReq request;
    AppletCommUtils::Json2OrderItem(jsonValue["item"], request.item);
    ret = TarsEncode<ConfirmOrderReq>(request, reqData);
    if (ret)
    {
        ERRORLOG("TarsEncode err|"<< endl) ;
    }
    return ret;
}

int ProtocolHandler::PackConfirmOrderRsp(string &rspData)
{
    int ret = 0;
    ConfirmOrderRsp response;
    ret = TarsDecode<ConfirmOrderRsp>(_ctx->_vtRsp, response);
    if (ret)
    {
        ERRORLOG("TarsDecode err|"<< endl);
        return ret;
    }
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Value body(rapidjson::kObjectType);
    body.AddMember("errmsg", rapidjson::Value(response.errmsg.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
    body.AddMember("ret", response.ret, document.GetAllocator());

    document.AddMember("body", body, document.GetAllocator());
    CreateRspHead(document);
    rspData = AppletCommUtils::Document2Str(document);
    return ret;
}
