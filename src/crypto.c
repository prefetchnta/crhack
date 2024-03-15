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

/* 最大块的字节数 */
#ifndef _CR_CRYPTO_MAX_BLK_
    #define _CR_CRYPTO_MAX_BLK_ 256
#endif

/*
---------------------------------------
    计算目标填充大小
---------------------------------------
*/
static leng_t
crypto_padding_size (
  __CR_IN__ leng_t  unit,
  __CR_IN__ leng_t  rest,
  __CR_IN__ uint_t  fill,
  __CR_IN__ leng_t  count
    )
{
    if (fill > 0xFF) {
        switch (fill & (~0xFF))
        {
            default:
                return (0);

            case CR_PADDING_PKCS5:
                if (unit != 8)
                    return (0);
            case CR_PADDING_PKCS7:
            case CR_PADDING_ANSI_X923:
            case CR_PADDING_ISO10126:
                break;

            case CR_PADDING_NOTHING:
                if (rest != 0 || count == 0)
                    return (0);
                count--;
                break;
        }
    }
    return ((count + 1) * unit);
}

/*
---------------------------------------
    填充目标数据数组
---------------------------------------
*/
static void_t
crypto_padding_data (
  __CR_IN__ leng_t  unit,
  __CR_IN__ leng_t  rest,
  __CR_IN__ uint_t  fill,
  __CR_IO__ void_t* buffs
    )
{
    byte_t  cha;

    unit -= rest;
    if (fill <= 0xFF) {
        cha = (byte_t)fill;
        for (; unit != 0; unit--, rest++)
            ((byte_t*)buffs)[rest] = cha;
        return;
    }

    switch (fill & (~0xFF))
    {
        default:
            break;

        case CR_PADDING_PKCS5:
        case CR_PADDING_PKCS7:
            cha = (byte_t)unit;
            for (; unit != 0; unit--, rest++)
                ((byte_t*)buffs)[rest] = cha;
            break;

        case CR_PADDING_ANSI_X923:
            cha = (byte_t)unit;
            for (; unit > 1; unit--, rest++)
                ((byte_t*)buffs)[rest] = 0x00;
            ((byte_t*)buffs)[rest] = cha;
            break;

        case CR_PADDING_ISO10126:
            cha = (byte_t)unit;
            for (; unit > 1; unit--, rest++)
                ((byte_t*)buffs)[rest] = (byte_t)rand_getx(256);
            ((byte_t*)buffs)[rest] = cha;
            break;
    }
}

/*
=======================================
    处理尾部数据填充
=======================================
*/
CR_API leng_t
crypto_padding (
  __CR_IO__ void_t* data,
  __CR_IN__ leng_t  size,
  __CR_IN__ leng_t  unit,
  __CR_IN__ uint_t  fill
    )
{
    leng_t  blk, rst;

    /* 计算填充长度 */
    if (unit <= 1)
        return (0);
    blk = size / unit;
    rst = size % unit;
    size = crypto_padding_size(unit, rst, fill, blk);
    if (data == NULL || size == 0)
        return (size);
    blk *= unit;

    /* 执行填充的动作 */
    crypto_padding_data(unit, rst, fill, ((byte_t*)data) + blk);
    return (size);
}

/*
=======================================
    返回真实数据长度
=======================================
*/
CR_API leng_t
crypto_real_size (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size,
  __CR_IN__ leng_t          unit,
  __CR_IN__ uint_t          fill
    )
{
    uint_t  cha;

    if (size == 0 ||
        unit <= 1 ||
        size % unit != 0)
        return (size);

    cha = ((byte_t*)data)[size - 1];
    if (fill <= 0xFF)
        return ((cha != fill) ? 0 : size);

    switch (fill & (~0xFF))
    {
        default:
            break;

        case CR_PADDING_PKCS5:
        case CR_PADDING_PKCS7:
        case CR_PADDING_ANSI_X923:
        case CR_PADDING_ISO10126:
            if (cha > unit)
                break;
            return (size - cha);

        case CR_PADDING_NOTHING:
            return (size);
    }
    return (0);
}

/*****************************************************************************/
/*                                   EBC                                     */
/*****************************************************************************/

/*
=======================================
    EBC 模式变换
=======================================
*/
CR_API leng_t
crypto_all_ebc_ops (
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
    if (srclen == 0 ||
        srcblk <= 1 ||
        srclen % srcblk != 0)
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

    /* 分块变换 */
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
  __CR_IN__ enc_parm_t      func
    )
{
    leng_t  idx, blk;
    byte_t  tmp[_CR_CRYPTO_MAX_BLK_];

    /* 安全检查 */
    if (srclen == 0 || block <= 1 ||
        block > sizeof(tmp) || srclen % block != 0)
        return (0);

    /* 计算目标大小 */
    if (dst == NULL)
        return (srclen);

    /* 安全检查 */
    if (srclen > dstlen)
        return (0);
    blk = srclen / block;

    /* 分块加密 */
    if (ivec != NULL)
        mem_cpy(tmp, ivec, block);
    else
        mem_zero(tmp, block);
    for (; blk != 0; blk--) {
        for (idx = 0; idx < block; idx++)
            tmp[idx] ^= ((byte_t*)src)[idx];
        func(dst, block, tmp, block, ctx);
        mem_cpy(tmp, dst, block);
        src = (byte_t*)src + block;
        dst = (byte_t*)dst + block;
    }
    return (srclen);
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
    leng_t  idx, blk;
    byte_t  tmp1[_CR_CRYPTO_MAX_BLK_];
    byte_t  tmp2[_CR_CRYPTO_MAX_BLK_];

    /* 安全检查 */
    if (srclen == 0 || block <= 1 ||
        block > sizeof(tmp1) || srclen % block != 0)
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
/*                                   CTR                                     */
/*****************************************************************************/

/*
---------------------------------------
    计数器加1
---------------------------------------
*/
static void_t
counter_add_one (
  __CR_IO__ byte_t* value,
  __CR_IN__ leng_t  block,
  __CR_IN__ bool_t  is_be
    )
{
    leng_t  idx;

    if (is_be) {
        for (idx = block; idx != 0; idx--) {
            if (value[idx - 1]++ != 0xFF)
                break;
        }
    }
    else {
        for (idx = 0; idx < block; idx++) {
            if (value[idx]++ == 0xFF)
                break;
        }
    }
}

/*
=======================================
    CTR 模式变换
=======================================
*/
CR_API leng_t
crypto_all_ctr_ops (
  __CR_IN__ void_t*         ctx,
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen,
  __CR_IN__ const void_t*   cntr,
  __CR_IN__ leng_t          block,
  __CR_IN__ bool_t          is_be,
  __CR_IN__ enc_parm_t      func
    )
{
    leng_t  idx, blk, rst;
    byte_t  tmp1[_CR_CRYPTO_MAX_BLK_];
    byte_t  tmp2[_CR_CRYPTO_MAX_BLK_];

    /* 安全检查 */
    if (srclen == 0 ||
        block  <= 1 || block > sizeof(tmp1))
        return (0);

    /* 计算目标大小 */
    if (dst == NULL)
        return (srclen);

    /* 安全检查 */
    if (srclen > dstlen)
        return (0);
    blk = srclen / block;
    rst = srclen % block;

    /* 分块变换 */
    if (cntr != NULL)
        mem_cpy(tmp1, cntr, block);
    else
        mem_zero(tmp1, block);
    for (; blk != 0; blk--) {
        func(tmp2, block, tmp1, block, ctx);
        for (idx = 0; idx < block; idx++)
            ((byte_t*)dst)[idx] = ((byte_t*)src)[idx] ^ tmp2[idx];
        src = (byte_t*)src + block;
        dst = (byte_t*)dst + block;
        counter_add_one(tmp1, block, is_be);
    }

    /* 处理尾部 */
    if (rst != 0) {
        func(tmp2, block, tmp1, block, ctx);
        for (idx = 0; idx < rst; idx++)
            ((byte_t*)dst)[idx] = ((byte_t*)src)[idx] ^ tmp2[idx];
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
