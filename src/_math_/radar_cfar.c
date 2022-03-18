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
/*  >>>>>>>>>>>>>>>>>>>>>>>>> 雷达 CFAR 滤波函数库 <<<<<<<<<<<<<<<<<<<<<<<<  */
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
    fpxx_t  sum, sum_left = 0, sum_right = 0;

    /* 跳过 */
    for (out_idx = 0; out_idx < guard_len; out_idx++)
        cut_lst[out_idx] = CR_LARGX;

    /* 单面 */
    param_mul /= 2;
    for (idx = 0; idx < noise_len; idx++)
        sum_right += input->fmcw_fft[idx + guard_len + 1].re;
    cut_lst[out_idx++] = sum_right * param_mul + param_add;
    idx_left_next = guard_len;
    idx_right_prev = guard_len + 1;
    idx_right_next = idx_right_prev + noise_len;
    for (idx = 0; idx < noise_len - 1; idx++) {
        sum_left += input->fmcw_fft[idx_left_next++].re;
        sum_right += input->fmcw_fft[idx_right_next++].re;
        sum_right -= input->fmcw_fft[idx_right_prev++].re;
        cut_lst[out_idx++] = sum_right * param_mul + param_add;
    }

    /* 双面 */
    param_mul *= 2;
    sum_left += input->fmcw_fft[idx_left_next++].re;
    sum_right += input->fmcw_fft[idx_right_next++].re;
    sum_right -= input->fmcw_fft[idx_right_prev++].re;
    sum = sum_left + sum_right;
    cut_lst[out_idx++] = sum * param_mul + param_add;
    idx_left_prev = guard_len;
    length = input->fft_max - guard_len - 2 * noise_len - 1;
    for (idx = 0; idx < length; idx++) {
        sum_left += input->fmcw_fft[idx_left_next++].re;
        sum_left -= input->fmcw_fft[idx_left_prev++].re;
        sum_right += input->fmcw_fft[idx_right_next++].re;
        sum_right -= input->fmcw_fft[idx_right_prev++].re;
        sum = sum_left + sum_right;
        cut_lst[out_idx++] = sum * param_mul + param_add;
    }

    /* 单面 */
    param_mul /= 2;
    for (idx = 0; idx < noise_len; idx++) {
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
    fpxx_t  sum, sum_left = 0, sum_right = 0;

    /* 跳过 */
    for (out_idx = 0; out_idx < guard_len; out_idx++)
        cut_lst[out_idx] = CR_LARGX;

    /* 单面 */
    param_mul /= 2;
    for (idx = 0; idx < noise_len; idx++)
        sum_right += input->fmcw_fft[idx + guard_len + 1].re;
    cut_lst[out_idx++] = sum_right * param_mul + param_add;
    idx_left_next = guard_len;
    idx_right_prev = guard_len + 1;
    idx_right_next = idx_right_prev + noise_len;
    for (idx = 0; idx < noise_len - 1; idx++) {
        sum_left += input->fmcw_fft[idx_left_next++].re;
        sum_right += input->fmcw_fft[idx_right_next++].re;
        sum_right -= input->fmcw_fft[idx_right_prev++].re;
        cut_lst[out_idx++] = sum_right * param_mul + param_add;
    }

    /* 双面 */
    sum_left += input->fmcw_fft[idx_left_next++].re;
    sum_right += input->fmcw_fft[idx_right_next++].re;
    sum_right -= input->fmcw_fft[idx_right_prev++].re;
    sum = (sum_left < sum_right) ? sum_left : sum_right;
    cut_lst[out_idx++] = sum * param_mul + param_add;
    idx_left_prev = guard_len;
    length = input->fft_max - guard_len - 2 * noise_len - 1;
    for (idx = 0; idx < length; idx++) {
        sum_left += input->fmcw_fft[idx_left_next++].re;
        sum_left -= input->fmcw_fft[idx_left_prev++].re;
        sum_right += input->fmcw_fft[idx_right_next++].re;
        sum_right -= input->fmcw_fft[idx_right_prev++].re;
        sum = (sum_left < sum_right) ? sum_left : sum_right;
        cut_lst[out_idx++] = sum * param_mul + param_add;
    }

    /* 单面 */
    for (idx = 0; idx < noise_len; idx++) {
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
    fpxx_t  sum, sum_left = 0, sum_right = 0;

    /* 跳过 */
    for (out_idx = 0; out_idx < guard_len; out_idx++)
        cut_lst[out_idx] = CR_LARGX;

    /* 单面 */
    param_mul /= 2;
    for (idx = 0; idx < noise_len; idx++)
        sum_right += input->fmcw_fft[idx + guard_len + 1].re;
    cut_lst[out_idx++] = sum_right * param_mul + param_add;
    idx_left_next = guard_len;
    idx_right_prev = guard_len + 1;
    idx_right_next = idx_right_prev + noise_len;
    for (idx = 0; idx < noise_len - 1; idx++) {
        sum_left += input->fmcw_fft[idx_left_next++].re;
        sum_right += input->fmcw_fft[idx_right_next++].re;
        sum_right -= input->fmcw_fft[idx_right_prev++].re;
        cut_lst[out_idx++] = sum_right * param_mul + param_add;
    }

    /* 双面 */
    sum_left += input->fmcw_fft[idx_left_next++].re;
    sum_right += input->fmcw_fft[idx_right_next++].re;
    sum_right -= input->fmcw_fft[idx_right_prev++].re;
    sum = (sum_left > sum_right) ? sum_left : sum_right;
    cut_lst[out_idx++] = sum * param_mul + param_add;
    idx_left_prev = guard_len;
    length = input->fft_max - guard_len - 2 * noise_len - 1;
    for (idx = 0; idx < length; idx++) {
        sum_left += input->fmcw_fft[idx_left_next++].re;
        sum_left -= input->fmcw_fft[idx_left_prev++].re;
        sum_right += input->fmcw_fft[idx_right_next++].re;
        sum_right -= input->fmcw_fft[idx_right_prev++].re;
        sum = (sum_left > sum_right) ? sum_left : sum_right;
        cut_lst[out_idx++] = sum * param_mul + param_add;
    }

    /* 单面 */
    for (idx = 0; idx < noise_len; idx++) {
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
  __CR_IN__ sint_t          select
    )
{
    fpxx_t  *buffer;
    sint_t  ptr, len;
    sint_t  idx, out_idx;

    /* 分配临时缓冲 */
    len = noise_len * 2;
    buffer = mem_talloc(len, fpxx_t);
    if (buffer == NULL)
        return (FALSE);

    /* 默认参数设置 */
    if (select < noise_len || select >= len)
        select = 3 * len / 4;

    /* 跳过 */
    for (out_idx = 0; out_idx < guard_len; out_idx++)
        cut_lst[out_idx] = CR_LARGX;

    /* 滑窗 */
    for (; out_idx < input->fft_max; out_idx++)
    {
        ptr = 0;

        /* 复制数据到滑窗 */
        for (idx = out_idx - noise_len; idx <= out_idx + noise_len; idx++)
        {
            /* 跳过自己 */
            if (idx == out_idx)
                continue;

            /* 超出边界的值放弃 */
            if (idx < guard_len || idx >= input->fft_max)
                buffer[ptr] = 0;
            else
                buffer[ptr] = input->fmcw_fft[idx].re;
            ptr += 1;
        }

        /* 升序排序并计算阈值 */
        quick_sort(buffer, len, sizeof(fpxx_t), radar_cfar_os_comp);
        cut_lst[out_idx] = buffer[select] * param_mul + param_add;
    }

    /* 释放 */
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
