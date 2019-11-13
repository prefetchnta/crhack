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
/*  >>>>>>>>>>>>>>>>>>>>> CrHack LZRW1 压缩解压函数库 <<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "enclib.h"
#ifndef LZRW1_USE_STACK
    #include "memlib.h"
#endif

/* 标准 LZRW1 常数 */
#ifndef LZRW1_BYTES
    #define LZRW1_BYTES     4   /* 标志用掉的字节数 */
    #define LZRW1_COMPRESS  0   /* 表示压缩是有效的 */
    #define LZRW1_COPY      1   /* 表示无法压缩, 直接复制 */
    #define LZRW1_ITEMMAX   16  /* 最大的展开字节数 */
#endif

/* 循环展开 */
#define LZRW1_PS    (*pp++ != *ss++)

/*
=======================================
    LZRW1 压缩
=======================================
*/
CR_API leng_t
compr_lzrw1 (
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen
    )
{
    byte_t* pp;
    byte_t* ss;
    byte_t* p_src;
    byte_t* p_dst;
    leng_t  offset;
    uint_t  len, index;
    byte_t* p_control;
    byte_t* p_src_post;
    byte_t* p_dst_post;
    byte_t* p_src_max1;
    byte_t* p_src_max16;
    byte_t* p_src_first;
    byte_t* p_dst_first;
    int16u  control_bits;
    int16u  control, unroll;

#ifndef LZRW1_USE_STACK
    byte_t  **hash;
#else
    byte_t  *hash[4096];
#endif

    if (dst == NULL)
        return (srclen + LZRW1_BYTES);

    if (dstlen < srclen + LZRW1_BYTES)
        return (0);

#ifndef LZRW1_USE_STACK
    hash = mem_talloc(4096, byte_t*);
    if (hash == NULL)
        return (0);
#endif
    p_src = (byte_t*)src;
    p_dst = (byte_t*)dst;
    p_src_first = p_src;
    p_dst_first = p_dst;
    p_src_post  = p_src + srclen;
    p_dst_post  = p_dst + srclen;
    p_src_max1  = p_src_post - LZRW1_ITEMMAX;
    p_src_max16 = p_src_post - LZRW1_ITEMMAX * 16;
    control = control_bits = 0;

    *p_dst = LZRW1_COMPRESS;
    p_dst += LZRW1_BYTES;
    p_control = p_dst;
    p_dst += 2;

    for (;;)
    {
        unroll = 16;
        if (p_dst > p_dst_post)
            goto _overrun;
        if (p_src > p_src_max16) {
            unroll = 1;
            if (p_src > p_src_max1) {
                if (p_src == p_src_post)
                    break;
                goto _literal;
            }
        }
_begin_unrolled_loop:
        index = ((40543 * (((((uint_t)p_src[0] << 4) ^
                              (uint_t)p_src[1]) << 4) ^
                              (uint_t)p_src[2])) >> 4) & 0xFFF;
        pp = hash[index];
        hash[index] = ss = p_src;
        offset = (leng_t)(ss - pp);
        if (offset > 4095 || pp < p_src_first || offset == 0 ||
            LZRW1_PS || LZRW1_PS || LZRW1_PS) {
_literal:
            *p_dst++ = *p_src++;
            control >>= 1;
            control_bits++;
        }
        else {
            LZRW1_PS || LZRW1_PS || LZRW1_PS || LZRW1_PS || LZRW1_PS ||
            LZRW1_PS || LZRW1_PS || LZRW1_PS || LZRW1_PS || LZRW1_PS ||
            LZRW1_PS || LZRW1_PS || LZRW1_PS || ss++;
            len = (uint_t)(ss - p_src - 1);
            *p_dst++ = (byte_t)(((offset & 0xF00) >> 4) + (len - 1));
            *p_dst++ = (byte_t)  (offset);
            p_src += len;
            control = (control >> 1) | 0x8000;
            control_bits++;
        }
/* _end_unrolled_loop: */
        if (--unroll)
            goto _begin_unrolled_loop;
        if (control_bits == 16) {
            p_control[0] = (byte_t)(control);
            p_control[1] = (byte_t)(control >> 8);
            p_control = p_dst;
            p_dst += 2;
            control = control_bits = 0;
        }
    }
    control >>= 16 - control_bits;
    *p_control++ = (byte_t)(control);
    *p_control++ = (byte_t)(control >> 8);
    if (p_control == p_dst)
        p_dst -= 2;
#ifndef LZRW1_USE_STACK
    mem_free(hash);
#endif
    return ((leng_t)(p_dst - p_dst_first));

_overrun:
    mem_cpy(p_dst_first + LZRW1_BYTES, p_src_first, srclen);
    *p_dst_first = LZRW1_COPY;
#ifndef LZRW1_USE_STACK
    mem_free(hash);
#endif
    return ((leng_t)(srclen + LZRW1_BYTES));
}

/*
=======================================
    LZRW1 解压
=======================================
*/
CR_API leng_t
uncompr_lzrw1 (
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen
    )
{
    byte_t* pp;
    byte_t* p_src_first = (byte_t*)src;
    byte_t* p_dst_first = (byte_t*)dst;
    byte_t* p_src = p_src_first + LZRW1_BYTES;
    byte_t* p_dst = p_dst_first;
    byte_t* p_src_post = p_src_first + srclen;
    int16u  offset, len, controlbits, control;

    if (srclen <= LZRW1_BYTES)
        return (0);
    srclen -= LZRW1_BYTES;

    if (*p_src_first == LZRW1_COPY) {
        if (dstlen < srclen)
            return (0);
        mem_cpy(p_dst_first, p_src_first + LZRW1_BYTES, srclen);
        return (srclen);
    }

    control = controlbits = 0;
    while (p_src != p_src_post) {
        if (controlbits == 0) {
            if (srclen < 2)
                return (0);
            srclen -= 2;
            control  = (int16u)(*p_src++);
            control |= (int16u)(*p_src++) << 8;
            controlbits = 16;
        }
        if (control & 1) {
            if (srclen < 2)
                return (0);
            srclen -= 2;
            offset  = (int16u)(*p_src   & 0xF0) << 4;
            len = 1 + (int16u)(*p_src++ & 0x0F);
            offset += (int16u)(*p_src++ & 0xFF);
            pp = p_dst - offset;
            if (dstlen < len)
                return (0);
            dstlen -= len;
            while (len-- != 0)
                *p_dst++ = *pp++;
        }
        else {
            if (dstlen-- == 0 || srclen-- == 0)
                return (0);
            *p_dst++ = *p_src++;
        }
        control >>= 1;
        controlbits--;
    }
    return ((leng_t)(p_dst - p_dst_first));
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
