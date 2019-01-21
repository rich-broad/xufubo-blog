/*********************************************************************************
 FileName: 
 Author: xufubo
 Date:  2019-01-01
 Description:
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/
#ifndef DependencedServiceFactory_H
#define	DependencedServiceFactory_H

#include "Global.h"
#include "Configuration.h"

class DependencedServiceFactory
{
public:
	DependencedServiceFactory(){}
	DependencedServiceFactory(ConfigurationFactory* pConfigurationFactory) : _pConfigurationFactory(pConfigurationFactory){}
	virtual ~DependencedServiceFactory()
	{
		//_dbRankBasic.disconnect();
	}
	void Initialize();
	void Initialize(ConfigurationFactory* pConfigurationFactory)
	{
		this->_pConfigurationFactory = pConfigurationFactory;
		Initialize();
	}
	

public:
	ConfigurationFactory* _pConfigurationFactory;

	//外部服务
	//AppInfoProxyPrx _pAppInfoProxyPrx;
	
	//DCache
    //DCache::ProxyPrx _pCacheProxyPrx;

	//Db
	//TC_Mysql _dbRankBasic;

	

};

#endif

