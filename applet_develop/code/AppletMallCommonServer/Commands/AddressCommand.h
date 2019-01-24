/*********************************************************************************
 FileName: AddressCommand.h
 Author: xufubo
 Date:  2019-01-01
 Description:
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/

#ifndef ADDRESS_COMMAND_H_
#define ADDRESS_COMMAND_H_

#include "Command.h"
#include "Global.h"
#include "AppletMallCommon.h"

class AddressCommand : public Command
{
public:
	AddressCommand(ConfigurationFactory* configurationFactory, DependencedServiceFactory* serviceFactory, TC_Mysql * _mysql);
	virtual ~AddressCommand();

	int addMyAddressInfo(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::AddMyAddressInfoReq& stReq, HardwareApplet::AddMyAddressInfoRsp& stRsp, tars::TarsCurrentPtr current);
    int getMyAddressList(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::GetMyAddressListReq& stReq, HardwareApplet::GetMyAddressListRsp& stRsp, tars::TarsCurrentPtr current);
    int getProvinceList(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::GetProvinceListReq& stReq, HardwareApplet::GetProvinceListRsp& stRsp, tars::TarsCurrentPtr current);
    int getCityListByProvince(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::GetCityListByProvinceReq& stReq, HardwareApplet::GetCityListByProvinceRsp& stRsp, tars::TarsCurrentPtr current);
    int getCountyListByCity(const HardwareApplet::AppletCommHead& stHead, const HardwareApplet::GetCountyListByCityReq& stReq, HardwareApplet::GetCountyListByCityRsp& stRsp, tars::TarsCurrentPtr current);
	
private:
    TC_Mysql * _mysql;
};

#endif /*APP_COMMAND_H_*/
