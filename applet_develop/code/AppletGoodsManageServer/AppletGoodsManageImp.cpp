/*********************************************************************************
 FileName: 
 Author: xufubo
 Date:  2019-01-01
 Description:
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/

#include "AppletGoodsManageImp.h"

using namespace std;
using namespace rapidjson;
using namespace tars;
using namespace HardwareApplet;

void AppletGoodsManageImp::initialize()
{
    _dbInfo = new TC_Mysql();
    TC_DBConf dbConf;
    dbConf.loadFromMap(DEF_CFG_SINGLETON->_dbInfoConf);
    _dbInfo->init(dbConf);
    _dbInfo->connect();

    _pServiceFactory->Initialize(_pConfigurationFactory);
	_pGoodsCommand = new GoodsCommand(_pConfigurationFactory, _pServiceFactory);
	_pOpCommand =new OpCommand(_pConfigurationFactory, _pServiceFactory);
    _pMetaDataCommand =new MetaDataCommand(_pConfigurationFactory, _pServiceFactory);
}

void AppletGoodsManageImp::destroy()
{
	if (_pGoodsCommand != NULL)
	{
		delete _pGoodsCommand; _pGoodsCommand = NULL;
	}
	if (_pOpCommand != NULL)
	{
		delete _pOpCommand; _pOpCommand = NULL;
	}
    if (_pMetaDataCommand)
    {
        delete _pMetaDataCommand; _pMetaDataCommand = NULL;
    }
    if (_dbInfo != NULL)
    {
        delete _dbInfo; _dbInfo = NULL;
    }
    
}


int AppletGoodsManageImp::getCategoryList(const HardwareApplet::AppletCommHead & stHead,const vector<tars::Char> & vtIn,vector<tars::Char> &vtOut,tars::TarsCurrentPtr current)
{
    int ret = 0;
    try
    {
        HardwareApplet::GetCategoryListReq stReq;
        HardwareApplet::GetCategoryListRsp stRsp;
        if (HardwareApplet::TarsDecode<HardwareApplet::GetCategoryListReq>(vtIn, stReq) != 0)
        {
            ERRORLOG(COMM_HEAD_ALL_INFO(stHead) << "|" << vtIn.size()) << endl;
            stRsp.ret = -1;
            return -1;
        }
        current->setResponse(false); // 设置非自动回包
        ret = _pMetaDataCommand->getCategoryList(stHead, stReq, stRsp, current);
    }
    __CATCH_EXCEPTION_WITH__("|AppletGoodsManageImp::getCategoryList");
	return ret;
}

int AppletGoodsManageImp::addCategoryInfo(const HardwareApplet::AppletCommHead & stHead,const vector<tars::Char> & vtIn,vector<tars::Char> &vtOut,tars::TarsCurrentPtr current)
{
    int ret = 0;
    try
    {
        HardwareApplet::AddCategoryInfoReq stReq;
        HardwareApplet::AddCategoryInfoRsp stRsp;
        if (HardwareApplet::TarsDecode<HardwareApplet::AddCategoryInfoReq>(vtIn, stReq) != 0)
        {
            ERRORLOG(COMM_HEAD_ALL_INFO(stHead) << "|" << vtIn.size()) << endl;
            stRsp.ret = -1;
            return -1;
        }
        current->setResponse(false); // 设置非自动回包
        ret = _pMetaDataCommand->addCategoryInfo(stHead, stReq, stRsp, current);
    }
    __CATCH_EXCEPTION_WITH__("|AppletGoodsManageImp::addCategoryInfo");
    return ret;
}

int AppletGoodsManageImp::getAttributeList(const HardwareApplet::AppletCommHead & stHead,const vector<tars::Char> & vtIn,vector<tars::Char> &vtOut,tars::TarsCurrentPtr current)
{
    int ret = 0;
    try
    {
        HardwareApplet::GetAttributeListReq stReq;
        HardwareApplet::GetAttributeListRsp stRsp;
        if (HardwareApplet::TarsDecode<HardwareApplet::GetAttributeListReq>(vtIn, stReq) != 0)
        {
            ERRORLOG(COMM_HEAD_ALL_INFO(stHead) << "|" << vtIn.size()) << endl;
            stRsp.ret = -1;
            return -1;
        }
        current->setResponse(false); // 设置非自动回包
        ret = _pMetaDataCommand->getAttributeList(stHead, stReq, stRsp, current);
    }
    __CATCH_EXCEPTION_WITH__("|AppletGoodsManageImp::getAttributeList");
    return ret;
}

int AppletGoodsManageImp::addAttributeInfo(const HardwareApplet::AppletCommHead & stHead,const vector<tars::Char> & vtIn,vector<tars::Char> &vtOut,tars::TarsCurrentPtr current)
{
    int ret = 0;
    try
    {
        HardwareApplet::AddAttributeInfoReq stReq;
        HardwareApplet::AddAttributeInfoRsp stRsp;
        if (HardwareApplet::TarsDecode<HardwareApplet::AddAttributeInfoReq>(vtIn, stReq) != 0)
        {
            ERRORLOG(COMM_HEAD_ALL_INFO(stHead) << "|" << vtIn.size()) << endl;
            stRsp.ret = -1;
            return -1;
        }
        current->setResponse(false); // 设置非自动回包
        ret = _pMetaDataCommand->addAttributeInfo(stHead, stReq, stRsp, current);
    }
    __CATCH_EXCEPTION_WITH__("|AppletGoodsManageImp::addAttributeInfo");
    return ret;
}

int AppletGoodsManageImp::getAttributeValueList(const HardwareApplet::AppletCommHead & stHead,const vector<tars::Char> & vtIn,vector<tars::Char> &vtOut,tars::TarsCurrentPtr current)
{
    int ret = 0;
    try
    {
        HardwareApplet::GetAttributeValueListReq stReq;
        HardwareApplet::GetAttributeValueListRsp stRsp;
        if (HardwareApplet::TarsDecode<HardwareApplet::GetAttributeValueListReq>(vtIn, stReq) != 0)
        {
            ERRORLOG(COMM_HEAD_ALL_INFO(stHead) << "|" << vtIn.size()) << endl;
            stRsp.ret = -1;
            return -1;
        }
        current->setResponse(false); // 设置非自动回包
        ret = _pMetaDataCommand->getAttributeValueList(stHead, stReq, stRsp, current);
    }
    __CATCH_EXCEPTION_WITH__("|AppletGoodsManageImp::getAttributeValueList");
    return ret;
}

int AppletGoodsManageImp::addAttributeValueInfo(const HardwareApplet::AppletCommHead & stHead,const vector<tars::Char> & vtIn,vector<tars::Char> &vtOut,tars::TarsCurrentPtr current)
{
    int ret = 0;
    try
    {
        HardwareApplet::AddAttributeValueInfoReq stReq;
        HardwareApplet::AddAttributeValueInfoRsp stRsp;
        if (HardwareApplet::TarsDecode<HardwareApplet::AddAttributeValueInfoReq>(vtIn, stReq) != 0)
        {
            ERRORLOG(COMM_HEAD_ALL_INFO(stHead) << "|" << vtIn.size()) << endl;
            stRsp.ret = -1;
            return -1;
        }
        current->setResponse(false); // 设置非自动回包
        ret = _pMetaDataCommand->addAttributeValueInfo(stHead, stReq, stRsp, current);
    }
    __CATCH_EXCEPTION_WITH__("|AppletGoodsManageImp::addAttributeValueInfo");
    return ret;
}

int AppletGoodsManageImp::getBrandList(const HardwareApplet::AppletCommHead & stHead,const vector<tars::Char> & vtIn,vector<tars::Char> &vtOut,tars::TarsCurrentPtr current)
{
    int ret = 0;
    try
    {
        HardwareApplet::GetBrandListReq stReq;
        HardwareApplet::GetBrandListRsp stRsp;
        if (HardwareApplet::TarsDecode<HardwareApplet::GetBrandListReq>(vtIn, stReq) != 0)
        {
            ERRORLOG(COMM_HEAD_ALL_INFO(stHead) << "|" << vtIn.size()) << endl;
            stRsp.ret = -1;
            return -1;
        }
        current->setResponse(false); // 设置非自动回包
        ret = _pMetaDataCommand->getBrandList(stHead, stReq, stRsp, current);
    }
    __CATCH_EXCEPTION_WITH__("|AppletGoodsManageImp::getBrandList");
    return ret;
}

int AppletGoodsManageImp::addBrandInfo(const HardwareApplet::AppletCommHead & stHead,const vector<tars::Char> & vtIn,vector<tars::Char> &vtOut,tars::TarsCurrentPtr current)
{
    int ret = 0;
    try
    {
        HardwareApplet::AddBrandInfoReq stReq;
        HardwareApplet::AddBrandInfoRsp stRsp;
        if (HardwareApplet::TarsDecode<HardwareApplet::AddBrandInfoReq>(vtIn, stReq) != 0)
        {
            ERRORLOG(COMM_HEAD_ALL_INFO(stHead) << "|" << vtIn.size()) << endl;
            stRsp.ret = -1;
            return -1;
        }
        current->setResponse(false); // 设置非自动回包
        ret = _pMetaDataCommand->addBrandInfo(stHead, stReq, stRsp, current);
    }
    __CATCH_EXCEPTION_WITH__("|AppletGoodsManageImp::addBrandInfo");
    return ret;
}

int AppletGoodsManageImp::getMakerList(const HardwareApplet::AppletCommHead & stHead,const vector<tars::Char> & vtIn,vector<tars::Char> &vtOut,tars::TarsCurrentPtr current)
{
    int ret = 0;
    try
    {
        HardwareApplet::GetMakerListReq stReq;
        HardwareApplet::GetMakerListRsp stRsp;
        if (HardwareApplet::TarsDecode<HardwareApplet::GetMakerListReq>(vtIn, stReq) != 0)
        {
            ERRORLOG(COMM_HEAD_ALL_INFO(stHead) << "|" << vtIn.size()) << endl;
            stRsp.ret = -1;
            return -1;
        }
        current->setResponse(false); // 设置非自动回包
        ret = _pMetaDataCommand->getMakerList(stHead, stReq, stRsp, current);
    }
    __CATCH_EXCEPTION_WITH__("|AppletGoodsManageImp::getMakerList");
    return ret;
}

int AppletGoodsManageImp::addMakerInfo(const HardwareApplet::AppletCommHead & stHead,const vector<tars::Char> & vtIn,vector<tars::Char> &vtOut,tars::TarsCurrentPtr current)
{
    int ret = 0;
    try
    {
        HardwareApplet::AddMakerInfoReq stReq;
        HardwareApplet::AddMakerInfoRsp stRsp;
        if (HardwareApplet::TarsDecode<HardwareApplet::AddMakerInfoReq>(vtIn, stReq) != 0)
        {
            ERRORLOG(COMM_HEAD_ALL_INFO(stHead) << "|" << vtIn.size()) << endl;
            stRsp.ret = -1;
            return -1;
        }
        current->setResponse(false); // 设置非自动回包
        ret = _pMetaDataCommand->addMakerInfo(stHead, stReq, stRsp, current);
    }
    __CATCH_EXCEPTION_WITH__("|AppletGoodsManageImp::addMakerInfo");
    return ret;
}

int AppletGoodsManageImp::getGoodsSPUList(const HardwareApplet::AppletCommHead & stHead,const vector<tars::Char> & vtIn,vector<tars::Char> &vtOut,tars::TarsCurrentPtr current)
{
    int ret = 0;
    try
    {
        HardwareApplet::GetGoodsSPUListReq stReq;
        HardwareApplet::GetGoodsSPUListRsp stRsp;
        if (HardwareApplet::TarsDecode<HardwareApplet::GetGoodsSPUListReq>(vtIn, stReq) != 0)
        {
            ERRORLOG(COMM_HEAD_ALL_INFO(stHead) << "|" << vtIn.size()) << endl;
            stRsp.ret = -1;
            return -1;
        }
        current->setResponse(false); // 设置非自动回包
        ret = _pGoodsCommand->getGoodsSPUList(stHead, stReq, stRsp, current);
    }
    __CATCH_EXCEPTION_WITH__("|AppletGoodsManageImp::getGoodsSPUList");
    return ret;
}

int AppletGoodsManageImp::addGoodsSPUInfo(const HardwareApplet::AppletCommHead & stHead,const vector<tars::Char> & vtIn,vector<tars::Char> &vtOut,tars::TarsCurrentPtr current)
{
    int ret = 0;
    try
    {
        HardwareApplet::AddGoodsSPUInfoReq stReq;
        HardwareApplet::AddGoodsSPUInfoRsp stRsp;
        if (HardwareApplet::TarsDecode<HardwareApplet::AddGoodsSPUInfoReq>(vtIn, stReq) != 0)
        {
            ERRORLOG(COMM_HEAD_ALL_INFO(stHead) << "|" << vtIn.size()) << endl;
            stRsp.ret = -1;
            return -1;
        }
        current->setResponse(false); // 设置非自动回包
        ret = _pGoodsCommand->addGoodsSPUInfo(stHead, stReq, stRsp, current);
    }
    __CATCH_EXCEPTION_WITH__("|AppletGoodsManageImp::addGoodsSPUInfo");
    return ret;
}

