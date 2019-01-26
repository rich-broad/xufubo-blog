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
#include "MetaManager.h"

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
        // 属性元数据
        AttributeMetaManagerSingleton->initialize();
        AsyncUpdateAttributeMetaInfoTask upMetaInfo;
        TC_Functor<void, TL::TLMaker<>::Result> cmd(upMetaInfo);
        TC_FunctorWrapper<TC_Functor<void, TL::TLMaker<>::Result> > wcmd(cmd);
        SUBMIT_ASYNC_TASK(wcmd);
    }

    {
        // 属性值元数据
        AttrValueMetaManagerSingleton->initialize();
        AsyncUpdateAttrValueMetaInfoTask upMetaInfo;
        TC_Functor<void, TL::TLMaker<>::Result> cmd(upMetaInfo);
        TC_FunctorWrapper<TC_Functor<void, TL::TLMaker<>::Result> > wcmd(cmd);
        SUBMIT_ASYNC_TASK(wcmd);
    }

    TARS_ADD_ADMIN_CMD_NORMAL("upAttributeMetaInfo", AppletTradeServer::upAttributeMetaInfo);
    TARS_ADD_ADMIN_CMD_NORMAL("upAttrValueMetaInfo", AppletTradeServer::upAttrValueMetaInfo);
}

void AppletTradeServer::destroyApp()
{
	async_executor.stop();
    delete AttributeMetaManagerSingleton;
    delete AttrValueMetaManagerSingleton;
	delete ConfigurationFactory::GetInstance();
}

bool AppletTradeServer::upAttributeMetaInfo(const string& command, const string& params, string& result)
{
    DEBUGLOG("AppletGoodsManageServer::upAttributeMetaInfo|admin cmd req|" << command << "|" << params << endl);
    if (params != "db_goods_data.t_attribute_meta_info")
    {
        return false;
    }

    int ret = AttributeMetaManagerSingleton->upAttributeList();
    if (ret == 0)
    {
        DEBUGLOG("AppletGoodsManageServer::upAttributeMetaInfo|admin cmd suc|" << params << endl);
    }
    else
    {
        ERRORLOG("AppletGoodsManageServer::upAttributeMetaInfo|admin cmd err|" << ret << endl);
    }
    return false;
}

bool AppletTradeServer::upAttrValueMetaInfo(const string& command, const string& params, string& result)
{
    DEBUGLOG("AppletGoodsManageServer::upAttrValueMetaInfo|admin cmd req|" << command << "|" << params << endl);
    if (params != "db_goods_data.t_attribute_value_meta_info")
    {
        return false;
    }

    int ret = AttrValueMetaManagerSingleton->upAttrValueList();
    if (ret == 0)
    {
        DEBUGLOG("AppletGoodsManageServer::upAttrValueMetaInfo|admin cmd suc|" << params << endl);
    }
    else
    {
        ERRORLOG("AppletGoodsManageServer::upAttrValueMetaInfo|admin cmd err|" << ret << endl);
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



