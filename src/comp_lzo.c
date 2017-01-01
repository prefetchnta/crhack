/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-01-13  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack LZO 压缩解压函数库 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "enclib.h"
#include "memlib.h"
#include "ex3rd/minilzo/minilzo.h"

/*
=======================================
    lzo1x_1 压缩
=======================================
*/
CR_API leng_t
compr_lzo (
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen
    )
{
    sint_t      result;
    byte_t*     wrkmem;
    lzo_uint    dst_size;

    if (dst == NULL)
        return (srclen + srclen / 16 + 64 + 3);

    if (lzo_init() != LZO_E_OK)
        return (0);

    wrkmem = (byte_t*)mem_malloc(LZO1X_1_MEM_COMPRESS);
    if (wrkmem == NULL)
        return (0);

    dst_size = dstlen;
    result = lzo1x_1_compress((const lzo_bytep)src, (lzo_uint)srclen,
                        (lzo_bytep)dst, (lzo_uintp)(&dst_size), wrkmem);
    mem_free(wrkmem);
    if (result != LZO_E_OK)
        return (0);
    return (dst_size);
}

/*
=======================================
    lzo1x 解压
=======================================
*/
CR_API leng_t
uncompr_lzo (
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen
    )
{
    lzo_uint    dst_size;

    if (lzo_init() != LZO_E_OK)
        return (0);

    dst_size = dstlen;
    if (lzo1x_decompress_safe((const lzo_bytep)src, (lzo_uint)srclen,
            (lzo_bytep)dst, (lzo_uintp)(&dst_size), NULL) != LZO_E_OK)
        return (0);
    return (dst_size);
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
