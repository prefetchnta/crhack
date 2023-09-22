/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2012-01-15  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>> CrHack IAPWS-IF97 函数库 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "phylib.h"

/*
=======================================
    IAPWS-IF97 密度热焓计算
=======================================
*/
CR_API double
iif97_ws_de (
  __CR_OT__ double*     e,
  __CR_IN__ double      t,
  __CR_IN__ double      mpa,
  __CR_IN__ double      wx,
  __CR_OT__ ufast_t*    sec
    )
{
    double  ps;

    /* 过滤参数 */
    if (mpa <= 0.0 || t < 0.0 ||
        mpa > 100.0 || t > 2000.0 ||
        (mpa > 50.0 && t > 800.0))
        return (CR_PHY_INV);

    /* 鉴别分区 */
    if (t > 800.0)
    {
        /* 第五区 */
        if (sec != NULL)
            *sec = 5;
        if (e != NULL)
            *e = iif97_5_ws_e(t, mpa);
        return (iif97_5_ws_d(t, mpa));
    }
    if (t <= 350.0)
    {
        ps = iif97_4_t2mpa(t);
        if (mpa > ps)
        {
            /* 第一区 */
            if (sec != NULL)
                *sec = 1;
            if (e != NULL)
                *e = iif97_1_ws_e(t, mpa);
            return (iif97_1_ws_d(t, mpa));
        }

        /* 第二区 */
        if (sec != NULL)
            *sec = 2;
        if (e != NULL)
            *e = iif97_2_ws_e(t, mpa);
        return (iif97_2_ws_d(t, mpa));
    }
    if (t >= 590.0)
    {
        /* 第二区 */
        if (sec != NULL)
            *sec = 2;
        if (e != NULL)
            *e = iif97_2_ws_e(t, mpa);
        return (iif97_2_ws_d(t, mpa));
    }
    ps = iif97_2_3_t2mpa(t);
    if (mpa > ps)
    {
        /* 第三区 */
        if (sec != NULL)
            *sec = 3;
        ps = iif97_3_ws_d(t, mpa, wx);
        if (ps > 0.0) {
            if (e != NULL)
                *e = iif97_3_ws_e(t, ps);
        }
        else {
            if (e != NULL)
                *e = CR_PHY_INV;
        }
        return (ps);
    }

    /* 第二区 */
    if (sec != NULL)
        *sec = 2;
    if (e != NULL)
        *e = iif97_2_ws_e(t, mpa);
    return (iif97_2_ws_d(t, mpa));
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
