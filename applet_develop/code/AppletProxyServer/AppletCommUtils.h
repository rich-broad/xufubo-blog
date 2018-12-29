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
};


#endif