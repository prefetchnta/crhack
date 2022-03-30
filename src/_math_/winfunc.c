/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2019-02-12  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>> CrHack 窗函数计算函数库 <<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "phylib.h"

#ifndef _CR_NO_STDC_
    #include <math.h>
#endif

/*
=======================================
    三角窗
=======================================
*/
CR_API void_t
winfunc_triange (
  __CR_OT__ double* W,
  __CR_IN__ sint_t  N,
  __CR_IN__ sint_t  bias
    )
{
    double  val;
    sint_t  idx;

    if      (bias < -1) bias = -1;
    else if (bias >  1) bias =  1;
    for (idx = 0; idx < N; idx++) {
        val = (double)(2 * idx - N + 1);
        W[idx] = 1 - DABS(val / (N + bias));
    }
}

/*
---------------------------------------
    帕尔逊窗 (内部)
---------------------------------------
*/
static double
winfunc_parzen_int (
  __CR_IN__ double  X,
  __CR_IN__ double  N
    )
{
    double  v1, n2 = N / 2;
    double  v2, an = DABS(X);

    v1 = 1 - an / n2;
    if (an <= n2 / 2) {
        v2 = X / n2;
        return (1 - 6 * v1 * v2 * v2);
    }
    return (2 * v1 * v1 * v1);
}

/*
=======================================
    帕尔逊窗
=======================================
*/
CR_API void_t
winfunc_parzen (
  __CR_OT__ double* W,
  __CR_IN__ sint_t  N
    )
{
    double  val;
    sint_t  idx;

    val = (double)(N - 1) / 2;
    for (idx = 0; idx < N; idx++)
        W[idx] = winfunc_parzen_int(idx - val, N);
}

/*
=======================================
    韦尔奇窗
=======================================
*/
CR_API void_t
winfunc_welch (
  __CR_OT__ double* W,
  __CR_IN__ sint_t  N
    )
{
    sint_t  idx;
    double  v1, v2;

    v1 = (double)(N - 1) / 2;
    for (idx = 0; idx < N; idx++) {
        v2 = (idx - v1) / v1;
        W[idx] = 1 - v2 * v2;
    }
}

/*
=======================================
    正弦窗
=======================================
*/
CR_API void_t
winfunc_sine (
  __CR_OT__ double* W,
  __CR_IN__ sint_t  N,
  __CR_IN__ double  alpha
    )
{
    double  val;
    sint_t  idx;

    for (idx = 0; idx < N; idx++) {
        val = DSIN((CR_PHY_PI * idx) / (N - 1));
        W[idx] = DPOW(val, alpha);
    }
}

/*
=======================================
    海明窗
=======================================
*/
CR_API void_t
winfunc_hamming (
  __CR_OT__ double* W,
  __CR_IN__ sint_t  N,
  __CR_IN__ double  alpha
    )
{
    double  val;
    sint_t  idx;

    for (idx = 0; idx < N; idx++) {
        val = DCOS((2 * CR_PHY_PI * idx) / (N - 1));
        W[idx] = alpha - (1 - alpha) * val;
    }
}

/*
=======================================
    汉宁窗 (MATLAB)
=======================================
*/
CR_API void_t
winfunc_hann_periodic (
  __CR_OT__ double* W,
  __CR_IN__ sint_t  N
    )
{
    double  val;
    sint_t  idx;

    for (idx = 0; idx < N; idx++) {
        val = DCOS((2 * CR_PHY_PI * (idx - 1)) / N);
        W[idx] = 0.5 * (1 - val);
    }
}

/*
=======================================
    汉宁窗 (MATLAB)
=======================================
*/
CR_API void_t
winfunc_hann_symmetric (
  __CR_OT__ double* W,
  __CR_IN__ sint_t  N
    )
{
    double  val;
    sint_t  idx;

    for (idx = 0; idx < N; idx++) {
        val = DCOS((2 * CR_PHY_PI * (idx - 1)) / (N - 1));
        W[idx] = 0.5 * (1 - val);
    }
}

/*
=======================================
    汉宁窗 (MATLAB)
=======================================
*/
CR_API void_t
winfunc_hanning_symmetric (
  __CR_OT__ double* W,
  __CR_IN__ sint_t  N
    )
{
    double  val;
    sint_t  idx;

    for (idx = 0; idx < N; idx++) {
        val = DCOS((2 * CR_PHY_PI * idx) / (N + 1));
        W[idx] = 0.5 * (1 - val);
    }
}

/*
=======================================
    布莱克曼窗
=======================================
*/
CR_API void_t
winfunc_blackman (
  __CR_OT__ double* W,
  __CR_IN__ sint_t  N,
  __CR_IN__ double  alpha
    )
{
    double  val;
    sint_t  idx;

    for (idx = 0; idx < N; idx++) {
        val = (2 * CR_PHY_PI * idx) / (N - 1);
        W[idx] = (1 - alpha) - DCOS(val) + alpha * DCOS(2 * val);
        W[idx] *= 0.5;
    }
}

/*
=======================================
    高斯窗
=======================================
*/
CR_API void_t
winfunc_gaussian (
  __CR_OT__ double* W,
  __CR_IN__ sint_t  N,
  __CR_IN__ double  alpha
    )
{
    double  val;
    sint_t  idx;

    if (alpha > 0.5)
        alpha = 0.5;
    for (idx = 0; idx < N; idx++) {
        val = (double)(2 * idx - N + 1);
        val /= (N - 1) * alpha;
        W[idx] = DEXP(-0.5 * val * val);
    }
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
