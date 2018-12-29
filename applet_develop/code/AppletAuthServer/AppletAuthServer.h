/*********************************************************************************
 FileName: AppletAuthServer.h
 Author: xufubo
 Date:  2018-12-28
 Description:
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/

#ifndef AppletAuthServer_H__
#define AppletAuthServer_H__

#include "servant/Application.h"
#include "Configuration.h"
#include "AppletAuthImp.h"

class AppletAuthServer : public Application
{
public:
	/**
	 *
	 **/
	virtual ~AppletAuthServer() {}

	/**
	 *
	 **/
	virtual void initialize();

	/**
	 *
	 **/
	virtual void destroyApp();

	bool updateDb(const string& command, const string& params, string& result);

	TC_HttpAsync httpAsync;
};

extern AppletAuthServer g_app;
#define SUBMIT_ASYNC_TASK(tf) g_app.async_executor.exec(tf)

#endif /*AppletAuthServer_H__*/

