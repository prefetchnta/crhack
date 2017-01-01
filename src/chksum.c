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
/*  >>>>>>>>>>>>>>>>>>>>>>> CrHack 和校验哈希函数库 <<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "hash.h"

/*****************************************************************************/
/*                                  XOR-8                                    */
/*****************************************************************************/

/*
=======================================
    XOR-8 初始化
=======================================
*/
CR_API byte_t
hash_xor8_init (void_t)
{
    return (0x00);
}

/*
=======================================
    XOR-8 计算数据块
=======================================
*/
CR_API byte_t
hash_xor8_update (
  __CR_IN__ byte_t          hash,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    for (; size != 0; size--)
    {
        hash = *(uchar*)data ^ hash;
        data =  (uchar*)data + 1;
    }
    return (hash);
}

/*
=======================================
    XOR-8 获取结果
=======================================
*/
CR_API byte_t
hash_xor8_finish (
  __CR_IN__ byte_t  hash
    )
{
    return (hash);
}

/*
=======================================
    XOR-8 一次性完成
=======================================
*/
CR_API byte_t
hash_xor8_total (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    byte_t  hash = hash_xor8_init();

    hash = hash_xor8_update(hash, data, size);

    return (hash_xor8_finish(hash));
}

/*****************************************************************************/
/*                                CheckSum32                                 */
/*****************************************************************************/

/*
=======================================
    CheckSum32 初始化
=======================================
*/
CR_API int32u
hash_sum32_init (void_t)
{
    return (0x00000000UL);
}

/*
=======================================
    CheckSum32 计算数据块
=======================================
*/
CR_API int32u
hash_sum32_update (
  __CR_IN__ int32u          hash,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    for (; size != 0; size--)
    {
        hash = *(uchar*)data + hash;
        data =  (uchar*)data + 1;
    }
    return (hash);
}

/*
=======================================
    CheckSum32 获取结果
=======================================
*/
CR_API int32u
hash_sum32_finish (
  __CR_IN__ int32u  hash
    )
{
    return (hash);
}

/*
=======================================
    CheckSum32 一次性完成
=======================================
*/
CR_API int32u
hash_sum32_total (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    int32u  hash = hash_sum32_init();

    hash = hash_sum32_update(hash, data, size);

    return (hash_sum32_finish(hash));
}

/*****************************************************************************/
/*                                CheckSum64                                 */
/*****************************************************************************/

/*
=======================================
    CheckSum64 初始化
=======================================
*/
CR_API int64u
hash_sum64_init (void_t)
{
    return (CR_ULL(0x0000000000000000));
}

/*
=======================================
    CheckSum64 计算数据块
=======================================
*/
CR_API int64u
hash_sum64_update (
  __CR_IN__ int64u          hash,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    for (; size != 0; size--)
    {
        hash = *(uchar*)data + hash;
        data =  (uchar*)data + 1;
    }
    return (hash);
}

/*
=======================================
    CheckSum64 获取结果
=======================================
*/
CR_API int64u
hash_sum64_finish (
  __CR_IN__ int64u  hash
    )
{
    return (hash);
}

/*
=======================================
    CheckSum64 一次性完成
=======================================
*/
CR_API int64u
hash_sum64_total (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    int64u  hash = hash_sum64_init();

    hash = hash_sum64_update(hash, data, size);

    return (hash_sum64_finish(hash));
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
