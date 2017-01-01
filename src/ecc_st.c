/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2012-02-07  */
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
/*  >>>>>>>>>>>>>>>>>>> CrHack STM32 硬件 ECC 纠错函数库 <<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "hash.h"

/* 有用的查表 */
static const sint_t _rom_ s_ecc_bits[] =
{
    22,             /*  256 / 512 = 0 */
    24,             /*  512 / 512 = 1 */
    26,             /* 1024 / 512 = 2 */
    -1,             /* XXXXXXXXXX = 3 */
    28,             /* 2048 / 512 = 4 */
    -1,             /* XXXXXXXXXX = 5 */
    -1,             /* XXXXXXXXXX = 6 */
    -1,             /* XXXXXXXXXX = 7 */
    30,             /* 4096 / 512 = 8 */
    -1,             /* XXXXXXXXXX = 9 */
    -1,             /* XXXXXXXXXX = 10 */
    -1,             /* XXXXXXXXXX = 11 */
    -1,             /* XXXXXXXXXX = 12 */
    -1,             /* XXXXXXXXXX = 13 */
    -1,             /* XXXXXXXXXX = 14 */
    -1,             /* XXXXXXXXXX = 15 */
    32,             /* 8192 / 512 = 16 */
};

static const int32u _rom_ s_ecc_mask[] =
{
    0x003FFFFFUL,   /*  256 / 512 = 0 */
    0x00FFFFFFUL,   /*  512 / 512 = 1 */
    0x03FFFFFFUL,   /* 1024 / 512 = 2 */
    0x00UL,         /* XXXXXXXXXX = 3 */
    0x0FFFFFFFUL,   /* 2048 / 512 = 4 */
    0x00UL,         /* XXXXXXXXXX = 5 */
    0x00UL,         /* XXXXXXXXXX = 6 */
    0x00UL,         /* XXXXXXXXXX = 7 */
    0x3FFFFFFFUL,   /* 4096 / 512 = 8 */
    0x00UL,         /* XXXXXXXXXX = 9 */
    0x00UL,         /* XXXXXXXXXX = 10 */
    0x00UL,         /* XXXXXXXXXX = 11 */
    0x00UL,         /* XXXXXXXXXX = 12 */
    0x00UL,         /* XXXXXXXXXX = 13 */
    0x00UL,         /* XXXXXXXXXX = 14 */
    0x00UL,         /* XXXXXXXXXX = 15 */
    0xFFFFFFFFUL,   /* 8192 / 512 = 16 */
};

/*
=======================================
    STM32 硬件 ECC 编码比较
=======================================
*/
CR_API sint_t
parity_ecc1_comp_st (
  __CR_IO__ void_t* data,
  __CR_IN__ int32u  parity1,
  __CR_IN__ int32u  parity2,
  __CR_IN__ uint_t  size
    )
{
    int32u  eccxor;
    uint_t  dat[4];
    uint_t  maxbit;
    uint_t  bit_addr;
    uint_t  idx, cnt1;
    uint_t  byte_addr;

    /* 过滤参数 */
    if ((size > 8192) ||
        (size != 256 && (size % 512) != 0))
        return (-2);
    maxbit = s_ecc_bits[size / 512];
    if (maxbit > 32)
        return (-2);
    eccxor = s_ecc_mask[size / 512];

    /* 无数据位出错 */
    eccxor &= (parity1 ^ parity2);
    if (eccxor == 0UL)
        return (0);

    /* 1位 ECC 出错 */
    for (cnt1 = 0, idx = 0; idx < maxbit; idx++) {
        if ((eccxor & (1UL << idx)) != 0)
            cnt1 += 1;
    }
    if (cnt1 == 1)
        return (-1);

    /* 纠正一位错误或两位以上的错误 */
    if (cnt1 == maxbit / 2)
    {
        dat[0] = (uint_t)((eccxor >>  0) & 0xFF);
        dat[1] = (uint_t)((eccxor >>  8) & 0xFF);
        dat[2] = (uint_t)((eccxor >> 16) & 0xFF);
        dat[3] = (uint_t)((eccxor >> 24) & 0xFF);

        byte_addr = ((dat[0] >> 7) & 0x0001) |  /* P8 */
                    ((dat[1] >> 0) & 0x0002) |  /* P16 */
                    ((dat[1] >> 1) & 0x0004) |  /* P32 */
                    ((dat[1] >> 2) & 0x0008) |  /* P64 */
                    ((dat[1] >> 3) & 0x0010) |  /* P128 */
                    ((dat[2] << 4) & 0x0020) |  /* P256 */
                    ((dat[2] << 3) & 0x0040) |  /* P512 */
                    ((dat[2] << 2) & 0x0080) |  /* P1024 */
                    ((dat[2] << 1) & 0x0100) |  /* P2048 */
                    ((dat[3] << 8) & 0x0200) |  /* P4096 */
                    ((dat[3] << 7) & 0x0400) |  /* P8192 */
                    ((dat[3] << 6) & 0x0800) |  /* P16384 */
                    ((dat[3] << 5) & 0x1000);   /* P32768 */

        if (byte_addr < size) {
            bit_addr = ((dat[0] >> 1) & 1) |    /* P1 */
                       ((dat[0] >> 2) & 2) |    /* P2 */
                       ((dat[0] >> 3) & 4);     /* P4 */
            ((byte_t*)data)[byte_addr] ^= (1 << bit_addr);
        }
        return (1);
    }
    return (2);     /* 无法纠正的错误 (两位) */
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
