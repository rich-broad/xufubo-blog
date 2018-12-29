/*********************************************************************************
 FileName: Configuration.h
 Author: xufubo
 Date:  2018-12-28
 Description:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "Global.h"
#include "AppletAuth.h"

#define DEF_CONFIG_FILE string(ServerConfig::ServerName + ".conf")

class Configuration
{
public:
	string _sFileName;
	TC_Config* _fileConfig;
	bool _bUpdated;

public:

	Configuration(string filename) : _sFileName(filename)
	{
		_bUpdated = false;
		_fileConfig = new TC_Config();
	}
	virtual ~Configuration() {}

	// ��������,����ʵ��
	virtual void Load() = 0;
	virtual Configuration* Clone() = 0;

	// ��װ��tars�Ľӿ�, ����ʱ���账���쳣
	template<typename T>
	T GetConfig(const string& key, const T& defaultValue)
	{
		if(key.empty()) return defaultValue;
		string v = _fileConfig->get(key); // ���׳��쳣
		if(!v.empty()) // ��Ϊ�ձ�ʾ�д�����
		{
			return TC_Common::strto<T>(v);
		}
		return defaultValue;
	}

	int32_t GetIntConfig(const string& key, int32_t defaultValue = 0)
	{
		return GetConfig<int32_t>(key, defaultValue);
	}
	string GetStringConfig(const string& key, string defaultValue = "")
	{
		return GetConfig<string>(key, defaultValue);
	}
};


class DefConfiguration: public Configuration
{
public:
	DefConfiguration(string filename) : Configuration(filename)
	{
	}
	virtual ~DefConfiguration() {}
	virtual void Load();
	virtual Configuration* Clone()
	{
		return new DefConfiguration(_sFileName);
	}

public:

	//db����
	map<string, string> _dbInfoConf;
	//http timeout
	int _iHttpTimeOut;

	//log
	int32_t _printLocal;

    string _wxProxyIpPort;
    string _SVRKEY;
    string _SVRMD5SIGKEY;
    string _appid;
    string _appSecret;
};

class ConfigurationFactory
{
protected:
	map<string, Configuration*> _mpFileConfigs;
	tars::TC_ThreadMutex _mutex;

	static ConfigurationFactory* volatile _instance;
	static tars::TC_ThreadMutex _instanceMutex;

public:

	ConfigurationFactory()
	{
		_mpFileConfigs[DEF_CONFIG_FILE] = new DefConfiguration(ServerConfig::ServerName + ".conf");
		Load();
	}
	virtual ~ConfigurationFactory();

	static ConfigurationFactory* GetInstance();

	// ���������ļ�
	void Load();

	// ͨ�������������¼�������
	void Reload(const string& filename = DEF_CONFIG_FILE);

	// ȡ������
	template<typename T>
	inline T* GetConfiguration(const string& filename)
	{
		MutexGuard mg(_mutex);
		map<string, Configuration*>::iterator it = _mpFileConfigs.find(filename);
		if ( it != _mpFileConfigs.end() )
		{
			Configuration* config = it->second;
			return dynamic_cast<T*>(config);
		}
		return NULL;
	}

	inline DefConfiguration* GetDefConfiguration()
	{
		return GetConfiguration<DefConfiguration>(DEF_CONFIG_FILE);
	}
};

#define DEF_CFG_SINGLETON ConfigurationFactory::GetInstance()->GetDefConfiguration()

#endif	/* CONFIGURATION_H */

