/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2016-10-17  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 磁场计算函数库 <<<<<<<<<<<<<<<<<<<<<<<<  */
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

/*
=======================================
    计算磁场参数
=======================================
*/
CR_API bool_t
maggeo_param (
  __CR_OT__ sMAGGEO*    mag,
  __CR_IN__ sint_t      x,
  __CR_IN__ sint_t      z,
  __CR_IN__ sint_t      y
    )
{
    fp32_t  xx, yy, zz;

    mag->X = (fp32_t)x;
    mag->Y = (fp32_t)y;
    mag->Z = (fp32_t)z;
    xx = (fp32_t)(x * x);
    yy = (fp32_t)(y * y);
    zz = (fp32_t)(z * z);
    xx += yy;
    zz += xx;
    mag->H = FSQRT(xx);
    mag->F = FSQRT(zz);
    if (x == 0 && y == 0) {
        mag->D = 0.0f;
        if (z > 0)
            mag->I = 90.0f;
        else
        if (z < 0)
            mag->I = -90.0f;
        else
            return (FALSE);
    }
    else {
        mag->D = FACOS(mag->X / mag->H) * CR_RTOD;
        mag->I = FATAN(mag->Z / mag->H) * CR_RTOD;
        if (y < 0) mag->D = 360.0f - mag->D;
    }
    mag->V[0] = mag->X / mag->F;
    mag->V[1] = mag->Y / mag->F;
    mag->V[2] = mag->Z / mag->F;
    return (TRUE);
}

/*
=======================================
    计算磁场夹角
=======================================
*/
CR_API fp32_t
maggeo_angle (
  __CR_IN__ const sMAGGEO*  mag1,
  __CR_IN__ const sMAGGEO*  mag2
    )
{
    fp32_t  acosz, angle;

    acosz  = mag1->V[0] * mag2->V[0];
    acosz += mag1->V[1] * mag2->V[1];
    acosz += mag1->V[2] * mag2->V[2];
    angle  = FACOS(acosz) * CR_RTOD;
    return (angle);
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
