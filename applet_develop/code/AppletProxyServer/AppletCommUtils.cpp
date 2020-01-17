/*********************************************************************************
 FileName: AppletCommonUtils.cpp
 Author: xufubo
 Date:  2018-12-26
 Description:
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/
#include "AppletCommUtils.h"

using namespace std;
using namespace tars;
using namespace HardwareApplet;
using namespace base_utils;

int AppletCommUtils::Value2Document(const rapidjson::Value& value, rapidjson::Document &document)
{
    document.CopyFrom(value, document.GetAllocator());
    return 0;
}

string AppletCommUtils::Value2Str(const rapidjson::Value& value)
{
    if (!value.IsObject())
    {
        return "is not object";
    }
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    value.Accept(writer);
    return buffer.GetString();
}

string AppletCommUtils::Document2Str(const rapidjson::Document &document)
{
    if (!document.IsObject())
    {
        return "is not object";
    }
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);
    return buffer.GetString();
}

//=======================================结构体和Json转换的函数 start=====================================//
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void AppletCommUtils::GoodsSKUInfo2Json(rapidjson::Document& document, const HardwareApplet::GoodsSKUInfo& sinfo, rapidjson::Value& info)
{
    info.AddMember("spuId", sinfo.spuId, document.GetAllocator());
    info.AddMember("skuId", sinfo.skuId, document.GetAllocator());
    info.AddMember("price", sinfo.price, document.GetAllocator());
    info.AddMember("minCount", sinfo.minCount, document.GetAllocator());
    info.AddMember("isDefault", sinfo.isDefault, document.GetAllocator());
    info.AddMember("name", rapidjson::Value(sinfo.name.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
    info.AddMember("imgUrl", rapidjson::Value(sinfo.imgUrl.c_str(), document.GetAllocator()).Move(), document.GetAllocator());

    rapidjson::Value attrList(rapidjson::kArrayType);
    for (size_t j = 0; j < sinfo.attrList.size(); ++j)
    {
        rapidjson::Value attr(rapidjson::kObjectType);
        GoodsSKUAttrInfo2Json(document, sinfo.attrList[j], attr);
        attrList.PushBack(attr, document.GetAllocator());
    }
    info.AddMember("attrList", attrList, document.GetAllocator());

    rapidjson::Value stockList(rapidjson::kArrayType);
    for (size_t j = 0; j < sinfo.stockList.size(); ++j)
    {
        rapidjson::Value stock(rapidjson::kObjectType);
        SKUStockItem2Json(document, sinfo.stockList[j], stock);
        stockList.PushBack(stock, document.GetAllocator());
    }
    info.AddMember("stockList", stockList, document.GetAllocator());
}

void AppletCommUtils::Json2GoodsSKUInfo(const rapidjson::Value& info, HardwareApplet::GoodsSKUInfo& sinfo)
{
    sinfo.skuId = RapidJsonUtil::GetJsonInt(info, "skuId");
    sinfo.spuId = RapidJsonUtil::GetJsonInt(info, "spuId");
    sinfo.price = RapidJsonUtil::GetJsonInt(info, "price");
    sinfo.minCount = RapidJsonUtil::GetJsonInt(info, "minCount");
    sinfo.isDefault = RapidJsonUtil::GetJsonInt(info, "isDefault");
    sinfo.name = RapidJsonUtil::GetJsonString(info, "name");
    sinfo.imgUrl = RapidJsonUtil::GetJsonString(info, "imgUrl");

    if (info.FindMember("attrList") != info.MemberEnd() && info["attrList"].IsArray())
    {
        const rapidjson::Value& attrList = info["attrList"];
        for (rapidjson::Value::ConstValueIterator itr = attrList.Begin(); itr != attrList.End(); ++itr)
        {
            HardwareApplet::GoodsSKUAttrInfo attrInfo;
            Json2GoodsSKUAttrInfo(*itr, attrInfo);
            sinfo.attrList.push_back(attrInfo);
        }
    }

    if (info.FindMember("stockList") != info.MemberEnd() && info["stockList"].IsArray())
    {
        const rapidjson::Value& stockList = info["stockList"];
        for (rapidjson::Value::ConstValueIterator itr = stockList.Begin(); itr != stockList.End(); ++itr)
        {
            HardwareApplet::SKUStockItem stockInfo;
            Json2SKUStockItem(*itr, stockInfo);
            sinfo.stockList.push_back(stockInfo);
        }
    }
}

//////////////////////////////////////////////////////////////////////////
void AppletCommUtils::GoodsSKUAttrInfo2Json(rapidjson::Document& document, const HardwareApplet::GoodsSKUAttrInfo &sattr, rapidjson::Value& attr)
{
    attr.AddMember("attrId", sattr.attrId, document.GetAllocator());
    attr.AddMember("attrValueId", sattr.attrValueId, document.GetAllocator());
    attr.AddMember("attrName", rapidjson::Value(sattr.attrName.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
    attr.AddMember("attrValueName", rapidjson::Value(sattr.attrValueName.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
}

void AppletCommUtils::Json2GoodsSKUAttrInfo(const rapidjson::Value& attr, HardwareApplet::GoodsSKUAttrInfo &sattr)
{
    sattr.attrId = RapidJsonUtil::GetJsonInt(attr, "attrId");
    sattr.attrValueId = RapidJsonUtil::GetJsonInt(attr, "attrValueId");
    sattr.attrName = RapidJsonUtil::GetJsonString(attr, "attrName");
    sattr.attrValueName = RapidJsonUtil::GetJsonString(attr, "attrValueName");
}

//////////////////////////////////////////////////////////////////////////
void AppletCommUtils::SKUStockItem2Json(rapidjson::Document& document, const HardwareApplet::SKUStockItem &sitem, rapidjson::Value& item)
{
    item.AddMember("id", sitem.id, document.GetAllocator());
    item.AddMember("stock", sitem.stock, document.GetAllocator());
    item.AddMember("warningStock", sitem.warningStock, document.GetAllocator());
    item.AddMember("skuId", sitem.skuId, document.GetAllocator());
    item.AddMember("warehouseId", sitem.warehouseId, document.GetAllocator());
    item.AddMember("warehouseName", rapidjson::Value(sitem.warehouseName.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
}

void AppletCommUtils::Json2SKUStockItem(const rapidjson::Value& item, HardwareApplet::SKUStockItem &sitem)
{
    sitem.id = RapidJsonUtil::GetJsonInt(item, "id");
    sitem.stock = RapidJsonUtil::GetJsonInt(item, "stock");
    sitem.warningStock = RapidJsonUtil::GetJsonInt(item, "warningStock");
    sitem.skuId = RapidJsonUtil::GetJsonInt(item, "skuId");
    sitem.warehouseId = RapidJsonUtil::GetJsonInt(item, "warehouseId");
    sitem.warehouseName = RapidJsonUtil::GetJsonString(item, "warehouseName");
}

//=======================================结构体和Json转换的函数 end=====================================//
