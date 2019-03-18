/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2019-02-09  */
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
/*  >>>>>>>>>>>>>>>>>>>>> CrHack 傅立叶变换计算函数库 <<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "phylib.h"

#ifndef _CR_NO_STDC_
    #include <math.h>
#endif

#define FFT1_MIN_PWR    2

/*
=======================================
    计算加权系数
=======================================
*/
CR_API sCOMPLEX*
fft1_init (
  __CR_OT__ sCOMPLEX*   W,
  __CR_IN__ sint_t      power
    )
{
    fpxx_t  angle;
    sint_t  idx, count;

    if (power <= FFT1_MIN_PWR)
        return (NULL);
    count = (1 << power) / 2;
    if (W == NULL) {
        W = mem_talloc(count, sCOMPLEX);
        if (W == NULL)
            return (NULL);
    }
    for (idx = 0; idx < count; idx++) {
        angle = -idx * CR_PHY_PI / count;
        W[idx].re = XCOS(angle);
        W[idx].im = XSIN(angle);
    }
    return (W);
}

/*
=======================================
    傅立叶变换
=======================================
*/
CR_API sCOMPLEX*
fft1_func (
  __CR_OT__ sCOMPLEX*       FD,
  __CR_IN__ const sCOMPLEX* TD,
  __CR_IN__ const sCOMPLEX* W,
  __CR_IN__ sint_t          power
    )
{
    sint_t  ii, jj, kk;
    sint_t  pp, count, bfsize;
    /* -------------------- */
    sCOMPLEX  T, *X1, *X2, *X;

    /* 分配内存 */
    if (power <= FFT1_MIN_PWR)
        return (NULL);
    count = 1 << power;
    X1 = mem_talloc(count, sCOMPLEX);
    X2 = mem_talloc(count, sCOMPLEX);
    if (X1 == NULL || X2 == NULL) {
        TRY_FREE(X1);
        TRY_FREE(X2);
        return (NULL);
    }

    /* 蝴形计算 */
    mem_cpy(X1, TD, count * sizeof(sCOMPLEX));
    for (kk = 0; kk < power; kk++) {
        for (jj = 0; jj < (1 << kk); jj++) {
            bfsize = (1 << (power - kk)) / 2;
            for (ii = 0; ii < bfsize; ii++) {
                pp = jj * bfsize * 2;
                complex_add(&X2[ii+pp], &X1[ii+pp], &X1[ii+pp+bfsize]);
                complex_sub(&T, &X1[ii+pp], &X1[ii+pp+bfsize]);
                complex_mul(&X2[ii+pp+bfsize], &T, &W[ii*(1<<kk)]);
            }
        }
        X = X1;
        X1 = X2;
        X2 = X;
    }

    /* 重新排序 */
    for (jj = 0; jj < count; jj++) {
        pp = 0;
        for (ii = 0; ii < power; ii++) {
            if (jj & (1 << ii))
                pp += 1 << (power - ii - 1);
        }
        FD[jj] = X1[pp];
    }

    /* 释放内存 */
    mem_free(X2);
    mem_free(X1);
    return (FD);
}

/*
=======================================
    傅立叶反变换
=======================================
*/
CR_API sCOMPLEX*
fft1_invert (
  __CR_OT__ sCOMPLEX*       TD,
  __CR_IN__ const sCOMPLEX* FD,
  __CR_IN__ const sCOMPLEX* W,
  __CR_IN__ sint_t          power
    )
{
    sCOMPLEX*   X;
    /* -------- */
    sint_t  idx, count;

    /* 分配内存 */
    if (power <= FFT1_MIN_PWR)
        return (NULL);
    count = 1 << power;
    X = mem_talloc(count, sCOMPLEX);
    if (X == NULL)
        return (NULL);

    /* 反变换计算 */
    mem_cpy(X, FD, count * sizeof(sCOMPLEX));
    for (idx = 0; idx < count; idx++)
        X[idx].im = -X[idx].im;
    if (fft1_func(TD, X, W, power) == NULL)
        return (NULL);
    for (idx = 0; idx < count; idx++) {
        TD[idx].re /= count;
        TD[idx].im = -TD[idx].im / count;
    }

    /* 释放内存 */
    mem_free(X);
    return (TD);
}

/*
=======================================
    傅立叶数据调整
=======================================
*/
CR_API sCOMPLEX*
fft1_shift (
  __CR_OT__ sCOMPLEX*       D,
  __CR_IN__ const sCOMPLEX* S,
  __CR_IN__ sint_t          power
    )
{
    sint_t  chalf;

    /* 对称数据互换 */
    if (power <= FFT1_MIN_PWR || D == S)
        return (NULL);
    chalf = (1 << power) / 2;
    mem_cpy(D, &S[chalf], chalf * sizeof(sCOMPLEX));
    mem_cpy(&D[chalf], S, chalf * sizeof(sCOMPLEX));
    return (D);
}

/*
=======================================
    生成用于显示的数据
=======================================
*/
CR_API fpxx_t*
fft1_ashow (
  __CR_OT__ fpxx_t*         D,
  __CR_IN__ const sCOMPLEX* S,
  __CR_IN__ sint_t          power
    )
{
    sint_t  idx, chalf, count;

    /* 对称数据互换 */
    if (power <= FFT1_MIN_PWR)
        return (NULL);
    count = 1 << power;
    chalf = count / 2;
    for (idx = 0; idx < chalf; idx++)
        D[idx] = complex_abs(&S[chalf + idx]) / chalf;
    for (; idx < count; idx++)
        D[idx] = complex_abs(&S[idx - chalf]) / chalf;
    return (D);
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
