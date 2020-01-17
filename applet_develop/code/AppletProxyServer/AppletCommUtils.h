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

    static void GoodsSKUInfo2Json(rapidjson::Document& document, const HardwareApplet::GoodsSKUInfo& sinfo, rapidjson::Value& info);
    static void Json2GoodsSKUInfo(const rapidjson::Value& info, HardwareApplet::GoodsSKUInfo& sinfo);

    static void GoodsSKUAttrInfo2Json(rapidjson::Document& document, const HardwareApplet::GoodsSKUAttrInfo &sattr, rapidjson::Value& attr);
    static void Json2GoodsSKUAttrInfo(const rapidjson::Value& attr, HardwareApplet::GoodsSKUAttrInfo &sattr);

    static void SKUStockItem2Json(rapidjson::Document& document, const HardwareApplet::SKUStockItem &sitem, rapidjson::Value& item);
    static void Json2SKUStockItem(const rapidjson::Value& item, HardwareApplet::SKUStockItem &sitem);

};


#endif