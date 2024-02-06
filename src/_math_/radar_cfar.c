/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2019-11-04  */
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
/*  >>>>>>>>>>>>>>>>>>>>> CrHack 雷达 CFAR 滤波函数库 <<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "msclib.h"
#include "phylib.h"

/*
=======================================
    生成 CFAR-CA 阈值列表
=======================================
*/
CR_API void_t
radar_cfar_ca (
  __CR_OT__ fpxx_t*         cut_lst,
  __CR_IN__ const sFMCW*    input,
  __CR_IN__ fpxx_t          param_mul,
  __CR_IN__ fpxx_t          param_add,
  __CR_IN__ sint_t          guard_len,
  __CR_IN__ sint_t          noise_len
    )
{
    sint_t  idx, out_idx, length;
    sint_t  idx_left_next, idx_left_prev;
    sint_t  idx_right_next, idx_right_prev;
    fpxx_t  sum, sum_left, sum_right;

    /* 检查 */
    if (noise_len <= 0 || guard_len < 0 ||
        input->fft_max <= 2 * (noise_len + guard_len))
        return;
    out_idx = 0;
    sum_left = sum_right = 0;

    /* 右面 */
    param_mul /= 2;
    for (idx = 0; idx < noise_len; idx++)
        sum_right += input->fmcw_fft[idx + guard_len + 1].re;
    cut_lst[out_idx++] = sum_right * param_mul + param_add;
    idx_left_next = 0;
    idx_right_prev = guard_len + 1;
    idx_right_next = idx_right_prev + noise_len;
    length = noise_len + guard_len - 1;
    for (idx = 0; idx < length; idx++) {
        if (idx < noise_len)
            sum_left += input->fmcw_fft[idx_left_next++].re;
        sum_right += input->fmcw_fft[idx_right_next++].re;
        sum_right -= input->fmcw_fft[idx_right_prev++].re;
        cut_lst[out_idx++] = sum_right * param_mul + param_add;
    }

    /* 双面 */
    param_mul *= 2;
    sum_right += input->fmcw_fft[idx_right_next++].re;
    sum_right -= input->fmcw_fft[idx_right_prev++].re;
    sum = sum_left + sum_right;
    cut_lst[out_idx++] = sum * param_mul + param_add;
    idx_left_prev = 0;
    length = input->fft_max - 2 * (noise_len + guard_len) - 1;
    for (idx = 0; idx < length; idx++) {
        sum_left += input->fmcw_fft[idx_left_next++].re;
        sum_left -= input->fmcw_fft[idx_left_prev++].re;
        sum_right += input->fmcw_fft[idx_right_next++].re;
        sum_right -= input->fmcw_fft[idx_right_prev++].re;
        sum = sum_left + sum_right;
        cut_lst[out_idx++] = sum * param_mul + param_add;
    }

    /* 左面 */
    param_mul /= 2;
    length = noise_len + guard_len;
    for (idx = 0; idx < length; idx++) {
        sum_left += input->fmcw_fft[idx_left_next++].re;
        sum_left -= input->fmcw_fft[idx_left_prev++].re;
        cut_lst[out_idx++] = sum_left * param_mul + param_add;
    }
}

/*
=======================================
    生成 CFAR-SO 阈值列表
=======================================
*/
CR_API void_t
radar_cfar_so (
  __CR_OT__ fpxx_t*         cut_lst,
  __CR_IN__ const sFMCW*    input,
  __CR_IN__ fpxx_t          param_mul,
  __CR_IN__ fpxx_t          param_add,
  __CR_IN__ sint_t          guard_len,
  __CR_IN__ sint_t          noise_len
    )
{
    sint_t  idx, out_idx, length;
    sint_t  idx_left_next, idx_left_prev;
    sint_t  idx_right_next, idx_right_prev;
    fpxx_t  sum, sum_left, sum_right;

    /* 检查 */
    if (noise_len <= 0 || guard_len < 0 ||
        input->fft_max <= 2 * (noise_len + guard_len))
        return;
    out_idx = 0;
    sum_left = sum_right = 0;

    /* 右面 */
    param_mul /= 2;
    for (idx = 0; idx < noise_len; idx++)
        sum_right += input->fmcw_fft[idx + guard_len + 1].re;
    cut_lst[out_idx++] = sum_right * param_mul + param_add;
    idx_left_next = 0;
    idx_right_prev = guard_len + 1;
    idx_right_next = idx_right_prev + noise_len;
    length = noise_len + guard_len - 1;
    for (idx = 0; idx < length; idx++) {
        if (idx < noise_len)
            sum_left += input->fmcw_fft[idx_left_next++].re;
        sum_right += input->fmcw_fft[idx_right_next++].re;
        sum_right -= input->fmcw_fft[idx_right_prev++].re;
        cut_lst[out_idx++] = sum_right * param_mul + param_add;
    }

    /* 双面 */
    param_mul *= 2;
    sum_right += input->fmcw_fft[idx_right_next++].re;
    sum_right -= input->fmcw_fft[idx_right_prev++].re;
    sum = (sum_left < sum_right) ? sum_left : sum_right;
    cut_lst[out_idx++] = sum * param_mul + param_add;
    idx_left_prev = 0;
    length = input->fft_max - 2 * (noise_len + guard_len) - 1;
    for (idx = 0; idx < length; idx++) {
        sum_left += input->fmcw_fft[idx_left_next++].re;
        sum_left -= input->fmcw_fft[idx_left_prev++].re;
        sum_right += input->fmcw_fft[idx_right_next++].re;
        sum_right -= input->fmcw_fft[idx_right_prev++].re;
        sum = (sum_left < sum_right) ? sum_left : sum_right;
        cut_lst[out_idx++] = sum * param_mul + param_add;
    }

    /* 左面 */
    param_mul /= 2;
    length = noise_len + guard_len;
    for (idx = 0; idx < length; idx++) {
        sum_left += input->fmcw_fft[idx_left_next++].re;
        sum_left -= input->fmcw_fft[idx_left_prev++].re;
        cut_lst[out_idx++] = sum_left * param_mul + param_add;
    }
}

/*
=======================================
    生成 CFAR-GO 阈值列表
=======================================
*/
CR_API void_t
radar_cfar_go (
  __CR_OT__ fpxx_t*         cut_lst,
  __CR_IN__ const sFMCW*    input,
  __CR_IN__ fpxx_t          param_mul,
  __CR_IN__ fpxx_t          param_add,
  __CR_IN__ sint_t          guard_len,
  __CR_IN__ sint_t          noise_len
    )
{
    sint_t  idx, out_idx, length;
    sint_t  idx_left_next, idx_left_prev;
    sint_t  idx_right_next, idx_right_prev;
    fpxx_t  sum, sum_left, sum_right;

    /* 检查 */
    if (noise_len <= 0 || guard_len < 0 ||
        input->fft_max <= 2 * (noise_len + guard_len))
        return;
    out_idx = 0;
    sum_left = sum_right = 0;

    /* 右面 */
    param_mul /= 2;
    for (idx = 0; idx < noise_len; idx++)
        sum_right += input->fmcw_fft[idx + guard_len + 1].re;
    cut_lst[out_idx++] = sum_right * param_mul + param_add;
    idx_left_next = 0;
    idx_right_prev = guard_len + 1;
    idx_right_next = idx_right_prev + noise_len;
    length = noise_len + guard_len - 1;
    for (idx = 0; idx < length; idx++) {
        if (idx < noise_len)
            sum_left += input->fmcw_fft[idx_left_next++].re;
        sum_right += input->fmcw_fft[idx_right_next++].re;
        sum_right -= input->fmcw_fft[idx_right_prev++].re;
        cut_lst[out_idx++] = sum_right * param_mul + param_add;
    }

    /* 双面 */
    param_mul *= 2;
    sum_right += input->fmcw_fft[idx_right_next++].re;
    sum_right -= input->fmcw_fft[idx_right_prev++].re;
    sum = (sum_left > sum_right) ? sum_left : sum_right;
    cut_lst[out_idx++] = sum * param_mul + param_add;
    idx_left_prev = 0;
    length = input->fft_max - 2 * (noise_len + guard_len) - 1;
    for (idx = 0; idx < length; idx++) {
        sum_left += input->fmcw_fft[idx_left_next++].re;
        sum_left -= input->fmcw_fft[idx_left_prev++].re;
        sum_right += input->fmcw_fft[idx_right_next++].re;
        sum_right -= input->fmcw_fft[idx_right_prev++].re;
        sum = (sum_left > sum_right) ? sum_left : sum_right;
        cut_lst[out_idx++] = sum * param_mul + param_add;
    }

    /* 左面 */
    param_mul /= 2;
    length = noise_len + guard_len;
    for (idx = 0; idx < length; idx++) {
        sum_left += input->fmcw_fft[idx_left_next++].re;
        sum_left -= input->fmcw_fft[idx_left_prev++].re;
        cut_lst[out_idx++] = sum_left * param_mul + param_add;
    }
}

/*
---------------------------------------
    数值排序比较
---------------------------------------
*/
static sint_t
radar_cfar_os_comp (
  __CR_IN__ const void_t*   elem1,
  __CR_IN__ const void_t*   elem2
    )
{
    fpxx_t  val1 = *(fpxx_t*)elem1;
    fpxx_t  val2 = *(fpxx_t*)elem2;

    /* 升序 */
    if (val1 < val2) return (-1);
    if (val1 > val2) return ( 1);
    return (0);
}

/*
=======================================
    生成 CFAR-OS 阈值列表
=======================================
*/
CR_API bool_t
radar_cfar_os (
  __CR_OT__ fpxx_t*         cut_lst,
  __CR_IN__ const sFMCW*    input,
  __CR_IN__ fpxx_t          param_mul,
  __CR_IN__ fpxx_t          param_add,
  __CR_IN__ sint_t          guard_len,
  __CR_IN__ sint_t          noise_len,
  __CR_IN__ fp32_t          select,
  __CR_IN__ fpxx_t*         buffer
    )
{
    sint_t  idx, out_idx;
    sint_t  ptr, beg, end, len;

    /* 检查 */
    if (noise_len <= 0 || guard_len < 0 ||
        input->fft_max <= 2 * (noise_len + guard_len))
        return (FALSE);
    if (buffer == NULL) {
        len = noise_len * 2;
        buffer = mem_talloc(len, fpxx_t);
        if (buffer == NULL)
            return (FALSE);
    }
    else {
        len = 0;
    }
    if (select <= 0.5f || select >= 1.0f)
        select = 0.75f;

    /* 滑窗 */
    for (out_idx = 0; out_idx < input->fft_max; out_idx++)
    {
        /* 边界 */
        beg = out_idx - guard_len - noise_len;
        end = out_idx + guard_len + noise_len;

        /* 复制 */
        for (ptr = 0, idx = beg; idx <= end; idx++)
        {
            /* 跳过 */
            if (idx >= out_idx - guard_len && idx <= out_idx + guard_len)
                continue;
            if (idx >= 0 && idx < input->fft_max)
                buffer[ptr++] = input->fmcw_fft[idx].re;
        }

        /* 排序 */
        quick_sort(buffer, ptr, sizeof(fpxx_t), radar_cfar_os_comp);

        /* 选择 */
        ptr = (sint_t)(ptr * select) - 1;
        cut_lst[out_idx] = buffer[ptr] * param_mul + param_add;
    }

    /* 释放 */
    if (len != 0)
        mem_free(buffer);
    return (TRUE);
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
