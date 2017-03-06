/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2017-03-04  */
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
/*  >>>>>>>>>>>>>>>>>>>> CrHack CRC-16/CCITT 哈希函数库 <<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "hash.h"

/* CRC-16/CCITT 常数表 (X^16 + X^12 + X^5 + 1) */
static const int16u _rom_ s_crc16[16] =
{
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
    0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
};

#define CRC16_INT_VALUE     0xFFFF
#define CRC16_XOR_VALUE     0x0000

#if !defined(__no_hash_crc16h_init)
/*
=======================================
    CRC-16/CCITT 初始化
=======================================
*/
CR_API int16u
hash_crc16h_init (void_t)
{
    return (CRC16_INT_VALUE);
}

#endif  /* !__no_hash_crc16h_init */

#if !defined(__no_hash_crc16h_update)
/*
=======================================
    CRC-16/CCITT 计算数据块
=======================================
*/
CR_API int16u
hash_crc16h_update (
  __CR_IN__ int16u          hash,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    int16u  temp;

    for (; size != 0; size--)
    {
        temp = ((uchar)(hash >> 8)) >> 4;
        hash <<= 4;
        hash ^= s_crc16[temp ^ (*((uchar*)data) >> 4)];
        temp = ((uchar)(hash >> 8)) >> 4;
        hash <<= 4;
        hash ^= s_crc16[temp ^ (*((uchar*)data) & 0x0F)];
        data = (const uchar*)data + 1;
    }
    return ((int16u)(hash));
}

#endif  /* !__no_hash_crc16h_update */

#if !defined(__no_hash_crc16h_finish)
/*
=======================================
    CRC-16/CCITT 获取结果
=======================================
*/
CR_API int16u
hash_crc16h_finish (
  __CR_IN__ int16u  hash
    )
{
    return ((int16u)(hash ^ CRC16_XOR_VALUE));
}

#endif  /* !__no_hash_crc16h_finish */

#if !defined(__no_hash_crc16h_total)
/*
=======================================
    CRC-16/CCITT 一次性完成
=======================================
*/
CR_API int16u
hash_crc16h_total (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    int16u  hash = hash_crc16h_init();

    hash = hash_crc16h_update(hash, data, size);

    return (hash_crc16h_finish(hash));
}

#endif  /* !__no_hash_crc16h_total */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
