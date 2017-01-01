/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2012-01-13  */
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
/*  >>>>>>>>>>>>>>>>>>>>> CrHack IAPWS-IF97 五区函数库 <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "phylib.h"

#ifndef _CR_NO_STDC_
    #if defined(_CR_OS_ANDROID_) && (ANDROID_API <= 19)
        #if defined(__pure2)
            #undef  __pure2
        #endif
    #endif
    #include <math.h>
#endif

/* IAPWS-IF97 五区（高温区）计算常数表 */
static const sint_t _rom_ s_iif97_ii[6] =
{
    1, 1, 1, 2, 2, 3,
};

static const sint_t _rom_ s_iif97_ji[6] =
{
    1, 2, 3, 3, 9, 7,
};

static const double _rom_ s_iif97_ni[6] =
{
    0.15736404855259E-02,  0.90153761673944E-03, -0.50270077677648E-02,
    0.22440037409485E-05, -0.41163275453471E-05,  0.37919454822955E-07,
};

static const double _rom_ s_iif97_n0i[6] =
{
    -0.13179983674201E+02,  0.68540841634434E+01, -0.24805148933466E-01,
     0.36901534980333E+00, -0.31161318213925E+01, -0.32961626538917E+00,
};

static const sint_t _rom_ s_iif97_j0i[6] =
{
    0, 1, -3, -2, -1, 2,
};

/*
=======================================
    IAPWS-IF97 五区密度计算
=======================================
*/
CR_API double
iif97_5_ws_d (
  __CR_IN__ double  t,
  __CR_IN__ double  mpa
    )
{
    double  xx;
    double  vvv;
    ufast_t idx;

    xx = 0.0;
    t += 273.15;
    for (vvv = 1000.0 / t, idx = 0; idx < 6; idx++) {
        xx += s_iif97_ni[idx] * s_iif97_ii[idx] *
                  pow(mpa, s_iif97_ii[idx] - 1) *
                  pow(vvv, s_iif97_ji[idx]);
    }
    return (1000.0 / (0.461526 * t * (1.0 / mpa + xx)));
}

/*
=======================================
    IAPWS-IF97 五区热焓计算
=======================================
*/
CR_API double
iif97_5_ws_e (
  __CR_IN__ double  t,
  __CR_IN__ double  mpa
    )
{
    double  xx;
    double  yy;
    double  vvv;
    ufast_t idx;

    xx = 0.0;
    yy = 0.0;
    t += 273.15;
    for (vvv = 1000.0 / t, idx = 0; idx < 6; idx++) {
        xx += s_iif97_n0i[idx] * s_iif97_j0i[idx] *
                  pow(vvv, s_iif97_j0i[idx] - 1);
    }
    for (idx = 0; idx < 6; idx++) {
        yy += s_iif97_ni[idx] * s_iif97_ji[idx] *
                  pow(mpa, s_iif97_ii[idx]) *
                  pow(vvv, s_iif97_ji[idx] - 1);
    }
    return ((1000.0 * 0.461526 / 4.187) * (xx + yy));
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
