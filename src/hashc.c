/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2015-01-09  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack 通用 CRC 哈希函数库 <<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "hash.h"

/*
=======================================
    CRC-X 哈希
=======================================
*/
CR_API byte_t
hash_crc_x (
  __CR_IN__ byte_t          seed,
  __CR_IN__ byte_t          poly,
  __CR_IN__ bool_t          lsb,
  __CR_IN__ ufast_t         bits,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    if (bits >= 8)
        return (hash_crc8x(seed, poly, lsb, data, size));
    bits = 8 - bits;
    if (lsb) {
        seed &= (0xFF >> bits);
        poly &= (0xFF >> bits);
        return (hash_crc8x(seed, poly, TRUE, data, size));
    }
    seed <<= bits;
    poly <<= bits;
    seed = hash_crc8x(seed, poly, FALSE, data, size);
    return ((byte_t)(seed >> bits));
}

/*
=======================================
    CRC-8 哈希
=======================================
*/
CR_API byte_t
hash_crc8x (
  __CR_IN__ byte_t          seed,
  __CR_IN__ byte_t          poly,
  __CR_IN__ bool_t          lsb,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    ufast_t idx;

    if (lsb) {
        for (; size != 0; size--) {
            seed ^= *(uchar*)data;
            data = (uchar*)data + 1;
            for (idx = 8; idx != 0; idx--) {
                if (seed & 1) {
                    seed >>= 1;
                    seed ^= poly;
                }
                else {
                    seed >>= 1;
                }
            }
        }
    }
    else {
        for (; size != 0; size--) {
            seed ^= *(uchar*)data;
            data = (uchar*)data + 1;
            for (idx = 8; idx != 0; idx--) {
                if (seed & 0x80) {
                    seed <<= 1;
                    seed ^= poly;
                }
                else {
                    seed <<= 1;
                }
            }
        }
    }
    return (seed);
}

/*
=======================================
    CRC-16 哈希
=======================================
*/
CR_API int16u
hash_crc16x (
  __CR_IN__ int16u          seed,
  __CR_IN__ int16u          poly,
  __CR_IN__ bool_t          lsb,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    int16u  tmp;
    ufast_t idx;

    if (lsb) {
        for (; size != 0; size--) {
            tmp = *(uchar*)data;
            seed ^= tmp;
            data = (uchar*)data + 1;
            for (idx = 8; idx != 0; idx--) {
                if (seed & 1) {
                    seed >>= 1;
                    seed ^= poly;
                }
                else {
                    seed >>= 1;
                }
            }
        }
    }
    else {
        for (; size != 0; size--) {
            tmp = *(uchar*)data;
            tmp <<= 8;
            seed ^= tmp;
            data = (uchar*)data + 1;
            for (idx = 8; idx != 0; idx--) {
                if (((int16s)seed) < 0) {
                    seed <<= 1;
                    seed ^= poly;
                }
                else {
                    seed <<= 1;
                }
            }
        }
    }
    return (seed);
}

/*
=======================================
    CRC-32 哈希
=======================================
*/
CR_API int32u
hash_crc32x (
  __CR_IN__ int32u          seed,
  __CR_IN__ int32u          poly,
  __CR_IN__ bool_t          lsb,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    int32u  tmp;
    ufast_t idx;

    if (lsb) {
        for (; size != 0; size--) {
            tmp = *(uchar*)data;
            seed ^= tmp;
            data = (uchar*)data + 1;
            for (idx = 8; idx != 0; idx--) {
                if (seed & 1) {
                    seed >>= 1;
                    seed ^= poly;
                }
                else {
                    seed >>= 1;
                }
            }
        }
    }
    else {
        for (; size != 0; size--) {
            tmp = *(uchar*)data;
            tmp <<= 24;
            seed ^= tmp;
            data = (uchar*)data + 1;
            for (idx = 8; idx != 0; idx--) {
                if (((int32s)seed) < 0) {
                    seed <<= 1;
                    seed ^= poly;
                }
                else {
                    seed <<= 1;
                }
            }
        }
    }
    return (seed);
}

/*
=======================================
    CRC-64 哈希
=======================================
*/
CR_API int64u
hash_crc64x (
  __CR_IN__ int64u          seed,
  __CR_IN__ int64u          poly,
  __CR_IN__ bool_t          lsb,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    int64u  tmp;
    ufast_t idx;

    if (lsb) {
        for (; size != 0; size--) {
            tmp = *(uchar*)data;
            seed ^= tmp;
            data = (uchar*)data + 1;
            for (idx = 8; idx != 0; idx--) {
                if (seed & 1) {
                    seed >>= 1;
                    seed ^= poly;
                }
                else {
                    seed >>= 1;
                }
            }
        }
    }
    else {
        for (; size != 0; size--) {
            tmp = *(uchar*)data;
            tmp <<= 56;
            seed ^= tmp;
            data = (uchar*)data + 1;
            for (idx = 8; idx != 0; idx--) {
                if (((int64s)seed) < 0) {
                    seed <<= 1;
                    seed ^= poly;
                }
                else {
                    seed <<= 1;
                }
            }
        }
    }
    return (seed);
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
