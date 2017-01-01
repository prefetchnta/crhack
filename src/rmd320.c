/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2014-04-24  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack RMD-320 哈希函数库 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "hash.h"
#include "morder.h"
#include "memlib.h"

/* RMD-320 计算宏 */
#define RMD320_K1   0x00000000UL
#define RMD320_K2   0x5A827999UL
#define RMD320_K3   0x6ED9EBA1UL
#define RMD320_K4   0x8F1BBCDCUL
#define RMD320_K5   0xA953FD4EUL
#define RMD320_KK1  0x50A28BE6UL
#define RMD320_KK2  0x5C4DD124UL
#define RMD320_KK3  0x6D703EF3UL
#define RMD320_KK4  0x7A6D76E9UL
#define RMD320_KK5  0x00000000UL

#define RMD320_F1(x, y, z)  ((x) ^ (y) ^ (z))
#define RMD320_F2(x, y, z)  ((z) ^ ((x) & ((y) ^ (z))))
#define RMD320_F3(x, y, z)  (((x) | (~(y))) ^ (z))
#define RMD320_F4(x, y, z)  ((y) ^ ((z) & ((x) ^ (y))))
#define RMD320_F5(x, y, z)  ((x) ^ ((y) | (~(z))))

#define RMD320_R(a, b, c, d, e, f, k, x, s) \
                    { (a) += f(b, c, d) + (x) + (k); \
                      (a) = rotl_int32u(a, s) + (e); \
                      (c) = rotl_int32u(c, 10); }
/* RMD-320 填充数组 */
static const byte_t _rom_ s_padding[64] =
{
    0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

/*
---------------------------------------
    RMD-320 变换计算
---------------------------------------
*/
static void_t
rmd320_transform (
  __CR_IO__ int32u          state[10],
  __CR_IN__ const int32u    block[16]
    )
{
    uint_t  idx;
    int32u  xx[16], tmp;
    int32u  aa  = state[0];
    int32u  bb  = state[1];
    int32u  cc  = state[2];
    int32u  dd  = state[3];
    int32u  ee  = state[4];
    int32u  aaa = state[5];
    int32u  bbb = state[6];
    int32u  ccc = state[7];
    int32u  ddd = state[8];
    int32u  eee = state[9];

    for (idx = 0; idx < 16; idx++)
        xx[idx] = DWORD_LE(block[idx]);

    RMD320_R(aa, bb, cc, dd, ee, RMD320_F1, RMD320_K1, xx[ 0], 11);
    RMD320_R(ee, aa, bb, cc, dd, RMD320_F1, RMD320_K1, xx[ 1], 14);
    RMD320_R(dd, ee, aa, bb, cc, RMD320_F1, RMD320_K1, xx[ 2], 15);
    RMD320_R(cc, dd, ee, aa, bb, RMD320_F1, RMD320_K1, xx[ 3], 12);
    RMD320_R(bb, cc, dd, ee, aa, RMD320_F1, RMD320_K1, xx[ 4],  5);
    RMD320_R(aa, bb, cc, dd, ee, RMD320_F1, RMD320_K1, xx[ 5],  8);
    RMD320_R(ee, aa, bb, cc, dd, RMD320_F1, RMD320_K1, xx[ 6],  7);
    RMD320_R(dd, ee, aa, bb, cc, RMD320_F1, RMD320_K1, xx[ 7],  9);
    RMD320_R(cc, dd, ee, aa, bb, RMD320_F1, RMD320_K1, xx[ 8], 11);
    RMD320_R(bb, cc, dd, ee, aa, RMD320_F1, RMD320_K1, xx[ 9], 13);
    RMD320_R(aa, bb, cc, dd, ee, RMD320_F1, RMD320_K1, xx[10], 14);
    RMD320_R(ee, aa, bb, cc, dd, RMD320_F1, RMD320_K1, xx[11], 15);
    RMD320_R(dd, ee, aa, bb, cc, RMD320_F1, RMD320_K1, xx[12],  6);
    RMD320_R(cc, dd, ee, aa, bb, RMD320_F1, RMD320_K1, xx[13],  7);
    RMD320_R(bb, cc, dd, ee, aa, RMD320_F1, RMD320_K1, xx[14],  9);
    RMD320_R(aa, bb, cc, dd, ee, RMD320_F1, RMD320_K1, xx[15],  8);

    RMD320_R(aaa, bbb, ccc, ddd, eee, RMD320_F5, RMD320_KK1, xx[ 5],  8);
    RMD320_R(eee, aaa, bbb, ccc, ddd, RMD320_F5, RMD320_KK1, xx[14],  9);
    RMD320_R(ddd, eee, aaa, bbb, ccc, RMD320_F5, RMD320_KK1, xx[ 7],  9);
    RMD320_R(ccc, ddd, eee, aaa, bbb, RMD320_F5, RMD320_KK1, xx[ 0], 11);
    RMD320_R(bbb, ccc, ddd, eee, aaa, RMD320_F5, RMD320_KK1, xx[ 9], 13);
    RMD320_R(aaa, bbb, ccc, ddd, eee, RMD320_F5, RMD320_KK1, xx[ 2], 15);
    RMD320_R(eee, aaa, bbb, ccc, ddd, RMD320_F5, RMD320_KK1, xx[11], 15);
    RMD320_R(ddd, eee, aaa, bbb, ccc, RMD320_F5, RMD320_KK1, xx[ 4],  5);
    RMD320_R(ccc, ddd, eee, aaa, bbb, RMD320_F5, RMD320_KK1, xx[13],  7);
    RMD320_R(bbb, ccc, ddd, eee, aaa, RMD320_F5, RMD320_KK1, xx[ 6],  7);
    RMD320_R(aaa, bbb, ccc, ddd, eee, RMD320_F5, RMD320_KK1, xx[15],  8);
    RMD320_R(eee, aaa, bbb, ccc, ddd, RMD320_F5, RMD320_KK1, xx[ 8], 11);
    RMD320_R(ddd, eee, aaa, bbb, ccc, RMD320_F5, RMD320_KK1, xx[ 1], 14);
    RMD320_R(ccc, ddd, eee, aaa, bbb, RMD320_F5, RMD320_KK1, xx[10], 14);
    RMD320_R(bbb, ccc, ddd, eee, aaa, RMD320_F5, RMD320_KK1, xx[ 3], 12);
    RMD320_R(aaa, bbb, ccc, ddd, eee, RMD320_F5, RMD320_KK1, xx[12],  6);

    tmp = aa; aa = aaa; aaa = tmp;

    RMD320_R(ee, aa, bb, cc, dd, RMD320_F2, RMD320_K2, xx[ 7],  7);
    RMD320_R(dd, ee, aa, bb, cc, RMD320_F2, RMD320_K2, xx[ 4],  6);
    RMD320_R(cc, dd, ee, aa, bb, RMD320_F2, RMD320_K2, xx[13],  8);
    RMD320_R(bb, cc, dd, ee, aa, RMD320_F2, RMD320_K2, xx[ 1], 13);
    RMD320_R(aa, bb, cc, dd, ee, RMD320_F2, RMD320_K2, xx[10], 11);
    RMD320_R(ee, aa, bb, cc, dd, RMD320_F2, RMD320_K2, xx[ 6],  9);
    RMD320_R(dd, ee, aa, bb, cc, RMD320_F2, RMD320_K2, xx[15],  7);
    RMD320_R(cc, dd, ee, aa, bb, RMD320_F2, RMD320_K2, xx[ 3], 15);
    RMD320_R(bb, cc, dd, ee, aa, RMD320_F2, RMD320_K2, xx[12],  7);
    RMD320_R(aa, bb, cc, dd, ee, RMD320_F2, RMD320_K2, xx[ 0], 12);
    RMD320_R(ee, aa, bb, cc, dd, RMD320_F2, RMD320_K2, xx[ 9], 15);
    RMD320_R(dd, ee, aa, bb, cc, RMD320_F2, RMD320_K2, xx[ 5],  9);
    RMD320_R(cc, dd, ee, aa, bb, RMD320_F2, RMD320_K2, xx[ 2], 11);
    RMD320_R(bb, cc, dd, ee, aa, RMD320_F2, RMD320_K2, xx[14],  7);
    RMD320_R(aa, bb, cc, dd, ee, RMD320_F2, RMD320_K2, xx[11], 13);
    RMD320_R(ee, aa, bb, cc, dd, RMD320_F2, RMD320_K2, xx[ 8], 12);

    RMD320_R(eee, aaa, bbb, ccc, ddd, RMD320_F4, RMD320_KK2, xx[ 6],  9);
    RMD320_R(ddd, eee, aaa, bbb, ccc, RMD320_F4, RMD320_KK2, xx[11], 13);
    RMD320_R(ccc, ddd, eee, aaa, bbb, RMD320_F4, RMD320_KK2, xx[ 3], 15);
    RMD320_R(bbb, ccc, ddd, eee, aaa, RMD320_F4, RMD320_KK2, xx[ 7],  7);
    RMD320_R(aaa, bbb, ccc, ddd, eee, RMD320_F4, RMD320_KK2, xx[ 0], 12);
    RMD320_R(eee, aaa, bbb, ccc, ddd, RMD320_F4, RMD320_KK2, xx[13],  8);
    RMD320_R(ddd, eee, aaa, bbb, ccc, RMD320_F4, RMD320_KK2, xx[ 5],  9);
    RMD320_R(ccc, ddd, eee, aaa, bbb, RMD320_F4, RMD320_KK2, xx[10], 11);
    RMD320_R(bbb, ccc, ddd, eee, aaa, RMD320_F4, RMD320_KK2, xx[14],  7);
    RMD320_R(aaa, bbb, ccc, ddd, eee, RMD320_F4, RMD320_KK2, xx[15],  7);
    RMD320_R(eee, aaa, bbb, ccc, ddd, RMD320_F4, RMD320_KK2, xx[ 8], 12);
    RMD320_R(ddd, eee, aaa, bbb, ccc, RMD320_F4, RMD320_KK2, xx[12],  7);
    RMD320_R(ccc, ddd, eee, aaa, bbb, RMD320_F4, RMD320_KK2, xx[ 4],  6);
    RMD320_R(bbb, ccc, ddd, eee, aaa, RMD320_F4, RMD320_KK2, xx[ 9], 15);
    RMD320_R(aaa, bbb, ccc, ddd, eee, RMD320_F4, RMD320_KK2, xx[ 1], 13);
    RMD320_R(eee, aaa, bbb, ccc, ddd, RMD320_F4, RMD320_KK2, xx[ 2], 11);

    tmp = bb; bb = bbb; bbb = tmp;

    RMD320_R(dd, ee, aa, bb, cc, RMD320_F3, RMD320_K3, xx[ 3], 11);
    RMD320_R(cc, dd, ee, aa, bb, RMD320_F3, RMD320_K3, xx[10], 13);
    RMD320_R(bb, cc, dd, ee, aa, RMD320_F3, RMD320_K3, xx[14],  6);
    RMD320_R(aa, bb, cc, dd, ee, RMD320_F3, RMD320_K3, xx[ 4],  7);
    RMD320_R(ee, aa, bb, cc, dd, RMD320_F3, RMD320_K3, xx[ 9], 14);
    RMD320_R(dd, ee, aa, bb, cc, RMD320_F3, RMD320_K3, xx[15],  9);
    RMD320_R(cc, dd, ee, aa, bb, RMD320_F3, RMD320_K3, xx[ 8], 13);
    RMD320_R(bb, cc, dd, ee, aa, RMD320_F3, RMD320_K3, xx[ 1], 15);
    RMD320_R(aa, bb, cc, dd, ee, RMD320_F3, RMD320_K3, xx[ 2], 14);
    RMD320_R(ee, aa, bb, cc, dd, RMD320_F3, RMD320_K3, xx[ 7],  8);
    RMD320_R(dd, ee, aa, bb, cc, RMD320_F3, RMD320_K3, xx[ 0], 13);
    RMD320_R(cc, dd, ee, aa, bb, RMD320_F3, RMD320_K3, xx[ 6],  6);
    RMD320_R(bb, cc, dd, ee, aa, RMD320_F3, RMD320_K3, xx[13],  5);
    RMD320_R(aa, bb, cc, dd, ee, RMD320_F3, RMD320_K3, xx[11], 12);
    RMD320_R(ee, aa, bb, cc, dd, RMD320_F3, RMD320_K3, xx[ 5],  7);
    RMD320_R(dd, ee, aa, bb, cc, RMD320_F3, RMD320_K3, xx[12],  5);

    RMD320_R(ddd, eee, aaa, bbb, ccc, RMD320_F3, RMD320_KK3, xx[15],  9);
    RMD320_R(ccc, ddd, eee, aaa, bbb, RMD320_F3, RMD320_KK3, xx[ 5],  7);
    RMD320_R(bbb, ccc, ddd, eee, aaa, RMD320_F3, RMD320_KK3, xx[ 1], 15);
    RMD320_R(aaa, bbb, ccc, ddd, eee, RMD320_F3, RMD320_KK3, xx[ 3], 11);
    RMD320_R(eee, aaa, bbb, ccc, ddd, RMD320_F3, RMD320_KK3, xx[ 7],  8);
    RMD320_R(ddd, eee, aaa, bbb, ccc, RMD320_F3, RMD320_KK3, xx[14],  6);
    RMD320_R(ccc, ddd, eee, aaa, bbb, RMD320_F3, RMD320_KK3, xx[ 6],  6);
    RMD320_R(bbb, ccc, ddd, eee, aaa, RMD320_F3, RMD320_KK3, xx[ 9], 14);
    RMD320_R(aaa, bbb, ccc, ddd, eee, RMD320_F3, RMD320_KK3, xx[11], 12);
    RMD320_R(eee, aaa, bbb, ccc, ddd, RMD320_F3, RMD320_KK3, xx[ 8], 13);
    RMD320_R(ddd, eee, aaa, bbb, ccc, RMD320_F3, RMD320_KK3, xx[12],  5);
    RMD320_R(ccc, ddd, eee, aaa, bbb, RMD320_F3, RMD320_KK3, xx[ 2], 14);
    RMD320_R(bbb, ccc, ddd, eee, aaa, RMD320_F3, RMD320_KK3, xx[10], 13);
    RMD320_R(aaa, bbb, ccc, ddd, eee, RMD320_F3, RMD320_KK3, xx[ 0], 13);
    RMD320_R(eee, aaa, bbb, ccc, ddd, RMD320_F3, RMD320_KK3, xx[ 4],  7);
    RMD320_R(ddd, eee, aaa, bbb, ccc, RMD320_F3, RMD320_KK3, xx[13],  5);

    tmp = cc; cc = ccc; ccc = tmp;

    RMD320_R(cc, dd, ee, aa, bb, RMD320_F4, RMD320_K4, xx[ 1], 11);
    RMD320_R(bb, cc, dd, ee, aa, RMD320_F4, RMD320_K4, xx[ 9], 12);
    RMD320_R(aa, bb, cc, dd, ee, RMD320_F4, RMD320_K4, xx[11], 14);
    RMD320_R(ee, aa, bb, cc, dd, RMD320_F4, RMD320_K4, xx[10], 15);
    RMD320_R(dd, ee, aa, bb, cc, RMD320_F4, RMD320_K4, xx[ 0], 14);
    RMD320_R(cc, dd, ee, aa, bb, RMD320_F4, RMD320_K4, xx[ 8], 15);
    RMD320_R(bb, cc, dd, ee, aa, RMD320_F4, RMD320_K4, xx[12],  9);
    RMD320_R(aa, bb, cc, dd, ee, RMD320_F4, RMD320_K4, xx[ 4],  8);
    RMD320_R(ee, aa, bb, cc, dd, RMD320_F4, RMD320_K4, xx[13],  9);
    RMD320_R(dd, ee, aa, bb, cc, RMD320_F4, RMD320_K4, xx[ 3], 14);
    RMD320_R(cc, dd, ee, aa, bb, RMD320_F4, RMD320_K4, xx[ 7],  5);
    RMD320_R(bb, cc, dd, ee, aa, RMD320_F4, RMD320_K4, xx[15],  6);
    RMD320_R(aa, bb, cc, dd, ee, RMD320_F4, RMD320_K4, xx[14],  8);
    RMD320_R(ee, aa, bb, cc, dd, RMD320_F4, RMD320_K4, xx[ 5],  6);
    RMD320_R(dd, ee, aa, bb, cc, RMD320_F4, RMD320_K4, xx[ 6],  5);
    RMD320_R(cc, dd, ee, aa, bb, RMD320_F4, RMD320_K4, xx[ 2], 12);

    RMD320_R(ccc, ddd, eee, aaa, bbb, RMD320_F2, RMD320_KK4, xx[ 8], 15);
    RMD320_R(bbb, ccc, ddd, eee, aaa, RMD320_F2, RMD320_KK4, xx[ 6],  5);
    RMD320_R(aaa, bbb, ccc, ddd, eee, RMD320_F2, RMD320_KK4, xx[ 4],  8);
    RMD320_R(eee, aaa, bbb, ccc, ddd, RMD320_F2, RMD320_KK4, xx[ 1], 11);
    RMD320_R(ddd, eee, aaa, bbb, ccc, RMD320_F2, RMD320_KK4, xx[ 3], 14);
    RMD320_R(ccc, ddd, eee, aaa, bbb, RMD320_F2, RMD320_KK4, xx[11], 14);
    RMD320_R(bbb, ccc, ddd, eee, aaa, RMD320_F2, RMD320_KK4, xx[15],  6);
    RMD320_R(aaa, bbb, ccc, ddd, eee, RMD320_F2, RMD320_KK4, xx[ 0], 14);
    RMD320_R(eee, aaa, bbb, ccc, ddd, RMD320_F2, RMD320_KK4, xx[ 5],  6);
    RMD320_R(ddd, eee, aaa, bbb, ccc, RMD320_F2, RMD320_KK4, xx[12],  9);
    RMD320_R(ccc, ddd, eee, aaa, bbb, RMD320_F2, RMD320_KK4, xx[ 2], 12);
    RMD320_R(bbb, ccc, ddd, eee, aaa, RMD320_F2, RMD320_KK4, xx[13],  9);
    RMD320_R(aaa, bbb, ccc, ddd, eee, RMD320_F2, RMD320_KK4, xx[ 9], 12);
    RMD320_R(eee, aaa, bbb, ccc, ddd, RMD320_F2, RMD320_KK4, xx[ 7],  5);
    RMD320_R(ddd, eee, aaa, bbb, ccc, RMD320_F2, RMD320_KK4, xx[10], 15);
    RMD320_R(ccc, ddd, eee, aaa, bbb, RMD320_F2, RMD320_KK4, xx[14],  8);

    tmp = dd; dd = ddd; ddd = tmp;

    RMD320_R(bb, cc, dd, ee, aa, RMD320_F5, RMD320_K5, xx[ 4],  9);
    RMD320_R(aa, bb, cc, dd, ee, RMD320_F5, RMD320_K5, xx[ 0], 15);
    RMD320_R(ee, aa, bb, cc, dd, RMD320_F5, RMD320_K5, xx[ 5],  5);
    RMD320_R(dd, ee, aa, bb, cc, RMD320_F5, RMD320_K5, xx[ 9], 11);
    RMD320_R(cc, dd, ee, aa, bb, RMD320_F5, RMD320_K5, xx[ 7],  6);
    RMD320_R(bb, cc, dd, ee, aa, RMD320_F5, RMD320_K5, xx[12],  8);
    RMD320_R(aa, bb, cc, dd, ee, RMD320_F5, RMD320_K5, xx[ 2], 13);
    RMD320_R(ee, aa, bb, cc, dd, RMD320_F5, RMD320_K5, xx[10], 12);
    RMD320_R(dd, ee, aa, bb, cc, RMD320_F5, RMD320_K5, xx[14],  5);
    RMD320_R(cc, dd, ee, aa, bb, RMD320_F5, RMD320_K5, xx[ 1], 12);
    RMD320_R(bb, cc, dd, ee, aa, RMD320_F5, RMD320_K5, xx[ 3], 13);
    RMD320_R(aa, bb, cc, dd, ee, RMD320_F5, RMD320_K5, xx[ 8], 14);
    RMD320_R(ee, aa, bb, cc, dd, RMD320_F5, RMD320_K5, xx[11], 11);
    RMD320_R(dd, ee, aa, bb, cc, RMD320_F5, RMD320_K5, xx[ 6],  8);
    RMD320_R(cc, dd, ee, aa, bb, RMD320_F5, RMD320_K5, xx[15],  5);
    RMD320_R(bb, cc, dd, ee, aa, RMD320_F5, RMD320_K5, xx[13],  6);

    RMD320_R(bbb, ccc, ddd, eee, aaa, RMD320_F1, RMD320_KK5, xx[12],  8);
    RMD320_R(aaa, bbb, ccc, ddd, eee, RMD320_F1, RMD320_KK5, xx[15],  5);
    RMD320_R(eee, aaa, bbb, ccc, ddd, RMD320_F1, RMD320_KK5, xx[10], 12);
    RMD320_R(ddd, eee, aaa, bbb, ccc, RMD320_F1, RMD320_KK5, xx[ 4],  9);
    RMD320_R(ccc, ddd, eee, aaa, bbb, RMD320_F1, RMD320_KK5, xx[ 1], 12);
    RMD320_R(bbb, ccc, ddd, eee, aaa, RMD320_F1, RMD320_KK5, xx[ 5],  5);
    RMD320_R(aaa, bbb, ccc, ddd, eee, RMD320_F1, RMD320_KK5, xx[ 8], 14);
    RMD320_R(eee, aaa, bbb, ccc, ddd, RMD320_F1, RMD320_KK5, xx[ 7],  6);
    RMD320_R(ddd, eee, aaa, bbb, ccc, RMD320_F1, RMD320_KK5, xx[ 6],  8);
    RMD320_R(ccc, ddd, eee, aaa, bbb, RMD320_F1, RMD320_KK5, xx[ 2], 13);
    RMD320_R(bbb, ccc, ddd, eee, aaa, RMD320_F1, RMD320_KK5, xx[13],  6);
    RMD320_R(aaa, bbb, ccc, ddd, eee, RMD320_F1, RMD320_KK5, xx[14],  5);
    RMD320_R(eee, aaa, bbb, ccc, ddd, RMD320_F1, RMD320_KK5, xx[ 0], 15);
    RMD320_R(ddd, eee, aaa, bbb, ccc, RMD320_F1, RMD320_KK5, xx[ 3], 13);
    RMD320_R(ccc, ddd, eee, aaa, bbb, RMD320_F1, RMD320_KK5, xx[ 9], 11);
    RMD320_R(bbb, ccc, ddd, eee, aaa, RMD320_F1, RMD320_KK5, xx[11], 11);

    tmp = ee; ee = eee; eee = tmp;

    state[0] += aa;
    state[1] += bb;
    state[2] += cc;
    state[3] += dd;
    state[4] += ee;
    state[5] += aaa;
    state[6] += bbb;
    state[7] += ccc;
    state[8] += ddd;
    state[9] += eee;
}

/*
=======================================
    RMD-320 初始化
=======================================
*/
CR_API void_t
hash_rmd320_init (
  __CR_OT__ sRMD320*    ctx
    )
{
    ctx->count[0] = 0UL;
    ctx->count[1] = 0UL;
    ctx->state[0] = 0x67452301UL;
    ctx->state[1] = 0xEFCDAB89UL;
    ctx->state[2] = 0x98BADCFEUL;
    ctx->state[3] = 0x10325476UL;
    ctx->state[4] = 0xC3D2E1F0UL;
    ctx->state[5] = 0x76543210UL;
    ctx->state[6] = 0xFEDCBA98UL;
    ctx->state[7] = 0x89ABCDEFUL;
    ctx->state[8] = 0x01234567UL;
    ctx->state[9] = 0x3C2D1E0FUL;
}

/*
=======================================
    RMD-320 计算数据块
=======================================
*/
CR_API void_t
hash_rmd320_update (
  __CR_IO__ sRMD320*        ctx,
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
        rmd320_transform(ctx->state, (int32u*)ctx->buffer);
        size -= fill;
        while (size >= 64) {
            mem_cpy(ctx->buffer, data, 64);
            rmd320_transform(ctx->state, (int32u*)ctx->buffer);
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
    RMD-320 获取结果
=======================================
*/
CR_API void_t
hash_rmd320_finish (
  __CR_OT__ byte_t      hash[40],
  __CR_IO__ sRMD320*    ctx
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
    hash_rmd320_update(ctx, s_padding, len);
    hash_rmd320_update(ctx, bits, 8);

    for (idx = 0; idx < 10; idx++)
        ctx->state[idx] = DWORD_LE(ctx->state[idx]);
    mem_cpy(hash, ctx->state, 40);
}

/*
=======================================
    RMD-320 一次性完成
=======================================
*/
CR_API void_t
hash_rmd320_total (
  __CR_OT__ byte_t          hash[40],
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    sRMD320     ctx;

    hash_rmd320_init(&ctx);
    hash_rmd320_update(&ctx, data, size);
    hash_rmd320_finish(hash, &ctx);
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
