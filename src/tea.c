/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-01-06  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack TEA 加密解密函数库 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "crypto.h"
#include "morder.h"

/*
=======================================
    TEA 加密
=======================================
*/
CR_API void_t
crypto_tea_enc (
  __CR_IO__ int32u          data[2],
  __CR_IN__ const int32u    key[4],
  __CR_IN__ uint_t          round
    )
{
    int32u  aa = DWORD_LE( key[0]);
    int32u  bb = DWORD_LE( key[1]);
    int32u  cc = DWORD_LE( key[2]);
    int32u  dd = DWORD_LE( key[3]);
    int32u  yy = DWORD_LE(data[0]);
    int32u  zz = DWORD_LE(data[1]);
    int32u  ss = 0UL, delta = 0x9E3779B9UL;

    for (; round != 0; round--)
    {
        ss += delta;
        yy += ((zz << 4) + aa) ^ (zz + ss) ^ ((zz >> 5) + bb);
        zz += ((yy << 4) + cc) ^ (yy + ss) ^ ((yy >> 5) + dd);
    }
    data[0] = DWORD_LE(yy);
    data[1] = DWORD_LE(zz);
}

/*
=======================================
    TEA 解密
=======================================
*/
CR_API void_t
crypto_tea_dec (
  __CR_IO__ int32u          data[2],
  __CR_IN__ const int32u    key[4],
  __CR_IN__ uint_t          round
    )
{
    int32u  aa = DWORD_LE( key[0]);
    int32u  bb = DWORD_LE( key[1]);
    int32u  cc = DWORD_LE( key[2]);
    int32u  dd = DWORD_LE( key[3]);
    int32u  yy = DWORD_LE(data[0]);
    int32u  zz = DWORD_LE(data[1]);
    int32u  ss, delta = 0x9E3779B9UL;

    ss = delta * round;

    for (; round != 0; round--)
    {
        zz -= ((yy << 4) + cc) ^ (yy + ss) ^ ((yy >> 5) + dd);
        yy -= ((zz << 4) + aa) ^ (zz + ss) ^ ((zz >> 5) + bb);
        ss -= delta;
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
