/*********************************************************************************
 FileName: 
 Author: xufubo
 Date:  2018-12-26
 Description: rapidjson的封装
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/

#ifndef __RAPIDJSON_UTIL_H_
#define __RAPIDJSON_UTIL_H_

#include <string>
#include <vector>
#include <set>
#include <utility>
#include <map>
#include <algorithm>
#include <cstdio>
#include <errno.h>
#include <stdint.h>

#include "util/tc_common.h"

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/reader.h"
#include "rapidjson/error/en.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace std;

namespace base_utils
{
// rapidjson的封装，成功返回true，出错返回false
class RapidJsonUtil
{
public:
    static bool ParseJsonFromStr(const string& json_str, rapidjson::Document& doc, int &err)
    {
        doc.Parse(json_str.c_str());
        if (doc.HasParseError())
        {
            cout << "parse json_str failed, req_data = " <<  json_str << "|errno = " << doc.GetParseError() << "|errmsg = "
                 << rapidjson::GetParseError_En(doc.GetParseError()) << endl;
            err = doc.GetParseError();
            return false;
        }
        return true;
    }

    static bool ParseJsonFromFile(const string& file_name, rapidjson::Document& doc, int &err)
    {
        FILE* ptr_file = fopen(file_name.c_str(), "rb");

        if (ptr_file == NULL)
        {
            cout << "Cannot open " << file_name << "|errno = " << errno << endl;
            err = errno;
            return false;
        }

        char buf[65536];
        rapidjson::FileReadStream is(ptr_file, buf, sizeof(buf));

        doc.ParseStream(is);
        fclose(ptr_file);

        if (doc.HasParseError())
        {
            cout << "parse json_str failed, req_data = " <<  file_name << "|errno = " << doc.GetParseError() << "|errmsg = "
                << rapidjson::GetParseError_En(doc.GetParseError()) << endl;
            err = doc.GetParseError();
            return false;
        }
        return true;
    }

    static bool WriteJsonToStr(const rapidjson::Value& obj, string& str)
    {
        rapidjson::StringBuffer buffer;
        buffer.Clear();
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

        bool ret = obj.Accept(writer);
        str = buffer.GetString();

        return ret;
    }

    //---------一组GetValue函数，把obj当作某种类型的值取出来---------

    static bool GetValue(const rapidjson::Value& obj, const rapidjson::Value* &value)
    {
        value = &obj;
        return true;
    }

    static bool GetValue(const rapidjson::Value& obj, bool& value)
    {
        if (obj.IsBool())
        {
            value = obj.IsTrue();
            return true;
        }
        return false;
    }

    static bool GetValue(const rapidjson::Value& obj, int32_t& value)
    {
        if (obj.IsInt())
        {
            value = obj.GetInt();
            return true;
        }
        else if (obj.IsString())
        {
            value = tars::TC_Common::strto<int32_t>(std::string(obj.GetString(), obj.GetStringLength()));
            return true;
        }
        return false;
    }

    static bool GetValue(const rapidjson::Value& obj, uint32_t& value)
    {
        if (obj.IsUint())
        {
            value = obj.GetUint();
            return true;
        }
        else if (obj.IsString())
        {
            value = tars::TC_Common::strto<uint32_t>(std::string(obj.GetString(), obj.GetStringLength()));
            return true;
        }
        return false;
    }

    static bool GetValue(const rapidjson::Value& obj, int64_t& value)
    {
        if (obj.IsInt64())
        {
            value = obj.GetInt64();
            return true;
        }
        else if (obj.IsString())
        {
            value = tars::TC_Common::strto<int64_t>(std::string(obj.GetString(), obj.GetStringLength()));
            return true;
        }
        return false;
    }

    static bool GetValue(const rapidjson::Value& obj, uint64_t& value)
    {
        if (obj.IsUint64())
        {
            value = obj.GetUint64();
            return true;
        }
        else if (obj.IsString())
        {
            value = tars::TC_Common::strto<uint64_t>(std::string(obj.GetString(), obj.GetStringLength()));
            return true;
        }
        return false;
    }

    static bool GetValue(const rapidjson::Value& obj, std::string& value)
    {
        if (obj.IsString())
        {
            value = std::string(obj.GetString(), obj.GetStringLength());
            return true;
        }
        else if (obj.IsInt())
        {
            value = tars::TC_Common::tostr(obj.GetInt());
            return true;
        }
        else if (obj.IsUint())
        {
            value = tars::TC_Common::tostr(obj.GetUint());
            return true;
        }
        else if (obj.IsDouble())
        {
            value = tars::TC_Common::tostr(obj.GetDouble());
            return true;
        }
        return false;
    }

    static bool GetValue(const rapidjson::Value& obj, double& value)
    {
        if (obj.IsDouble())
        {
            value = obj.GetDouble();
            return true;
        }
        else if (obj.IsString())
        {
            value = tars::TC_Common::strto<double>(std::string(obj.GetString(), obj.GetStringLength()));
            return true;
        }
        return false;
    }

    template<typename T, typename A>
    static bool GetValue(const rapidjson::Value& obj, std::set<T,A>& value)
    {
        if (!obj.IsArray())
        {
            return false;
        }
        value.clear();
        for (rapidjson::Value::ConstArray::ConstValueIterator it = obj.Begin(); it != obj.End(); ++it)
        {
            T item;
            if (GetValue(*it, item))
            {
                value.insert(item);
            }
        }
        return true;
    }

    template<typename T, typename A>
    static bool GetValue(const rapidjson::Value& obj, std::vector<T,A>& value)
    {
        if (!obj.IsArray())
        {
            return false;
        }
        value.clear();
        for (rapidjson::Value::ConstArray::ConstValueIterator it = obj.Begin(); it != obj.End(); ++it)
        {
            T item;
            if (GetValue(*it, item))
            {
                value.push_back(item);
            }
        }
        return true;
    }

    template<typename K, typename V, typename C, typename A>
    static bool GetValue(const rapidjson::Value& obj, std::map<K,V,C,A>& value)
    {
        if (!obj.IsObject())
        {
            return false;
        }
        value.clear();
        for (rapidjson::Value::ConstMemberIterator it = obj.MemberBegin(); it != obj.MemberEnd(); ++it)
        {
            K k; V v;
            if (GetValue(it->name, k) && GetValue(it->value, v))
            {
                value.insert(std::make_pair(k, v));
            }
        }
        return true;
    }

    //---------一个带key参数的GetValue函数，调用上面的GetValue系列函数，取出obj[key]---------

    template<typename T>
    static bool GetValue(const rapidjson::Value& obj, const std::string& key, T& value)
    {
        if (!obj.IsObject())
        {
            return false;
        }
        rapidjson::Value::ConstMemberIterator it = obj.FindMember(key.c_str());
        if (it == obj.MemberEnd())
        {
            return false;
        }
        return GetValue(it->value, value);
    }

    //---------一组支持配置默认值的带key参数的GetValue函数，取出obj[key]，失败时返回默认值---------

    template<typename T>
    static T GetValueWithDefault(const rapidjson::Value& obj, const std::string& key, const T& def_value)
    {
        T value;
        if (GetValue(obj, key, value))
        {
            return value;
        }
        return def_value;
    }

    static std::string GetJsonString(const rapidjson::Value& obj, const std::string& key, const std::string& def_value = "")
    {
        return GetValueWithDefault(obj, key, def_value);
    }

    static int32_t GetJsonInt(const rapidjson::Value& obj, const std::string& key, int32_t def_value = 0)
    {
        return GetValueWithDefault(obj, key, def_value);
    }

    static uint32_t GetJsonUInt(const rapidjson::Value& obj, const std::string& key, uint32_t def_value = 0)
    {
        return GetValueWithDefault(obj, key, def_value);
    }

    static int64_t GetJsonInt64(const rapidjson::Value& obj, const std::string& key, int64_t def_value = 0)
    {
        return GetValueWithDefault(obj, key, def_value);
    }

    static uint64_t GetJsonUInt64(const rapidjson::Value& obj, const std::string& key, uint64_t def_value = 0)
    {
        return GetValueWithDefault(obj, key, def_value);
    }

    static bool GetJsonBool(const rapidjson::Value& obj, const std::string& key, bool def_value = false)
    {
        return GetValueWithDefault(obj, key, def_value);
    }

    static double GetJsonDouble(const rapidjson::Value& obj, const std::string& key, double def_value = 0.0)
    {
        return GetValueWithDefault(obj, key, def_value);
    }

    static const rapidjson::Value* GetJsonObject(const rapidjson::Value& obj, const std::string& key, const rapidjson::Value* def_value = NULL)
    {
        return GetValueWithDefault<const rapidjson::Value*>(obj, key, def_value);
    }
};

};

#define GET_RAPIDJSON_VALUE(obj, key, value)  base_utils::RapidJsonUtil::GetValue(obj, key, value)

#define GET_RAPIDJSON_VALUE_WITH_DEFAULT(obj, key, value)  base_utils::RapidJsonUtil::GetValueWithDefault(obj, key, value)

#endif // !__RAPIDJSON_UTIL_H_
