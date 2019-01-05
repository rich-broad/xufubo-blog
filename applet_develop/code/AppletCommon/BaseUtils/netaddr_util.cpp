/*********************************************************************************
 FileName: 
 Author: xufubo
 Date:  2018-12-26
 Description: 
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/


#include "netaddr_util.h"

using namespace std;
namespace base_utils
{

#define INADDR(x) (*BaseTypePtrConv<struct in_addr>(&ifr->x[sizeof(sa.sin_port)]))

#define MAX_INTERFACE_CNT 128

/**
 * 专门用于获取虚拟机的内网IP
 */
static std::string GetSpecialIP()
{
    static  char  ip[20] = { 0 };
    if (ip[0]!=0)
    {
        return  ip;
    }

    struct ifreq intfBuf[MAX_INTERFACE_CNT];
    int         sockfd = -1;

    const char      *ipaddr = "";

    struct ifconf      ifc;

    struct ifreq       *ifr;
    struct sockaddr_in sa;

    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (sockfd < 0 )
    {
        return "";
    }

    // init the ifc struct
    memset(&ifc, 0, sizeof(ifc));
    ifc.ifc_len = sizeof(intfBuf);
    ifc.ifc_req = intfBuf;
    if (0 != ioctl(sockfd, SIOCGIFCONF, &ifc))
    {
        close(sockfd);
        return "";
    }

    if (sizeof(intfBuf) <=(unsigned) ifc.ifc_len)
    {
        close(sockfd);
        return "";
    }

    ifr = ifc.ifc_req;
    for (; (char *) ifr < (char *) ifc.ifc_req + ifc.ifc_len; ++ifr)
    {
        if (ifr->ifr_addr.sa_data == (ifr+1)->ifr_addr.sa_data)
        {
            continue;  /* duplicate, skip it */
        }

        if (ioctl(sockfd, SIOCGIFFLAGS, ifr))
        {
            continue;  /* failed to get flags, skip it */
        }

        NetAddrInt2Str(INADDR(ifr_addr.sa_data).s_addr, ip);
        if (0 == ::strncmp(ip, "172", 3) || 0 == ::strncmp(ip, "192", 3) || 0 == ::strncmp(ip, "10.", 3) || 0 == ::strncmp(ip, "100", 3))
        {
            ipaddr = ip;
            break;
        }
    }

    close (sockfd);
    return ipaddr;
}

static std::string GetInterfaceIP(int sd, const char *interface)
{
    struct ifreq ifr;
    std::string interfaceIP("");
    strncpy(ifr.ifr_name, interface, sizeof(ifr.ifr_name));
    ifr.ifr_addr.sa_family = AF_INET;

    int ret = ioctl(sd, SIOCGIFADDR, &ifr, sizeof(ifr));
    if (0 == ret) 
    {
        const int MAX_ADDR_LENGTH = 128;
        char ipaddr[MAX_ADDR_LENGTH] = {0};
        struct sockaddr_in *addr = (struct sockaddr_in*)&ifr.ifr_addr;
        strncpy(ipaddr, inet_ntoa(addr->sin_addr), sizeof(ipaddr));
        interfaceIP = ipaddr;
    }

    return interfaceIP;
}

static std::string GetNomalIP()
{
    static  char ip[20] = { 0 };
    if (ip[0]!=0)   return  ip;

    int i;
    int sd = socket (PF_INET, SOCK_STREAM, 0);
    if (sd < 0)
    {
        return "";
    }

    // 优先取eth1的地址
    std::string eth1ip = GetInterfaceIP(sd, "eth1");
    if (!eth1ip.empty())
    {
        // 取到eth1的地址，关闭句柄后返回
        close(sd);
        snprintf(ip, sizeof(ip), "%s", eth1ip.c_str());
        return eth1ip;
    }

    for (i = 1; ; ++i)
    {
        struct ifreq ifr;
        struct sockaddr_in *sin = (struct sockaddr_in *) &ifr.ifr_addr;

        ifr.ifr_ifindex = i;
        if (ioctl (sd, SIOCGIFNAME, &ifr) < 0)
        {
            break;
        }

        /* now ifr.ifr_name is set */
        if (ioctl (sd, SIOCGIFADDR, &ifr) < 0)
        {
            continue;
        }

        memset(ip, 0, sizeof(ip));
        NetAddrInt2Str(sin->sin_addr.s_addr, ip);
        if (0 == strncmp(ip, "172", 3) || 0 == strncmp(ip, "192", 3) || 0 == strncmp(ip, "10.", 3) || 0 == strncmp(ip, "100", 3))
        {
            close (sd);
            return ip;
        }
        else
        {
            continue;
        }
    }

    close (sd);
    return "";
}

u_int GetRemoteIP()
{
    char* pszRemoteAddr = getenv("HTTP_QVIA");
    if (pszRemoteAddr != NULL) // proxy
    {
        uint addr;
        return (sscanf(pszRemoteAddr, "%8x", &addr) == 1) ? addr : -1;
    }
    else                       // not proxy
    {
        pszRemoteAddr = getenv("REMOTE_ADDR");
        return (pszRemoteAddr != NULL) ? ntohl(NetAddrStr2Int(pszRemoteAddr)) : -1;
    }
}

const std::string& GetLocalIPStr()
{
    static std::string ip;

    if (!ip.empty())
    {
        return ip;
    }

    ip = GetNomalIP();
    if (ip.empty())
    {
        ip = GetSpecialIP();
    }

    return ip;
}

u_int GetLocalIP()
{
    static u_int ip = (u_int)ntohl(NetAddrStr2Int(GetLocalIPStr().c_str()));
    return ip;
}

std::string GetRemoteIPStr()
{
    char strip[32];
    in_addr ia;

    ia.s_addr = htonl(GetRemoteIP());
    NetAddrInt2Str(ia, strip);
    return strip;
}

std::string IP2Str(u_int ip)
{
    char strip[32];
    return NetAddrInt2Str(htonl(ip), strip);
}

}
