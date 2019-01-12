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

void AppletCommUtils::ShopCartItem2Json(rapidjson::Document& document, const HardwareApplet::ShopCartItem& sitem, rapidjson::Value& item)
{
    item.AddMember("cartId", sitem.cartId, document.GetAllocator());
    item.AddMember("spuId", sitem.spuId, document.GetAllocator());
    item.AddMember("skuId", sitem.skuId, document.GetAllocator());
    item.AddMember("uid", sitem.uid, document.GetAllocator());
    item.AddMember("price", sitem.price, document.GetAllocator());
    item.AddMember("tran_price", sitem.tran_price, document.GetAllocator());
    item.AddMember("num", sitem.num, document.GetAllocator());
    item.AddMember("status", sitem.status, document.GetAllocator());

    rapidjson::Value skuInfo(rapidjson::kObjectType);
    GoodsSKUInfo2Json(document, sitem.skuInfo, skuInfo);
    item.AddMember("skuInfo", skuInfo, document.GetAllocator());
}

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

void AppletCommUtils::GoodsSKUInfo2Json(rapidjson::Document& document, const HardwareApplet::GoodsSKUInfo& sinfo, rapidjson::Value& info)
{
    info.AddMember("spuId", sinfo.spuId, document.GetAllocator());
    info.AddMember("skuId", sinfo.skuId, document.GetAllocator());
    info.AddMember("stock", sinfo.stock, document.GetAllocator());
    info.AddMember("warnStock", sinfo.warnStock, document.GetAllocator());
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
}

void AppletCommUtils::GoodsSKUAttrInfo2Json(rapidjson::Document& document, const HardwareApplet::GoodsSKUAttrInfo &sattr, rapidjson::Value& attr)
{
    attr.AddMember("attrId", sattr.attrId, document.GetAllocator());
    attr.AddMember("attrValueId", sattr.attrValueId, document.GetAllocator());
    attr.AddMember("attrName", rapidjson::Value(sattr.attrName.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
    attr.AddMember("attrValueName", rapidjson::Value(sattr.attrValueName.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
}

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

void AppletCommUtils::GoodsSPUSimpleInfo2Json(rapidjson::Document& document, const HardwareApplet::GoodsSPUSimpleInfo &sinfo, rapidjson::Value& info)
{
    info.AddMember("spuId", sinfo.spuId, document.GetAllocator());
    info.AddMember("status", sinfo.status, document.GetAllocator());
    info.AddMember("name", rapidjson::Value(sinfo.name.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
    info.AddMember("brandName", rapidjson::Value(sinfo.brandName.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
    info.AddMember("maker", rapidjson::Value(sinfo.maker.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
    info.AddMember("sDesc", rapidjson::Value(sinfo.sDesc.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
}

void AppletCommUtils::MakerItem2Json(rapidjson::Document& document, const HardwareApplet::MakerItem &sitem, rapidjson::Value& item)
{
    item.AddMember("makerId", sitem.makerId, document.GetAllocator());
    item.AddMember("makerName", rapidjson::Value(sitem.makerName.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
    item.AddMember("makerDesc", rapidjson::Value(sitem.makerDesc.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
}


void AppletCommUtils::BrandItem2Json(rapidjson::Document& document, const HardwareApplet::BrandItem &sitem, rapidjson::Value& item)
{
    item.AddMember("brandId", sitem.brandId, document.GetAllocator());
    item.AddMember("brandName", rapidjson::Value(sitem.brandName.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
    item.AddMember("brandDesc", rapidjson::Value(sitem.brandDesc.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
}

void AppletCommUtils::AttributeValueItem2Json(rapidjson::Document& document, const HardwareApplet::AttributeValueItem &sitem, rapidjson::Value& item)
{
    item.AddMember("attrValueId", sitem.attrValueId, document.GetAllocator());
    item.AddMember("attrId", sitem.attrId, document.GetAllocator());
    item.AddMember("name", rapidjson::Value(sitem.name.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
}

void AppletCommUtils::AttributeItem2Json(rapidjson::Document& document, const HardwareApplet::AttributeItem &sitem, rapidjson::Value& item)
{
    item.AddMember("attrId", sitem.attrId, document.GetAllocator());
    item.AddMember("attrName", rapidjson::Value(sitem.attrName.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
    item.AddMember("attrDesc", rapidjson::Value(sitem.attrDesc.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
}

void AppletCommUtils::CategoryItem2Json(rapidjson::Document& document, const HardwareApplet::CategoryItem &sitem, rapidjson::Value& item)
{
    item.AddMember("categoryId", sitem.categoryId, document.GetAllocator());
    item.AddMember("parentId", sitem.parentId, document.GetAllocator());
    item.AddMember("level", sitem.level, document.GetAllocator());
    item.AddMember("categoryName", rapidjson::Value(sitem.categoryName.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
    item.AddMember("imgUrl", rapidjson::Value(sitem.imgUrl.c_str(), document.GetAllocator()).Move(), document.GetAllocator());
}