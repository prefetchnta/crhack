/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2012-08-22  */
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
/*  >>>>>>>>>>>>>>>>>>>>> CrHack BASE16 编码解码函数库 <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "enclib.h"
#include "strlib.h"

/* 常数查找表 */
static const ansi_t _rom_ s_cb16[] = "0123456789ABCDEF";

/*
=======================================
    BASE16 编码
=======================================
*/
CR_API leng_t
encode_base16 (
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen,
  __CR_IN__ uint_t          linesize
    )
{
    byte_t  in;
    uint_t  idx;
    uint_t  lines;
    ansi_t  ot[2];
    uchar*  i_buf;
    uchar*  o_buf;
    leng_t  i_ptr;
    leng_t  o_ptr;

    if (dst == NULL) {
        dstlen = srclen * 2;
        if (linesize == 0) return (dstlen);
        return (dstlen + (dstlen / linesize + 1) * 2);
    }
    lines = 0;
    i_ptr = 0;
    o_ptr = 0;
    i_buf = (uchar*)src;
    o_buf = (uchar*)dst;

    while (i_ptr < srclen)
    {
        in = i_buf[i_ptr++];
        ot[0] = s_cb16[in >> 4];
        ot[1] = s_cb16[in & 15];
        for (idx = 0; idx < 2; idx++)
        {
            if (o_ptr >= dstlen)
                return (0);
            o_buf[o_ptr++] = ot[idx];

            /* 断行 */
            if (linesize == 0)
                continue;
            if (++lines == linesize)
            {
                if (o_ptr + 1 >= dstlen)
                    return (0);
                o_buf[o_ptr++] = (byte_t)('\r');
                o_buf[o_ptr++] = (byte_t)('\n');
                lines = 0;
            }
        }
    }
    return (o_ptr);
}

/*
=======================================
    BASE16 解码 (支持过滤非法字符)
=======================================
*/
CR_API leng_t
decode_base16 (
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen
    )
{
    byte_t  ot;
    uint_t  idx;
    ansi_t  in[2];
    leng_t  i_ptr = 0;
    leng_t  o_ptr = 0;
    uchar*  i_buf = (uchar*)src;
    uchar*  o_buf = (uchar*)dst;

    while (i_ptr < srclen)
    {
        /* 跳过非法字符 */
        if (!is_xnumbA(i_buf[i_ptr])) {
            i_ptr += 1;
            continue;
        }
        for (idx = 0; idx < 2; i_ptr++) {
            if (i_ptr >= srclen)
                return (0);
            if (is_xnumbA(i_buf[i_ptr]))
                in[idx++] = i_buf[i_ptr];
        }

        /* 输出解码 */
        if (o_ptr >= dstlen)
            return (0);
        if (is_digitA(in[0]))
            ot = (in[0] & 0x0F);
        else
            ot = (in[0] & 0x0F) + 9;
        ot <<= 4;
        if (is_digitA(in[1]))
            ot |= (in[1] & 0x0F);
        else
            ot |= (in[1] & 0x0F) + 9;
        o_buf[o_ptr++] = ot;
    }
    return (o_ptr);
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
