/*********************************************************************************
 FileName: 
 Author: xufubo
 Date:  2019-01-01
 Description:
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/
#include "AppletGoodsManageServer.h"
#include "MetaManager.h"

AppletGoodsManageServer g_app;

void AppletGoodsManageServer::initialize()
{
	srand(TNOW);
	addConfig(ServerConfig::ServerName + ".conf");
	addServant<AppletGoodsManageImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".AppletGoodsManageObj");
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

    {
        // 品牌元数据
        BrandMetaManagerSingleton->initialize();
        AsyncUpdateBrandMetaInfoTask upMetaInfo;
        TC_Functor<void, TL::TLMaker<>::Result> cmd(upMetaInfo);
        TC_FunctorWrapper<TC_Functor<void, TL::TLMaker<>::Result> > wcmd(cmd);
        SUBMIT_ASYNC_TASK(wcmd);
    }

    {
        // 厂商元数据
        MakerMetaManagerSingleton->initialize();
        AsyncUpdateMakerMetaInfoTask upMetaInfo;
        TC_Functor<void, TL::TLMaker<>::Result> cmd(upMetaInfo);
        TC_FunctorWrapper<TC_Functor<void, TL::TLMaker<>::Result> > wcmd(cmd);
        SUBMIT_ASYNC_TASK(wcmd);
    }


    TARS_ADD_ADMIN_CMD_NORMAL("upCategoryMetaInfo", AppletGoodsManageServer::upCategoryMetaInfo);
    TARS_ADD_ADMIN_CMD_NORMAL("upAttributeMetaInfo", AppletGoodsManageServer::upAttributeMetaInfo);
    TARS_ADD_ADMIN_CMD_NORMAL("upAttrValueMetaInfo", AppletGoodsManageServer::upAttrValueMetaInfo);
    TARS_ADD_ADMIN_CMD_NORMAL("upBrandMetaInfo", AppletGoodsManageServer::upBrandMetaInfo);
    TARS_ADD_ADMIN_CMD_NORMAL("upMakerMetaInfo", AppletGoodsManageServer::upMakerMetaInfo);
}

void AppletGoodsManageServer::destroyApp()
{
	async_executor.stop();
	delete CategoryMetaManagerSingleton;
    delete AttributeMetaManagerSingleton;
    delete AttrValueMetaManagerSingleton;
    delete BrandMetaManagerSingleton;
    delete MakerMetaManagerSingleton;
	delete ConfigurationFactory::GetInstance();
}

bool AppletGoodsManageServer::upCategoryMetaInfo(const string& command, const string& params, string& result)
{
    DEBUGLOG("AppletGoodsManageServer::upCategoryMetaInfo|admin cmd req|" << command << "|" << params << endl);
    if (params != "db_goods_data.t_category_meta_info")
    {
        return false;
    }

    int ret = CategoryMetaManagerSingleton->upCategoryList();
    if (ret == 0)
    {
        DEBUGLOG("AppletGoodsManageServer::upCategoryMetaInfo|admin cmd suc|" << params << endl);
    }
    else
    {
        ERRORLOG("AppletGoodsManageServer::upCategoryMetaInfo|admin cmd err|" << ret << endl);
    }
    return false;
}

bool AppletGoodsManageServer::upAttributeMetaInfo(const string& command, const string& params, string& result)
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

bool AppletGoodsManageServer::upAttrValueMetaInfo(const string& command, const string& params, string& result)
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

bool AppletGoodsManageServer::upBrandMetaInfo(const string& command, const string& params, string& result)
{
    DEBUGLOG("AppletGoodsManageServer::upBrandMetaInfo|admin cmd req|" << command << "|" << params << endl);
    if (params != "db_goods_data.t_brand_info")
    {
        return false;
    }

    int ret = BrandMetaManagerSingleton->upBrandList();
    if (ret == 0)
    {
        DEBUGLOG("AppletGoodsManageServer::upBrandMetaInfo|admin cmd suc|" << params << endl);
    }
    else
    {
        ERRORLOG("AppletGoodsManageServer::upBrandMetaInfo|admin cmd err|" << ret << endl);
    }
    return false;
}

bool AppletGoodsManageServer::upMakerMetaInfo(const string& command, const string& params, string& result)
{
    DEBUGLOG("AppletGoodsManageServer::upMakerMetaInfo|admin cmd req|" << command << "|" << params << endl);
    if (params != "db_goods_data.t_maker_info")
    {
        return false;
    }

    int ret = MakerMetaManagerSingleton->upMakerList();
    if (ret == 0)
    {
        DEBUGLOG("AppletGoodsManageServer::upMakerMetaInfo|admin cmd suc|" << params << endl);
    }
    else
    {
        ERRORLOG("AppletGoodsManageServer::upMakerMetaInfo|admin cmd err|" << ret << endl);
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



