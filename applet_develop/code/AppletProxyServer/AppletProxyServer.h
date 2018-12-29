
/*********************************************************************************
 FileName: AppletProxyServer.h
 Author: xufubo
 Date:  2018-12-26
 Description:
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/
#ifndef _AppletProxyServer_H_
#define _AppletProxyServer_H_

#include <iostream>
#include "servant/Application.h"
#include "AppletProxyImp.h"

using namespace tars;

/**
 *
 **/
class AppletProxyServer : public Application
{
public:
    /**
     *
     **/
    virtual ~AppletProxyServer() {};

    /**
     *
     **/
    virtual void initialize();

    /**
     *
     **/
    virtual void destroyApp();

    static int parseRecv(string& in, string& out);
};

extern AppletProxyServer g_app;

////////////////////////////////////////////
#endif
