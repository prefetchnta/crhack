/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2014-01-22  */
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
/*  >>>>>>>>>>>>>>>>>>> CrHack 日期时间函数库 for POSIX <<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "rtclib.h"

#include <sys/time.h>
#include <time.h>

/*
=======================================
    获取系统时间
=======================================
*/
CR_API bool_t
datetime_get (
  __CR_OT__ sDATETIME*  datetime
    )
{
    time_t      tm;
    struct tm   dt;

    tm = time(NULL);
    if (tm == (time_t)-1)
        return (FALSE);
    if (localtime_r(&tm, &dt) == NULL)
        return (FALSE);
    datetime->year   = ( uint_t)dt.tm_year + 1900;
    datetime->month  = (ufast_t)dt.tm_mon + 1;
    datetime->day    = (ufast_t)dt.tm_mday;
    datetime->hour   = (ufast_t)dt.tm_hour;
    datetime->minute = (ufast_t)dt.tm_min;
    datetime->second = (ufast_t)dt.tm_sec;
    datetime->week   = (ufast_t)dt.tm_wday;
    return (TRUE);
}

/*
=======================================
    设置系统时间
=======================================
*/
CR_API bool_t
datetime_set (
  __CR_IN__ const sDATETIME*    datetime
    )
{
    time_t          tm;
    struct tm       dt;
    struct timeval  tv;
    struct timezone tz;

    if (datetime->year < 1900)
        return (FALSE);
    tm = time(NULL);
    if (tm == (time_t)-1)
        return (FALSE);
    if (localtime_r(&tm, &dt) == NULL)
        return (FALSE);
    dt.tm_year = (int)datetime->year - 1900;
    dt.tm_mon  = (int)datetime->month - 1;
    dt.tm_mday = (int)datetime->day;
    dt.tm_hour = (int)datetime->hour;
    dt.tm_min  = (int)datetime->minute;
    dt.tm_sec  = (int)datetime->second;
    tm = mktime(&dt);
    if (tm == (time_t)-1)
        return (FALSE);
    if (gettimeofday(&tv, &tz) != 0)
        return (FALSE);
    tv.tv_sec = tm;
    tv.tv_usec = 0;
    if (settimeofday(&tv, &tz) != 0)
        return (FALSE);
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
