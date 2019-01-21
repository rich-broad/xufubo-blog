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
#include "AppletGoodsManage.h"
#include "MetaManager.h"

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
        
        if(funcName == "getCategoryList")
        {
            HardwareApplet::AppletGoodsManage::async_response_getCategoryList(current, retVal, vtOut);
        }
        else if(funcName == "addCategoryInfo")
        {
            HardwareApplet::AppletGoodsManage::async_response_addCategoryInfo(current, retVal, vtOut);
        }
        else if(funcName == "getAttributeList")
        {
            HardwareApplet::AppletGoodsManage::async_response_getAttributeList(current, retVal, vtOut);
        }
        else if(funcName == "addAttributeInfo")
        {
            HardwareApplet::AppletGoodsManage::async_response_addAttributeInfo(current, retVal, vtOut);
        }
        else if(funcName == "getAttributeValueList")
        {
            HardwareApplet::AppletGoodsManage::async_response_getAttributeValueList(current, retVal, vtOut);
        }
        else if(funcName == "addAttributeValueInfo")
        {
            HardwareApplet::AppletGoodsManage::async_response_addAttributeValueInfo(current, retVal, vtOut);
        }
        else if(funcName == "getBrandList")
        {
            HardwareApplet::AppletGoodsManage::async_response_getBrandList(current, retVal, vtOut);
        }
        else if(funcName == "addBrandInfo")
        {
            HardwareApplet::AppletGoodsManage::async_response_addBrandInfo(current, retVal, vtOut);
        }
        else if(funcName == "getMakerList")
        {
            HardwareApplet::AppletGoodsManage::async_response_getMakerList(current, retVal, vtOut);
        }
        else if(funcName == "addMakerInfo")
        {
            HardwareApplet::AppletGoodsManage::async_response_addMakerInfo(current, retVal, vtOut);
        }
        else if(funcName == "getGoodsSPUList")
        {
            HardwareApplet::AppletGoodsManage::async_response_getGoodsSPUList(current, retVal, vtOut);
        }
        else if(funcName == "addGoodsSPUInfo")
        {
            HardwareApplet::AppletGoodsManage::async_response_addGoodsSPUInfo(current, retVal, vtOut);
        }
        else 
        {
            ERRORLOG("sendReponse|funcNameError|" << ret << "|" << funcName << "|" << COMM_HEAD_ALL_INFO(stHead) << endl);
        }
        
    }
    __CATCH_EXCEPTION_WITH__("sendReponse")

}

#endif

