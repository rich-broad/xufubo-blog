/*********************************************************************************
 FileName: 
 Author: xufubo
 Date:  2018-12-26
 Description: 
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/

#ifndef __IP_CHAMGE_H__
#define __IP_CHAMGE_H__

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/route.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <string>
#include "base_define.h"

namespace base_utils 
{
    /// @brief 获取远端ip
    /// @return 远端ip, 整数形式, 本地字节序!
    u_int GetRemoteIP();

    /// @brief 获取本地ip
    /// @return 本地ip, 整数形式, 本地字节序!
    u_int GetLocalIP();

    /// @brief 获取远端ip
    /// @return 远端ip, 字符串形式
    std::string GetRemoteIPStr();

    /// @brief 获取本地ip
    /// @return 本地ip, 字符串形式
    const std::string& GetLocalIPStr();

    /// @brief 将ip从整数形式转换成字符串形式
    /// @param[in] ip 整数ip, 本地字节序!
    /// @return 字符串ip
    std::string IP2Str(u_int ip);
}

#endif
