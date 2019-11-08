/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2012-08-25  */
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
/*  >>>>>>>>>>>>>>>>>>>>> CrHack BASE32 编码解码函数库 <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "enclib.h"
#include "strlib.h"

/* 常数查找表 */
static const ansi_t _rom_ s_cb32[] =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";

static const byte_t _rom_ s_cd32[] =
{
    /* '0', '1', '2', '3', '4', '5', '6', '7' */
    0xFF, 0xFF, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
    /* '8', '9', ':', ';', '<', '=', '>', '?' */
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    /* '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G' */
    0xFF, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
    /* 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O' */
    0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
    /* 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W' */
    0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16,
    /* 'X', 'Y', 'Z', '[', '\', ']', '^', '_' */
    0x17, 0x18, 0x19, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    /* '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g' */
    0xFF, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
    /* 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o' */
    0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
    /* 'p', 'q', 'r', 's', 't', 'u', 'v', 'w' */
    0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16,
    /* 'x', 'y', 'z', '{', '|', '}', '~', DEL */
    0x17, 0x18, 0x19, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
};

/*
=======================================
    BASE32 编码
=======================================
*/
CR_API leng_t
encode_base32 (
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen,
  __CR_IN__ uint_t          linesize
    )
{
    byte_t  curt;
    byte_t  next;
    uint_t  index;
    uint_t  digit;
    uint_t  lines;
    uchar*  i_buf;
    uchar*  o_buf;
    leng_t  i_ptr;
    leng_t  o_ptr;

    if (dst == NULL) {
        dstlen = (srclen / 5 + 1) * 8;
        if (linesize == 0) return (dstlen);
        return (dstlen + (dstlen / linesize + 1) * 2);
    }
    lines = 0;
    index = 0;
    i_ptr = 0;
    o_ptr = 0;
    i_buf = (uchar*)src;
    o_buf = (uchar*)dst;

    while (i_ptr < srclen)
    {
        curt = i_buf[i_ptr];
        if (index > 3) {
            if ((i_ptr + 1) < srclen)
                next = i_buf[i_ptr + 1];
            else
                next = 0;
            digit = curt & (0xFF >> index);
            index = (index + 5) % 8;
            digit <<= index;
            digit |= next >> (8 - index);
            i_ptr++;
        }
        else {
            digit = curt >> (8 - (index + 5));
            index = (index + 5) % 8;
            if (index == 0)
                i_ptr++;
        }

        if (o_ptr >= dstlen)
            return (0);
        o_buf[o_ptr++] = s_cb32[digit & 0x1F];

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

    /* 输出填充 */
    i_ptr = (leng_t)CR_PADDED(o_ptr, 8);
    for (; i_ptr != 0; i_ptr--)
    {
        if (o_ptr >= dstlen)
            return (0);
        o_buf[o_ptr++] = '=';

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
    return (o_ptr);
}

/*
=======================================
    BASE32 解码 (支持过滤非法字符)
=======================================
*/
CR_API leng_t
decode_base32 (
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen
    )
{
    byte_t  digit;
    uint_t  index;
    uint_t  lookup;
    leng_t  i_ptr = 0;
    leng_t  o_ptr = 0;
    uchar*  i_buf = (uchar*)src;
    uchar*  o_buf = (uchar*)dst;

    for (index = 0; i_ptr < srclen; i_ptr++)
    {
        lookup = (uint_t)(i_buf[i_ptr] - '0');
        if (lookup >= sizeof(s_cd32))
            continue;
        digit = s_cd32[lookup];
        if (digit == 0xFF)
            continue;

        if (index <= 3) {
            index = (index + 5) % 8;
            if (index == 0) {
                if (o_ptr >= dstlen)
                    return (0);
                o_buf[o_ptr++] |= digit;
                if (o_ptr >= dstlen)
                    break;
                o_buf[o_ptr] = 0x00;
            }
            else {
                o_buf[o_ptr] |= digit << (8 - index);
            }
        }
        else {
            if (o_ptr >= dstlen)
                return (0);
            index = (index + 5) % 8;
            o_buf[o_ptr++] |= digit >> index;
            if (o_ptr >= dstlen)
                break;
            o_buf[o_ptr] = (byte_t)(digit << (8 - index));
        }
    }
    return (o_ptr);
}

/*
=======================================
    BASE32 to Z-BASE32
=======================================
*/
CR_API leng_t
encode_zbase32 (
  __CR_IO__ void_t* data,
  __CR_IN__ leng_t  size
    )
{
    byte_t  cha;
    leng_t  bck = size;
    byte_t* ptr = (byte_t*)data;

    for (; size != 0; size--, ptr++)
    {
        cha = *ptr;
        if (is_lowerA(cha))
            cha -= 'a' - 'A';
        switch (cha)
        {
            default: break;
            case 'V': *ptr = 'W'; break;
            case 'W': *ptr = 'X'; break;
            case 'X': *ptr = 'Y'; break;
            case 'Y': *ptr = 'Z'; break;
            case 'Z': *ptr = '3'; break;
            case '2': *ptr = '4'; break;
            case '3': *ptr = '5'; break;
            case '4': *ptr = '6'; break;
            case '5': *ptr = '7'; break;
            case '6': *ptr = '8'; break;
            case '7': *ptr = '9'; break;
        }
    }
    return (bck);
}

/*
=======================================
    Z-BASE32 to BASE32
=======================================
*/
CR_API leng_t
decode_zbase32 (
  __CR_IO__ void_t* data,
  __CR_IN__ leng_t  size
    )
{
    byte_t  cha;
    leng_t  bck = size;
    byte_t* ptr = (byte_t*)data;

    for (; size != 0; size--, ptr++)
    {
        cha = *ptr;
        if (is_lowerA(cha))
            cha -= 'a' - 'A';
        switch (cha)
        {
            default: break;
            case 'W': *ptr = 'V'; break;
            case 'X': *ptr = 'W'; break;
            case 'Y': *ptr = 'X'; break;
            case 'Z': *ptr = 'Y'; break;
            case '3': *ptr = 'Z'; break;
            case '4': *ptr = '2'; break;
            case '5': *ptr = '3'; break;
            case '6': *ptr = '4'; break;
            case '7': *ptr = '5'; break;
            case '8': *ptr = '6'; break;
            case '9': *ptr = '7'; break;
        }
    }
    return (bck);
}

/*
=======================================
    BASE32 to BASE32-CROCKFORD
=======================================
*/
CR_API leng_t
encode_base32crk (
  __CR_IO__ void_t* data,
  __CR_IN__ leng_t  size
    )
{
    byte_t  cha;
    leng_t  bck = size;
    byte_t* ptr = (byte_t*)data;

    for (; size != 0; size--, ptr++)
    {
        cha = *ptr;
        if (is_lowerA(cha))
            cha -= 'a' - 'A';
        switch (cha)
        {
            default: break;
            case 'A': *ptr = '0'; break;
            case 'B': *ptr = '1'; break;
            case 'C': *ptr = '2'; break;
            case 'D': *ptr = '3'; break;
            case 'E': *ptr = '4'; break;
            case 'F': *ptr = '5'; break;
            case 'G': *ptr = '6'; break;
            case 'H': *ptr = '7'; break;
            case 'I': *ptr = '8'; break;
            case 'J': *ptr = '9'; break;
            case 'K': *ptr = 'A'; break;
            case 'L': *ptr = 'B'; break;
            case 'M': *ptr = 'C'; break;
            case 'N': *ptr = 'D'; break;
            case 'O': *ptr = 'E'; break;
            case 'P': *ptr = 'F'; break;
            case 'Q': *ptr = 'G'; break;
            case 'R': *ptr = 'H'; break;
            case 'S': *ptr = 'J'; break;
            case 'T': *ptr = 'K'; break;
            case 'U': *ptr = 'M'; break;
            case 'V': *ptr = 'N'; break;
            case 'W': *ptr = 'P'; break;
            case 'X': *ptr = 'Q'; break;
            case 'Y': *ptr = 'R'; break;
            case 'Z': *ptr = 'S'; break;
            case '2': *ptr = 'T'; break;
            case '3': *ptr = 'V'; break;
            case '4': *ptr = 'W'; break;
            case '5': *ptr = 'X'; break;
            case '6': *ptr = 'Y'; break;
            case '7': *ptr = 'Z'; break;
        }
    }
    return (bck);
}

/*
=======================================
    BASE32-CROCKFORD to BASE32
=======================================
*/
CR_API leng_t
decode_base32crk (
  __CR_IO__ void_t* data,
  __CR_IN__ leng_t  size
    )
{
    byte_t  cha;
    leng_t  bck = size;
    byte_t* ptr = (byte_t*)data;

    for (; size != 0; size--, ptr++)
    {
        cha = *ptr;
        if (is_lowerA(cha))
            cha -= 'a' - 'A';
        switch (cha)
        {
            default: break;
            case 'O':
            case '0': *ptr = 'A'; break;
            case 'I':
            case 'L':
            case '1': *ptr = 'B'; break;
            case '2': *ptr = 'C'; break;
            case '3': *ptr = 'D'; break;
            case '4': *ptr = 'E'; break;
            case '5': *ptr = 'F'; break;
            case '6': *ptr = 'G'; break;
            case '7': *ptr = 'H'; break;
            case '8': *ptr = 'I'; break;
            case '9': *ptr = 'J'; break;
            case 'A': *ptr = 'K'; break;
            case 'B': *ptr = 'L'; break;
            case 'C': *ptr = 'M'; break;
            case 'D': *ptr = 'N'; break;
            case 'E': *ptr = 'O'; break;
            case 'F': *ptr = 'P'; break;
            case 'G': *ptr = 'Q'; break;
            case 'H': *ptr = 'R'; break;
            case 'J': *ptr = 'S'; break;
            case 'K': *ptr = 'T'; break;
            case 'M': *ptr = 'U'; break;
            case 'N': *ptr = 'V'; break;
            case 'P': *ptr = 'W'; break;
            case 'Q': *ptr = 'X'; break;
            case 'R': *ptr = 'Y'; break;
            case 'S': *ptr = 'Z'; break;
            case 'T': *ptr = '2'; break;
            case 'V': *ptr = '3'; break;
            case 'W': *ptr = '4'; break;
            case 'X': *ptr = '5'; break;
            case 'Y': *ptr = '6'; break;
            case 'Z': *ptr = '7'; break;
        }
    }
    return (bck);
}

/*
=======================================
    BASE32 to BASE32-HEX
=======================================
*/
CR_API leng_t
encode_base32hex (
  __CR_IO__ void_t* data,
  __CR_IN__ leng_t  size
    )
{
    byte_t  cha;
    leng_t  bck = size;
    byte_t* ptr = (byte_t*)data;

    for (; size != 0; size--, ptr++)
    {
        cha = *ptr;
        if (is_lowerA(cha))
            cha -= 'a' - 'A';
        switch (cha)
        {
            default: break;
            case 'A': *ptr = '0'; break;
            case 'B': *ptr = '1'; break;
            case 'C': *ptr = '2'; break;
            case 'D': *ptr = '3'; break;
            case 'E': *ptr = '4'; break;
            case 'F': *ptr = '5'; break;
            case 'G': *ptr = '6'; break;
            case 'H': *ptr = '7'; break;
            case 'I': *ptr = '8'; break;
            case 'J': *ptr = '9'; break;
            case 'K': *ptr = 'A'; break;
            case 'L': *ptr = 'B'; break;
            case 'M': *ptr = 'C'; break;
            case 'N': *ptr = 'D'; break;
            case 'O': *ptr = 'E'; break;
            case 'P': *ptr = 'F'; break;
            case 'Q': *ptr = 'G'; break;
            case 'R': *ptr = 'H'; break;
            case 'S': *ptr = 'I'; break;
            case 'T': *ptr = 'J'; break;
            case 'U': *ptr = 'K'; break;
            case 'V': *ptr = 'L'; break;
            case 'W': *ptr = 'M'; break;
            case 'X': *ptr = 'N'; break;
            case 'Y': *ptr = 'O'; break;
            case 'Z': *ptr = 'P'; break;
            case '2': *ptr = 'Q'; break;
            case '3': *ptr = 'R'; break;
            case '4': *ptr = 'S'; break;
            case '5': *ptr = 'T'; break;
            case '6': *ptr = 'U'; break;
            case '7': *ptr = 'V'; break;
        }
    }
    return (bck);
}

/*
=======================================
    BASE32-HEX to BASE32
=======================================
*/
CR_API leng_t
decode_base32hex (
  __CR_IO__ void_t* data,
  __CR_IN__ leng_t  size
    )
{
    byte_t  cha;
    leng_t  bck = size;
    byte_t* ptr = (byte_t*)data;

    for (; size != 0; size--, ptr++)
    {
        cha = *ptr;
        if (is_lowerA(cha))
            cha -= 'a' - 'A';
        switch (cha)
        {
            default: break;
            case '0': *ptr = 'A'; break;
            case '1': *ptr = 'B'; break;
            case '2': *ptr = 'C'; break;
            case '3': *ptr = 'D'; break;
            case '4': *ptr = 'E'; break;
            case '5': *ptr = 'F'; break;
            case '6': *ptr = 'G'; break;
            case '7': *ptr = 'H'; break;
            case '8': *ptr = 'I'; break;
            case '9': *ptr = 'J'; break;
            case 'A': *ptr = 'K'; break;
            case 'B': *ptr = 'L'; break;
            case 'C': *ptr = 'M'; break;
            case 'D': *ptr = 'N'; break;
            case 'E': *ptr = 'O'; break;
            case 'F': *ptr = 'P'; break;
            case 'G': *ptr = 'Q'; break;
            case 'H': *ptr = 'R'; break;
            case 'I': *ptr = 'S'; break;
            case 'J': *ptr = 'T'; break;
            case 'K': *ptr = 'U'; break;
            case 'L': *ptr = 'V'; break;
            case 'M': *ptr = 'W'; break;
            case 'N': *ptr = 'X'; break;
            case 'O': *ptr = 'Y'; break;
            case 'P': *ptr = 'Z'; break;
            case 'Q': *ptr = '2'; break;
            case 'R': *ptr = '3'; break;
            case 'S': *ptr = '4'; break;
            case 'T': *ptr = '5'; break;
            case 'U': *ptr = '6'; break;
            case 'V': *ptr = '7'; break;
        }
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
