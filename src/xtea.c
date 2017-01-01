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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack XTEA 加密解密函数库 <<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "crypto.h"
#include "morder.h"

/*
=======================================
    XTEA 加密
=======================================
*/
CR_API void_t
crypto_xtea_enc (
  __CR_IO__ int32u          data[2],
  __CR_IN__ const int32u    key[4],
  __CR_IN__ uint_t          round
    )
{
    int32u  yy = DWORD_LE(data[0]);
    int32u  zz = DWORD_LE(data[1]);
    int32u  kk[4], ss = 0UL, delta = 0x9E3779B9UL;

    kk[0] = DWORD_LE(key[0]);
    kk[1] = DWORD_LE(key[1]);
    kk[2] = DWORD_LE(key[2]);
    kk[3] = DWORD_LE(key[3]);

    for (; round != 0; round--)
    {
        yy += (((zz << 4) ^ (zz >> 5)) + zz) ^ (ss + kk[ss & 3]);
        ss += delta;
        zz += (((yy << 4) ^ (yy >> 5)) + yy) ^ (ss + kk[(ss >> 11) & 3]);
    }
    data[0] = DWORD_LE(yy);
    data[1] = DWORD_LE(zz);
}

/*
=======================================
    XTEA 解密
=======================================
*/
CR_API void_t
crypto_xtea_dec (
  __CR_IO__ int32u          data[2],
  __CR_IN__ const int32u    key[4],
  __CR_IN__ uint_t          round
    )
{
    int32u  yy = DWORD_LE(data[0]);
    int32u  zz = DWORD_LE(data[1]);
    int32u  kk[4], ss, delta = 0x9E3779B9UL;

    ss = delta * round;
    kk[0] = DWORD_LE(key[0]);
    kk[1] = DWORD_LE(key[1]);
    kk[2] = DWORD_LE(key[2]);
    kk[3] = DWORD_LE(key[3]);

    for (; round != 0; round--)
    {
        zz -= (((yy << 4) ^ (yy >> 5)) + yy) ^ (ss + kk[(ss >> 11) & 3]);
        ss -= delta;
        yy -= (((zz << 4) ^ (zz >> 5)) + zz) ^ (ss + kk[ss & 3]);
    }
    data[0] = DWORD_LE(yy);
    data[1] = DWORD_LE(zz);
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
