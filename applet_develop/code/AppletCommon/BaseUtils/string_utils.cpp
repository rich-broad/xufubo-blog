/*********************************************************************************
 FileName: 
 Author: xufubo
 Date:  2018-12-26
 Description: 常用字符串操作
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/
#include <stdarg.h>
#include "string_utils.h"
#include "comm_errno.h"
#include "sec_base64.h"
#include "time_utils.h"
#include "netaddr_util.h"
#include <string.h>
#include "str_comm.h"


using namespace base_utils;
using namespace std;

namespace base_utils
{

int StringUtils::Hex2Deci(const std::string& hex, uint16_t& deci)
{
    deci = 0;
    size_t size = hex.size();
    static const size_t pre_size = strlen("0x");
    //跳过0x前缀
    if (size <= pre_size)
    {
        return -1;
    }

    size_t index = pre_size;
    for (; index < size; ++index)
    {
        char c = hex[index];
        if ( (c >= '0') && (c <= '9') )
        {
            deci = deci*16 + c - '0';
        }
        else if ( (c >= 'A') && (c <= 'F') )
        {
            deci = deci*16 + c - 'A' + 10;
        }
        else if((c >= 'a') && (c <= 'f'))
        {
            deci = deci*16 + c - 'a' + 10;
        }
        else
        {
            return -2;
        }
    }

    return 0;
}

int StringUtils::Hex2Deci(const std::string& hex, uint32_t& deci, bool ignore_0x)
{
    deci = 0;
    size_t size = hex.size();
    size_t index = 0;

    // 如果以0x开头，跳过0x前缀
    static const size_t pre_size = strlen("0x");
    if (!ignore_0x)
    {
        if (size <= pre_size || !(hex[0] == '0' && hex[1] == 'x'))
        {
            return -1;
        }
        index = pre_size;
    }

    for (; index < size; ++index)
    {
        char c = hex[index];
        printf("c:%c", c);
        if ( (c >= '0') && (c <= '9') )
        {
            deci = deci*16 + c - '0';
        }
        else if ( (c >= 'A') && (c <= 'F') )
        {
            deci = deci*16 + c - 'A' + 10;
        }
        else if((c >= 'a') && (c <= 'f'))
        {
            deci = deci*16 + c - 'a' + 10;
        }
        else
        {
            return -2;
        }
        printf("deci:%u", deci);
    }

    return 0;
}

int StringUtils::GetUniKey(std::string& uni_key)
{
    static bool init = false;
    if (!init)
    {
        srand(getpid());
        init = true;
    }
    int ret = 0;
    unsigned ip = base_utils::GetLocalIP();
    pid_t pid = getpid();
    // 获取微妙
    uint64_t us_ts = base_utils::TimeUtils::GetCurrentuTime();
    //unsigned random = rand() % RAND_MAX;
    //自增计数器
    static unsigned incr = 0;
    incr++;
    if(incr >= INT_MAX)
    {
        incr = 1;
    }

    unsigned index = 0;
    static char bin_code[BIN_UNI_KEY_LEN] = {0};
    memcpy(bin_code + index, &ip, sizeof(ip) );
    index += sizeof(ip);
    memcpy(bin_code + index, &pid, sizeof(pid) );
    index += sizeof(pid);
    memcpy(bin_code + index, &us_ts, sizeof(us_ts) );
    index += sizeof(us_ts);
    memcpy(bin_code + index, &incr, sizeof(incr) );
    index += sizeof(incr);
    //再填充一个字符，凑够21字符
    char a = 'a';
    memcpy(bin_code + index, &a, sizeof(a) );
    index += sizeof(a);

    static char base64_code[UNI_KEY_LEN] = {0};
    unsigned base64_len = sizeof(base64_code);
    ret = base_utils::Base64Encode((uint8_t*)bin_code, BIN_UNI_KEY_LEN, base64_code, &base64_len);
    if (0 != ret)
    {
        printf("Base64Encode error:%d", ret);
        return ECODE_BASE64_ENCODE;
    }

    uni_key.assign(base64_code, base64_len);
    return 0;
}

int StringUtils::VersionCompare(const std::string& ver1, const std::string& ver2)
{
    char *ptr1 = const_cast<char*>(ver1.c_str());
    char *ptr2 = const_cast<char*>(ver2.c_str());

    while (*ptr1 != '\0' || *ptr2 != '\0')
    {
        int val1 = *ptr1 == '\0' ? 0 : strtol(ptr1, &ptr1, 10);
        int val2 = *ptr2 == '\0' ? 0 : strtol(ptr2, &ptr2, 10);

        if (val1 > val2)
        {
            return 1;
        }
        if (val1 < val2)
        {
            return -1;
        }

        if (*ptr1 != '\0')
        {
            ptr1++;
        }
        if (*ptr2 != '\0')
        {
            ptr2++;
        }
    }

    return 0;
}



std::string& StringUtils::StringReplace(std::string& sData, const std::string& sSrc, const std::string& sDst)
{
    std::string::size_type pos = 0;
    std::string::size_type slen = sSrc.size();
    std::string::size_type dlen = sDst.size();
    while ((pos=sData.find(sSrc, pos)) != std::string::npos)
    {
        sData.replace(pos, slen, sDst);
        pos += dlen;
    }

    return sData;
}

std::vector<std::string>& StringUtils::StringSplit(const std::string& sData, const std::string& sDelim, std::vector<std::string>& vItems)
{
    vItems.clear();

    std::string::size_type bpos = 0;
    std::string::size_type epos = 0;
    std::string::size_type nlen = sDelim.size();
    while ((epos=sData.find(sDelim, epos)) != std::string::npos)
    {
        vItems.push_back(sData.substr(bpos, epos-bpos));
        epos += nlen;
        bpos = epos;
    }

    vItems.push_back(sData.substr(bpos, sData.size()-bpos));

    return vItems;
}

std::vector<std::string>& StringUtils::StringSplitTrim(const std::string& sData, const std::string& sDelim, std::vector<std::string>& vItems)
{
    vItems.clear();

    std::string::size_type bpos = 0;
    std::string::size_type epos = 0;
    std::string::size_type nlen = sDelim.size();

    while(sData.substr(epos,nlen) == sDelim)
    {
        epos += nlen;
    }

    bpos = epos;

    while ((epos=sData.find(sDelim, epos)) != std::string::npos)
    {
        vItems.push_back(sData.substr(bpos, epos-bpos));

        epos += nlen;

        while(sData.substr(epos,nlen) == sDelim)
        {
            epos += nlen;
        }

        bpos = epos;
    }

    if(bpos != sData.size())
    {
        vItems.push_back(sData.substr(bpos, sData.size()-bpos));
    }

    return vItems;
}

std::set<std::string>& StringUtils::StringSplitTrim(const std::string& sData, const std::string& sDelim, std::set<std::string>& vItems)
{
    vItems.clear();

    std::string::size_type bpos = 0;
    std::string::size_type epos = 0;
    std::string::size_type nlen = sDelim.size();

    while(sData.substr(epos,nlen) == sDelim)
    {
        epos += nlen;
    }

    bpos = epos;

    while ((epos=sData.find(sDelim, epos)) != std::string::npos)
    {
        vItems.insert(sData.substr(bpos, epos-bpos));

        epos += nlen;

        while(sData.substr(epos,nlen) == sDelim)
        {
            epos += nlen;
        }

        bpos = epos;
    }

    if(bpos != sData.size())
    {
        vItems.insert(sData.substr(bpos, sData.size()-bpos));
    }

    return vItems;
}

std::string StringUtils::StringTrim(std::string& sData, const std::string& sDelim)
{
    // trim right
    sData.erase(sData.find_last_not_of(sDelim)+1);
    // trim left
    return sData.erase(0,sData.find_first_not_of(sDelim));
}

std::string StringUtils::StringJoin(const std::vector<std::string>& vstElem,
                       const std::string& sDelim)
{
    std::string sText;
    std::vector<std::string>::const_iterator bpos = vstElem.begin();
    std::vector<std::string>::const_iterator epos = vstElem.end();
    std::vector<std::string>::const_iterator cpos = bpos;
    while (cpos != epos)
    {
        if (cpos != bpos)
        {
            sText += sDelim;
        }
        sText += *cpos++;
    }
    return sText;
}

std::string StringUtils::StringHumanJoin(const std::vector<std::string>& vstElem,
                            const std::string& lastDelim, const std::string& listDelim)
{
    size_t size = vstElem.size();
    if(size<3)
    {
        return StringJoin(vstElem, lastDelim);
    }
    std::string sText(vstElem.front());
    --size;
    for(size_t i=1; i<size; ++i)
    {
        sText.append(listDelim);
        sText.append(vstElem.at(i));
    }
    sText.append(lastDelim);
    sText.append(vstElem.back());
    return sText;
}

std::string& StringUtils::StringChnCut(std::string& sData, int nSize)
{
    if (nSize >= (int)sData.size())
    {
        return sData;
    }
    sData.erase(CheckChn(sData.data(), nSize), std::string::npos);
    return sData;
}

int StringUtils::CheckChn(const char* pcData, int nSize)
{
    const char* pcEnd = pcData+nSize;
    while (pcData < pcEnd)
    {
        if (*pcData >= 0)
        {
            ++pcData;
        }
        else
        {
            pcData += 2;
        }
    }
    return (pcData==pcEnd) ? (nSize) : (nSize-1);
}

int StringUtils::Wordlen(const std::string& sData)
{
    int nlen = 0;
    const char* bp = sData.data();
    const char* ep = bp+sData.size();
    while (bp < ep)
    {
        nlen++;
        if (*bp>=0)
        {
            ++bp;
        }
        else
        {
            bp += 2;
        }
    }
    return nlen;
}

std::string StringUtils::Wordreserve(const std::string& sData, int nSize)
{
    const char* bp = sData.data();
    const char* ep = bp+sData.size();
    while (nSize>0 && bp<ep)
    {
        nSize--;
        if (*bp>=0)
        {
            ++bp;
        }
        else
        {
            bp += 2;
        }
    }
    if (bp==ep)
    {
        return sData;
    }
    return sData.substr(0, bp-sData.data());
}

std::string StringUtils::HtmlEncode(string &sString)
{
    int length = sString.length();
    char *buf = new char[length * 10 + 128];

    char* it = (char*) sString.c_str();
    char *out = buf;

    for (int i = 0; i < length; i++)
    {
        switch (*it)
        {
        case '<':
            memcpy(out, "&lt;", strlen("&lt;"));
            out += strlen("&lt;");
            break;

        case '>':
            memcpy(out, "&gt;", strlen("&gt;"));
            out += strlen("&gt;");
            break;


        case '&':
            memcpy(out, "&amp;", strlen("&amp;"));
            out += strlen("&amp;");
            break;

        case '\"':
            memcpy(out, "&quot;", strlen("&quot;"));
            out += strlen("&quot;");
            break;

        case '\'':
            memcpy(out, "&#39;", strlen("&#39;"));
            out += strlen("&#39;");
            break;

        default:
            *out = *it;
            out++;
        }
        it++;
    }
    *out = '\0';

    sString = buf;
    delete []buf;
    return sString;
}

void StringUtils::HtmlSpecialchar(const string & sSrc, string &sResult)
{
    sResult = "";
    sResult.reserve(1024);
    char szTmp[4] = {0};
    for (unsigned int i = 0; i < sSrc.size(); i++)
    {
        if (isalnum(sSrc.at(i)) || (sSrc.at(i) == '-')
            ||(sSrc.at(i) == '_') || (sSrc.at(i) == '.'))
        {
            sResult.push_back(sSrc.at(i));
        }
        else
        {
            if (sSrc.at(i) == ' ')
            {
                sResult.push_back('+');
            }
            else
            {
                memset(szTmp, 0, sizeof(szTmp));
                snprintf(szTmp, sizeof(szTmp), "%%%02x", sSrc.at(i));
                sResult.append(szTmp);
            }

        }
    }
}

}