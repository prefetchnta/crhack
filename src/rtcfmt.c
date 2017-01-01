/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2013-03-19  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack 日期时间解析函数库 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "msclib.h"
#include "rtclib.h"

/*
=======================================
    转换到 DOSDATE 格式
=======================================
*/
CR_API bool_t
date_to_dos (
  __CR_OT__ int16u*             value,
  __CR_IN__ const sDATETIME*    date
    )
{
    int16u  temp;

    if (!datetime_chk(date))
        return (FALSE);
    if (date->year < 1980 || date->year > 1980 + 0x7F)
        return (FALSE);
    temp  = (int16u)(date->year - 1980);
    temp <<= 4;
    temp |= (int16u)(date->month);
    temp <<= 5;
    temp |= (int16u)(date->day);
    *value = temp;
    return (TRUE);
}

/*
=======================================
    转换到 DOSTIME 格式
=======================================
*/
CR_API bool_t
time_to_dos (
  __CR_OT__ int16u*             value,
  __CR_IN__ const sDATETIME*    time
    )
{
    int16u  temp;

    if (!datetime_chk(time))
        return (FALSE);
    temp  = (int16u)(time->hour);
    temp <<= 6;
    temp |= (int16u)(time->minute);
    temp <<= 5;
    temp |= (int16u)(time->second / 2);
    *value = temp;
    return (TRUE);
}

/*
=======================================
    从 DOSDATE 格式转入
=======================================
*/
CR_API bool_t
date_from_dos (
  __CR_OT__ sDATETIME*  date,
  __CR_IN__ int16u      value
    )
{
    bool_t  rett;
    ufast_t hour, mint, secs;

    hour = date->hour;
    mint = date->minute;
    secs = date->second;
    date->day   = (ufast_t)((value >> 0) & 0x1F);
    date->month = (ufast_t)((value >> 5) & 0x0F);
    date->year  = ( uint_t)((value >> 9) & 0x7F);
    date->year += 1980;
    date->hour   = 0;
    date->minute = 0;
    date->second = 0;
    rett = datetime_chk(date);
    date->hour   = hour;
    date->minute = mint;
    date->second = secs;
    if (rett)
        date_set_week(date);
    return (rett);
}

/*
=======================================
    从 DOSTIME 格式转入
=======================================
*/
CR_API bool_t
time_from_dos (
  __CR_OT__ sDATETIME*  time,
  __CR_IN__ int16u      value
    )
{
    bool_t  rett;
    uint_t  year;
    ufast_t month, day;

    year  = time->year;
    month = time->month;
    day   = time->day;
    time->second  = (ufast_t)((value >>  0) & 0x1F);
    time->minute  = (ufast_t)((value >>  5) & 0x3F);
    time->hour    = (ufast_t)((value >> 11) & 0x1F);
    time->second *= 2;
    time->year  = 1980;
    time->month = 1;
    time->day   = 1;
    rett = datetime_chk(time);
    time->year  = year;
    time->month = month;
    time->day   = day;
    return (rett);
}

/*
=======================================
    转换到 DOS 格式
=======================================
*/
CR_API bool_t
datetime_to_dos (
  __CR_OT__ int16u              value[2],
  __CR_IN__ const sDATETIME*    datetime
    )
{
    int16u  temp;

    if (!datetime_chk(datetime))
        return (FALSE);
    if (datetime->year < 1980 || datetime->year > 1980 + 0x7F)
        return (FALSE);
    temp  = (int16u)(datetime->year - 1980);
    temp <<= 4;
    temp |= (int16u)(datetime->month);
    temp <<= 5;
    temp |= (int16u)(datetime->day);
    value[0] = temp;
    temp  = (int16u)(datetime->hour);
    temp <<= 6;
    temp |= (int16u)(datetime->minute);
    temp <<= 5;
    temp |= (int16u)(datetime->second / 2);
    value[1] = temp;
    return (TRUE);
}

/*
=======================================
    从 DOS 格式转入
=======================================
*/
CR_API bool_t
datetime_from_dos (
  __CR_OT__ sDATETIME*      datetime,
  __CR_IN__ const int16u    value[2]
    )
{
    datetime->day     = (ufast_t)((value[0] >> 0) & 0x1F);
    datetime->month   = (ufast_t)((value[0] >> 5) & 0x0F);
    datetime->year    = ( uint_t)((value[0] >> 9) & 0x7F);
    datetime->year   += 1980;
    datetime->second  = (ufast_t)((value[1] >>  0) & 0x1F);
    datetime->minute  = (ufast_t)((value[1] >>  5) & 0x3F);
    datetime->hour    = (ufast_t)((value[1] >> 11) & 0x1F);
    datetime->second *= 2;
    if (!datetime_chk(datetime))
        return (FALSE);
    date_set_week(datetime);
    return (TRUE);
}

/*
=======================================
    转换到 FILETIME 格式
=======================================
*/
CR_API bool_t
datetime_to_w32 (
  __CR_OT__ int64u*             value,
  __CR_IN__ const sDATETIME*    datetime
    )
{
    int64u      betw;
    int64u      temp;
    int32u      days;
    sDATETIME   start;

    if (!datetime_chk(datetime))
        return (FALSE);
    start.year   = 1601;
    start.month  = 1;
    start.day    = 1;
    start.hour   = 0;
    start.minute = 0;
    start.second = 0;
    if (datetime_cmp(datetime, &start) < 0)
        return (FALSE);
    betw  = date_sub(datetime, &start);
    betw *= CR_ULL(864000000000);
    days  = datetime->hour;
    days *= 60UL;
    days += datetime->minute;
    days *= 60UL;
    days += datetime->second;
    temp  = days;
    temp *= CR_ULL(10000000);
    *value = betw + temp;
    return (TRUE);
}

/*
=======================================
    从 FILETIME 格式转入
=======================================
*/
CR_API bool_t
datetime_from_w32 (
  __CR_OT__ sDATETIME*  datetime,
  __CR_IN__ int64u      value
    )
{
    int32u  days, secs;

    /* 计算日期部分 */
    datetime->year  = 1601;
    datetime->month = 1;
    datetime->day   = 1;
    value /= CR_ULL(10000000);
    days = (int32u)(value / CR_ULL(86400));
    secs = (int32u)(value % CR_ULL(86400));
    if (days != 0) {
        date_inc(datetime, days);
        if (datetime->year > 9999)
            return (FALSE);
    }
    else {
        date_set_week(datetime);
    }

    /* 计算时间部分 */
    datetime->hour = (ufast_t)(secs / 3600);
    secs %= 3600;
    datetime->minute = (ufast_t)(secs / 60);
    datetime->second = (ufast_t)(secs % 60);
    return (TRUE);
}

/*
=======================================
    转换到 time64_t 格式
=======================================
*/
CR_API bool_t
datetime_to_unx (
  __CR_OT__ int64u*             value,
  __CR_IN__ const sDATETIME*    datetime
    )
{
    int64u  temp;

    if (!datetime_to_w32(&temp, datetime))
        return (FALSE);
    temp -= CR_ULL(116444736000000000);
    if ((int64s)temp < 0)
        return (FALSE);
    *value = temp / CR_ULL(10000000);
    return (TRUE);
}

/*
=======================================
    从 time64_t 格式转入
=======================================
*/
CR_API bool_t
datetime_from_unx (
  __CR_OT__ sDATETIME*  datetime,
  __CR_IN__ int64u      value
    )
{
    int64u  temp1, temp2;

    temp1 = value * CR_ULL(10000000);
    if (temp1 < value)
        return (FALSE);
    temp2 = temp1 + CR_ULL(116444736000000000);
    if (temp2 < temp1)
        return (FALSE);
    return (datetime_from_w32(datetime, temp2));
}

/*
=======================================
    转换到 longdt 格式
=======================================
*/
CR_API bool_t
datetime_to_mac (
  __CR_OT__ int64u*             value,
  __CR_IN__ const sDATETIME*    datetime
    )
{
    int64u  temp;

    if (!datetime_to_w32(&temp, datetime))
        return (FALSE);
    temp -= CR_ULL(95616288000000000);
    if ((int64s)temp < 0)
        return (FALSE);
    *value = temp / CR_ULL(10000000);
    return (TRUE);
}

/*
=======================================
    从 longdt 格式转入
=======================================
*/
CR_API bool_t
datetime_from_mac (
  __CR_OT__ sDATETIME*  datetime,
  __CR_IN__ int64u      value
    )
{
    int64u  temp1, temp2;

    temp1 = value * CR_ULL(10000000);
    if (temp1 < value)
        return (FALSE);
    temp2 = temp1 + CR_ULL(95616288000000000);
    if (temp2 < temp1)
        return (FALSE);
    return (datetime_from_w32(datetime, temp2));
}

/*
=======================================
    转换到 OLETIME 格式
=======================================
*/
CR_API bool_t
datetime_to_ole (
  __CR_OT__ fp64_t*             value,
  __CR_IN__ const sDATETIME*    datetime
    )
{
    fp64_t      betw;
    int32s      temp;
    bool_t      sign;
    sDATETIME   start;

    if (!datetime_chk(datetime))
        return (FALSE);
    start.year  = 1899;
    start.month = 12;
    start.day   = 30;
    betw = (fp64_t)date_sub(datetime, &start);
    if (betw < 0.0) {
        sign = TRUE;
        betw = -betw;
    }
    else {
        sign = FALSE;
    }
    temp  = datetime->hour;
    temp *= 60UL;
    temp += datetime->minute;
    temp *= 60UL;
    temp += datetime->second;
    betw += ((fp64_t)temp / 86400.0);
    *value = sign ? -betw : betw;
    return (TRUE);
}

/*
=======================================
    从 OLETIME 格式转入
=======================================
*/
CR_API bool_t
datetime_from_ole (
  __CR_OT__ sDATETIME*  datetime,
  __CR_IN__ fp64_t      value
    )
{
    int32s  days;
    bool_t  sign;

    if (value < 0.0) {
        if (value < -657434.0)
            return (FALSE);
        sign = TRUE;
        value = -value;
    }
    else {
        if (value > 2958465.0)
            return (FALSE);
        sign = FALSE;
    }
    datetime->year  = 1899;
    datetime->month = 12;
    datetime->day   = 30;
    days = (int32s)value;
    if (days != 0) {
        if (sign)
            date_dec(datetime, days);
        else
            date_inc(datetime, days);
        if (datetime->year == 0 || datetime->year > 9999)
            return (FALSE);
        value -= (fp64_t)days;
    }
    else {
        date_set_week(datetime);
    }
    value *= 24.0;
    days = (int32s)value;
    datetime->hour = (ufast_t)days;
    value -= (fp64_t)days;
    value *= 60.0;
    days = (int32s)value;
    datetime->minute = (ufast_t)days;
    value -= (fp64_t)days;
    value *= 60.0;
    days = (int32s)value;
    datetime->second = (ufast_t)days;
    return (TRUE);
}

/*
=======================================
    转换到 BCD 格式
=======================================
*/
CR_API bool_t
datetime_to_bcd (
  __CR_OT__ sBCD_DT*            value,
  __CR_IN__ const sDATETIME*    datetime
    )
{
    if (!datetime_chk(datetime))
        return (FALSE);
    value->yr_hi  = bin2bcd((byte_t)(datetime->year / 100));
    value->yr_lo  = bin2bcd((byte_t)(datetime->year % 100));
    value->month  = bin2bcd((byte_t)(datetime->month));
    value->day    = bin2bcd((byte_t)(datetime->day));
    value->hour   = bin2bcd((byte_t)(datetime->hour));
    value->minute = bin2bcd((byte_t)(datetime->minute));
    value->second = bin2bcd((byte_t)(datetime->second));
    return (TRUE);
}

/*
=======================================
    从 BCD 格式转入
=======================================
*/
CR_API bool_t
datetime_from_bcd (
  __CR_OT__ sDATETIME*      datetime,
  __CR_IN__ const sBCD_DT*  value
    )
{
    ufast_t         idx;
    const byte_t*   ptr;

    ptr = &value->yr_hi;
    for (idx = 0; idx < 7; idx++) {
        if (!bcd2bin_okay(ptr[idx]))
            return (FALSE);
    }
    datetime->year   = bcd2bin(value->yr_hi);
    datetime->year  *= 100;
    datetime->year  += bcd2bin(value->yr_lo);
    datetime->month  = bcd2bin(value->month);
    datetime->day    = bcd2bin(value->day);
    datetime->hour   = bcd2bin(value->hour);
    datetime->minute = bcd2bin(value->minute);
    datetime->second = bcd2bin(value->second);
    if (!datetime_chk(datetime))
        return (FALSE);
    date_set_week(datetime);
    return (TRUE);
}

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
