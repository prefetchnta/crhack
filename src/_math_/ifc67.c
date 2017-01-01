/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2012-01-08  */
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
/*  >>>>>>>>>>>>>>>>>>>> CrHack IFC-67 水蒸汽密度函数库 <<<<<<<<<<<<<<<<<<<  */
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

/* IFC-67 计算常数 */
#define _Pc     (22.12)
#define _Tc     (647.3)
#define _Uc     (0.00317)
#define _I1     (4.260321148)
#define _L0     (15.74373327)
#define _L1     (-34.17061978)
#define _L2     (19.31830707)
#define _B11    (0.066703759)
#define _B12    (1.388983801)
#define _B21    (0.08390104328)
#define _B22    (0.02614670893)
#define _B23    (-0.03373439453)
#define _B31    (0.4520918904)
#define _B32    (0.1069036614)
#define _B41    (-0.5975336707)
#define _B42    (-0.08847535804)
#define _B51    (0.5958051609)
#define _B52    (-0.5159303373)
#define _B53    (0.2075021122)
#define _B61    (0.1190610271)
#define _B62    (-0.09867174132)
#define _B71    (0.1683998803)
#define _B72    (-0.05809438001)
#define _B81    (0.006552390126)
#define _B82    (0.0005770218649)
#define _B90    (193.6587558)
#define _B91    (-1388.522425)
#define _B92    (4126.607219)
#define _B93    (-6508.211677)
#define _B94    (5745.984054)
#define _B95    (-2693.088365)
#define _B96    (523.5718623)
#define _BB61   (0.4006073948)
#define _BB71   (0.08636081627)
#define _BB81   (-0.8532322921)
#define _BB82   (0.3460208861)

/*
=======================================
    IFC-67 水蒸汽密度计算
=======================================
*/
CR_API double
ifc67_ws_d (
  __CR_IN__ double  t,
  __CR_IN__ double  mpa
    )
{
    double  ur, xx;
    double  tr, pr, p1;

    t += 273.15;
    tr = t / _Tc;
    pr = mpa / _Pc;
    p1 = _L0 + tr * (_L1 + _L2 * tr);
    xx = exp(0.7633333333 * (1 - tr));
    ur = _I1 * tr / pr - _B11 * pow(xx, 13) - _B12 * pow(xx, 3) -
        (2 * pr * (_B21 * pow(xx, 18) + _B22 * pow(xx, 2) + _B23 * xx)) -
        (3 * pow(pr,  2) * (_B31 * pow(xx, 18) + _B32 * pow(xx, 10))) -
        (4 * pow(pr,  3) * (_B41 * pow(xx, 25) + _B42 * pow(xx, 14))) -
        (5 * pow(pr,  4) * (_B51 * pow(xx, 32) + _B52 * pow(xx, 28) +
                            _B53 * pow(xx, 24))) -
        (4 * pow(pr, -5) * (_B61 * pow(xx, 12) + _B62 * pow(xx, 11))) *
                            pow(pow(pr, -4) - _BB61 * pow(xx, 14), -2) -
        (5 * pow(pr, -6) * (_B71 * pow(xx, 24) + _B72 * pow(xx, 18))) *
                            pow(pow(pr, -5) + _BB71 * pow(xx, 19), -2) -
        (6 * pow(pr, -7) * (_B81 * pow(xx, 24) + _B82 * pow(xx, 14))) *
                            pow(pow(pr, -6) + _BB81 * pow(xx, 54) +
                                              _BB82 * pow(xx, 27), -2) +
        11 * pow(pr / p1, 10) * (_B90 + _B91 * xx  + _B92 * pow(xx, 2) +
                                 _B93 * pow(xx, 3) + _B94 * pow(xx, 4) +
                                 _B95 * pow(xx, 5) + _B96 * pow(xx, 6));
    return (1.0 / (ur * _Uc));
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
