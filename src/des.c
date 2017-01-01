/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-07-06  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack DES 加密解密函数库 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "crypto.h"

/* DES 常数表 */
static const byte_t _rom_ s_bytebit[8] =
{
    0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01,
};

static const int32u _rom_ s_bigbyte[24] =
{
    0x800000UL, 0x400000UL, 0x200000UL, 0x100000UL,
    0x80000UL,  0x40000UL,  0x20000UL,  0x10000UL,
    0x8000UL,   0x4000UL,   0x2000UL,   0x1000UL,
    0x800UL,    0x400UL,    0x200UL,    0x100UL,
    0x80UL,     0x40UL,     0x20UL,     0x10UL,
    0x8UL,      0x4UL,      0x2UL,      0x1UL,
};

static const byte_t _rom_ s_pc1[56] =
{
    56, 48, 40, 32, 24, 16,  8,  0, 57, 49, 41, 33, 25, 17,
     9,  1, 58, 50, 42, 34, 26, 18, 10,  2, 59, 51, 43, 35,
    62, 54, 46, 38, 30, 22, 14,  6, 61, 53, 45, 37, 29, 21,
    13,  5, 60, 52, 44, 36, 28, 20, 12,  4, 27, 19, 11,  3,
};

static const byte_t _rom_ s_pc2[48] =
{
    13, 16, 10, 23,  0,  4,          2, 27, 14,  5, 20,  9,
    22, 18, 11,  3, 25,  7,         15,  6, 26, 19, 12,  1,
    40, 51, 30, 36, 46, 54,         29, 39, 50, 44, 32, 47,
    43, 48, 38, 55, 33, 52,         45, 41, 49, 35, 28, 31,
};

static const int32u _rom_ s_sp1[64] =
{
    0x01010400UL, 0x00000000UL, 0x00010000UL, 0x01010404UL,
    0x01010004UL, 0x00010404UL, 0x00000004UL, 0x00010000UL,
    0x00000400UL, 0x01010400UL, 0x01010404UL, 0x00000400UL,
    0x01000404UL, 0x01010004UL, 0x01000000UL, 0x00000004UL,
    0x00000404UL, 0x01000400UL, 0x01000400UL, 0x00010400UL,
    0x00010400UL, 0x01010000UL, 0x01010000UL, 0x01000404UL,
    0x00010004UL, 0x01000004UL, 0x01000004UL, 0x00010004UL,
    0x00000000UL, 0x00000404UL, 0x00010404UL, 0x01000000UL,
    0x00010000UL, 0x01010404UL, 0x00000004UL, 0x01010000UL,
    0x01010400UL, 0x01000000UL, 0x01000000UL, 0x00000400UL,
    0x01010004UL, 0x00010000UL, 0x00010400UL, 0x01000004UL,
    0x00000400UL, 0x00000004UL, 0x01000404UL, 0x00010404UL,
    0x01010404UL, 0x00010004UL, 0x01010000UL, 0x01000404UL,
    0x01000004UL, 0x00000404UL, 0x00010404UL, 0x01010400UL,
    0x00000404UL, 0x01000400UL, 0x01000400UL, 0x00000000UL,
    0x00010004UL, 0x00010400UL, 0x00000000UL, 0x01010004UL,
};

static const int32u _rom_ s_sp2[64] =
{
    0x80108020UL, 0x80008000UL, 0x00008000UL, 0x00108020UL,
    0x00100000UL, 0x00000020UL, 0x80100020UL, 0x80008020UL,
    0x80000020UL, 0x80108020UL, 0x80108000UL, 0x80000000UL,
    0x80008000UL, 0x00100000UL, 0x00000020UL, 0x80100020UL,
    0x00108000UL, 0x00100020UL, 0x80008020UL, 0x00000000UL,
    0x80000000UL, 0x00008000UL, 0x00108020UL, 0x80100000UL,
    0x00100020UL, 0x80000020UL, 0x00000000UL, 0x00108000UL,
    0x00008020UL, 0x80108000UL, 0x80100000UL, 0x00008020UL,
    0x00000000UL, 0x00108020UL, 0x80100020UL, 0x00100000UL,
    0x80008020UL, 0x80100000UL, 0x80108000UL, 0x00008000UL,
    0x80100000UL, 0x80008000UL, 0x00000020UL, 0x80108020UL,
    0x00108020UL, 0x00000020UL, 0x00008000UL, 0x80000000UL,
    0x00008020UL, 0x80108000UL, 0x00100000UL, 0x80000020UL,
    0x00100020UL, 0x80008020UL, 0x80000020UL, 0x00100020UL,
    0x00108000UL, 0x00000000UL, 0x80008000UL, 0x00008020UL,
    0x80000000UL, 0x80100020UL, 0x80108020UL, 0x00108000UL,
};

static const int32u _rom_ s_sp3[64] =
{
    0x00000208UL, 0x08020200UL, 0x00000000UL, 0x08020008UL,
    0x08000200UL, 0x00000000UL, 0x00020208UL, 0x08000200UL,
    0x00020008UL, 0x08000008UL, 0x08000008UL, 0x00020000UL,
    0x08020208UL, 0x00020008UL, 0x08020000UL, 0x00000208UL,
    0x08000000UL, 0x00000008UL, 0x08020200UL, 0x00000200UL,
    0x00020200UL, 0x08020000UL, 0x08020008UL, 0x00020208UL,
    0x08000208UL, 0x00020200UL, 0x00020000UL, 0x08000208UL,
    0x00000008UL, 0x08020208UL, 0x00000200UL, 0x08000000UL,
    0x08020200UL, 0x08000000UL, 0x00020008UL, 0x00000208UL,
    0x00020000UL, 0x08020200UL, 0x08000200UL, 0x00000000UL,
    0x00000200UL, 0x00020008UL, 0x08020208UL, 0x08000200UL,
    0x08000008UL, 0x00000200UL, 0x00000000UL, 0x08020008UL,
    0x08000208UL, 0x00020000UL, 0x08000000UL, 0x08020208UL,
    0x00000008UL, 0x00020208UL, 0x00020200UL, 0x08000008UL,
    0x08020000UL, 0x08000208UL, 0x00000208UL, 0x08020000UL,
    0x00020208UL, 0x00000008UL, 0x08020008UL, 0x00020200UL,
};

static const int32u _rom_ s_sp4[64] =
{
    0x00802001UL, 0x00002081UL, 0x00002081UL, 0x00000080UL,
    0x00802080UL, 0x00800081UL, 0x00800001UL, 0x00002001UL,
    0x00000000UL, 0x00802000UL, 0x00802000UL, 0x00802081UL,
    0x00000081UL, 0x00000000UL, 0x00800080UL, 0x00800001UL,
    0x00000001UL, 0x00002000UL, 0x00800000UL, 0x00802001UL,
    0x00000080UL, 0x00800000UL, 0x00002001UL, 0x00002080UL,
    0x00800081UL, 0x00000001UL, 0x00002080UL, 0x00800080UL,
    0x00002000UL, 0x00802080UL, 0x00802081UL, 0x00000081UL,
    0x00800080UL, 0x00800001UL, 0x00802000UL, 0x00802081UL,
    0x00000081UL, 0x00000000UL, 0x00000000UL, 0x00802000UL,
    0x00002080UL, 0x00800080UL, 0x00800081UL, 0x00000001UL,
    0x00802001UL, 0x00002081UL, 0x00002081UL, 0x00000080UL,
    0x00802081UL, 0x00000081UL, 0x00000001UL, 0x00002000UL,
    0x00800001UL, 0x00002001UL, 0x00802080UL, 0x00800081UL,
    0x00002001UL, 0x00002080UL, 0x00800000UL, 0x00802001UL,
    0x00000080UL, 0x00800000UL, 0x00002000UL, 0x00802080UL,
};

static const int32u _rom_ s_sp5[64] =
{
    0x00000100UL, 0x02080100UL, 0x02080000UL, 0x42000100UL,
    0x00080000UL, 0x00000100UL, 0x40000000UL, 0x02080000UL,
    0x40080100UL, 0x00080000UL, 0x02000100UL, 0x40080100UL,
    0x42000100UL, 0x42080000UL, 0x00080100UL, 0x40000000UL,
    0x02000000UL, 0x40080000UL, 0x40080000UL, 0x00000000UL,
    0x40000100UL, 0x42080100UL, 0x42080100UL, 0x02000100UL,
    0x42080000UL, 0x40000100UL, 0x00000000UL, 0x42000000UL,
    0x02080100UL, 0x02000000UL, 0x42000000UL, 0x00080100UL,
    0x00080000UL, 0x42000100UL, 0x00000100UL, 0x02000000UL,
    0x40000000UL, 0x02080000UL, 0x42000100UL, 0x40080100UL,
    0x02000100UL, 0x40000000UL, 0x42080000UL, 0x02080100UL,
    0x40080100UL, 0x00000100UL, 0x02000000UL, 0x42080000UL,
    0x42080100UL, 0x00080100UL, 0x42000000UL, 0x42080100UL,
    0x02080000UL, 0x00000000UL, 0x40080000UL, 0x42000000UL,
    0x00080100UL, 0x02000100UL, 0x40000100UL, 0x00080000UL,
    0x00000000UL, 0x40080000UL, 0x02080100UL, 0x40000100UL,
};

static const int32u _rom_ s_sp6[64] =
{
    0x20000010UL, 0x20400000UL, 0x00004000UL, 0x20404010UL,
    0x20400000UL, 0x00000010UL, 0x20404010UL, 0x00400000UL,
    0x20004000UL, 0x00404010UL, 0x00400000UL, 0x20000010UL,
    0x00400010UL, 0x20004000UL, 0x20000000UL, 0x00004010UL,
    0x00000000UL, 0x00400010UL, 0x20004010UL, 0x00004000UL,
    0x00404000UL, 0x20004010UL, 0x00000010UL, 0x20400010UL,
    0x20400010UL, 0x00000000UL, 0x00404010UL, 0x20404000UL,
    0x00004010UL, 0x00404000UL, 0x20404000UL, 0x20000000UL,
    0x20004000UL, 0x00000010UL, 0x20400010UL, 0x00404000UL,
    0x20404010UL, 0x00400000UL, 0x00004010UL, 0x20000010UL,
    0x00400000UL, 0x20004000UL, 0x20000000UL, 0x00004010UL,
    0x20000010UL, 0x20404010UL, 0x00404000UL, 0x20400000UL,
    0x00404010UL, 0x20404000UL, 0x00000000UL, 0x20400010UL,
    0x00000010UL, 0x00004000UL, 0x20400000UL, 0x00404010UL,
    0x00004000UL, 0x00400010UL, 0x20004010UL, 0x00000000UL,
    0x20404000UL, 0x20000000UL, 0x00400010UL, 0x20004010UL,
};

static const int32u _rom_ s_sp7[64] =
{
    0x00200000UL, 0x04200002UL, 0x04000802UL, 0x00000000UL,
    0x00000800UL, 0x04000802UL, 0x00200802UL, 0x04200800UL,
    0x04200802UL, 0x00200000UL, 0x00000000UL, 0x04000002UL,
    0x00000002UL, 0x04000000UL, 0x04200002UL, 0x00000802UL,
    0x04000800UL, 0x00200802UL, 0x00200002UL, 0x04000800UL,
    0x04000002UL, 0x04200000UL, 0x04200800UL, 0x00200002UL,
    0x04200000UL, 0x00000800UL, 0x00000802UL, 0x04200802UL,
    0x00200800UL, 0x00000002UL, 0x04000000UL, 0x00200800UL,
    0x04000000UL, 0x00200800UL, 0x00200000UL, 0x04000802UL,
    0x04000802UL, 0x04200002UL, 0x04200002UL, 0x00000002UL,
    0x00200002UL, 0x04000000UL, 0x04000800UL, 0x00200000UL,
    0x04200800UL, 0x00000802UL, 0x00200802UL, 0x04200800UL,
    0x00000802UL, 0x04000002UL, 0x04200802UL, 0x04200000UL,
    0x00200800UL, 0x00000000UL, 0x00000002UL, 0x04200802UL,
    0x00000000UL, 0x00200802UL, 0x04200000UL, 0x00000800UL,
    0x04000002UL, 0x04000800UL, 0x00000800UL, 0x00200002UL,
};

static const int32u _rom_ s_sp8[64] =
{
    0x10001040UL, 0x00001000UL, 0x00040000UL, 0x10041040UL,
    0x10000000UL, 0x10001040UL, 0x00000040UL, 0x10000000UL,
    0x00040040UL, 0x10040000UL, 0x10041040UL, 0x00041000UL,
    0x10041000UL, 0x00041040UL, 0x00001000UL, 0x00000040UL,
    0x10040000UL, 0x10000040UL, 0x10001000UL, 0x00001040UL,
    0x00041000UL, 0x00040040UL, 0x10040040UL, 0x10041000UL,
    0x00001040UL, 0x00000000UL, 0x00000000UL, 0x10040040UL,
    0x10000040UL, 0x10001000UL, 0x00041040UL, 0x00040000UL,
    0x00041040UL, 0x00040000UL, 0x10041000UL, 0x00001000UL,
    0x00000040UL, 0x10040040UL, 0x00001000UL, 0x00041040UL,
    0x10001000UL, 0x00000040UL, 0x10000040UL, 0x10040000UL,
    0x10040040UL, 0x10000000UL, 0x00040000UL, 0x10001040UL,
    0x00000000UL, 0x10041040UL, 0x00040040UL, 0x10000040UL,
    0x10040000UL, 0x10001000UL, 0x10001040UL, 0x00000000UL,
    0x10041040UL, 0x00041000UL, 0x00041000UL, 0x00001040UL,
    0x00001040UL, 0x00040040UL, 0x10000000UL, 0x10041000UL,
};

static const byte_t _rom_ s_totrot[16] =
{
    1, 2, 4, 6, 8, 10, 12, 14, 15, 17, 19, 21, 23, 25, 27, 28,
};

/*
---------------------------------------
    DES 处理密钥
---------------------------------------
*/
static void_t
cookey (
  __CR_OT__ sDES*   ctx,
  __CR_IO__ int32u* raw1
    )
{
    uint_t  idx;
    int32u* cook;
    int32u* raw0;
    int32u  dough[32];

    cook = dough;
    for (idx = 0; idx < 16; idx++, raw1++)
    {
         raw0 = raw1++;
        *cook    = (int32u)((*raw0 & 0x00FC0000UL) <<  6);
        *cook   |= (int32u)((*raw0 & 0x00000FC0UL) << 10);
        *cook   |= (int32u)((*raw1 & 0x00FC0000UL) >> 10);
        *cook++ |= (int32u)((*raw1 & 0x00000FC0UL) >>  6);
        *cook    = (int32u)((*raw0 & 0x0003F000UL) << 12);
        *cook   |= (int32u)((*raw0 & 0x0000003FUL) << 16);
        *cook   |= (int32u)((*raw1 & 0x0003F000UL) >>  4);
        *cook++ |= (int32u)((*raw1 & 0x0000003FUL));
    }
    for (idx = 0; idx < 32; idx++)
        ctx->knl[idx] = dough[idx];
}

/*
---------------------------------------
    DES 编码字节数据
---------------------------------------
*/
static void_t
scrunch (
  __CR_IN__ const byte_t*   src,
  __CR_OT__ int32u*         dst
    )
{
    *dst    = ((int32u)(*src++)) << 24;
    *dst   |= ((int32u)(*src++)) << 16;
    *dst   |= ((int32u)(*src++)) <<  8;
    *dst++ |= ((int32u)(*src++));
    *dst    = ((int32u)(*src++)) << 24;
    *dst   |= ((int32u)(*src++)) << 16;
    *dst   |= ((int32u)(*src++)) <<  8;
    *dst   |= ((int32u)(*src  ));
}

/*
---------------------------------------
    DES 解码字节数据
---------------------------------------
*/
static void_t
unscrun (
  __CR_IN__ const int32u*   src,
  __CR_OT__ byte_t*         dst
    )
{
    *dst++ = (byte_t)(*src >> 24);
    *dst++ = (byte_t)(*src >> 16);
    *dst++ = (byte_t)(*src >>  8);
    *dst++ = (byte_t)(*src++    );
    *dst++ = (byte_t)(*src >> 24);
    *dst++ = (byte_t)(*src >> 16);
    *dst++ = (byte_t)(*src >>  8);
    *dst   = (byte_t)(*src >>  0);
}

/*
---------------------------------------
    DES 加密解密数据块
---------------------------------------
*/
static void_t
desfunc (
  __CR_IO__ int32u*         block,
  __CR_IN__ const int32u*   keys
    )
{
    uint_t  round;
    int32u  fval, work, right, leftt;

    leftt = block[0];
    right = block[1];
    work = ((leftt >>  4) ^ right) & 0x0F0F0F0FUL;
    right ^= (work <<  0);
    leftt ^= (work <<  4);
    work = ((leftt >> 16) ^ right) & 0x0000FFFFUL;
    right ^= (work <<  0);
    leftt ^= (work << 16);
    work = ((right >>  2) ^ leftt) & 0x33333333UL;
    leftt ^= (work <<  0);
    right ^= (work <<  2);
    work = ((right >>  8) ^ leftt) & 0x00FF00FFUL;
    leftt ^= (work <<  0);
    right ^= (work <<  8);
    right = (right <<  1) | ((right >> 31) & 1);
    work = (leftt ^ right) & 0xAAAAAAAAUL;
    leftt ^= (work <<  0);
    right ^= (work <<  0);
    leftt = (leftt <<  1) | ((leftt >> 31) & 1);

    for (round = 0; round < 8; round++)
    {
        work = (right << 28) | (right >> 4);
        work ^= *keys++;
        fval  = s_sp7[(work >>  0) & 0x3F];
        fval |= s_sp5[(work >>  8) & 0x3F];
        fval |= s_sp3[(work >> 16) & 0x3F];
        fval |= s_sp1[(work >> 24) & 0x3F];
        work = right ^ *keys++;
        fval |= s_sp8[(work >>  0) & 0x3F];
        fval |= s_sp6[(work >>  8) & 0x3F];
        fval |= s_sp4[(work >> 16) & 0x3F];
        fval |= s_sp2[(work >> 24) & 0x3F];
        leftt ^= fval;
        work = (leftt << 28) | (leftt >> 4);
        work ^= *keys++;
        fval  = s_sp7[(work >>  0) & 0x3F];
        fval |= s_sp5[(work >>  8) & 0x3F];
        fval |= s_sp3[(work >> 16) & 0x3F];
        fval |= s_sp1[(work >> 24) & 0x3F];
        work = leftt ^ *keys++;
        fval |= s_sp8[(work >>  0) & 0x3F];
        fval |= s_sp6[(work >>  8) & 0x3F];
        fval |= s_sp4[(work >> 16) & 0x3F];
        fval |= s_sp2[(work >> 24) & 0x3F];
        right ^= fval;
    }
    right = (right << 31) | (right >> 1);
    work = (leftt ^ right) & 0xAAAAAAAAUL;
    leftt ^= (work <<  0);
    right ^= (work <<  0);
    leftt = (leftt << 31) | (leftt >> 1);
    work = ((leftt >>  8) ^ right) & 0x00FF00FFUL;
    right ^= (work <<  0);
    leftt ^= (work <<  8);
    work = ((leftt >>  2) ^ right) & 0x33333333UL;
    right ^= (work <<  0);
    leftt ^= (work <<  2);
    work = ((right >> 16) ^ leftt) & 0x0000FFFFUL;
    leftt ^= (work <<  0);
    right ^= (work << 16);
    work = ((right >>  4) ^ leftt) & 0x0F0F0F0FUL;
    leftt ^= (work <<  0);
    right ^= (work <<  4);
    block[0] = right;
    block[1] = leftt;
}

/*
=======================================
    DES 设置密钥
=======================================
*/
CR_API void_t
crypto_des_key (
  __CR_OT__ sDES*           ctx,
  __CR_IN__ const byte_t    key[8],
  __CR_IN__ bool_t          is_dec
    )
{
    int32u  kn[32];
    byte_t  pc1m[56], pcr[56];
    uint_t  ii, jj, ll, mm, nn;

    for (jj = 0; jj < 56; jj++)
    {
        ll = s_pc1[jj];
        mm = ll & 0x07;
        if (key[ll >> 3] & s_bytebit[mm])
            pc1m[jj] = 1;
        else
            pc1m[jj] = 0;
    }

    for (ii = 0; ii < 16; ii++)
    {
        if (is_dec)
            mm = (15 - ii) << 1;
        else
            mm = ii << 1;
        nn = mm + 1;
        kn[mm] = kn[nn] = 0UL;
        for (jj = 0; jj < 28; jj++)
        {
            ll = jj + s_totrot[ii];
            if (ll < 28)
                pcr[jj] = pc1m[ll];
            else
                pcr[jj] = pc1m[ll - 28];
        }
        for (jj = 28; jj < 56; jj++)
        {
            ll = jj + s_totrot[ii];
            if (ll < 56)
                pcr[jj] = pc1m[ll];
            else
                pcr[jj] = pc1m[ll - 28];
        }
        for (jj = 0; jj < 24; jj++)
        {
            if (pcr[s_pc2[jj +  0]])
                kn[mm] |= s_bigbyte[jj];
            if (pcr[s_pc2[jj + 24]])
                kn[nn] |= s_bigbyte[jj];
        }
    }
    cookey(ctx, kn);
}

/*
=======================================
    DES 加密解密 (根据密钥来区别)
=======================================
*/
CR_API void_t
crypto_des_ops (
  __CR_IN__ const sDES*     ctx,
  __CR_OT__ byte_t          dst[8],
  __CR_IN__ const byte_t    src[8]
    )
{
    int32u  work[2];

    scrunch(src, work);
    desfunc(work, ctx->knl);
    unscrun(work, dst);
}

/*
=======================================
    3DES 设置密钥 (双密钥)
=======================================
*/
CR_API void_t
crypto_3des_key2 (
  __CR_OT__ s3DES*          ctx,
  __CR_IN__ const byte_t    key[16]
    )
{
    crypto_des_key(&ctx->dk1, &key[0], TRUE);
    crypto_des_key(&ctx->dk2, &key[8], TRUE);
    crypto_des_key(&ctx->dk3, &key[0], TRUE);
    crypto_des_key(&ctx->ek1, &key[0], FALSE);
    crypto_des_key(&ctx->ek2, &key[8], FALSE);
    crypto_des_key(&ctx->ek3, &key[0], FALSE);
}

/*
=======================================
    3DES 设置密钥 (三密钥)
=======================================
*/
CR_API void_t
crypto_3des_key3 (
  __CR_OT__ s3DES*          ctx,
  __CR_IN__ const byte_t    key[24]
    )
{
    crypto_des_key(&ctx->dk1, &key[ 0], TRUE);
    crypto_des_key(&ctx->dk2, &key[ 8], TRUE);
    crypto_des_key(&ctx->dk3, &key[16], TRUE);
    crypto_des_key(&ctx->ek1, &key[ 0], FALSE);
    crypto_des_key(&ctx->ek2, &key[ 8], FALSE);
    crypto_des_key(&ctx->ek3, &key[16], FALSE);
}

/*
=======================================
    3DES 加密1
=======================================
*/
CR_API void_t
crypto_3des_enc1 (
  __CR_IN__ const s3DES*    ctx,
  __CR_OT__ byte_t          dst[8],
  __CR_IN__ const byte_t    src[8]
    )
{
    byte_t  tmp[8];

    crypto_des_ops(&ctx->ek1, tmp, src);
    crypto_des_ops(&ctx->dk2, tmp, tmp);
    crypto_des_ops(&ctx->ek3, dst, tmp);
}

/*
=======================================
    3DES 解密1
=======================================
*/
CR_API void_t
crypto_3des_dec1 (
  __CR_IN__ const s3DES*    ctx,
  __CR_OT__ byte_t          dst[8],
  __CR_IN__ const byte_t    src[8]
    )
{
    byte_t  tmp[8];

    crypto_des_ops(&ctx->dk3, tmp, src);
    crypto_des_ops(&ctx->ek2, tmp, tmp);
    crypto_des_ops(&ctx->dk1, dst, tmp);
}

/*
=======================================
    3DES 加密2
=======================================
*/
CR_API void_t
crypto_3des_enc2 (
  __CR_IN__ const s3DES*    ctx,
  __CR_OT__ byte_t          dst[8],
  __CR_IN__ const byte_t    src[8]
    )
{
    byte_t  tmp[8];

    crypto_des_ops(&ctx->ek1, tmp, src);
    crypto_des_ops(&ctx->ek2, tmp, tmp);
    crypto_des_ops(&ctx->ek3, dst, tmp);
}

/*
=======================================
    3DES 解密2
=======================================
*/
CR_API void_t
crypto_3des_dec2 (
  __CR_IN__ const s3DES*    ctx,
  __CR_OT__ byte_t          dst[8],
  __CR_IN__ const byte_t    src[8]
    )
{
    byte_t  tmp[8];

    crypto_des_ops(&ctx->dk3, tmp, src);
    crypto_des_ops(&ctx->dk2, tmp, tmp);
    crypto_des_ops(&ctx->dk1, dst, tmp);
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
