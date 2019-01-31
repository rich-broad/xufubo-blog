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
void AppletCommUtils::ShopCartItem2Json(rapidjson::Document& document, const HardwareApplet::ShopCartItem& sitem, rapidjson::Value& item)
{
    item.AddMember("cartId", sitem.cartId, document.GetAllocator());
    item.AddMember("spuId", sitem.spuId, document.GetAllocator());
    item.AddMember("skuId", sitem.skuId, document.GetAllocator());
    item.AddMember("uid", sitem.uid, document.GetAllocator());
    item.AddMember("price", sitem.price, document.GetAllocator());
    item.AddMember("tranPrice", sitem.tranPrice, document.GetAllocator());
    item.AddMember("num", sitem.num, document.GetAllocator());
    item.AddMember("status", sitem.status, document.GetAllocator());
    item.AddMember("billNo", rapidjson::Value(sitem.billNo.c_str(), document.GetAllocator()).Move(), document.GetAllocator());

    rapidjson::Value skuInfo(rapidjson::kObjectType);
    GoodsSKUInfo2Json(document, sitem.skuInfo, skuInfo);
    item.AddMember("skuInfo", skuInfo, document.GetAllocator());
}

void AppletCommUtils::Json2ShopCartItem(const rapidjson::Value& item, HardwareApplet::ShopCartItem& sitem)
{
    sitem.cartId = RapidJsonUtil::GetJsonInt(item, "cartId");
    sitem.spuId = RapidJsonUtil::GetJsonInt(item, "spuId");
    sitem.skuId = RapidJsonUtil::GetJsonInt(item, "skuId");
    sitem.uid = RapidJsonUtil::GetJsonInt(item, "uid");
    sitem.price = RapidJsonUtil::GetJsonInt(item, "price");
    sitem.tranPrice = RapidJsonUtil::GetJsonInt(item, "tranPrice");
    sitem.num = RapidJsonUtil::GetJsonInt(item, "num");
    sitem.status = RapidJsonUtil::GetJsonInt(item, "status");
    sitem.billNo = RapidJsonUtil::GetJsonString(item, "billNo");
    if (item.FindMember("skuInfo") != item.MemberEnd())
    {
        Json2GoodsSKUInfo(item["skuInfo"], sitem.skuInfo);
    }
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void AppletCommUtils::OrderItem2Json(rapidjson::Document& document, const HardwareApplet::OrderItem& sitem, rapidjson::Value& item)
{
    item.AddMember("orderId", sitem.orderId, document.GetAllocator());
    item.AddMember("uid", sitem.uid, document.GetAllocator());
    item.AddMember("payWay", sitem.payWay, document.GetAllocator());
    item.AddMember("status", sitem.status, document.GetAllocator());
    item.AddMember("money", sitem.money, document.GetAllocator());
    item.AddMember("tranMoney", sitem.tranMoney, document.GetAllocator());
    item.AddMember("freight", sitem.freight, document.GetAllocator());
    item.AddMember("billNo", rapidjson::Value(sitem.billNo.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
    rapidjson::Value addressInfo(rapidjson::kObjectType);
    AddressInfo2Json(document, sitem.addressInfo, addressInfo);
    item.AddMember("addressInfo", addressInfo, document.GetAllocator());

    rapidjson::Value itemList(rapidjson::kArrayType);
    for (size_t i = 0; i < sitem.itemList.size(); ++i)
    {
        rapidjson::Value cartItem(rapidjson::kObjectType);
        ShopCartItem2Json(document, sitem.itemList[i], cartItem);
        itemList.PushBack(cartItem, document.GetAllocator());
    }
    item.AddMember("itemList", itemList, document.GetAllocator());
}

void AppletCommUtils::Json2OrderItem(const rapidjson::Value& item, HardwareApplet::OrderItem& sitem)
{
    sitem.orderId = RapidJsonUtil::GetJsonInt(item, "orderId");
    sitem.uid = RapidJsonUtil::GetJsonInt(item, "uid");
    sitem.payWay = RapidJsonUtil::GetJsonInt(item, "payWay");
    sitem.money = RapidJsonUtil::GetJsonInt(item, "money");
    sitem.tranMoney = RapidJsonUtil::GetJsonInt(item, "tranMoney");
    sitem.freight = RapidJsonUtil::GetJsonInt(item, "freight");
    sitem.billNo = RapidJsonUtil::GetJsonString(item, "billNo");
    if (item.FindMember("addressInfo") != item.MemberEnd())
    {
        Json2AddressInfo(item["addressInfo"], sitem.addressInfo);
    }

    if (item.FindMember("itemList") != item.MemberEnd() && item["itemList"].IsArray())
    {
        const rapidjson::Value& itemList = item["itemList"];
        for (rapidjson::Value::ConstValueIterator itr = itemList.Begin(); itr != itemList.End(); ++itr)
        {
            HardwareApplet::ShopCartItem cartItem;
            Json2ShopCartItem(*itr, cartItem);
            sitem.itemList.push_back(cartItem);
        }
    }
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void AppletCommUtils::GoodsSKUInfo2Json(rapidjson::Document& document, const HardwareApplet::GoodsSKUInfo& sinfo, rapidjson::Value& info)
{
    info.AddMember("spuId", sinfo.spuId, document.GetAllocator());
    info.AddMember("skuId", sinfo.skuId, document.GetAllocator());
//     info.AddMember("stock", sinfo.stock, document.GetAllocator());
//     info.AddMember("warnStock", sinfo.warnStock, document.GetAllocator());
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
//     sinfo.stock = RapidJsonUtil::GetJsonInt(info, "stock");
//     sinfo.warnStock = RapidJsonUtil::GetJsonInt(info, "warnStock");
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
//////////////////////////////////////////////////////////////////////////
void AppletCommUtils::AddressInfo2Json(rapidjson::Document& document, const HardwareApplet::AddressInfo &sinfo, rapidjson::Value& info)
{
    info.AddMember("addressId", sinfo.addressId, document.GetAllocator());
    info.AddMember("uid", sinfo.uid, document.GetAllocator());
    info.AddMember("phoneNum", rapidjson::Value(sinfo.phoneNum.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
    info.AddMember("province", rapidjson::Value(sinfo.province.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
    info.AddMember("city", rapidjson::Value(sinfo.city.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
    info.AddMember("county", rapidjson::Value(sinfo.county.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
    info.AddMember("addressDetail", rapidjson::Value(sinfo.addressDetail.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
    info.AddMember("receiver", rapidjson::Value(sinfo.receiver.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
}

void AppletCommUtils::Json2AddressInfo(const rapidjson::Value& info, HardwareApplet::AddressInfo &addressInfo)
{
    addressInfo.addressId = RapidJsonUtil::GetJsonInt(info, "addressId");
    addressInfo.uid = RapidJsonUtil::GetJsonInt(info, "uid");
    addressInfo.phoneNum = RapidJsonUtil::GetJsonString(info, "phoneNum");
    addressInfo.province = RapidJsonUtil::GetJsonString(info, "province");
    addressInfo.city = RapidJsonUtil::GetJsonString(info, "city");
    addressInfo.county = RapidJsonUtil::GetJsonString(info, "county");
    addressInfo.addressDetail = RapidJsonUtil::GetJsonString(info, "addressDetail");
    addressInfo.receiver = RapidJsonUtil::GetJsonString(info, "receiver");
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void AppletCommUtils::GoodsSPUSimpleInfo2Json(rapidjson::Document& document, const HardwareApplet::GoodsSPUSimpleInfo &sinfo, rapidjson::Value& info)
{
    info.AddMember("spuId", sinfo.spuId, document.GetAllocator());
    info.AddMember("status", sinfo.status, document.GetAllocator());
    info.AddMember("name", rapidjson::Value(sinfo.name.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
    info.AddMember("brandName", rapidjson::Value(sinfo.brandName.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
    info.AddMember("maker", rapidjson::Value(sinfo.maker.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
    info.AddMember("sDesc", rapidjson::Value(sinfo.sDesc.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
}

void AppletCommUtils::Json2GoodsSPUSimpleInfo(const rapidjson::Value& info, HardwareApplet::GoodsSPUSimpleInfo &sinfo)
{
    sinfo.spuId = RapidJsonUtil::GetJsonInt(info, "spuId");
    sinfo.name = RapidJsonUtil::GetJsonString(info, "name");
    sinfo.brandName = RapidJsonUtil::GetJsonString(info, "brandName");
    sinfo.maker = RapidJsonUtil::GetJsonString(info, "maker");
    sinfo.sDesc = RapidJsonUtil::GetJsonString(info, "sDesc");
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void AppletCommUtils::MakerItem2Json(rapidjson::Document& document, const HardwareApplet::MakerItem &sitem, rapidjson::Value& item)
{
    item.AddMember("makerId", sitem.makerId, document.GetAllocator());
    item.AddMember("makerName", rapidjson::Value(sitem.makerName.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
    item.AddMember("makerDesc", rapidjson::Value(sitem.makerDesc.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
}

void AppletCommUtils::Json2MakerItem(const rapidjson::Value& item, HardwareApplet::MakerItem &sitem)
{
    sitem.makerId = RapidJsonUtil::GetJsonInt(item, "makerId");
    sitem.makerName = RapidJsonUtil::GetJsonString(item, "makerName");
    sitem.makerDesc = RapidJsonUtil::GetJsonString(item, "makerDesc");
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void AppletCommUtils::WarehouseItem2Json(rapidjson::Document& document, const HardwareApplet::WarehouseItem &sitem, rapidjson::Value& item)
{
    item.AddMember("warehouseId", sitem.warehouseId, document.GetAllocator());
    item.AddMember("warehouseName", rapidjson::Value(sitem.warehouseName.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
    item.AddMember("warehouseDesc", rapidjson::Value(sitem.warehouseDesc.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
}

void AppletCommUtils::Json2WarehouseItem(const rapidjson::Value& item, HardwareApplet::WarehouseItem &sitem)
{
    sitem.warehouseId = RapidJsonUtil::GetJsonInt(item, "warehouseId");
    sitem.warehouseName = RapidJsonUtil::GetJsonString(item, "warehouseName");
    sitem.warehouseDesc = RapidJsonUtil::GetJsonString(item, "warehouseDesc");
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void AppletCommUtils::BrandItem2Json(rapidjson::Document& document, const HardwareApplet::BrandItem &sitem, rapidjson::Value& item)
{
    item.AddMember("brandId", sitem.brandId, document.GetAllocator());
    item.AddMember("brandName", rapidjson::Value(sitem.brandName.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
    item.AddMember("brandDesc", rapidjson::Value(sitem.brandDesc.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
}

void AppletCommUtils::Json2BrandItem(const rapidjson::Value& item, HardwareApplet::BrandItem &sitem)
{
    sitem.brandId = RapidJsonUtil::GetJsonInt(item, "brandId");
    sitem.brandName = RapidJsonUtil::GetJsonString(item, "brandName");
    sitem.brandDesc = RapidJsonUtil::GetJsonString(item, "brandDesc");
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void AppletCommUtils::AttributeValueItem2Json(rapidjson::Document& document, const HardwareApplet::AttributeValueItem &sitem, rapidjson::Value& item)
{
    item.AddMember("attrValueId", sitem.attrValueId, document.GetAllocator());
    item.AddMember("attrId", sitem.attrId, document.GetAllocator());
    item.AddMember("name", rapidjson::Value(sitem.name.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
}

void AppletCommUtils::Json2AttributeValueItem(const rapidjson::Value& item, HardwareApplet::AttributeValueItem &sitem)
{
    sitem.attrValueId = RapidJsonUtil::GetJsonInt(item, "attrValueId");
    sitem.attrId = RapidJsonUtil::GetJsonInt(item, "attrId");
    sitem.name = RapidJsonUtil::GetJsonString(item, "name");
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void AppletCommUtils::AttributeItem2Json(rapidjson::Document& document, const HardwareApplet::AttributeItem &sitem, rapidjson::Value& item)
{
    item.AddMember("attrId", sitem.attrId, document.GetAllocator());
    item.AddMember("attrName", rapidjson::Value(sitem.attrName.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
    item.AddMember("attrDesc", rapidjson::Value(sitem.attrDesc.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
}

void AppletCommUtils::Json2AttributeItem(const rapidjson::Value& item, HardwareApplet::AttributeItem &sitem)
{
    sitem.attrId = RapidJsonUtil::GetJsonInt(item, "attrId");
    sitem.attrName = RapidJsonUtil::GetJsonString(item, "attrName");
    sitem.attrDesc = RapidJsonUtil::GetJsonString(item, "attrDesc");
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void AppletCommUtils::CategoryItem2Json(rapidjson::Document& document, const HardwareApplet::CategoryItem &sitem, rapidjson::Value& item)
{
    item.AddMember("categoryId", sitem.categoryId, document.GetAllocator());
    item.AddMember("parentId", sitem.parentId, document.GetAllocator());
    item.AddMember("level", sitem.level, document.GetAllocator());
    item.AddMember("categoryName", rapidjson::Value(sitem.categoryName.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
    item.AddMember("imgUrl", rapidjson::Value(sitem.imgUrl.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
    item.AddMember("desc", rapidjson::Value(sitem.desc.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
}

void AppletCommUtils::Json2CategoryItem(const rapidjson::Value& item, HardwareApplet::CategoryItem &sitem)
{
    sitem.categoryId = RapidJsonUtil::GetJsonInt(item, "categoryId");
    sitem.parentId = RapidJsonUtil::GetJsonInt(item, "parentId");
    sitem.level = RapidJsonUtil::GetJsonInt(item, "level");
    sitem.categoryName = RapidJsonUtil::GetJsonString(item, "categoryName");
    sitem.imgUrl = RapidJsonUtil::GetJsonString(item, "imgUrl");
    sitem.desc = RapidJsonUtil::GetJsonString(item, "desc");
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void AppletCommUtils::GoodsSPUDetail2Json(rapidjson::Document& document, const HardwareApplet::GoodsSPUDetail &sitem, rapidjson::Value& item)
{

}
void AppletCommUtils::Json2GoodsSPUDetail(const rapidjson::Value& detail, HardwareApplet::GoodsSPUDetail &sdetail)
{
    sdetail.name = RapidJsonUtil::GetJsonString(detail, "name");
    sdetail.brandName = RapidJsonUtil::GetJsonString(detail, "brandName");
    sdetail.makerName = RapidJsonUtil::GetJsonString(detail, "makerName");
    sdetail.sDesc = RapidJsonUtil::GetJsonString(detail, "sDesc");
    sdetail.brandId = RapidJsonUtil::GetJsonInt(detail, "brandId");
    sdetail.desc = RapidJsonUtil::GetJsonString(detail, "desc");
    sdetail.makerId = RapidJsonUtil::GetJsonInt(detail, "makerId");
    sdetail.model = RapidJsonUtil::GetJsonString(detail, "model");

    if (detail.FindMember("mediaInfo") != detail.MemberEnd() && detail["categoryInfo"].IsArray())
    {
        const rapidjson::Value& mediaInfo = detail["mediaInfo"];
        for (rapidjson::Value::ConstValueIterator itr = mediaInfo.Begin(); itr != mediaInfo.End(); ++itr)
        {
            HardwareApplet::GoodsMediaInfo item;
            Json2GoodsMediaInfo(*itr, item);
            sdetail.mediaInfo.push_back(item);
        }
    }
    if (detail.FindMember("categoryInfo") != detail.MemberEnd() && detail["categoryInfo"].IsArray())
    {
        const rapidjson::Value& categoryInfo = detail["categoryInfo"];
        for (rapidjson::Value::ConstValueIterator itr = categoryInfo.Begin(); itr != categoryInfo.End(); ++itr)
        {
            HardwareApplet::CategoryItem item;
            Json2CategoryItem(*itr, item);
            sdetail.categoryInfo.push_back(item);
        }
    }
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void AppletCommUtils::GoodsMediaInfo2Json(rapidjson::Document& document, const HardwareApplet::GoodsMediaInfo &sitem, rapidjson::Value& item)
{
    item.AddMember("mediaId", sitem.mediaId, document.GetAllocator());
    item.AddMember("type", sitem.type, document.GetAllocator());
    item.AddMember("position", sitem.position, document.GetAllocator());
    item.AddMember("mediaUrl", rapidjson::Value(sitem.mediaUrl.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
}
void AppletCommUtils::Json2GoodsMediaInfo(const rapidjson::Value& item, HardwareApplet::GoodsMediaInfo &sitem)
{
    sitem.mediaId = RapidJsonUtil::GetJsonInt(item, "mediaId");
    sitem.type = RapidJsonUtil::GetJsonInt(item, "type");
    sitem.position = RapidJsonUtil::GetJsonInt(item, "position");
    sitem.mediaUrl = RapidJsonUtil::GetJsonString(item, "mediaUrl");
}

//////////////////////////////////////////////////////////////////////////
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
