/*********************************************************************************
 FileName: AppletCommonUtils.h
 Author: xufubo
 Date:  2018-12-26
 Description:
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/
#include "Global.h"

#ifndef _AppletCommUtils_H_
#define _AppletCommUtils_H_
using namespace std;


class AppletCommUtils 
{
public:
    AppletCommUtils(){}
    ~AppletCommUtils(){}

    static int Value2Document(const rapidjson::Value& value, rapidjson::Document &document);
    static string Value2Str(const rapidjson::Value& value);
    static string Document2Str(const rapidjson::Document &document);

    static void ShopCartItem2Json(rapidjson::Document& document, const HardwareApplet::ShopCartItem& sitem, rapidjson::Value& item);
    static void Json2ShopCartItem(const rapidjson::Value& item, HardwareApplet::ShopCartItem& sitem);

    static void OrderItem2Json(rapidjson::Document& document, const HardwareApplet::OrderItem& sitem, rapidjson::Value& item);
    static void Json2OrderItem(const rapidjson::Value& item, HardwareApplet::OrderItem& sitem);

    static void GoodsSKUInfo2Json(rapidjson::Document& document, const HardwareApplet::GoodsSKUInfo& sinfo, rapidjson::Value& info);
    static void Json2GoodsSKUInfo(const rapidjson::Value& info, HardwareApplet::GoodsSKUInfo& sinfo);

    static void GoodsSKUAttrInfo2Json(rapidjson::Document& document, const HardwareApplet::GoodsSKUAttrInfo &sattr, rapidjson::Value& attr);
    static void Json2GoodsSKUAttrInfo(const rapidjson::Value& attr, HardwareApplet::GoodsSKUAttrInfo &sattr);

    static void AddressInfo2Json(rapidjson::Document& document, const HardwareApplet::AddressInfo &sinfo, rapidjson::Value& info);
    static void Json2AddressInfo(const rapidjson::Value& info, HardwareApplet::AddressInfo &sinfo);

    static void GoodsSPUSimpleInfo2Json(rapidjson::Document& document, const HardwareApplet::GoodsSPUSimpleInfo &sinfo, rapidjson::Value& info);
    static void Json2GoodsSPUSimpleInfo(const rapidjson::Value& info, HardwareApplet::GoodsSPUSimpleInfo &sinfo);

    static void MakerItem2Json(rapidjson::Document& document, const HardwareApplet::MakerItem &sitem, rapidjson::Value& item);
    static void Json2MakerItem(const rapidjson::Value& item, HardwareApplet::MakerItem &sitem);

    static void BrandItem2Json(rapidjson::Document& document, const HardwareApplet::BrandItem &sitem, rapidjson::Value& item);
    static void Json2BrandItem(const rapidjson::Value& item, HardwareApplet::BrandItem &sitem);

    static void AttributeValueItem2Json(rapidjson::Document& document, const HardwareApplet::AttributeValueItem &sitem, rapidjson::Value& item);
    static void Json2AttributeValueItem(const rapidjson::Value& item, HardwareApplet::AttributeValueItem &sitem);

    static void AttributeItem2Json(rapidjson::Document& document, const HardwareApplet::AttributeItem &sitem, rapidjson::Value& item);
    static void Json2AttributeItem(const rapidjson::Value& item, HardwareApplet::AttributeItem &sitem);

    static void CategoryItem2Json(rapidjson::Document& document, const HardwareApplet::CategoryItem &sitem, rapidjson::Value& item);
    static void Json2CategoryItem(const rapidjson::Value& item, HardwareApplet::CategoryItem &sitem);
};


#endif