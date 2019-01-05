/*********************************************************************************
 FileName: 
 Author: xufubo
 Date:  2018-12-26
 Description: 随机数
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/

#include "time_utils.h"
#include <ctime>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <string>
#include <vector>
#include "string.h"

using namespace std;
using namespace base_utils;

/*
struct tm 
{
    int tm_sec;     // 秒 – 取值区间为[0,59]
    int tm_min;     // 分 - 取值区间为[0,59]
    int tm_hour;    //时 - 取值区间为[0,23]
    int tm_mday;    // 一个月中的日期 - 取值区间为[1,31]
    int tm_mon;     // 月份（从一月开始，0代表一月） - 取值区间为[0,11]
    int tm_year;    // 年份，其值等于实际年份减去1900
    int tm_wday;    // 星期 – 取值区间为[0,6]，其中0代表星期天，1代表星期一，以此类推
    int tm_yday;    // 从每年的1月1日开始的天数 – 取值区间为[0,365]，其中0代表1月1日，1代表1月2日，以此类推
    int tm_isdst;   // 夏令时标识符，实行夏令时的时候，tm_isdst为正。不实行夏令时的进候，tm_isdst为0；不了解情况时，tm_isdst()为负。
};
*/


namespace base_utils
{
#define ONE_DAY_SECONDS (86400)
#define ONE_MINUTE_SECONDS  (60)
#define ONE_WEEK_SECONDS (604800)
#define ONE_HOUR_SECONDS (3600)

bool CTimeUtils::IsSameMinute(time_t time1, time_t time2)
{
    unsigned iDiff = time1 > time2 ? time1 - time2 : time2 - time1;

    if (iDiff > ONE_MINUTE_SECONDS)
    {//时间60s以上，一定不是同一分钟
        return false;
    }

    if (MinuteOfDay(time1) == MinuteOfDay(time2))
    {
        return true;
    }

    return false;
}

unsigned int CTimeUtils::MinuteOfDay(time_t ts)
{
    static unsigned int iDayBegin = 0;
    if (iDayBegin + ONE_DAY_SECONDS <= unsigned(ts))
    {
        iDayBegin = GetDayBeginTime(ts);//更新一天的开始时间
    }
    unsigned int iMinute = (ts - iDayBegin) / 60 + 1;
    return iMinute;
}

time_t CTimeUtils::GetDayBeginTime(time_t ts)
{
    struct tm tm_time;
    localtime_r(&ts, &tm_time);

    tm_time.tm_sec = 0;
    tm_time.tm_min = 0;
    tm_time.tm_hour = 0;
    return mktime(&tm_time);
}

bool CTimeUtils::IsSameWeek(time_t time1, time_t time2, bool isFromSunDay)
{
    struct tm tm1, tm2;
    localtime_r(&time1, &tm1);
    localtime_r(&time2, &tm2);

    char buf1[10] = {0};
    char buf2[10] = {0};

    std::string sFormat;
    if(isFromSunDay)
    {
        sFormat = "%U";
    }
    else
    {
        sFormat = "%W";
    }

    strftime(buf1, sizeof(buf1), sFormat.c_str(), &tm1);
    strftime(buf2, sizeof(buf2), sFormat.c_str(), &tm2);

    if(0 == strncmp(buf1, buf2, strlen(buf1)) )
    {
        return true;
    }

    return false;
}

bool CTimeUtils::IsSameMonth(time_t time1, time_t time2)
{
    if (GetYear(time1) != GetYear(time2))
    {
        return false;
    }

    if(GetMonth(time1) != GetMonth(time2))
    {
        return false;
    }

    return true;
}

bool CTimeUtils::IsSameDay(time_t time1, time_t time2)
{
    if (GetDayBeginTime((time_t)time1) == GetDayBeginTime((time_t)time2))
    {
        return true;
    }

    return false;

}


char* CTimeUtils::GetMinuteFormat(void)
{
    time_t now  = time(NULL);
    struct tm data;
    localtime_r(&now, &data);

    static char format_data[64] = {0};
    snprintf(format_data, sizeof(format_data), "%04u%02u%02u_%02u%02u",
        data.tm_year+1900, data.tm_mon+1, data.tm_mday, data.tm_hour, data.tm_min);

    return format_data;
}


unsigned CTimeUtils::GetMonth(time_t time)
{
    time_t v = (time_t)time;
    struct tm data;
    localtime_r(&v, &data);
    return data.tm_mon+1;
}

unsigned CTimeUtils::GetYear(time_t time)
{
    time_t v = (time_t)time;
    struct tm data;
    localtime_r(&v, &data);
    return data.tm_year+1900;
}

char* CTimeUtils::GetYearMonth(time_t time)
{
    struct tm data;
    localtime_r(&time, &data);

    static char format_data[64] = {0};
    snprintf(format_data, sizeof(format_data), "%04u-%02u", data.tm_year+1900, data.tm_mon+1);
    return format_data;
}

char* CTimeUtils::GetYearMonthDay(time_t time)
{
    struct tm data;
    localtime_r(&time, &data);
    static char format_data[64] = {0};
    snprintf(format_data, sizeof(format_data), "%04u%02u%02u", data.tm_year+1900, data.tm_mon+1, data.tm_mday);
    return format_data;
}

char* CTimeUtils::GetMonthDay(time_t time)
{
    struct tm data;
    localtime_r(&time, &data);
    static char format_data[64] = {0};
    snprintf(format_data, sizeof(format_data), "%02u月%02u日", data.tm_mon+1, data.tm_mday);
    return format_data;
}


uint64_t CTimeUtils::GetCurrentMSTime(void)
{
    struct timeval now;
    gettimeofday( &now, NULL);
    return GetMSTime(now.tv_sec, now.tv_usec);
}

uint64_t CTimeUtils::GetMSTime(unsigned sec, unsigned usec)
{
    uint64_t ms_time = sec;
    ms_time *= 1000;
    ms_time += (usec / 1000);
    return ms_time;
}

uint64_t CTimeUtils::GetCurrentUSTime(void)
{
    struct timeval now;
    gettimeofday( &now, NULL);
    return GetUSTime(now.tv_sec, now.tv_usec);
}

uint64_t CTimeUtils::GetUSTime(unsigned sec, unsigned usec)
{
    uint64_t us_time = sec;
    us_time *= 1000000;
    us_time += usec;
    return us_time;
}

char* CTimeUtils::GetTenpayTimeFormat(time_t time)
{
    struct tm data;
    localtime_r(&time, &data);

    static char format_data[64] = {0};
    snprintf(format_data, sizeof(format_data), "%04u%02u%02u%02u%02u%02u",
        data.tm_year+1900, data.tm_mon+1, data.tm_mday, data.tm_hour, data.tm_min, data.tm_sec);
    return format_data;
}

time_t CTimeUtils::GetTenpayTimeSec(const char *time_str)
{
    tm tm1;
    strptime((char*)time_str,"%Y%m%d%H%M%S",&tm1);
    return mktime(&tm1);
}


int CTimeUtils::GetDiffDays(time_t uiFirTime, time_t uiSecTime)
{
    unsigned int uiFirBeginTs = GetDayBeginTime(uiFirTime);
    unsigned int uiSecBeginTs = GetDayBeginTime(uiSecTime);

    return (uiSecBeginTs - uiFirBeginTs) / (24 * 3600);
}

int CTimeUtils::GetDiffMonth(time_t time1, time_t time2)
{
    struct tm data;
    localtime_r(&time1, &data);
    int year1  = data.tm_year;
    int month1 = data.tm_mon;
    localtime_r(&time2, &data);
    int year2  = data.tm_year;
    int month2 = data.tm_mon;
    int month_diff = 12 * (year1 -year2) + month1 - month2;
    return month_diff;
}


/* uint32_t CTimeUtils::GetCurrentMonthDayCount(const std::string& ts, const char* format) */
/* { */
/*     uint32_t year  = 0; */
/*     uint32_t month = 0; */

/*     int nSucc = sscanf(ts.c_str(), format, &year, &month); */
/*     if (nSucc < 2) */
/*     { */
/*         return 0; */
/*     } */
/*     return GetCurrentMonthDayCount(year, month); */
/* } */

uint32_t CTimeUtils::GetCurrentMonthDayCount(uint32_t year, uint32_t month)
{
    uint32_t day_count = 0;
    if( 1==month || 3==month || 5==month || 7==month ||
            8==month || 10==month || 12==month)
    {
        day_count = 31;
    }
    else if ( 4 == month || 6==month || 9==month || 11==month)
    {
        day_count = 30;
    }
    else if (2 == month)
    {
        day_count = 28;
        if ( (0 == year%4 && 0 != year%100) || 0 == year%400)
        {
            day_count = 29;
        }
    }
    return day_count;
}

uint32_t CTimeUtils::GetCurrentMonthDayCount(time_t sec)
{
    struct tm data;
    localtime_r(&sec, &data);
    uint32_t year  = data.tm_year + 1900;
    uint32_t month = data.tm_mon + 1;
    return GetCurrentMonthDayCount(year, month);
}

void CTimeUtils::ChangeTmMonth(struct tm &tm_now, int change)
{
    int year  = tm_now.tm_year;
    int month = tm_now.tm_mon + change;
    while (month < 0)
    {
        year -= 1;
        month += 12;
    }


    while(month > 11)
    {
        year += 1;
        month -= 12;
    }

    tm_now.tm_year = year;
    tm_now.tm_mon  = month;
}

//判断从1970-01-01 到目标时间过来多少天
int64_t CTimeUtils::GetHasPassedDay(int64_t time_stamp, int64_t start_ts)
{
   int64_t diff_sec = time_stamp - start_ts;
   diff_sec += 8 * ONE_HOUR_SECONDS; //修正东八区时间
   return (diff_sec/ONE_DAY_SECONDS); 
}

//判断从1970-01-01 到目标时间过了多少个星期
int64_t CTimeUtils::GetHasPassedWeek(int64_t time_stamp, int64_t start_ts)
{
    int64_t diff_sec = time_stamp - start_ts;
    diff_sec += 8 * ONE_HOUR_SECONDS;
    diff_sec += 3 * ONE_DAY_SECONDS; // 1970-1-1 是星期四， 所以把这一周作为第一周(增加三天)
    return (diff_sec/ONE_WEEK_SECONDS); 
}



int GetDayInterval(time_t tFrom, time_t tTo)
{
    return ((int)GetDayBeginTime(tTo) - (int)GetDayBeginTime(tFrom)) / (60 * 60 * 24);
}

time_t GetDayBeginTime(time_t tTime)
{
    tm tTm = *localtime(&tTime);
    tTm.tm_hour = 0;
    tTm.tm_min  = 0;
    tTm.tm_sec  = 0;
    return MakeTime(tTm);
}

int GetHourInterval(time_t tFrom, time_t tTo)
{
    return ((int)GetHourBeginTime(tTo) - (int)GetHourBeginTime(tFrom)) / (60 * 60);
}

time_t GetHourBeginTime(time_t tTime)
{
    tm tTm = *localtime(&tTime);
    tTm.tm_min  = 0;
    tTm.tm_sec  = 0;
    return MakeTime(tTm);
}

time_t GetFirstMondayOfMonth(time_t tTime)
{
    time_t tMonthBegin =  GetMonthBeginTime( tTime) ;

    tm tTm = *localtime(&tMonthBegin);

    if(tTm.tm_wday !=  1 )
    {
        tTm.tm_mday =  (1+7 - tTm.tm_wday)%7 +1;
    }

    tTm.tm_hour = 0;
    tTm.tm_min  = 0;
    tTm.tm_sec  = 0;

    return MakeTime(tTm);
}

time_t GetWeekBeginTime(time_t tTime)
{
    tm tTm = *localtime(&tTime);
    tTm.tm_hour = 0;
    tTm.tm_min  = 0;
    tTm.tm_sec  = 0;
    return (MakeTime(tTm) - tTm.tm_wday*HD_SECONDS_OF_DAY);
}

time_t GetMonthBeginTime(time_t tTime)
{
    tm tTm = *localtime(&tTime);
    tTm.tm_hour = 0;
    tTm.tm_min  = 0;
    tTm.tm_sec  = 0;
    tTm.tm_mday = 1 ;
    return MakeTime(tTm);
}

time_t GetYearBeginTime(time_t tTime)
{
    tm tTm = *localtime(&tTime);
    tTm.tm_hour = 0;
    tTm.tm_min  = 0;
    tTm.tm_sec  = 0;
    tTm.tm_mday = 1 ;
    tTm.tm_mon = 0;
    return MakeTime(tTm);
}

const std::string FormatTime(const std::string& sFmt, const tm& stTime)
{
    char buf[255]={};
    strftime(buf, sizeof(buf)-1, sFmt.c_str(), &stTime);
    return buf;
}

const std::string FormatTime(const std::string& sFmt, time_t tTime)
{
    char buf[255]={};
    strftime(buf, sizeof(buf)-1, sFmt.c_str(), localtime(&tTime));
    return buf;
}

time_t MakeTime(tm& stTime)
{
    return mktime(&stTime);
}

time_t MakeTime(int iYear, int iMon, int iDay, int iHour,
                int iMin, int iSec)
{
    tm stTime;
    stTime.tm_year  = iYear-1900;
    stTime.tm_mon   = iMon-1;
    stTime.tm_mday  = iDay;
    stTime.tm_hour  = iHour;
    stTime.tm_min   = iMin;
    stTime.tm_sec   = iSec;

    return mktime(&stTime);
}


int GetDate(int& iYear, int& iMon, int& iDay, int iTime)
{
    if (iTime < 0)
    {
        iTime = time(NULL);
    }
    time_t t = time_t(iTime);
    tm* ptm = localtime(&t);

    iYear   = ptm->tm_year+1900;
    iMon    = ptm->tm_mon+1;
    iDay    = ptm->tm_mday;

    return iTime;
}

int GetCurrentYear( void )
{
    int iYear = 0;
    time_t iTime = time(NULL);

    tm* ptm = localtime(&iTime);
    iYear   = ptm->tm_year+1900;

    return iYear;
}

int GetCurrentMonth( void )
{
    int iMonth = 0;
    time_t iTime = time(NULL);
    tm* ptm = localtime(&iTime);
    iMonth = ptm->tm_mon+1;
    return iMonth;
}

int GetCurrentWeek( void )
{
    int iWeek = 0;
    time_t iTime = time(NULL);
    tm* ptm = localtime(&iTime);
    iWeek = ptm->tm_wday;
    return iWeek;
}

int CheckDate(int& iYear, int& iMonth, int& iDay)
{
    static int s_nDays[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    int iRetCode = 0;
    if (iMonth < 1)
    {
        iMonth = 1;
        iRetCode = 0x02;
    }
    else if (iMonth > 12)
    {
        iMonth = 12;
        iRetCode = 0x02;
    }
    else if (iMonth == 2)
    {
        if (iDay < 1)
        {
            iDay = 1;
            iRetCode |= 0x01;
            return iRetCode;
        }
        int nDays = ((iYear%400==0) || (iYear%100!=0 && iYear%4==0)) ? 29 : 28;
        if (iDay > nDays)
        {
            iDay = nDays;
            iRetCode |= 0x01;
            return iRetCode;
        }
        return iRetCode;
    }
    if (iDay < 1)
    {
        iDay = 1;
        iRetCode |= 0x01;
    }
    else if (iDay > s_nDays[iMonth-1])
    {
        iDay = s_nDays[iMonth-1];
        iRetCode |= 0x01;
    }

    return iRetCode;
}

std::string UTCTime(time_t tTime)
{
    char buf[255]={};
    strftime(buf, sizeof(buf)-1, "%a, %d %b %Y %H:%M:%S UTC", gmtime(&tTime));
    return buf;
}

timeval GetTimeOfDay()
{
    timeval tv;
    gettimeofday(&tv, 0);
    return tv;
}

time_t GetUSInterval(const timeval& tvfrom, const timeval& tvto)
{
    return (tvto.tv_sec-tvfrom.tv_sec)*1000000+(tvto.tv_usec-tvfrom.tv_usec);
}

time_t GetMSInterval(const timeval& tvfrom, const timeval& tvto)
{
    return (tvto.tv_sec-tvfrom.tv_sec)*1000+(tvto.tv_usec-tvfrom.tv_usec)/1000;
}

int64_t GetCurrentuTime()
{
    uint64_t time=0;
    struct timeval now;
    gettimeofday( &now , NULL );
    time = now.tv_sec;
    time = time*1000000;
    time += now.tv_usec;
    return time;
}

time_t GetTodayZero()
{
    time_t timep;
    struct tm *p;
    time(&timep);
    p=localtime(&timep); /*取得当地时间*/
    p->tm_sec=0;
    p->tm_min=0;
    p->tm_hour=0;
    return mktime(p);
}

int64_t GetCurrentMSTime()
{
    return GetCurrentuTime()/1000;
}


bool IsDiffDay( time_t t1, time_t t2 )
{
    // 中国是东八区，比格林威治时间早8小时
    t1 = ( t1 + 8*3600 ) / 86400;
    t2 = ( t2 + 8*3600 ) / 86400;
    return t1 != t2;
}

bool IsDiffHour( time_t t1, time_t t2 )
{
    if (IsDiffDay(t1, t2))
    {
        return true;
    }

    t1 = (t1%86400)/3600;
    t2 = (t2%86400)/3600;

    return t1 != t2;
}

bool IsSameDay( time_t t1, time_t t2 )
{
    // 中国是东八区，比格林威治时间早8小时
    t1 = ( t1 + 8*3600 ) / 86400;
    t2 = ( t2 + 8*3600 ) / 86400;
    return t1 == t2;
}

bool IsSameHour( time_t t1, time_t t2 )
{
    if (IsDiffDay(t1, t2))
    {
        return true;
    }

    t1 = (t1%86400)/3600;
    t2 = (t2%86400)/3600;

    return t1 == t2;
}

bool IsSameWeek( time_t t1, time_t t2 )
{
    int64_t iDays = 0;
    int64_t iBeforSeconds = 0;
    int64_t iAffterSeconds = 0;
    tm tTm1 = *localtime(&t1);

    if (tTm1.tm_wday == 0)
    {
        iDays = 6;
    }
    else
    {
        iDays = tTm1.tm_wday - 1;
    }

    iBeforSeconds = iDays * 24 * 3600 + tTm1.tm_hour * 3600 + tTm1.tm_min * 60 + tTm1.tm_sec;
    iAffterSeconds = 7 * 24 * 3600 - iBeforSeconds;

    if ((static_cast<int64_t>(t2) >= static_cast<int64_t>(t1) - iBeforSeconds)
        && (static_cast<int64_t>(t2) < static_cast<int64_t>(t1) + iAffterSeconds))
    {
        return true;
    }

    return false;
}

bool IsSameMonth( time_t t1, time_t t2 )
{
    tm tTm1 = *localtime(&t1);
    tm tTm2 = *localtime(&t2);

    return (tTm1.tm_year == tTm2.tm_year
        && tTm1.tm_mon == tTm2.tm_mon);
}

bool IsSameYear( time_t t1, time_t t2 )
{
    tm tTm1 = *localtime(&t1);
    tm tTm2 = *localtime(&t2);

    return (tTm1.tm_year == tTm2.tm_year);
}

time_t MakeTime(tm& stTime)
{
    return mktime(&stTime);
}

std::string TimeLengthToMMSS(int time_length)
{
    int minute = time_length / 60;
    int second = time_length % 60;

    char buf[10] = {0};
    snprintf(buf, sizeof(buf), "%02d:%02d", minute, second);

    return std::string(buf);
}

std::string TimeLengthToHHMMSS(int time_length)
{
    int hour = time_length / 3600;
    int minute = (time_length / 60) % 60;
    int second = time_length % 60;

    char buf[10] = {0};
    snprintf(buf, sizeof(buf), "%02d:%02d:%02d", hour, minute, second);

    return std::string(buf);
}

std::string TimeLengthToStr(int time_length)
{
    if (time_length < 3600)
    {
        return TimeLengthToMMSS(time_length);
    }
    return TimeLengthToHHMMSS(time_length);
}

std::pair<time_t, time_t> GetDayBeginEnd(time_t tm_now)
{
    time_t now = tm_now;
    struct tm * timeInfo_now = localtime(&now);
    time_t day_begin, day_end;
    day_begin = now - timeInfo_now->tm_sec - timeInfo_now->tm_min * 60 - timeInfo_now->tm_hour * 3600;
    day_end = day_begin + 86400;
    return std::make_pair(day_begin, day_end);
}




}

