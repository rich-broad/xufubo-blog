
/*********************************************************************************
 FileName: 
 Author: xufubo
 Date:  2018-12-26
 Description: 地址范围检查，检查地址类型
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/

#ifndef __ADDR_CHECKER_H__
#define __ADDR_CHECKER_H__

namespace base_utils 
{

// @brief 检查addr是否在栈上
// @param[in] addr 待检查的内存
// @return true: addr在栈上, false：不是
bool IsAddrStack(const void* addr);

// @brief 检查addr是否mmap或者brk分配的内存
// @param[in] addr 待检查的内存
// @return true: 是mmap或者brk分配的内存，false：不是
bool IsAddrDynamic(const void* addr);

// @brief 检查addr是否全局变量或者静态变量
// @param[in] addr 待检查的内存
// @return true: 是全局变量或者静态变量的地址, false: 不是 
bool IsAddrStatic(const void* addr);

}

#endif
