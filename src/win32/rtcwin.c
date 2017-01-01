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
/*  >>>>>>>>>>>>>>>>>> CrHack 日期时间函数库 for Windows <<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "rtclib.h"

#include <windows.h>

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
    SYSTEMTIME  time;

    GetLocalTime(&time);
    datetime->year   = ( uint_t)time.wYear;
    datetime->month  = (ufast_t)time.wMonth;
    datetime->day    = (ufast_t)time.wDay;
    datetime->hour   = (ufast_t)time.wHour;
    datetime->minute = (ufast_t)time.wMinute;
    datetime->second = (ufast_t)time.wSecond;
    datetime->week   = (ufast_t)time.wDayOfWeek;
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
    SYSTEMTIME  time;

    time.wMilliseconds = 0;
    time.wYear   = (WORD)datetime->year;
    time.wMonth  = (WORD)datetime->month;
    time.wDay    = (WORD)datetime->day;
    time.wHour   = (WORD)datetime->hour;
    time.wMinute = (WORD)datetime->minute;
    time.wSecond = (WORD)datetime->second;
    if (!SetLocalTime(&time))
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
