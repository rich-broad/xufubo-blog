/*********************************************************************************
 FileName: 
 Author: xufubo
 Date:  2019-01-01
 Description:
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/
#ifndef AppletGoodsManageImp_H__
#define AppletGoodsManageImp_H__

#include "AppletGoodsManage.h"
#include "AppletGoodsManageServer.h"
#include "DependencedServiceFactory.h"
#include "Commands/OpCommand.h"
#include "Commands/GoodsCommand.h"
#include "Commands/MetaDataCommand.h"

#include "Global.h"

using namespace std;
using namespace rapidjson;
using namespace tars;
using namespace HardwareApplet;

class AppletGoodsManageImp : public HardwareApplet::AppletGoodsManage
{
public:
	AppletGoodsManageImp()
	{
		_pConfigurationFactory = ConfigurationFactory::GetInstance();
		_pServiceFactory = new DependencedServiceFactory();
	}
	virtual ~AppletGoodsManageImp()
	{
		delete _pServiceFactory;
	}
	virtual void initialize();
    virtual void destroy();

public:
	virtual tars::Int32 getCategoryList(const HardwareApplet::AppletCommHead & stHead,const vector<tars::Char> & vtIn,vector<tars::Char> &vtOut,tars::TarsCurrentPtr current);
	virtual tars::Int32 addCategoryInfo(const HardwareApplet::AppletCommHead & stHead,const vector<tars::Char> & vtIn,vector<tars::Char> &vtOut,tars::TarsCurrentPtr current);

    virtual tars::Int32 getAttributeList(const HardwareApplet::AppletCommHead & stHead,const vector<tars::Char> & vtIn,vector<tars::Char> &vtOut,tars::TarsCurrentPtr current);
    virtual tars::Int32 addAttributeInfo(const HardwareApplet::AppletCommHead & stHead,const vector<tars::Char> & vtIn,vector<tars::Char> &vtOut,tars::TarsCurrentPtr current);

    virtual tars::Int32 getAttributeValueList(const HardwareApplet::AppletCommHead & stHead,const vector<tars::Char> & vtIn,vector<tars::Char> &vtOut,tars::TarsCurrentPtr current);
    virtual tars::Int32 addAttributeValueInfo(const HardwareApplet::AppletCommHead & stHead,const vector<tars::Char> & vtIn,vector<tars::Char> &vtOut,tars::TarsCurrentPtr current);

    virtual tars::Int32 getBrandList(const HardwareApplet::AppletCommHead & stHead,const vector<tars::Char> & vtIn,vector<tars::Char> &vtOut,tars::TarsCurrentPtr current);
    virtual tars::Int32 addBrandInfo(const HardwareApplet::AppletCommHead & stHead,const vector<tars::Char> & vtIn,vector<tars::Char> &vtOut,tars::TarsCurrentPtr current);

    virtual tars::Int32 getMakerList(const HardwareApplet::AppletCommHead & stHead,const vector<tars::Char> & vtIn,vector<tars::Char> &vtOut,tars::TarsCurrentPtr current);
    virtual tars::Int32 addMakerInfo(const HardwareApplet::AppletCommHead & stHead,const vector<tars::Char> & vtIn,vector<tars::Char> &vtOut,tars::TarsCurrentPtr current);

    virtual tars::Int32 getGoodsSPUList(const HardwareApplet::AppletCommHead & stHead,const vector<tars::Char> & vtIn,vector<tars::Char> &vtOut,tars::TarsCurrentPtr current);
    virtual tars::Int32 addGoodsSPUInfo(const HardwareApplet::AppletCommHead & stHead,const vector<tars::Char> & vtIn,vector<tars::Char> &vtOut,tars::TarsCurrentPtr current);


private:
	ConfigurationFactory* _pConfigurationFactory;
	DependencedServiceFactory* _pServiceFactory;
    GoodsCommand* _pGoodsCommand;
    OpCommand* _pOpCommand;
    MetaDataCommand* _pMetaDataCommand;
    TC_Mysql * _dbInfo;
};

#endif

