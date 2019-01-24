/*********************************************************************************
 FileName: 
 Author: xufubo
 Date:  2019-01-01
 Description:
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/
#ifndef COMMAND_H
#define	COMMAND_H

#include "Configuration.h"
#include "DependencedServiceFactory.h"
#include "AppletTrade.h"
 
class Command {
public:
    Command(){}
    Command(ConfigurationFactory* configurationFactory, DependencedServiceFactory* serviceFactory)
    {
        Initialize(configurationFactory, serviceFactory);
    }
    virtual void Initialize(ConfigurationFactory* configurationFactory, DependencedServiceFactory* serviceFactory)
    {
        this->_pConfigurationFactory = configurationFactory;
        this->_pServiceFactory = serviceFactory;
    }
    virtual ~Command(){}

protected:
    ConfigurationFactory* _pConfigurationFactory;
    DependencedServiceFactory* _pServiceFactory;
};

template<typename T>
void sendReponse(const T &t, const HardwareApplet::AppletCommHead& stHead, const string& funcName, const int ret, tars::TarsCurrentPtr current)
{
    try
    {
        vector<char> vtOut;
        int retVal = ret;
        if(HardwareApplet::TarsEncode<T>(t, vtOut) != 0 )
        {
            ERRORLOG ("sendReponse|" << COMM_HEAD_ALL_INFO(stHead) << "|" <<  endl);
        }
        /*
        if(funcName == "getNewTicket")
        {
            HardwareApplet::AppletTrade::async_response_getNewTicket(current, retVal, vtOut);
        }
        else if(funcName == "getNewTicket")
        {
            HardwareApplet::AppletTrade::async_response_getNewTicket(current, retVal, vtOut);
        }
        else 
        {
            ERRORLOG("sendReponse|funcNameError|" << ret << "|" << funcName << "|" << COMM_HEAD_ALL_INFO(stHead) << endl);
        }
        */
    }
    __CATCH_EXCEPTION_WITH__("sendReponse")

}

#endif

