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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack RSA 加密解密函数库 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "crypto.h"
#include "memlib.h"

/* 默认和最大支持的密钥位数 */
#define RSA_DEF_BITS    1024
#define RSA_MAX_BITS    2048

/*
=======================================
    RSA 生成密钥
=======================================
*/
CR_API void_t
crypto_rsa_key (
  __CR_OT__ sRSA*           pub,
  __CR_OT__ sRSA*           prv,
  __CR_IN__ const ansi_t*   e10,    /* 十进制 */
  __CR_IN__ uint_t          bits,
  __CR_IN__ uint_t          split
    )
{
    sBIGINT     P, Q;

    /* 参数过滤 */
    if (bits == 0) {
        bits = RSA_DEF_BITS;
    }
    else
    if (bits > RSA_MAX_BITS) {
        bits = RSA_MAX_BITS;
    }
    else
    if (bits % 64 != 0) {
        bits = (bits + 63) / 64;
        bits *= 64;
    }

    /* 生成公钥指数 */
    bigint_from(&pub->E, e10, 0, 10);

    /* 生成两个素数 */
    do {
        bigint_prime(&P, bits / 2);
    } while (!bigint_rab(&P));

    do {
        bigint_prime(&Q, bits / 2);
    } while (!bigint_rab(&Q));

    /* 计算私钥指数 */
    bigint_private(&prv->E, &pub->N, &P, &Q, &pub->E);

    /* 设置附加参数 */
    pub->block = bits / 8;
    pub->split = split;
    if (pub->split == 0 || pub->split >= pub->block)
    {
        /* 加密分组必须比取模的数小 */
        pub->split = pub->block - 1;
    }
    prv->block = pub->block;
    prv->split = pub->split;
    bigint_cpy(&prv->N, &pub->N);
}

/*
=======================================
    RSA 加密
=======================================
*/
CR_API leng_t
crypto_rsa_enc (
  __CR_IN__ const sRSA*     pub,
  __CR_OT__ void_t*         dst,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          size
    )
{
    sBIGINT     X;
    /* -------- */
    leng_t  blk = size / pub->split;
    leng_t  rst = size % pub->split;
    leng_t  dstlen = blk * pub->block;

    if (dst == NULL)
        return (dstlen + rst);
    for (; blk != 0; blk--) {
        bigint_from(&X, src, pub->split, 0);
        src = (byte_t*)src + pub->split;
        bigint_rsa(&X, &X, &pub->E, &pub->N);
        bigint_to(&X, dst, pub->block, 0);
        dst = (byte_t*)dst + pub->block;
    }
    if (rst != 0) {
        mem_cpy(dst, src, rst);
        dstlen += rst;
    }
    return (dstlen);
}

/*
=======================================
    RSA 解密
=======================================
*/
CR_API leng_t
crypto_rsa_dec (
  __CR_IN__ const sRSA*     prv,
  __CR_OT__ void_t*         dst,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          size
    )
{
    sBIGINT     X;
    /* -------- */
    byte_t  tmp[RSA_MAX_BITS / 8];
    leng_t  blk = size / prv->block;
    leng_t  rst = size % prv->block;
    leng_t  dstlen = blk * prv->split;

    if (dst == NULL)
        return (dstlen + rst);
    for (; blk != 0; blk--) {
        bigint_from(&X, src, prv->block, 0);
        src = (byte_t*)src + prv->block;
        bigint_rsa(&X, &X, &prv->E, &prv->N);
        bigint_to(&X, tmp, prv->block, 0);
        mem_cpy(dst, tmp, prv->split);
        dst = (byte_t*)dst + prv->split;
    }
    if (rst != 0) {
        mem_cpy(dst, src, rst);
        dstlen += rst;
    }
    return (dstlen);
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
