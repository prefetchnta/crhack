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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack MD5 哈希函数库 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "hash.h"

#if defined(_CR_OS_MACOSX_)
/*
=======================================
    MD5 初始化
=======================================
*/
CR_API void_t
hash_md5_init (
  __CR_OT__ sMD5*   ctx
    )
{
    CC_MD5_Init(ctx);
}

/*
=======================================
    MD5 计算数据块
=======================================
*/
CR_API void_t
hash_md5_update (
  __CR_IO__ sMD5*           ctx,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    CC_MD5_Update(ctx, data, (CC_LONG)size);
}

/*
=======================================
    MD5 获取结果
=======================================
*/
CR_API void_t
hash_md5_finish (
  __CR_OT__ byte_t  hash[16],
  __CR_IO__ sMD5*   ctx
    )
{
    CC_MD5_Final(hash, ctx);
}

/*
=======================================
    MD5 一次性完成
=======================================
*/
CR_API void_t
hash_md5_total (
  __CR_OT__ byte_t          hash[16],
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    CC_MD5(data, (CC_LONG)size, hash);
}

#else

#include "morder.h"
#include "memlib.h"

/* MD5 计算宏 */
#define MD5_F1(x, y, z)     ((z) ^ ((x) & ((y) ^ (z))))
#define MD5_F2(x, y, z)     ((y) ^ ((z) & ((x) ^ (y))))
#define MD5_F3(x, y, z)     ((x) ^ (y) ^ (z))
#define MD5_F4(x, y, z)     ((y) ^ ((x) | (~(z))))

#define MD5_STEP(f, w, x, y, z, dat, s) \
            ((w) += f(x, y, z) + (dat), (w) = rotl_int32u(w, s), (w) += (x))

/* MD5 填充数组 */
static const byte_t _rom_ s_padding[64] =
{
    0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

/*
---------------------------------------
    MD5 变换计算
---------------------------------------
*/
static void_t
md5_transform (
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

    MD5_STEP(MD5_F1, aa, bb, cc, dd, xx[ 0] + 0xD76AA478UL,  7);
    MD5_STEP(MD5_F1, dd, aa, bb, cc, xx[ 1] + 0xE8C7B756UL, 12);
    MD5_STEP(MD5_F1, cc, dd, aa, bb, xx[ 2] + 0x242070DBUL, 17);
    MD5_STEP(MD5_F1, bb, cc, dd, aa, xx[ 3] + 0xC1BDCEEEUL, 22);
    MD5_STEP(MD5_F1, aa, bb, cc, dd, xx[ 4] + 0xF57C0FAFUL,  7);
    MD5_STEP(MD5_F1, dd, aa, bb, cc, xx[ 5] + 0x4787C62AUL, 12);
    MD5_STEP(MD5_F1, cc, dd, aa, bb, xx[ 6] + 0xA8304613UL, 17);
    MD5_STEP(MD5_F1, bb, cc, dd, aa, xx[ 7] + 0xFD469501UL, 22);
    MD5_STEP(MD5_F1, aa, bb, cc, dd, xx[ 8] + 0x698098D8UL,  7);
    MD5_STEP(MD5_F1, dd, aa, bb, cc, xx[ 9] + 0x8B44F7AFUL, 12);
    MD5_STEP(MD5_F1, cc, dd, aa, bb, xx[10] + 0xFFFF5BB1UL, 17);
    MD5_STEP(MD5_F1, bb, cc, dd, aa, xx[11] + 0x895CD7BEUL, 22);
    MD5_STEP(MD5_F1, aa, bb, cc, dd, xx[12] + 0x6B901122UL,  7);
    MD5_STEP(MD5_F1, dd, aa, bb, cc, xx[13] + 0xFD987193UL, 12);
    MD5_STEP(MD5_F1, cc, dd, aa, bb, xx[14] + 0xA679438EUL, 17);
    MD5_STEP(MD5_F1, bb, cc, dd, aa, xx[15] + 0x49B40821UL, 22);

    MD5_STEP(MD5_F2, aa, bb, cc, dd, xx[ 1] + 0xF61E2562UL,  5);
    MD5_STEP(MD5_F2, dd, aa, bb, cc, xx[ 6] + 0xC040B340UL,  9);
    MD5_STEP(MD5_F2, cc, dd, aa, bb, xx[11] + 0x265E5A51UL, 14);
    MD5_STEP(MD5_F2, bb, cc, dd, aa, xx[ 0] + 0xE9B6C7AAUL, 20);
    MD5_STEP(MD5_F2, aa, bb, cc, dd, xx[ 5] + 0xD62F105DUL,  5);
    MD5_STEP(MD5_F2, dd, aa, bb, cc, xx[10] + 0x02441453UL,  9);
    MD5_STEP(MD5_F2, cc, dd, aa, bb, xx[15] + 0xD8A1E681UL, 14);
    MD5_STEP(MD5_F2, bb, cc, dd, aa, xx[ 4] + 0xE7D3FBC8UL, 20);
    MD5_STEP(MD5_F2, aa, bb, cc, dd, xx[ 9] + 0x21E1CDE6UL,  5);
    MD5_STEP(MD5_F2, dd, aa, bb, cc, xx[14] + 0xC33707D6UL,  9);
    MD5_STEP(MD5_F2, cc, dd, aa, bb, xx[ 3] + 0xF4D50D87UL, 14);
    MD5_STEP(MD5_F2, bb, cc, dd, aa, xx[ 8] + 0x455A14EDUL, 20);
    MD5_STEP(MD5_F2, aa, bb, cc, dd, xx[13] + 0xA9E3E905UL,  5);
    MD5_STEP(MD5_F2, dd, aa, bb, cc, xx[ 2] + 0xFCEFA3F8UL,  9);
    MD5_STEP(MD5_F2, cc, dd, aa, bb, xx[ 7] + 0x676F02D9UL, 14);
    MD5_STEP(MD5_F2, bb, cc, dd, aa, xx[12] + 0x8D2A4C8AUL, 20);

    MD5_STEP(MD5_F3, aa, bb, cc, dd, xx[ 5] + 0xFFFA3942UL,  4);
    MD5_STEP(MD5_F3, dd, aa, bb, cc, xx[ 8] + 0x8771F681UL, 11);
    MD5_STEP(MD5_F3, cc, dd, aa, bb, xx[11] + 0x6D9D6122UL, 16);
    MD5_STEP(MD5_F3, bb, cc, dd, aa, xx[14] + 0xFDE5380CUL, 23);
    MD5_STEP(MD5_F3, aa, bb, cc, dd, xx[ 1] + 0xA4BEEA44UL,  4);
    MD5_STEP(MD5_F3, dd, aa, bb, cc, xx[ 4] + 0x4BDECFA9UL, 11);
    MD5_STEP(MD5_F3, cc, dd, aa, bb, xx[ 7] + 0xF6BB4B60UL, 16);
    MD5_STEP(MD5_F3, bb, cc, dd, aa, xx[10] + 0xBEBFBC70UL, 23);
    MD5_STEP(MD5_F3, aa, bb, cc, dd, xx[13] + 0x289B7EC6UL,  4);
    MD5_STEP(MD5_F3, dd, aa, bb, cc, xx[ 0] + 0xEAA127FAUL, 11);
    MD5_STEP(MD5_F3, cc, dd, aa, bb, xx[ 3] + 0xD4EF3085UL, 16);
    MD5_STEP(MD5_F3, bb, cc, dd, aa, xx[ 6] + 0x04881D05UL, 23);
    MD5_STEP(MD5_F3, aa, bb, cc, dd, xx[ 9] + 0xD9D4D039UL,  4);
    MD5_STEP(MD5_F3, dd, aa, bb, cc, xx[12] + 0xE6DB99E5UL, 11);
    MD5_STEP(MD5_F3, cc, dd, aa, bb, xx[15] + 0x1FA27CF8UL, 16);
    MD5_STEP(MD5_F3, bb, cc, dd, aa, xx[ 2] + 0xC4AC5665UL, 23);

    MD5_STEP(MD5_F4, aa, bb, cc, dd, xx[ 0] + 0xF4292244UL,  6);
    MD5_STEP(MD5_F4, dd, aa, bb, cc, xx[ 7] + 0x432AFF97UL, 10);
    MD5_STEP(MD5_F4, cc, dd, aa, bb, xx[14] + 0xAB9423A7UL, 15);
    MD5_STEP(MD5_F4, bb, cc, dd, aa, xx[ 5] + 0xFC93A039UL, 21);
    MD5_STEP(MD5_F4, aa, bb, cc, dd, xx[12] + 0x655B59C3UL,  6);
    MD5_STEP(MD5_F4, dd, aa, bb, cc, xx[ 3] + 0x8F0CCC92UL, 10);
    MD5_STEP(MD5_F4, cc, dd, aa, bb, xx[10] + 0xFFEFF47DUL, 15);
    MD5_STEP(MD5_F4, bb, cc, dd, aa, xx[ 1] + 0x85845DD1UL, 21);
    MD5_STEP(MD5_F4, aa, bb, cc, dd, xx[ 8] + 0x6FA87E4FUL,  6);
    MD5_STEP(MD5_F4, dd, aa, bb, cc, xx[15] + 0xFE2CE6E0UL, 10);
    MD5_STEP(MD5_F4, cc, dd, aa, bb, xx[ 6] + 0xA3014314UL, 15);
    MD5_STEP(MD5_F4, bb, cc, dd, aa, xx[13] + 0x4E0811A1UL, 21);
    MD5_STEP(MD5_F4, aa, bb, cc, dd, xx[ 4] + 0xF7537E82UL,  6);
    MD5_STEP(MD5_F4, dd, aa, bb, cc, xx[11] + 0xBD3AF235UL, 10);
    MD5_STEP(MD5_F4, cc, dd, aa, bb, xx[ 2] + 0x2AD7D2BBUL, 15);
    MD5_STEP(MD5_F4, bb, cc, dd, aa, xx[ 9] + 0xEB86D391UL, 21);

    state[0] += aa;
    state[1] += bb;
    state[2] += cc;
    state[3] += dd;
}

/*
=======================================
    MD5 初始化
=======================================
*/
CR_API void_t
hash_md5_init (
  __CR_OT__ sMD5*   ctx
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
    MD5 计算数据块
=======================================
*/
CR_API void_t
hash_md5_update (
  __CR_IO__ sMD5*           ctx,
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
        md5_transform(ctx->state, (int32u*)ctx->buffer);
        size -= fill;
        while (size >= 64) {
            mem_cpy(ctx->buffer, data, 64);
            md5_transform(ctx->state, (int32u*)ctx->buffer);
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
    MD5 获取结果
=======================================
*/
CR_API void_t
hash_md5_finish (
  __CR_OT__ byte_t  hash[16],
  __CR_IO__ sMD5*   ctx
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
    hash_md5_update(ctx, s_padding, len);
    hash_md5_update(ctx, bits, 8);

    for (idx = 0; idx < 4; idx++)
        ctx->state[idx] = DWORD_LE(ctx->state[idx]);
    mem_cpy(hash, ctx->state, 16);
}

/*
=======================================
    MD5 一次性完成
=======================================
*/
CR_API void_t
hash_md5_total (
  __CR_OT__ byte_t          hash[16],
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    sMD5    ctx;

    hash_md5_init(&ctx);
    hash_md5_update(&ctx, data, size);
    hash_md5_finish(hash, &ctx);
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
