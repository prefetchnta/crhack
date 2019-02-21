/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2019-01-21  */
/*     #######          ###    ###      [MATH]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack 统计相关计算函数库 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "phylib.h"

#ifndef _CR_NO_STDC_
    #include <math.h>
#endif

/*
=======================================
    均值/方差/标准差计算
=======================================
*/
CR_API double
statistics (
  __CR_IN__ const double*   data,
  __CR_IN__ uint_t          count,
  __CR_OT__ double*         sd2,
  __CR_OT__ double*         sd
    )
{
    uint_t  idx;
    double  acc, tmp;
    double  num, avg = 0;

    /* 平均值 */
    num = (double)count;
    for (idx = 0; idx < count; idx++)
        avg += data[idx];
    avg /= num;

    if (sd2 != NULL || sd != NULL)
    {
        /* 方差 */
        for (acc = 0, idx = 0; idx < count; idx++) {
            tmp = data[idx] - avg;
            acc += tmp * tmp;
        }
        acc /= num;
        if (sd2 != NULL)
            *sd2 = acc;

        /* 标准差 */
        if (sd != NULL)
            *sd = sqrt(acc);
    }
    return (avg);
}

/*
=======================================
    协方差/相关系数计算
=======================================
*/
CR_API double
covariance (
  __CR_IN__ const double*   x,
  __CR_IN__ const double*   y,
  __CR_IN__ uint_t          count,
  __CR_OT__ double*         pxy
    )
{
    uint_t  idx;
    double  num, exy = 0;
    double  ex, ey, sdx, sdy;

    /* 计算均值和标准差 */
    if (pxy != NULL) {
        ex = statistics(x, count, &sdx, NULL);
        ey = statistics(y, count, &sdy, NULL);
    }
    else {
        ex = statistics(x, count, NULL, NULL);
        ey = statistics(y, count, NULL, NULL);
        sdx = sdy = 1;
    }

    /* 计算协方差 */
    num = (double)count;
    for (idx = 0; idx < count; idx++)
        exy += x[idx] * y[idx];
    exy /= num;
    exy -= ex * ey;

    /* 计算相关系数 */
    if (pxy != NULL)
        *pxy = exy / sqrt(sdx * sdy);
    return (exy);
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
