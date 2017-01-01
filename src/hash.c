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
/*  >>>>>>>>>>>>>>>>>>>>> CrHack 常用字符串哈希函数库 <<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "hash.h"

#if !defined(__no_hash_count)

/* 哈希素数表 */
static const leng_t _rom_ s_primes[] =
{
    17, 37, 53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593,

#ifndef _CR_SYS16_
    49157, 98317, 196613, 393241, 786433, 1572869, 3145739, 6291469,
/*  12582917, 25165843, 50331653, 100663319, 201326611, 402653189,
    805306457, 1610612741,  */
#endif
};

/*
=======================================
    获取哈希表大小
=======================================
*/
CR_API uint_t
hash_count (
  __CR_IN__ leng_t  count
    )
{
    uint_t  idx, len = cntsof(s_primes);

    for (idx = 0; idx < len; idx++)
    {
        if (count <= s_primes[idx])
            break;
    }

    if (idx != len)
        return ((uint_t)s_primes[idx]);
    return ((uint_t)s_primes[len-1]);
}

#endif  /* !__no_hash_count */

#if !defined(__no_hash_str_ap)
/*
=======================================
    AP 哈希
=======================================
*/
CR_API uint_t
hash_str_ap (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    uint_t  idx = 0;
    int32u  hash = 0xAAAAAAAAUL;

    for (; size != 0; size--, idx++)
    {
        if ((idx & 1) != 0)
            hash ^= ~(*(uchar*)data ^ (hash << 11) ^ (hash >> 5));
        else
            hash ^=  (*(uchar*)data ^ (hash <<  7) ^ (hash >> 3));
        data = (const uchar*)data + 1;
    }
    return ((uint_t)hash);
}

#endif  /* !__no_hash_str_ap */

#if !defined(__no_hash_str_bp)
/*
=======================================
    BP 哈希
=======================================
*/
CR_API uint_t
hash_str_bp (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    int32u  hash = 0UL;

    for (; size != 0; size--)
    {
        hash = *(uchar*)data ^ (hash << 7);
        data =  (uchar*)data + 1;
    }
    return ((uint_t)hash);
}

#endif  /* !__no_hash_str_bp */

#if !defined(__no_hash_str_js)
/*
=======================================
    JS 哈希
=======================================
*/
CR_API uint_t
hash_str_js (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    int32u  hash = 1315423911UL;

    for (; size != 0; size--)
    {
        hash ^= *(uchar*)data + (hash << 5) + (hash >> 2);
        data  =  (uchar*)data + 1;
    }
    return ((uint_t)hash);
}

#endif  /* !__no_hash_str_js */

#if !defined(__no_hash_str_rs)
/*
=======================================
    RS 哈希
=======================================
*/
CR_API uint_t
hash_str_rs (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    int32u  hash = 0UL;
    int32u  aa = 63689UL;
    int32u  bb = 378551UL;

    for (; size != 0; size--)
    {
        hash = *(uchar*)data + aa * hash;
        data =  (uchar*)data + 1;
        aa *= bb;
    }
    return ((uint_t)hash);
}

#endif  /* !__no_hash_str_rs */

#if !defined(__no_hash_str_elf)
/*
=======================================
    ELF 哈希
=======================================
*/
CR_API uint_t
hash_str_elf (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    int32u  aa, hash = 0UL;

    for (; size != 0; size--)
    {
        hash = *(uchar*)data + (hash << 4);
        data =  (uchar*)data + 1;

        aa = hash & 0xF0000000UL;
        if (aa != 0)
        {
            hash ^= (aa >> 24);
            hash &= ~aa;
        }
    }
    return ((uint_t)hash);
}

#endif  /* !__no_hash_str_elf */

#if !defined(__no_hash_str_dek)
/*
=======================================
    DEK 哈希
=======================================
*/
CR_API uint_t
hash_str_dek (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    int32u  hash = (int32u)size;

    for (; size != 0; size--)
    {
        hash = *(uchar*)data ^ (hash << 5) ^ (hash >> 27);
        data =  (uchar*)data + 1;
    }
    return ((uint_t)hash);
}

#endif  /* !__no_hash_str_dek */

#if !defined(__no_hash_str_fnv)
/*
=======================================
    FNV 哈希
=======================================
*/
CR_API uint_t
hash_str_fnv (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    int32u  hash = 0UL;

    for (; size != 0; size--)
    {
        hash *= 0x811C9DC5UL;
        hash = *(uchar*)data ^ hash;
        data =  (uchar*)data + 1;
    }
    return ((uint_t)hash);
}

#endif  /* !__no_hash_str_fnv */

#if !defined(__no_hash_str_djb2)
/*
=======================================
    DJB2 哈希
=======================================
*/
CR_API uint_t
hash_str_djb2 (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    int32u  hash = 5381UL;

    for (; size != 0; size--)
    {
        hash = *(uchar*)data + (hash << 5) + hash;
        data =  (uchar*)data + 1;
    }
    return ((uint_t)hash);
}

#endif  /* !__no_hash_str_djb2 */

#if !defined(__no_hash_str_sdbm)
/*
=======================================
    SDBM 哈希
=======================================
*/
CR_API uint_t
hash_str_sdbm (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    int32u  hash = 0UL;

    for (; size != 0; size--)
    {
        hash = *(uchar*)data + (hash << 6) + (hash << 16) - hash;
        data =  (uchar*)data + 1;
    }
    return ((uint_t)hash);
}

#endif  /* !__no_hash_str_sdbm */

#if !defined(__no_hash_str_bkdr)
/*
=======================================
    BKDR 哈希
=======================================
*/
CR_API uint_t
hash_str_bkdr (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    int32u  hash = 0UL;
    int32u  seed = 131UL;   /* 31, 131, 1313, 13131, 131313... */

    for (; size != 0; size--)
    {
        hash = *(uchar*)data + hash * seed;
        data =  (uchar*)data + 1;
    }
    return ((uint_t)hash);
}

#endif  /* !__no_hash_str_bkdr */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
