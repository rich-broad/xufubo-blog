/*********************************************************************************
 FileName: 
 Author: xufubo
 Date:  2018-12-26
 Description: 随机数
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/

#ifndef __STR_COMM_H__
#define __STR_COMM_H__ 

#include <stdio.h>
#include <stdlib.h>
#include "base_define.h"

#ifdef __cplusplus
extern "C" {
#endif
extern const uchar g_hydra_letter_digit[256];
extern const uchar g_hydra_space[256];
extern const char  g_hydra_hexadecimal[16];
#ifdef __cplusplus
}
#endif

namespace base_utils 
{


static inline bool IsAscii(uchar c)
{
    return c < 128U;
}

static inline bool IsEuroSign(uchar c)
{
    return 128U == c;
}

static inline bool IsAsciiEuroSign(uchar c)
{
    return c <= 128U;
}

static inline bool IsAsciiCtl(uchar c)
{
    return 127U == c || c <= 31U;
}

static inline bool IsUtf8TwoBytesLeading(uchar c)
{
    return ((c & 0xE0) == 0xC0);
}

static inline bool IsUtf8ThreeBytesLeading(uchar c)
{
    return ((c & 0xF0) == 0xE0);
}

static inline bool IsUtf8FourBytesLeading(uchar c)
{
    return ((c & 0xF8) == 0xF0);
}

static inline bool IsUtf8ByteTrailing(uchar c)
{
    return ((c & 0xC0) == 0x80);
}

static inline bool IsGbkTwoBytesLeading(uchar c)
{
    return (c >= 0x81 && c <= 0xFE);
}

static inline bool IsGbkByteTrailing(uchar c)
{
    return (c != 0x7F && c >= 0x40 && c <= 0xFE);
}

/// @brief 将一个byte转换成2个字节(16进制形式), Hex2Char的反操作
static inline void Char2Hex(uchar c, char hex[2])
{
    hex[0] = g_hydra_hexadecimal[c >> 4];
    hex[1] = g_hydra_hexadecimal[c & 15];
}

/// @brief 字符转数字，比如'0' => 0, 'A'/'a' => 10, etc.
static inline char Char2Num(char c)
{
    return (c >= 'A') ? (c & 0xDF) - 'A' + 10 : c - '0';
}

/// @brief 将2个字节(16进制形式)转换成1个byte, Char2Hex的反操作
static inline char Hex2Char(const char hex[2])
{
    return (Char2Num(hex[0]) << 4) + Char2Num(hex[1]);
}

/// @brief 判断是否 字母数字
static inline bool IsLetterDigit(uchar c)
{
    return g_hydra_letter_digit[c];
}

/// @brief 判断是否 空白字符
static inline bool IsSpace(uchar c)
{
    return g_hydra_space[c];
}

}

#endif
