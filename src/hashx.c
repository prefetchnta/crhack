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

#define CR_HASHSTR_GETSTEP \
            leng_t  ptr, step = 1; \
            if (size > _CR_HASHSTR_X_) \
                step = size / _CR_HASHSTR_X_ + 1;

#if !defined(__no_hash_str_apx)
/*
=======================================
    AP 哈希
=======================================
*/
CR_API uint_t
hash_str_apx (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    uint_t  idx = 0;
    int32u  hash = 0xAAAAAAAAUL;

    CR_HASHSTR_GETSTEP

    for (ptr = 0; ptr < size; ptr += step)
    {
        if (((idx++) & 1) != 0)
            hash ^= ~(*(uchar*)data ^ (hash << 11) ^ (hash >> 5));
        else
            hash ^=  (*(uchar*)data ^ (hash <<  7) ^ (hash >> 3));
        data = (const uchar*)data + step;
    }
    return ((uint_t)hash);
}

#endif  /* !__no_hash_str_apx */

#if !defined(__no_hash_str_bpx)
/*
=======================================
    BP 哈希
=======================================
*/
CR_API uint_t
hash_str_bpx (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    int32u  hash = 0UL;

    CR_HASHSTR_GETSTEP

    for (ptr = 0; ptr < size; ptr += step)
    {
        hash = *(uchar*)data ^ (hash << 7);
        data =  (uchar*)data + step;
    }
    return ((uint_t)hash);
}

#endif  /* !__no_hash_str_bpx */

#if !defined(__no_hash_str_jsx)
/*
=======================================
    JS 哈希
=======================================
*/
CR_API uint_t
hash_str_jsx (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    int32u  hash = 1315423911UL;

    CR_HASHSTR_GETSTEP

    for (ptr = 0; ptr < size; ptr += step)
    {
        hash ^= *(uchar*)data + (hash << 5) + (hash >> 2);
        data  =  (uchar*)data + step;
    }
    return ((uint_t)hash);
}

#endif  /* !__no_hash_str_jsx */

#if !defined(__no_hash_str_rsx)
/*
=======================================
    RS 哈希
=======================================
*/
CR_API uint_t
hash_str_rsx (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    int32u  hash = 0UL;
    int32u  aa = 63689UL;
    int32u  bb = 378551UL;

    CR_HASHSTR_GETSTEP

    for (ptr = 0; ptr < size; ptr += step)
    {
        hash = *(uchar*)data + aa * hash;
        data =  (uchar*)data + step;
        aa *= bb;
    }
    return ((uint_t)hash);
}

#endif  /* !__no_hash_str_rsx */

#if !defined(__no_hash_str_elfx)
/*
=======================================
    ELF 哈希
=======================================
*/
CR_API uint_t
hash_str_elfx (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    int32u  aa, hash = 0UL;

    CR_HASHSTR_GETSTEP

    for (ptr = 0; ptr < size; ptr += step)
    {
        hash = *(uchar*)data + (hash << 4);
        data =  (uchar*)data + step;

        aa = hash & 0xF0000000UL;
        if (aa != 0)
        {
            hash ^= (aa >> 24);
            hash &= ~aa;
        }
    }
    return ((uint_t)hash);
}

#endif  /* !__no_hash_str_elfx */

#if !defined(__no_hash_str_dekx)
/*
=======================================
    DEK 哈希
=======================================
*/
CR_API uint_t
hash_str_dekx (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    int32u  hash = (int32u)size;

    CR_HASHSTR_GETSTEP

    for (ptr = 0; ptr < size; ptr += step)
    {
        hash = *(uchar*)data ^ (hash << 5) ^ (hash >> 27);
        data =  (uchar*)data + step;
    }
    return ((uint_t)hash);
}

#endif  /* !__no_hash_str_dekx */

#if !defined(__no_hash_str_fnvx)
/*
=======================================
    FNV 哈希
=======================================
*/
CR_API uint_t
hash_str_fnvx (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    int32u  hash = 0UL;

    CR_HASHSTR_GETSTEP

    for (ptr = 0; ptr < size; ptr += step)
    {
        hash *= 0x811C9DC5UL;
        hash = *(uchar*)data ^ hash;
        data =  (uchar*)data + step;
    }
    return ((uint_t)hash);
}

#endif  /* !__no_hash_str_fnvx */

#if !defined(__no_hash_str_djb2x)
/*
=======================================
    DJB2 哈希
=======================================
*/
CR_API uint_t
hash_str_djb2x (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    int32u  hash = 5381UL;

    CR_HASHSTR_GETSTEP

    for (ptr = 0; ptr < size; ptr += step)
    {
        hash = *(uchar*)data + (hash << 5) + hash;
        data =  (uchar*)data + step;
    }
    return ((uint_t)hash);
}

#endif  /* !__no_hash_str_djb2x */

#if !defined(__no_hash_str_sdbmx)
/*
=======================================
    SDBM 哈希
=======================================
*/
CR_API uint_t
hash_str_sdbmx (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    int32u  hash = 0UL;

    CR_HASHSTR_GETSTEP

    for (ptr = 0; ptr < size; ptr += step)
    {
        hash = *(uchar*)data + (hash << 6) + (hash << 16) - hash;
        data =  (uchar*)data + step;
    }
    return ((uint_t)hash);
}

#endif  /* !__no_hash_str_sdbmx */

#if !defined(__no_hash_str_bkdrx)
/*
=======================================
    BKDR 哈希
=======================================
*/
CR_API uint_t
hash_str_bkdrx (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    int32u  hash = 0UL;
    int32u  seed = 131UL;   /* 31, 131, 1313, 13131, 131313... */

    CR_HASHSTR_GETSTEP

    for (ptr = 0; ptr < size; ptr += step)
    {
        hash = *(uchar*)data + hash * seed;
        data =  (uchar*)data + step;
    }
    return ((uint_t)hash);
}

#endif  /* !__no_hash_str_bkdrx */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
