/*********************************************************************************
 FileName: 
 Author: xufubo
 Date:  2019-01-01
 Description:
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/

#include "AppletMallCommonImp.h"

using namespace std;
using namespace rapidjson;
using namespace tars;
using namespace HardwareApplet;

void AppletMallCommonImp::initialize()
{
    _dbInfo = new TC_Mysql();
    TC_DBConf dbConf;
    dbConf.loadFromMap(DEF_CFG_SINGLETON->_dbInfoConf);
    _dbInfo->init(dbConf);
    _dbInfo->connect();

    _pServiceFactory->Initialize(_pConfigurationFactory);
	_pAddressCommand = new AddressCommand(_pConfigurationFactory, _pServiceFactory, _dbInfo);
	_pOpCommand =new OpCommand(_pConfigurationFactory, _pServiceFactory);
}

void AppletMallCommonImp::destroy()
{
	if (_pAddressCommand != NULL)
	{
		delete _pAddressCommand; _pAddressCommand = NULL;
	}
	if (_pOpCommand != NULL)
	{
		delete _pOpCommand; _pOpCommand = NULL;
	}
    if (_dbInfo != NULL)
    {
        delete _dbInfo; _dbInfo = NULL;
    }
}

tars::Int32 AppletMallCommonImp::addMyAddressInfo(const HardwareApplet::AppletCommHead & stHead,const vector<tars::Char> & vtIn,vector<tars::Char> &vtOut,tars::TarsCurrentPtr current)
{
    int ret = 0;
    try
    {
        HardwareApplet::AddMyAddressInfoReq stReq;
        HardwareApplet::AddMyAddressInfoRsp stRsp;
        if (HardwareApplet::TarsDecode<HardwareApplet::AddMyAddressInfoReq>(vtIn, stReq) != 0)
        {
            ERRORLOG_WITHHEAD(stHead, vtIn.size()) << endl;
            stRsp.ret = -1;
            return -1;
        }
        current->setResponse(false); // 设置非自动回包
        ret = _pAddressCommand->addMyAddressInfo(stHead, stReq, stRsp, current);
    }
    __CATCH_EXCEPTION_WITH__("|AppletMallCommonImp::addMyAddressInfo");
    return ret;
}

tars::Int32 AppletMallCommonImp::getMyAddressList(const HardwareApplet::AppletCommHead & stHead,const vector<tars::Char> & vtIn,vector<tars::Char> &vtOut,tars::TarsCurrentPtr current)
{
    int ret = 0;
    try
    {
        HardwareApplet::GetMyAddressListReq stReq;
        HardwareApplet::GetMyAddressListRsp stRsp;
        if (HardwareApplet::TarsDecode<HardwareApplet::GetMyAddressListReq>(vtIn, stReq) != 0)
        {
            ERRORLOG_WITHHEAD(stHead, vtIn.size()) << endl;
            stRsp.ret = -1;
            return -1;
        }
        current->setResponse(false); // 设置非自动回包
        ret = _pAddressCommand->getMyAddressList(stHead, stReq, stRsp, current);
    }
    __CATCH_EXCEPTION_WITH__("|AppletMallCommonImp::getMyAddressList");
    return ret;
}

tars::Int32 AppletMallCommonImp::getProvinceList(const HardwareApplet::AppletCommHead & stHead,const vector<tars::Char> & vtIn,vector<tars::Char> &vtOut,tars::TarsCurrentPtr current)
{
    int ret = 0;
    try
    {
        HardwareApplet::GetProvinceListReq stReq;
        HardwareApplet::GetProvinceListRsp stRsp;
        if (HardwareApplet::TarsDecode<HardwareApplet::GetProvinceListReq>(vtIn, stReq) != 0)
        {
            ERRORLOG_WITHHEAD(stHead, vtIn.size()) << endl;
            stRsp.ret = -1;
            return -1;
        }
        current->setResponse(false); // 设置非自动回包
        ret = _pAddressCommand->getProvinceList(stHead, stReq, stRsp, current);
    }
    __CATCH_EXCEPTION_WITH__("|AppletMallCommonImp::getProvinceList");
    return ret;
}

tars::Int32 AppletMallCommonImp::getCityListByProvince(const HardwareApplet::AppletCommHead & stHead,const vector<tars::Char> & vtIn,vector<tars::Char> &vtOut,tars::TarsCurrentPtr current)
{
    int ret = 0;
    try
    {
        HardwareApplet::GetCityListByProvinceReq stReq;
        HardwareApplet::GetCityListByProvinceRsp stRsp;
        if (HardwareApplet::TarsDecode<HardwareApplet::GetCityListByProvinceReq>(vtIn, stReq) != 0)
        {
            ERRORLOG_WITHHEAD(stHead, vtIn.size()) << endl;
            stRsp.ret = -1;
            return -1;
        }
        current->setResponse(false); // 设置非自动回包
        ret = _pAddressCommand->getCityListByProvince(stHead, stReq, stRsp, current);
    }
    __CATCH_EXCEPTION_WITH__("|AppletMallCommonImp::getCityListByProvince");
    return ret;
}

tars::Int32 AppletMallCommonImp::getCountyListByCity(const HardwareApplet::AppletCommHead & stHead,const vector<tars::Char> & vtIn,vector<tars::Char> &vtOut,tars::TarsCurrentPtr current)
{
    int ret = 0;
    try
    {
        HardwareApplet::GetCountyListByCityReq stReq;
        HardwareApplet::GetCountyListByCityRsp stRsp;
        if (HardwareApplet::TarsDecode<HardwareApplet::GetCountyListByCityReq>(vtIn, stReq) != 0)
        {
            ERRORLOG_WITHHEAD(stHead, vtIn.size()) << endl;
            stRsp.ret = -1;
            return -1;
        }
        current->setResponse(false); // 设置非自动回包
        ret = _pAddressCommand->getCountyListByCity(stHead, stReq, stRsp, current);
    }
    __CATCH_EXCEPTION_WITH__("|AppletMallCommonImp::getCountyListByCity");
    return ret;
}
