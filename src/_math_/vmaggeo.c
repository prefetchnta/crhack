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
    #include <math.h>
#endif

/*****************************************************************************/
/*                                 基础计算                                  */
/*****************************************************************************/

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

/*
=======================================
    获取9维特征向量
=======================================
*/
CR_API void_t
maggeo_delta (
  __CR_OT__ fp32_t          dvec[9],
  __CR_IN__ const sMAGGEO*  mag_crrt,
  __CR_IN__ const sMAGGEO*  mag_base
    )
{
    fp32_t  dist;

    dvec[0] = mag_crrt->X - mag_base->X;            /* 北向强度 */
    dvec[1] = mag_crrt->Y - mag_base->Y;            /* 东向强度 */
    dvec[2] = mag_crrt->Z - mag_base->Z;            /* 垂直强度 */
    dvec[3] = mag_crrt->H - mag_base->H;            /* 水平强度 */
    dvec[4] = mag_crrt->F - mag_base->F;            /* 总 强 度 */
    dvec[5] = mag_crrt->D - mag_base->D;            /* 磁 偏 角 */
    if      (dvec[5] >  180.0f) dvec[5] -= 360.0f;
    else if (dvec[5] < -180.0f) dvec[5] += 360.0f;
    dvec[6] = mag_crrt->I - mag_base->I;            /* 磁 倾 角 */
    dist = dvec[0] * dvec[0] + dvec[1] * dvec[1] + dvec[2] * dvec[2];
    dvec[7] = FSQRT(dist);                          /* 欧氏距离 */
    dvec[8] = maggeo_angle(mag_crrt, mag_base);     /* 余玄距离 */
}

/*****************************************************************************/
/*                                 判定计算                                  */
/*****************************************************************************/

/*
=======================================
    九门估值参数检查
=======================================
*/
CR_API bool_t
gate9_check (
  __CR_IO__ sRFGEO_GATE9*   param
    )
{
    fp32_t  sum;

    if (param->gate_x_lms[0] <= 0 ||
        param->gate_x_lms[1] <= 0 ||
        param->gate_x_lms[2] <= 0 ||
        param->gate_x_lms[0] <= param->gate_x_lms[1] ||
        param->gate_x_lms[0] <= param->gate_x_lms[2] ||
        param->gate_x_lms[1] <= param->gate_x_lms[2])
        return (FALSE);

    if (param->gate_y_lms[0] <= 0 ||
        param->gate_y_lms[1] <= 0 ||
        param->gate_y_lms[2] <= 0 ||
        param->gate_y_lms[0] <= param->gate_y_lms[1] ||
        param->gate_y_lms[0] <= param->gate_y_lms[2] ||
        param->gate_y_lms[1] <= param->gate_y_lms[2])
        return (FALSE);

    if (param->gate_z_lms[0] <= 0 ||
        param->gate_z_lms[1] <= 0 ||
        param->gate_z_lms[2] <= 0 ||
        param->gate_z_lms[0] <= param->gate_z_lms[1] ||
        param->gate_z_lms[0] <= param->gate_z_lms[2] ||
        param->gate_z_lms[1] <= param->gate_z_lms[2])
        return (FALSE);

    if (param->scale_xyz[0] < 0)
        param->scale_xyz[0] = 1.0f;
    if (param->scale_xyz[1] < 0)
        param->scale_xyz[1] = 1.0f;
    if (param->scale_xyz[2] < 0)
        param->scale_xyz[2] = 1.0f;

    if (param->weight_xyz[0] < 0)
        param->weight_xyz[0] = 1.0f;
    sum  = param->weight_xyz[0];
    if (param->weight_xyz[1] < 0)
        param->weight_xyz[1] = 1.0f;
    sum += param->weight_xyz[1];
    if (param->weight_xyz[2] < 0)
        param->weight_xyz[2] = 1.0f;
    sum += param->weight_xyz[2];
    if (sum < 0.1f)
        return (FALSE);
    sum = 1.0f / sum;
    param->weight_xyz[0] *= sum;
    param->weight_xyz[1] *= sum;
    param->weight_xyz[2] *= sum;

    if (param->probe_x_lms[0] <= 0)
        param->probe_x_lms[0] = 1.21f;
    if (param->probe_x_lms[1] <= 0)
        param->probe_x_lms[1] = 0.55f;
    if (param->probe_x_lms[2] <= 0)
        param->probe_x_lms[2] = 0.33f;
    if (param->probe_x_lms[1] >= 1 ||
        param->probe_x_lms[2] >= 1 ||
        param->probe_x_lms[0] <= param->probe_x_lms[1] ||
        param->probe_x_lms[0] <= param->probe_x_lms[2] ||
        param->probe_x_lms[1] <= param->probe_x_lms[2])
        return (FALSE);

    if (param->probe_y_lms[0] <= 0)
        param->probe_y_lms[0] = 0.99f;
    if (param->probe_y_lms[1] <= 0)
        param->probe_y_lms[1] = 0.44f;
    if (param->probe_y_lms[2] <= 0)
        param->probe_y_lms[2] = 0.165f;
    if (param->probe_y_lms[1] >= 1 ||
        param->probe_y_lms[2] >= 1 ||
        param->probe_y_lms[0] <= param->probe_y_lms[1] ||
        param->probe_y_lms[0] <= param->probe_y_lms[2] ||
        param->probe_y_lms[1] <= param->probe_y_lms[2])
        return (FALSE);

    if (param->probe_z_lms[0] <= 0)
        param->probe_z_lms[0] = 1.21f;
    if (param->probe_z_lms[1] <= 0)
        param->probe_z_lms[1] = 0.55f;
    if (param->probe_z_lms[2] <= 0)
        param->probe_z_lms[2] = 0.33f;
    if (param->probe_z_lms[1] >= 1 ||
        param->probe_z_lms[2] >= 1 ||
        param->probe_z_lms[0] <= param->probe_z_lms[1] ||
        param->probe_z_lms[0] <= param->probe_z_lms[2] ||
        param->probe_z_lms[1] <= param->probe_z_lms[2])
        return (FALSE);
    return (TRUE);
}

/*
---------------------------------------
    九门线性估值计算
---------------------------------------
*/
static fp32_t
gate9_compute (
  __CR_IN__ sint_t          delta,
  __CR_IN__ const fp32_t*   percent_lms,
  __CR_IN__ const sint_t*   gate_lms
    )
{
    fp32_t  dlt, val;

    /* LM 段 - 线性插值 */
    if (delta > gate_lms[1]) {
        dlt = (fp32_t)(delta - gate_lms[1]);
        val = (percent_lms[0] - percent_lms[1]) * dlt;
        dlt = 1.0f / (fp32_t)(gate_lms[0] - gate_lms[1]);
        return (val * dlt + percent_lms[1]);
    }

    /* MS 段 - 线性插值 */
    if (delta > gate_lms[2]) {
        dlt = (fp32_t)(delta - gate_lms[2]);
        val = (percent_lms[1] - percent_lms[2]) * dlt;
        dlt = 1.0f / (fp32_t)(gate_lms[1] - gate_lms[2]);
        return (val * dlt + percent_lms[2]);
    }

    /* S0 段 - 线性插值 */
    val = percent_lms[2] * (fp32_t)delta;
    dlt = 1.0f / (fp32_t)gate_lms[2];
    return (val * dlt);
}

/*
=======================================
    九门估值算法
=======================================
*/
CR_API fp32_t
gate9_evaluate (
  __CR_IN__ const sint_t*       xyzt,
  __CR_IN__ const sint_t*       base,
  __CR_IN__ const sRFGEO_GATE9* param
    )
{
    sint_t  delta[3];
    fp32_t  value[3];

    /* 计算差值 */
    delta[0] = xyzt[0] - base[0];
    delta[1] = xyzt[1] - base[1];
    delta[2] = xyzt[2] - base[2];
    if (delta[0] < 0) delta[0] = -delta[0];
    if (delta[1] < 0) delta[1] = -delta[1];
    if (delta[2] < 0) delta[2] = -delta[2];

    /* 缩放调整 */
    delta[0] = (sint_t)((fp32_t)delta[0] * param->scale_xyz[0] + 0.5f);
    delta[1] = (sint_t)((fp32_t)delta[1] * param->scale_xyz[1] + 0.5f);
    delta[2] = (sint_t)((fp32_t)delta[2] * param->scale_xyz[2] + 0.5f);

    /* 三轴处理 */
    value[0] = gate9_compute(delta[0], param->probe_x_lms, param->gate_x_lms);
    value[1] = gate9_compute(delta[1], param->probe_y_lms, param->gate_y_lms);
    value[2] = gate9_compute(delta[2], param->probe_z_lms, param->gate_z_lms);

    /* 权重合成 */
    return (value[0] * param->weight_xyz[0] +
            value[1] * param->weight_xyz[1] +
            value[2] * param->weight_xyz[2]);
}

/*
=======================================
    九门判定算法
=======================================
*/
CR_API bool_t
gate9_judgment (
  __CR_IN__ const sint_t*       xyzt,
  __CR_IN__ const sint_t*       base,
  __CR_IN__ const sRFGEO_GATE9* param,
  __CR_IN__ fp32_t              updn
    )
{
    uint_t  count;
    sint_t  delta[4];
    sint_t  gate_x[3];
    sint_t  gate_y[3];
    sint_t  gate_z[3];
    sint_t  gate_avgn;

    /* 计算差值 */
    delta[0] = xyzt[0] - base[0];
    delta[1] = xyzt[1] - base[1];
    delta[2] = xyzt[2] - base[2];
    if (delta[0] < 0) delta[0] = -delta[0];
    if (delta[1] < 0) delta[1] = -delta[1];
    if (delta[2] < 0) delta[2] = -delta[2];

    /* 缩放调整 */
    delta[0] = (sint_t)((fp32_t)delta[0] * param->scale_xyz[0] + 0.5f);
    delta[1] = (sint_t)((fp32_t)delta[1] * param->scale_xyz[1] + 0.5f);
    delta[2] = (sint_t)((fp32_t)delta[2] * param->scale_xyz[2] + 0.5f);

    /* 计算阈值分界线 */
    for (count = 0; count < 3; count++) {
        gate_x[count] = (sint_t)(param->gate_x_lms[count] * updn);
        gate_y[count] = (sint_t)(param->gate_y_lms[count] * updn);
        gate_z[count] = (sint_t)(param->gate_z_lms[count] * updn);
    }

    /* 一轴大于最大 */
    if (delta[0] > gate_x[0] || delta[1] > gate_y[0] || delta[2] > gate_z[0])
        return (TRUE);
    count = 0;

    /* 二轴大于中间 */
    if (delta[0] > gate_x[1]) count += 1;
    if (delta[1] > gate_y[1]) count += 1;
    if (delta[2] > gate_z[1]) count += 1;
    if (count >= 2)
        return (TRUE);
    count = 0;

    /* 考虑三轴均值 */
    delta[3] = ((delta[0] + delta[1] + delta[2]) * 10 + 15) / 30;
    gate_avgn = ((gate_x[2] + gate_y[2] + gate_z[2]) * 10 + 15) / 30;

    /* 三轴大于最小 */
    if (delta[0] > gate_x[2]) count += 1;
    if (delta[1] > gate_y[2]) count += 1;
    if (delta[2] > gate_z[2]) count += 1;
    if (delta[3] > gate_avgn) count += 1;
    return ((count >= 3) ? TRUE : FALSE);
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
