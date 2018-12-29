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


