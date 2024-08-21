/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2019-01-21  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack 统计相关计算函数库 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "msclib.h"
#include "phylib.h"

#ifndef _CR_NO_STDC_
    #include <math.h>
#endif

/*
=======================================
    均值/方差/标准差计算
=======================================
*/
CR_API fpxx_t
statistics (
  __CR_IN__ const fpxx_t*   data,
  __CR_IN__ uint_t          count,
  __CR_OT__ fpxx_t*         sd2,
  __CR_OT__ fpxx_t*         sd
    )
{
    uint_t  idx;
    fpxx_t  acc, tmp;
    fpxx_t  num, avg = 0;

    /* 平均值 */
    num = (fpxx_t)count;
    for (idx = 0; idx < count; idx++)
        avg += data[idx];
    avg /= num;

    if (sd2 != NULL || sd != NULL)
    {
        /* 方差 */
        for (acc = 0, idx = 0; idx < count; idx++) {
            tmp = data[idx] - avg;
            acc += tmp * tmp;
        }
        acc /= num;
        if (sd2 != NULL)
            *sd2 = acc;

        /* 标准差 */
        if (sd != NULL)
            *sd = XSQRT(acc);
    }
    return (avg);
}

/*
=======================================
    协方差/相关系数计算
=======================================
*/
CR_API fpxx_t
covariance (
  __CR_IN__ const fpxx_t*   x,
  __CR_IN__ const fpxx_t*   y,
  __CR_IN__ uint_t          count,
  __CR_OT__ fpxx_t*         pxy
    )
{
    uint_t  idx;
    fpxx_t  num, exy = 0;
    fpxx_t  ex, ey, sdx, sdy;

    /* 计算均值和方差 */
    if (pxy != NULL) {
        ex = statistics(x, count, &sdx, NULL);
        ey = statistics(y, count, &sdy, NULL);
    }
    else {
        ex = statistics(x, count, NULL, NULL);
        ey = statistics(y, count, NULL, NULL);
        sdx = sdy = 1;
    }

    /* 计算协方差 */
    num = (fpxx_t)count;
    for (idx = 0; idx < count; idx++)
        exy += x[idx] * y[idx];
    exy /= num;
    exy -= ex * ey;

    /* 计算相关系数 */
    if (pxy != NULL) {
        sdx *= sdy;
        if (sdx <= CR_ABITX)
            *pxy = (XABS(exy) <= CR_ABITX) ? 1 : 0;
        else
            *pxy = exy / XSQRT(sdx);
    }
    return (exy);
}

/*
=======================================
    皮尔逊相似度
=======================================
*/
CR_API fpxx_t
sim_pearson (
  __CR_IN__ const fpxx_t*   x,
  __CR_IN__ const fpxx_t*   y,
  __CR_IN__ uint_t          num
    )
{
    fpxx_t  pxy;

    covariance(x, y, num, &pxy);
    return (pxy);
}

/*
=======================================
    欧几里得相似度
=======================================
*/
CR_API fpxx_t
sim_euclidean (
  __CR_IN__ const fpxx_t*   x,
  __CR_IN__ const fpxx_t*   y,
  __CR_IN__ uint_t          num
    )
{
    uint_t  idx;
    fpxx_t  tmp, dlt2 = 0, sum2 = 0;

    for (idx = 0; idx < num; idx++) {
        tmp  = x[idx];
        sum2 += tmp * tmp;
        tmp -= y[idx];
        dlt2 += tmp * tmp;
    }
    if (sum2 <= CR_ABITX)
        return ((dlt2 <= CR_ABITX) ? 1 : 0);
    return (1 - XSQRT(dlt2 / sum2));
}

/*
=======================================
    余弦相似度
=======================================
*/
CR_API fpxx_t
sim_cosine (
  __CR_IN__ const fpxx_t*   x,
  __CR_IN__ const fpxx_t*   y,
  __CR_IN__ uint_t          num
    )
{
    uint_t  idx;
    fpxx_t  mul = 0, dv1 = 0, dv2 = 0;

    for (idx = 0; idx < num; idx++) {
        mul += x[idx] * y[idx];
        dv1 += x[idx] * x[idx];
        dv2 += y[idx] * y[idx];
    }
    dv1 = XSQRT(dv1 * dv2);
    if (dv1 <= CR_ABITX)
        return ((XABS(mul) <= CR_ABITX) ? 1 : 0);
    return (mul / dv1);
}

/*
=======================================
    杰卡德相似度
=======================================
*/
CR_API fpxx_t
sim_jaccard (
  __CR_IN__ const fpxx_t*   x,
  __CR_IN__ uint_t          nx,
  __CR_IN__ const fpxx_t*   y,
  __CR_IN__ uint_t          ny
    )
{
    uint_t  ii, jj;
    sint_t  uu = 0, nn = nx + ny;

    /* 传入的必须为集合
       即数组内的元素不能重复 */
    for (ii = 0; ii < nx; ii++)
    for (jj = 0; jj < ny; jj++) {
        if (XABS(x[ii] - y[jj]) <= CR_ABITX) {
            uu++;
            nn--;
            break;
        }
    }
    return ((fpxx_t)uu / nn);
}

/*
=======================================
    曼哈顿距离
=======================================
*/
CR_API fpxx_t
distance_manhattan (
  __CR_IN__ const fpxx_t*   x,
  __CR_IN__ const fpxx_t*   y,
  __CR_IN__ uint_t          count
    )
{
    uint_t  idx;
    fpxx_t  dlt, sum = 0;

    for (idx = 0; idx < count; idx++) {
        dlt = x[idx] - y[idx];
        sum += XABS(dlt);
    }
    return (sum);
}

/*
=======================================
    欧几里得距离
=======================================
*/
CR_API fpxx_t
distance_euclidean (
  __CR_IN__ const fpxx_t*   x,
  __CR_IN__ const fpxx_t*   y,
  __CR_IN__ uint_t          count
    )
{
    uint_t  idx;
    fpxx_t  dlt, sum = 0;

    for (idx = 0; idx < count; idx++) {
        dlt = x[idx] - y[idx];
        sum += dlt * dlt;
    }
    return (XSQRT(sum));
}

/*
=======================================
    切比雪夫距离
=======================================
*/
CR_API fpxx_t
distance_chebyshev (
  __CR_IN__ const fpxx_t*   x,
  __CR_IN__ const fpxx_t*   y,
  __CR_IN__ uint_t          count
    )
{
    uint_t  idx;
    fpxx_t  dlt, big = 0;

    for (idx = 0; idx < count; idx++) {
        dlt = XABS(x[idx] - y[idx]);
        if (big < dlt) big = dlt;
    }
    return (big);
}

/*
=======================================
    闵可夫斯基距离
=======================================
*/
CR_API fpxx_t
distance_minkowski (
  __CR_IN__ const fpxx_t*   x,
  __CR_IN__ const fpxx_t*   y,
  __CR_IN__ uint_t          count,
  __CR_IN__ fpxx_t          p
    )
{
    uint_t  idx;
    fpxx_t  dlt, sum = 0;

    for (idx = 0; idx < count; idx++) {
        dlt = XABS(x[idx] - y[idx]);
        sum += XPOW(dlt, p);
    }
    return (XPOW(sum, 1 / p));
}

/*
=======================================
    余弦距离
=======================================
*/
CR_API fpxx_t
distance_cosine (
  __CR_IN__ const fpxx_t*   x,
  __CR_IN__ const fpxx_t*   y,
  __CR_IN__ uint_t          count
    )
{
    return (1 - sim_cosine(x, y, count));
}

/*
=======================================
    汉明距离 (字符)
=======================================
*/
CR_API uint_t
distance_hamming_cha (
  __CR_IN__ const ansi_t*   x,
  __CR_IN__ const ansi_t*   y,
  __CR_IN__ uint_t          count
    )
{
    uint_t  idx, chg = 0;

    for (idx = 0; idx < count; idx++) {
        if (x[idx] != y[idx])
            chg++;
    }
    return (chg);
}

/*
=======================================
    汉明距离 (二进制位)
=======================================
*/
CR_API uint_t
distance_hamming_bit (
  __CR_IN__ const byte_t*   x,
  __CR_IN__ const byte_t*   y,
  __CR_IN__ uint_t          count
    )
{
    uint_t  idx, aa = 0, bb = 0;

    for (idx = 0; idx < count; idx++) {
        aa += count_bits08(x[idx]);
        bb += count_bits08(y[idx]);
    }
    return (aa >= bb ? aa - bb : bb - aa);
}

/*
=======================================
    杰卡德距离
=======================================
*/
CR_API fpxx_t
distance_jaccard (
  __CR_IN__ const fpxx_t*   x,
  __CR_IN__ uint_t          nx,
  __CR_IN__ const fpxx_t*   y,
  __CR_IN__ uint_t          ny
    )
{
    return (1 - sim_jaccard(x, nx, y, ny));
}

/*
=======================================
    马哈拉诺比斯距离 (单点)
=======================================
*/
CR_API fpxx_t
distance_mahalanobis_arr (
  __CR_IN__ const fpxx_t*   x,
  __CR_IN__ uint_t          nx,
  __CR_IN__ fpxx_t          ey,
  __CR_IN__ fpxx_t          sdy
    )
{
    uint_t  idx;
    fpxx_t  sum;

    if (sdy <= CR_ABITX)
        return (CR_PHY_INV);
    sum = 0;
    for (idx = 0; idx < nx; idx++)
        sum += XABS(x[idx] - ey);
    return (sum / sdy);
}

/*
=======================================
    马哈拉诺比斯距离 (单点)
=======================================
*/
CR_API fpxx_t
distance_mahalanobis_lst (
  __CR_IN__ const fpxx_t*   x,
  __CR_IN__ uint_t          nx,
  __CR_IN__ const fpxx_t*   y,
  __CR_IN__ uint_t          ny
    )
{
    fpxx_t  ey, sdy;

    ey = statistics(y, ny, NULL, &sdy);
    return (distance_mahalanobis_arr(x, nx, ey, sdy));
}

/*
=======================================
    马哈拉诺比斯距离的平方
=======================================
*/
CR_API fpxx_t
distance_mahalanobis_xy2 (
  __CR_IN__ const fpxx_t*   x,
  __CR_IN__ const fpxx_t*   y,
  __CR_IN__ uint_t          count
    )
{
    uint_t  idx;
    fpxx_t  exy, dlt, sum;

    exy = covariance(x, y, count, NULL);
    if (XABS(exy) <= CR_ABITX)
        return (CR_PHY_INV);
    sum = 0;
    for (idx = 0; idx < count; idx++) {
        dlt = x[idx] - y[idx];
        sum += dlt * dlt;
    }
    return (sum / exy);
}

/*
=======================================
    DTW 距离
=======================================
*/
CR_API fpxx_t
distance_dtw (
  __CR_IN__ const fpxx_t*   x,
  __CR_IN__ uint_t          nx,
  __CR_IN__ const fpxx_t*   y,
  __CR_IN__ uint_t          ny,
  __CR_OT__ uint_t**        path,
  __CR_OT__ uint_t*         count
    )
{
    fpxx_t  *d0, *d1, tt[3];
    uint_t  *road, ii, jj, kk, mm;
    uint_t  *rett, nn, nbpl = nx + 1;
    uint_t  size = nbpl * (ny + 1);

    /* 初始化矩阵 */
    d0 = mem_talloc(size, fpxx_t);
    if (d0 == NULL)
        return (CR_PHY_INV);
    d0[0] = 0;
    for (ii = 0; ii < nx; ii++)
        d0[(ii + 1)] = CR_LARGX;
    for (ii = 0; ii < ny; ii++)
        d0[(ii + 1) * nbpl] = CR_LARGX;
    d1 = &d0[nbpl + 1];

    /* 计算距离图 */
    for (ii = 0; ii < ny; ii++)
    for (jj = 0; jj < nx; jj++)
        d1[ii * nbpl + jj] = XABS(x[jj] - y[ii]);

    /* 计算最短距离 */
    for (ii = 0; ii < ny; ii++)
    for (jj = 0; jj < nx; jj++) {
        kk = ii * nbpl;
        tt[0] = d0[kk + jj];
        tt[1] = d0[kk + jj + 1];
        tt[2] = d0[kk + nbpl + jj];
        if (tt[0] > tt[1])
            tt[0] = tt[1];
        if (tt[0] > tt[2])
            tt[0] = tt[2];
        d1[ii * nbpl + jj] += tt[0];
    }

    /* 生成回溯路径 */
    if (path != NULL && count != NULL) {
        road = mem_talloc(nx + ny, uint_t);
        if (road == NULL) {
            mem_free(d0);
            return (CR_PHY_INV);
        }
        ii = ny - 1;
        jj = nx - 1;
        road[0] = jj;
        road[1] = ii;
        nn = 2;
        while (ii > 0 || jj > 0) {
            kk = ii * nbpl;
            tt[0] = d0[kk + jj];
            tt[1] = d0[kk + jj + 1];
            tt[2] = d0[kk + nbpl + jj];
            mm = 0;
            if (tt[0] > tt[1]) {
                tt[0] = tt[1];
                mm = 1;
            }
            if (tt[0] > tt[2]) {
                tt[0] = tt[2];
                mm = 2;
            }

            if (mm == 0) {
                ii--;
                jj--;
            }
            else
            if (mm == 1) {
                ii--;
            }
            else {
                jj--;
            }
            road[nn++] = jj;
            road[nn++] = ii;
        }

        /* 调整路径顺序 */
        rett = mem_talloc(nn, uint_t);
        if (rett == NULL) {
            mem_free(road);
            mem_free(d0);
            return (CR_PHY_INV);
        }
        *count = kk = nn / 2;
        for (ii = 0; ii < kk; ii++) {
            jj = kk - 1 - ii;
            rett[ii * 2 + 0] = road[jj * 2 + 0];
            rett[ii * 2 + 1] = road[jj * 2 + 1];
        }
        *path = rett;
        mem_free(road);
    }

    /* 返回结果 */
    tt[0] = d0[size - 1];
    mem_free(d0);
    return (tt[0]);
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
