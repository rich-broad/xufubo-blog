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
    for (map<int, CategoryItem>::const_iterator iter = response.mpCategory.begin(); iter != response.mpCategory.end(); ++iter)
    {
        rapidjson::Value item(rapidjson::kObjectType);
        AppletCommUtils::CategoryItem2Json(document, iter->second, item);
        // mpCategory.AddMember(I2S(iter->first).c_str(), item, document.GetAllocator());
        mpCategory.AddMember(rapidjson::Value(I2S(iter->first).c_str(), document.GetAllocator()).Move(), item, document.GetAllocator());
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
    const Value& item = jsonValue["item"];
    AddCategoryInfoReq request;
    request.item.categoryName = RapidJsonUtil::GetJsonString(item, "categoryName");
    request.item.parentId = RapidJsonUtil::GetJsonInt(item, "parentId");
    request.item.level = RapidJsonUtil::GetJsonInt(item, "level");
    request.item.imgUrl = RapidJsonUtil::GetJsonString(item, "imgUrl");
    
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
    AddAttributeInfoReq request;
    if (jsonValue.FindMember("item") == jsonValue.MemberEnd())
    {
        return -1;
    }
    const Value& item = jsonValue["item"];
    request.item.attrName = RapidJsonUtil::GetJsonString(item, "attrName");
    request.item.attrDesc = RapidJsonUtil::GetJsonString(item, "attrDesc");
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
    for (map<int, AttributeValueItem>::const_iterator iter = response.mpAttrValue.begin(); iter != response.mpAttrValue.end(); ++iter)
    {
        rapidjson::Value item(rapidjson::kObjectType);
        AppletCommUtils::AttributeValueItem2Json(document, iter->second, item);
        mpAttrValue.AddMember(rapidjson::Value(I2S(iter->first).c_str(), document.GetAllocator()).Move(), item, document.GetAllocator());
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
    AddAttributeValueInfoReq request;
    if (jsonValue.FindMember("item") == jsonValue.MemberEnd())
    {
        return -1;
    }
    const Value& item = jsonValue["item"];
    request.item.name = RapidJsonUtil::GetJsonString(item, "name");
    request.item.attrId = RapidJsonUtil::GetJsonInt(item, "attrId");

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
    AddBrandInfoReq request;
    if (jsonValue.FindMember("item") == jsonValue.MemberEnd())
    {
        return -1;
    }
    const Value& item = jsonValue["item"];
    request.item.brandName = RapidJsonUtil::GetJsonString(item, "brandName");
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
    AddMakerInfoReq request;
    if (jsonValue.FindMember("item") == jsonValue.MemberEnd())
    {
        return -1;
    }
    const Value& item = jsonValue["item"];
    request.item.makerName = RapidJsonUtil::GetJsonString(item, "makerName");
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
    
    const Value& detail = jsonValue["detail"];
    AddGoodsSPUInfoReq request;
    request.detail.name = RapidJsonUtil::GetJsonString(detail, "name");
    request.detail.brandName = RapidJsonUtil::GetJsonString(detail, "brandName");
    request.detail.maker = RapidJsonUtil::GetJsonString(detail, "maker");
    request.detail.sDesc = RapidJsonUtil::GetJsonString(detail, "sDesc");
    request.detail.brandId = RapidJsonUtil::GetJsonInt(detail, "brandId");
    request.detail.desc = RapidJsonUtil::GetJsonString(detail, "desc");

    const Value& mediaInfo = detail["mediaInfo"];
    request.detail.mediaInfo.mediaId = RapidJsonUtil::GetJsonInt(mediaInfo, "mediaId");
    request.detail.mediaInfo.type = RapidJsonUtil::GetJsonInt(mediaInfo, "type");
    request.detail.mediaInfo.position = RapidJsonUtil::GetJsonInt(mediaInfo, "position");
    request.detail.mediaInfo.mediaUrl = RapidJsonUtil::GetJsonString(mediaInfo, "mediaUrl");

    const Value& categoryInfo = detail["categoryInfo"];
    for (Value::ConstValueIterator itr = categoryInfo.Begin(); itr != categoryInfo.End(); ++itr)
    {
        HardwareApplet::CategoryItem item;
        item.categoryId = RapidJsonUtil::GetJsonInt(*itr, "categoryId");
        item.categoryName = RapidJsonUtil::GetJsonString(*itr, "categoryName");
        item.parentId = RapidJsonUtil::GetJsonInt(*itr, "parentId");
        item.level = RapidJsonUtil::GetJsonInt(*itr, "level");
        item.imgUrl = RapidJsonUtil::GetJsonString(*itr, "imgUrl");
        request.detail.categoryInfo.push_back(item);
    }

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

    const Value& skuInfo = jsonValue["skuInfo"];
    AddGoodsSKUInfoReq request;
    request.skuInfo.spuId = RapidJsonUtil::GetJsonInt(skuInfo, "spuId");
    request.skuInfo.name = RapidJsonUtil::GetJsonString(skuInfo, "name");
    request.skuInfo.stock = RapidJsonUtil::GetJsonInt(skuInfo, "stock");
    request.skuInfo.warnStock = RapidJsonUtil::GetJsonInt(skuInfo, "warnStock");
    request.skuInfo.price = RapidJsonUtil::GetJsonInt(skuInfo, "price");
    request.skuInfo.minCount = RapidJsonUtil::GetJsonInt(skuInfo, "minCount");
    request.skuInfo.isDefault = RapidJsonUtil::GetJsonInt(skuInfo, "isDefault");
    request.skuInfo.imgUrl = RapidJsonUtil::GetJsonString(skuInfo, "imgUrl");

    const Value& attrList = skuInfo["attrList"];
    for (Value::ConstValueIterator itr = attrList.Begin(); itr != attrList.End(); ++itr)
    {
        HardwareApplet::GoodsSKUAttrInfo item;
        item.attrId = RapidJsonUtil::GetJsonInt(*itr, "attrId");
        item.attrName = RapidJsonUtil::GetJsonString(*itr, "attrName");
        item.attrValueId = RapidJsonUtil::GetJsonInt(*itr, "attrValueId");
        item.attrValueName = RapidJsonUtil::GetJsonString(*itr, "attrValueName");
        request.skuInfo.attrList.push_back(item);
    }

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
            item.attrId = RapidJsonUtil::GetJsonInt(*itr, "attrId");
            item.attrName = RapidJsonUtil::GetJsonString(*itr, "attrName");
            item.attrValueId = RapidJsonUtil::GetJsonInt(*itr, "attrValueId");
            item.attrValueName = RapidJsonUtil::GetJsonString(*itr, "attrValueName");
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
    const Value& addressInfo = jsonValue["addressInfo"];
    request.addressInfo.uid = RapidJsonUtil::GetJsonInt(addressInfo, "uid");
    request.addressInfo.phoneNum = RapidJsonUtil::GetJsonString(addressInfo, "phoneNum");
    request.addressInfo.province = RapidJsonUtil::GetJsonString(addressInfo, "province");
    request.addressInfo.city = RapidJsonUtil::GetJsonString(addressInfo, "city");
    request.addressInfo.county = RapidJsonUtil::GetJsonString(addressInfo, "county");
    request.addressInfo.addressDetail = RapidJsonUtil::GetJsonString(addressInfo, "addressDetail");
    request.addressInfo.receiver = RapidJsonUtil::GetJsonString(addressInfo, "receiver");
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
    request.tran_price = RapidJsonUtil::GetJsonInt(jsonValue, "tran_price");
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
    const Value& item = jsonValue["item"];
    request.item.uid = RapidJsonUtil::GetJsonInt(item, "uid");
    request.item.payWay = RapidJsonUtil::GetJsonInt(item, "payWay");
    request.item.money = RapidJsonUtil::GetJsonInt(item, "money");
    request.item.tranMoney = RapidJsonUtil::GetJsonInt(item, "tranMoney");
    request.item.freight = RapidJsonUtil::GetJsonInt(item, "freight");

    const Value& addressInfo = item["addressInfo"];
    request.item.addressInfo.addressId = RapidJsonUtil::GetJsonInt(addressInfo, "addressId");

    const Value& itemList = item["itemList"];
    for (Value::ConstValueIterator itr = itemList.Begin(); itr != itemList.End(); ++itr)
    {
        HardwareApplet::ShopCartItem item;
        item.cartId = RapidJsonUtil::GetJsonInt(*itr, "cartId");
        request.item.itemList.push_back(item);
    }
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
