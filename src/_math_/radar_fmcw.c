/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2019-03-09  */
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
/*  >>>>>>>>>>>>>>>>>>>>> CrHack 雷达 FMCW 测距函数库 <<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "phylib.h"

#ifndef _CR_NO_STDC_
    #include <math.h>
#endif

#define FMCW_MIN_PWR    8

/*
=======================================
    初始化雷达 FMCW 结构
=======================================
*/
CR_API bool_t
radar_fmcw_init (
  __CR_IO__ sFMCW*  fmcw
    )
{
    /* 参数过滤 */
    if (fmcw->npower < FMCW_MIN_PWR)
        return (FALSE);

    /* 全空间分配 */
    fmcw->fft_cnts = 1 << fmcw->npower;
    fmcw->fft_data = mem_talloc(fmcw->fft_cnts, sCOMPLEX);
    if (fmcw->fft_data == NULL)
        return (FALSE);
    fmcw->fft_vals = mem_talloc(fmcw->fft_cnts, sCOMPLEX);
    if (fmcw->fft_vals == NULL)
        goto _failure1;

    /* 半空间 & 位空间计算 */
    if (fmcw->fft_max > fmcw->fft_cnts / 2)
        fmcw->fft_max = fmcw->fft_cnts / 2;
    fmcw->bit_size = fmcw->fft_max / 8;
    if (fmcw->fft_max % 8 != 0)
        fmcw->bit_size += 1;
    if (fmcw->bit_size < sizeof(int32u))
        fmcw->bit_size = sizeof(int32u);

    /* 缓存区分配 */
    fmcw->fft_temp = mem_talloc(fmcw->fft_max, fpxx_t);
    if (fmcw->fft_temp == NULL)
        goto _failure2;
    fmcw->fft_back = mem_talloc(fmcw->fft_max, fpxx_t);
    if (fmcw->fft_back == NULL)
        goto _failure3;
    fmcw->fft_bits = mem_talloc(fmcw->bit_size, byte_t);
    if (fmcw->fft_bits == NULL)
        goto _failure4;
    fmcw->fmcw_fft = mem_talloc(fmcw->fft_max, sCOMPLEX);
    if (fmcw->fmcw_fft == NULL)
        goto _failure5;

    /* FFT 初始化 */
    fmcw->fft_coes = fft1_init(NULL, fmcw->npower);
    if (fmcw->fft_coes == NULL)
        goto _failure6;
    fmcw->chk_time = 0;
    mem_zero(fmcw->fft_temp, fmcw->fft_max * sizeof(fpxx_t));
    mem_zero(fmcw->fft_back, fmcw->fft_max * sizeof(fpxx_t));
    mem_zero(fmcw->fft_bits, fmcw->bit_size * sizeof(byte_t));
    mem_zero(fmcw->fmcw_fft, fmcw->fft_max * sizeof(sCOMPLEX));
    return (TRUE);

_failure6:
    mem_free(fmcw->fmcw_fft);
    fmcw->fmcw_fft = NULL;
_failure5:
    mem_free(fmcw->fft_bits);
    fmcw->fft_bits = NULL;
_failure4:
    mem_free(fmcw->fft_back);
    fmcw->fft_back = NULL;
_failure3:
    mem_free(fmcw->fft_temp);
    fmcw->fft_temp = NULL;
_failure2:
    mem_free(fmcw->fft_vals);
    fmcw->fft_vals = NULL;
_failure1:
    mem_free(fmcw->fft_data);
    fmcw->fft_data = NULL;
    return (FALSE);
}

/*
=======================================
    释放雷达 FMCW 结构
=======================================
*/
CR_API void_t
radar_fmcw_free (
  __CR_IN__ sFMCW*  fmcw
    )
{
    SAFE_FREE(fmcw->fft_temp);
    SAFE_FREE(fmcw->fft_back);
    SAFE_FREE(fmcw->fft_data);
    SAFE_FREE(fmcw->fft_coes);
    SAFE_FREE(fmcw->fft_bits);
    SAFE_FREE(fmcw->fft_vals);
    SAFE_FREE(fmcw->fmcw_fft);
}

/*
=======================================
    雷达 FMCW 一遍计算
=======================================
*/
CR_API sint_t
radar_fmcw_pass (
  __CR_IO__ sFMCW*          fmcw,
  __CR_IN__ const sint_t*   data,
  __CR_IN__ bool_t          reset
    )
{
    sint_t  cnt, rst;
    sint_t  idx, chalf;

    /* 清上次结果 */
    if (fmcw->chk_time == 0) {
        mem_zero(fmcw->fft_bits, fmcw->bit_size * sizeof(byte_t));
        mem_zero(fmcw->fmcw_fft, fmcw->fft_max * sizeof(sCOMPLEX));
    }

    /* 转成虚数数组 */
    if (fmcw->fft_win != NULL)
    {
        /* 加窗操作 */
        for (idx = 0; idx < fmcw->fft_cnts; idx++) {
            fmcw->fft_data[idx].re = (fpxx_t)data[idx] * fmcw->fft_win[idx];
            fmcw->fft_data[idx].im = 0;
        }
    }
    else
    {
        /* 直接复制 */
        for (idx = 0; idx < fmcw->fft_cnts; idx++) {
            fmcw->fft_data[idx].re = (fpxx_t)data[idx];
            fmcw->fft_data[idx].im = 0;
        }
    }

    /* 计算频谱 */
    if (fft1_func(fmcw->fft_vals, fmcw->fft_data,
            fmcw->fft_coes, fmcw->npower) == NULL)
        return (-1);

    /* 计算幅值 */
    chalf = fmcw->fft_cnts / 2;
    for (idx = 0; idx < fmcw->fft_max; idx++) {
        fmcw->fft_temp[idx] = complex_abs(&fmcw->fft_vals[idx]) / chalf;
        fmcw->fft_temp[idx] *= fmcw->fft_mul;
    }

    /* 谱线采样的类型 */
    if (fmcw->filter == CR_FMCW_DIR || fmcw->totals <= 1)
    {
        /* 单次采样 */
        for (idx = 0; idx < fmcw->fft_max; idx++)
            fmcw->fmcw_fft[idx].re = fmcw->fft_temp[idx];
    }
    else
    {
        /* 多重采样 */
        if (fmcw->chk_time++ < fmcw->totals) {
            switch (fmcw->filter)
            {
                default:
                    break;

                case CR_FMCW_AVG:
                    for (idx = 0; idx < fmcw->fft_max; idx++)
                        fmcw->fmcw_fft[idx].re += fmcw->fft_temp[idx];
                    break;

                case CR_FMCW_MAX:
                    for (idx = 0; idx < fmcw->fft_max; idx++) {
                        if (fmcw->fmcw_fft[idx].re < fmcw->fft_temp[idx])
                            fmcw->fmcw_fft[idx].re = fmcw->fft_temp[idx];
                    }
                    break;
            }
            if (fmcw->chk_time != fmcw->totals)
                return (0);
        }
        if (fmcw->filter == CR_FMCW_AVG) {
            for (idx = 0; idx < fmcw->fft_max; idx++)
                fmcw->fmcw_fft[idx].re /= fmcw->totals;
        }
    }

    /* 是否进行复位操作 */
    if (reset)
    {
        /* 保存背景频谱 */
        for (idx = 0; idx < fmcw->fft_max; idx++)
            fmcw->fft_back[idx] = fmcw->fmcw_fft[idx].re;
    }

    /* 把峰值标记成有效谱线 */
    for (idx = 1; idx < fmcw->fft_max - 1; idx++) {
        if ((fmcw->fmcw_fft[idx].re >= fmcw->fmcw_fft[idx - 1].re) &&
            (fmcw->fmcw_fft[idx].re >= fmcw->fmcw_fft[idx + 1].re)) {
            cnt = idx / 8;
            rst = idx % 8;
            fmcw->fft_bits[cnt] |= (byte_t)(1 << (7 - rst));
        }
    }

    /* 处理完成 */
    fmcw->chk_time = 0;
    return (1);
}

/*
=======================================
    雷达 FMCW 门限过滤操作
=======================================
*/
CR_API void_t
radar_fmcw_cutdown (
  __CR_IN__ const sFMCW*    fmcw,
  __CR_IN__ bool_t          noback
    )
{
    byte_t  mask;
    fpxx_t  value;
    sint_t  idx, cnt, rst;

    if (fmcw->cut_lst == NULL)
    {
        /* 使用固定的阈值 */
        for (idx = 0; idx < fmcw->fft_max; idx++)
        {
            /* 跳过非峰值 */
            cnt = idx / 8;
            rst = idx % 8;
            mask = (byte_t)(1 << (7 - rst));
            if (!(fmcw->fft_bits[cnt] & mask))
                continue;
            value = fmcw->fmcw_fft[idx].re;
            if (noback) value -= fmcw->fft_back[idx];
            if (value <= fmcw->cut_fft)
                fmcw->fft_bits[cnt] &= ~mask;
        }
    }
    else
    {
        /* 使用可变的阈值 */
        for (idx = 0; idx < fmcw->fft_max; idx++)
        {
            /* 跳过非峰值 */
            cnt = idx / 8;
            rst = idx % 8;
            mask = (byte_t)(1 << (7 - rst));
            if (!(fmcw->fft_bits[cnt] & mask))
                continue;
            value = fmcw->fmcw_fft[idx].re;
            if (noback) value -= fmcw->fft_back[idx];
            if (value <= fmcw->cut_lst[idx])
                fmcw->fft_bits[cnt] &= ~mask;
        }
    }
}

/*
=======================================
    雷达 FMCW 计算距离
=======================================
*/
CR_API sint_t
radar_fmcw_dist (
  __CR_IN__ const sFMCW*    fmcw,
  __CR_OT__ fp32_t*         dist
    )
{
    byte_t  mask;
    sint_t  cnt, rst;
    fpxx_t  k_max = -1;
    sint_t  idx, k_idx = -1;

    for (idx = 0; idx < fmcw->fft_max; idx++)
    {
        /* 跳过非峰值 */
        cnt = idx / 8;
        rst = idx % 8;
        mask = (byte_t)(1 << (7 - rst));
        if (!(fmcw->fft_bits[cnt] & mask)) {
            dist[idx] = -1.0f;
            continue;
        }
        if (k_max < fmcw->fmcw_fft[idx].re) {
            k_max = fmcw->fmcw_fft[idx].re;
            k_idx = idx;
        }
        dist[idx] = radar_fmcw_dist_ex(fmcw, idx);

        /* 计算谱线相位 */
        fmcw->fmcw_fft[idx].im = complex_ang(&fmcw->fft_vals[idx]);
    }
    return (k_idx);
}

/*
=======================================
    雷达 FMCW 谱估计距离 (结果)
=======================================
*/
CR_API fp32_t
radar_fmcw_dist_ex (
  __CR_IN__ const sFMCW*    fmcw,
  __CR_IN__ sint_t          k_idx
    )
{
    fpxx_t  cmpa, cmpb, cmpc;
    fpxx_t  ki = (fpxx_t)k_idx;

    if (k_idx > 0 && k_idx < fmcw->fft_max - 1) {
        cmpa = fmcw->fmcw_fft[k_idx - 1].re;
        cmpb = fmcw->fmcw_fft[k_idx    ].re;
        cmpc = fmcw->fmcw_fft[k_idx + 1].re;
        if (XABS(cmpc - cmpa) > CR_ABITX) {
            if (cmpc > cmpa)
                ki += 1 / ((cmpb - cmpc) / (cmpc - cmpa) * 2 + 2);
            else
                ki -= 1 / ((cmpb - cmpa) / (cmpa - cmpc) * 2 + 2);
        }
    }
    return ((fp32_t)(ki * fmcw->ka + fmcw->kb));
}

/*
=======================================
    雷达 FMCW 谱估计距离 (基值)
=======================================
*/
CR_API fp32_t
radar_fmcw_base_ex (
  __CR_IN__ const sFMCW*    fmcw,
  __CR_IN__ sint_t          k_idx
    )
{
    fpxx_t  cmpa, cmpb, cmpc;
    fpxx_t  ki = (fpxx_t)k_idx;

    if (k_idx > 0 && k_idx < fmcw->fft_max - 1) {
        cmpa = fmcw->fft_back[k_idx - 1];
        cmpb = fmcw->fft_back[k_idx    ];
        cmpc = fmcw->fft_back[k_idx + 1];
        if (XABS(cmpc - cmpa) > CR_ABITX) {
            if (cmpc > cmpa)
                ki += 1 / ((cmpb - cmpc) / (cmpc - cmpa) * 2 + 2);
            else
                ki -= 1 / ((cmpb - cmpa) / (cmpa - cmpc) * 2 + 2);
        }
    }
    return ((fp32_t)(ki * fmcw->ka + fmcw->kb));
}

/*
=======================================
    雷达 FMCW 频谱能量累计
=======================================
*/
CR_API fpxx_t
radar_fmcw_power_sum (
  __CR_IN__ const sFMCW*    fmcw,
  __CR_IN__ uint_t          beg,
  __CR_IN__ uint_t          end,
  __CR_IN__ sint_t          type
    )
{
    uint_t  idx;
    fpxx_t  front, backs;

    if (beg >= end ||
        beg >= (uint_t)fmcw->fft_max ||
        end >= (uint_t)fmcw->fft_max)
        return (0);
    front = backs = 0;

    if (type == 0)
    {
        /* 计算前景值和背景值的差值 */
        for (idx = beg; idx <= end; idx++) {
            front += fmcw->fmcw_fft[idx].re;
            backs += fmcw->fft_back[idx];
        }
        return (front - backs);
    }

    if (type > 0)
    {
        /* 计算前景值 */
        for (idx = beg; idx <= end; idx++)
            front += fmcw->fmcw_fft[idx].re;
        return (front);
    }

    /* 计算背景值 */
    for (idx = beg; idx <= end; idx++)
        backs += fmcw->fft_back[idx];
    return (backs);
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
