/*********************************************************************************
 FileName: Configuration.cpp
 Author: xufubo
 Date:  2018-12-28
 Description:
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/

#include "Configuration.h"
#include "util/tc_mysql.h"

void DefConfiguration::Load()
{
	string filepath = ServerConfig::BasePath + _sFileName;
	try
	{
        _fileConfig->parseFile(filepath);
        DEBUGLOG("loading " << this->_sFileName << endl);

        _printLocal = GetIntConfig("/ServerConf<PrintLocal>", 1);
        //dbConf
        _fileConfig->getDomainMap("/applet/db_login", _dbInfoConf);
        //httptimeout
        _iHttpTimeOut = GetIntConfig("/HttpTimeOut<TimeOut>", 10000);
        // http协议会被nginx转换为https
        _wxProxyIpPort = GetStringConfig("/ServerConf<wxProxyIpPort>", "http://172.27.16.2:800");
        _SVRKEY = GetStringConfig("/ServerConf<SVRKEY>", "");
        _SVRMD5SIGKEY = GetStringConfig("/ServerConf<SVRMD5SIGKEY>", "");
        _appSecret = GetStringConfig("/ServerConf<AppSecret>", "");
        _appid = GetStringConfig("/ServerConf<appid>", "");
    }
	__CATCH_EXCEPTION_WITH__(filepath);
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


