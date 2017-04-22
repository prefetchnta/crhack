/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2013-05-24  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack 直线几何运算函数库 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "memlib.h"
#include "phylib.h"

#ifndef _CR_NO_STDC_
    #include <math.h>
#endif

/*
=======================================
    标准直线 Hough 变换
=======================================
*/
CR_API bool_t
line_hough (
  __CR_OT__ sLINE_HOUGH*    hough,
  __CR_IN__ const sIMAGE*   img,
  __CR_IN__ fp32_t          rho,
  __CR_IN__ fp32_t          theta,
  __CR_IN__ uint_t          gate,
  __CR_IN__ byte_t          index
    )
{
    byte_t  *line;
    sint_t  xx, yy, ww, hh, rr;
    fp32_t  *tsin, *tcos, ang, irho;
    uint_t  *accum, idx, num, n_ang, n_rho, xmove;

    /* 参数过滤 */
    if (img->fmt != CR_INDEX8 ||
        rho <= 0.0f || theta <= 0.0f)
        return (FALSE);

    /* 初始化一些常量 */
    n_ang = (sint_t)(CR_PI / theta + 0.5f);
    tsin = mem_talloc(n_ang, fp32_t);
    if (tsin == NULL)
        return (FALSE);
    tcos = mem_talloc(n_ang, fp32_t);
    if (tcos == NULL) {
        mem_free(tsin);
        return (FALSE);
    }
    ww = (sint_t)img->position.ww;
    hh = (sint_t)img->position.hh;
    n_rho = (sint_t)(((ww + hh) * 2 + 1) / rho + 0.5f);
    xmove = (n_rho - 1) / 2;
    num = n_ang * n_rho;
    accum = mem_talloc(num, uint_t);
    if (accum == NULL) {
        mem_free(tcos);
        mem_free(tsin);
        return (FALSE);
    }
    irho = 1.0f / rho;
    mem_tzero(accum, num, uint_t);
    for (ang = 0.0f, idx = 0; idx < n_ang; idx++, ang += theta) {
        tsin[idx] = FSIN(ang * irho);
        tcos[idx] = FCOS(ang * irho);
    }

    /* 计算累积缓冲 */
    line = img->data;
    for (yy = 0; yy < hh; yy++)
    {
        for (xx = 0; xx < ww; xx++)
        {
            /* 只找指定颜色 */
            if (line[xx] != index)
                continue;
            for (idx = 0; idx < n_ang; idx++) {
                rr = fp32_to_sint(xx * tcos[idx] + yy * tsin[idx]);
                rr += xmove;
                accum[idx * n_rho + rr] += 1;
            }
        }
        line += img->bpl;
    }

    /* 删除过短的直线 */
    for (idx = 0; idx < num; idx++) {
        if (accum[idx] < gate)
            accum[idx] = 0;
    }

    /* 返回结果 */
    mem_free(tcos);
    mem_free(tsin);
    hough->asize = num;
    hough->n_rho = n_rho;
    hough->n_ang = n_ang;
    hough->xmove = xmove;
    hough->accum = accum;
    return (TRUE);
}

/*
=======================================
    获取直线极坐标参数
=======================================
*/
CR_API bool_t
line_hough_get (
  __CR_IN__ uint_t              idx,
  __CR_IO__ fp32_t*             rho,
  __CR_IO__ fp32_t*             theta,
  __CR_IN__ const sLINE_HOUGH*  hough
    )
{
    uint_t  rr, nn;
    sint_t  xx, yy;

    /* 计算极坐标参数 */
    if (idx >= hough->asize)
        return (FALSE);
    nn = idx / hough->n_rho;
    rr = idx % hough->n_rho;
    xx = (sint_t)(rr - hough->xmove);
    yy = (sint_t)(nn);
    *rho *= xx;
    *theta *= yy;
    return (TRUE);
}

/*
=======================================
    获取最长直线索引值
=======================================
*/
CR_API uint_t
line_hough_max (
  __CR_IN__ const sLINE_HOUGH*  hough
    )
{
    uint_t  idx, max = 0;
    uint_t  retc = hough->asize + 1;

    for (idx = 0; idx < hough->asize; idx++) {
        if (hough->accum[idx] > max) {
            max = hough->accum[idx];
            retc = idx;
        }
    }
    return (retc);
}

/*
=======================================
    释放 Hough 变换结果
=======================================
*/
CR_API void_t
line_hough_free (
  __CR_IN__ sLINE_HOUGH*    hough
    )
{
    SAFE_FREE(hough->accum);
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
