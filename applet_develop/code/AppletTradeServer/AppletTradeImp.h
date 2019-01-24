/*********************************************************************************
 FileName: 
 Author: xufubo
 Date:  2019-01-01
 Description:
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/
#ifndef AppletTradeImp_H__
#define AppletTradeImp_H__

#include "AppletTrade.h"
#include "AppletTradeServer.h"
#include "DependencedServiceFactory.h"
#include "Commands/OpCommand.h"
#include "Commands/AppCommand.h"

#include "Global.h"

using namespace std;
using namespace rapidjson;
using namespace tars;
using namespace HardwareApplet;

class AppletTradeImp : public HardwareApplet::AppletTrade
{
public:
	AppletTradeImp()
	{
		_pConfigurationFactory = ConfigurationFactory::GetInstance();
		_pServiceFactory = new DependencedServiceFactory();
	}
	virtual ~AppletTradeImp()
	{
		delete _pServiceFactory;
	}
	virtual void initialize();
    virtual void destroy();

public:
	//virtual tars::Int32 getAppList(const HardwareApplet::HardwareApplet & stHead,const vector<tars::Char> & vtIn,vector<tars::Char> &vtOut,tars::TarsCurrentPtr current);
	//virtual tars::Int32 getAppDetailBatch(const HardwareApplet::HardwareApplet & stHead,const vector<tars::Char> & vtIn,vector<tars::Char> &vtOut,tars::TarsCurrentPtr current);

private:
	ConfigurationFactory* _pConfigurationFactory;
	DependencedServiceFactory* _pServiceFactory;
    AppCommand* _pAppCommand;
    OpCommand* _pOpCommand;
};

#endif

