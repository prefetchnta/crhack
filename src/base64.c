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
/*  >>>>>>>>>>>>>>>>>>>>> CrHack BASE64 编码解码函数库 <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "enclib.h"

/* 常数查找表 */
static const ansi_t _rom_ s_cb64[] =
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static const ansi_t _rom_ s_cd64[] =
"|$$$}rstuvwxyz{$$$$$$$>?@ABCDEFGHIJKLMNOPQRSTUVW$$$$$$XYZ[\\]^_`"\
"abcdefghijklmnopq";

/*
---------------------------------------
    编码一组数据
---------------------------------------
*/
static void_t
encodeblock (
  __CR_IN__ const byte_t    in[3],
  __CR_OT__ byte_t          out[4],
  __CR_IN__ uint_t          len
    )
{
    out[0] = s_cb64[  in[0] >> 2];
    out[1] = s_cb64[((in[0] & 0x03) << 4) | ((in[1] & 0xF0) >> 4)];
    out[2] = (uchar)((len > 1) ? s_cb64[((in[1] & 0x0F) << 2) |
                                 ((in[2] & 0xC0) >> 6)] : '=');
    out[3] = (uchar)((len > 2) ? s_cb64[  in[2] & 0x3F] : '=');
}

/*
---------------------------------------
    解码一组数据
---------------------------------------
*/
static void_t
decodeblock (
  __CR_IN__ const byte_t    in[4],
  __CR_OT__ byte_t          out[3]
    )
{
    out[0] = (uchar)  (in[0] << 2 | in[1] >> 4);
    out[1] = (uchar)  (in[1] << 4 | in[2] >> 2);
    out[2] = (uchar)(((in[2] << 6) & 0xC0) | in[3]);
}

/*
=======================================
    BASE64 编码
=======================================
*/
CR_API leng_t
encode_base64 (
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen,
  __CR_IN__ uint_t          linesize
    )
{
    uint_t  idx;
    uint_t  len;
    byte_t  in[3];
    byte_t  ot[4];
    uint_t  lines;
    uchar*  i_buf;
    uchar*  o_buf;
    leng_t  i_ptr;
    leng_t  o_ptr;

    if (dst == NULL) {
        dstlen = (srclen / 3 + 1) * 4;
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
        for (len = 0, idx = 0; idx < 3; idx++)
        {
            if (i_ptr >= srclen)
            {
                in[idx] = 0;
            }
            else
            {
                len++;
                in[idx] = i_buf[i_ptr++];
            }
        }

        if (len != 0)
        {
            encodeblock(in, ot, len);

            for (idx = 0; idx < 4; idx++)
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
    }
    return (o_ptr);
}

/*
=======================================
    BASE64 解码 (支持过滤非法字符)
=======================================
*/
CR_API leng_t
decode_base64 (
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen
    )
{
    byte_t  val;
    uint_t  idx;
    uint_t  len;
    byte_t  in[4];
    byte_t  ot[3];
    leng_t  i_ptr = 0;
    leng_t  o_ptr = 0;
    uchar*  i_buf = (uchar*)src;
    uchar*  o_buf = (uchar*)dst;

    while (i_ptr < srclen)
    {
        for (len = 0, idx = 0; idx < 4 && i_ptr < srclen; idx++)
        {
            val = 0;
            while (i_ptr < srclen && val == 0)
            {
                val = i_buf[i_ptr++];
                val = (val < 43 || val > 122) ? 0 : s_cd64[val - 43];
                if (val != 0)
                    val = (val == (byte_t)('$')) ? 0 : val - 61;
            }

            if (val != 0)
            {
                len++;
                in[idx] = val - 1;
            }
            else
            {
                in[idx] = 0;
            }
        }

        if (len != 0)
        {
            decodeblock(in, ot);

            for (idx = 0; idx < len - 1; idx++)
            {
                if (o_ptr >= dstlen)
                    return (0);
                o_buf[o_ptr++] = ot[idx];
            }
        }
    }
    return (o_ptr);
}

/*
=======================================
    BASE64 to BASE64-URL
=======================================
*/
CR_API leng_t
encode_base64url (
  __CR_IO__ void_t* data,
  __CR_IN__ leng_t  size
    )
{
    leng_t  bck = size;
    byte_t* ptr = (byte_t*)data;

    for (; size != 0; size--, ptr++) {
        if (*ptr == '+')
            *ptr = '-';
        else
        if (*ptr == '/')
            *ptr = '_';
    }
    return (bck);
}

/*
=======================================
    BASE64-URL to BASE64
=======================================
*/
CR_API leng_t
decode_base64url (
  __CR_IO__ void_t* data,
  __CR_IN__ leng_t  size
    )
{
    leng_t  bck = size;
    byte_t* ptr = (byte_t*)data;

    for (; size != 0; size--, ptr++) {
        if (*ptr == '-')
            *ptr = '+';
        else
        if (*ptr == '_')
            *ptr = '/';
    }
    return (bck);
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
