/*********************************************************************************
 FileName: 
 Author: xufubo
 Date:  2018-12-26
 Description: 网络地址转换工具
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/
#ifndef __NETADDR_UTIL_H__
#define __NETADDR_UTIL_H__ 

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <net/route.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <string>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include "base_define.h"

namespace base_utils 
{

struct SIpBuf
{
    char ip[16];
    SIpBuf() { ip[0] = ip[15] = 0; }
};

/// @brief 网络地址转换，方便调用, 举例：printf("%s\n", HydraIp(intip).ip)
/// @param[in] intip 网络字节序整数ip
/// @return 字符串ip结构体
inline SIpBuf HydraIp(uint intip)
{
    SIpBuf buf;
    inet_ntop(AF_INET, &intip, buf.ip, 16); 
    return buf;
}

/// @brief 网络地址转换，方便调用, 举例：printf("%s\n", HydraIp(addr).ip)
/// @param[in] addr 网络地址结构体
/// @return 字符串ip结构体
inline SIpBuf HydraIp(const sockaddr_in& addr)
{
    return HydraIp(addr.sin_addr.s_addr);
}

/// @brief 端口转换，方便调用, 举例: printf("%d\n", HydraPort(addr));
/// @param[in] addr 网络地址结构体
/// @return 主机序端口
inline int HydraPort(const sockaddr_in& addr)
{
    return ntohs(addr.sin_port);
}

/// @brief 网络地址转换，转换整数ip到字符串ip, 替换ntoa
/// @param[in] ia 网络字节序整数ip
/// @param[out] strip 字符串ip，类似10.6.207.22
/// @return 转换后的字符串ip 
inline const char* NetAddrInt2Str(in_addr ia, char strip[16])
{
    inet_ntop(AF_INET, &ia, strip, 16);
    return strip; 
}

/// @brief 网络地址转换，转换整数ip到字符串ip, 替换ntoa
/// @param[in] addr 网络地址结构体
/// @param[out] strip 字符串ip，类似10.6.207.22
/// @return 转换后的字符串ip 
inline const char* NetAddrInt2Str(const sockaddr_in& addr, char strip[16])
{
    return NetAddrInt2Str(addr.sin_addr, strip);
}

/// @brief 网络地址转换，转换整数ip到字符串ip, 替换ntoa
/// @param[in] intip 网络字节序整数ip
/// @param[out] strip 字符串ip，类似10.6.207.22
/// @return 转换后的字符串ip 
inline const char* NetAddrInt2Str(uint intip, char strip[16])
{
    in_addr ia;
    ia.s_addr = intip;
    return NetAddrInt2Str(ia, strip);
}

/// @brief 网络地址转换转,换字符串ip到整数ip, 替换inet_addr
/// @param[in] strip 字符串ip，类似10.6.207.22
/// @return 转换后的整数ip（网络字节序）
inline uint NetAddrStr2Int(const char* strip)
{
	if (NULL == strip)
	{
		return 0;
	}

    in_addr ia;
    ia.s_addr = 0;
    inet_aton(strip, &ia);
    return ia.s_addr;
}

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
