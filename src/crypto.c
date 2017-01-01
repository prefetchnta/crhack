/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2015-08-07  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack 通用加密解密函数库 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "crypto.h"
#include "memlib.h"
#include "msclib.h"

/*****************************************************************************/
/*                                   EBC                                     */
/*****************************************************************************/

/*
=======================================
    EBC 模式加密
=======================================
*/
CR_API leng_t
crypto_all_ebc_enc (
  __CR_IN__ void_t*         ctx,
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ leng_t          dstblk,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen,
  __CR_IN__ leng_t          srcblk,
  __CR_IN__ uint_t          fill,
  __CR_IN__ enc_parm_t      func
    )
{
    byte_t  cha, tmp[512];
    leng_t  blk, rst, size;

    /* 安全检查 */
    if (srcblk > sizeof(tmp))
        return (0);

    /* 计算目标大小 */
    if (dstblk == 0)
        dstblk = srcblk;
    blk = srclen / srcblk;
    rst = srclen % srcblk;
    size = blk * dstblk;
    if (rst != 0)
        size += dstblk;
    if (dst == NULL)
        return (size);

    /* 安全检查 */
    if (size > dstlen)
        return (0);

    /* 填充类型检查 */
    if (fill > 0xFF) {
        switch (fill & (~0xFF))
        {
            default:
                return (0);

            case CR_PADDING_PKCS5:
                if (srcblk != 8)
                    return (0);
            case CR_PADDING_PKCS7:
            case CR_PADDING_ANSI_X923:
            case CR_PADDING_ISO10126:
                break;
        }
    }

    /* 分块加密 */
    for (; blk != 0; blk--) {
        func(dst, dstblk, src, srcblk, ctx);
        dst = (byte_t*)dst + dstblk;
        src = (byte_t*)src + srcblk;
    }

    /* 处理尾部 */
    if (rst != 0) {
        mem_cpy(tmp, src, rst);
        blk = srcblk - rst;
        if (fill <= 0xFF) {
            cha = (byte_t)fill;
            for (; blk != 0; blk--, rst++)
                tmp[rst] = cha;
        }
        else {
            switch (fill & (~0xFF))
            {
                default:
                    break;

                case CR_PADDING_PKCS5:
                case CR_PADDING_PKCS7:
                    cha = (byte_t)blk;
                    for (; blk != 0; blk--, rst++)
                        tmp[rst] = cha;
                    break;

                case CR_PADDING_ANSI_X923:
                    cha = (byte_t)blk;
                    for (; blk > 1; blk--, rst++)
                        tmp[rst] = 0x00;
                    tmp[rst] = cha;
                    break;

                case CR_PADDING_ISO10126:
                    cha = (byte_t)blk;
                    for (; blk > 1; blk--, rst++)
                        tmp[rst] = (byte_t)rand_getx(256);
                    tmp[rst] = cha;
                    break;
            }
        }
        func(dst, dstblk, tmp, srcblk, ctx);
    }
    return (size);
}

/*
=======================================
    EBC 模式解密
=======================================
*/
CR_API leng_t
crypto_all_ebc_dec (
  __CR_IN__ void_t*         ctx,
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ leng_t          dstblk,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen,
  __CR_IN__ leng_t          srcblk,
  __CR_IN__ enc_parm_t      func
    )
{
    leng_t  blk, size;

    /* 安全检查 */
    if (srclen % srcblk != 0)
        return (0);

    /* 计算目标大小 */
    if (dstblk == 0)
        dstblk = srcblk;
    blk = srclen / srcblk;
    size = blk * dstblk;
    if (dst == NULL)
        return (size);

    /* 安全检查 */
    if (size > dstlen)
        return (0);

    /* 分块解密 */
    for (; blk != 0; blk--) {
        func(dst, dstblk, src, srcblk, ctx);
        dst = (byte_t*)dst + dstblk;
        src = (byte_t*)src + srcblk;
    }
    return (size);
}

/*****************************************************************************/
/*                                   CBC                                     */
/*****************************************************************************/

/*
=======================================
    CBC 模式加密
=======================================
*/
CR_API leng_t
crypto_all_cbc_enc (
  __CR_IN__ void_t*         ctx,
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen,
  __CR_IN__ const void_t*   ivec,
  __CR_IN__ leng_t          block,
  __CR_IN__ uint_t          fill,
  __CR_IN__ enc_parm_t      func
    )
{
    leng_t  idx, blk, rst, size;
    byte_t  cha, tmp1[32], tmp2[32];

    /* 安全检查 */
    if (block > sizeof(tmp1))
        return (0);

    /* 计算目标大小 */
    blk = srclen / block;
    rst = srclen % block;
    size = blk * block;
    if (rst != 0)
        size += block;
    if (dst == NULL)
        return (size);

    /* 安全检查 */
    if (size > dstlen)
        return (0);

    /* 填充类型检查 */
    if (fill > 0xFF) {
        switch (fill & (~0xFF))
        {
            default:
                return (0);

            case CR_PADDING_PKCS5:
                if (block != 8)
                    return (0);
            case CR_PADDING_PKCS7:
            case CR_PADDING_ANSI_X923:
            case CR_PADDING_ISO10126:
                break;
        }
    }

    /* 分块加密 */
    if (ivec != NULL)
        mem_cpy(tmp1, ivec, block);
    else
        mem_zero(tmp1, block);
    for (; blk != 0; blk--) {
        for (idx = 0; idx < block; idx++)
            tmp1[idx] ^= ((byte_t*)src)[idx];
        func(dst, block, tmp1, block, ctx);
        mem_cpy(tmp1, dst, block);
        src = (byte_t*)src + block;
        dst = (byte_t*)dst + block;
    }

    /* 处理尾部 */
    if (rst != 0) {
        mem_cpy(tmp2, src, rst);
        blk = block - rst;
        if (fill <= 0xFF) {
            cha = (byte_t)fill;
            for (; blk != 0; blk--, rst++)
                tmp2[rst] = cha;
        }
        else {
            switch (fill & (~0xFF))
            {
                default:
                    break;

                case CR_PADDING_PKCS5:
                case CR_PADDING_PKCS7:
                    cha = (byte_t)blk;
                    for (; blk != 0; blk--, rst++)
                        tmp2[rst] = cha;
                    break;

                case CR_PADDING_ANSI_X923:
                    cha = (byte_t)blk;
                    for (; blk > 1; blk--, rst++)
                        tmp2[rst] = 0x00;
                    tmp2[rst] = cha;
                    break;

                case CR_PADDING_ISO10126:
                    cha = (byte_t)blk;
                    for (; blk > 1; blk--, rst++)
                        tmp2[rst] = (byte_t)rand_getx(256);
                    tmp2[rst] = cha;
                    break;
            }
        }
        for (idx = 0; idx < block; idx++)
            tmp1[idx] ^= tmp2[idx];
        func(dst, block, tmp1, block, ctx);
    }
    return (size);
}

/*
=======================================
    CBC 模式解密
=======================================
*/
CR_API leng_t
crypto_all_cbc_dec (
  __CR_IN__ void_t*         ctx,
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen,
  __CR_IN__ const void_t*   ivec,
  __CR_IN__ leng_t          block,
  __CR_IN__ enc_parm_t      func
    )
{
    byte_t  tmp1[32];
    byte_t  tmp2[32];
    leng_t  idx, blk;

    /* 安全检查 */
    if (srclen % block != 0)
        return (0);

    /* 计算目标大小 */
    if (dst == NULL)
        return (srclen);

    /* 安全检查 */
    if (srclen > dstlen)
        return (0);
    blk = srclen / block;

    /* 分块解密 */
    if (ivec != NULL)
        mem_cpy(tmp1, ivec, block);
    else
        mem_zero(tmp1, block);
    for (; blk != 0; blk--) {
        mem_cpy(tmp2, src, block);
        func(dst, block, src, block, ctx);
        for (idx = 0; idx < block; idx++)
            ((byte_t*)dst)[idx] ^= tmp1[idx];
        mem_cpy(tmp1, tmp2, block);
        src = (byte_t*)src + block;
        dst = (byte_t*)dst + block;
    }
    return (srclen);
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
