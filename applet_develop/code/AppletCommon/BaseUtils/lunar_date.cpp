/*********************************************************************************
 FileName: 
 Author: xufubo
 Date:  2018-12-26
 Description: 
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/
#include "lunar_date.h"

namespace base_utils
{

const unsigned LunarDate::LUNAR_YEARS[] = 
{
        0x06d549, 0x6ed23e, 0x075251, 0x06a546, 0x554aba, 0x054bcd, 0x02abc2, 0x355ab6, 0x056aca, 0x8b693f, // 1891-1900
        0x03a953, 0x0752c8, 0x5b253d, 0x0325d0, 0x054dc4, 0x4aab39, 0x02b54d, 0x05acc2, 0x2ba936, 0x03a94a, // 1901-1910
        0x6d92be, 0x0592d2, 0x0525c6, 0x5a55ba, 0x0156ce, 0x02b5c3, 0x25b4b7, 0x06d4cb, 0x7ec941, 0x074954, // 1911-1920
        0x0692c8, 0x5d26bc, 0x052b50, 0x015b45, 0x4adab8, 0x036a4d, 0x0754c2, 0x2f4937, 0x07494a, 0x66933e, // 1921-1930
        0x0295d1, 0x052bc6, 0x596b3a, 0x05ad4e, 0x036a44, 0x3764b8, 0x03a4cb, 0x7b49bf, 0x0549d3, 0x0295c8, // 1931-1940
        0x652dbb, 0x0556cf, 0x02b545, 0x4daab9, 0x05d24d, 0x05a4c2, 0x2d49b6, 0x054aca, 0x7a96bd, 0x029b51, // 1941-1950
        0x0556c6, 0x5ad53b, 0x02d94e, 0x06d2c3, 0x3ea538, 0x06a54c, 0x854abf, 0x054bd2, 0x02ab48, 0x655abc, // 1951-1960
        0x056acf, 0x036945, 0x4752b9, 0x0752cd, 0x032542, 0x364bb5, 0x054dc9, 0x7aad3e, 0x02b551, 0x05b4c6, // 1961-1970
        0x5ba93b, 0x05a94f, 0x0592c3, 0x4b25b7, 0x0525cb, 0x8a55bf, 0x0156d2, 0x02b6c7, 0x65b4bc, 0x06d4d0, // 1971-1980
        0x06c945, 0x4e92b9, 0x0692cd, 0xad26c2, 0x052b54, 0x015b49, 0x62dabd, 0x036ad1, 0x0754c6, 0x5f493b, // 1981-1990
        0x07494f, 0x069344, 0x352b37, 0x052bca, 0x8a6b3f, 0x01d553, 0x036ac7, 0x5b64bc, 0x03a4d0, 0x0349c5, // 1991-2000
        0x4a95b8, 0x0295cc, 0x052dc1, 0x2aad36, 0x02b549, 0x7daabd, 0x05d252, 0x05a4c7, 0x5d49ba, 0x054ace, // 2001-2010
        0x0296c3, 0x4556b7, 0x055aca, 0x9ad53f, 0x02e953, 0x06d2c8, 0x6ea53c, 0x06a550, 0x064ac5, 0x4a9739, // 2011-2020
        0x02ab4c, 0x055ac1, 0x2ad936, 0x03694a, 0x6752bd, 0x0392d1, 0x0325c6, 0x564bba, 0x0655cd, 0x02ad43, // 2021-2030
        0x356b37, 0x05b4cb, 0x7ba93f, 0x05a953, 0x0592c8, 0x6d25bc, 0x0525cf, 0x0255c4, 0x52adb8, 0x02d6cc, // 2031-2040
        0x05b541, 0x2da936, 0x06c94a, 0x7e92be, 0x0692d1, 0x052ac6, 0x5a56ba, 0x025b4e, 0x02dac2
        // 2041-2049
};

// 天干名
const char * LunarDate::LUNAR_YEAR_PART1[] = 
{
    "甲", "乙", "丙", "丁", "戊", "己", "庚", "辛", "壬", "癸"
};

// 地支名
const char * LunarDate::LUNAR_YEAR_PART2[] = 
{
    "子", "丑", "寅", "卯", "辰", "巳", "午", "未", "申", "酉", "戌", "亥"
};

// 属相名
const char * LunarDate::LUNAR_ANIMAL[] = 
{
    "鼠", "牛", "虎", "兔", "龙", "蛇", "马", "羊", "猴", "鸡", "狗", "猪"
};

// 农历月份名
const char * LunarDate::LUNAR_MONTH_NAMES[] = 
{
    "闰*", "正月", "二月", "三月", "四月", "五月", "六月", "七月", "八月", "九月", "十月", "冬月", "腊月"
};

// 农历日名
const char * LunarDate::LUNAR_DAYS_NAMES[] = 
{
    "*", "初一", "初二", "初三", "初四", "初五", "初六", "初七", "初八",
    "初九", "初十", "十一", "十二", "十三", "十四", "十五", "十六",
    "十七", "十八", "十九", "二十", "廿一", "廿二", "廿三", "廿四",
    "廿五", "廿六", "廿七", "廿八", "廿九", "三十"
};


int LunarDate::from_date(time_t ts)
{
    struct tm tm;
    localtime_r(&ts, &tm);
    int solar_year  = tm.tm_year + 1900;
    int solar_month = tm.tm_mon + 1;
    int solar_day   = tm.tm_mday;

    int lunar_year = solar_year;
    int lunar_month = 0;
    int lunar_day = 0;
    bool is_leap_year = false;

    if (solar_year < BEGIN_YEAR || solar_year > BEGIN_YEAR + NUMBER_YEAR - 1)
        return -1; // 返回无效日期

    int year_index = solar_year - BEGIN_YEAR;

    // 记录春节的公历日期
    int spring_month_in_lunar_year = (LUNAR_YEARS[year_index] & 0x60) >> 5;
    int spring_day_in_lunar_year = (LUNAR_YEARS[year_index] & 0x1f);

    // 计算今天是公历年的第几天
    int days_since_solar_year_begin = day_of_solar_year(solar_year, solar_month, solar_day);

    // 计算春节是公历年的第几天
    int days_since_solar_year_begin_to_spring_festival = day_of_solar_year(solar_year,
                        spring_month_in_lunar_year, spring_day_in_lunar_year);

    // 计算今天是农历年的第几天
    int days_since_lunar_year_begin = days_since_solar_year_begin - days_since_solar_year_begin_to_spring_festival + 1;

    // 如果今天在春节前面,重新计算daysSinceLunarYearBegin
    if (days_since_lunar_year_begin <= 0) 
    {
        // 农历年比当前公历年小1
        year_index--;
        lunar_year--;

        // 如果越界,返回无效日期
        if (year_index < 0) return -2;

        spring_month_in_lunar_year = (LUNAR_YEARS[year_index] & 0x60) >> 5;
        spring_day_in_lunar_year = (LUNAR_YEARS[year_index] & 0x1f);

        days_since_solar_year_begin_to_spring_festival = day_of_solar_year(solar_year,
                spring_month_in_lunar_year, spring_day_in_lunar_year);

        // 计算上个公历年总天数
        int total_days_in_last_solar_year = day_of_solar_year(lunar_year, 12, 31);

        // 计算这种情况下，今天是农历年的第多少天：今天是公历年的多少天+去年全年天数-去年春节是第多少天+1
        days_since_lunar_year_begin = days_since_solar_year_begin + total_days_in_last_solar_year
                    - days_since_solar_year_begin_to_spring_festival + 1;
    }

    // 计算月份和日期
    for (lunar_month = 1; lunar_month <= 13; lunar_month++) 
    {
        int days_in_month = 29;
        if (0 != ((LUNAR_YEARS[year_index] >> (6 + lunar_month)) & 0x1))
        {
            days_in_month = 30;
        }
        if (days_since_lunar_year_begin <= days_in_month)
        {
            break;
        }
        else
        {
            days_since_lunar_year_begin -= days_in_month;
        }
    }

    lunar_day = days_since_lunar_year_begin;

    // 处理闰月
    int leap_month = (LUNAR_YEARS[year_index] >> 20) & 0xf;
    if (leap_month > 0 && leap_month < lunar_month) 
    {
        lunar_month--;
        // 如果当前月是闰月,则设置闰月标记
        if (lunar_month == leap_month)
        {
            is_leap_year = true;
        }
    }

    if (leap_month > 12)
    {
        return -3;
    }


    m_year  = lunar_year;
    m_month = lunar_month;
    m_day   = lunar_day;
    m_is_leap_month = is_leap_year;
    return 0;
}


int LunarDate::get_date(int & year, int & month, int &day) const
{
        int solar_year = m_year - 1900;
        int year_index = m_year - BEGIN_YEAR;

        int spring_month_in_lunar_year = (LUNAR_YEARS[year_index] & 0x60) >> 5;
        int spring_day_in_lunar_year = (LUNAR_YEARS[year_index] & 0x1f);
        int solar_month = spring_month_in_lunar_year;
        int solar_day = spring_day_in_lunar_year;

        int distance = 0;
        int leap_month = (LUNAR_YEARS[year_index] >> 20) & 0xf;

        int m = 0; // 要计算到第几月的天数
        if (leap_month == 0 || (m_month <= leap_month && !m_is_leap_month))
        {
            m = m_month;
        }
        else
        {
            m = m_month + 1;
        }

        for (int i = 1; i < m; i++) 
        {
            // 大月30天,小月29天
            bool is_big_month = ((LUNAR_YEARS[year_index] >> (6 + i)) & 0x1) != 0 ? true : false;
            if (is_big_month)
            {
                distance += 30;
            }
            else
            {
                distance += 29;
            }
        }
        distance += m_day;
        solar_day += distance - 1;

    struct tm tm;
    memset(&tm, 0, sizeof(tm));
    tm.tm_year  = solar_year;
    tm.tm_mon   = solar_month -1;
    tm.tm_mday  = solar_day;

    time_t ts = mktime(&tm);
    localtime_r(&ts, &tm);
    year    = tm.tm_year +1900;
    month   = tm.tm_mon + 1;
    day = tm.tm_mday;
    return 0;
}

int LunarDate::day_of_solar_year(int year, int month, int day)
{
    // 为了提高效率,记录每月1日是一年中的第几天
    static const int NORMAL_YDAY[] = 
    {
        1, 32, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335
    };

    // 闰年的情况
    static const int LEAP_YDAY[] = 
    {
        1, 32, 61, 92, 122, 153, 183, 214, 245, 275, 306, 336
    };
    const int * t_year_yday_ = NORMAL_YDAY;

    // 判断是否是公历闰年
    if (year % 4 == 0) 
    {
        if (year % 100 != 0)
        {
            t_year_yday_ = LEAP_YDAY;
        }
        if (year % 400 == 0)
        {
            t_year_yday_ = LEAP_YDAY;
        }
    }
    return t_year_yday_[month - 1] + (day - 1);
}



}


#if 0
int main(int argc, const char * argv[])
{
    base_utils::LunarDate lunar;
    for (int i=1; i<argc; i++) {
        //公历转农历
        time_t ts = atoll(argv[i]);
        int ret = lunar.from_date(ts);
        if (ret) 
        {
            printf("%u from_date err %d\n", ts, ret);
            continue;
        }
        printf ("%u lunar date %d - %d -%d\n", ts, lunar.get_year(), lunar.get_month(), lunar.get_day());

        //农历转公历，先设置农历年 月 日 是否闰月
        lunar.set_lunar(lunar.get_year(), lunar.get_month(), lunar.get_day(), lunar.is_leap_month());
        int year = 0, month = 0, day = 0;
        lunar.get_date(year, month, day);
        printf("solar date %d - %d - %d\n", year, month, day);
    }
    return  0;
}

#endif
