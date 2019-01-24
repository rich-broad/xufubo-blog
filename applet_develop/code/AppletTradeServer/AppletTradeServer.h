/*********************************************************************************
 FileName: 
 Author: xufubo
 Date:  2019-01-01
 Description:
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/
#ifndef AppletTradeServer_H__
#define AppletTradeServer_H__

#include "Global.h"
#include "servant/Application.h"
#include "Configuration.h"
#include "AppletTradeImp.h"


class AppletTradeServer : public Application
{
public:
	/**
	 *
	 **/
	virtual ~AppletTradeServer() {}

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

};

extern AppletTradeServer g_app;
#define SUBMIT_ASYNC_TASK(tf) g_app.async_executor.exec(tf)

#endif

