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
/*                                   MTF                                     */
/*****************************************************************************/

/*
=======================================
    MTF 编码
=======================================
*/
CR_API leng_t
encode_mtf (
  __CR_IO__ void_t* data,
  __CR_IN__ leng_t  size
    )
{
    uint_t  idx;
    byte_t  tbl[256];
    leng_t  bck = size;
    byte_t  cc, cp, ccc;

    for (idx = 0; idx < sizeof(tbl); idx++)
        tbl[idx] = (byte_t)idx;

    for (; size != 0; size--) {
        cc = *(byte_t*)data;
        cp = cc;
        for (idx = 0; ; idx++) {
            ccc = tbl[idx];
            tbl[idx] = cp;
            if (ccc == cc) {
                *(byte_t*)data = (byte_t)idx;
                data = (byte_t*)data + 1;
                break;
            }
            cp = ccc;
        }
    }
    return (bck);
}

/*
=======================================
    MTF 解码
=======================================
*/
CR_API leng_t
decode_mtf (
  __CR_IO__ void_t* data,
  __CR_IN__ leng_t  size
    )
{
    uint_t  idx;
    leng_t  bck = size;
    byte_t  cc, tbl[256];

    for (idx = 0; idx < sizeof(tbl); idx++)
        tbl[idx] = (byte_t)idx;

    for (; size != 0; size--) {
        idx = *(byte_t*)data;
        cc = tbl[idx];
        *(byte_t*)data = cc;
        data = (byte_t*)data + 1;
        if (idx != 0) {
            while (idx-- != 0)
                tbl[idx + 1] = tbl[idx];
            tbl[0] = cc;
        }
    }
    return (bck);
}

/*****************************************************************************/
/*                                   BRR                                     */
/*****************************************************************************/

/* 默认掩码表 */
static const byte_t _rom_ s_brr_mask[8] =
{
    0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01
};

/*
=======================================
    BRR 编码
=======================================
*/
CR_API leng_t
encode_brr (
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen,
  __CR_IN__ const byte_t*   mask
    )
{
    byte_t  cha;
    leng_t  ii, jj, kk;

    /* 参数过滤 */
    if (dst == NULL)
        return (srclen);
    if (dstlen < srclen)
        return (0);
    if (mask == NULL)
        mask = s_brr_mask;
    kk = 0;

    /* 清零后按位输出 */
    mem_zero(dst, srclen);
    for (ii = 0; ii < 8; ii++) {
        for (jj = 0; jj < srclen; jj++) {
            cha = *(byte_t*)dst;
            if (((byte_t*)src)[jj] & mask[ii])
                cha |= 1;
            if (++kk >= 8) {
                kk = 0;
                *(byte_t*)dst = cha;
                dst = (byte_t*)dst + 1;
            }
            else {
                *(byte_t*)dst = cha << 1;
            }
        }
    }
    return (srclen);
}

/*
=======================================
    BRR 解码
=======================================
*/
CR_API leng_t
decode_brr (
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen,
  __CR_IN__ const byte_t*   mask
    )
{
    byte_t  cha;
    leng_t  ii, jj, kk, mm;

    /* 参数过滤 */
    if (dstlen < srclen)
        return (0);
    if (mask == NULL)
        mask = s_brr_mask;
    kk = mm = 0;

    /* 清零后按位输出 */
    mem_zero(dst, srclen);
    for (ii = 0; ii < srclen; ii++) {
        cha = *(byte_t*)src;
        src = (byte_t*)src + 1;
        for (jj = 0; jj < 8; jj++) {
            if ((cha >> (7 - jj)) & 1)
                ((byte_t*)dst)[kk] |= mask[mm];
            if (++kk >= srclen) {
                kk = 0;
                mm += 1;
            }
        }
    }
    return (srclen);
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
