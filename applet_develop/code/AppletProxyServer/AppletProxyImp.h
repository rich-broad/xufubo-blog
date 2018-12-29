/*********************************************************************************
 FileName: AppletProxyImp.h
 Author: xufubo
 Date:  2018-12-26
 Description:
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/

#ifndef _AppletProxyImp_H_
#define _AppletProxyImp_H_

#include "servant/Application.h"
#include "AppletCommon.h"
#include "CmdProcess.h"

class AppletProxyImp : public tars::Servant
{
public:
    virtual ~AppletProxyImp() {}
    virtual void initialize();
    virtual void destroy();

    // 处理请求入口
    int doRequest(tars::TarsCurrentPtr current, vector<char>& response);

private:
    CmdProcess* _pCmdProcess;
};


#endif
