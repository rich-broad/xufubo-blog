/*********************************************************************************
 FileName: 
 Author: xufubo
 Date:  2018-12-26
 Description: 随机数
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/
#include "utf8_utils.h"

namespace base_utils
{

int CUTF8_Utils::CountCharacter(const std::string& str)
{
    int wordNum = 0;
    if(str.empty())
    {
        return wordNum;
    }
    const char* ptr = str.c_str();
    while( *ptr != '\0')
    {
        if(IsZh(ptr))
            wordNum++;
        else
        {
            wordNum++;
            ptr++;
        }
    }
    return wordNum;
}


size_t CUTF8_Utils::CountCharacter2(const std::string &str)
{

    size_t i = 0, w = 0;
    while (i < str.size())
    {
        if(hydra::IsAscii(str[i]))
        {
            i += 1;
        }
        else if(hydra::IsUtf8TwoBytesLeading(str[i]))
        {
            i += 2;
        }
        else if(hydra::IsUtf8ThreeBytesLeading(str[i]))
        {
            i += 3;
        }
        else if(hydra::IsUtf8FourBytesLeading(str[i]))
        {
            i += 4;
        }
        else
        {
            printf("Unknow utf8 char=%02x", str[i]);
            i += 1;
        }
        ++w;
    }

    return w;
}

std::string CUTF8_Utils::Truncate(const std::string& str, unsigned num)
{
    if(num == 0)
    {
        return str;
    }

    std::string tmpstr;
    int wordNum = 0;
    if(str.empty())
    {
        return "";
    }

    const char* ptr = str.c_str();
    while( *ptr != '\0')
    {
        if((unsigned)wordNum == num)
        {
            break;
        }

        const char* tmpptr = ptr;
        if(IsZh(ptr))
        {
            wordNum++;
        }
        else
        {
            wordNum++;
            ptr++;
        }

        tmpstr.append(tmpptr, (ptr-tmpptr));
    }
    return tmpstr;
}

int CUTF8_Utils::CheckAlphaDigitZh(const std::string& str)
{
    int wordNum = 0;
    if(str.empty())
    {
        return wordNum;
    }

    const char* ptr = str.c_str();
    while( *ptr != '\0')
    {
        if(IsAlpha(ptr) || IsDigit(ptr) || IsZh(ptr))
        {
            wordNum++;
        }
        else
        {
            return -1;
        }
    }

    return wordNum;
}

int CUTF8_Utils::CheckDigit(const std::string& str)
{
    int wordNum = 0;
    if(str.empty())
    {
        return wordNum;
    }

    const char* ptr = str.c_str();
    while( *ptr != '\0')
    {
        if(IsDigit(ptr))
        {
            wordNum++;
        }
        else
        {
            return -1;
        }
    }

    return wordNum;
}

bool CUTF8_Utils::IsAlpha(const char*& input)
{
    if ( NULL ==  input)
    {
        return false;
    }

    unsigned char c  = *input;
    if( (c >= 65 && c <= 90) || (c >= 97 && c <= 122))
    {
        input++;
        return true;
    }

    return false;
}

bool CUTF8_Utils::IsDigit(const char*& input)
{
    if ( NULL ==  input)
    {
        return false;
    }

    unsigned char c  = *input;
    if(c >= 48 && c <= 57)
    {
        input++;
        return true;
    }

    return false;
}

bool CUTF8_Utils::IsZh(const char*& input)
{
    if ( NULL ==  input)
    {
        return false;
    }

    if ((*input&0xF0) == 0xE0)
    {
        //check end
        if( *(input+1) == '\0' || *(input+2) == '\0')
        {
            return false;
        }

        unsigned char c  = *input;
        unsigned char c1  = *(input+1);
        unsigned char c2  = *(input+2);

        //check utf8 encode
        if( (c1&0xC0) != 0x80 || (c2&0xC0) != 0x80 )
        {
            return false;
        }

        //check zh

        //1. check low E4B880
        if( c < 0xE4)
        {
            return false;
        }

        if( c == 0xE4 && c1 < 0xB8)
        {
            return false;
        }

        if( c == 0xE4 && c1 == 0xB8 && c2 < 0x80)
        {
            return false;
        }

        //2.check high E9BEB5
        if(c > 0xE9)
        {
            return false;
        }

        if(c == 0xE9 && c1 > 0xBE)
        {
            return false;
        }

        if(c == 0xE9 && c1 == 0xBE && c2 > 0xB5)
        {
            return false;
        }

        input += 3;

        return true;
    }

    return false;
}

}