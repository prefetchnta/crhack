/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2022-03-09  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 矩阵计算函数库 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "phylib.h"

#ifndef _CR_NO_STDC_
    #include <math.h>
#endif

/*
=======================================
    矩阵分配
=======================================
*/
CR_API sMATRIX2*
matrix2_new (
  __CR_IO__ sMATRIX2*   m
    )
{
    uint_t  size = m->cols * m->rows;

    m->vals = mem_talloc(size, fpxx_t);
    if (m->vals == NULL)
        return (NULL);
    mem_zero(m->vals, size * sizeof(fpxx_t));
    return (m);
}

/*
=======================================
    矩阵释放
=======================================
*/
CR_API void_t
matrix2_del (
  __CR_IN__ sMATRIX2*   m
    )
{
    SAFE_FREE(m->vals);
}

/*
=======================================
    矩阵设值 (填充)
=======================================
*/
CR_API sMATRIX2*
matrix2_set (
  __CR_IO__ const sMATRIX2* m,
  __CR_IN__ fpxx_t          v
    )
{
    uint_t  idx, size = m->cols * m->rows;

    for (idx = 0; idx < size; idx++)
        m->vals[idx] = v;
    return ((sMATRIX2*)m);
}

/*
=======================================
    矩阵设值 (对角)
=======================================
*/
CR_API sMATRIX2*
matrix2_dlg (
  __CR_IO__ const sMATRIX2* m,
  __CR_IN__ fpxx_t          v
    )
{
    uint_t  idx, size;

    if (m->cols != m->rows)
        return (NULL);
    size = m->cols * m->rows;
    mem_zero(m->vals, size * sizeof(fpxx_t));
    for (idx = 0; idx < m->cols; idx++)
        m->vals[idx * m->cols + idx] = v;
    return ((sMATRIX2*)m);
}

/*
=======================================
    矩阵复制
=======================================
*/
CR_API sMATRIX2*
matrix2_cpy (
  __CR_OT__ sMATRIX2*       dm,
  __CR_IN__ const sMATRIX2* sm
    )
{
    uint_t  size;

    if (dm->vals == NULL) {
        dm->cols = sm->cols;
        dm->rows = sm->rows;
        dm = matrix2_new(dm);
        if (dm == NULL)
            return (NULL);
    }
    else {
        if (dm->cols != sm->cols ||
            dm->rows != sm->rows)
            return (NULL);
    }
    size = sm->cols * sm->rows;
    mem_cpy(dm->vals, sm->vals, size * sizeof(fpxx_t));
    return (dm);
}

/*
=======================================
    矩阵转置
=======================================
*/
CR_API sMATRIX2*
matrix2_trn (
  __CR_OT__ sMATRIX2*       dm,
  __CR_IN__ const sMATRIX2* sm
    )
{
    uint_t  xx, yy;

    if (dm->vals == NULL) {
        dm->cols = sm->rows;
        dm->rows = sm->cols;
        dm = matrix2_new(dm);
        if (dm == NULL)
            return (NULL);
    }
    else {
        if (dm->cols != sm->rows ||
            dm->rows != sm->cols)
            return (NULL);
    }
    for (yy = 0; yy < sm->rows; yy++)
    for (xx = 0; xx < sm->cols; xx++)
        dm->vals[xx * dm->cols + yy] = sm->vals[yy * sm->cols + xx];
    return (dm);
}

/*
=======================================
    矩阵的迹
=======================================
*/
CR_API sMATRIX2*
matrix2_tra (
  __CR_IN__ const sMATRIX2* m,
  __CR_OT__ fpxx_t*         tr
    )
{
    fpxx_t  sum;
    uint_t  idx;

    if (m->cols != m->rows)
        return (NULL);
    sum = 0;
    for (idx = 0; idx < m->cols; idx++)
        sum += m->vals[idx * m->cols + idx];
    *tr = sum;
    return ((sMATRIX2*)m);
}

/*
=======================================
    矩阵数加
=======================================
*/
CR_API sMATRIX2*
matrix2_sadd (
  __CR_IO__ const sMATRIX2* m,
  __CR_IN__ fpxx_t          v
    )
{
    uint_t  idx, size = m->cols * m->rows;

    for (idx = 0; idx < size; idx++)
        m->vals[idx] += v;
    return ((sMATRIX2*)m);
}

/*
=======================================
    矩阵数乘
=======================================
*/
CR_API sMATRIX2*
matrix2_smul (
  __CR_IO__ const sMATRIX2* m,
  __CR_IN__ fpxx_t          v
    )
{
    uint_t  idx, size = m->cols * m->rows;

    for (idx = 0; idx < size; idx++)
        m->vals[idx] *= v;
    return ((sMATRIX2*)m);
}

/*
=======================================
    矩阵加法
=======================================
*/
CR_API sMATRIX2*
matrix2_add2 (
  __CR_IO__ const sMATRIX2* dm,
  __CR_IN__ const sMATRIX2* sm
    )
{
    uint_t  idx, size;

    if (dm->cols != sm->cols ||
        dm->rows != sm->rows)
        return (NULL);
    size = sm->cols * sm->rows;
    for (idx = 0; idx < size; idx++)
        dm->vals[idx] += sm->vals[idx];
    return ((sMATRIX2*)dm);
}

/*
=======================================
    矩阵加法
=======================================
*/
CR_API sMATRIX2*
matrix2_add3 (
  __CR_OT__ sMATRIX2*       dm,
  __CR_IN__ const sMATRIX2* m1,
  __CR_IN__ const sMATRIX2* m2
    )
{
    dm = matrix2_cpy(dm, m1);
    if (dm == NULL)
        return (NULL);
    return (matrix2_add2(dm, m2));
}

/*
=======================================
    矩阵减法
=======================================
*/
CR_API sMATRIX2*
matrix2_sub2 (
  __CR_IO__ const sMATRIX2* dm,
  __CR_IN__ const sMATRIX2* sm
    )
{
    uint_t  idx, size;

    if (dm->cols != sm->cols ||
        dm->rows != sm->rows)
        return (NULL);
    size = sm->cols * sm->rows;
    for (idx = 0; idx < size; idx++)
        dm->vals[idx] -= sm->vals[idx];
    return ((sMATRIX2*)dm);
}

/*
=======================================
    矩阵减法
=======================================
*/
CR_API sMATRIX2*
matrix2_sub3 (
  __CR_OT__ sMATRIX2*       dm,
  __CR_IN__ const sMATRIX2* m1,
  __CR_IN__ const sMATRIX2* m2
    )
{
    dm = matrix2_cpy(dm, m1);
    if (dm == NULL)
        return (NULL);
    return (matrix2_sub2(dm, m2));
}

/*
=======================================
    矩阵基本积 (哈达玛积)
=======================================
*/
CR_API sMATRIX2*
matrix2_dot2 (
  __CR_IO__ const sMATRIX2* dm,
  __CR_IN__ const sMATRIX2* sm
    )
{
    uint_t  idx, size;

    if (dm->cols != sm->cols ||
        dm->rows != sm->rows)
        return (NULL);
    size = sm->cols * sm->rows;
    for (idx = 0; idx < size; idx++)
        dm->vals[idx] *= sm->vals[idx];
    return ((sMATRIX2*)dm);
}

/*
=======================================
    矩阵基本积 (哈达玛积)
=======================================
*/
CR_API sMATRIX2*
matrix2_dot3 (
  __CR_OT__ sMATRIX2*       dm,
  __CR_IN__ const sMATRIX2* m1,
  __CR_IN__ const sMATRIX2* m2
    )
{
    dm = matrix2_cpy(dm, m1);
    if (dm == NULL)
        return (NULL);
    return (matrix2_dot2(dm, m2));
}

/*
=======================================
    矩阵乘法
=======================================
*/
CR_API sMATRIX2*
matrix2_vmul (
  __CR_OT__ sMATRIX2*       dm,
  __CR_IN__ const sMATRIX2* m1,
  __CR_IN__ const sMATRIX2* m2
    )
{
    fpxx_t  tt;
    uint_t  ii, xx, yy;

    if (m1->cols != m2->rows)
        return (NULL);
    if (dm->vals == NULL) {
        dm->cols = m2->cols;
        dm->rows = m1->rows;
        dm = matrix2_new(dm);
        if (dm == NULL)
            return (NULL);
    }
    else {
        if (dm->cols != m2->cols ||
            dm->rows != m1->rows)
            return (NULL);
    }

    for (yy = 0; yy < dm->rows; yy++)
    for (xx = 0; xx < dm->cols; xx++) {
        for (tt = 0, ii = 0; ii < m1->cols; ii++)
            tt += m1->vals[yy * m1->cols + ii] * m2->vals[ii * m2->cols + xx];
        dm->vals[yy * dm->cols + xx] = tt;
    }
    return (dm);
}

/*
---------------------------------------
    矩阵拉普拉斯降阶
---------------------------------------
*/
static sMATRIX2*
laplace_expansion (
  __CR_OT__ fpxx_t*         v,
  __CR_IN__ const sMATRIX2* m,
  __CR_IN__ uint_t          y,
  __CR_IN__ uint_t          x
    )
{
    sMATRIX2    bb;
    sMATRIX2*   rr;
    /* --------- */
    uint_t  ii, jj;
    uint_t  oi, oj;
    uint_t  nn = m->cols;

    bb.rows = bb.cols = nn - 1;
    if (matrix2_new(&bb) == NULL)
        return (NULL);

    for (ii = 0; ii < nn; ii++)
    for (jj = 0; jj < nn; jj++) {
        oi = ii;
        oj = jj;
        if (ii != y && jj != x) {
            if (ii > y) ii--;
            if (jj > x) jj--;
            bb.vals[ii * bb.cols + jj] = m->vals[oi * nn + oj];
            ii = oi;
            jj = oj;
        }
    }
    rr = matrix2_det(v, &bb);
    matrix2_del(&bb);
    return (rr);
}

/*
=======================================
    矩阵行列式
=======================================
*/
CR_API sMATRIX2*
matrix2_det (
  __CR_OT__ fpxx_t*         v,
  __CR_IN__ const sMATRIX2* m
    )
{
    uint_t  ii, nn;
    fpxx_t  sum, tmp, sig;

    if (m->cols != m->rows)
        return (NULL);

    if ((nn = m->cols) == 1) {
        *v = m->vals[0];
        return ((sMATRIX2*)m);
    }

    sum = 0;
    sig = 1;
    for (ii = 0; ii < nn; ii++) {
        if (laplace_expansion(&tmp, m, ii, 0) == NULL)
            return (NULL);
        sum += sig * m->vals[ii * nn] * tmp;
        sig *= -1;
    }
    *v = sum;
    return ((sMATRIX2*)m);
}

/*
=======================================
    矩阵比较
=======================================
*/
CR_API bool_t
matrix2_equ (
  __CR_IN__ const sMATRIX2* m1,
  __CR_IN__ const sMATRIX2* m2,
  __CR_IN__ fpxx_t          dlt
    )
{
    uint_t  idx, size;

    if (m1->cols != m2->cols ||
        m1->rows != m2->rows)
        return (FALSE);
    size = m1->cols * m1->rows;
    for (idx = 0; idx < size; idx++) {
        if (XABS(m1->vals[idx] - m2->vals[idx]) > dlt)
            return (FALSE);
    }
    return (TRUE);
}

/*
---------------------------------------
    矩阵交换行
---------------------------------------
*/
static void_t
mat2_row_swap (
  __CR_IN__ const sMATRIX2* m,
  __CR_IN__ uint_t          r1,
  __CR_IN__ uint_t          r2
    )
{
    fpxx_t  tmp;
    uint_t  y1 = r1 * m->cols;
    uint_t  y2 = r2 * m->cols;
    uint_t  idx, nn = m->cols;

    for (idx = 0; idx < nn; idx++) {
        CR_SWAP(m->vals[y1 + idx], m->vals[y2 + idx], tmp);
    }
}

/*
---------------------------------------
    矩阵行数乘
---------------------------------------
*/
static void_t
mat2_row_scale (
  __CR_IN__ const sMATRIX2* m,
  __CR_IN__ uint_t          r,
  __CR_IN__ fpxx_t          v
    )
{
    uint_t  idx;
    uint_t  nn = m->cols;
    uint_t  yy = m->cols * r;

    for (idx = 0; idx < nn; idx++)
        m->vals[yy + idx] *= v;
}

/*
---------------------------------------
    矩阵行乘加
---------------------------------------
*/
static void_t
mat2_row_shear (
  __CR_IN__ const sMATRIX2* m,
  __CR_IN__ uint_t          r1,
  __CR_IN__ uint_t          r2,
  __CR_IN__ fpxx_t          v
    )
{
    uint_t  y1 = r1 * m->cols;
    uint_t  y2 = r2 * m->cols;
    uint_t  idx, nn = m->cols;

    for (idx = 0; idx < nn; idx++)
        m->vals[y1 + idx] += m->vals[y2 + idx] * v;
}

/*
=======================================
    矩阵求逆
=======================================
*/
CR_API sMATRIX2*
matrix2_inv (
  __CR_OT__ sMATRIX2*       dm,
  __CR_IN__ const sMATRIX2* sm
    )
{
    sMATRIX2    bb;
    /* --------- */
    fpxx_t  sc, sh;
    uint_t  ii, jj, rr;

    if (sm->cols != sm->rows)
        return (NULL);

    bb.vals = NULL;
    if (matrix2_cpy(&bb, sm) == NULL)
        return (NULL);

    if (dm->vals == NULL) {
        dm->cols = sm->rows;
        dm->rows = sm->rows;
        dm = matrix2_new(dm);
        if (dm == NULL) {
            matrix2_del(&bb);
            return (NULL);
        }
    }
    else {
        if (dm->cols != sm->rows ||
            dm->rows != sm->rows) {
            matrix2_del(&bb);
            return (NULL);
        }
    }

    matrix2_dlg(dm, 1);
    for (ii = 0; ii < bb.cols; ii++) {
        if (XABS(bb.vals[ii * bb.cols + ii]) <= CR_ABITX) {
            for (rr = ii + 1; rr < bb.cols; rr++) {
                if (XABS(bb.vals[rr * bb.cols + ii]) > CR_ABITX)
                    break;
            }
            if (rr == bb.cols) {
                matrix2_del(&bb);
                return (NULL);
            }
            mat2_row_swap(&bb, ii, rr);
            mat2_row_swap( dm, ii, rr);
        }

        sc = 1 / bb.vals[ii * bb.cols + ii];
        mat2_row_scale(&bb, ii, sc);
        mat2_row_scale( dm, ii, sc);
        for (jj = 0; jj < bb.cols; jj++) {
            if (ii == jj)
                continue;
            sh = -bb.vals[jj * bb.cols + ii];
            mat2_row_shear(&bb, jj, ii, sh);
            mat2_row_shear( dm, jj, ii, sh);
        }
    }
    matrix2_del(&bb);
    return (dm);
}

/*
=======================================
    矩阵求 L1 范数
=======================================
*/
CR_API fpxx_t
matrix2_norm_l1 (
  __CR_IN__ const sMATRIX2* m
    )
{
    uint_t  xx, yy;
    fpxx_t  sum, big = 0;

    for (xx = 0; xx < m->cols; xx++) {
        sum = 0;
        for (yy = 0; yy < m->rows; yy++)
            sum += XABS(m->vals[yy * m->cols + xx]);
        if (big < sum) big = sum;
    }
    return (big);
}

/*
=======================================
    矩阵求 Frobenius 范数
=======================================
*/
CR_API fpxx_t
matrix2_norm_lf (
  __CR_IN__ const sMATRIX2* m
    )
{
    fpxx_t  sum = 0;
    uint_t  idx, size = m->cols * m->rows;

    for (idx = 0; idx < size; idx++)
        sum += m->vals[idx] * m->vals[idx];
    return (XSQRT(sum));
}

/*
=======================================
    矩阵求最大范数
=======================================
*/
CR_API fpxx_t
matrix2_norm_lm (
  __CR_IN__ const sMATRIX2* m
    )
{
    uint_t  xx, yy;
    fpxx_t  sum, big = 0;

    for (yy = 0; yy < m->rows; yy++) {
        sum = 0;
        for (xx = 0; xx < m->cols; xx++)
            sum += XABS(m->vals[yy * m->cols + xx]);
        if (big < sum) big = sum;
    }
    return (big);
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
