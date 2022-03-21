/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2022-01-17  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>> CrHack 通用滤波器函数库 <<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "phylib.h"

/*
=======================================
    通用滤波器参数归一化
=======================================
*/
CR_API void_t
xfilter_normal (
  __CR_IO__ fpxx_t* a,
  __CR_IO__ fpxx_t* b,
  __CR_IN__ uint_t  nab
    )
{
    uint_t  ii;
    fpxx_t  a0 = 1 / a[0];

    for (ii = 0; ii < nab; ii++) {
        a[ii] *= a0;
        b[ii] *= a0;
    }
    a[ii] = 0;
}

/*
=======================================
    通用滤波器
=======================================
*/
CR_API void_t
xfilter (
  __CR_OT__ fpxx_t*         y,
  __CR_IN__ const fpxx_t*   x,
  __CR_IN__ uint_t          nxy,
  __CR_IO__ const fpxx_t*   a,
  __CR_IO__ const fpxx_t*   b,
  __CR_IN__ uint_t          nab,
  __CR_IN__ const fpxx_t*   zi      /* 长度为 nab - 1 */
    )
{
    uint_t  ii, jj;

    mem_zero(y, nxy * sizeof(fpxx_t));

    for (ii = 0; ii < nxy; ii++) {
        for (jj = 0; jj <= ii && jj < nab; jj++)
            y[ii] += (b[jj] * x[ii - jj] - a[jj] * y[ii - jj]);
        if (zi != NULL && ii < nab - 1)
            y[ii] += zi[ii];
    }
}

/*
=======================================
    零相位滤波器
=======================================
*/
CR_API bool_t
xfiltfilt (
  __CR_OT__ fpxx_t*         y,
  __CR_IN__ const fpxx_t*   x,
  __CR_IN__ uint_t          nxy,
  __CR_IN__ const fpxx_t*   a,
  __CR_IN__ const fpxx_t*   b,
  __CR_IN__ uint_t          nab
    )
{
    uint_t  idx, tlen, nfact;
    fpxx_t  *sp, *zi, *pp, *tvec, tmp1;
    fpxx_t  *tt, *tx1, *tx2, *end, tmp2;
    /* ------------------------------ */
    sMATRIX2    mat1, mat2, mat3, *mm;

    if (nab < 2)
        return (FALSE);
    nfact = nab - 1;
    if (nxy <= 3 * nfact)
        return (FALSE);

    tlen = 6 * nfact + nxy;
    tx1 = mem_talloc(tlen, fpxx_t);
    if (tx1 == NULL)
        return (FALSE);
    tx2 = mem_talloc(tlen, fpxx_t);
    if (tx2 == NULL)
        goto _failure1;
    sp = mem_talloc(nfact * nfact, fpxx_t);
    if (sp == NULL)
        goto _failure2;
    tvec = mem_talloc(nfact, fpxx_t);
    if (tvec == NULL)
        goto _failure3;
    zi = mem_talloc(nfact, fpxx_t);
    if (zi == NULL)
        goto _failure4;

    tmp1 = x[0];
    for (pp = (fpxx_t*)x + 3 * nfact, tt = tx1; pp > x; pp--, tt++)
        *tt = 2 * tmp1 - *pp;
    for (end = (fpxx_t*)x + nxy; pp < end; pp++, tt++)
        *tt = *pp;
    tmp1 = x[nxy - 1];
    for (end = tx1 + tlen, pp -= 2; tt < end; pp--, tt++)
        *tt = 2 * tmp1 - *pp;

    end = sp + nfact * nfact;
    pp = sp;
    while (pp < end)
        *pp++ = 0;
    sp[0] = 1 + a[1];
    for (idx = 1; idx < nfact; idx++) {
        sp[idx * nfact] = a[idx + 1];
        sp[idx * nfact + idx] = 1;
        sp[(idx - 1) * nfact + idx] = -1;
    }

    for (idx = 0; idx < nfact; idx++)
        tvec[idx] = b[idx + 1] - a[idx + 1] * b[0];

    mat1.cols = nfact;
    mat1.rows = nfact;
    mat1.vals = sp;
    mat2.cols = nfact;
    mat2.rows = nfact;
    mat2.vals = NULL;
    mm = matrix2_inv(&mat2, &mat1);
    if (mat2.vals == NULL)
        goto _failure5;
    if (mm == NULL) {
        mem_free(zi);
        zi = NULL;
    }
    else {
        mat1.cols = 1;
        mat1.rows = nfact;
        mat1.vals = tvec;
        mat3.cols = 1;
        mat3.rows = nfact;
        mat3.vals = zi;
        matrix2_vmul(&mat3, &mat2, &mat1);
    }
    matrix2_del(&mat2);

    mem_free(sp);
    mem_free(tvec);

    tmp2 = tx1[0];
    if (zi != NULL) {
        for (pp = zi, end = zi + nfact; pp < end; pp++)
            pp[0] *= tmp2;
    }
    xfilter(tx2, tx1, tlen, a, b, nab, zi);

    for (pp = tx2, end = tx2 + tlen - 1; pp < end; pp++, end--) {
        tmp1 = *pp;
        *pp = *end;
        *end = tmp1;
    }

    tmp2 = tx2[0] / tmp2;
    if (zi != NULL) {
        for (pp = zi, end = zi + nfact; pp < end; pp++)
            pp[0] *= tmp2;
    }
    xfilter(tx1, tx2, tlen, a, b, nab, zi);

    end = y + nxy;
    pp = tx1 + 3 * nfact + nxy - 1;
    while (y < end)
        *y++ = *pp--;

    if (zi != NULL)
        mem_free(zi);
    mem_free(tx2);
    mem_free(tx1);
    return (TRUE);

_failure5:
    mem_free(zi);
_failure4:
    mem_free(tvec);
_failure3:
    mem_free(sp);
_failure2:
    mem_free(tx2);
_failure1:
    mem_free(tx1);
    return (FALSE);
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
