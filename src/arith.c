/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2019-11-12  */
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
/*  >>>>>>>>>>>>>>>>>>>>> CrHack ARITH 压缩解压函数库 <<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "enclib.h"
#include "memlib.h"

/* ARITH 内部结构 */
typedef struct
{
        int32s  cnt[0x100];
        int32s  diff[0x10000];
        int32s  inc0tab[0x10000];
        int32s  inc1tab[0x10000];

} sARITH;

/* 简化操作的宏 */
#define ARITH_DIFF(x)   handle->diff[(uint_t)(x)]
#define ARITH_INC0(x)   handle->inc0tab[(uint_t)(x)]
#define ARITH_INC1(x)   handle->inc1tab[(uint_t)(x)]

/*
---------------------------------------
    ARITH 初始化
---------------------------------------
*/
static void_t
arith_init (
  __CR_OT__ sARITH* handle
    )
{
    sint_t  ii, jj;

    for (ii = 0; ii < 256; ii++)
    for (jj = 0; jj < 256; jj++)
    {
        if (ii != 255)
            handle->inc0tab[(ii << 8) + jj] = ((ii + 1) << 8) + jj;
        else
            handle->inc0tab[(ii << 8) + jj] = (127 << 8) + (jj >> 1);
        if (jj != 255)
            handle->inc1tab[(ii << 8) + jj] = (ii << 8) + (jj + 1);
        else
            handle->inc1tab[(ii << 8) + jj] = ((ii >> 1) << 8) + 127;
        if (ii < jj) {
            handle->diff[(ii << 8) + jj] =  175 * (ii + 1) / (ii + jj + 2);
            if (!handle->diff[(ii << 8) + jj])
                handle->diff[(ii << 8) + jj]++;
        }
        else {
            handle->diff[(ii << 8) + jj] = -175 * (jj + 1) / (ii + jj + 2);
            if (!handle->diff[(ii << 8) + jj])
                handle->diff[(ii << 8) + jj]--;
        }
    }
}

/*
=======================================
    ARITH 压缩
=======================================
*/
CR_API leng_t
compr_arith (
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen
    )
{
    sint_t  count;
    leng_t  o_ptr;
    sARITH  *handle;
    byte_t  cha, mask;
    int32s  aa, ll, xx;
    int32s  space, min, index;

    if (srclen <= 2)
        return (0);

    if (dst == NULL)
        return (srclen * 2 + 4);

    handle = struct_new(sARITH);
    if (handle == NULL)
        return (0);
    struct_zero(handle, sARITH);
    min = 0;
    o_ptr = 0;
    count = -24;
    space = 0xFF;

    arith_init(handle);

    for (;;) {
        if (srclen-- == 0)
            break;
        cha = *(byte_t*)src;
        src =  (byte_t*)src + 1;
        index = 1;
        for (mask = 0x80; mask; mask >>= 1) {
            ll = index;
            aa = handle->cnt[ll];
            xx = ARITH_DIFF(aa);
            if (xx > 0) {
                if (cha & mask) {
                    space -= xx;
                    handle->cnt[ll] = ARITH_INC1(aa);
                    index += index + 1;
                }
                else {
                    min += space - xx;
                    space = xx;
                    handle->cnt[ll] = ARITH_INC0(aa);
                    index += index;
                }
            }
            else {
                if (cha & mask) {
                    min += space + xx;
                    space = -xx;
                    handle->cnt[ll] = ARITH_INC1(aa);
                    index += index + 1;
                }
                else {
                    space += xx;
                    handle->cnt[ll] = ARITH_INC0(aa);
                    index += index;
                }
            }

            while (space < 128) {
                space <<= 1;
                min <<= 1;
                if (!++count) {
                    count = -8;
                    if (dstlen-- == 0)
                        goto _failure;
                    *(byte_t*)dst = (byte_t)(min >> 24);
                    dst = (byte_t*)dst + 1;
                    min &= 0xFFFFFF;
                    if (0xFFFFFF - min < space)
                        space = 0xFFFFFF - min;
                    o_ptr += 1;
                }
            }
        }
    }

    if (dstlen < 4)
        goto _failure;
    min <<= 8 - (count & 7);
    ((byte_t*)dst)[0] = (byte_t)(min >> 24);
    ((byte_t*)dst)[1] = (byte_t)(min >> 16);
    ((byte_t*)dst)[2] = (byte_t)(min >>  8);
    ((byte_t*)dst)[3] = (byte_t)(min);
    mem_free(handle);
    return (o_ptr + 4);

_failure:
    mem_free(handle);
    return (0);
}

/*
=======================================
    ARITH 解压
=======================================
*/
CR_API leng_t
uncompr_arith (
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen
    )
{
    sint_t  count;
    leng_t  o_ptr;
    sARITH  *handle;
    byte_t  cha, gch, mask;
    int32s  aa, ll, xx, val;
    int32s  space, min, index;

    if (srclen < 4)
        return (0);

    handle = struct_new(sARITH);
    if (handle == NULL)
        return (0);
    struct_zero(handle, sARITH);
    min = 0;
    o_ptr = 0;
    count = -8;
    space = 0xFF;

    arith_init(handle);

    val = ((byte_t*)src)[0];
    gch = ((byte_t*)src)[1];
    src =  (byte_t*)src + 2;
    srclen -= 2;

    for (;;) {
        cha = 0;
        index = 1;
        for (mask = 0x80; mask; mask >>= 1) {
            ll = index;
            aa = handle->cnt[ll];
            xx = ARITH_DIFF(aa);
            if (xx > 0) {
                if ((int32u)val < (int32u)(min + space - xx)) {
                    cha |= mask;
                    space -= xx;
                    handle->cnt[ll] = ARITH_INC1(aa);
                    index += index + 1;
                }
                else {
                    min += space - xx;
                    space = xx;
                    handle->cnt[ll] = ARITH_INC0(aa);
                    index += index;
                }
            }
            else {
                if ((int32u)val < (int32u)(min + space + xx)) {
                    space += xx;
                    handle->cnt[ll] = ARITH_INC0(aa);
                    index += index;
                }
                else {
                    cha |= mask;
                    min += space + xx;
                    space = -xx;
                    handle->cnt[ll] = ARITH_INC1(aa);
                    index += index + 1;
                }
            }

            while (space < 128) {
                space <<= 1;
                min <<= 1;
                val = (val << 1) | (gch >> (7 - (count & 7)));
                if (!++count) {
                    if (srclen-- == 0)
                        goto _failure;
                    gch = *(byte_t*)src;
                    src =  (byte_t*)src + 1;
                    count = -8;
                    min &= 0xFFFFFF;
                    val &= 0xFFFFFF;
                    if (0xFFFFFF - min < space)
                        space = 0xFFFFFF - min;
                }
            }
        }

        if (dstlen-- == 0)
            goto _failure;
        *(byte_t*)dst = cha;
        dst = (byte_t*)dst + 1;
        o_ptr += 1;
        if (dstlen == 0)
            break;
    }
    mem_free(handle);
    return (o_ptr);

_failure:
    mem_free(handle);
    return (0);
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
