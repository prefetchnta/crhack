/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2014-04-14  */
/*     #######          ###    ###      [CORE]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>>>>>> CrHack XXTEA 加密解密函数库 <<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "crypto.h"
#include "morder.h"

#define XXTEA_MX    \
    ((((zz >> 5) ^ (yy << 2)) + ((yy >> 3) ^ (zz << 4))) ^ \
      ((ss ^ yy) + (kk[(pp & 3) ^ ee] ^ zz)))
/*
---------------------------------------
    XXTEA 加密解密
---------------------------------------
*/
static void_t
xxtea_crypt (
  __CR_IO__ int32u*         data,
  __CR_IN__ dist_t          num,
  __CR_IN__ const int32u    key[4]
    )
{
    leng_t  pp, nn, ee, round;
    int32u  yy, zz, ss, kk[4];
    int32u  delta = 0x9E3779B9UL;

    if (num > 1)
        nn = (leng_t)(num);
    else
    if (num < -1)
        nn = (leng_t)(-num);
    else
        return;
    round = 6 + 52 / nn;

#if defined(_CR_ORDER_BE_)
    for (pp = 0; pp < nn; pp++)
        data[pp] = xchg_int32u(data[pp]);
#endif
    kk[0] = DWORD_LE(key[0]);
    kk[1] = DWORD_LE(key[1]);
    kk[2] = DWORD_LE(key[2]);
    kk[3] = DWORD_LE(key[3]);

    if (num > 0) {
        ss = 0UL;
        zz = data[nn - 1];
        while (round-- != 0) {
            ss += delta;
            ee = (leng_t)((ss >> 2) & 3);
            for (pp = 0; pp < nn - 1; pp++) {
                yy = data[pp + 1];
                zz = data[pp] += XXTEA_MX;
            }
            yy = data[0];
            zz = data[nn - 1] += XXTEA_MX;
        }
    }
    else {
        ss = (int32u)(round * delta);
        yy = data[0];
        while (ss != 0UL) {
            ee = (leng_t)((ss >> 2) & 3);
            for (pp = nn - 1; pp > 0; pp--) {
                zz = data[pp - 1];
                yy = data[pp] -= XXTEA_MX;
            }
            zz = data[nn - 1];
            yy = data[0] -= XXTEA_MX;
            ss -= delta;
        }
    }

#if defined(_CR_ORDER_BE_)
    for (pp = 0; pp < nn; pp++)
        data[pp] = xchg_int32u(data[pp]);
#endif
}

/*
=======================================
    XXTEA 加密
=======================================
*/
CR_API void_t
crypto_xxtea_enc (
  __CR_IO__ int32u*         data,
  __CR_IN__ leng_t          num,
  __CR_IN__ const int32u    key[4]
    )
{
    xxtea_crypt(data, (dist_t)(num), key);
}

/*
=======================================
    XXTEA 解密
=======================================
*/
CR_API void_t
crypto_xxtea_dec (
  __CR_IO__ int32u*         data,
  __CR_IN__ leng_t          num,
  __CR_IN__ const int32u    key[4]
    )
{
    xxtea_crypt(data, (dist_t)(0 - num), key);
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
