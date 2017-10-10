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

/*
=======================================
    线段方向倒转
=======================================
*/
CR_API void_t
line_swap (
  __CR_IO__ sPNT2*  pnts,
  __CR_IN__ leng_t  count
    )
{
    sPNT2   tmp;
    leng_t  idx, num = count / 2;

    count -= 1;
    for (idx = 0; idx < num; idx++) {
        struct_cpy(&tmp, &pnts[idx], sPNT2);
        struct_cpy(&pnts[idx], &pnts[count - idx], sPNT2);
        struct_cpy(&pnts[count - idx], &tmp, sPNT2);
    }
}

/*
=======================================
    线段点集压缩
=======================================
*/
CR_API leng_t
line_compress (
  __CR_IO__ sPNT2*  pnts,
  __CR_IN__ leng_t  count
    )
{
    sPNT2   dir;
    sPNT2   tmp;
    leng_t  idx;

    /* 合并方向一致的多余点 */
    if (count <= 2)
        return (count);
    dir.x = pnts[1].x - pnts[0].x;
    dir.y = pnts[1].y - pnts[0].y;
    for (idx = 1; idx < count - 1;) {
        tmp.x = pnts[idx + 1].x - pnts[idx].x;
        tmp.y = pnts[idx + 1].y - pnts[idx].y;
        if (tmp.x == dir.x && tmp.y == dir.y) {
            count -= 1;
            mem_cpy(&pnts[idx], &pnts[idx + 1],
                (count - idx) * sizeof(sPNT2));
        }
        else {
            struct_cpy(&dir, &tmp, sPNT2);
            idx += 1;
        }
    }
    return (count);
}

/*
---------------------------------------
    线段点集分割 (内部)
---------------------------------------
*/
static void_t
line_split_int (
  __CR_IO__ sPNT2*  pnts,
  __CR_IN__ leng_t  count,
  __CR_IN__ fp32_t  gate
    )
{
    leng_t  idx, split;
    fp32_t  aa, bb, cc;
    fp32_t  len, max, dist;

    /* 计算直线方程参数 */
    if (count <= 2)
        return;
    aa = (fp32_t)(pnts[0].y - pnts[count - 1].y);
    bb = (fp32_t)(pnts[count - 1].x - pnts[0].x);
    cc = aa * (fp32_t)(pnts[0].x) + bb * (fp32_t)(pnts[0].y);
    len = 1.0f / FSQRT(aa * aa + bb * bb);
    max = -1.0f;
    split = 0;

    /* 找出点集到直线的最大距离 */
    for (idx = 1; idx <= count - 2; idx++) {
        dist = aa * (fp32_t)(pnts[idx].x) + bb * (fp32_t)(pnts[idx].y) + cc;
        dist *= len;
        if (dist < 0.0f)
            dist = -dist;
        if (dist > max) {
            max = dist;
            split = idx;
        }
    }

    /* 最大距离小于阈值可视为一条直线 */
    if (max < gate)
    {
        /* 中间所有点用起始点填充表示无效 */
        for (idx = 1; idx <= count - 2; idx++) {
            pnts[idx].x = pnts[0].x;
            pnts[idx].y = pnts[0].y;
        }
    }
    else
    {
        /* 递归处理分开的两段 */
        line_split_int(&pnts[0], split + 1, gate);
        line_split_int(&pnts[split], count - split, gate);
    }
}

/*
=======================================
    删除无效的点
=======================================
*/
CR_API leng_t
line_clean (
  __CR_IO__ sPNT2*  pnts,
  __CR_IN__ leng_t  count
    )
{
    leng_t  idx;

    for (idx = 0; idx < count - 1;) {
        if (pnts[idx].x == pnts[idx + 1].x &&
            pnts[idx].y == pnts[idx + 1].y) {
            count -= 1;
            mem_cpy(&pnts[idx], &pnts[idx + 1],
                (count - idx) * sizeof(sPNT2));
        }
        else {
            idx += 1;
        }
    }
    return (count);
}

/*
=======================================
    线段点集分割
=======================================
*/
CR_API leng_t
line_split (
  __CR_IO__ sPNT2*  pnts,
  __CR_IN__ leng_t  count,
  __CR_IN__ fp32_t  gate
    )
{
    if (count <= 2 || gate <= 0.0f)
        return (count);
    line_split_int(pnts, count, gate);
    return (line_clean(pnts, count));
}

/*
=======================================
    直线直角修补
=======================================
*/
CR_API leng_t
line_corner (
  __CR_IO__ sPNT2*  pnts,
  __CR_IN__ leng_t  count,
  __CR_IN__ sint_t  gmin,
  __CR_IN__ sint_t  gmax
    )
{
    sPNT2*  dir;
    leng_t  idx;
    leng_t  len;

    /* 参数过滤 */
    if (count <= 2 ||
        gmin <= 0 || gmax <= 0)
        return (count);

    /* 计算线段的斜率 */
    len = count - 1;
    dir = mem_talloc(len, sPNT2);
    if (dir == NULL)
        return (count);
    for (idx = 0; idx < len; idx++) {
        dir[idx].x = pnts[idx + 1].x - pnts[idx].x;
        dir[idx].y = pnts[idx + 1].y - pnts[idx].y;
        if (dir[idx].x < 0)
            dir[idx].x = -dir[idx].x;
        if (dir[idx].y < 0)
            dir[idx].y = -dir[idx].y;
        if (dir[idx].x == 0 && dir[idx].y == 0) {
            mem_free(dir);
            return (count);
        }
    }

    /* 查找符合条件的直角 */
    for (idx = 0; idx < len - 2; idx++)
    {
        /* 首先, 必须是直角 */
        if (dir[idx].x * dir[idx + 2].x == 0 &&
            dir[idx].y * dir[idx + 2].y == 0)
        {
            /* 其次, 两条直边必须都长于一个阈值 */
            if (dir[idx].x + dir[idx + 2].x <= gmax ||
                dir[idx].y + dir[idx + 2].y <= gmax) {
                idx += 1;
                continue;
            }

            /* 最后, 中间斜边的倾斜必须大于一定程度 */
            if (dir[idx + 1].x >= gmin || dir[idx + 1].y >= gmin) {
                idx += 1;
                continue;
            }

            /* 处理掉那个斜边拐角 */
            if (dir[idx].x == 0)
            {
                /* 先竖再横 */
                pnts[idx + 1].y = pnts[idx + 2].y;
            }
            else
            {
                /* 先横再竖 */
                pnts[idx + 1].x = pnts[idx + 2].x;
            }

            /* 删掉下一个点 */
            pnts[idx + 2].x = pnts[idx + 1].x;
            pnts[idx + 2].y = pnts[idx + 1].y;
            idx += 1;
        }
    }
    mem_free(dir);
    return (line_clean(pnts, count));
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
