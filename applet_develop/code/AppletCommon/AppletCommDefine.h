
#ifndef _AppletCommDefine_H_
#define _AppletCommDefine_H_

// std
#include <string>
#include <map>
#include <vector>
#include <list>
#include <cmath>
#include <algorithm>

// tars
#include "util/tc_base64.h"
#include "util/tc_tea.h"
#include "util/tc_common.h"
#include "util/tc_http.h"
#include "util/tc_mysql.h"
#include "util/tc_http_async.h"
#include "util/tc_md5.h"
#include "util/tc_thread_mutex.h"
#include "util/tc_thread_rwlock.h"
#include "util/tc_sha.h"
#include "util/tc_encoder.h"
#include "util/tc_timeprovider.h"
#include "util/tc_file.h"
#include "util/tc_cgi.h"
#include "util/tc_config.h"
#include "util/tc_functor.h"
#include "servant/Application.h"
#include "servant/TarsLogger.h"

// rapidjson
#include "rapidjson/document.h"
#include "rapidjson/allocators.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/ostreamwrapper.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/reader.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson_util.h"

// ÒµÎñµÄ
#include "TarsEnDecode.h"
#include "AppletCommon.h"

using namespace std;
using namespace tars;

#define USER_LOGIN_DATA_COLUMN_NOT_UID  "`custom_session_key`, `open_id`, `session_key`, `session_key_time`"

#define USER_LOGIN_DATA_ALL_COLUMN  "`uid`, `custom_session_key`, `open_id`, `session_key`, `session_key_time`"


#define I2S(i) TC_Common::tostr<int>(i)
#define U2S(i) TC_Common::tostr<unsigned>(i)
#define S2I(s) TC_Common::strto<int>(s)
#define S2U(s) TC_Common::strto<unsigned>(s)
#define L2S(i) TC_Common::tostr<int64_t>(i)
#define S2L(s) TC_Common::strto<int64_t>(s)



#undef POSITION
#define POSITION __FILE__ << "|" << __LINE__ << "|" << __FUNCTION__

#define ERRORLOG(info) FDLOG("default") << POSITION << "|ERROR|" << info
#define DEBUGLOG(info) FDLOG("default") << POSITION << "|DEBUG|" << info

typedef TC_LockT<TC_ThreadMutex> MutexGuard;
typedef TC_RW_RLockT<TC_ThreadRWLocker> ReadLock;
typedef TC_RW_WLockT<TC_ThreadRWLocker> WriteLock;

#define __COMMON_EXCEPTION_CATCH_EXT__(customMsg)                           \
    catch(std::exception & e)                                               \
    {                                                                       \
        ERRORLOG("exception |" << e.what() << "|" << customMsg << endl);   \
    }                                                                       \
    catch(...)                                                              \
    {                                                                       \
        ERRORLOG("exception unknow|" << customMsg << endl);                \
    }

#define  __CATCH_EXCEPTION__                                            \
    catch ( TC_Exception& e )                                           \
    {                                                                   \
    ERRORLOG("exception" << "tc|" << e.what() << endl);                \
    }                                                                   \
    catch ( exception& e )                                              \
    {                                                                   \
    ERRORLOG("exception" << "exception|" << e.what() << endl);         \
    }                                                                   \
    catch ( ... )                                                       \
    {                                                                   \
    ERRORLOG("exception" << "unkown" << endl);                         \
    }

#define  __CATCH_EXCEPTION_WITH__(info)                                            \
    catch ( TC_Exception& e )                                                      \
    {                                                                              \
    ERRORLOG("exception" << "tc|" << e.what() << "|" << info << endl);            \
    }                                                                              \
    catch ( exception& e )                                                         \
    {                                                                              \
    ERRORLOG("exception" << "exception|" << e.what() << "|" << info << endl);     \
    }                                                                              \
    catch ( ... )                                                                  \
    {                                                                              \
    ERRORLOG("exception" << "unkown|" << info << endl);                           \
    }

#define REQ_HEAD_BASE_INFO(head) \
    head.requestId << "|" << head.cmd << "|" << head.st << "|" << head.clientTimestamp << "|" << head.svrTimestamp

#define DEVICE_INFO(deviceInfo)  \
    deviceInfo.imei1 << "|" << deviceInfo.imei2 << "|" << deviceInfo.macAddr << "|" << deviceInfo.brand << "|" << deviceInfo.mode

#define ROM_INFO(romInfo)  \
    romInfo.sysId << "|" << romInfo.sysVersionName << "|" << romInfo.sysVersionCode << "|" << romInfo.rootFlag

#define NET_INFO(netInfo)  \
    netInfo.netType << "|" << netInfo.wifiSsid << "|" << netInfo.wifiBssid

#define SESSION_INFO(sessionInfo) \
    sessionInfo.uid << "|" << sessionInfo.openid
    

#define REQ_HEAD_ALL_INFO(head) \
    REQ_HEAD_BASE_INFO(head) << "|" << DEVICE_INFO(head.deviceInfo) << "|" << ROM_INFO(head.romInfo) << "|" << NET_INFO(head.netInfo)

#define RSP_HEAD_ALL_INFO(head) \
    head.requestId << "|" << head.ret << "|" << head.svrTimestamp << "|" << head.csTicketState << "|" << head.st

// ÒµÎñÍ·²¿
#define COMM_HEAD_ALL_INFO(head)    \
    DEVICE_INFO(head.deviceInfo) << "|" << ROM_INFO(head.romInfo) << "|" << NET_INFO(head.netInfo) << "|" << SESSION_INFO(head.sessionInfo) << "|" << head.clientTimestamp

#endif