/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2014-04-16  */
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
/*  >>>>>>>>>>>>>>>>>>>> CrHack SHA-384/512 哈希函数库 <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "hash.h"

#if defined(_CR_OS_MACOSX_)
/*
=======================================
    SHA-384 初始化
=======================================
*/
CR_API void_t
hash_sha384_init (
  __CR_OT__ sSHA384*    ctx
    )
{
    CC_SHA384_Init(ctx);
}

/*
=======================================
    SHA-384 计算数据块
=======================================
*/
CR_API void_t
hash_sha384_update (
  __CR_IO__ sSHA384*        ctx,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    CC_SHA384_Update(ctx, data, (CC_LONG)size);
}

/*
=======================================
    SHA-384 获取结果
=======================================
*/
CR_API void_t
hash_sha384_finish (
  __CR_OT__ byte_t      hash[48],
  __CR_IO__ sSHA384*    ctx
    )
{
    CC_SHA384_Final(hash, ctx);
}

/*
=======================================
    SHA-384 一次性完成
=======================================
*/
CR_API void_t
hash_sha384_total (
  __CR_OT__ byte_t          hash[48],
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    CC_SHA384(data, (CC_LONG)size, hash);
}

/*
=======================================
    SHA-512 初始化
=======================================
*/
CR_API void_t
hash_sha512_init (
  __CR_OT__ sSHA512*    ctx
    )
{
    CC_SHA512_Init(ctx);
}

/*
=======================================
    SHA-512 计算数据块
=======================================
*/
CR_API void_t
hash_sha512_update (
  __CR_IO__ sSHA512*        ctx,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    CC_SHA512_Update(ctx, data, (CC_LONG)size);
}

/*
=======================================
    SHA-512 获取结果
=======================================
*/
CR_API void_t
hash_sha512_finish (
  __CR_OT__ byte_t      hash[64],
  __CR_IO__ sSHA512*    ctx
    )
{
    CC_SHA512_Final(hash, ctx);
}

/*
=======================================
    SHA-512 一次性完成
=======================================
*/
CR_API void_t
hash_sha512_total (
  __CR_OT__ byte_t          hash[64],
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    CC_SHA512(data, (CC_LONG)size, hash);
}

#else

#include "morder.h"
#include "memlib.h"

/*****************************************************************************/
/*                                 SHA-512                                   */
/*****************************************************************************/

/* SHA-512 计算宏 */
#define SHA5_CH(x, y, z)    ((z) ^ ((x) & ((y) ^ (z))))
#define SHA5_MA(x, y, z)    (((x) & (y)) | ((z) & ((x) | (y))))

#define SHA5_E0(x)  (rotr_int64u(x, 28) ^ rotr_int64u(x, 34) ^ \
                     rotr_int64u(x, 39))
#define SHA5_E1(x)  (rotr_int64u(x, 14) ^ rotr_int64u(x, 18) ^ \
                     rotr_int64u(x, 41))
#define SHA5_S0(x)  (rotr_int64u(x,  1) ^ rotr_int64u(x,  8) ^ ((x) >> 7))
#define SHA5_S1(x)  (rotr_int64u(x, 19) ^ rotr_int64u(x, 61) ^ ((x) >> 6))

#define SHA5_BLEND(i)   xx[i] = SHA5_S1(xx[(i) -  2]) + xx[(i) -  7] + \
                                SHA5_S0(xx[(i) - 15]) + xx[(i) - 16]
/* SHA-512 常数表 */
static const int64u _rom_ s_s5k[80] =
{
    CR_ULL(0x428A2F98D728AE22), CR_ULL(0x7137449123EF65CD),
    CR_ULL(0xB5C0FBCFEC4D3B2F), CR_ULL(0xE9B5DBA58189DBBC),
    CR_ULL(0x3956C25BF348B538), CR_ULL(0x59F111F1B605D019),
    CR_ULL(0x923F82A4AF194F9B), CR_ULL(0xAB1C5ED5DA6D8118),
    CR_ULL(0xD807AA98A3030242), CR_ULL(0x12835B0145706FBE),
    CR_ULL(0x243185BE4EE4B28C), CR_ULL(0x550C7DC3D5FFB4E2),
    CR_ULL(0x72BE5D74F27B896F), CR_ULL(0x80DEB1FE3B1696B1),
    CR_ULL(0x9BDC06A725C71235), CR_ULL(0xC19BF174CF692694),
    CR_ULL(0xE49B69C19EF14AD2), CR_ULL(0xEFBE4786384F25E3),
    CR_ULL(0x0FC19DC68B8CD5B5), CR_ULL(0x240CA1CC77AC9C65),
    CR_ULL(0x2DE92C6F592B0275), CR_ULL(0x4A7484AA6EA6E483),
    CR_ULL(0x5CB0A9DCBD41FBD4), CR_ULL(0x76F988DA831153B5),
    CR_ULL(0x983E5152EE66DFAB), CR_ULL(0xA831C66D2DB43210),
    CR_ULL(0xB00327C898FB213F), CR_ULL(0xBF597FC7BEEF0EE4),
    CR_ULL(0xC6E00BF33DA88FC2), CR_ULL(0xD5A79147930AA725),
    CR_ULL(0x06CA6351E003826F), CR_ULL(0x142929670A0E6E70),
    CR_ULL(0x27B70A8546D22FFC), CR_ULL(0x2E1B21385C26C926),
    CR_ULL(0x4D2C6DFC5AC42AED), CR_ULL(0x53380D139D95B3DF),
    CR_ULL(0x650A73548BAF63DE), CR_ULL(0x766A0ABB3C77B2A8),
    CR_ULL(0x81C2C92E47EDAEE6), CR_ULL(0x92722C851482353B),
    CR_ULL(0xA2BFE8A14CF10364), CR_ULL(0xA81A664BBC423001),
    CR_ULL(0xC24B8B70D0F89791), CR_ULL(0xC76C51A30654BE30),
    CR_ULL(0xD192E819D6EF5218), CR_ULL(0xD69906245565A910),
    CR_ULL(0xF40E35855771202A), CR_ULL(0x106AA07032BBD1B8),
    CR_ULL(0x19A4C116B8D2D0C8), CR_ULL(0x1E376C085141AB53),
    CR_ULL(0x2748774CDF8EEB99), CR_ULL(0x34B0BCB5E19B48A8),
    CR_ULL(0x391C0CB3C5C95A63), CR_ULL(0x4ED8AA4AE3418ACB),
    CR_ULL(0x5B9CCA4F7763E373), CR_ULL(0x682E6FF3D6B2B8A3),
    CR_ULL(0x748F82EE5DEFB2FC), CR_ULL(0x78A5636F43172F60),
    CR_ULL(0x84C87814A1F0AB72), CR_ULL(0x8CC702081A6439EC),
    CR_ULL(0x90BEFFFA23631E28), CR_ULL(0xA4506CEBDE82BDE9),
    CR_ULL(0xBEF9A3F7B2C67915), CR_ULL(0xC67178F2E372532B),
    CR_ULL(0xCA273ECEEA26619C), CR_ULL(0xD186B8C721C0C207),
    CR_ULL(0xEADA7DD6CDE0EB1E), CR_ULL(0xF57D4F7FEE6ED178),
    CR_ULL(0x06F067AA72176FBA), CR_ULL(0x0A637DC5A2C898A6),
    CR_ULL(0x113F9804BEF90DAE), CR_ULL(0x1B710B35131C471B),
    CR_ULL(0x28DB77F523047D84), CR_ULL(0x32CAAB7B40C72493),
    CR_ULL(0x3C9EBE0A15C9BEBC), CR_ULL(0x431D67C49C100D4C),
    CR_ULL(0x4CC5D4BECB3E42B6), CR_ULL(0x597F299CFC657E2A),
    CR_ULL(0x5FCB6FAB3AD6FAEC), CR_ULL(0x6C44198C4A475817),
};

/* SHA-512 填充数组 */
static const byte_t _rom_ s_padding[128] =
{
    0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

/*
---------------------------------------
    SHA-512 变换计算
---------------------------------------
*/
static void_t
sha512_transform (
  __CR_IO__ int64u          state[8],
  __CR_IN__ const int64u    block[16]
    )
{
    uint_t  idx;
    int64u  aa = state[0];
    int64u  bb = state[1];
    int64u  cc = state[2];
    int64u  dd = state[3];
    int64u  ee = state[4];
    int64u  ff = state[5];
    int64u  gg = state[6];
    int64u  hh = state[7];
    int64u  t1, t2, xx[80];

    for (idx =  0; idx < 16; idx++)
        xx[idx] = QWORD_BE(block[idx]);
    for (idx = 16; idx < 80; idx++)
        SHA5_BLEND(idx);

    for (idx = 0; idx < 80; idx += 8) {
        t1 = hh + SHA5_E1(ee) + SHA5_CH(ee, ff, gg) + s_s5k[idx+0] + xx[idx+0];
        t2 = SHA5_E0(aa) + SHA5_MA(aa, bb, cc); dd += t1; hh = t1 + t2;
        t1 = gg + SHA5_E1(dd) + SHA5_CH(dd, ee, ff) + s_s5k[idx+1] + xx[idx+1];
        t2 = SHA5_E0(hh) + SHA5_MA(hh, aa, bb); cc += t1; gg = t1 + t2;
        t1 = ff + SHA5_E1(cc) + SHA5_CH(cc, dd, ee) + s_s5k[idx+2] + xx[idx+2];
        t2 = SHA5_E0(gg) + SHA5_MA(gg, hh, aa); bb += t1; ff = t1 + t2;
        t1 = ee + SHA5_E1(bb) + SHA5_CH(bb, cc, dd) + s_s5k[idx+3] + xx[idx+3];
        t2 = SHA5_E0(ff) + SHA5_MA(ff, gg, hh); aa += t1; ee = t1 + t2;
        t1 = dd + SHA5_E1(aa) + SHA5_CH(aa, bb, cc) + s_s5k[idx+4] + xx[idx+4];
        t2 = SHA5_E0(ee) + SHA5_MA(ee, ff, gg); hh += t1; dd = t1 + t2;
        t1 = cc + SHA5_E1(hh) + SHA5_CH(hh, aa, bb) + s_s5k[idx+5] + xx[idx+5];
        t2 = SHA5_E0(dd) + SHA5_MA(dd, ee, ff); gg += t1; cc = t1 + t2;
        t1 = bb + SHA5_E1(gg) + SHA5_CH(gg, hh, aa) + s_s5k[idx+6] + xx[idx+6];
        t2 = SHA5_E0(cc) + SHA5_MA(cc, dd, ee); ff += t1; bb = t1 + t2;
        t1 = aa + SHA5_E1(ff) + SHA5_CH(ff, gg, hh) + s_s5k[idx+7] + xx[idx+7];
        t2 = SHA5_E0(bb) + SHA5_MA(bb, cc, dd); ee += t1; aa = t1 + t2;
    }

    state[0] += aa;
    state[1] += bb;
    state[2] += cc;
    state[3] += dd;
    state[4] += ee;
    state[5] += ff;
    state[6] += gg;
    state[7] += hh;
}

/*
=======================================
    SHA-512 初始化
=======================================
*/
CR_API void_t
hash_sha512_init (
  __CR_OT__ sSHA512*    ctx
    )
{
    ctx->count[0] = CR_ULL(0);
    ctx->count[1] = CR_ULL(0);
    ctx->state[0] = CR_ULL(0x6A09E667F3BCC908);
    ctx->state[1] = CR_ULL(0xBB67AE8584CAA73B);
    ctx->state[2] = CR_ULL(0x3C6EF372FE94F82B);
    ctx->state[3] = CR_ULL(0xA54FF53A5F1D36F1);
    ctx->state[4] = CR_ULL(0x510E527FADE682D1);
    ctx->state[5] = CR_ULL(0x9B05688C2B3E6C1F);
    ctx->state[6] = CR_ULL(0x1F83D9ABFB41BD6B);
    ctx->state[7] = CR_ULL(0x5BE0CD19137E2179);
}

/*
=======================================
    SHA-512 计算数据块
=======================================
*/
CR_API void_t
hash_sha512_update (
  __CR_IO__ sSHA512*        ctx,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    int64u  temp;
    uint_t  rest, fill;

    rest = (uint_t)(ctx->count[0] & 0x7F);
    temp = (int64u)(ctx->count[0] + size);
    if (temp < ctx->count[0])
        ctx->count[1]++;
    ctx->count[0] = temp;
    fill = 128 - rest;
    if (size >= fill) {
        mem_cpy(&ctx->buffer[rest], data, fill);
        data = (byte_t*)data + fill;
        sha512_transform(ctx->state, (int64u*)ctx->buffer);
        size -= fill;
        while (size >= 128) {
            mem_cpy(ctx->buffer, data, 128);
            sha512_transform(ctx->state, (int64u*)ctx->buffer);
            data = (byte_t*)data + 128;
            size -= 128;
        }
        rest = 0;
    }
    if (size != 0)
        mem_cpy(&ctx->buffer[rest], data, size);
}

/*
=======================================
    SHA-512 获取结果
=======================================
*/
CR_API void_t
hash_sha512_finish (
  __CR_OT__ byte_t      hash[64],
  __CR_IO__ sSHA512*    ctx
    )
{
    int64u  bits[2];
    uint_t  idx, len;

    idx = (uint_t)(ctx->count[0] & 0x7F);
    len = (idx < 112) ? (112 - idx) : (240 - idx);
    bits[0]  = ctx->count[1] << 3;
    bits[0] |= ctx->count[0] >> 29;
    bits[1]  = ctx->count[0] << 3;
    bits[0]  = QWORD_BE(bits[0]);
    bits[1]  = QWORD_BE(bits[1]);
    hash_sha512_update(ctx, s_padding, len);
    hash_sha512_update(ctx, bits, 16);

    for (idx = 0; idx < 8; idx++)
        ctx->state[idx] = QWORD_BE(ctx->state[idx]);
    mem_cpy(hash, ctx->state, 64);
}

/*
=======================================
    SHA-512 一次性完成
=======================================
*/
CR_API void_t
hash_sha512_total (
  __CR_OT__ byte_t          hash[64],
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    sSHA512     ctx;

    hash_sha512_init(&ctx);
    hash_sha512_update(&ctx, data, size);
    hash_sha512_finish(hash, &ctx);
}

/*****************************************************************************/
/*                                 SHA-384                                   */
/*****************************************************************************/

/*
=======================================
    SHA-384 初始化
=======================================
*/
CR_API void_t
hash_sha384_init (
  __CR_OT__ sSHA384*    ctx
    )
{
    ctx->count[0] = CR_ULL(0);
    ctx->count[1] = CR_ULL(0);
    ctx->state[0] = CR_ULL(0xCBBB9D5DC1059ED8);
    ctx->state[1] = CR_ULL(0x629A292A367CD507);
    ctx->state[2] = CR_ULL(0x9159015A3070DD17);
    ctx->state[3] = CR_ULL(0x152FECD8F70E5939);
    ctx->state[4] = CR_ULL(0x67332667FFC00B31);
    ctx->state[5] = CR_ULL(0x8EB44A8768581511);
    ctx->state[6] = CR_ULL(0xDB0C2E0D64F98FA7);
    ctx->state[7] = CR_ULL(0x47B5481DBEFA4FA4);
}

/*
=======================================
    SHA-384 计算数据块
=======================================
*/
CR_API void_t
hash_sha384_update (
  __CR_IO__ sSHA384*        ctx,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    hash_sha512_update(ctx, data, size);
}

/*
=======================================
    SHA-384 获取结果
=======================================
*/
CR_API void_t
hash_sha384_finish (
  __CR_OT__ byte_t      hash[48],
  __CR_IO__ sSHA384*    ctx
    )
{
    byte_t  tmp[64];

    hash_sha512_finish(tmp, ctx);
    mem_cpy(hash, tmp, 48);
}

/*
=======================================
    SHA-384 一次性完成
=======================================
*/
CR_API void_t
hash_sha384_total (
  __CR_OT__ byte_t          hash[48],
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    sSHA384     ctx;

    hash_sha384_init(&ctx);
    hash_sha384_update(&ctx, data, size);
    hash_sha384_finish(hash, &ctx);
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
