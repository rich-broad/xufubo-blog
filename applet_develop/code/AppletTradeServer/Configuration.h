/*********************************************************************************
 FileName: 
 Author: xufubo
 Date:  2019-01-01
 Description:
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/
#ifndef Configuration_H
#define Configuration_H

#include "Global.h"

#define CM_CONFIG_FILE string(ServerConfig::ServerName + ".conf")

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

	// ��װ��taf�Ľӿ�, ����ʱ���账���쳣
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


class CmConfiguration: public Configuration
{
public:
	CmConfiguration(string filename) : Configuration(filename)
	{
	}
	virtual ~CmConfiguration() {}
	virtual void Load();
	virtual Configuration* Clone()
	{
		return new CmConfiguration(_sFileName);
	}

public:

	// obj
	string _sAppInfoProxyObj;
	int _timeOut;

	//�첽�߳���
	int _dwAsyncThreadCount;

    // ���ػ������ʱ��
    int _cacheUpTime;

	//db����
	map<string, string> _dbConfAndroidApp; 
	
	//DCACHE	
	string _dcacheProxyObj;
	int _dcacheTimeOut;

    string _commonConfigDcacheName;

    //ServerConf
    int _printLocal;
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
		_mpFileConfigs[CM_CONFIG_FILE] = new CmConfiguration(ServerConfig::ServerName + ".conf");
		Load();
	}
	virtual ~ConfigurationFactory();

	static ConfigurationFactory* GetInstance();

	// ���������ļ�
	void Load();

	// ͨ�������������¼�������
	void Reload(const string& filename = CM_CONFIG_FILE);

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

	inline CmConfiguration* GetCmConfiguration()
	{
		return GetConfiguration<CmConfiguration>(CM_CONFIG_FILE);
	}
};

#define DEF_CFG_SINGLETON ConfigurationFactory::GetInstance()->GetCmConfiguration()

#endif

