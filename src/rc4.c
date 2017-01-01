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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack RC4 加密解密函数库 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "crypto.h"

/*
=======================================
    RC4 设置密钥
=======================================
*/
CR_API void_t
crypto_rc4_key (
  __CR_OT__ sRC4*           ctx,
  __CR_IN__ const void_t*   key,
  __CR_IN__ leng_t          keylen
    )
{
    byte_t  tmp;
    uint_t  idx;
    uint_t  add = 0;
    leng_t  ptr = 0;

    ctx->x = 1;
    ctx->y = 0;

    for (idx = 0; idx < 256; idx++)
        ctx->s[idx] = (uchar)idx;

    if (keylen == 0)
        return;
    for (idx = 0; idx < 256; idx++)
    {
        tmp = ctx->s[idx];
        add += ((uchar*)key)[ptr] + tmp;
        add &= 0xFF;
        ctx->s[idx] = ctx->s[add];
        ctx->s[add] = tmp;
        if (++ptr >= keylen)
            ptr = 0;
    }
}

/*
=======================================
    RC4 加密解密
=======================================
*/
CR_API void_t
crypto_rc4_ops (
  __CR_IO__ sRC4*   ctx,
  __CR_IO__ void_t* data,
  __CR_IN__ leng_t  size
    )
{
    uint_t  xx = ctx->x;
    uint_t  yy = ctx->y;
    byte_t  aa, bb, *tmp = (uchar*)data;

    for (; size != 0; size--)
    {
        aa = ctx->s[xx];
        yy = yy + aa;
        yy &= 0xFF;
        bb = ctx->s[yy];
        ctx->s[xx] = bb;
        ctx->s[yy] = aa;
        xx = xx + 1;
        xx &= 0xFF;
        *tmp++ ^= ctx->s[(aa + bb) & 0xFF];
    }
    ctx->x = (byte_t)xx;
    ctx->y = (byte_t)yy;
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
