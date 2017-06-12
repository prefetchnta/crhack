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
/*  >>>>>>>>>>>>>>>>>>>> CrHack 定时器函数库 for POSIX <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "applib.h"
#include "memlib.h"

#include <sys/time.h>
#include <time.h>

#if defined(_CR_NO_API_CLOCK_GETTIME_)
/*
---------------------------------------
    clock_gettime() 的替代函数
---------------------------------------
*/
static int
counter_get (
  __CR_OT__ struct timespec*    tp
    )
{
    struct timeval now;
    int rv = gettimeofday(&now, NULL);

    if (rv) return (rv);
    tp->tv_sec  = now.tv_sec;
    tp->tv_nsec = now.tv_usec * 1000UL;
    return (0);
}

#else
/* 获取系统计时映射 */
#if     defined(CLOCK_MONOTONIC_HR)
    #define counter_get(tp) clock_gettime(CLOCK_MONOTONIC_HR, tp)
#elif   defined(CLOCK_MONOTONIC_RAW)
    #define counter_get(tp) clock_gettime(CLOCK_MONOTONIC_RAW, tp)
#else
    #define counter_get(tp) clock_gettime(CLOCK_MONOTONIC, tp)
#endif

#endif  /* _CR_NO_API_CLOCK_GETTIME_ */

/* 定时器内部结构 */
typedef struct
{
        struct timespec base;

} sCOUNTER;

/*
=======================================
    创建定时器
=======================================
*/
CR_API xtime_t
timer_new (void_t)
{
    sCOUNTER*   timer;

    timer = struct_new(sCOUNTER);
    if (timer == NULL)
        return (NULL);
    timer->base.tv_sec  = 0;
    timer->base.tv_nsec = 0;
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
    int64u          tm;
    struct timespec tp;

    counter_get(&tp);
    tm  = tp.tv_sec;
    tm *= CR_ULL(1000);
    tm += tp.tv_nsec / 1000000;
    return ((int32u)tm);
}

/*
=======================================
    获取系统计时 (高精度)
=======================================
*/
CR_API int64u
timer_get64 (void_t)
{
    int64u          tm;
    struct timespec tp;

    counter_get(&tp);
    tm  = tp.tv_sec;
    tm *= CR_ULL(1000);
    tm += tp.tv_nsec / 1000000;
    return ((int64u)tm);
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
    counter_get(&(((sCOUNTER*)timer)->base));
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
    fp64_t      dlt;
    sCOUNTER*   obj;
    /* ---------- */
    int64s  time1, time2;
    struct timespec delta;

    counter_get(&delta);
    time1  = delta.tv_sec;
    time1 *= CR_SLL(1000000000);
    time1 += delta.tv_nsec;

    obj = (sCOUNTER*)timer;
    time2  = obj->base.tv_sec;
    time2 *= CR_SLL(1000000000);
    time2 += obj->base.tv_nsec;
    dlt = (fp64_t)(time1 - time2);
    return ((fp32_t)(dlt / 1000000.0));
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
