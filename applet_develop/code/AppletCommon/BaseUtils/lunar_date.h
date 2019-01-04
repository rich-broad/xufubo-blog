/*********************************************************************************
 FileName: 
 Author: xufubo
 Date:  2018-12-26
 Description:
 Version:
 History:
     <author>        <time>          <version>          <desc>
 ********************************************************************************/

#ifndef	_LUNAR_DATE_H_
#define	_LUNAR_DATE_H_ 

#include <string>
#include <time.h>
#include <string.h>

using namespace std;

namespace base_utils
{

class LunarDate 
{
private:
	int	m_year;
	int	m_month;
	int	m_day;

	bool m_is_leap_month;
	
	static const int BEGIN_YEAR	= 1891;
	static const int NUMBER_YEAR	= 159;

	// 用比特位记录每年的情况
	// 0~4共5bit 春节日分
	// 5~6共2bit 春节月份
	// 7~19共13bit 13个月的大小情况(如无闰月,最后位无效),大月为1,小月为0
	// 20~23共4bit 记录闰月的月份,如果没有则闰月为0
	static const unsigned LUNAR_YEARS[];
	
	// 天干名
	static const char * LUNAR_YEAR_PART1[];

	// 地支名
	static const char * LUNAR_YEAR_PART2[];

	// 属相名
	static const char * LUNAR_ANIMAL[];

	// 农历月份名
	static const char * LUNAR_MONTH_NAMES[];

	// 农历日名
	static const char * LUNAR_DAYS_NAMES[];
	
	/**
	* @return 0 or leap month 1-12
	*/
	static inline int get_yearbit(int year) 
    {
		year -= BEGIN_YEAR;
		if (year < 0 || year >= NUMBER_YEAR)
        {
            return 0;
        }
		return LUNAR_YEARS[year];
	}
	
	/**
	 * @brief 计算这个公历日期是一年中的第几天
	 * @param year
	 * @param month
	 * @param day
	 * @return
	*/
	static int day_of_solar_year(int year, int month, int day);
public:

	LunarDate() 
    {	
        m_year = m_month = m_day = -1;	
        m_is_leap_month = false;	
    }
	
	int	get_year() const
    {	
        return	m_year;
    }
	int	get_month() const
    {	
        return	m_month;	
    }
	int	get_day() const	
    {	
        return	m_day;		
    }
	int	is_leap_month() const	
    {
        return	m_is_leap_month;	
    }
	
	void set_lunar(int year, int month, int day, bool is_leap) 
	{
		m_year	= year;
		m_month	= month;
		m_day	= day;
		m_is_leap_month = is_leap;
	}
	
	/**
	* @brief 将公历日期转换为农历
	* @param ts
	* @return 0 ok
	*/
	int from_date(time_t ts); 
	
	/**
	* @brief 将农历转成公历
	* @return
	*/
	int get_date(int & year, int & month, int &day) const; 
	
	
	/**
	 * @brief 获得农历月的名字
	 * @return
	 */
	inline string	lunar_month_name() const;
	
	/**
	 * @brief获得农历天的名字
	 *
	 * @return
	 */
	inline string	lunar_day_name() const;
	
	/**
	 * @brief获得农历生肖
	 * @return
	 */
	inline string animal_of_lunar_year() const;
	
	 /**
	  * @brief 生成农历年字符串
	  * @return
	*/
	inline	string lunar_year_name() const;
	
	/**
	* @brief生成汉语字符串
	* @return
	*/
	inline string year_name_in_chinese() const; 
};

inline	string LunarDate::lunar_month_name() const
{
    int lunar_month = m_month;
    string month_name;
    if (lunar_month >= 0 && lunar_month < 13) 
    {
        month_name = LUNAR_MONTH_NAMES[lunar_month];
        if (m_is_leap_month) 
        {
            month_name = "闰" + month_name;
        }
    } 
    return	month_name;
        
}

inline string	LunarDate::lunar_day_name() const
{
    int lunar_day = m_day;
    string day_name;
    if (lunar_day > 0 && lunar_day < 31) 
    {
        day_name = LUNAR_DAYS_NAMES[lunar_day];
    }
    return day_name;
}


inline string	LunarDate::animal_of_lunar_year() const
{
    return LUNAR_ANIMAL[(m_year - 4) % 60 % 12];
}


inline string	LunarDate::lunar_year_name() const
{
    string par1 = LUNAR_YEAR_PART1[(m_year - 4) % 60 % 10];
    string par2 = LUNAR_YEAR_PART2[(m_year - 4) % 60 % 12];
    return par1 + par2 + "年";
}

inline string LunarDate::year_name_in_chinese() const
{
    const char * chinese_number[] = 
    { 
        "零", "一", "二", "三", "四", "五", "六", "七", "八", "九" 
    };
    string name;
    int num = 0;
    int year = m_year;
    while (year > 0) 
    {
        num = year % 10;
        name.append(chinese_number[num]);
        year = year / 10;
    }
    return	name;
}
}

#endif