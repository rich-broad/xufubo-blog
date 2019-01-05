/*********************************************************************************
 FileName: 
 Author: xufubo
 Date:  2018-12-26
 Description: 随机数
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/

#ifndef __TIME_UTILS_H_
#define __TIME_UTILS_H_

#include <ctime>
#include <string>
#include <stdint.h>
#include <utility>
#include <sys/time.h>
#include "base_define.h"

using namespace std;

namespace base_utils
{
    
enum 
{
    HD_SECONDS_OF_DAY = 86400 
};
class TimeUtils
{
public:

    static unsigned GetMonth(time_t time);
    static unsigned GetYear(time_t time);

    static char* GetYearMonth(time_t time);
    static char* GetMinuteFormat(void);
    static char* GetTenpayTimeFormat(time_t time);
    static time_t GetTenpayTimeSec(const char *time_str);
    static char* GetYearMonthDay(time_t time);
    static char* GetMonthDay(time_t time);

    //返回 uiFirTime 和 uiSecTime两个时间差别的天数， uiFirTime大则返回负数
    //例如 2016-11-10 和 2016-11-11 则返回1
    static int GetDiffDays(time_t uiFirTime, time_t uiSecTime);

    //获取当前时间是 当天的第几分钟 分钟从1算起
    static unsigned int MinuteOfDay(time_t ts);

    static bool IsSameMinute(time_t time1, time_t time2);

    static int  GetDiffMonth(time_t time1, time_t time2);

    //直接通过时间戳访问
    static uint32_t GetCurrentMonthDayCount(time_t sec);
    
    //直接通过年月访问
    static uint32_t GetCurrentMonthDayCount(uint32_t year, uint32_t month);

    //输入时间字符串格式是 YYMM 仅仅通过年月来控制
    //static uint32_t GetCurrentMonthDayCount(const std::string& ts, const char* format="%04u%02u");

    //修改tm的月信息,并且正确格式化
    static void ChangeTmMonth(struct tm &tm_now, int change);
    
    //判断从1970-01-01 到目标时间过来多少天
    static int64_t GetHasPassedDay(int64_t time_stamp, int64_t start_ts = 0);

    //判断从1970-01-01 到目标时间过了多少个星期
    static int64_t GetHasPassedWeek(int64_t time_stamp, int64_t start_ts = 0); 

    static bool IsSameDay( time_t t1, time_t t2 );
    static bool IsSameHour( time_t t1, time_t t2 );
    
    static bool IsSameWeek( time_t t1, time_t t2, bool isFromSunDay);
    static bool IsSameMonth( time_t t1, time_t t2 );
    static bool IsSameYear( time_t t1, time_t t2 );
    
    //将时间长度转换为HH:MM:SS或者MM:SS字符串
    static std::string TimeLengthToStr(int time_length);
    static std::string TimeLengthToMMSS(int time_length);
    static std::string TimeLengthToHHMMSS(int time_length);
    
    static std::pair<time_t, time_t> GetDayBeginEnd(time_t tm_now);
    
    
    /*
     * brief 获取某时间点之间的天数
     * 如 2005-01-10 12:00:00 -> 2005-01-11: 06:00:00 = 1天
     * param[in] tFrom 时间起点
     * param[in] tTo   时间终点
     * return int [ 时间天数差, 分正负 ]
     * remark 以本地时间为准进行处理
     */
    static int GetDayInterval(time_t tFrom,time_t tTo);
    
    /*
     * brief 获取某时间点之间的小时数
     * 如 2005-01-11 02:04:00 -> 2005-01-11: 06:00:00 = 4小时
     * param[in] tFrom 时间起点
     * param[in] tTo   时间终点
     * return int [ 时间小时数差, 分正负 ]
     * remark 以本地时间为准进行处理
     */
    static int GetHourInterval(time_t tFrom,time_t tTo);
    
    /*
     * brief 获取当前年份
     * return 年份
     * remark 以本地时间为准进行处理
     */
    static int GetCurrentYear( void );
    
    /*
     * brief 获取当前月份
     * return 月份(1,2,3,4,5,6,7,8,9,10,11,12)
     * remark 以本地时间为准进行处理
     */
    static int GetCurrentMonth( void );
    
    /*
     * brief 获取当前星期几
     * return 星期几(0,1,2,3,4,5,6)
     * remark 以本地时间为准进行处理
     */
    static int GetCurrentWeek( void );
    
    /*
     * brief 获取系统微秒数
     * return int [ 当前微妙 ]
     */
    static int64_t GetCurrentuTime();
    
    /*
     * brief 获取系统毫秒数
     * return int [ 当前毫秒 ]
     */
    static int64_t GetCurrentMSTime(void);
    
    /*
     * brief 返回时间点所以小时开始时间点:
     * 如 2005-01-10 12:10:29 -> 2005-01-10 12:00:00
     * param[in] tTime 时间点
     * return int [ 返回一个小时开始时间 ]
     * remark 以本地时间为准进行处理
     */
    static time_t GetHourBeginTime(time_t tTime);
    
    /*
     * brief 返回时间点所以日开始时间点:
     * 如 2005-01-10 12:10:29 -> 2005-01-10 00:00:00
     * param[in] tTime 时间点
     * return int [ 返回一天开始时间 ]
     * remark 以本地时间为准进行处理
     */
    static time_t GetDayBeginTime(time_t tTime);
    
    /*
     * brief 返回时间点所在月的第1个周一的开始时间 
     * param[in] tTIme: 时间点
     * return int [ 返回第1个周一的开始时间  ]
     * remark 以本地时间为准进行处理
     */
    static time_t GetFirstMondayOfMonth(time_t tTime);
    
    /*
     * brief 返回时间点所在周的开始时间 
     * param[in] tTIme: 时间点
     * return int [ 返回一周开始时间 ]
     * remark 以本地时间为准进行处理
     */
    static time_t GetWeekBeginTime(time_t tTime);
    
    /*
     * brief 返回时间点所以日开始时间点:
     * 如 2005-01-10 12:10:29 -> 2005-01-01 00:00:00
     * param[in] tTime 时间点
     * return int [ 返回一个月的始时间 ]
     * remark 以本地时间为准进行处理	
     */
    
    static time_t GetMonthBeginTime(time_t tTime) ; 
    
    /*
     * brief 返回时间点所在年开始时间点: 如 2005-09-10 12:10:29 -> 2005-01-01 00:00:00
     * param[in] tTIme: 时间点
     * return int [ 返回一年的开始时间 ]
     * remark 以本地时间为准进行处理	
     */
    static time_t GetYearBeginTime(time_t tTime) ; 
    
    
    /*
     * brief 格式化时间
     */
    static const std::string FormatTime(const std::string& sFmt, const tm& stTime);
    
    
    /*
     * brief 格式化时间
     * remark 以本地时间为准进行处理
     */
    static const std::string FormatTime(const std::string& sFmt,time_t tTime);
    
    
    /*
     * brief 时间结构转换
     */
    static time_t MakeTime(tm& stTime);
    
    
    /*
     * brief 时间结构转换
     */
    static time_t MakeTime(int iYear, int iMon, int iDay, int iHour=0, int iMin=0, int iSec=0);
    
    /*
     * brief 返回时间代表的年月日
     * param[out] iYear  年
     * param[out] iMon   月
     * param[out] iDay   日
     * param[in]  iTime  时间, <0 时为当前时间
     * return int [ 返回iTime, 如 iTime<0, 返回当前时间值 ]
     * remark 以本地时间为准进行处理
     */
    static int GetDate(int& iYear, int& iMon, int& iDay, int iTime=-1);
    
    /*
     * brief 检查日期合法性并作修正
     * return int [ 0--合法 !0--不合法并作修正 ]
     */
    static int CheckDate(int& iYear, int& iMonth, int& iDay);
    
    /*
     * brief 时间格式化为UTC格式: Tue, 21 Feb 2006 02:20:04 UTC
     * param[in] tTime 时间
     * remark 返回全球标准时间而非本地时间
     */
    static std::string UTCTime(time_t tTime);
    
    /*
     * brief 取当前时间
     */
    static timeval GetTimeOfDay();
    
    /*
     * brief 计算两时间之间微秒差
     * return time_t[tvto-tvfrom]
     */
    static time_t GetUSInterval(const timeval& tvfrom, const timeval& tvto);
    
    /*
     * brief 计算两时间之间毫秒差
     * return time_t[tvto-tvfrom]
     */
    static time_t GetMSInterval(const timeval& tvfrom, const timeval& tvto);
    
    /*
     * brief 返回今天零点的时间戳
     * return time_t[tvto-tvfrom]
     */
    static time_t GetTodayZero();
    
    static bool IsDiffDay(time_t t1,time_t t2 );
    static bool IsDiffHour(time_t t1,time_t t2);

};

};

#endif
