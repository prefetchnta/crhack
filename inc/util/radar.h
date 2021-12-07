/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2019-03-22  */
/*     #######          ###    ###      [UTIL]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>>>>>> 雷达算法头文件 <<<<<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_RADAR_H__
#define __CR_RADAR_H__

#include "../phylib.h"

/*****************************************************************************/
/*                                   FMCW                                    */
/*****************************************************************************/

/* FMCW 结构 */
typedef struct
{
        /* 输入参数 */
        uint_t          filter;     /* 频谱滤波类型 */
        uint_t          totals;     /* 采集的总次数 */
        fp32_t          ka, kb;     /* 测距标定参数 */
        sint_t          npower;     /* FFT 点数参数 */
        sint_t          fft_max;    /* 最远频谱点位 */
        fpxx_t          cut_pxy;    /* 频谱相关系数 */
        fpxx_t          cut_fft;    /* 幅值掐断门限 */
        const fpxx_t*   cut_lst;    /* 幅值掐断门限 */
        const fpxx_t*   fft_win;    /* FFT 的窗函数 */

        /* 临时缓存 */
        uint_t      chk_time;
        uint_t      bit_size;
        fpxx_t*     fft_tmp1;
        fpxx_t*     fft_tmp2;
        fpxx_t*     fft_back;
        sCOMPLEX*   fft_data;
        sCOMPLEX*   fft_coes;

        /* 返回结果 */
        sint_t      fft_cnts;   /* FFT 实际点数 */
        byte_t*     fft_bits;   /* FFT 有效标志 */
        sCOMPLEX*   fft_vals;   /* FFT 变换的值 */
        sCOMPLEX*   fmcw_fft;   /* FMCW 返回结果 */
} sFMCW;

/* FMCW 滤波 */
#define CR_FMCW_DIR     0   /* 直接使用 */
#define CR_FMCW_AVG     1   /* 均值滤波 */
#define CR_FMCW_MAX     2   /* 大值滤波 */

CR_API bool_t   radar_fmcw_init (sFMCW *fmcw);
CR_API void_t   radar_fmcw_free (sFMCW *fmcw);
CR_API sint_t   radar_fmcw_pass (sFMCW *fmcw, const sint_t *data,
                                 bool_t reset);
CR_API void_t   radar_fmcw_cutdown (const sFMCW *fmcw);
CR_API sint_t   radar_fmcw_dist (const sFMCW *fmcw, fp32_t *dist);
CR_API fp32_t   radar_fmcw_dist_ex (const sFMCW *fmcw, sint_t k_idx);
CR_API fp32_t   radar_fmcw_base_ex (const sFMCW *fmcw, sint_t k_idx);

/*****************************************************************************/
/*                                   CFAR                                    */
/*****************************************************************************/

CR_API void_t   radar_cfar_ca (fpxx_t *cut_lst, const sFMCW *input,
                               fpxx_t param_mul, fpxx_t param_add,
                               sint_t guard_len, sint_t noise_len);
CR_API void_t   radar_cfar_so (fpxx_t *cut_lst, const sFMCW *input,
                               fpxx_t param_mul, fpxx_t param_add,
                               sint_t guard_len, sint_t noise_len);
CR_API void_t   radar_cfar_go (fpxx_t *cut_lst, const sFMCW *input,
                               fpxx_t param_mul, fpxx_t param_add,
                               sint_t guard_len, sint_t noise_len);
CR_API bool_t   radar_cfar_os (fpxx_t *cut_lst, const sFMCW *input,
                               fpxx_t param_mul, fpxx_t param_add,
                               sint_t guard_len, sint_t noise_len,
                               sint_t select);

#endif  /* !__CR_RADAR_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
