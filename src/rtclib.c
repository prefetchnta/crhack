/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-01-15  */
/*     #######          ###    ###      [CORE]      ###  ~~~~~~~~~~~~~~~~~~  */
/*    ########          ###    ###                  ###  MODIFY: XXXX-XX-XX  */
/*    ####  ##          ###    ###                  ###  ~~~~~~~~~~~~~~~~~~  */
/*   ###       ### ###  ###    ###    ####    ####  ###   ##  +-----------+  */
/*  ####       ######## ##########  #######  ###### ###  ###  |  A NEW C  |  */
/*  ###        ######## ########## ########  ###### ### ###   | FRAMEWORK |  */
/*  ###     ## #### ### ########## ###  ### ###     ######    |  FOR ALL  |  */
/*  ####   ### ###  ### ###    ### ###  ### ###     ######    | PLATFORMS |  */
/*  ########## ###      ###    ### ######## ####### #######   |  AND ALL  |  */
/*   #######   ###      ###    ### ########  ###### ###  ###  | COMPILERS |  */
/*    #####    ###      ###    ###  #### ##   ####  ###   ##  +-----------+  */
/*  =======================================================================  */
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack 日期时间计算函数库 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "memlib.h"
#include "rtclib.h"
#include "strlib.h"

#if !defined(__no_s_day_tab)

/* 每月天数查找表 */
static const ufast_t _rom_ s_day_tab[2][13] =
{
    { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
    { 0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
};

#endif  /* !__no_s_day_tab */

#if !defined(__no_is_leap_year)
/*
---------------------------------------
    是否是闰年
---------------------------------------
*/
cr_inline bool_t
is_leap_year (
  __CR_IN__ uint_t  year
    )
{
    return (((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) & 1);
}

#endif  /* !__no_is_leap_year */

#if !defined(__no_leap_year_count)
/*
---------------------------------------
    闰年的个数
---------------------------------------
*/
static uint_t
leap_year_count (
  __CR_IN__ uint_t  year1,
  __CR_IN__ uint_t  year2
    )
{
    uint_t  count1, count2;

    year2 -= 1;
    count1 = (year1 / 4 - year1 / 100 + year1 / 400);
    count2 = (year2 / 4 - year2 / 100 + year2 / 400);
    return (count1 - count2);
}

#endif  /* !__no_leap_year_count */

#if !defined(__no_day_count)
/*
---------------------------------------
    计算天数 (当天不算)
---------------------------------------
*/
static sint_t
day_count (
  __CR_IN__ const sDATETIME*    date,
  __CR_IN__ bool_t              flag
    )
{
    uint_t  sum;
    ufast_t idx, leap = is_leap_year(date->year);

    if (flag)
    {
        /* 年初算起 */
        sum = date->day - 1;
        for (idx = 1; idx < date->month; idx++)
            sum += s_day_tab[leap][idx];
    }
    else
    {
        /* 算到年底 */
        sum = s_day_tab[leap][date->month] - date->day;
        for (idx = date->month + 1; idx <= 12; idx++)
            sum += s_day_tab[leap][idx];
    }
    return (sum);
}

#endif  /* !__no_day_count */

#if !defined(__no_date_get_week)
/*
=======================================
    日期周数 (基姆拉尔森计算公式)
=======================================
*/
CR_API ufast_t
date_get_week (
  __CR_IN__ const sDATETIME*    date
    )
{
    uint_t  year;
    ufast_t month;

    /* 输入的日期必须有效 */
    if (date->month >= 3) {
        year  = date->year;
        month = date->month;
    }
    else {
        year  = date->year - 1;
        month = date->month + 12;
    }
    return ((ufast_t)((date->day + 2 * month + 3 * (month + 1) / 5 +
                year + year / 4 - year / 100 + year / 400 + 1) % 7));
}

#endif  /* !__no_date_get_week */

#if !defined(__no_date_set_week)
/*
=======================================
    日期周数
=======================================
*/
CR_API ufast_t
date_set_week (
  __CR_IO__ sDATETIME*  date
    )
{
    date->week = date_get_week(date);
    return (date->week);
}

#endif  /* !__no_date_set_week */

#if !defined(__no_date_inc) || !defined(__no_date_dec)
/*
---------------------------------------
    日期加法 (内部)
---------------------------------------
*/
static void_t
date_inc_int (
  __CR_IO__ sDATETIME*  date,
  __CR_IN__ uint_t      days,
  __CR_IN__ ufast_t     leap,
  __CR_IN__ ufast_t     count
    )
{
    while (days != 0) {
        if (date->day == count) {
            if (date->month == 12) {
                date->year++;
                leap = is_leap_year(date->year);
                count = 31;
                date->month = 1;
            }
            else {
                date->month++;
                count = s_day_tab[leap][date->month];
            }
            date->day = 1;
            days -= 1;
        }
        else {
            if (days <= count - date->day) {
                date->day += days;
                break;
            }
            days -= count - date->day;
            date->day = count;
        }
    }
}

/*
---------------------------------------
    日期减法 (内部)
---------------------------------------
*/
static void_t
date_dec_int (
  __CR_IO__ sDATETIME*  date,
  __CR_IN__ uint_t      days,
  __CR_IN__ ufast_t     leap,
  __CR_IN__ ufast_t     count
    )
{
    while (days != 0) {
        if (date->day == 1) {
            if (date->month == 1) {
                date->year--;
                leap = is_leap_year(date->year);
                count = 31;
                date->month = 12;
            }
            else {
                date->month--;
                count = s_day_tab[leap][date->month];
            }
            date->day = count;
            days -= 1;
        }
        else {
            if (days < date->day) {
                date->day -= days;
                break;
            }
            days -= date->day - 1;
            date->day = 1;
        }
    }
}

#endif  /* !__no_date_inc || !__no_date_dec */

#if !defined(__no_date_inc)
/*
=======================================
    日期加法
=======================================
*/
CR_API void_t
date_inc (
  __CR_IO__ sDATETIME*  date,
  __CR_IN__ int32u      days
    )
{
    uint_t  rest, delta;
    ufast_t leap, count;

    /* 输入的日期必须有效 */
    if (days == 0)
        return;
    if (date->year == 0 ||
        date->year > 9999 || date->day == 0 ||
        date->month == 0 || date->month > 12)
        return;
    leap = is_leap_year(date->year);
    count = s_day_tab[leap][date->month];
    if (date->day > count)
        return;

    /* 先算到下年的起始 */
    if (date->month != 1 || date->day != 1) {
        rest = day_count(date, FALSE) + 1;
        if (rest >= days) {
            date_inc_int(date, (uint_t)days, leap, count);
            return;
        }
        days -= rest;
        date->year += 1;
        date->month = 1;
        date->day   = 1;
    }

    /* 直接穿越到目的年份 */
    rest = (uint_t)(days % 365);
    days = (int32u)(days / 365);
    if (days != 0) {
        days += date->year;
        if (days >= 9999) {
            date->year = 10000;
            return;
        }

        /* 计算多加的天数 (闰年的个数) */
        delta = leap_year_count((uint_t)(days - 1), date->year);
        date->year = (uint_t)days;
    }
    else {
        delta = 0;
    }

    /* 剩下的年内天数要减掉多加的天数 */
    if (rest > delta) {
        rest -= delta;
        leap = is_leap_year(date->year);
        date_inc_int(date, rest, leap, 31);
    }
    else
    if (rest < delta) {
        delta -= rest;
        leap = is_leap_year(date->year);
        date_dec_int(date, delta, leap, 31);
    }
    date_set_week(date);
}

#endif  /* !__no_date_inc */

#if !defined(__no_date_dec)
/*
=======================================
    日期减法
=======================================
*/
CR_API void_t
date_dec (
  __CR_IO__ sDATETIME*  date,
  __CR_IN__ int32u      days
    )
{
    uint_t  rest, delta;
    ufast_t leap, count;

    /* 输入的日期必须有效 */
    if (days == 0)
        return;
    if (date->year == 0 ||
        date->year > 9999 || date->day == 0 ||
        date->month == 0 || date->month > 12)
        return;
    leap = is_leap_year(date->year);
    count = s_day_tab[leap][date->month];
    if (date->day > count)
        return;

    /* 先算到当年的起始 */
    if (date->month != 1 || date->day != 1) {
        rest = day_count(date, TRUE);
        if (rest >= days) {
            date_dec_int(date, (uint_t)days, leap, count);
            return;
        }
        days -= rest;
        date->month = 1;
        date->day   = 1;
    }

    /* 直接穿越到目的年份 */
    rest = (uint_t)(days % 365);
    days = (int32u)(days / 365);
    if (days != 0) {
        if (days >= date->year) {
            date->year = 0;
            return;
        }
        days = date->year - days;

        /* 计算多减的天数 (闰年的个数) */
        delta = leap_year_count(date->year - 1, (uint_t)days);
        date->year = (uint_t)days;
    }
    else {
        delta = 0;
    }

    /* 剩下的年内天数要减掉多减的天数 */
    if (rest > delta) {
        rest -= delta;
        leap = is_leap_year(date->year);
        date_dec_int(date, rest, leap, 31);
    }
    else
    if (rest < delta) {
        delta -= rest;
        leap = is_leap_year(date->year);
        date_inc_int(date, delta, leap, 31);
    }
    date_set_week(date);
}

#endif  /* !__no_date_dec */

#if !defined(__no_time_inc)
/*
=======================================
    时间加法
=======================================
*/
CR_API void_t
time_inc (
  __CR_IO__ sDATETIME*  time,
  __CR_IN__ int32u      seconds
    )
{
    int32u  days, secs;

    /* 计算日期部分 */
    if (time->hour >= 24 ||
        time->minute >= 60 ||
        time->second >= 60)
        return;
    days = seconds / 86400UL;
    secs = seconds % 86400UL;
    date_inc(time, days);

    /* 计算时间部分 */
    seconds  = time->hour;
    seconds *= 60UL;
    seconds += time->minute;
    seconds *= 60UL;
    seconds += time->second;
    secs += seconds;
    if (secs >= 86400UL) {
        secs -= 86400UL;
        date_inc(time, 1);
    }
    time->hour = (ufast_t)(secs / 3600);
    secs %= 3600;
    time->minute = (ufast_t)(secs / 60);
    time->second = (ufast_t)(secs % 60);
}

#endif  /* !__no_time_inc */

#if !defined(__no_time_dec)
/*
=======================================
    时间减法
=======================================
*/
CR_API void_t
time_dec (
  __CR_IO__ sDATETIME*  time,
  __CR_IN__ int32u      seconds
    )
{
    int32u  days, secs;

    /* 计算日期部分 */
    if (time->hour >= 24 ||
        time->minute >= 60 ||
        time->second >= 60)
        return;
    days = seconds / 86400UL;
    secs = seconds % 86400UL;
    date_dec(time, days);

    /* 计算时间部分 */
    seconds  = time->hour;
    seconds *= 60UL;
    seconds += time->minute;
    seconds *= 60UL;
    seconds += time->second;
    if (secs <= seconds) {
        secs = seconds - secs;
    }
    else {
        secs = 86400UL + seconds - secs;
        date_dec(time, 1);
    }
    time->hour = (ufast_t)(secs / 3600);
    secs %= 3600;
    time->minute = (ufast_t)(secs / 60);
    time->second = (ufast_t)(secs % 60);
}

#endif  /* !__no_time_dec */

#if !defined(__no_date_sub)
/*
=======================================
    日期相减
=======================================
*/
CR_API maxs_t
date_sub (
  __CR_IN__ const sDATETIME*    date1,
  __CR_IN__ const sDATETIME*    date2
    )
{
    maxs_t  days;

    /* 返回正数 */
    if (date1->year > date2->year) {
        days = date1->year - date2->year - 1;
        if (days != 0) {
            days *= 365L;
            days += leap_year_count(date1->year - 1, date2->year + 1);
        }
        days += day_count(date2, FALSE) + 1;
        return ((days + (maxs_t)day_count(date1, TRUE)));
    }

    /* 返回负数 */
    if (date2->year > date1->year) {
        days = date2->year - date1->year - 1;
        if (days != 0) {
            days *= 365L;
            days += leap_year_count(date2->year - 1, date1->year + 1);
        }
        days += day_count(date1, FALSE) + 1;
        return ((days + (maxs_t)day_count(date2, TRUE)) * (-1L));
    }
    return ((maxs_t)(day_count(date1, TRUE) - day_count(date2, TRUE)));
}

#endif  /* !__no_date_sub */

#if !defined(__no_datetime_sub)
/*
=======================================
    日期时间相减
=======================================
*/
CR_API maxs_t
datetime_sub (
  __CR_IN__ const sDATETIME*    datetime1,
  __CR_IN__ const sDATETIME*    datetime2
    )
{
    maxs_t  secs1, secs2, days_secs;
    maxs_t  day = date_sub(datetime1, datetime2);

    secs1 = (((maxs_t)datetime1->hour * 60L) +
            datetime1->minute) * 60L + datetime1->second;
    secs2 = (((maxs_t)datetime2->hour * 60L) +
            datetime2->minute) * 60L + datetime2->second;
    days_secs = day * 86400L;
    days_secs += secs1 - secs2;
    return (days_secs);
}

#endif  /* !__no_datetime_sub */

#if !defined(__no_datetime_chk)
/*
=======================================
    日期时间检查
=======================================
*/
CR_API bool_t
datetime_chk (
  __CR_IN__ const sDATETIME*    datetime
    )
{
    ufast_t leap, count;

    /* 年 & 闰年 & 年天数 */
    if (datetime->year == 0 || datetime->year > 9999)
        return (FALSE);
    leap = is_leap_year(datetime->year);

    /* 月 & 月天数 */
    if (datetime->month == 0 || datetime->month > 12)
        return (FALSE);
    count = s_day_tab[leap][datetime->month];

    /* 日 */
    if (datetime->day == 0 || datetime->day > count)
        return (FALSE);

    /* 时 */
    if (datetime->hour >= 24)
        return (FALSE);

    /* 分 */
    if (datetime->minute >= 60)
        return (FALSE);

    /* 秒 */
    if (datetime->second >= 60)
        return (FALSE);
    return (TRUE);
}

#endif  /* !__no_datetime_chk */

#if !defined(__no_datetime_cmp)
/*
=======================================
    日期时间比较
=======================================
*/
CR_API sint_t
datetime_cmp (
  __CR_IN__ const sDATETIME*    datetime1,
  __CR_IN__ const sDATETIME*    datetime2
    )
{
    /* 年比较 */
    if (datetime1->year > datetime2->year)
        return ( 1);
    if (datetime1->year < datetime2->year)
        return (-1);

    /* 月比较 */
    if (datetime1->month > datetime2->month)
        return ( 1);
    if (datetime1->month < datetime2->month)
        return (-1);

    /* 日比较 */
    if (datetime1->day > datetime2->day)
        return ( 1);
    if (datetime1->day < datetime2->day)
        return (-1);

    /* 时比较 */
    if (datetime1->hour > datetime2->hour)
        return ( 1);
    if (datetime1->hour < datetime2->hour)
        return (-1);

    /* 分比较 */
    if (datetime1->minute > datetime2->minute)
        return ( 1);
    if (datetime1->minute < datetime2->minute)
        return (-1);

    /* 秒比较 */
    if (datetime1->second > datetime2->second)
        return ( 1);
    if (datetime1->second < datetime2->second)
        return (-1);

    return (0);
}

#endif  /* !__no_datetime_cmp */

#if !defined(__no_datetime_attr)
/*
=======================================
    日期时间属性
=======================================
*/
CR_API bool_t
datetime_attr (
  __CR_OT__ sDATEATTR*          dateattr,
  __CR_IN__ const sDATETIME*    datetime
    )
{
    ufast_t leap;

    /* 年 & 闰年 & 年天数 */
    if (datetime->year == 0 || datetime->year > 9999)
        return (FALSE);
    dateattr->year_leap = leap = is_leap_year(datetime->year);
    dateattr->year_days = leap + 365;

    /* 月 & 月天数 */
    if (datetime->month == 0 || datetime->month > 12)
        return (FALSE);
    dateattr->month_day = s_day_tab[leap][datetime->month];

    /* 日 */
    if (datetime->day == 0 || datetime->day > dateattr->month_day)
        return (FALSE);
    dateattr->days_year = day_count(datetime, TRUE) + 1;

    /* 时 */
    if (datetime->hour >= 24)
        return (FALSE);

    /* 分 */
    if (datetime->minute >= 60)
        return (FALSE);

    /* 秒 */
    if (datetime->second >= 60)
        return (FALSE);
    dateattr->days_secs = (((int32u)datetime->hour * 60UL) +
                        datetime->minute) * 60UL + datetime->second;
    return (TRUE);
}

#endif  /* !__no_datetime_attr */

#if !defined(_CR_RTC_NO_STRING_)

#if !defined(__no_str2datetimeA)
/*
=======================================
    字符串转日期时间A
=======================================
*/
CR_API bool_t
str2datetimeA (
  __CR_OT__ sDATETIME*      datetime,
  __CR_IN__ const ansi_t*   string,
  __CR_IN__ ansi_t          split_char
    )
{
    leng_t  skip;
    ufast_t leap, count;

    /* 年 & 闰年 & 年天数 */
    struct_zero(datetime, sDATETIME);
    datetime->year = (uint_t)str2intA(string, &skip);
    if (skip != 4 || datetime->year == 0 || datetime->year > 9999)
        return (FALSE);

    if (!is_datesA(string[4]))
        return (TRUE);

    /* 月 & 月天数 */
    datetime->month = (ufast_t)str2intA(string + 5, &skip);
    if (skip != 2 || datetime->month == 0 || datetime->month > 12)
        return (FALSE);

    if (!is_datesA(string[7]))
        return (TRUE);
    leap = is_leap_year(datetime->year);
    count = s_day_tab[leap][datetime->month];

    /* 日 */
    datetime->day = (ufast_t)str2intA(string + 8, &skip);
    if (skip != 2 || datetime->day == 0 || datetime->day > count)
        return (FALSE);

    /* 周 */
    date_set_week(datetime);
    if (string[10] != split_char)
        return (TRUE);

    /* 时 */
    datetime->hour = (ufast_t)str2intA(string + 11, &skip);
    if (skip != 2 || datetime->hour >= 24)
        return (FALSE);

    if (!is_timesA(string[13]))
        return (TRUE);

    /* 分 */
    datetime->minute = (ufast_t)str2intA(string + 14, &skip);
    if (skip != 2 || datetime->minute >= 60)
        return (FALSE);

    if (!is_timesA(string[16]))
        return (TRUE);

    /* 秒 */
    datetime->second = (ufast_t)str2intA(string + 17, &skip);
    if (skip != 2 || datetime->second >= 60)
        return (FALSE);
    return (TRUE);
}

#endif  /* !__no_str2datetimeA */

#if !defined(_CR_NAKED_NO_WSTRING_)

#if !defined(__no_str2datetimeW)
/*
=======================================
    字符串转日期时间W
=======================================
*/
CR_API bool_t
str2datetimeW (
  __CR_OT__ sDATETIME*      datetime,
  __CR_IN__ const wide_t*   string,
  __CR_IN__ wide_t          split_char
    )
{
    leng_t  skip;
    ufast_t leap, count;

    /* 年 & 闰年 & 年天数 */
    struct_zero(datetime, sDATETIME);
    datetime->year = (uint_t)str2intW(string, &skip);
    if (skip != 4 || datetime->year == 0 || datetime->year > 9999)
        return (FALSE);

    if (!is_datesW(string[4]))
        return (TRUE);

    /* 月 & 月天数 */
    datetime->month = (ufast_t)str2intW(string + 5, &skip);
    if (skip != 2 || datetime->month == 0 || datetime->month > 12)
        return (FALSE);

    if (!is_datesW(string[7]))
        return (TRUE);
    leap = is_leap_year(datetime->year);
    count = s_day_tab[leap][datetime->month];

    /* 日 */
    datetime->day = (ufast_t)str2intW(string + 8, &skip);
    if (skip != 2 || datetime->day == 0 || datetime->day > count)
        return (FALSE);

    /* 周 */
    date_set_week(datetime);
    if (string[10] != split_char)
        return (TRUE);

    /* 时 */
    datetime->hour = (ufast_t)str2intW(string + 11, &skip);
    if (skip != 2 || datetime->hour >= 24)
        return (FALSE);

    if (!is_timesW(string[13]))
        return (TRUE);

    /* 分 */
    datetime->minute = (ufast_t)str2intW(string + 14, &skip);
    if (skip != 2 || datetime->minute >= 60)
        return (FALSE);

    if (!is_timesW(string[16]))
        return (TRUE);

    /* 秒 */
    datetime->second = (ufast_t)str2intW(string + 17, &skip);
    if (skip != 2 || datetime->second >= 60)
        return (FALSE);
    return (TRUE);
}

#endif  /* !__no_str2datetimeW */

#endif  /* !_CR_NAKED_NO_WSTRING_ */

#endif  /* !_CR_RTC_NO_STRING_ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
