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
#include "ip.h"
#include "string.h"
#include "str_comm.h"


using namespace vas;

namespace base_utils
{

std::string CStringUtils::I64TOS(uin_64 key)
{
    char szkey[64] = {0};
    snprintf(szkey, sizeof(szkey), "%lu", key);
    return  std::string(szkey);
}

int CStringUtils::Hex2Deci(const std::string& hex, uint16_t& deci)
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

int CStringUtils::Hex2Deci(const std::string& hex, uint32_t& deci, bool ignore_0x)
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

int CStringUtils::GetUniKey(std::string& uni_key)
{
    static bool init = false;
    if (!init)
    {
        srand(getpid());
        init = true;
    }
    int ret = 0;
    unsigned ip = hydra::GetLocalIP();
    pid_t pid = getpid();
    uint64_t us_ts = base_utils::CTimeUtils::GetCurrentUSTime();
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

int CStringUtils::VersionCompare(const std::string& ver1, const std::string& ver2)
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

}