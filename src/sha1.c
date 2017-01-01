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
/*  >>>>>>>>>>>>>>>>>>>>>>> CrHack SHA-1 哈希函数库 <<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "hash.h"

#if defined(_CR_OS_MACOSX_)
/*
=======================================
    SHA-1 初始化
=======================================
*/
CR_API void_t
hash_sha1_init (
  __CR_OT__ sSHA1*  ctx
    )
{
    CC_SHA1_Init(ctx);
}

/*
=======================================
    SHA-1 计算数据块
=======================================
*/
CR_API void_t
hash_sha1_update (
  __CR_IO__ sSHA1*          ctx,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    CC_SHA1_Update(ctx, data, (CC_LONG)size);
}

/*
=======================================
    SHA-1 获取结果
=======================================
*/
CR_API void_t
hash_sha1_finish (
  __CR_OT__ byte_t  hash[20],
  __CR_IO__ sSHA1*  ctx
    )
{
    CC_SHA1_Final(hash, ctx);
}

/*
=======================================
    SHA-1 一次性完成
=======================================
*/
CR_API void_t
hash_sha1_total (
  __CR_OT__ byte_t          hash[20],
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    CC_SHA1(data, (CC_LONG)size, hash);
}

#else

#include "morder.h"
#include "memlib.h"

/* SHA-1 计算宏 */
#define SHA1_K1     0x5A827999UL
#define SHA1_K2     0x6ED9EBA1UL
#define SHA1_K3     0x8F1BBCDCUL
#define SHA1_K4     0xCA62C1D6UL

#define SHA1_F1(x, y, z)    ((z) ^ ((x) & ((y) ^ (z))))
#define SHA1_F2(x, y, z)    ((x) ^ (y) ^ (z))
#define SHA1_F3(x, y, z)    (((x) & (y)) | ((z) & ((x) | (y))))
#define SHA1_F4(x, y, z)    ((x) ^ (y) ^ (z))

#define SHA1_M(i)   (tm = xx[((i) - 0) & 0x0F] ^ xx[((i) - 14) & 0x0F] ^ \
                          xx[((i) - 8) & 0x0F] ^ xx[((i) -  3) & 0x0F], \
                         (xx[((i) - 0) & 0x0F] = rotl_int32u(tm, 1)))

#define SHA1_R(a, b, c, d, e, f, k, m) \
                    { (e) += rotl_int32u(a, 5) + f(b, c, d) + (k) + (m); \
                      (b)  = rotl_int32u(b, 30); }
/* SHA-1 填充数组 */
static const byte_t _rom_ s_padding[64] =
{
    0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

/*
---------------------------------------
    SHA-1 变换计算
---------------------------------------
*/
static void_t
sha1_transform (
  __CR_IO__ int32u          state[5],
  __CR_IN__ const int32u    block[16]
    )
{
    uint_t  idx;
    int32u  tm, xx[16];
    int32u  aa = state[0];
    int32u  bb = state[1];
    int32u  cc = state[2];
    int32u  dd = state[3];
    int32u  ee = state[4];

    for (idx = 0; idx < 16; idx++)
        xx[idx] = DWORD_BE(block[idx]);

    SHA1_R(aa, bb, cc, dd, ee, SHA1_F1, SHA1_K1,     xx[ 0]);
    SHA1_R(ee, aa, bb, cc, dd, SHA1_F1, SHA1_K1,     xx[ 1]);
    SHA1_R(dd, ee, aa, bb, cc, SHA1_F1, SHA1_K1,     xx[ 2]);
    SHA1_R(cc, dd, ee, aa, bb, SHA1_F1, SHA1_K1,     xx[ 3]);
    SHA1_R(bb, cc, dd, ee, aa, SHA1_F1, SHA1_K1,     xx[ 4]);
    SHA1_R(aa, bb, cc, dd, ee, SHA1_F1, SHA1_K1,     xx[ 5]);
    SHA1_R(ee, aa, bb, cc, dd, SHA1_F1, SHA1_K1,     xx[ 6]);
    SHA1_R(dd, ee, aa, bb, cc, SHA1_F1, SHA1_K1,     xx[ 7]);
    SHA1_R(cc, dd, ee, aa, bb, SHA1_F1, SHA1_K1,     xx[ 8]);
    SHA1_R(bb, cc, dd, ee, aa, SHA1_F1, SHA1_K1,     xx[ 9]);
    SHA1_R(aa, bb, cc, dd, ee, SHA1_F1, SHA1_K1,     xx[10]);
    SHA1_R(ee, aa, bb, cc, dd, SHA1_F1, SHA1_K1,     xx[11]);
    SHA1_R(dd, ee, aa, bb, cc, SHA1_F1, SHA1_K1,     xx[12]);
    SHA1_R(cc, dd, ee, aa, bb, SHA1_F1, SHA1_K1,     xx[13]);
    SHA1_R(bb, cc, dd, ee, aa, SHA1_F1, SHA1_K1,     xx[14]);
    SHA1_R(aa, bb, cc, dd, ee, SHA1_F1, SHA1_K1,     xx[15]);
    SHA1_R(ee, aa, bb, cc, dd, SHA1_F1, SHA1_K1, SHA1_M(16));
    SHA1_R(dd, ee, aa, bb, cc, SHA1_F1, SHA1_K1, SHA1_M(17));
    SHA1_R(cc, dd, ee, aa, bb, SHA1_F1, SHA1_K1, SHA1_M(18));
    SHA1_R(bb, cc, dd, ee, aa, SHA1_F1, SHA1_K1, SHA1_M(19));
    SHA1_R(aa, bb, cc, dd, ee, SHA1_F2, SHA1_K2, SHA1_M(20));
    SHA1_R(ee, aa, bb, cc, dd, SHA1_F2, SHA1_K2, SHA1_M(21));
    SHA1_R(dd, ee, aa, bb, cc, SHA1_F2, SHA1_K2, SHA1_M(22));
    SHA1_R(cc, dd, ee, aa, bb, SHA1_F2, SHA1_K2, SHA1_M(23));
    SHA1_R(bb, cc, dd, ee, aa, SHA1_F2, SHA1_K2, SHA1_M(24));
    SHA1_R(aa, bb, cc, dd, ee, SHA1_F2, SHA1_K2, SHA1_M(25));
    SHA1_R(ee, aa, bb, cc, dd, SHA1_F2, SHA1_K2, SHA1_M(26));
    SHA1_R(dd, ee, aa, bb, cc, SHA1_F2, SHA1_K2, SHA1_M(27));
    SHA1_R(cc, dd, ee, aa, bb, SHA1_F2, SHA1_K2, SHA1_M(28));
    SHA1_R(bb, cc, dd, ee, aa, SHA1_F2, SHA1_K2, SHA1_M(29));
    SHA1_R(aa, bb, cc, dd, ee, SHA1_F2, SHA1_K2, SHA1_M(30));
    SHA1_R(ee, aa, bb, cc, dd, SHA1_F2, SHA1_K2, SHA1_M(31));
    SHA1_R(dd, ee, aa, bb, cc, SHA1_F2, SHA1_K2, SHA1_M(32));
    SHA1_R(cc, dd, ee, aa, bb, SHA1_F2, SHA1_K2, SHA1_M(33));
    SHA1_R(bb, cc, dd, ee, aa, SHA1_F2, SHA1_K2, SHA1_M(34));
    SHA1_R(aa, bb, cc, dd, ee, SHA1_F2, SHA1_K2, SHA1_M(35));
    SHA1_R(ee, aa, bb, cc, dd, SHA1_F2, SHA1_K2, SHA1_M(36));
    SHA1_R(dd, ee, aa, bb, cc, SHA1_F2, SHA1_K2, SHA1_M(37));
    SHA1_R(cc, dd, ee, aa, bb, SHA1_F2, SHA1_K2, SHA1_M(38));
    SHA1_R(bb, cc, dd, ee, aa, SHA1_F2, SHA1_K2, SHA1_M(39));
    SHA1_R(aa, bb, cc, dd, ee, SHA1_F3, SHA1_K3, SHA1_M(40));
    SHA1_R(ee, aa, bb, cc, dd, SHA1_F3, SHA1_K3, SHA1_M(41));
    SHA1_R(dd, ee, aa, bb, cc, SHA1_F3, SHA1_K3, SHA1_M(42));
    SHA1_R(cc, dd, ee, aa, bb, SHA1_F3, SHA1_K3, SHA1_M(43));
    SHA1_R(bb, cc, dd, ee, aa, SHA1_F3, SHA1_K3, SHA1_M(44));
    SHA1_R(aa, bb, cc, dd, ee, SHA1_F3, SHA1_K3, SHA1_M(45));
    SHA1_R(ee, aa, bb, cc, dd, SHA1_F3, SHA1_K3, SHA1_M(46));
    SHA1_R(dd, ee, aa, bb, cc, SHA1_F3, SHA1_K3, SHA1_M(47));
    SHA1_R(cc, dd, ee, aa, bb, SHA1_F3, SHA1_K3, SHA1_M(48));
    SHA1_R(bb, cc, dd, ee, aa, SHA1_F3, SHA1_K3, SHA1_M(49));
    SHA1_R(aa, bb, cc, dd, ee, SHA1_F3, SHA1_K3, SHA1_M(50));
    SHA1_R(ee, aa, bb, cc, dd, SHA1_F3, SHA1_K3, SHA1_M(51));
    SHA1_R(dd, ee, aa, bb, cc, SHA1_F3, SHA1_K3, SHA1_M(52));
    SHA1_R(cc, dd, ee, aa, bb, SHA1_F3, SHA1_K3, SHA1_M(53));
    SHA1_R(bb, cc, dd, ee, aa, SHA1_F3, SHA1_K3, SHA1_M(54));
    SHA1_R(aa, bb, cc, dd, ee, SHA1_F3, SHA1_K3, SHA1_M(55));
    SHA1_R(ee, aa, bb, cc, dd, SHA1_F3, SHA1_K3, SHA1_M(56));
    SHA1_R(dd, ee, aa, bb, cc, SHA1_F3, SHA1_K3, SHA1_M(57));
    SHA1_R(cc, dd, ee, aa, bb, SHA1_F3, SHA1_K3, SHA1_M(58));
    SHA1_R(bb, cc, dd, ee, aa, SHA1_F3, SHA1_K3, SHA1_M(59));
    SHA1_R(aa, bb, cc, dd, ee, SHA1_F4, SHA1_K4, SHA1_M(60));
    SHA1_R(ee, aa, bb, cc, dd, SHA1_F4, SHA1_K4, SHA1_M(61));
    SHA1_R(dd, ee, aa, bb, cc, SHA1_F4, SHA1_K4, SHA1_M(62));
    SHA1_R(cc, dd, ee, aa, bb, SHA1_F4, SHA1_K4, SHA1_M(63));
    SHA1_R(bb, cc, dd, ee, aa, SHA1_F4, SHA1_K4, SHA1_M(64));
    SHA1_R(aa, bb, cc, dd, ee, SHA1_F4, SHA1_K4, SHA1_M(65));
    SHA1_R(ee, aa, bb, cc, dd, SHA1_F4, SHA1_K4, SHA1_M(66));
    SHA1_R(dd, ee, aa, bb, cc, SHA1_F4, SHA1_K4, SHA1_M(67));
    SHA1_R(cc, dd, ee, aa, bb, SHA1_F4, SHA1_K4, SHA1_M(68));
    SHA1_R(bb, cc, dd, ee, aa, SHA1_F4, SHA1_K4, SHA1_M(69));
    SHA1_R(aa, bb, cc, dd, ee, SHA1_F4, SHA1_K4, SHA1_M(70));
    SHA1_R(ee, aa, bb, cc, dd, SHA1_F4, SHA1_K4, SHA1_M(71));
    SHA1_R(dd, ee, aa, bb, cc, SHA1_F4, SHA1_K4, SHA1_M(72));
    SHA1_R(cc, dd, ee, aa, bb, SHA1_F4, SHA1_K4, SHA1_M(73));
    SHA1_R(bb, cc, dd, ee, aa, SHA1_F4, SHA1_K4, SHA1_M(74));
    SHA1_R(aa, bb, cc, dd, ee, SHA1_F4, SHA1_K4, SHA1_M(75));
    SHA1_R(ee, aa, bb, cc, dd, SHA1_F4, SHA1_K4, SHA1_M(76));
    SHA1_R(dd, ee, aa, bb, cc, SHA1_F4, SHA1_K4, SHA1_M(77));
    SHA1_R(cc, dd, ee, aa, bb, SHA1_F4, SHA1_K4, SHA1_M(78));
    SHA1_R(bb, cc, dd, ee, aa, SHA1_F4, SHA1_K4, SHA1_M(79));

    state[0] += aa;
    state[1] += bb;
    state[2] += cc;
    state[3] += dd;
    state[4] += ee;
}

/*
=======================================
    SHA-1 初始化
=======================================
*/
CR_API void_t
hash_sha1_init (
  __CR_OT__ sSHA1*  ctx
    )
{
    ctx->count[0] = 0UL;
    ctx->count[1] = 0UL;
    ctx->state[0] = 0x67452301UL;
    ctx->state[1] = 0xEFCDAB89UL;
    ctx->state[2] = 0x98BADCFEUL;
    ctx->state[3] = 0x10325476UL;
    ctx->state[4] = 0xC3D2E1F0UL;
}

/*
=======================================
    SHA-1 计算数据块
=======================================
*/
CR_API void_t
hash_sha1_update (
  __CR_IO__ sSHA1*          ctx,
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
        sha1_transform(ctx->state, (int32u*)ctx->buffer);
        size -= fill;
        while (size >= 64) {
            mem_cpy(ctx->buffer, data, 64);
            sha1_transform(ctx->state, (int32u*)ctx->buffer);
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
    SHA-1 获取结果
=======================================
*/
CR_API void_t
hash_sha1_finish (
  __CR_OT__ byte_t  hash[20],
  __CR_IO__ sSHA1*  ctx
    )
{
    int32u  bits[2];
    uint_t  idx, len;

    idx = (uint_t)(ctx->count[0] & 0x3F);
    len = (idx < 56) ? (56 - idx) : (120 - idx);
    bits[0]  = ctx->count[1] << 3;
    bits[0] |= ctx->count[0] >> 29;
    bits[1]  = ctx->count[0] << 3;
    bits[0]  = DWORD_BE(bits[0]);
    bits[1]  = DWORD_BE(bits[1]);
    hash_sha1_update(ctx, s_padding, len);
    hash_sha1_update(ctx, bits, 8);

    for (idx = 0; idx < 5; idx++)
        ctx->state[idx] = DWORD_BE(ctx->state[idx]);
    mem_cpy(hash, ctx->state, 20);
}

/*
=======================================
    SHA-1 一次性完成
=======================================
*/
CR_API void_t
hash_sha1_total (
  __CR_OT__ byte_t          hash[20],
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    sSHA1   ctx;

    hash_sha1_init(&ctx);
    hash_sha1_update(&ctx, data, size);
    hash_sha1_finish(hash, &ctx);
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
