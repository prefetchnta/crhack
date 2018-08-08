/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2018-08-07  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>> CrHack 低通滤波器函数库 <<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "phylib.h"

/*
=======================================
    清除 FIR 延时线
=======================================
*/
CR_API void_t
fir_lp_clear (
  __CR_IN__ sint_t  ntaps,
  __CR_OT__ double  z[]
    )
{
    sint_t  ii;

    for (ii = 0; ii < ntaps; ii++)
        z[ii] = 0;
}

/*
=======================================
    线性 FIR 低通滤波
=======================================
*/
CR_API double
fir_lp_basic (
  __CR_IN__ double          input,
  __CR_IN__ sint_t          ntaps,
  __CR_IN__ const double    h[],
  __CR_IO__ double          z[]
    )
{
    sint_t  ii;
    double  accum = 0;

    z[0] = input;
    for (ii = 0; ii < ntaps; ii++)
        accum += h[ii] * z[ii];
    for (ii = ntaps - 2; ii >= 0; ii--)
        z[ii + 1] = z[ii];
    return (accum);
}

/*
=======================================
    环形 FIR 低通滤波
=======================================
*/
CR_API double
fir_lp_circular (
  __CR_IN__ double          input,
  __CR_IN__ sint_t          ntaps,
  __CR_IN__ const double    h[],
  __CR_IO__ double          z[],
  __CR_IO__ sint_t*         p_state
    )
{
    double  accum = 0;
    sint_t  ii, state = *p_state;

    z[state] = input;
    if (++state >= ntaps)
        state = 0;
    for (ii = ntaps - 1; ii >= 0; ii--) {
        accum += h[ii] * z[state];
        if (++state >= ntaps)
            state = 0;
    }
    *p_state = state;
    return (accum);
}

/*
=======================================
    平移 FIR 低通滤波
=======================================
*/
CR_API double
fir_lp_shuffle (
  __CR_IN__ double          input,
  __CR_IN__ sint_t          ntaps,
  __CR_IN__ const double    h[],
  __CR_IO__ double          z[]
    )
{
    sint_t  ii;
    double  accum;

    z[0] = input;
    accum = h[ntaps - 1] * z[ntaps - 1];
    for (ii = ntaps - 2; ii >= 0; ii--) {
        accum += h[ii] * z[ii];
        z[ii + 1] = z[ii];
    }
    return (accum);
}

/*
=======================================
    分割 FIR 低通滤波
=======================================
*/
CR_API double
fir_lp_split (
  __CR_IN__ double          input,
  __CR_IN__ sint_t          ntaps,
  __CR_IN__ const double    h[],
  __CR_IO__ double          z[],
  __CR_IO__ sint_t*         p_state
    )
{
    double  accum = 0, *p_h, *p_z;
    sint_t  ii, end_ntaps, state = *p_state;

    p_h = (double*)h;
    p_z = (double*)z + state;
    p_z[0] = input;
    end_ntaps = ntaps - state;
    for (ii = 0; ii < end_ntaps; ii++)
        accum += *p_h++ * *p_z++;
    p_z = (double*)z;
    for (ii = 0; ii < state; ii++)
        accum += *p_h++ * *p_z++;
    if (--state < 0)
        state += ntaps;
    *p_state = state;
    return (accum);
}

/*
=======================================
    双重 FIR 低通滤波 (Z)
=======================================
*/
CR_API double
fir_lp_double_z (
  __CR_IN__ double          input,
  __CR_IN__ sint_t          ntaps,
  __CR_IN__ const double    h[],
  __CR_IO__ double          z[],
  __CR_IO__ sint_t*         p_state
    )
{
    sint_t  ii, state = *p_state;
    double  accum = 0, *p_h, *p_z;

    z[state] = z[state + ntaps] = input;
    p_h = (double*)h;
    p_z = (double*)z + state;
    for (ii = 0; ii < ntaps; ii++)
        accum += *p_h++ * *p_z++;
    if (--state < 0)
        state += ntaps;
    *p_state = state;
    return (accum);
}

/*
=======================================
    双重 FIR 低通滤波 (H)
=======================================
*/
CR_API double
fir_lp_double_h (
  __CR_IN__ double          input,
  __CR_IN__ sint_t          ntaps,
  __CR_IN__ const double    h[],
  __CR_IO__ double          z[],
  __CR_IO__ sint_t*         p_state
    )
{
    sint_t  ii, state = *p_state;
    double  accum = 0, *p_h, *p_z;

    z[state] = input;
    p_h = (double*)h + ntaps - state;
    p_z = (double*)z;
    for (ii = 0; ii < ntaps; ii++)
        accum += *p_h++ * *p_z++;
    if (--state < 0)
        state += ntaps;
    *p_state = state;
    return (accum);
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
