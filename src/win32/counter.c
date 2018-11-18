/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-01-12  */
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
/*  >>>>>>>>>>>>>>>>>>> CrHack 定时器函数库 for Windows <<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "applib.h"
#include "memlib.h"

#include <windows.h>

/* 定时器内部结构 */
typedef struct
{
        bool_t  high;       /* 是否使用高精度计数器 */
        fp32_t  freq;       /*  计数器的频率 (KHz)  */
        int64s  base64;     /*  计数器的基数 (64位) */
        int32u  base32;     /*  计数器的基数 (32位) */

} sCOUNTER;

/*
=======================================
    创建定时器
=======================================
*/
CR_API xtime_t
timer_new (void_t)
{
    int64s      count;
    sCOUNTER*   timer;

    timer = struct_new(sCOUNTER);
    if (timer == NULL)
        return (NULL);
    count = 0;

    /* 高精度计数器失败使用低精度的计数器 */
    if (!QueryPerformanceFrequency((LARGE_INTEGER*)(&count))
        || count == 0) {
        timer->high = FALSE;
        timer->freq = 1000.0f;
    }
    else {
        timer->high = TRUE;
        timer->freq = (fp32_t)(count / 1000);
    }
    timer->base64 = 0;
    timer->base32 = 0;
    return ((xtime_t)timer);
}

/*
=======================================
    释放定时器
=======================================
*/
CR_API void_t
timer_del (
  __CR_IN__ xtime_t timer
    )
{
    mem_free(timer);
}

/*
=======================================
    获取系统计时 (低精度)
=======================================
*/
CR_API int32u
timer_get32 (void_t)
{
    return (GetTickCount());
}

/*
=======================================
    获取系统计时 (高精度)
=======================================
*/
CR_API int64u
timer_get64 (void_t)
{
    int64u          count;
    static int64u   freq = 1;

    if (freq == 0)
        return ((int64u)GetTickCount());
    if (freq == 1) {
        count = 0;
        QueryPerformanceFrequency((LARGE_INTEGER*)(&count));
        freq = count;
        if (freq == 0)
            return ((int64u)GetTickCount());
    }
    QueryPerformanceCounter((LARGE_INTEGER*)(&count));
    return (count * 1000UL / freq);
}

/*
=======================================
    设置定时器基数
=======================================
*/
CR_API void_t
timer_set_base (
  __CR_IN__ xtime_t timer
    )
{
    sCOUNTER*   obj = (sCOUNTER*)timer;

    if (obj->high)
        QueryPerformanceCounter((LARGE_INTEGER*)(&obj->base64));
    else
        obj->base32 = GetTickCount();
}

/*
=======================================
    获取定时器时差 (ms)
=======================================
*/
CR_API fp32_t
timer_get_delta (
  __CR_IN__ xtime_t timer
    )
{
    int32u      cnlow;
    int64s      count;
    sCOUNTER*   obj = (sCOUNTER*)timer;

    if (!obj->high) {
        cnlow = GetTickCount();
        if (rarely(cnlow < obj->base32))
            return ((fp32_t)(0xFFFFFFFFUL - obj->base32 + cnlow + 1));
        return ((fp32_t)(cnlow - obj->base32));
    }

    /* 使用高精度计数器 */
    QueryPerformanceCounter((LARGE_INTEGER*)(&count));
    return ((fp32_t)(count - obj->base64) / obj->freq);
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
