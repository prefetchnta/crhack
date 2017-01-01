/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2011-06-08  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack ECC-512 编码函数库 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "hash.h"

/*
=======================================
    ECC-512 编码生成
=======================================
*/
CR_API void_t
par512_ecc1_make (
  __CR_OT__ byte_t          parity[3],
  __CR_IN__ const byte_t    data[512]
    )
{
    int32u  parity_col;
    int32u  xorbit = 0;
    int32u  uiparity = 0;
    int32u  idx, tmp, ecc = 0;
    int32u  parity_col_tot = 0;
    int32u  parity_col4321 = 0;
    int32u  parity_col4343 = 0;
    int32u  parity_col4242 = 0;
    int32u* ptr = (int32u*)data;

    /* 1位纠正, 2位检查 */
    for (idx = 0; idx < 16; idx++)
    {
        parity_col = *ptr++;
        tmp = *ptr++;
        parity_col     ^= tmp;
        parity_col4242 ^= tmp;
        tmp = *ptr++;
        parity_col     ^= tmp;
        parity_col4343 ^= tmp;
        tmp = *ptr++;
        parity_col     ^= tmp;
        parity_col4343 ^= tmp;
        parity_col4242 ^= tmp;
        tmp = *ptr++;
        parity_col     ^= tmp;
        parity_col4321 ^= tmp;
        tmp = *ptr++;
        parity_col     ^= tmp;
        parity_col4242 ^= tmp;
        parity_col4321 ^= tmp;
        tmp = *ptr++;
        parity_col     ^= tmp;
        parity_col4343 ^= tmp;
        parity_col4321 ^= tmp;
        tmp = *ptr++;
        parity_col     ^= tmp;
        parity_col4242 ^= tmp;
        parity_col4343 ^= tmp;
        parity_col4321 ^= tmp;
        parity_col_tot ^= parity_col;
        tmp = (parity_col >> 16) ^ parity_col;
        tmp = ((tmp >> 8) ^ tmp);
        tmp = ((tmp >> 4) ^ tmp);
        tmp = ((tmp >> 2) ^ tmp) & 0x03;
        if (tmp == 0x01 || tmp == 0x02) {
            uiparity ^= idx;
            xorbit ^= 0x01;
        }
    }
    tmp  = (parity_col4321 >> 16) ^
                     parity_col4321;
    tmp  = ((tmp <<  8) ^ tmp);
    tmp  = ((tmp >>  4) ^ tmp);
    tmp  = ((tmp >>  2) ^ tmp);
    ecc |= ((tmp <<  1) ^ tmp) & 0x200UL;
    tmp  = (parity_col4343 >> 16) ^
                    parity_col4343;
    tmp  = ((tmp >>  8) ^ tmp);
    tmp  = ((tmp <<  4) ^ tmp);
    tmp  = ((tmp <<  2) ^ tmp);
    ecc |= ((tmp <<  1) ^ tmp) & 0x80UL;
    tmp  = (parity_col4242 >> 16) ^
                    parity_col4242;
    tmp  = ((tmp >>  8) ^ tmp);
    tmp  = ((tmp <<  4) ^ tmp);
    tmp  = ((tmp >>  2) ^ tmp);
    ecc |= ((tmp <<  1) ^ tmp) & 0x20UL;
    tmp  = parity_col_tot & 0xFFFF0000UL;
    tmp  = ((tmp >> 16)      );
    tmp  = ((tmp >>  8) ^ tmp);
    tmp  = ((tmp >>  4) ^ tmp);
    tmp  = ((tmp <<  2) ^ tmp);
    ecc |= ((tmp <<  1) ^ tmp) & 0x08UL;
    tmp  = parity_col_tot & 0xFF00FF00UL;
    tmp  = ((tmp >> 16) ^ tmp);
    tmp  = ((tmp >>  8)      );
    tmp  = ((tmp >>  4) ^ tmp);
    tmp  = ((tmp >>  2) ^ tmp);
    ecc |= ((tmp <<  1) ^ tmp) & 0x02UL;
    tmp  = parity_col_tot & 0xF0F0F0F0UL;
    tmp  = ((tmp << 16) ^ tmp);
    tmp  = ((tmp >>  8) ^ tmp);
    tmp  = ((tmp <<  2) ^ tmp);
    ecc |= ((tmp <<  1) ^ tmp) & 0x800000UL;
    tmp  = parity_col_tot & 0xCCCCCCCCUL;
    tmp  = ((tmp << 16) ^ tmp);
    tmp  = ((tmp >>  8) ^ tmp);
    tmp  = ((tmp <<  4) ^ tmp);
    tmp  = ((tmp >>  2)      );
    ecc |= ((tmp <<  1) ^ tmp) & 0x200000UL;
    tmp  = parity_col_tot & 0xAAAAAAAAUL;
    tmp  = ((tmp << 16) ^ tmp);
    tmp  = ((tmp >>  8) ^ tmp);
    tmp  = ((tmp >>  4) ^ tmp);
    tmp  = ((tmp <<  2) ^ tmp);
    ecc |= ((tmp <<  0)      ) & 0x80000UL;
    ecc |= (uiparity & 0x01) << 11;
    ecc |= (uiparity & 0x02) << 12;
    ecc |= (uiparity & 0x04) << 13;
    ecc |= (uiparity & 0x08) << 14;
    if (xorbit)
        ecc |= (ecc ^ 0x00AAAAAAUL) >> 1;
    else
        ecc |= (ecc >> 1);
    ecc = ~ecc;
    parity[2] = (byte_t)(ecc >> 16);
    parity[1] = (byte_t)(ecc >>  8);
    parity[0] = (byte_t)(ecc >>  0);
}

/*
=======================================
    ECC-512 编码比较
=======================================
*/
CR_API sint_t
par512_ecc1_comp (
  __CR_IO__ byte_t          data[512],
  __CR_IN__ const byte_t    parity1[3],
  __CR_IN__ const byte_t    parity2[3]
    )
{
    uint_t  index;
    int32u  t1 = 0;
    int32u  t2 = 0;
    byte_t  find_bt;
    int32u  find_by;
    int32u  ecc_cmp = 0;
    int32u  ecc_sum = 0;
    byte_t* ecc1 = (byte_t*)parity1;
    byte_t* ecc2 = (byte_t*)parity2;

    for (index = 0; index <  2; index++) {
        t1 ^= (((*ecc1) >> index) & 0x01);
        t2 ^= (((*ecc2) >> index) & 0x01);
    }
    for (index = 0; index <  3; index++)
        ecc_cmp |= ((~(*ecc1++) ^ ~(*ecc2++)) << (index * 8));
    for (index = 0; index < 24; index++)
        ecc_sum += ((ecc_cmp >> index) & 0x01);

    switch (ecc_sum)
    {
        default:
            break;

        case 0:     /* 无数据位出错 */
            return (0);

        case 1:     /* 1位 ECC 出错 */
            return (-1);

        case 12:    /* 纠正一位错误或两位以上的错误 */
            if (t1 == t2)
                break;
            find_by = (((ecc_cmp >> 17) & 1) << 8) +
                      (((ecc_cmp >> 15) & 1) << 7) +
                      (((ecc_cmp >> 13) & 1) << 6) +
                      (((ecc_cmp >> 11) & 1) << 5) +
                      (((ecc_cmp >>  9) & 1) << 4) +
                      (((ecc_cmp >>  7) & 1) << 3) +
                      (((ecc_cmp >>  5) & 1) << 2) +
                      (((ecc_cmp >>  3) & 1) << 1) +
                      (((ecc_cmp >>  1) & 1) << 0);
            find_bt = (byte_t)((((ecc_cmp >> 23) & 1) << 2) +
                               (((ecc_cmp >> 21) & 1) << 1) +
                                ((ecc_cmp >> 19) & 1));
            data[find_by] = (byte_t)(data[find_by] ^ (1 << find_bt));
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
