/*********************************************************************************
 FileName: 
 Author: xufubo
 Date:  2019-01-01
 Description:
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/

#include "Configuration.h"
#include "util/tc_mysql.h"
#include "util/tc_encoder.h"

void CmConfiguration::Load()
{
	string filepath = ServerConfig::BasePath + _sFileName;
	try
	{
        _fileConfig->parseFile(filepath);
        DEBUGLOG("loading " << this->_sFileName << endl);

        //外部服务
        _sAppInfoProxyObj = GetStringConfig("/AppInfoProxyServer<Obj>");

        //dbConf
        _fileConfig->getDomainMap("/DbConf/android_app", _dbConfAndroidApp);

        //dcache        
        _dcacheProxyObj = GetStringConfig("/Dcache<ProxyObj>");
        _commonConfigDcacheName = GetStringConfig("/Dcache<CommonConfigDcache>", "MobileAssistCoFTCommonConfig");
        _dcacheTimeOut = GetIntConfig("/Dcache<TimeOut>", 30000);

        _dwAsyncThreadCount = GetIntConfig("/ServerConf<AsyncThreadCount>", 2);
        _cacheUpTime = GetIntConfig("/ServerConf<CacheUpTime>", 60);
        _printLocal = GetIntConfig("/ServerConf<PrintLocal>", 1);
    }
	__CATCH_EXCEPTION_WITH__("parse filepathException|" << filepath);
}

ConfigurationFactory* volatile ConfigurationFactory::_instance = NULL;
tars::TC_ThreadMutex ConfigurationFactory::_instanceMutex;

ConfigurationFactory* ConfigurationFactory::GetInstance()
{
	if( _instance == NULL )
	{
		MutexGuard mg(_instanceMutex);
		if(_instance == NULL)
		{
			ConfigurationFactory* volatile tmp = new ConfigurationFactory();
			_instance = tmp;
		}
	}
	return _instance;
}

void ConfigurationFactory::Load()
{
	MutexGuard mg(_mutex);
	map<string, Configuration*>::iterator iter = _mpFileConfigs.begin();
	for( ; iter != _mpFileConfigs.end(); ++iter )
	{
		Configuration* config = iter->second;
		config->Load();
	}
}

void ConfigurationFactory::Reload(const string& filename)
{
	MutexGuard mg(_mutex);
	map<string, Configuration*>::iterator found = _mpFileConfigs.find(filename);
	if( found != _mpFileConfigs.end() )
	{

		Configuration* config = (found->second);
		Configuration* newconfig = config->Clone();
		newconfig->Load();
		_mpFileConfigs.erase(found);
		_mpFileConfigs.insert(make_pair(config->_sFileName, newconfig));
		delete config;
	}
	else
	{
		DEBUGLOG("configuration " << filename << " not found" << endl);
		ERRORLOG("configuration " << filename << " not found" << endl);
	}
}

ConfigurationFactory::~ConfigurationFactory()
{
	MutexGuard mg(_mutex);
	map<string, Configuration*>::iterator iter = _mpFileConfigs.begin();
	for(; iter != _mpFileConfigs.end(); )
	{
		Configuration* config = iter->second;
		_mpFileConfigs.erase(iter++);
		delete config->_fileConfig;
		delete config;
	}
}
