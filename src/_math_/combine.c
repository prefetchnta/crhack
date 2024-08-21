/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2019-02-13  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 组合数学函数库 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "phylib.h"

#ifndef _CR_NO_STDC_
    #include <math.h>
#endif

/*
=======================================
    阶乘
=======================================
*/
CR_API uint_t
factorial (
  __CR_IN__ uint_t  n
    )
{
    uint_t  idx, val = 1;

    for (idx = 2; idx <= n; idx++)
        val *= idx;
    return (val);
}

/*
=======================================
    排列
=======================================
*/
CR_API uint_t
permutation (
  __CR_IN__ uint_t  m,
  __CR_IN__ uint_t  n
    )
{
    if (m > n)
        return (0);
    return (factorial(n) / factorial(n - m));
}

/*
=======================================
    组合
=======================================
*/
CR_API uint_t
combination (
  __CR_IN__ uint_t  m,
  __CR_IN__ uint_t  n
    )
{
    if (m > n)
        return (0);
    return (factorial(n) / (factorial(m) * factorial(n - m)));
}

/*
=======================================
    集合I
=======================================
*/
CR_API uint_t
make_setI (
  __CR_IO__ sint_t* array,
  __CR_IN__ uint_t  count
    )
{
    uint_t  ii, jj, kk;

    for (ii = 0; ii < count - 1; ii++) {
        for (jj = ii + 1; jj < count; jj++) {
            if (array[ii] == array[jj]) {
                for (kk = jj; kk < count - 1; kk++)
                    array[kk] = array[kk + 1];
                count--;
                jj--;
            }
        }
    }
    return (count);
}

/*
=======================================
    集合F
=======================================
*/
CR_API uint_t
make_setF (
  __CR_IO__ fpxx_t* array,
  __CR_IN__ uint_t  count
    )
{
    uint_t  ii, jj, kk;

    for (ii = 0; ii < count - 1; ii++) {
        for (jj = ii + 1; jj < count; jj++) {
            if (XABS(array[ii] - array[jj]) <= CR_ABITX) {
                for (kk = jj; kk < count - 1; kk++)
                    array[kk] = array[kk + 1];
                count--;
                jj--;
            }
        }
    }
    return (count);
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
