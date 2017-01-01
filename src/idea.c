/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-10-12  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack IDEA 加密解密函数库 <<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "crypto.h"
#include "morder.h"

/* IDEA 加法反转 */
#define addinv(x)   (-x & 0xFFFF)

/* IDEA 乘法取模 */
#define mulmod(a, b) \
            if (a == 0) { \
                a = 0x10001 - b; \
            } \
            else \
            if (b == 0) { \
                a = 0x10001 - a; \
            } \
            else { \
                a *= b; \
                if ((a & 0xFFFF) >= (b = a >> 16)) \
                    a -= b; \
                else \
                    a += 0x10001 - b; \
            }
/*
---------------------------------------
    IDEA 乘法反转
---------------------------------------
*/
static int16u
mulinv (
  __CR_IN__ int16u  x
    )
{
    int32s  n1, n2, nn;
    int32s  a1, a2, b1, b2;

    if (x <= 1)
        return (x);
    n1 = nn = (int32s)x;
    n2 = 0x10001;
    a1 = b2 = 1;
    a2 = b1 = 0;
    do
    {
        while ((n1 & 1) == 0)
        {
            if (a1 & 1) {
                if (a1 < 0) {
                    a1 += 0x10001;
                    b1 -= nn;
                }
                else {
                    a1 -= 0x10001;
                    b1 += nn;
                }
            }
            n1 >>= 1;
            a1 >>= 1;
            b1 >>= 1;
        }

        if (n1 < n2)
        {
            do
            {
                n2 -= n1;
                a2 -= a1;
                b2 -= b1;
                if (n2 == 0)
                    return ((int16u)((a1 < 0) ? a1 + 0x10001 : a1));

                while ((n2 & 1) == 0)
                {
                    if (a2 & 1) {
                        if (a2 < 0) {
                            a2 += 0x10001;
                            b2 -= nn;
                        }
                        else {
                            a2 -= 0x10001;
                            b2 += nn;
                        }
                    }
                    n2 >>= 1;
                    a2 >>= 1;
                    b2 >>= 1;
                }
            } while (n1 <= n2);
        }
        n1 -= n2;
        a1 -= a2;
        b1 -= b2;
    } while (n1 != 0);

    return ((int16u)((a2 < 0) ? a2 + 0x10001 : a2));
}

/*
---------------------------------------
    IDEA 扩展用户密钥
---------------------------------------
*/
static void_t
key_expand (
  __CR_IN__ const int16u    ukey[8],
  __CR_OT__ int16u          skey[52]
    )
{
    uint_t  idx;

    for (idx = 0; idx <  8; idx++)
        skey[idx] = WORD_LE(ukey[idx]);
    for (idx = 8; idx < 52; idx++) {
        if ((idx & 7) <  6)
            skey[idx] = ((skey[idx- 7] & 127) << 9) | (skey[idx- 6] >> 7);
        else
        if ((idx & 7) == 6)
            skey[idx] = ((skey[idx- 7] & 127) << 9) | (skey[idx-14] >> 7);
        else
            skey[idx] = ((skey[idx-15] & 127) << 9) | (skey[idx-14] >> 7);
    }
}

/*
---------------------------------------
    IDEA 反转加解密密钥
---------------------------------------
*/
static void_t
key_invert (
  __CR_IN__ const int16u    in[52],
  __CR_OT__ int16u          out[52]
    )
{
    int16u  tt;
    uint_t  idx = (8 - 1) / 2;
    uint_t  lo = 0, hi = 6 * 8;

    tt = mulinv(in[lo]);
    out[lo++] = mulinv(in[hi]);
    out[hi++] = tt;
    tt = addinv(in[lo]);
    out[lo++] = addinv(in[hi]);
    out[hi++] = tt;
    tt = addinv(in[lo]);
    out[lo++] = addinv(in[hi]);
    out[hi++] = tt;
    tt = mulinv(in[lo]);
    out[lo++] = mulinv(in[hi]);
    out[hi+0] = tt;
    for (; idx != 0 ; idx--) {
        tt = in[lo];
        out[lo++] = in[hi -= 5];
        out[hi++] = tt;
        tt = in[lo];
        out[lo++] = in[hi];
        out[hi+0] = tt;
        tt = mulinv(in[lo]);
        out[lo++] = mulinv(in[hi -= 5]);
        out[hi++] = tt;
        tt = addinv(in[lo]);
        out[lo++] = addinv(in[++hi]);
        out[hi--] = tt;
        tt = addinv(in[lo]);
        out[lo++] = addinv(in[hi]);
        out[hi++] = tt;
        tt = mulinv(in[lo]);
        out[lo++] = mulinv(in[++hi]);
        out[hi+0] = tt;
    }
    tt = in[lo];
    out[lo++] = in[hi -= 5];
    out[hi++] = tt;
    tt = in[lo];
    out[lo++] = in[hi];
    out[hi+0] = tt;
    out[lo] = mulinv(in[lo]);
    lo += 1;
    tt = addinv(in[lo]);
    out[lo] = addinv(in[lo + 1]);
    lo += 1;
    out[lo++] = tt;
    out[lo] = mulinv(in[lo]);
}

/*
---------------------------------------
    IDEA 加密解密
---------------------------------------
*/
static void_t
idea_crypto (
  __CR_IN__ const int16u    in[4],
  __CR_OT__ int16u          out[4],
  __CR_IN__ const int16u    key[52]
    )
{
    int32u  t0, t1, t2;
    int32u  x0, x1, x2, x3;
    uint_t  rnd = 8, idx = 0;

    x0 = (int32u)WORD_LE(in[0]);
    x1 = (int32u)WORD_LE(in[1]);
    x2 = (int32u)WORD_LE(in[2]);
    x3 = (int32u)WORD_LE(in[3]);
    for (; rnd != 0; rnd--)
    {
        t1  = key[idx++];
        x1 += key[idx++];
        x2 += key[idx++];
        x2 &= 0xFFFF;
        t2  = key[idx++];
        mulmod(x0, t1);
        x0 &= 0xFFFF;
        mulmod(x3, t2);
        t0  = key[idx++];
        t1  = x0 ^ x2;
        mulmod(t0, t1);
        t0 &= 0xFFFF;
        t1  = key[idx++];
        t2  = (x1 ^ x3) + t0;
        t2 &= 0xFFFF;
        mulmod(t1, t2);
        t1 &= 0xFFFF;
        t0 += t1;
        x0 ^= t1;
        x3 ^= t0;
        x3 &= 0xFFFF;
        t0 ^= x1;
        x1  = x2 ^ t1;
        x2  = t0;
    }
    t0 = key[idx++];
    mulmod(x0, t0);
    out[0] = WORD_LE((int16u)(x0));
    out[1] = WORD_LE((int16u)(key[idx++] + x2));
    out[2] = WORD_LE((int16u)(key[idx++] + x1));
    t0 = key[idx];
    mulmod(x3, t0);
    out[3] = WORD_LE((int16u)(x3));
}

/*
=======================================
    IDEA 加密
=======================================
*/
CR_API void_t
crypto_idea_enc (
  __CR_OT__ int16u          dst[4],
  __CR_IN__ const int16u    src[4],
  __CR_IN__ const int16u    key[8]
    )
{
    int16u  skey[52];

    key_expand(key, skey);
    idea_crypto(src, dst, skey);
}

/*
=======================================
    IDEA 解密
=======================================
*/
CR_API void_t
crypto_idea_dec (
  __CR_OT__ int16u          dst[4],
  __CR_IN__ const int16u    src[4],
  __CR_IN__ const int16u    key[8]
    )
{
    int16u  skey[52];
    int16u  ikey[52];

    key_expand(key, skey);
    key_invert(skey, ikey);
    idea_crypto(src, dst, ikey);
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
