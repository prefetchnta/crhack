/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2013-03-20  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack 数据压缩助手函数库 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "enclib.h"
#include "memlib.h"

/*****************************************************************************/
/*                                   RLE                                     */
/*****************************************************************************/

/*
=======================================
    RLE 数据匹配 (字节)
=======================================
*/
CR_API leng_t
match_rle_1 (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size,
  __CR_IN__ leng_t          max_count
    )
{
    leng_t  len;

    if (size < sizeof(byte_t))
        return (0);
    len = 1;
    for (;;) {
        if (len >= max_count || len >= size ||
            ((byte_t*)data)[0] != ((byte_t*)data)[1])
            break;
        data = (byte_t*)data + sizeof(byte_t);
        len += 1;
    }
    return (len);
}

/*
=======================================
    RLE 数据匹配 (单字)
=======================================
*/
CR_API leng_t
match_rle_2 (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size,
  __CR_IN__ leng_t          max_count
    )
{
    leng_t  len;

    if (size < sizeof(int16u))
        return (0);
    len = 1;
    size /= sizeof(int16u);
    for (;;) {
        if (len >= max_count || len >= size ||
            ((int16u*)data)[0] != ((int16u*)data)[1])
            break;
        data = (byte_t*)data + sizeof(int16u);
        len += 1;
    }
    return (len);
}

/*
=======================================
    RLE 数据匹配 (双字)
=======================================
*/
CR_API leng_t
match_rle_4 (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size,
  __CR_IN__ leng_t          max_count
    )
{
    leng_t  len;

    if (size < sizeof(int32u))
        return (0);
    len = 1;
    size /= sizeof(int32u);
    for (;;) {
        if (len >= max_count || len >= size ||
            ((int32u*)data)[0] != ((int32u*)data)[1])
            break;
        data = (byte_t*)data + sizeof(int32u);
        len += 1;
    }
    return (len);
}

/*
=======================================
    RLE 数据匹配 (多字节)
=======================================
*/
CR_API leng_t
match_rle_x (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size,
  __CR_IN__ leng_t          unit,
  __CR_IN__ leng_t          max_count
    )
{
    leng_t  len;

    switch (unit)
    {
        default:
            break;
        case 0: return (0);
        case 1: return (match_rle_1(data, size, max_count));
        case 2: return (match_rle_2(data, size, max_count));
        case 4: return (match_rle_4(data, size, max_count));
    }
    if (size < unit)
        return (0);
    len = 1;
    size /= unit;
    for (;;) {
        if (len >= max_count || len >= size ||
            mem_cmp(data, (byte_t*)data + unit, unit) != 0)
            break;
        data = (byte_t*)data + unit;
        len += 1;
    }
    return (len);
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
