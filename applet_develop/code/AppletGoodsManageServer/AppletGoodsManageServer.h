/*********************************************************************************
 FileName: 
 Author: xufubo
 Date:  2019-01-01
 Description:
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/
#ifndef AppletGoodsManageServer_H__
#define AppletGoodsManageServer_H__

#include "Global.h"
#include "servant/Application.h"
#include "Configuration.h"
#include "AppletGoodsManageImp.h"


class AppletGoodsManageServer : public Application
{
public:
	/**
	 *
	 **/
	virtual ~AppletGoodsManageServer() {}

	/**
	 *
	 **/
	virtual void initialize();

	/**
	 *
	 **/
	virtual void destroyApp();

	// 异步执行线程池
	TC_ThreadPool async_executor;

    bool upCategoryMetaInfo(const string& command, const string& params, string& result);
    bool upAttributeMetaInfo(const string& command, const string& params, string& result);
    bool upAttrValueMetaInfo(const string& command, const string& params, string& result);
    bool upBrandMetaInfo(const string& command, const string& params, string& result);
    bool upMakerMetaInfo(const string& command, const string& params, string& result);
    bool upWarehouseMetaInfo(const string& command, const string& params, string& result);

};

extern AppletGoodsManageServer g_app;
#define SUBMIT_ASYNC_TASK(tf) g_app.async_executor.exec(tf)

#endif

