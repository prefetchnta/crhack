/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2012-08-13  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack UUE 编码解码函数库 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "enclib.h"

/* 解码一个字符 */
#define UU_DEC(c)   (((c) - ' ') & 077)

/* 编码一个字符 */
#define UU_ENC(c)   ((c) ? ((c) & 077) + ' ' : '`')

/*
---------------------------------------
    判断字符是否有效
---------------------------------------
*/
static bool_t
uu_is_okay (
  __CR_IN__ byte_t  ch
    )
{
    if (ch <= ' ' || ch > '`')
        return (FALSE);
    return (TRUE);
}

/*
---------------------------------------
    编码一组数据
---------------------------------------
*/
static void_t
uu_outenc (
  __CR_IN__ const byte_t    in[3],
  __CR_OT__ byte_t          out[4]
    )
{
    byte_t  c1, c2, c3, c4;

    c1 = ((in[0] >> 2));
    c2 = ((in[0] << 4) & 060) | ((in[1] >> 4) & 017);
    c3 = ((in[1] << 2) & 074) | ((in[2] >> 6) & 0x3);
    c4 = ((in[2] & 077));
    out[0] = UU_ENC(c1);
    out[1] = UU_ENC(c2);
    out[2] = UU_ENC(c3);
    out[3] = UU_ENC(c4);
}

/*
---------------------------------------
    解码一组数据
---------------------------------------
*/
static leng_t
uu_outdec (
  __CR_IN__ const byte_t    in[4],
  __CR_OT__ byte_t          out[3],
  __CR_IN__ leng_t          len
    )
{
    byte_t  c1, c2, c3;

    c1 = (UU_DEC(in[0]) << 2) | (UU_DEC(in[1]) >> 4);
    c2 = (UU_DEC(in[1]) << 4) | (UU_DEC(in[2]) >> 2);
    c3 = (UU_DEC(in[2]) << 6) | (UU_DEC(in[3]) >> 0);
    if (len >= 1) out[0] = c1;
    if (len >= 2) out[1] = c2;
    if (len >= 3) out[2] = c3;
    return ((len >= 3) ? 3 : len);
}

/*
=======================================
    UUE 编码
=======================================
*/
CR_API leng_t
encode_uue (
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen
    )
{
    byte_t  in[3];
    uchar*  i_buf;
    uchar*  o_buf;
    leng_t  i_ptr;
    leng_t  o_ptr;
    leng_t  ii, idx, len;

    if (dst == NULL) {
        dstlen = (srclen / 3 + 1) * 4;
        return (dstlen + (srclen / 45 + 1) * 3);
    }
    i_ptr = 0;
    o_ptr = 0;
    i_buf = (uchar*)src;
    o_buf = (uchar*)dst;

    /* 1 - 45 字节一行 */
    len = srclen / 45;
    for (idx = 0; idx < len; idx++, i_ptr += 45) {
        if (o_ptr >= dstlen)
            return (0);
        o_buf[o_ptr++] = UU_ENC(45);
        for (ii = 0; ii < 45; ii += 3) {
            if (o_ptr + 3 >= dstlen)
                return (0);
            uu_outenc(&i_buf[i_ptr + ii], &o_buf[o_ptr]);
            o_ptr += 4;
        }

        /* 输出换行字符 */
        if (o_ptr + 1 >= dstlen)
            return (0);
        o_buf[o_ptr++] = (byte_t)('\r');
        o_buf[o_ptr++] = (byte_t)('\n');
    }

    /* 处理剩余数据 */
    len = srclen % 45;
    if (len != 0)
    {
        if (o_ptr >= dstlen)
            return (0);
        in[0] = (byte_t)len;
        o_buf[o_ptr++] = UU_ENC(in[0]);
        idx = len % 3;
        len -= idx;
        for (ii = 0; ii < len; ii += 3) {
            if (o_ptr + 3 >= dstlen)
                return (0);
            uu_outenc(&i_buf[i_ptr + ii], &o_buf[o_ptr]);
            o_ptr += 4;
        }

        /* 余下的补齐到3字节 */
        if (idx != 0)
        {
            if (o_ptr + 3 >= dstlen)
                return (0);
            i_ptr += len;
            in[0] = in[1] = in[2] = 0;
            for (ii = 0; ii < idx; ii++)
                in[ii] = i_buf[i_ptr++];
            uu_outenc(in, &o_buf[o_ptr]);
            o_ptr += 4;
        }

        /* 输出换行字符 */
        if (o_ptr + 1 >= dstlen)
            return (0);
        o_buf[o_ptr++] = (byte_t)('\r');
        o_buf[o_ptr++] = (byte_t)('\n');
    }
    return (o_ptr);
}

/*
=======================================
    UUE 解码 (支持过滤非法字符)
=======================================
*/
CR_API leng_t
decode_uue (
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen
    )
{
    byte_t  in[4];
    leng_t  i_ptr = 0;
    leng_t  o_ptr = 0;
    uchar*  i_buf = (uchar*)src;
    uchar*  o_buf = (uchar*)dst;
    leng_t  ii, jj, idx, len;

    while (i_ptr < srclen)
    {
        /* 跳过非法字符 */
        if (!uu_is_okay(i_buf[i_ptr])) {
            i_ptr += 1;
            continue;
        }

        /* 读取长度字符 */
        idx = UU_DEC(i_buf[i_ptr]);
        if (idx == 0)
            break;
        if (++i_ptr >= srclen)
            return (0);
        if (o_ptr + idx > dstlen)
            return (0);
        len = ((idx + 2) / 3) * 4;
        if (i_ptr + len > srclen)
            return (0);

        /* 解码后续数据 */
        for (ii = 0; ii < len; ii += 4)
        {
            /* 跳过非法字符 */
            for (jj = 0; jj < 4; i_ptr++) {
                if (i_ptr >= srclen)
                    return (0);
                if (uu_is_okay(i_buf[i_ptr]))
                    in[jj++] = i_buf[i_ptr];
            }
            jj = uu_outdec(in, &o_buf[o_ptr], idx);
            idx   -= jj;
            o_ptr += jj;
        }
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
