/*********************************************************************************
 FileName: AppletAuthServer.cpp
 Author: xufubo
 Date:  2018-12-28
 Description:
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/

#include "AppletAuthServer.h"
#include "util/tc_mysql.h"
#include "util/tc_config.h"

AppletAuthServer g_app;

void AppletAuthServer::initialize()
{
	addConfig(ServerConfig::ServerName + ".conf");
	addServant<AppletAuthImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".AppletAuthObj");

    if (0 == DEF_CFG_SINGLETON->_printLocal)
    {
        TENLOCAL(false);
    }

	httpAsync.start();
	httpAsync.setTimeout(DEF_CFG_SINGLETON->_iHttpTimeOut);
}

void AppletAuthServer::destroyApp()
{
	httpAsync.terminate();
	delete ConfigurationFactory::GetInstance();
}

int main(int argc, char* argv[])
{
	try
	{
		g_app.main(argc, argv);
		g_app.waitForShutdown();
	}
	catch (std::exception& e)
	{
		cerr << "std::exception:" << e.what() << std::endl;
	}
	catch (...)
	{
		cerr << "unknown exception." << std::endl;
	}
	return -1;
}

