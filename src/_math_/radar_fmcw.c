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
    fmcw->fft_back = mem_talloc(fmcw->fft_max, fpxx_t);
    if (fmcw->fft_back == NULL)
        goto _failure2;
    fmcw->fft_bits = mem_talloc(fmcw->bit_size * 2, byte_t);
    if (fmcw->fft_bits == NULL)
        goto _failure3;
    fmcw->fmcw_fft = mem_talloc(fmcw->fft_max, sCOMPLEX);
    if (fmcw->fmcw_fft == NULL)
        goto _failure4;

    /* FFT 初始化 */
    fmcw->fft_coes = fft1_init(NULL, fmcw->npower);
    if (fmcw->fft_coes == NULL)
        goto _failure5;
    mem_zero(fmcw->fft_back, fmcw->fft_max * sizeof(fpxx_t));
    mem_zero(fmcw->fft_bits, fmcw->bit_size * sizeof(int16u));
    mem_zero(fmcw->fmcw_fft, fmcw->fft_max * sizeof(sCOMPLEX));
    return (TRUE);

_failure5:
    mem_free(fmcw->fmcw_fft);
    fmcw->fmcw_fft = NULL;
_failure4:
    mem_free(fmcw->fft_bits);
    fmcw->fft_bits = NULL;
_failure3:
    mem_free(fmcw->fft_back);
    fmcw->fft_back = NULL;
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
    SAFE_FREE(fmcw->fft_back);
    SAFE_FREE(fmcw->fft_data);
    SAFE_FREE(fmcw->fft_coes);
    SAFE_FREE(fmcw->fft_bits);
    SAFE_FREE(fmcw->fft_vals);
    SAFE_FREE(fmcw->fmcw_fft);
}

/*
=======================================
    雷达 FMCW 主流程
=======================================
*/
CR_API bool_t
radar_fmcw_doit (
  __CR_IO__ sFMCW*          fmcw,
  __CR_IN__ const sint_t*   data,
  __CR_IN__ bool_t          reset
    )
{
    sint_t  idx, chalf;

    /* 清上次结果 */
    mem_zero(fmcw->fft_bits, fmcw->bit_size * sizeof(int16u));
    mem_zero(fmcw->fmcw_fft, fmcw->fft_max * sizeof(sCOMPLEX));

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
        return (FALSE);

    /* 计算幅值 */
    chalf = fmcw->fft_cnts / 2;
    for (idx = 0; idx < fmcw->fft_max; idx++) {
        fmcw->fmcw_fft[idx].re = complex_abs(&fmcw->fft_vals[idx]) / chalf;
        fmcw->fmcw_fft[idx].re *= fmcw->fft_mul;
    }

    /* 把峰值标记成有效谱线 */
    if (fmcw->fmcw_fft[0].re >= fmcw->fmcw_fft[1].re) {
        fmcw->fft_bits[0] = 0x80;
        fmcw->fmcw_fft[0].im = complex_ang(&fmcw->fft_vals[0]);
    }
    for (idx = 1; idx < fmcw->fft_max - 1; idx++) {
        if ((fmcw->fmcw_fft[idx].re >= fmcw->fmcw_fft[idx - 1].re) &&
            (fmcw->fmcw_fft[idx].re >= fmcw->fmcw_fft[idx + 1].re)) {
            fmcw->fft_bits[idx / 8] |= (byte_t)(1 << (7 - idx % 8));
            fmcw->fmcw_fft[idx].im = complex_ang(&fmcw->fft_vals[idx]);
        }
    }

    /* 是否进行复位操作 */
    if (reset)
    {
        /* 保存背景频谱和尖峰位置 */
        for (idx = 0; idx < fmcw->fft_max; idx++)
            fmcw->fft_back[idx] = fmcw->fmcw_fft[idx].re;
        mem_cpy(&fmcw->fft_bits[fmcw->bit_size], &fmcw->fft_bits[0],
                                fmcw->bit_size);
    }

    /* 处理完成 */
    return (TRUE);
}

/*
=======================================
    雷达 FMCW 门限过滤操作
=======================================
*/
CR_API void_t
radar_fmcw_cutdown (
  __CR_IN__ const sFMCW*    fmcw,
  __CR_IN__ bool_t          back
    )
{
    byte_t  mask;
    sint_t  idx, cnt;

    if (fmcw->cut_lst == NULL)
    {
        /* 使用固定的阈值 */
        if (back)
        {
            /* 针对背景值 */
            for (idx = 0; idx < fmcw->fft_max; idx++) {
                cnt = fmcw->bit_size + idx / 8;
                mask = (byte_t)(1 << (7 - idx % 8));
                if (!(fmcw->fft_bits[cnt] & mask))
                    continue;
                if (fmcw->fft_back[idx] <= fmcw->cut_fft)
                    fmcw->fft_bits[cnt] &= ~mask;
            }
        }
        else
        {
            /* 针对当前值 */
            for (idx = 0; idx < fmcw->fft_max; idx++) {
                cnt = idx / 8;
                mask = (byte_t)(1 << (7 - idx % 8));
                if (!(fmcw->fft_bits[cnt] & mask))
                    continue;
                if (fmcw->fmcw_fft[idx].re <= fmcw->cut_fft)
                    fmcw->fft_bits[cnt] &= ~mask;
            }
        }
    }
    else
    {
        /* 使用可变的阈值 */
        if (back)
        {
            /* 针对背景值 */
            for (idx = 0; idx < fmcw->fft_max; idx++) {
                cnt = fmcw->bit_size + idx / 8;
                mask = (byte_t)(1 << (7 - idx % 8));
                if (!(fmcw->fft_bits[cnt] & mask))
                    continue;
                if (fmcw->fft_back[idx] <= fmcw->cut_lst[idx])
                    fmcw->fft_bits[cnt] &= ~mask;
            }
        }
        else
        {
            /* 针对当前值 */
            for (idx = 0; idx < fmcw->fft_max; idx++) {
                cnt = idx / 8;
                mask = (byte_t)(1 << (7 - idx % 8));
                if (!(fmcw->fft_bits[cnt] & mask))
                    continue;
                if (fmcw->fmcw_fft[idx].re <= fmcw->cut_lst[idx])
                    fmcw->fft_bits[cnt] &= ~mask;
            }
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
    fpxx_t  k_max = -1;
    sint_t  idx, k_idx = -1;

    for (idx = 0; idx < fmcw->fft_max; idx++)
    {
        /* 跳过非峰值 */
        mask = (byte_t)(1 << (7 - idx % 8));
        if (!(fmcw->fft_bits[idx / 8] & mask)) {
            dist[idx] = -1.0f;
            continue;
        }
        if (k_max < fmcw->fmcw_fft[idx].re) {
            k_max = fmcw->fmcw_fft[idx].re;
            k_idx = idx;
        }
        dist[idx] = radar_fmcw_dist_ex(fmcw, idx);
    }
    return (k_idx);
}

/*
=======================================
    雷达 FMCW 计算距离 (基值)
=======================================
*/
CR_API sint_t
radar_fmcw_base (
  __CR_IN__ const sFMCW*    fmcw,
  __CR_OT__ fp32_t*         dist
    )
{
    byte_t  mask;
    fpxx_t  k_max = -1;
    sint_t  idx, k_idx = -1;

    for (idx = 0; idx < fmcw->fft_max; idx++)
    {
        /* 跳过非峰值 */
        mask = (byte_t)(1 << (7 - idx % 8));
        if (!(fmcw->fft_bits[fmcw->bit_size + idx / 8] & mask)) {
            dist[idx] = -1.0f;
            continue;
        }
        if (k_max < fmcw->fft_back[idx]) {
            k_max = fmcw->fft_back[idx];
            k_idx = idx;
        }
        dist[idx] = radar_fmcw_base_ex(fmcw, idx);
    }
    return (k_idx);
}

/*
=======================================
    雷达 FMCW 谱估计距离
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
    byte_t  mask;
    uint_t  idx, cnt;
    fpxx_t  front, backs;

    if (beg >= end ||
        beg >= (uint_t)fmcw->fft_max ||
        end >= (uint_t)fmcw->fft_max)
        return (0);
    front = backs = 0;

    if (type == 0)
    {
        /* 计算前景值和背景值的差值 */
        for (idx = beg; idx <= end; idx++)
        {
            /* 只计算峰值 */
            cnt = idx / 8;
            mask = (byte_t)(1 << (7 - idx % 8));
            if (fmcw->fft_bits[cnt] & mask)
                front += fmcw->fmcw_fft[idx].re;
            if (fmcw->fft_bits[fmcw->bit_size + cnt] & mask)
                backs += fmcw->fft_back[idx];
        }
        return (front - backs);
    }

    if (type > 0)
    {
        /* 计算前景值 */
        for (idx = beg; idx <= end; idx++)
        {
            /* 只计算峰值 */
            mask = (byte_t)(1 << (7 - idx % 8));
            if (fmcw->fft_bits[idx / 8] & mask)
                front += fmcw->fmcw_fft[idx].re;
        }
        return (front);
    }

    /* 计算背景值 */
    for (idx = beg; idx <= end; idx++)
    {
        /* 只计算峰值 */
        mask = (byte_t)(1 << (7 - idx % 8));
        if (fmcw->fft_bits[fmcw->bit_size + idx / 8] & mask)
            backs += fmcw->fft_back[idx];
    }
    return (backs);
}

/* 代码简化 */
#define FPXX_GET(_idx) \
    *(fpxx_t*)(&vals[(_idx) * step])
/*
---------------------------------------
    雷达 FMCW 计算尖峰能量 (内部)
---------------------------------------
*/
static void_t
radar_fmcw_tango_sum_int (
  __CR_OT__ fpxx_t*         sums,
  __CR_IN__ const byte_t*   vals,
  __CR_IN__ leng_t          step,
  __CR_IN__ const byte_t*   tango,
  __CR_IN__ sint_t          length
    )
{
    byte_t  mask;
    sint_t  idx, fnd;
    fpxx_t  prev, next;

    // 只需计算单边的能量
    sums[0] = 0;
    if (tango[0] & 0x80)
    {
        // 向一边查找拐点
        prev = FPXX_GET(0);
        sums[0] = prev;
        for (fnd = 1; fnd < length; fnd++) {
            next = FPXX_GET(fnd);
            if (next >= prev)
                break;
            prev = next;
            sums[0] += prev * 2;    /* 镜像 */
        }
    }

    // 需要计算两边的能量
    for (idx = 1; idx < length; idx++)
    {
        // 定位到尖峰
        sums[idx] = 0;
        mask = (byte_t)(1 << (7 - idx % 8));
        if (!(tango[idx / 8] & mask))
            continue;

        // 向两边查找拐点
        prev = FPXX_GET(idx);
        sums[idx] = prev;
        for (fnd = idx + 1; fnd < length; fnd++) {
            next = FPXX_GET(fnd);
            if (next >= prev)
                break;
            prev = next;
            sums[idx] += prev;
        }
        prev = FPXX_GET(idx);
        for (fnd = idx - 1; fnd >= 0; fnd--) {
            next = FPXX_GET(fnd);
            if (next >= prev)
                break;
            prev = next;
            sums[idx] += prev;
        }
    }
}

/*
=======================================
    雷达 FMCW 计算尖峰能量
=======================================
*/
CR_API void_t
radar_fmcw_tango_sum (
  __CR_IN__ const sFMCW*    fmcw,
  __CR_OT__ fpxx_t*         sums,
  __CR_IN__ bool_t          back
    )
{
    if (back) {
        radar_fmcw_tango_sum_int(sums, (byte_t*)fmcw->fft_back,
            sizeof(fpxx_t), &fmcw->fft_bits[fmcw->bit_size],
                            fmcw->fft_max);
    }
    else {
        radar_fmcw_tango_sum_int(sums, (byte_t*)fmcw->fmcw_fft,
            sizeof(sCOMPLEX), fmcw->fft_bits, fmcw->fft_max);
    }
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
