/*********************************************************************************
 FileName: AppletAuthImp.h
 Author: xufubo
 Date:  2018-12-29
 Description:
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/

#ifndef _AppletAuthImp_H__
#define _AppletAuthImp_H__

#include "AppletAuth.h"
#include "AppletAuthServer.h"
#include "Commands/AuthCommand.h"


using namespace tars;
using namespace std;

class AppletAuthImp : public HardwareApplet::AppletAuth
{
public:
	AppletAuthImp()
	{
		_pConfigurationFactory = ConfigurationFactory::GetInstance();
	}
	virtual ~AppletAuthImp()
	{
		delete _pConfigurationFactory;
	}
	virtual void initialize();
    virtual void destroy();

public:

	virtual tars::Int32 getNewTicket(const HardwareApplet::AppletCommHead& stHead,const vector<tars::Char> & vtIn,vector<tars::Char> &vtOut,tars::TarsCurrentPtr current);
	virtual tars::Int32 getNewCookie(const HardwareApplet::AppletCommHead& stHead,const vector<tars::Char> & vtIn,vector<tars::Char> &vtOut,tars::TarsCurrentPtr current);

private:
	ConfigurationFactory* _pConfigurationFactory;
	AuthCommand *_pAuthCommand;
    TC_Mysql *_dbInfo;
};
#endif /*_AppletAuthImp_H__*/

