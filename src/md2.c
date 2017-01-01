/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2014-04-21  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack MD2 哈希函数库 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "hash.h"

#if defined(_CR_OS_MACOSX_)
/*
=======================================
    MD2 初始化
=======================================
*/
CR_API void_t
hash_md2_init (
  __CR_OT__ sMD2*   ctx
    )
{
    CC_MD2_Init(ctx);
}

/*
=======================================
    MD2 计算数据块
=======================================
*/
CR_API void_t
hash_md2_update (
  __CR_IO__ sMD2*           ctx,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    CC_MD2_Update(ctx, data, (CC_LONG)size);
}

/*
=======================================
    MD2 获取结果
=======================================
*/
CR_API void_t
hash_md2_finish (
  __CR_OT__ byte_t  hash[16],
  __CR_IO__ sMD2*   ctx
    )
{
    CC_MD2_Final(hash, ctx);
}

/*
=======================================
    MD2 一次性完成
=======================================
*/
CR_API void_t
hash_md2_total (
  __CR_OT__ byte_t          hash[16],
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    CC_MD2(data, (CC_LONG)size, hash);
}

#else

#include "memlib.h"

/* MD2 计算常数表 */
static const byte_t _rom_ s_pi_subst[256] =
{
    0x29, 0x2E, 0x43, 0xC9, 0xA2, 0xD8, 0x7C, 0x01,
    0x3D, 0x36, 0x54, 0xA1, 0xEC, 0xF0, 0x06, 0x13,
    0x62, 0xA7, 0x05, 0xF3, 0xC0, 0xC7, 0x73, 0x8C,
    0x98, 0x93, 0x2B, 0xD9, 0xBC, 0x4C, 0x82, 0xCA,
    0x1E, 0x9B, 0x57, 0x3C, 0xFD, 0xD4, 0xE0, 0x16,
    0x67, 0x42, 0x6F, 0x18, 0x8A, 0x17, 0xE5, 0x12,
    0xBE, 0x4E, 0xC4, 0xD6, 0xDA, 0x9E, 0xDE, 0x49,
    0xA0, 0xFB, 0xF5, 0x8E, 0xBB, 0x2F, 0xEE, 0x7A,
    0xA9, 0x68, 0x79, 0x91, 0x15, 0xB2, 0x07, 0x3F,
    0x94, 0xC2, 0x10, 0x89, 0x0B, 0x22, 0x5F, 0x21,
    0x80, 0x7F, 0x5D, 0x9A, 0x5A, 0x90, 0x32, 0x27,
    0x35, 0x3E, 0xCC, 0xE7, 0xBF, 0xF7, 0x97, 0x03,
    0xFF, 0x19, 0x30, 0xB3, 0x48, 0xA5, 0xB5, 0xD1,
    0xD7, 0x5E, 0x92, 0x2A, 0xAC, 0x56, 0xAA, 0xC6,
    0x4F, 0xB8, 0x38, 0xD2, 0x96, 0xA4, 0x7D, 0xB6,
    0x76, 0xFC, 0x6B, 0xE2, 0x9C, 0x74, 0x04, 0xF1,
    0x45, 0x9D, 0x70, 0x59, 0x64, 0x71, 0x87, 0x20,
    0x86, 0x5B, 0xCF, 0x65, 0xE6, 0x2D, 0xA8, 0x02,
    0x1B, 0x60, 0x25, 0xAD, 0xAE, 0xB0, 0xB9, 0xF6,
    0x1C, 0x46, 0x61, 0x69, 0x34, 0x40, 0x7E, 0x0F,
    0x55, 0x47, 0xA3, 0x23, 0xDD, 0x51, 0xAF, 0x3A,
    0xC3, 0x5C, 0xF9, 0xCE, 0xBA, 0xC5, 0xEA, 0x26,
    0x2C, 0x53, 0x0D, 0x6E, 0x85, 0x28, 0x84, 0x09,
    0xD3, 0xDF, 0xCD, 0xF4, 0x41, 0x81, 0x4D, 0x52,
    0x6A, 0xDC, 0x37, 0xC8, 0x6C, 0xC1, 0xAB, 0xFA,
    0x24, 0xE1, 0x7B, 0x08, 0x0C, 0xBD, 0xB1, 0x4A,
    0x78, 0x88, 0x95, 0x8B, 0xE3, 0x63, 0xE8, 0x6D,
    0xE9, 0xCB, 0xD5, 0xFE, 0x3B, 0x00, 0x1D, 0x39,
    0xF2, 0xEF, 0xB7, 0x0E, 0x66, 0x58, 0xD0, 0xE4,
    0xA6, 0x77, 0x72, 0xF8, 0xEB, 0x75, 0x4B, 0x0A,
    0x31, 0x44, 0x50, 0xB4, 0x8F, 0xED, 0x1F, 0x1A,
    0xDB, 0x99, 0x8D, 0x33, 0x9F, 0x11, 0x83, 0x14,
};

/*
---------------------------------------
    MD2 变换计算
---------------------------------------
*/
static void_t
md2_transform (
  __CR_IO__ sMD2*   ctx
    )
{
    uint_t  ii, jj;
    byte_t  tt = 0;

    for (ii = 0; ii < 16; ii++) {
        ctx->state[ii + 16] = ctx->buffer[ii];
        ctx->state[ii + 32] = ctx->buffer[ii] ^ ctx->state[ii];
    }

    for (ii = 0; ii < 18; ii++) {
        for (jj = 0; jj < 48; jj++) {
            ctx->state[jj] ^= s_pi_subst[tt];
            tt = ctx->state[jj];
        }
        tt = (byte_t)(tt + ii);
    }

    tt = ctx->cksum[15];
    for (ii = 0; ii < 16; ii++) {
        ctx->cksum[ii] ^= s_pi_subst[ctx->buffer[ii] ^ tt];
        tt = ctx->cksum[ii];
    }
}

/*
=======================================
    MD2 初始化
=======================================
*/
CR_API void_t
hash_md2_init (
  __CR_OT__ sMD2*   ctx
    )
{
    struct_zero(ctx, sMD2);
}

/*
=======================================
    MD2 计算数据块
=======================================
*/
CR_API void_t
hash_md2_update (
  __CR_IO__ sMD2*           ctx,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    leng_t  fill;

    while (size != 0) {
        if (ctx->left + size > 16)
            fill = 16 - ctx->left;
        else
            fill = size;
        mem_cpy(ctx->buffer + ctx->left, data, fill);
        data = (byte_t*)data + fill;
        size -= fill;
        ctx->left += fill;
        if (ctx->left == 16) {
            ctx->left = 0;
            md2_transform(ctx);
        }
    }
}

/*
=======================================
    MD2 获取结果
=======================================
*/
CR_API void_t
hash_md2_finish (
  __CR_OT__ byte_t  hash[16],
  __CR_IO__ sMD2*   ctx
    )
{
    leng_t  idx;
    byte_t  xx = (byte_t)(16 - ctx->left);

    for (idx = ctx->left; idx < 16; idx++)
        ctx->buffer[idx] = xx;
    md2_transform(ctx);

    mem_cpy(ctx->buffer, ctx->cksum, 16);
    md2_transform(ctx);
    mem_cpy(hash, ctx->state, 16);
}

/*
=======================================
    MD2 一次性完成
=======================================
*/
CR_API void_t
hash_md2_total (
  __CR_OT__ byte_t          hash[16],
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    sMD2    ctx;

    hash_md2_init(&ctx);
    hash_md2_update(&ctx, data, size);
    hash_md2_finish(hash, &ctx);
}

#endif  /* _CR_OS_MACOSX_ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
