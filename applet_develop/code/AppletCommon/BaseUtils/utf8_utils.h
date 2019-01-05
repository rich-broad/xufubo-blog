/*********************************************************************************
 FileName: 
 Author: xufubo
 Date:  2018-12-26
 Description: 随机数
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/

#ifndef __UTF8_UTILS_H
#define __UTF8_UTILS_H

#include <string>

namespace base_utils
{

class CUTF8_Utils
{
public:
    static int CheckAlphaDigitZh(const std::string& str);
    static int CheckDigit(const std::string& str);
    static int CountCharacter(const std::string& str);
    static std::string Truncate(const std::string& str, unsigned num);

    // 使用hydra中的库来判断是否utf8字符. 支持各种特殊utf8字符
    static size_t CountCharacter2(const std::string &str);

public:
    static bool IsAlpha(const char*& input);
    static bool IsDigit(const char*& input);
    static bool IsZh(const char*& input);
};

};

#endif