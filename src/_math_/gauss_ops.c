/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2023-12-04  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 高斯方法函数库 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "msclib.h"
#include "phylib.h"

#ifndef _CR_NO_STDC_
    #include <math.h>
#endif

/*
=======================================
    高斯随机初始化
=======================================
*/
CR_API void_t
gauss_rnd_init (
  __CR_OT__ sGAUSS_RND* grnd
    )
{
    grnd->S = 0;
    grnd->M = 1 / (fpxx_t)rand_get_max();
    grnd->V1 = grnd->V2 = 0;
    grnd->phase = 0;
}

/*
=======================================
    高斯随机数获取
=======================================
*/
CR_API fpxx_t
gauss_rnd_get (
  __CR_IO__ sGAUSS_RND* grnd
    )
{
    fpxx_t  X, U1, U2;

    if (grnd->phase == 0) {
        do {
            U1 = (fpxx_t)rand_get() * grnd->M;
            U2 = (fpxx_t)rand_get() * grnd->M;
            grnd->V1 = 2 * U1 - 1;
            grnd->V2 = 2 * U2 - 1;
            grnd->S = grnd->V1 * grnd->V1 + grnd->V2 * grnd->V2;
        }
        while (grnd->S >= 1 || XABS(grnd->S) <= CR_ABITX);

        X = grnd->V1 * XSQRT(-2 * XLOGE(grnd->S) / grnd->S);
    }
    else {
        X = grnd->V2 * XSQRT(-2 * XLOGE(grnd->S) / grnd->S);
    }
    grnd->phase = 1 - grnd->phase;
    return (X);
}

/*
=======================================
    高斯随机分布生成
=======================================
*/
CR_API void_t
gauss_rnd_make (
  __CR_IO__ sGAUSS_RND* grnd,
  __CR_IN__ fpxx_t      mean,
  __CR_IN__ fpxx_t      sd,
  __CR_OT__ fpxx_t*     output,
  __CR_IN__ leng_t      count
    )
{
    for (; count != 0; count--)
        *output++ = gauss_rnd_get(grnd) * sd + mean;
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
