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
/*  >>>>>>>>>>>>>>>>>> CrHack CRC-32/HONEYMAN 哈希函数库 <<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "hash.h"

/* CRC-32/HONEYMAN 常数表 (X^31 + X^3 + 1)
  (X^32 + X^7 + X^5 + X^3 + X^2 + X^1 + 1) */
static const int32u _rom_ s_crc32[128] =
{
    0x00000000UL, 0x48000000UL, 0x24000000UL, 0x6C000000UL,
    0x12000000UL, 0x5A000000UL, 0x36000000UL, 0x7E000000UL,
    0x09000000UL, 0x41000000UL, 0x2D000000UL, 0x65000000UL,
    0x1B000000UL, 0x53000000UL, 0x3F000000UL, 0x77000000UL,
    0x04800000UL, 0x4C800000UL, 0x20800000UL, 0x68800000UL,
    0x16800000UL, 0x5E800000UL, 0x32800000UL, 0x7A800000UL,
    0x0D800000UL, 0x45800000UL, 0x29800000UL, 0x61800000UL,
    0x1F800000UL, 0x57800000UL, 0x3B800000UL, 0x73800000UL,
    0x02400000UL, 0x4A400000UL, 0x26400000UL, 0x6E400000UL,
    0x10400000UL, 0x58400000UL, 0x34400000UL, 0x7C400000UL,
    0x0B400000UL, 0x43400000UL, 0x2F400000UL, 0x67400000UL,
    0x19400000UL, 0x51400000UL, 0x3D400000UL, 0x75400000UL,
    0x06C00000UL, 0x4EC00000UL, 0x22C00000UL, 0x6AC00000UL,
    0x14C00000UL, 0x5CC00000UL, 0x30C00000UL, 0x78C00000UL,
    0x0FC00000UL, 0x47C00000UL, 0x2BC00000UL, 0x63C00000UL,
    0x1DC00000UL, 0x55C00000UL, 0x39C00000UL, 0x71C00000UL,
    0x01200000UL, 0x49200000UL, 0x25200000UL, 0x6D200000UL,
    0x13200000UL, 0x5B200000UL, 0x37200000UL, 0x7F200000UL,
    0x08200000UL, 0x40200000UL, 0x2C200000UL, 0x64200000UL,
    0x1A200000UL, 0x52200000UL, 0x3E200000UL, 0x76200000UL,
    0x05A00000UL, 0x4DA00000UL, 0x21A00000UL, 0x69A00000UL,
    0x17A00000UL, 0x5FA00000UL, 0x33A00000UL, 0x7BA00000UL,
    0x0CA00000UL, 0x44A00000UL, 0x28A00000UL, 0x60A00000UL,
    0x1EA00000UL, 0x56A00000UL, 0x3AA00000UL, 0x72A00000UL,
    0x03600000UL, 0x4B600000UL, 0x27600000UL, 0x6F600000UL,
    0x11600000UL, 0x59600000UL, 0x35600000UL, 0x7D600000UL,
    0x0A600000UL, 0x42600000UL, 0x2E600000UL, 0x66600000UL,
    0x18600000UL, 0x50600000UL, 0x3C600000UL, 0x74600000UL,
    0x07E00000UL, 0x4FE00000UL, 0x23E00000UL, 0x6BE00000UL,
    0x15E00000UL, 0x5DE00000UL, 0x31E00000UL, 0x79E00000UL,
    0x0EE00000UL, 0x46E00000UL, 0x2AE00000UL, 0x62E00000UL,
    0x1CE00000UL, 0x54E00000UL, 0x38E00000UL, 0x70E00000UL,
};

#define HONEYMAN_INT_VALUE  0x00000000UL
#define HONEYMAN_XOR_VALUE  0x00000000UL

/*
=======================================
    CRC-32/HONEYMAN 初始化
=======================================
*/
CR_API int32u
hash_crc32h_init (void_t)
{
    return (HONEYMAN_INT_VALUE);
}

/*
=======================================
    CRC-32/HONEYMAN 计算数据块
=======================================
*/
CR_API int32u
hash_crc32h_update (
  __CR_IN__ int32u          hash,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    for (; size != 0; size--)
    {
        hash = s_crc32[(hash ^ *(uchar*)data) & 0x7F] ^ (hash >> 7);
        data = (const uchar*)data + 1;
    }
    return ((int32u)(hash));
}

/*
=======================================
    CRC-32/HONEYMAN 获取结果
=======================================
*/
CR_API int32u
hash_crc32h_finish (
  __CR_IN__ int32u  hash
    )
{
    return ((int32u)(hash ^ HONEYMAN_XOR_VALUE));
}

/*
=======================================
    CRC-32/HONEYMAN 一次性完成
=======================================
*/
CR_API int32u
hash_crc32h_total (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    int32u  hash = hash_crc32h_init();

    hash = hash_crc32h_update(hash, data, size);

    return (hash_crc32h_finish(hash));
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
