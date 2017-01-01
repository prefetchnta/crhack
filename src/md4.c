/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2009-12-18  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack MD4 哈希函数库 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "hash.h"

#if defined(_CR_OS_MACOSX_)
/*
=======================================
    MD4 初始化
=======================================
*/
CR_API void_t
hash_md4_init (
  __CR_OT__ sMD4*   ctx
    )
{
    CC_MD4_Init(ctx);
}

/*
=======================================
    MD4 计算数据块
=======================================
*/
CR_API void_t
hash_md4_update (
  __CR_IO__ sMD4*           ctx,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    CC_MD4_Update(ctx, data, (CC_LONG)size);
}

/*
=======================================
    MD4 获取结果
=======================================
*/
CR_API void_t
hash_md4_finish (
  __CR_OT__ byte_t  hash[16],
  __CR_IO__ sMD4*   ctx
    )
{
    CC_MD4_Final(hash, ctx);
}

/*
=======================================
    MD4 一次性完成
=======================================
*/
CR_API void_t
hash_md4_total (
  __CR_OT__ byte_t          hash[16],
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    CC_MD4(data, (CC_LONG)size, hash);
}

#else

#include "morder.h"
#include "memlib.h"

/* MD4 计算宏 */
#define MD4_H(x, y, z)  ((x) ^ (y) ^ (z))
#define MD4_F(x, y, z)  (((x) & (y)) | ((~(x)) & (z)))
#define MD4_G(x, y, z)  (((x) & (y)) | ((x) & (z)) | ((y) & (z)))

#define MD4_FF(a, b, c, d, x, s) \
                    { (a) += MD4_F(b, c, d) + (x); \
                      (a) = rotl_int32u(a, s); }

#define MD4_GG(a, b, c, d, x, s) \
                    { (a) += MD4_G(b, c, d) + (x) + 0x5A827999UL; \
                      (a) = rotl_int32u(a, s); }

#define MD4_HH(a, b, c, d, x, s) \
                    { (a) += MD4_H(b, c, d) + (x) + 0x6ED9EBA1UL; \
                      (a) = rotl_int32u(a, s); }
/* MD4 填充数组 */
static const byte_t _rom_ s_padding[64] =
{
    0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

/*
---------------------------------------
    MD4 变换计算
---------------------------------------
*/
static void_t
md4_transform (
  __CR_IO__ int32u          state[4],
  __CR_IN__ const int32u    block[16]
    )
{
    uint_t  idx;
    int32u  xx[16];
    int32u  aa = state[0];
    int32u  bb = state[1];
    int32u  cc = state[2];
    int32u  dd = state[3];

    for (idx = 0; idx < 16; idx++)
        xx[idx] = DWORD_LE(block[idx]);

    MD4_FF(aa, bb, cc, dd, xx[ 0],  3);
    MD4_FF(dd, aa, bb, cc, xx[ 1],  7);
    MD4_FF(cc, dd, aa, bb, xx[ 2], 11);
    MD4_FF(bb, cc, dd, aa, xx[ 3], 19);
    MD4_FF(aa, bb, cc, dd, xx[ 4],  3);
    MD4_FF(dd, aa, bb, cc, xx[ 5],  7);
    MD4_FF(cc, dd, aa, bb, xx[ 6], 11);
    MD4_FF(bb, cc, dd, aa, xx[ 7], 19);
    MD4_FF(aa, bb, cc, dd, xx[ 8],  3);
    MD4_FF(dd, aa, bb, cc, xx[ 9],  7);
    MD4_FF(cc, dd, aa, bb, xx[10], 11);
    MD4_FF(bb, cc, dd, aa, xx[11], 19);
    MD4_FF(aa, bb, cc, dd, xx[12],  3);
    MD4_FF(dd, aa, bb, cc, xx[13],  7);
    MD4_FF(cc, dd, aa, bb, xx[14], 11);
    MD4_FF(bb, cc, dd, aa, xx[15], 19);

    MD4_GG(aa, bb, cc, dd, xx[ 0],  3);
    MD4_GG(dd, aa, bb, cc, xx[ 4],  5);
    MD4_GG(cc, dd, aa, bb, xx[ 8],  9);
    MD4_GG(bb, cc, dd, aa, xx[12], 13);
    MD4_GG(aa, bb, cc, dd, xx[ 1],  3);
    MD4_GG(dd, aa, bb, cc, xx[ 5],  5);
    MD4_GG(cc, dd, aa, bb, xx[ 9],  9);
    MD4_GG(bb, cc, dd, aa, xx[13], 13);
    MD4_GG(aa, bb, cc, dd, xx[ 2],  3);
    MD4_GG(dd, aa, bb, cc, xx[ 6],  5);
    MD4_GG(cc, dd, aa, bb, xx[10],  9);
    MD4_GG(bb, cc, dd, aa, xx[14], 13);
    MD4_GG(aa, bb, cc, dd, xx[ 3],  3);
    MD4_GG(dd, aa, bb, cc, xx[ 7],  5);
    MD4_GG(cc, dd, aa, bb, xx[11],  9);
    MD4_GG(bb, cc, dd, aa, xx[15], 13);

    MD4_HH(aa, bb, cc, dd, xx[ 0],  3);
    MD4_HH(dd, aa, bb, cc, xx[ 8],  9);
    MD4_HH(cc, dd, aa, bb, xx[ 4], 11);
    MD4_HH(bb, cc, dd, aa, xx[12], 15);
    MD4_HH(aa, bb, cc, dd, xx[ 2],  3);
    MD4_HH(dd, aa, bb, cc, xx[10],  9);
    MD4_HH(cc, dd, aa, bb, xx[ 6], 11);
    MD4_HH(bb, cc, dd, aa, xx[14], 15);
    MD4_HH(aa, bb, cc, dd, xx[ 1],  3);
    MD4_HH(dd, aa, bb, cc, xx[ 9],  9);
    MD4_HH(cc, dd, aa, bb, xx[ 5], 11);
    MD4_HH(bb, cc, dd, aa, xx[13], 15);
    MD4_HH(aa, bb, cc, dd, xx[ 3],  3);
    MD4_HH(dd, aa, bb, cc, xx[11],  9);
    MD4_HH(cc, dd, aa, bb, xx[ 7], 11);
    MD4_HH(bb, cc, dd, aa, xx[15], 15);

    state[0] += aa;
    state[1] += bb;
    state[2] += cc;
    state[3] += dd;
}

/*
=======================================
    MD4 初始化
=======================================
*/
CR_API void_t
hash_md4_init (
  __CR_OT__ sMD4*   ctx
    )
{
    ctx->count[0] = 0UL;
    ctx->count[1] = 0UL;
    ctx->state[0] = 0x67452301UL;
    ctx->state[1] = 0xEFCDAB89UL;
    ctx->state[2] = 0x98BADCFEUL;
    ctx->state[3] = 0x10325476UL;
}

/*
=======================================
    MD4 计算数据块
=======================================
*/
CR_API void_t
hash_md4_update (
  __CR_IO__ sMD4*           ctx,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    int32u  temp;
    uint_t  rest, fill;

    rest = (uint_t)(ctx->count[0] & 0x3F);
    temp = (int32u)(ctx->count[0] + size);
    if (temp < ctx->count[0])
        ctx->count[1]++;
    ctx->count[0] = temp;
#if defined(_CR_SYS64_)
    ctx->count[1] += (int32u)(size >> 32);
#endif
    fill = 64 - rest;
    if (size >= fill) {
        mem_cpy(&ctx->buffer[rest], data, fill);
        data = (byte_t*)data + fill;
        md4_transform(ctx->state, (int32u*)ctx->buffer);
        size -= fill;
        while (size >= 64) {
            mem_cpy(ctx->buffer, data, 64);
            md4_transform(ctx->state, (int32u*)ctx->buffer);
            data = (byte_t*)data + 64;
            size -= 64;
        }
        rest = 0;
    }
    if (size != 0)
        mem_cpy(&ctx->buffer[rest], data, size);
}

/*
=======================================
    MD4 获取结果
=======================================
*/
CR_API void_t
hash_md4_finish (
  __CR_OT__ byte_t  hash[16],
  __CR_IO__ sMD4*   ctx
    )
{
    int32u  bits[2];
    uint_t  idx, len;

    idx = (uint_t)(ctx->count[0] & 0x3F);
    len = (idx < 56) ? (56 - idx) : (120 - idx);
    bits[1]  = ctx->count[1] << 3;
    bits[1] |= ctx->count[0] >> 29;
    bits[0]  = ctx->count[0] << 3;
    bits[0]  = DWORD_LE(bits[0]);
    bits[1]  = DWORD_LE(bits[1]);
    hash_md4_update(ctx, s_padding, len);
    hash_md4_update(ctx, bits, 8);

    for (idx = 0; idx < 4; idx++)
        ctx->state[idx] = DWORD_LE(ctx->state[idx]);
    mem_cpy(hash, ctx->state, 16);
}

/*
=======================================
    MD4 一次性完成
=======================================
*/
CR_API void_t
hash_md4_total (
  __CR_OT__ byte_t          hash[16],
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    sMD4    ctx;

    hash_md4_init(&ctx);
    hash_md4_update(&ctx, data, size);
    hash_md4_finish(hash, &ctx);
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
