/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2011-06-26  */
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
/*  >>>>>>>>>>>>>>>>>>>> CrHack ECC-256/512 编码函数库 <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "hash.h"

/* 有用的查表 */
static const byte_t _rom_ s_invparity[256] =
{
    1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
    1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
    1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
    1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
    1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
    1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
    1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
    1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
};

static const byte_t _rom_ s_bitsperbyte[256] =
{
    0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8,
};

static const byte_t _rom_ s_addressbits[256] =
{
    0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01,
    0x02, 0x02, 0x03, 0x03, 0x02, 0x02, 0x03, 0x03,
    0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01,
    0x02, 0x02, 0x03, 0x03, 0x02, 0x02, 0x03, 0x03,
    0x04, 0x04, 0x05, 0x05, 0x04, 0x04, 0x05, 0x05,
    0x06, 0x06, 0x07, 0x07, 0x06, 0x06, 0x07, 0x07,
    0x04, 0x04, 0x05, 0x05, 0x04, 0x04, 0x05, 0x05,
    0x06, 0x06, 0x07, 0x07, 0x06, 0x06, 0x07, 0x07,
    0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01,
    0x02, 0x02, 0x03, 0x03, 0x02, 0x02, 0x03, 0x03,
    0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01,
    0x02, 0x02, 0x03, 0x03, 0x02, 0x02, 0x03, 0x03,
    0x04, 0x04, 0x05, 0x05, 0x04, 0x04, 0x05, 0x05,
    0x06, 0x06, 0x07, 0x07, 0x06, 0x06, 0x07, 0x07,
    0x04, 0x04, 0x05, 0x05, 0x04, 0x04, 0x05, 0x05,
    0x06, 0x06, 0x07, 0x07, 0x06, 0x06, 0x07, 0x07,
    0x08, 0x08, 0x09, 0x09, 0x08, 0x08, 0x09, 0x09,
    0x0A, 0x0A, 0x0B, 0x0B, 0x0A, 0x0A, 0x0B, 0x0B,
    0x08, 0x08, 0x09, 0x09, 0x08, 0x08, 0x09, 0x09,
    0x0A, 0x0A, 0x0B, 0x0B, 0x0A, 0x0A, 0x0B, 0x0B,
    0x0C, 0x0C, 0x0D, 0x0D, 0x0C, 0x0C, 0x0D, 0x0D,
    0x0E, 0x0E, 0x0F, 0x0F, 0x0E, 0x0E, 0x0F, 0x0F,
    0x0C, 0x0C, 0x0D, 0x0D, 0x0C, 0x0C, 0x0D, 0x0D,
    0x0E, 0x0E, 0x0F, 0x0F, 0x0E, 0x0E, 0x0F, 0x0F,
    0x08, 0x08, 0x09, 0x09, 0x08, 0x08, 0x09, 0x09,
    0x0A, 0x0A, 0x0B, 0x0B, 0x0A, 0x0A, 0x0B, 0x0B,
    0x08, 0x08, 0x09, 0x09, 0x08, 0x08, 0x09, 0x09,
    0x0A, 0x0A, 0x0B, 0x0B, 0x0A, 0x0A, 0x0B, 0x0B,
    0x0C, 0x0C, 0x0D, 0x0D, 0x0C, 0x0C, 0x0D, 0x0D,
    0x0E, 0x0E, 0x0F, 0x0F, 0x0E, 0x0E, 0x0F, 0x0F,
    0x0C, 0x0C, 0x0D, 0x0D, 0x0C, 0x0C, 0x0D, 0x0D,
    0x0E, 0x0E, 0x0F, 0x0F, 0x0E, 0x0E, 0x0F, 0x0F,
};

/*
=======================================
    ECC-256/512 编码生成
=======================================
*/
CR_API bool_t
parity_ecc1_make (
  __CR_OT__ byte_t          parity[3],
  __CR_IN__ const void_t*   data,
  __CR_IN__ uint_t          size
    )
{
    int32u  cur, par, tmppar, *bp;
    int32u  rp0, rp1, rp2, rp3, rp4;
    int32u  rp5, rp6, rp7, rp8, rp9;
    int32u  rp10, rp11, rp12, rp13;
    int32u  rp14, rp15, rp16, rp17;
    uint_t  idx, bd, mult = size >> 8;

    /* 参数过滤 */
    if (mult != 1 && mult != 2)
        return (FALSE);
    bd = mult << 2;
    bp = (int32u*)data;
    par = rp4 = rp6 = rp8 = 0;
    rp10 = rp12 = rp14 = rp16 = rp17 = 0;

    /* 1位纠正, 2位检查 */
    for (idx = 0; idx < bd; idx++)
    {
        cur = *bp++;
        tmppar = cur;
        rp4 ^= cur;
        cur = *bp++;
        tmppar ^= cur;
        rp6 ^= tmppar;
        cur = *bp++;
        tmppar ^= cur;
        rp4 ^= cur;
        cur = *bp++;
        tmppar ^= cur;
        rp8 ^= tmppar;
        cur = *bp++;
        tmppar ^= cur;
        rp4 ^= cur;
        rp6 ^= cur;
        cur = *bp++;
        tmppar ^= cur;
        rp6 ^= cur;
        cur = *bp++;
        tmppar ^= cur;
        rp4 ^= cur;
        cur = *bp++;
        tmppar ^= cur;
        rp10 ^= tmppar;
        cur = *bp++;
        tmppar ^= cur;
        rp4 ^= cur;
        rp6 ^= cur;
        rp8 ^= cur;
        cur = *bp++;
        tmppar ^= cur;
        rp6 ^= cur;
        rp8 ^= cur;
        cur = *bp++;
        tmppar ^= cur;
        rp4 ^= cur;
        rp8 ^= cur;
        cur = *bp++;
        tmppar ^= cur;
        rp8 ^= cur;
        cur = *bp++;
        tmppar ^= cur;
        rp4 ^= cur;
        rp6 ^= cur;
        cur = *bp++;
        tmppar ^= cur;
        rp6 ^= cur;
        cur = *bp++;
        tmppar ^= cur;
        rp4 ^= cur;
        cur = *bp++;
        tmppar ^= cur;
        par ^= tmppar;
        if ((idx & 1) == 0)
            rp12 ^= tmppar;
        if ((idx & 2) == 0)
            rp14 ^= tmppar;
        if (mult == 2 &&
            (idx & 4) == 0)
            rp16 ^= tmppar;
    }
    rp4 ^= (rp4 >> 16);
    rp4 ^= (rp4 >> 8);
    rp4 &= 0xFF;
    rp6 ^= (rp6 >> 16);
    rp6 ^= (rp6 >> 8);
    rp6 &= 0xFF;
    rp8 ^= (rp8 >> 16);
    rp8 ^= (rp8 >> 8);
    rp8 &= 0xFF;
    rp10 ^= (rp10 >> 16);
    rp10 ^= (rp10 >> 8);
    rp10 &= 0xFF;
    rp12 ^= (rp12 >> 16);
    rp12 ^= (rp12 >> 8);
    rp12 &= 0xFF;
    rp14 ^= (rp14 >> 16);
    rp14 ^= (rp14 >> 8);
    rp14 &= 0xFF;
    if (mult == 2) {
        rp16 ^= (rp16 >> 16);
        rp16 ^= (rp16 >> 8);
        rp16 &= 0xFF;
    }
#ifndef _CR_ORDER_LE_
    rp2 = (par >> 16);
    rp2 ^= (rp2 >> 8);
    rp2 &= 0xFF;
    rp3 = par & 0xFFFF;
    rp3 ^= (rp3 >> 8);
    rp3 &= 0xFF;
#else
    rp3 = (par >> 16);
    rp3 ^= (rp3 >> 8);
    rp3 &= 0xFF;
    rp2 = par & 0xFFFF;
    rp2 ^= (rp2 >> 8);
    rp2 &= 0xFF;
#endif
    par ^= (par >> 16);
#ifndef _CR_ORDER_LE_
    rp0 = (par >> 8) & 0xFF;
    rp1 = (par & 0xFF);
#else
    rp1 = (par >> 8) & 0xFF;
    rp0 = (par & 0xFF);
#endif
    par ^= (par >> 8);
    par &= 0xFF;
    rp5 = (par ^ rp4) & 0xFF;
    rp7 = (par ^ rp6) & 0xFF;
    rp9 = (par ^ rp8) & 0xFF;
    rp11 = (par ^ rp10) & 0xFF;
    rp13 = (par ^ rp12) & 0xFF;
    rp15 = (par ^ rp14) & 0xFF;
    if (mult == 2)
        rp17 = (par ^ rp16) & 0xFF;

    /* 输出结果 */
    parity[0] = (s_invparity[rp7 ] << 7) |
                (s_invparity[rp6 ] << 6) |
                (s_invparity[rp5 ] << 5) |
                (s_invparity[rp4 ] << 4) |
                (s_invparity[rp3 ] << 3) |
                (s_invparity[rp2 ] << 2) |
                (s_invparity[rp1 ] << 1) |
                (s_invparity[rp0 ] << 0);
    parity[1] = (s_invparity[rp15] << 7) |
                (s_invparity[rp14] << 6) |
                (s_invparity[rp13] << 5) |
                (s_invparity[rp12] << 4) |
                (s_invparity[rp11] << 3) |
                (s_invparity[rp10] << 2) |
                (s_invparity[rp9 ] << 1) |
                (s_invparity[rp8 ] << 0);
    if (mult == 1) {
        parity[2] = (s_invparity[par & 0xF0] << 7) |
                    (s_invparity[par & 0x0F] << 6) |
                    (s_invparity[par & 0xCC] << 5) |
                    (s_invparity[par & 0x33] << 4) |
                    (s_invparity[par & 0xAA] << 3) |
                    (s_invparity[par & 0x55] << 2) | 3;
    }
    else {
        parity[2] = (s_invparity[par & 0xF0] << 7) |
                    (s_invparity[par & 0x0F] << 6) |
                    (s_invparity[par & 0xCC] << 5) |
                    (s_invparity[par & 0x33] << 4) |
                    (s_invparity[par & 0xAA] << 3) |
                    (s_invparity[par & 0x55] << 2) |
                    (s_invparity[   rp17   ] << 1) |
                    (s_invparity[   rp16   ] << 0);
    }
    return (TRUE);
}

/*
=======================================
    ECC-256/512 编码比较
=======================================
*/
CR_API sint_t
parity_ecc1_comp (
  __CR_IO__ void_t*         data,
  __CR_IN__ const byte_t    parity1[3],
  __CR_IN__ const byte_t    parity2[3],
  __CR_IN__ uint_t          size
    )
{
    byte_t  b0, b1, b2, bit_addr;
    uint_t  byte_addr, mult = size >> 8;

    /* 参数过滤 */
    if (mult != 1 && mult != 2)
        return (-2);
    b0 = parity1[0] ^ parity2[0];
    b1 = parity1[1] ^ parity2[1];
    b2 = parity1[2] ^ parity2[2];

    /* 无数据位出错 */
    if ((b0 | b1 | b2) == 0)
        return (0);

    /* 纠正一位错误或两位以上的错误 */
    if ((((b0 ^ (b0 >> 1)) & 0x55) == 0x55) &&
        (((b1 ^ (b1 >> 1)) & 0x55) == 0x55) &&
        ((mult == 1 && ((b2 ^ (b2 >> 1)) & 0x54) == 0x54) ||
         (mult == 2 && ((b2 ^ (b2 >> 1)) & 0x55) == 0x55)))
    {
        if (mult == 1) {
            byte_addr = (s_addressbits[b1] << 4) + s_addressbits[b0];
        }
        else {
            byte_addr = (s_addressbits[b2 & 0x3] << 8) +
                        (s_addressbits[b1] << 4) + s_addressbits[b0];
        }
        bit_addr = s_addressbits[b2 >> 2];
        ((byte_t*)data)[byte_addr] ^= (1 << bit_addr);
        return (1);
    }

    /* 1位 ECC 出错 */
    if ((s_bitsperbyte[b0] + s_bitsperbyte[b1] + s_bitsperbyte[b2]) == 1)
        return (-1);
    return (2);         /* 无法纠正的错误 (两位) */
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
