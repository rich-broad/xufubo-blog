/*********************************************************************************
 FileName: 
 Author: xufubo
 Date:  2018-12-26
 Description: 随机数
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/

#ifndef __SEC_BASE64_H_
#define __SEC_BASE64_H_

#include <stdint.h>

namespace base_utils
{

int Base64Encode(const uint8_t *in_str, uint32_t length, char *out_str,uint32_t *ret_length);
int Base64Decode(const char *in_str, uint32_t length, uint8_t *out_str, uint32_t *ret_length);

}

#endif

