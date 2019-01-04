/*********************************************************************************
 FileName: 
 Author: xufubo
 Date:  2018-12-26
 Description: 
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/

#include "addr_checker.h"
#include "base_define.h"
#include <stdio.h>

extern void* _end;

namespace base_utils 
{

bool IsAddrStack(const void* addr)
{
    _UNUSED_ char dummy = 0;
    return addr > &dummy;
}

bool IsAddrDynamic(const void* addr)
{
    _UNUSED_ char dummy = 0;
    return addr > &_end && addr < &dummy;
}

bool IsAddrStatic(const void* addr)
{
    return addr < &_end;
}

}
