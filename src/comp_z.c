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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack ZLIB 压缩解压函数库 <<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "safe.h"
#include "enclib.h"
#include "ex3rd/zlib/zlib.h"

/* Z_SOLO 需要自己声明函数 */
uLong ZEXPORT compressBound (uLong sourceLen);
int ZEXPORT compress2 (Bytef *dest, uIntf *destLen,
                       const Bytef *source, uInt sourceLen, int level);
int ZEXPORT uncompress (Bytef *dest, uIntf *destLen,
                        const Bytef *source, uInt sourceLen);
int ZEXPORT compress2_raw (Bytef *dest, uIntf *destLen,
                           const Bytef *source, uInt sourceLen, int level);
int ZEXPORT uncompress_raw (Bytef *dest, uIntf *destLen,
                            const Bytef *source, uInt sourceLen);
/*
=======================================
    ZLib 压缩
=======================================
*/
CR_API leng_t
compr_zlib (
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen,
  __CR_IN__ uint_t          level
    )
{
    uint_t  ssize, dsize;

    if (cut_size(&ssize, srclen))
        return (0);

    if (dst == NULL)
        return (compressBound(ssize));

    if (cut_size(&dsize, dstlen))
        return (0);

    if (compress2((Bytef*)dst, (uIntf*)(&dsize),
            (const Bytef*)src, ssize, level) != Z_OK)
        return (0);
    return (dsize);
}

/*
=======================================
    ZLib 解压
=======================================
*/
CR_API leng_t
uncompr_zlib (
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen
    )
{
    uint_t  ssize, dsize;

    if (cut_size(&ssize, srclen))
        return (0);
    if (cut_size(&dsize, dstlen))
        return (0);

    if (uncompress((Bytef*)dst, (uIntf*)(&dsize),
            (const Bytef*)src, ssize) != Z_OK)
        return (0);
    return (dsize);
}

/*
=======================================
    FLATE 压缩
=======================================
*/
CR_API leng_t
compr_flate (
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen,
  __CR_IN__ uint_t          level
    )
{
    uint_t  ssize, dsize;

    if (cut_size(&ssize, srclen))
        return (0);

    if (dst == NULL)
        return (compressBound(ssize));

    if (cut_size(&dsize, dstlen))
        return (0);

    if (compress2_raw((Bytef*)dst, (uIntf*)(&dsize),
            (const Bytef*)src, ssize, level) != Z_OK)
        return (0);
    return (dsize);
}

/*
=======================================
    FLATE 解压
=======================================
*/
CR_API leng_t
uncompr_flate (
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen
    )
{
    uint_t  ssize, dsize;

    if (cut_size(&ssize, srclen))
        return (0);
    if (cut_size(&dsize, dstlen))
        return (0);

    if (uncompress_raw((Bytef*)dst, (uIntf*)(&dsize),
            (const Bytef*)src, ssize) != Z_OK)
        return (0);
    return (dsize);
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
