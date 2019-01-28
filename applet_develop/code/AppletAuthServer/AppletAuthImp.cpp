/*********************************************************************************
 FileName: AppletAuthImp.cpp
 Author: xufubo
 Date:  2018-12-29
 Description:
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/

#include "AppletAuthImp.h"

using namespace HardwareApplet;

void AppletAuthImp::initialize()
{
    _dbInfo = new TC_Mysql();
    TC_DBConf dbConf;
    dbConf.loadFromMap(DEF_CFG_SINGLETON->_dbInfoConf);
    _dbInfo->init(dbConf);
    _dbInfo->connect();
	_pAuthCommand = new AuthCommand(_pConfigurationFactory, _dbInfo);   
}

void AppletAuthImp::destroy()
{
	delete _pAuthCommand;
    delete _dbInfo;
}

int AppletAuthImp::getNewTicket(const HardwareApplet::AppletCommHead& stHead,const vector<tars::Char> & vtIn,vector<tars::Char> &vtOut,tars::TarsCurrentPtr current)
{
    DEBUGLOG("getNewTicket enter: vtIn.size = " << vtIn.size() << endl);
    HardwareApplet::GetNewTicketReq stReq;
    HardwareApplet::GetNewTicketRsp stRsp;
    int ret = 0;
    try
    {
        if (TarsDecode<HardwareApplet::GetNewTicketReq>(vtIn, stReq) != 0)
        {
            ERRORLOG("getNewTicket TarsDecode error|" << COMM_HEAD_ALL_INFO(stHead) << "|" << vtIn.size() << endl);
            stRsp.ret = -1;
            return -1;
        }
        current->setResponse(false);
        ret = _pAuthCommand->getNewTicket(stHead, stReq, stRsp, current);
    }
    __CATCH_EXCEPTION_WITH__("AppletAuthImp::getNewTicket");
    return ret;
}

int AppletAuthImp::getNewCookie(const HardwareApplet::AppletCommHead& stHead,const vector<tars::Char> & vtIn,vector<tars::Char> &vtOut,tars::TarsCurrentPtr current)
{
    DEBUGLOG("getNewCookie enter: vtIn.size = " << vtIn.size() << endl);
    HardwareApplet::GetNewCookieReq stReq;
    HardwareApplet::GetNewCookieRsp stRsp;
    int ret = 0;
    try
    {
        if (TarsDecode<HardwareApplet::GetNewCookieReq>(vtIn, stReq) != 0)
        {
            ERRORLOG("getNewCookie TarsDecode error|" << COMM_HEAD_ALL_INFO(stHead) << "|" << vtIn.size() << endl);
            stRsp.ret = -1;
            return -1;
        }
        current->setResponse(false);
        ret = _pAuthCommand->getNewCookie(stHead, stReq, stRsp, current);
    }
    __CATCH_EXCEPTION_WITH__("AppletAuthImp::getNewCookie");
    return ret;
}
