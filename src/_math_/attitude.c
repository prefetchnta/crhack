/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2025-04-22  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 姿态计算函数库 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "phylib.h"

#ifndef _CR_NO_STDC_
    #include <math.h>
#endif

/*
---------------------------------------
    一阶互补滤波姿态计算 (1个轴)
---------------------------------------
*/
static void_t
attitude_order1st_one (
  __CR_IO__ sFLT_O1ST*  ctx,
  __CR_IN__ uint_t      idx,
  __CR_IN__ fp32_t      acc1,
  __CR_IN__ fp32_t      acc3,
  __CR_IN__ fp32_t      gyro2
    )
{
    ctx->a[idx] = ctx->K * (ctx->a[idx] - gyro2 * ctx->T) + (1 - ctx->K) *
                                (FATAN2(acc1, acc3) * CR_RTOD);
}

/*
=======================================
    一阶互补滤波姿态计算
=======================================
*/
CR_API void_t
attitude_order1st (
  __CR_IO__ sFLT_O1ST*      ctx,
  __CR_IN__ const fp32_t    aac_xyz[3],
  __CR_IN__ const fp32_t    gyro_xyz[3]
    )
{
    attitude_order1st_one(ctx, 0, aac_xyz[1], aac_xyz[2], gyro_xyz[0]);
    attitude_order1st_one(ctx, 1, aac_xyz[0], aac_xyz[2], gyro_xyz[1]);
    attitude_order1st_one(ctx, 2, aac_xyz[0], aac_xyz[1], gyro_xyz[2]);
}

/*
---------------------------------------
    二阶互补滤波姿态计算 (1个轴)
---------------------------------------
*/
static void_t
attitude_order2nd_one (
  __CR_IO__ sFLT_O2ND*  ctx,
  __CR_IN__ uint_t      idx,
  __CR_IN__ fp32_t      acc1,
  __CR_IN__ fp32_t      acc3,
  __CR_IN__ fp32_t      gyro2
    )
{
    fp32_t  da = FATAN2(acc1, acc3) * CR_RTOD - ctx->a[idx];
    fp32_t  xx = da * ctx->K * ctx->K;

    ctx->y[idx] += xx * ctx->T;
    xx = ctx->y[idx] + 2 * ctx->K * da - gyro2;
    ctx->a[idx] += xx * ctx->T;
}

/*
=======================================
    二互补滤波姿态计算
=======================================
*/
CR_API void_t
attitude_order2nd (
  __CR_IO__ sFLT_O2ND*      ctx,
  __CR_IN__ const fp32_t    aac_xyz[3],
  __CR_IN__ const fp32_t    gyro_xyz[3]
    )
{
    attitude_order2nd_one(ctx, 0, aac_xyz[1], aac_xyz[2], gyro_xyz[0]);
    attitude_order2nd_one(ctx, 1, aac_xyz[0], aac_xyz[2], gyro_xyz[1]);
    attitude_order2nd_one(ctx, 2, aac_xyz[0], aac_xyz[1], gyro_xyz[2]);
}

/*
---------------------------------------
    卡尔曼滤波姿态计算 (1个轴)
---------------------------------------
*/
static void_t
attitude_f_kalman_one (
  __CR_IO__ sFLT_KLMN*  ctx,
  __CR_IN__ uint_t      idx,
  __CR_IN__ fp32_t      acc1,
  __CR_IN__ fp32_t      acc3,
  __CR_IN__ fp32_t      gyro2
    )
{
    fp32_t  ee, k0, k1;
    fp32_t  na = FATAN2(acc1, acc3) * CR_RTOD - ctx->a[idx];

    ctx->a[idx]   -= ctx->T * (gyro2 + ctx->qbx[idx]);
    ctx->p00[idx] += ctx->T * (ctx->Qa - ctx->p10[idx] - ctx->p01[idx]);
    ctx->p01[idx] -= ctx->T * ctx->p11[idx];
    ctx->p10[idx] -= ctx->T * ctx->p11[idx];
    ctx->p11[idx] += ctx->T * ctx->Qg;

    ee = ctx->p00[idx] + ctx->Ra;
    k0 = ctx->p00[idx] / ee;
    k1 = ctx->p10[idx] / ee;

    ctx->a[idx]   += k0 * na;
    ctx->qbx[idx] += k1 * na;
    ctx->p00[idx] -= k0 * ctx->p00[idx];
    ctx->p01[idx] -= k0 * ctx->p01[idx];
    ctx->p10[idx] -= k1 * ctx->p00[idx];
    ctx->p11[idx] -= k1 * ctx->p01[idx];
}

/*
=======================================
    卡尔曼滤波姿态计算
=======================================
*/
CR_API void_t
attitude_f_kalman (
  __CR_IO__ sFLT_KLMN*      ctx,
  __CR_IN__ const fp32_t    aac_xyz[3],
  __CR_IN__ const fp32_t    gyro_xyz[3]
    )
{
    attitude_f_kalman_one(ctx, 0, aac_xyz[1], aac_xyz[2], gyro_xyz[0]);
    attitude_f_kalman_one(ctx, 1, aac_xyz[0], aac_xyz[2], gyro_xyz[1]);
    attitude_f_kalman_one(ctx, 2, aac_xyz[0], aac_xyz[1], gyro_xyz[2]);
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
