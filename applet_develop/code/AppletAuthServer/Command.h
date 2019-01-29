/*********************************************************************************
 FileName: Command.h
 Author: xufubo
 Date:  2018-12-29
 Description:
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/

#ifndef _COMMAND_H
#define	_COMMAND_H

#include "Configuration.h"

class Command {
public:
    Command(){}
    Command(ConfigurationFactory* configurationFactory)
    {
        Initialize(configurationFactory);
    }
    virtual void Initialize(ConfigurationFactory* configurationFactory)
    {
        this->_pConfigurationFactory = configurationFactory;
    }
    virtual ~Command(){}

protected:
    ConfigurationFactory* _pConfigurationFactory;
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

        if(funcName == "getNewTicket")
        {
            HardwareApplet::AppletAuth::async_response_getNewTicket(current, retVal, vtOut);
        }
        else if(funcName == "getNewCookie")
        {
            HardwareApplet::AppletAuth::async_response_getNewCookie(current, retVal, vtOut);
        }
        else 
        {
            ERRORLOG("sendReponse|funcNameError|" << ret << "|" << funcName << "|" << COMM_HEAD_ALL_INFO(stHead) << endl);
        }
    }
    __CATCH_EXCEPTION_WITH__("sendReponse")

}

#endif

