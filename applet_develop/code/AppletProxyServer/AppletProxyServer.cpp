
/*********************************************************************************
 FileName: AppletProxyServer.cpp
 Author: xufubo
 Date:  2018-12-26
 Description:
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/
#include "AppletProxyServer.h"
#include "AppletProxyImp.h"

using namespace std;

AppletProxyServer g_app;

/////////////////////////////////////////////////////////////////
void AppletProxyServer::initialize()
{
    string httpObj = ServerConfig::Application + "." + ServerConfig::ServerName + ".AppletProxyObj";
    addServant<AppletProxyImp>(httpObj);
    addServantProtocol(httpObj, AppletProxyServer::parseRecv);
    addConfig(ServerConfig::ServerName + ".conf");
    
    DEF_CFG_SINGLETON;
}
/////////////////////////////////////////////////////////////////
void AppletProxyServer::destroyApp()
{
    //destroy application here:
    delete ConfigurationFactory::GetInstance();
}

int AppletProxyServer::parseRecv(string& in, string& out)
{
    DEBUGLOG(in.size() << "|" << endl);

    try
    {
        bool b = TC_HttpRequest ::checkRequest(in.c_str(), in.length());
        //完整的HTTP请求
        if(b)
        {
            out = in;
            in  = "";
            DEBUGLOG("parseRecv out size: " << out.size() << endl);
            return TC_EpollServer::PACKET_FULL;
        }
        else
        {
            return TC_EpollServer::PACKET_LESS;
        }
    }
    __CATCH_EXCEPTION__;

    return TC_EpollServer::PACKET_ERR;
}


/////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
    try
    {
        g_app.main(argc, argv);
        g_app.waitForShutdown();
    }
    catch (std::exception& e)
    {
        cerr << "std::exception:" << e.what() << std::endl;
    }
    catch (...)
    {
        cerr << "unknown exception." << std::endl;
    }
    return -1;
}
/////////////////////////////////////////////////////////////////
