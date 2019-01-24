/*********************************************************************************
 FileName: 
 Author: xufubo
 Date:  2019-01-01
 Description:
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/
#include "DependencedServiceFactory.h"

void DependencedServiceFactory::Initialize()
{
	try
    {

// 	_pAppInfoProxyPrx = Application::getCommunicator()->stringToProxy<AppInfoProxyPrx>(CM_CFG->_sAppInfoProxyObj);
// 	_pAppInfoProxyPrx->taf_async_timeout(CM_CFG->_timeOut);
// 	
// 
// 	TC_DBConf dbConf;
// 	dbConf.loadFromMap(CM_CFG->_dbConfRankBasic);
// 	_dbRankBasic.init(dbConf);
// 	_dbRankBasic.connect();
// 
//     _pCacheProxyPrx = Application::getCommunicator()->stringToProxy<DCache::ProxyPrx>(CM_CFG->_dcacheProxyObj);
//     _pCacheProxyPrx->taf_async_timeout(CM_CFG->_dcacheTimeOut);       
//     _appId2pkgNameCache.setModuleName(CM_CFG->_commonConfigDcacheName);       
//     _appId2pkgNameCache.initProxy(_pCacheProxyPrx);       
//     _appId2pkgNameCache.initComm(Application::getCommunicator());
    }
	__CATCH_EXCEPTION_WITH__("DependencedServiceFactory::Initialize");
}
