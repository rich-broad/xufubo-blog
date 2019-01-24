/*********************************************************************************
 FileName: 
 Author: xufubo
 Date:  2019-01-01
 Description:
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/
#ifndef AppletMallCommonImp_H__
#define AppletMallCommonImp_H__

#include "AppletMallCommon.h"
#include "AppletMallCommonServer.h"
#include "DependencedServiceFactory.h"
#include "Commands/OpCommand.h"
#include "Commands/AddressCommand.h"

#include "Global.h"

using namespace std;
using namespace rapidjson;
using namespace tars;
using namespace HardwareApplet;

class AppletMallCommonImp : public HardwareApplet::AppletMallCommon
{
public:
	AppletMallCommonImp()
	{
		_pConfigurationFactory = ConfigurationFactory::GetInstance();
		_pServiceFactory = new DependencedServiceFactory();
	}
	virtual ~AppletMallCommonImp()
	{
		delete _pServiceFactory;
	}
	virtual void initialize();
    virtual void destroy();

public:
	virtual tars::Int32 addMyAddressInfo(const HardwareApplet::AppletCommHead & stHead,const vector<tars::Char> & vtIn,vector<tars::Char> &vtOut,tars::TarsCurrentPtr current);
    virtual tars::Int32 getMyAddressList(const HardwareApplet::AppletCommHead & stHead,const vector<tars::Char> & vtIn,vector<tars::Char> &vtOut,tars::TarsCurrentPtr current);
    virtual tars::Int32 getProvinceList(const HardwareApplet::AppletCommHead & stHead,const vector<tars::Char> & vtIn,vector<tars::Char> &vtOut,tars::TarsCurrentPtr current);
    virtual tars::Int32 getCityListByProvince(const HardwareApplet::AppletCommHead & stHead,const vector<tars::Char> & vtIn,vector<tars::Char> &vtOut,tars::TarsCurrentPtr current);
    virtual tars::Int32 getCountyListByCity(const HardwareApplet::AppletCommHead & stHead,const vector<tars::Char> & vtIn,vector<tars::Char> &vtOut,tars::TarsCurrentPtr current);

private:
	ConfigurationFactory* _pConfigurationFactory;
	DependencedServiceFactory* _pServiceFactory;
    AddressCommand* _pAddressCommand;
    OpCommand* _pOpCommand;
    TC_Mysql * _dbInfo;
};

#endif

