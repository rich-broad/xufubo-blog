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


int ProtocolHandler::Json2Tars(vector<char>& reqData)
{
    int ret = 0;
    if (_ctx->_funcName == "getNewTicket")
    {
        ret = ParseJson2Request<GetNewTicketReq>(reqData);
    }
    else if (_ctx->_funcName == "getCategoryList")
    {
        ret = ParseJson2Request<GetCategoryListReq>(reqData);
    }
    else if (_ctx->_funcName == "addCategoryInfo")
    {
        ret = ParseJson2Request<AddCategoryInfoReq>(reqData);
    }
    else if (_ctx->_funcName == "getAttributeList")
    {
        ret = ParseJson2Request<GetAttributeListReq>(reqData);
    }
    else if (_ctx->_funcName == "addAttributeInfo")
    {
        ret = ParseJson2Request<AddAttributeInfoReq>(reqData);
    }
    else if (_ctx->_funcName == "getAttributeValueList")
    {
        ret = ParseJson2Request<GetAttributeValueListReq>(reqData);
    }
    else if (_ctx->_funcName == "addAttributeValueInfo")
    {
        ret = ParseJson2Request<AddAttributeValueInfoReq>(reqData);
    }
    else if (_ctx->_funcName == "getBrandList")
    {
        ret = ParseJson2Request<GetBrandListReq>(reqData);
    }
    else if (_ctx->_funcName == "addBrandInfo")
    {
        ret = ParseJson2Request<AddBrandInfoReq>(reqData);
    }
    else if (_ctx->_funcName == "getMakerList")
    {
        ret = ParseJson2Request<GetMakerListReq>(reqData);
    }
    else if (_ctx->_funcName == "addMakerInfo")
    {
        ret = ParseJson2Request<AddMakerInfoReq>(reqData);
    }
    else if (_ctx->_funcName == "getGoodsSPUList")
    {
        ret = ParseJson2Request<GetGoodsSPUListReq>(reqData);
    }
    else if (_ctx->_funcName == "addGoodsSPUInfo")
    {
        ret = ParseJson2Request<AddGoodsSPUInfoReq>(reqData);
    }
    else if (_ctx->_funcName == "addGoodsSKUInfo")
    {
        ret = ParseJson2Request<AddGoodsSKUInfoReq>(reqData);
    }
    else if (_ctx->_funcName == "getGoodsSKUList")
    {
        ret = ParseJson2Request<GetGoodsSKUListReq>(reqData);
    }
    else if (_ctx->_funcName == "addMyAddressInfo")
    {
        ret = ParseJson2Request<AddMyAddressInfoReq>(reqData);
    }
    else if (_ctx->_funcName == "getMyAddressList")
    {
        ret = ParseJson2Request<GetMyAddressListReq>(reqData);
    }
    else if (_ctx->_funcName == "addGoodsToShopCart")
    {
        ret = ParseJson2Request<AddGoodsToShopCartReq>(reqData);
    }
    else if (_ctx->_funcName == "getMyShopCartInfo")
    {
        ret = ParseJson2Request<GetMyShopCartInfoReq>(reqData);
    }
    else if (_ctx->_funcName == "submitOrder")
    {
        ret = ParseJson2Request<SubmitOrderReq>(reqData);
    }
    else if (_ctx->_funcName == "confirmOrder")
    {
        ret = ParseJson2Request<ConfirmOrderReq>(reqData);
    }
    else if (_ctx->_funcName == "getProvinceList")
    {
        ret = ParseJson2Request<GetProvinceListReq>(reqData);
    }
    else if (_ctx->_funcName == "getCityListByProvince")
    {
        ret = ParseJson2Request<GetCityListByProvinceReq>(reqData);
    }
    else if (_ctx->_funcName == "getCountyListByCity")
    {
        ret = ParseJson2Request<GetCountyListByCityReq>(reqData);
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
        ret = PackJsonFromResponse<ProxyGetNewTicketRsp, GetNewTicketRsp>(rspData);
    }
    else if (_ctx->_funcName == "getCategoryList")
    {
        ret = PackJsonFromResponse<ProxyGetCategoryListRsp, GetCategoryListRsp>(rspData);
    }
    else if (_ctx->_funcName == "addCategoryInfo")
    {
        ret = PackJsonFromResponse<ProxyAddCategoryInfoRsp, AddCategoryInfoRsp>(rspData);
    }
    else if (_ctx->_funcName == "getAttributeList")
    {
        ret = PackJsonFromResponse<ProxyGetAttributeListRsp, GetAttributeListRsp>(rspData);
    }
    else if (_ctx->_funcName == "addAttributeInfo")
    {
        ret = PackJsonFromResponse<ProxyAddAttributeInfoRsp, AddAttributeInfoRsp>(rspData);
    }
    else if (_ctx->_funcName == "getAttributeValueList")
    {
        ret = PackJsonFromResponse<ProxyGetAttributeValueListRsp, GetAttributeValueListRsp>(rspData);
    }
    else if (_ctx->_funcName == "addAttributeValueInfo")
    {
        ret = PackJsonFromResponse<ProxyAddAttributeValueInfoRsp, AddAttributeValueInfoRsp>(rspData);
    }
    else if (_ctx->_funcName == "getBrandList")
    {
        ret = PackJsonFromResponse<ProxyGetBrandListRsp, GetBrandListRsp>(rspData);
    }
    else if (_ctx->_funcName == "addBrandInfo")
    {
        ret = PackJsonFromResponse<ProxyAddBrandInfoRsp, AddBrandInfoRsp>(rspData);
    }
    else if (_ctx->_funcName == "getMakerList")
    {
        ret = PackJsonFromResponse<ProxyGetMakerListRsp, GetMakerListRsp>(rspData);
    }
    else if (_ctx->_funcName == "addMakerInfo")
    {
        ret = PackJsonFromResponse<ProxyAddMakerInfoRsp, AddMakerInfoRsp>(rspData);
    }
    else if (_ctx->_funcName == "getGoodsSPUList")
    {
        ret = PackJsonFromResponse<ProxyGetGoodsSPUListRsp, GetGoodsSPUListRsp>(rspData);
    }
    else if (_ctx->_funcName == "addGoodsSPUInfo")
    {
        ret = PackJsonFromResponse<ProxyAddGoodsSPUInfoRsp, AddGoodsSPUInfoRsp>(rspData);
    }
    else if (_ctx->_funcName == "addGoodsSKUInfo")
    {
        ret = PackJsonFromResponse<ProxyAddGoodsSKUInfoRsp, AddGoodsSKUInfoRsp>(rspData);
    }
    else if (_ctx->_funcName == "getGoodsSKUList")
    {
        ret = PackJsonFromResponse<ProxyGetGoodsSKUListRsp, GetGoodsSKUListRsp>(rspData);
    }
    else if (_ctx->_funcName == "addMyAddressInfo")
    {
        ret = PackJsonFromResponse<ProxyAddMyAddressInfoRsp, AddMyAddressInfoRsp>(rspData);
    }
    else if (_ctx->_funcName == "getMyAddressList")
    {
        ret = PackJsonFromResponse<ProxyGetMyAddressListRsp, GetMyAddressListRsp>(rspData);
    }
    else if (_ctx->_funcName == "addGoodsToShopCart")
    {
        ret = PackJsonFromResponse<ProxyAddGoodsToShopCartRsp, AddGoodsToShopCartRsp>(rspData);
    }
    else if (_ctx->_funcName == "getMyShopCartInfo")
    {
        ret = PackJsonFromResponse<ProxyGetMyShopCartInfoRsp, GetMyShopCartInfoRsp>(rspData);
    }
    else if (_ctx->_funcName == "submitOrder")
    {
        ret = PackJsonFromResponse<ProxySubmitOrderRsp, SubmitOrderRsp>(rspData);
    }
    else if (_ctx->_funcName == "confirmOrder")
    {
        ret = PackJsonFromResponse<ProxyConfirmOrderRsp, ConfirmOrderRsp>(rspData);
    }
    else if (_ctx->_funcName == "getProvinceList")
    {
        ret = PackJsonFromResponse<ProxyGetProvinceListRsp, GetProvinceListRsp>(rspData);
    }
    else if (_ctx->_funcName == "getCityListByProvince")
    {
        ret = PackJsonFromResponse<ProxyGetCityListByProvinceRsp, GetCityListByProvinceRsp>(rspData);
    }
    else if (_ctx->_funcName == "getCountyListByCity")
    {
        ret = PackJsonFromResponse<ProxyGetCountyListByCityRsp, GetCountyListByCityRsp>(rspData);
    }
    else
    {
        return -1;
    }
    return ret;
}
