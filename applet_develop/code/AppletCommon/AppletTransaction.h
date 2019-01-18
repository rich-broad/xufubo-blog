
#ifndef _AppletTransaction_H__
#define _AppletTransaction_H__

#include "AppletCommDefine.h"

class AppletTransaction
{
public:
    AppletTransaction(TC_Mysql *mysql):_mysql(mysql){}

    ~AppletTransaction(){}

    // TC_Mysql��ķ����ǳ��ִ�����׳��쳣��������ǵķ�������try���н�retְλ0.˵����ִ�гɹ��ˡ�
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

    // mysql����Ĳ�ѯ
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