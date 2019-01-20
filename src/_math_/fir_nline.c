/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2018-08-08  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack 非线性滤波器函数库 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "msclib.h"
#include "phylib.h"

/*
---------------------------------------
    数值排序比较
---------------------------------------
*/
static sint_t
value_comp_double (
  __CR_IN__ const void_t*   elem1,
  __CR_IN__ const void_t*   elem2
    )
{
    double  val1 = *(double*)elem1;
    double  val2 = *(double*)elem2;

    /* 升序 */
    if (val1 < val2) return (-1);
    if (val1 > val2) return ( 1);
    return (0);
}

/*
=======================================
    中值滤波器
=======================================
*/
CR_API double
fir_nl_median (
  __CR_IN__ double  input,
  __CR_IN__ sint_t  ntaps,
  __CR_IO__ double  z[],
  __CR_IN__ double  t[]
    )
{
    sint_t  ii;
    double  middle;

    z[0] = input;
    mem_cpy(t, z, ntaps * sizeof(double));
    quick_sort(t, ntaps, sizeof(double), value_comp_double);
    middle = t[ntaps / 2];
    for (ii = ntaps - 2; ii >= 0; ii--)
        z[ii + 1] = z[ii];
    return (middle);
}

/*
---------------------------------------
    数值排序比较
---------------------------------------
*/
static sint_t
value_comp_integer (
  __CR_IN__ const void_t*   elem1,
  __CR_IN__ const void_t*   elem2
    )
{
    /* 降序 */
    return (*(sint_t*)elem2 - *(sint_t*)elem1);
}

/*
=======================================
    计数滤波器
=======================================
*/
CR_API double
fir_nl_counts (
  __CR_IN__ sint_t              input,
  __CR_IN__ sint_t              ntaps,
  __CR_IO__ sint_t              z[],
  __CR_IN__ sint_t              t[],
  __CR_IN__ sint_t              rle[],
  __CR_IN__ const sNL_COUNTS*   param
    )
{
    double  value;
    sint_t  middle, total;
    sint_t  ii, jj, kk, mm, vv;

    z[0] = input;

    /* 排序获得计数统计特征 */
    mem_cpy(t, z, ntaps * sizeof(sint_t));
    quick_sort(t, ntaps, sizeof(sint_t), value_comp_integer);
    mem_zero(rle, ntaps * sizeof(sint_t) * 2);

    /* 统计每个数字出现的次数, 当某个数字出现的比例大于阈值时
       使用这个值作为中心值, 否则, 当变化幅度大于等于要求值时
       使用中值作为中心值, 小于要求值时只用均值作为中心值 */
    for (kk = jj = 0; jj < ntaps; jj++) {
        mm = ntaps - jj;
        for (ii = 1;; ii++, jj++) {
            if (ii >= mm || t[jj] != t[jj + 1]) {
                vv = t[jj];
                break;
            }
        }
        rle[kk++] = ii;
        rle[kk++] = vv;
    }

    /* 只有一种值 */
    if (kk == 2)
        return ((double)rle[1]);
    kk /= 2;

    /* 查找出现最多的那个值, 有两个一样表示无法判定 */
    quick_sort(rle, kk, sizeof(sint_t) * 2, value_comp_integer);
    if ((rle[0] == rle[2]) || (((double)rle[0] / ntaps) <= param->percent)) {
        if (param->mode == CR_NL_CNTS_MODE_AUTO) {
            if (t[0] - t[ntaps - 1] < param->v_up + param->v_dn) {
                for (value = 0.0, jj = ii = 0; ii < kk; ii++, jj += 2)
                    value += (double)rle[jj + 1] * rle[jj];
                middle = (sint_t)(value / ntaps + 0.5);
            }
            else {
                middle = t[ntaps / 2];
            }
        }
        else
        if (param->mode == CR_NL_CNTS_MODE_AVGS) {
            for (value = 0.0, jj = ii = 0; ii < kk; ii++, jj += 2)
                value += (double)rle[jj + 1] * rle[jj];
            middle = (sint_t)(value / ntaps + 0.5);
        }
        else {
            middle = t[ntaps / 2];
        }
    }
    else {
        middle = rle[1];
    }

    /* 根据上下振幅去掉超范围值并平均 */
    mm = middle + param->v_up;
    vv = middle - param->v_dn;
    for (value = 0.0, total = jj = ii = 0; ii < kk; ii++, jj += 2) {
        if (rle[jj + 1] > vv && rle[jj + 1] < mm) {
            value += (double)rle[jj + 1] * rle[jj];
            total += rle[jj];
        }
    }

    /* 滑窗滚动 */
    for (ii = ntaps - 2; ii >= 0; ii--)
        z[ii + 1] = z[ii];
    if (total == 0)
        return ((double)middle);
    return (value / total);
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
