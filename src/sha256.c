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
/*  >>>>>>>>>>>>>>>>>>>> CrHack SHA-224/256 哈希函数库 <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "hash.h"

#if defined(_CR_OS_MACOSX_)
/*
=======================================
    SHA-224 初始化
=======================================
*/
CR_API void_t
hash_sha224_init (
  __CR_OT__ sSHA224*    ctx
    )
{
    CC_SHA224_Init(ctx);
}

/*
=======================================
    SHA-224 计算数据块
=======================================
*/
CR_API void_t
hash_sha224_update (
  __CR_IO__ sSHA224*        ctx,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    CC_SHA224_Update(ctx, data, (CC_LONG)size);
}

/*
=======================================
    SHA-224 获取结果
=======================================
*/
CR_API void_t
hash_sha224_finish (
  __CR_OT__ byte_t      hash[28],
  __CR_IO__ sSHA224*    ctx
    )
{
    CC_SHA224_Final(hash, ctx);
}

/*
=======================================
    SHA-224 一次性完成
=======================================
*/
CR_API void_t
hash_sha224_total (
  __CR_OT__ byte_t          hash[28],
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    CC_SHA224(data, (CC_LONG)size, hash);
}

/*
=======================================
    SHA-256 初始化
=======================================
*/
CR_API void_t
hash_sha256_init (
  __CR_OT__ sSHA256*    ctx
    )
{
    CC_SHA256_Init(ctx);
}

/*
=======================================
    SHA-256 计算数据块
=======================================
*/
CR_API void_t
hash_sha256_update (
  __CR_IO__ sSHA256*        ctx,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    CC_SHA256_Update(ctx, data, (CC_LONG)size);
}

/*
=======================================
    SHA-256 获取结果
=======================================
*/
CR_API void_t
hash_sha256_finish (
  __CR_OT__ byte_t      hash[32],
  __CR_IO__ sSHA256*    ctx
    )
{
    CC_SHA256_Final(hash, ctx);
}

/*
=======================================
    SHA-256 一次性完成
=======================================
*/
CR_API void_t
hash_sha256_total (
  __CR_OT__ byte_t          hash[32],
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    CC_SHA256(data, (CC_LONG)size, hash);
}

#else

#include "morder.h"
#include "memlib.h"

/*****************************************************************************/
/*                                 SHA-256                                   */
/*****************************************************************************/

/* SHA-256 计算宏 */
#define SHA2_CH(x, y, z)    ((z) ^ ((x) & ((y) ^ (z))))
#define SHA2_MA(x, y, z)    (((x) & (y)) | ((z) & ((x) | (y))))

#define SHA2_E0(x)  (rotr_int32u(x, 2) ^ rotr_int32u(x, 13) ^ \
                     rotr_int32u(x, 22))
#define SHA2_E1(x)  (rotr_int32u(x, 6) ^ rotr_int32u(x, 11) ^ \
                     rotr_int32u(x, 25))
#define SHA2_S0(x)  (rotr_int32u(x,  7) ^ rotr_int32u(x, 18) ^ ((x) >>  3))
#define SHA2_S1(x)  (rotr_int32u(x, 17) ^ rotr_int32u(x, 19) ^ ((x) >> 10))

#define SHA2_BLEND(i)   xx[i] = SHA2_S1(xx[(i) -  2]) + xx[(i) -  7] + \
                                SHA2_S0(xx[(i) - 15]) + xx[(i) - 16]
/* SHA-256 填充数组 */
static const byte_t _rom_ s_padding[64] =
{
    0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

/*
---------------------------------------
    SHA-256 变换计算
---------------------------------------
*/
static void_t
sha256_transform (
  __CR_IO__ int32u          state[8],
  __CR_IN__ const int32u    block[16]
    )
{
    uint_t  idx;
    int32u  aa = state[0];
    int32u  bb = state[1];
    int32u  cc = state[2];
    int32u  dd = state[3];
    int32u  ee = state[4];
    int32u  ff = state[5];
    int32u  gg = state[6];
    int32u  hh = state[7];
    int32u  t1, t2, xx[64];

    for (idx =  0; idx < 16; idx++)
        xx[idx] = DWORD_BE(block[idx]);
    for (idx = 16; idx < 64; idx++)
        SHA2_BLEND(idx);

    t1 = hh + SHA2_E1(ee) + SHA2_CH(ee, ff, gg) + 0x428A2F98UL + xx[ 0];
    t2 = SHA2_E0(aa) + SHA2_MA(aa, bb, cc); dd += t1; hh = t1 + t2;
    t1 = gg + SHA2_E1(dd) + SHA2_CH(dd, ee, ff) + 0x71374491UL + xx[ 1];
    t2 = SHA2_E0(hh) + SHA2_MA(hh, aa, bb); cc += t1; gg = t1 + t2;
    t1 = ff + SHA2_E1(cc) + SHA2_CH(cc, dd, ee) + 0xB5C0FBCFUL + xx[ 2];
    t2 = SHA2_E0(gg) + SHA2_MA(gg, hh, aa); bb += t1; ff = t1 + t2;
    t1 = ee + SHA2_E1(bb) + SHA2_CH(bb, cc, dd) + 0xE9B5DBA5UL + xx[ 3];
    t2 = SHA2_E0(ff) + SHA2_MA(ff, gg, hh); aa += t1; ee = t1 + t2;
    t1 = dd + SHA2_E1(aa) + SHA2_CH(aa, bb, cc) + 0x3956C25BUL + xx[ 4];
    t2 = SHA2_E0(ee) + SHA2_MA(ee, ff, gg); hh += t1; dd = t1 + t2;
    t1 = cc + SHA2_E1(hh) + SHA2_CH(hh, aa, bb) + 0x59F111F1UL + xx[ 5];
    t2 = SHA2_E0(dd) + SHA2_MA(dd, ee, ff); gg += t1; cc = t1 + t2;
    t1 = bb + SHA2_E1(gg) + SHA2_CH(gg, hh, aa) + 0x923F82A4UL + xx[ 6];
    t2 = SHA2_E0(cc) + SHA2_MA(cc, dd, ee); ff += t1; bb = t1 + t2;
    t1 = aa + SHA2_E1(ff) + SHA2_CH(ff, gg, hh) + 0xAB1C5ED5UL + xx[ 7];
    t2 = SHA2_E0(bb) + SHA2_MA(bb, cc, dd); ee += t1; aa = t1 + t2;

    t1 = hh + SHA2_E1(ee) + SHA2_CH(ee, ff, gg) + 0xD807AA98UL + xx[ 8];
    t2 = SHA2_E0(aa) + SHA2_MA(aa, bb, cc); dd += t1; hh = t1 + t2;
    t1 = gg + SHA2_E1(dd) + SHA2_CH(dd, ee, ff) + 0x12835B01UL + xx[ 9];
    t2 = SHA2_E0(hh) + SHA2_MA(hh, aa, bb); cc += t1; gg = t1 + t2;
    t1 = ff + SHA2_E1(cc) + SHA2_CH(cc, dd, ee) + 0x243185BEUL + xx[10];
    t2 = SHA2_E0(gg) + SHA2_MA(gg, hh, aa); bb += t1; ff = t1 + t2;
    t1 = ee + SHA2_E1(bb) + SHA2_CH(bb, cc, dd) + 0x550C7DC3UL + xx[11];
    t2 = SHA2_E0(ff) + SHA2_MA(ff, gg, hh); aa += t1; ee = t1 + t2;
    t1 = dd + SHA2_E1(aa) + SHA2_CH(aa, bb, cc) + 0x72BE5D74UL + xx[12];
    t2 = SHA2_E0(ee) + SHA2_MA(ee, ff, gg); hh += t1; dd = t1 + t2;
    t1 = cc + SHA2_E1(hh) + SHA2_CH(hh, aa, bb) + 0x80DEB1FEUL + xx[13];
    t2 = SHA2_E0(dd) + SHA2_MA(dd, ee, ff); gg += t1; cc = t1 + t2;
    t1 = bb + SHA2_E1(gg) + SHA2_CH(gg, hh, aa) + 0x9BDC06A7UL + xx[14];
    t2 = SHA2_E0(cc) + SHA2_MA(cc, dd, ee); ff += t1; bb = t1 + t2;
    t1 = aa + SHA2_E1(ff) + SHA2_CH(ff, gg, hh) + 0xC19BF174UL + xx[15];
    t2 = SHA2_E0(bb) + SHA2_MA(bb, cc, dd); ee += t1; aa = t1 + t2;

    t1 = hh + SHA2_E1(ee) + SHA2_CH(ee, ff, gg) + 0xE49B69C1UL + xx[16];
    t2 = SHA2_E0(aa) + SHA2_MA(aa, bb, cc); dd += t1; hh = t1 + t2;
    t1 = gg + SHA2_E1(dd) + SHA2_CH(dd, ee, ff) + 0xEFBE4786UL + xx[17];
    t2 = SHA2_E0(hh) + SHA2_MA(hh, aa, bb); cc += t1; gg = t1 + t2;
    t1 = ff + SHA2_E1(cc) + SHA2_CH(cc, dd, ee) + 0x0FC19DC6UL + xx[18];
    t2 = SHA2_E0(gg) + SHA2_MA(gg, hh, aa); bb += t1; ff = t1 + t2;
    t1 = ee + SHA2_E1(bb) + SHA2_CH(bb, cc, dd) + 0x240CA1CCUL + xx[19];
    t2 = SHA2_E0(ff) + SHA2_MA(ff, gg, hh); aa += t1; ee = t1 + t2;
    t1 = dd + SHA2_E1(aa) + SHA2_CH(aa, bb, cc) + 0x2DE92C6FUL + xx[20];
    t2 = SHA2_E0(ee) + SHA2_MA(ee, ff, gg); hh += t1; dd = t1 + t2;
    t1 = cc + SHA2_E1(hh) + SHA2_CH(hh, aa, bb) + 0x4A7484AAUL + xx[21];
    t2 = SHA2_E0(dd) + SHA2_MA(dd, ee, ff); gg += t1; cc = t1 + t2;
    t1 = bb + SHA2_E1(gg) + SHA2_CH(gg, hh, aa) + 0x5CB0A9DCUL + xx[22];
    t2 = SHA2_E0(cc) + SHA2_MA(cc, dd, ee); ff += t1; bb = t1 + t2;
    t1 = aa + SHA2_E1(ff) + SHA2_CH(ff, gg, hh) + 0x76F988DAUL + xx[23];
    t2 = SHA2_E0(bb) + SHA2_MA(bb, cc, dd); ee += t1; aa = t1 + t2;

    t1 = hh + SHA2_E1(ee) + SHA2_CH(ee, ff, gg) + 0x983E5152UL + xx[24];
    t2 = SHA2_E0(aa) + SHA2_MA(aa, bb, cc); dd += t1; hh = t1 + t2;
    t1 = gg + SHA2_E1(dd) + SHA2_CH(dd, ee, ff) + 0xA831C66DUL + xx[25];
    t2 = SHA2_E0(hh) + SHA2_MA(hh, aa, bb); cc += t1; gg = t1 + t2;
    t1 = ff + SHA2_E1(cc) + SHA2_CH(cc, dd, ee) + 0xB00327C8UL + xx[26];
    t2 = SHA2_E0(gg) + SHA2_MA(gg, hh, aa); bb += t1; ff = t1 + t2;
    t1 = ee + SHA2_E1(bb) + SHA2_CH(bb, cc, dd) + 0xBF597FC7UL + xx[27];
    t2 = SHA2_E0(ff) + SHA2_MA(ff, gg, hh); aa += t1; ee = t1 + t2;
    t1 = dd + SHA2_E1(aa) + SHA2_CH(aa, bb, cc) + 0xC6E00BF3UL + xx[28];
    t2 = SHA2_E0(ee) + SHA2_MA(ee, ff, gg); hh += t1; dd = t1 + t2;
    t1 = cc + SHA2_E1(hh) + SHA2_CH(hh, aa, bb) + 0xD5A79147UL + xx[29];
    t2 = SHA2_E0(dd) + SHA2_MA(dd, ee, ff); gg += t1; cc = t1 + t2;
    t1 = bb + SHA2_E1(gg) + SHA2_CH(gg, hh, aa) + 0x06CA6351UL + xx[30];
    t2 = SHA2_E0(cc) + SHA2_MA(cc, dd, ee); ff += t1; bb = t1 + t2;
    t1 = aa + SHA2_E1(ff) + SHA2_CH(ff, gg, hh) + 0x14292967UL + xx[31];
    t2 = SHA2_E0(bb) + SHA2_MA(bb, cc, dd); ee += t1; aa = t1 + t2;

    t1 = hh + SHA2_E1(ee) + SHA2_CH(ee, ff, gg) + 0x27B70A85UL + xx[32];
    t2 = SHA2_E0(aa) + SHA2_MA(aa, bb, cc); dd += t1; hh = t1 + t2;
    t1 = gg + SHA2_E1(dd) + SHA2_CH(dd, ee, ff) + 0x2E1B2138UL + xx[33];
    t2 = SHA2_E0(hh) + SHA2_MA(hh, aa, bb); cc += t1; gg = t1 + t2;
    t1 = ff + SHA2_E1(cc) + SHA2_CH(cc, dd, ee) + 0x4D2C6DFCUL + xx[34];
    t2 = SHA2_E0(gg) + SHA2_MA(gg, hh, aa); bb += t1; ff = t1 + t2;
    t1 = ee + SHA2_E1(bb) + SHA2_CH(bb, cc, dd) + 0x53380D13UL + xx[35];
    t2 = SHA2_E0(ff) + SHA2_MA(ff, gg, hh); aa += t1; ee = t1 + t2;
    t1 = dd + SHA2_E1(aa) + SHA2_CH(aa, bb, cc) + 0x650A7354UL + xx[36];
    t2 = SHA2_E0(ee) + SHA2_MA(ee, ff, gg); hh += t1; dd = t1 + t2;
    t1 = cc + SHA2_E1(hh) + SHA2_CH(hh, aa, bb) + 0x766A0ABBUL + xx[37];
    t2 = SHA2_E0(dd) + SHA2_MA(dd, ee, ff); gg += t1; cc = t1 + t2;
    t1 = bb + SHA2_E1(gg) + SHA2_CH(gg, hh, aa) + 0x81C2C92EUL + xx[38];
    t2 = SHA2_E0(cc) + SHA2_MA(cc, dd, ee); ff += t1; bb = t1 + t2;
    t1 = aa + SHA2_E1(ff) + SHA2_CH(ff, gg, hh) + 0x92722C85UL + xx[39];
    t2 = SHA2_E0(bb) + SHA2_MA(bb, cc, dd); ee += t1; aa = t1 + t2;

    t1 = hh + SHA2_E1(ee) + SHA2_CH(ee, ff, gg) + 0xA2BFE8A1UL + xx[40];
    t2 = SHA2_E0(aa) + SHA2_MA(aa, bb, cc); dd += t1; hh = t1 + t2;
    t1 = gg + SHA2_E1(dd) + SHA2_CH(dd, ee, ff) + 0xA81A664BUL + xx[41];
    t2 = SHA2_E0(hh) + SHA2_MA(hh, aa, bb); cc += t1; gg = t1 + t2;
    t1 = ff + SHA2_E1(cc) + SHA2_CH(cc, dd, ee) + 0xC24B8B70UL + xx[42];
    t2 = SHA2_E0(gg) + SHA2_MA(gg, hh, aa); bb += t1; ff = t1 + t2;
    t1 = ee + SHA2_E1(bb) + SHA2_CH(bb, cc, dd) + 0xC76C51A3UL + xx[43];
    t2 = SHA2_E0(ff) + SHA2_MA(ff, gg, hh); aa += t1; ee = t1 + t2;
    t1 = dd + SHA2_E1(aa) + SHA2_CH(aa, bb, cc) + 0xD192E819UL + xx[44];
    t2 = SHA2_E0(ee) + SHA2_MA(ee, ff, gg); hh += t1; dd = t1 + t2;
    t1 = cc + SHA2_E1(hh) + SHA2_CH(hh, aa, bb) + 0xD6990624UL + xx[45];
    t2 = SHA2_E0(dd) + SHA2_MA(dd, ee, ff); gg += t1; cc = t1 + t2;
    t1 = bb + SHA2_E1(gg) + SHA2_CH(gg, hh, aa) + 0xF40E3585UL + xx[46];
    t2 = SHA2_E0(cc) + SHA2_MA(cc, dd, ee); ff += t1; bb = t1 + t2;
    t1 = aa + SHA2_E1(ff) + SHA2_CH(ff, gg, hh) + 0x106AA070UL + xx[47];
    t2 = SHA2_E0(bb) + SHA2_MA(bb, cc, dd); ee += t1; aa = t1 + t2;

    t1 = hh + SHA2_E1(ee) + SHA2_CH(ee, ff, gg) + 0x19A4C116UL + xx[48];
    t2 = SHA2_E0(aa) + SHA2_MA(aa, bb, cc); dd += t1; hh = t1 + t2;
    t1 = gg + SHA2_E1(dd) + SHA2_CH(dd, ee, ff) + 0x1E376C08UL + xx[49];
    t2 = SHA2_E0(hh) + SHA2_MA(hh, aa, bb); cc += t1; gg = t1 + t2;
    t1 = ff + SHA2_E1(cc) + SHA2_CH(cc, dd, ee) + 0x2748774CUL + xx[50];
    t2 = SHA2_E0(gg) + SHA2_MA(gg, hh, aa); bb += t1; ff = t1 + t2;
    t1 = ee + SHA2_E1(bb) + SHA2_CH(bb, cc, dd) + 0x34B0BCB5UL + xx[51];
    t2 = SHA2_E0(ff) + SHA2_MA(ff, gg, hh); aa += t1; ee = t1 + t2;
    t1 = dd + SHA2_E1(aa) + SHA2_CH(aa, bb, cc) + 0x391C0CB3UL + xx[52];
    t2 = SHA2_E0(ee) + SHA2_MA(ee, ff, gg); hh += t1; dd = t1 + t2;
    t1 = cc + SHA2_E1(hh) + SHA2_CH(hh, aa, bb) + 0x4ED8AA4AUL + xx[53];
    t2 = SHA2_E0(dd) + SHA2_MA(dd, ee, ff); gg += t1; cc = t1 + t2;
    t1 = bb + SHA2_E1(gg) + SHA2_CH(gg, hh, aa) + 0x5B9CCA4FUL + xx[54];
    t2 = SHA2_E0(cc) + SHA2_MA(cc, dd, ee); ff += t1; bb = t1 + t2;
    t1 = aa + SHA2_E1(ff) + SHA2_CH(ff, gg, hh) + 0x682E6FF3UL + xx[55];
    t2 = SHA2_E0(bb) + SHA2_MA(bb, cc, dd); ee += t1; aa = t1 + t2;

    t1 = hh + SHA2_E1(ee) + SHA2_CH(ee, ff, gg) + 0x748F82EEUL + xx[56];
    t2 = SHA2_E0(aa) + SHA2_MA(aa, bb, cc); dd += t1; hh = t1 + t2;
    t1 = gg + SHA2_E1(dd) + SHA2_CH(dd, ee, ff) + 0x78A5636FUL + xx[57];
    t2 = SHA2_E0(hh) + SHA2_MA(hh, aa, bb); cc += t1; gg = t1 + t2;
    t1 = ff + SHA2_E1(cc) + SHA2_CH(cc, dd, ee) + 0x84C87814UL + xx[58];
    t2 = SHA2_E0(gg) + SHA2_MA(gg, hh, aa); bb += t1; ff = t1 + t2;
    t1 = ee + SHA2_E1(bb) + SHA2_CH(bb, cc, dd) + 0x8CC70208UL + xx[59];
    t2 = SHA2_E0(ff) + SHA2_MA(ff, gg, hh); aa += t1; ee = t1 + t2;
    t1 = dd + SHA2_E1(aa) + SHA2_CH(aa, bb, cc) + 0x90BEFFFAUL + xx[60];
    t2 = SHA2_E0(ee) + SHA2_MA(ee, ff, gg); hh += t1; dd = t1 + t2;
    t1 = cc + SHA2_E1(hh) + SHA2_CH(hh, aa, bb) + 0xA4506CEBUL + xx[61];
    t2 = SHA2_E0(dd) + SHA2_MA(dd, ee, ff); gg += t1; cc = t1 + t2;
    t1 = bb + SHA2_E1(gg) + SHA2_CH(gg, hh, aa) + 0xBEF9A3F7UL + xx[62];
    t2 = SHA2_E0(cc) + SHA2_MA(cc, dd, ee); ff += t1; bb = t1 + t2;
    t1 = aa + SHA2_E1(ff) + SHA2_CH(ff, gg, hh) + 0xC67178F2UL + xx[63];
    t2 = SHA2_E0(bb) + SHA2_MA(bb, cc, dd); ee += t1; aa = t1 + t2;

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
    SHA-256 初始化
=======================================
*/
CR_API void_t
hash_sha256_init (
  __CR_OT__ sSHA256*    ctx
    )
{
    ctx->count[0] = 0UL;
    ctx->count[1] = 0UL;
    ctx->state[0] = 0x6A09E667UL;
    ctx->state[1] = 0xBB67AE85UL;
    ctx->state[2] = 0x3C6EF372UL;
    ctx->state[3] = 0xA54FF53AUL;
    ctx->state[4] = 0x510E527FUL;
    ctx->state[5] = 0x9B05688CUL;
    ctx->state[6] = 0x1F83D9ABUL;
    ctx->state[7] = 0x5BE0CD19UL;
}

/*
=======================================
    SHA-256 计算数据块
=======================================
*/
CR_API void_t
hash_sha256_update (
  __CR_IO__ sSHA256*        ctx,
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
        sha256_transform(ctx->state, (int32u*)ctx->buffer);
        size -= fill;
        while (size >= 64) {
            mem_cpy(ctx->buffer, data, 64);
            sha256_transform(ctx->state, (int32u*)ctx->buffer);
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
    SHA-256 获取结果
=======================================
*/
CR_API void_t
hash_sha256_finish (
  __CR_OT__ byte_t      hash[32],
  __CR_IO__ sSHA256*    ctx
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
    hash_sha256_update(ctx, s_padding, len);
    hash_sha256_update(ctx, bits, 8);

    for (idx = 0; idx < 8; idx++)
        ctx->state[idx] = DWORD_BE(ctx->state[idx]);
    mem_cpy(hash, ctx->state, 32);
}

/*
=======================================
    SHA-256 一次性完成
=======================================
*/
CR_API void_t
hash_sha256_total (
  __CR_OT__ byte_t          hash[32],
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    sSHA256     ctx;

    hash_sha256_init(&ctx);
    hash_sha256_update(&ctx, data, size);
    hash_sha256_finish(hash, &ctx);
}

/*****************************************************************************/
/*                                 SHA-224                                   */
/*****************************************************************************/

/*
=======================================
    SHA-224 初始化
=======================================
*/
CR_API void_t
hash_sha224_init (
  __CR_OT__ sSHA224*    ctx
    )
{
    ctx->count[0] = 0UL;
    ctx->count[1] = 0UL;
    ctx->state[0] = 0xC1059ED8UL;
    ctx->state[1] = 0x367CD507UL;
    ctx->state[2] = 0x3070DD17UL;
    ctx->state[3] = 0xF70E5939UL;
    ctx->state[4] = 0xFFC00B31UL;
    ctx->state[5] = 0x68581511UL;
    ctx->state[6] = 0x64F98FA7UL;
    ctx->state[7] = 0xBEFA4FA4UL;
}

/*
=======================================
    SHA-224 计算数据块
=======================================
*/
CR_API void_t
hash_sha224_update (
  __CR_IO__ sSHA224*        ctx,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    hash_sha256_update(ctx, data, size);
}

/*
=======================================
    SHA-224 获取结果
=======================================
*/
CR_API void_t
hash_sha224_finish (
  __CR_OT__ byte_t      hash[28],
  __CR_IO__ sSHA224*    ctx
    )
{
    byte_t  tmp[32];

    hash_sha256_finish(tmp, ctx);
    mem_cpy(hash, tmp, 28);
}

/*
=======================================
    SHA-224 一次性完成
=======================================
*/
CR_API void_t
hash_sha224_total (
  __CR_OT__ byte_t          hash[28],
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    sSHA224     ctx;

    hash_sha224_init(&ctx);
    hash_sha224_update(&ctx, data, size);
    hash_sha224_finish(hash, &ctx);
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
