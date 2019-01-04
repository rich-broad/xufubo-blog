/*********************************************************************************
 FileName: 
 Author: xufubo
 Date:  2018-12-26
 Description: 常用字符串操作
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/

#pragma once

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <stdint.h>
#include <stdarg.h>


namespace base_utils
{

using namespace std;
#define BIN_UNI_KEY_LEN 21

class CStringUtils
{
public:
    static std::string I64TOS(uin_64 key);
    static int Hex2Deci(const std::string& hex, uint16_t& deci);
    static int Hex2Deci(const std::string& hex, uint32_t& deci, bool ignore_0x=true);
    static int GetUniKey(std::string& uni_key);
    static int VersionCompare(const std::string& ver1, const std::string& ver2);
    // 把一组迭代器指向的对象组合起来，并且以指定的分隔符分隔
    template<typename Iter, typename Func>
    static std::string Join(Iter begin, Iter end, Func iter_func, const std::string& sep)
    {
        std::string ret;
        for (Iter curr = begin; curr != end; ++curr)
        {
            if (curr != begin)
            {
                ret += sep;
            }
            ret += iter_func(*curr);
        }
        return ret;
    }
};

}

