/*********************************************************************************
 FileName: 
 Author: xufubo
 Date:  2019-01-01
 Description:
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/
#include "AppletTradeServer.h"
#include "CategoryMetaManager.h"

AppletTradeServer g_app;

void AppletTradeServer::initialize()
{
	srand(TNOW);
	addConfig(ServerConfig::ServerName + ".conf");
	addServant<AppletTradeImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".AppletTradeObj");
    if (0 == DEF_CFG_SINGLETON->_printLocal)
    {
        TENLOCAL(false);
    }

    // 异步执行线程池初始化及启动
    async_executor.init(DEF_CFG_SINGLETON->_dwAsyncThreadCount);
    async_executor.start();	
    srand(TNOW);
    {
        // 分类元数据
        CategoryMetaManagerSingleton->initialize();
        AsyncUpdateCategoryMetaInfoTask upCategoryMetaInfo;
        TC_Functor<void, TL::TLMaker<>::Result> cmd(upCategoryMetaInfo);
        TC_FunctorWrapper<TC_Functor<void, TL::TLMaker<>::Result> > wcmd(cmd);
        SUBMIT_ASYNC_TASK(wcmd);
    }

    TARS_ADD_ADMIN_CMD_NORMAL("upCategoryMetaInfo", AppletTradeServer::upCategoryMetaInfo);
}

void AppletTradeServer::destroyApp()
{
	async_executor.stop();
	delete CategoryMetaManagerSingleton;
	delete ConfigurationFactory::GetInstance();
}

bool AppletTradeServer::upCategoryMetaInfo(const string& command, const string& params, string& result)
{
    DEBUGLOG("AppletTradeServer::upCategoryMetaInfo|admin cmd req|" << command << "|" << params << endl);
    if (params != "db_superapp_sdkcs.t_superapp_appinfo")
    {
        return false;
    }

    int ret = CategoryMetaManagerSingleton->upCategoryMetaList();
    if (ret == 0)
    {
        DEBUGLOG("AppletTradeServer::upCategoryMetaInfo|admin cmd suc|" << params << endl);
    }
    else
    {
        ERRORLOG("AppletTradeServer::upCategoryMetaInfo|admin cmd err|" << ret) << endl;
    }
    return false;
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



