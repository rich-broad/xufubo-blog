
#ifndef _AppletTransaction_H__
#define _AppletTransaction_H__

#include "AppletCommDefine.h"

class AppletTransaction
{
public:
    AppletTransaction(TC_Mysql *mysql):_mysql(mysql){}

    ~AppletTransaction(){}

    // TC_Mysql库的风格就是出现错误就抛出异常，因此我们的风格就是在try块中将ret职位0.说明就执行成功了。
    int Start()
    {
        int ret = -1;
        try
        {
            _mysql->connect();
            _mysql->execute("START TRANSACTION");
            ret = 0;
        }
        __CATCH_EXCEPTION_WITH__("AppletTransaction::Start");
        return ret;
    }

    // mysql广义的查询
    int Query(const string& sqlStr)
    {
        DEBUGLOG("sql = " << sqlStr.c_str() << endl);
        int ret = -1;
        try
        {
            _mysql->connect();
            _mysql->execute(sqlStr);
            ret = 0;
        }
        __CATCH_EXCEPTION_WITH__("AppletTransaction::Execute");
        return ret;
    }

    int Commit()
    {
        int ret = -1;
        try
        {
            _mysql->connect();
            _mysql->execute("COMMIT");
            ret = 0;
        }
        __CATCH_EXCEPTION_WITH__("AppletTransaction::Commit");
        return ret;
    }

    int Rollback()
    {
        int ret = -1;
        try
        {
            _mysql->connect();
            _mysql->execute("ROLLBACK");
            ret = 0;
        }
        __CATCH_EXCEPTION_WITH__("AppletTransaction::Rollback");
        return ret;
    }

private:
    TC_Mysql *_mysql;
};

#endif