/*********************************************************************************
 FileName: Configuration.cpp
 Author: xufubo
 Date:  2018-12-26
 Description:
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/
#include "Configuration.h"

void DefConfiguration::Load()
{
    string filepath = ServerConfig::BasePath + _sFileName;
    try
    {
        _fileConfig->parseFile(filepath);
        DEBUGLOG("loading " << this->_sFileName << endl);

        _outdateTime = GetConfig<int64_t>("/Auth<OutdateTime>", 7200);
        _preOutedateTime = GetConfig<int64_t>("/Auth<PreOutdateTime>", 3600);
        _printLocal = GetIntConfig("/ServerConf<PrintLocal>", 1);
        _SVRKEY = GetStringConfig("/ServerConf<SVRKEY>", "");
        _SVRMD5SIGKEY = GetStringConfig("/ServerConf<SVRMD5SIGKEY>", "");
        //dbConf
        _fileConfig->getDomainMap("/applet/db_login", _dbInfoConf);
        map<string, string> mpFunc = _fileConfig->getDomainMap("/FuncMap");
        for (map<string, string>::iterator iter = mpFunc.begin(); iter != mpFunc.end(); ++iter)
        {
            _mpFunc[iter->first] = 0;
        }
        //特性
        _pParseErrPropery = Application::getCommunicator()->getStatReport()->createPropertyReport("parseErrNum", PropertyReport::sum());

        //外部服务
        map<string, string> mpCmdObj = _fileConfig->getDomainMap("/BeckendServers");
        for (map<string, string>::iterator iter = mpCmdObj.begin(); iter != mpCmdObj.end(); ++iter)
        {
            tars::ServantPrx prx = Application::getCommunicator()->stringToProxy<ServantPrx>(iter->second);
            _mpCmdServant[iter->first] = prx;
        }
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