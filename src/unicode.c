/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-01-12  */
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
/*  >>>>>>>>>>>>>>>>>>>> CrHack UNICODE 编码转换函数库 <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "safe.h"
#include "memlib.h"
#include "strlib.h"

/*
=======================================
    ASCII 字符串转换到宽字符串
=======================================
*/
CR_API wide_t*
str_char2wide (
  __CR_IN__ const ansi_t*   str
    )
{
    leng_t  len = str_lenA(str) + 1;
    wide_t* tmp = mem_talloc(len, wide_t);
    wide_t* ptr = (wide_t*)tmp;

    if (ptr == NULL)
        return (NULL);

    for (; len != 0; len--)
        *ptr++ = (byte_t)(*str++);
    return ((wide_t*)tmp);
}

/*
=======================================
    宽字符串转换到 ASCII 字符串
=======================================
*/
CR_API ansi_t*
str_wide2char (
  __CR_IN__ const wide_t*   str
    )
{
    leng_t  len = str_lenW(str) + 1;
    byte_t* tmp = (byte_t*)mem_malloc(len);
    byte_t* ptr = (byte_t*)tmp;

    if (ptr == NULL)
        return (NULL);

    for (; len != 0; len--)
        *ptr++ = (byte_t)(*str++);
    return ((ansi_t*)tmp);
}

/*
=======================================
    UTF-8 字符转换到 UCS4 字符
=======================================
*/
CR_API uint_t
utf8_to_ucs4 (
  __CR_OT__ int32u*         ucs4,
  __CR_IN__ const void_t*   chr
    )
{
    byte_t  cha = *(uchar*)chr;
    byte_t* ptr =  (uchar*)chr;

    if (cha < 0x80)
    {
        *ucs4 = cha;
        return (1);
    }

    if (cha < 0xC2)
        return (0);

    if (cha < 0xE0)
    {
        if (!((ptr[1] ^ 0x80) < 0x40))
            return (0);
        *ucs4 = (int32u)(ptr[1] ^ 0x80) | ((int32u)(cha & 0x1F) << 6);
        return (2);
    }

    if (cha < 0xF0)
    {
        if (!((ptr[1] ^ 0x80) < 0x40 &&
              (ptr[2] ^ 0x80) < 0x40 && (cha >= 0xE1 || ptr[1] >= 0xA0)))
            return (0);
        *ucs4 = ((int32u)(ptr[1] ^ 0x80) << 6) |
                ((int32u)(ptr[2] ^ 0x80)) | ((int32u)(cha & 0x0F) << 12);
        return (3);
    }

    if (cha < 0xF8)
    {
        if (!((ptr[1] ^ 0x80) < 0x40 &&
              (ptr[2] ^ 0x80) < 0x40 &&
              (ptr[3] ^ 0x80) < 0x40 && (cha >= 0xF1 || ptr[1] >= 0x90)))
            return (0);
        *ucs4 = ((int32u)(ptr[1] ^ 0x80) << 12) |
                ((int32u)(ptr[2] ^ 0x80) <<  6) |
                ((int32u)(ptr[3] ^ 0x80)) | ((int32u)(cha & 0x07) << 18);
        return (4);
    }

    if (cha < 0xFC)
    {
        if (!((ptr[1] ^ 0x80) < 0x40 &&
              (ptr[2] ^ 0x80) < 0x40 &&
              (ptr[3] ^ 0x80) < 0x40 &&
              (ptr[4] ^ 0x80) < 0x40 && (cha >= 0xF9 || ptr[1] >= 0x88)))
            return (0);
        *ucs4 = ((int32u)(ptr[1] ^ 0x80) << 18) |
                ((int32u)(ptr[2] ^ 0x80) << 12) |
                ((int32u)(ptr[3] ^ 0x80) <<  6) |
                ((int32u)(ptr[4] ^ 0x80)) | ((int32u)(cha & 0x03) << 24);
        return (5);
    }

    if (cha < 0xFE)
    {
        if (!((ptr[1] ^ 0x80) < 0x40 &&
              (ptr[2] ^ 0x80) < 0x40 &&
              (ptr[3] ^ 0x80) < 0x40 &&
              (ptr[4] ^ 0x80) < 0x40 &&
              (ptr[5] ^ 0x80) < 0x40 && (cha >= 0xFD || ptr[1] >= 0x84)))
            return (0);
        *ucs4 = ((int32u)(ptr[1] ^ 0x80) << 24) |
                ((int32u)(ptr[2] ^ 0x80) << 18) |
                ((int32u)(ptr[3] ^ 0x80) << 12) |
                ((int32u)(ptr[4] ^ 0x80) <<  6) |
                ((int32u)(ptr[5] ^ 0x80)) | ((int32u)(cha & 0x01) << 30);
        return (6);
    }

    return (0);
}

/*
=======================================
    UTF-16LE 字符转换到 UCS4 字符
=======================================
*/
CR_API uint_t
utf16le_to_ucs4 (
  __CR_OT__ int32u*         ucs4,
  __CR_IN__ const void_t*   chr
    )
{
    int32u  ucs41;
    int32u  ucs42;
    byte_t* ptr = (uchar*)chr;

    ucs41 = (int32u)ptr[0] | ((int32u)ptr[1] << 8);
    if (ucs41 >= 0xDC00 && ucs41 < 0xE000)
        return (0);

    if (ucs41 >= 0xD800 && ucs41 < 0xDC00)
    {
        ucs42 = (int32u)ptr[2] | ((int32u)ptr[3] << 8);
        if (!(ucs42 >= 0xDC00 && ucs42 < 0xE000))
            return (0);
        *ucs4 = 0x10000UL + ((ucs41 - 0xD800) << 10) + (ucs42 - 0xDC00);
        return (4);
    }
    *ucs4 = ucs41;
    return (2);
}

/*
=======================================
    UTF-16BE 字符转换到 UCS4 字符
=======================================
*/
CR_API uint_t
utf16be_to_ucs4 (
  __CR_OT__ int32u*         ucs4,
  __CR_IN__ const void_t*   chr
    )
{
    int32u  ucs41;
    int32u  ucs42;
    byte_t* ptr = (uchar*)chr;

    ucs41 = ((int32u)ptr[0] << 8) | (int32u)ptr[1];
    if (ucs41 >= 0xDC00 && ucs41 < 0xE000)
        return (0);

    if (ucs41 >= 0xD800 && ucs41 < 0xDC00)
    {
        ucs42 = ((int32u)ptr[2] << 8) | (int32u)ptr[3];
        if (!(ucs42 >= 0xDC00 && ucs42 < 0xE000))
            return (0);
        *ucs4 = 0x10000UL + ((ucs41 - 0xD800) << 10) + (ucs42 - 0xDC00);
        return (4);
    }
    *ucs4 = ucs41;
    return (2);
}

/*
=======================================
    UTF-32LE 字符转换到 UCS4 字符
=======================================
*/
CR_API uint_t
utf32le_to_ucs4 (
  __CR_OT__ int32u*         ucs4,
  __CR_IN__ const void_t*   chr
    )
{
    int32u  ucs41;
    byte_t* ptr = (uchar*)chr;

    ucs41 = (int32u)ptr[0] | ((int32u)ptr[1] <<  8) |
                             ((int32u)ptr[2] << 16) |
                             ((int32u)ptr[3] << 24);
    if (!(ucs41 < 0x110000UL && !(ucs41 >= 0xD800 && ucs41 < 0xE000)))
        return (0);
    *ucs4 = ucs41;
    return (4);
}

/*
=======================================
    UTF-32BE 字符转换到 UCS4 字符
=======================================
*/
CR_API uint_t
utf32be_to_ucs4 (
  __CR_OT__ int32u*         ucs4,
  __CR_IN__ const void_t*   chr
    )
{
    int32u  ucs41;
    byte_t* ptr = (uchar*)chr;

    ucs41 = ((int32u)ptr[0] << 24) |
            ((int32u)ptr[1] << 16) |
            ((int32u)ptr[2] <<  8) | (int32u)ptr[3];
    if (!(ucs41 < 0x110000UL && !(ucs41 >= 0xD800 && ucs41 < 0xE000)))
        return (0);
    *ucs4 = ucs41;
    return (4);
}

/*
=======================================
    UCS4 字符转换到 UTF-8 字符
=======================================
*/
CR_API uint_t
ucs4_to_utf8 (
  __CR_OT__ void_t* chr,
  __CR_IN__ int32u  ucs4
    )
{
    uint_t  count;
    byte_t* ptr = (uchar*)chr;

    if      (ucs4 <  0x00000080UL) count = 1;
    else if (ucs4 <  0x00000800UL) count = 2;
    else if (ucs4 <  0x00010000UL) count = 3;
    else if (ucs4 <  0x00200000UL) count = 4;
    else if (ucs4 <  0x04000000UL) count = 5;
    else if (ucs4 <= 0x7FFFFFFFUL) count = 6;
    else
        return (0);

    switch (count)
    {
        case 6: ptr[5]=(uchar)(0x80|(ucs4 & 0x3F)); ucs4=(ucs4>>6)|0x4000000UL;
        case 5: ptr[4]=(uchar)(0x80|(ucs4 & 0x3F)); ucs4=(ucs4>>6)|0x200000UL;
        case 4: ptr[3]=(uchar)(0x80|(ucs4 & 0x3F)); ucs4=(ucs4>>6)|0x10000UL;
        case 3: ptr[2]=(uchar)(0x80|(ucs4 & 0x3F)); ucs4=(ucs4>>6)|0x800UL;
        case 2: ptr[1]=(uchar)(0x80|(ucs4 & 0x3F)); ucs4=(ucs4>>6)|0xC0UL;
        case 1: ptr[0]=(uchar)(ucs4);
    }
    return (count);
}

/*
=======================================
    UCS4 字符转换到 UTF-16LE 字符
=======================================
*/
CR_API uint_t
ucs4_to_utf16le (
  __CR_OT__ void_t* chr,
  __CR_IN__ int32u  ucs4
    )
{
    int32u  ucs41;
    int32u  ucs42;
    byte_t* ptr = (uchar*)chr;

    if (ucs4 >= 0xD800 && ucs4 < 0xE000)
        return (0);

    if (ucs4 < 0x10000UL) {
        ptr[0] = (uchar)(ucs4 >> 0);
        ptr[1] = (uchar)(ucs4 >> 8);
        return (2);
    }

    if (ucs4 < 0x110000UL) {
        ucs41 = 0xD800 + ((ucs4 - 0x10000UL) >> 10);
        ucs42 = 0xDC00 + ((ucs4 - 0x10000UL) & 0x3FF);

        ptr[0] = (uchar)(ucs41 >> 0);
        ptr[1] = (uchar)(ucs41 >> 8);
        ptr[2] = (uchar)(ucs42 >> 0);
        ptr[3] = (uchar)(ucs42 >> 8);
        return (4);
    }

    return (0);
}

/*
=======================================
    UCS4 字符转换到 UTF-16BE 字符
=======================================
*/
CR_API uint_t
ucs4_to_utf16be (
  __CR_OT__ void_t* chr,
  __CR_IN__ int32u  ucs4
    )
{
    int32u  ucs41;
    int32u  ucs42;
    byte_t* ptr = (uchar*)chr;

    if (ucs4 >= 0xD800 && ucs4 < 0xE000)
        return (0);

    if (ucs4 < 0x10000UL) {
        ptr[0] = (uchar)(ucs4 >> 8);
        ptr[1] = (uchar)(ucs4 >> 0);
        return (2);
    }

    if (ucs4 < 0x110000UL) {
        ucs41 = 0xD800 + ((ucs4 - 0x10000UL) >> 10);
        ucs42 = 0xDC00 + ((ucs4 - 0x10000UL) & 0x3FF);

        ptr[0] = (uchar)(ucs41 >> 8);
        ptr[1] = (uchar)(ucs41 >> 0);
        ptr[2] = (uchar)(ucs42 >> 8);
        ptr[3] = (uchar)(ucs42 >> 0);
        return (4);
    }

    return (0);
}

/*
=======================================
    UCS4 字符转换到 UTF-32LE 字符
=======================================
*/
CR_API uint_t
ucs4_to_utf32le (
  __CR_OT__ void_t* chr,
  __CR_IN__ int32u  ucs4
    )
{
    byte_t* ptr = (uchar*)chr;

    if (!(ucs4 < 0x110000UL && !(ucs4 >= 0xD800 && ucs4 < 0xE000)))
        return (0);
    ptr[0] = (uchar)(ucs4 >>  0);
    ptr[1] = (uchar)(ucs4 >>  8);
    ptr[2] = (uchar)(ucs4 >> 16);
    ptr[3] = 0x00;
    return (4);
}

/*
=======================================
    UCS4 字符转换到 UTF-32BE 字符
=======================================
*/
CR_API uint_t
ucs4_to_utf32be (
  __CR_OT__ void_t* chr,
  __CR_IN__ int32u  ucs4
    )
{
    byte_t* ptr = (uchar*)chr;

    if (!(ucs4 < 0x110000UL && !(ucs4 >= 0xD800 && ucs4 < 0xE000)))
        return (0);
    ptr[0] = 0x00;
    ptr[1] = (uchar)(ucs4 >> 16);
    ptr[2] = (uchar)(ucs4 >>  8);
    ptr[3] = (uchar)(ucs4 >>  0);
    return (4);
}

/*
=======================================
    UTF-8 转到 UTF-16
=======================================
*/
CR_API wide_t*
utf8_to_utf16 (
  __CR_IN__ const ansi_t*   utf8
    )
{
    leng_t          len;
    leng_t          move;
    int32u          ucs4;
    wide_t*         rett;
    const ansi_t*   temp;

    /* 计算长度 */
    for (temp = utf8; *temp != CR_AC(NIL); temp += move)
    {
        move = utf8_to_ucs4(&ucs4, temp);
        if (move == 0)
            return (NULL);
    }

    /* 分配内存 */
    rett = str_allocW((leng_t)(temp - utf8 + 1));
    if (rett == NULL)
        return (NULL);

    /* 转换编码 */
    for (len = 0; *utf8 != CR_AC(NIL); utf8 += move)
    {
        move = utf8_to_ucs4(&ucs4, utf8);
        len += ucs4_to_utf16(&rett[len], ucs4) / sizeof(wide_t);
    }
    rett[len] = CR_AC(NIL);
    return (rett);
}

/*
=======================================
    UTF-16 转到 UTF-8
=======================================
*/
CR_API ansi_t*
utf16_to_utf8 (
  __CR_IN__ const wide_t*   wide
    )
{
    leng_t          len;
    leng_t          move;
    int32u          ucs4;
    ansi_t*         rett;
    const wide_t*   temp;

    /* 计算长度 */
    for (temp = wide; *temp != CR_NC(NIL); temp += move)
    {
        move = utf16_to_ucs4(&ucs4, temp) / sizeof(wide_t);
        if (move == 0)
            return (NULL);
    }

    /* 安全检查 */
    len = (leng_t)(temp - wide);
    if (cut_mad(&move, len, 6, 1))
        return (NULL);

    /* 分配内存 */
    rett = str_allocA(move);
    if (rett == NULL)
        return (NULL);

    /* 转换编码 */
    for (len = 0; *wide != CR_NC(NIL); wide += move)
    {
        move = utf16_to_ucs4(&ucs4, wide) / sizeof(wide_t);
        len += ucs4_to_utf8(&rett[len], ucs4);
    }
    rett[len] = CR_NC(NIL);
    return (rett);
}

/*
=======================================
    UTF-8 定位 UTF-16
=======================================
*/
CR_API leng_t
utf8_seek_utf16 (
  __CR_IN__ const ansi_t*   utf8,
  __CR_IN__ leng_t          pos
    )
{
    leng_t  move;
    int32u  ucs4;
    wide_t  ch[2];
    leng_t  ptr = 0;
    leng_t  len = 0;

    for (; ptr < pos; utf8 += move, ptr += move)
    {
        move = utf8_to_ucs4(&ucs4, utf8);
        if (move == 0)
            return ((leng_t)-1);
        len += ucs4_to_utf16(ch, ucs4);
    }
    return (len / sizeof(wide_t));
}

/*
=======================================
    UTF-16 定位 UTF-8
=======================================
*/
CR_API leng_t
utf16_seek_utf8 (
  __CR_IN__ const wide_t*   wide,
  __CR_IN__ leng_t          pos
    )
{
    leng_t  move;
    int32u  ucs4;
    ansi_t  ch[6];
    leng_t  ptr = 0;
    leng_t  len = 0;

    for (; ptr < pos; wide += move, ptr += move)
    {
        move = utf16_to_ucs4(&ucs4, wide) / sizeof(wide_t);
        if (move == 0)
            return ((leng_t)-1);
        len += ucs4_to_utf8(ch, ucs4);
    }
    return (len / sizeof(ansi_t));
}

/*
=======================================
    把文本转换成 UTF-8
=======================================
*/
CR_API ansi_t*
text_to_utf8 (
  __CR_IN__ const ansi_t*   text,
  __CR_OT__ uint_t*         codepage,
  __CR_IN__ bool_t          is_xml
    )
{
    int32u          ucs4;
    uint_t          page;
    ansi_t*         rett;
    leng_t          move, len;
    const ansi_t*   strx = text;

    /* UTF-8 */
    if (chr_cmpA(text, BOM_UTF8, 3) == 0)
    {
        /* 直接复制 */
        rett = str_dupA(text + 3);
        if (rett == NULL)
            return (NULL);

        /* 返回结果 */
        if (codepage != NULL)
            *codepage = CR_UTF8;
        return (rett);
    }

    /* UTF-32LE */
    if (chr_cmpA(text, BOM_UTF32LE, 4) == 0)
    {
        /* 计算长度 */
        for (text += 4; *(int32u*)text != 0; text += move)
        {
            move = utf32le_to_ucs4(&ucs4, text);
            if (move == 0)
                goto _local;
        }

        /* 安全检查 */
        len = (leng_t)((text - strx) / 4);
        if (cut_mul(&move, len, 6))
            return (NULL);

        /* 分配内存 */
        rett = str_allocA(move);
        if (rett == NULL)
            return (NULL);
        len = 0;

        /* 转换编码 */
        for (strx += 4; *(int32u*)strx != 0; strx += move)
        {
            move = utf32le_to_ucs4(&ucs4, strx);
            len += ucs4_to_utf8(&rett[len], ucs4);
        }

        /* 返回结果 */
        if (codepage != NULL)
            *codepage = CR_UTF32LE;
        rett[len] = CR_AC(NIL);
        return (rett);
    }

    /* UTF-32BE */
    if (chr_cmpA(text, BOM_UTF32BE, 4) == 0)
    {
        /* 计算长度 */
        for (text += 4; *(int32u*)text != 0; text += move)
        {
            move = utf32be_to_ucs4(&ucs4, text);
            if (move == 0)
                goto _local;
        }

        /* 安全检查 */
        len = (leng_t)((text - strx) / 4);
        if (cut_mul(&move, len, 6))
            return (NULL);

        /* 分配内存 */
        rett = str_allocA(move);
        if (rett == NULL)
            return (NULL);
        len = 0;

        /* 转换编码 */
        for (strx += 4; *(int32u*)strx != 0; strx += move)
        {
            move = utf32be_to_ucs4(&ucs4, strx);
            len += ucs4_to_utf8(&rett[len], ucs4);
        }

        /* 返回结果 */
        if (codepage != NULL)
            *codepage = CR_UTF32BE;
        rett[len] = CR_AC(NIL);
        return (rett);
    }

    /* UTF-16LE */
    if (chr_cmpA(text, BOM_UTF16LE, 2) == 0)
    {
        /* 计算长度 */
        for (text += 2; *(int16u*)text != 0; text += move)
        {
            move = utf16le_to_ucs4(&ucs4, text);
            if (move == 0)
                goto _local;
        }

        /* 安全检查 */
        len = (leng_t)(text - strx);
        if (cut_mul(&move, len, 3))
            return (NULL);

        /* 分配内存 */
        rett = str_allocA(move);
        if (rett == NULL)
            return (NULL);
        len = 0;

        /* 转换编码 */
        for (strx += 2; *(int16u*)strx != 0; strx += move)
        {
            move = utf16le_to_ucs4(&ucs4, strx);
            len += ucs4_to_utf8(&rett[len], ucs4);
        }

        /* 返回结果 */
        if (codepage != NULL)
            *codepage = CR_UTF16LE;
        rett[len] = CR_AC(NIL);
        return (rett);
    }

    /* UTF-16BE */
    if (chr_cmpA(text, BOM_UTF16BE, 2) == 0)
    {
        /* 计算长度 */
        for (text += 2; *(int16u*)text != 0; text += move)
        {
            move = utf16be_to_ucs4(&ucs4, text);
            if (move == 0)
                goto _local;
        }

        /* 安全检查 */
        len = (leng_t)(text - strx);
        if (cut_mul(&move, len, 3))
            return (NULL);

        /* 分配内存 */
        rett = str_allocA(move);
        if (rett == NULL)
            return (NULL);
        len = 0;

        /* 转换编码 */
        for (strx += 2; *(int16u*)strx != 0; strx += move)
        {
            move = utf16be_to_ucs4(&ucs4, strx);
            len += ucs4_to_utf8(&rett[len], ucs4);
        }

        /* 返回结果 */
        if (codepage != NULL)
            *codepage = CR_UTF16BE;
        rett[len] = CR_AC(NIL);
        return (rett);
    }

    /* 无 BOM */
    if (is_xml)
    {
        /* XML 形式 */
        if (chr_cmpA(text, "<!-- encoding:", 14) == 0)
        {
            /* 指定了编码 */
            page = str2intA(text + 14, NULL);

            /* 跳过此注释 */
            text = skip_xml_cmtA(text);
            if (text != NULL)
            {
                if (page != CR_UTF8)
                {
                    /* 转换编码 (需外部库支持) */
                    rett = local_to_utf8(page, text);
                }
                else
                {
                    /* 直接复制 */
                    rett = str_dupA(text);
                }
                if (rett == NULL)
                    return (NULL);

                /* 返回结果 */
                if (codepage != NULL)
                    *codepage = page;
                return (rett);
            }
        }
    }
    else
    {
        /* INI 形式 */
        if (chr_cmpA(text, "#encoding:", 10) == 0)
        {
            /* 指定了编码 */
            text += 10;
            page = str2intA(text, &len);
            if (page != CR_UTF8)
            {
                /* 转换编码 (需外部库支持) */
                rett = local_to_utf8(page, text + len);
            }
            else
            {
                /* 直接复制 */
                rett = str_dupA(text + len);
            }
            if (rett == NULL)
                return (NULL);

            /* 返回结果 */
            if (codepage != NULL)
                *codepage = page;
            return (rett);
        }
    }

_local:
    /* 本地编码 (需外部库支持) */
    rett = local_to_utf8(CR_LOCAL, strx);
    if (rett == NULL)
        return (NULL);

    /* 返回结果 */
    if (codepage != NULL)
        *codepage = CR_LOCAL;
    return (rett);
}

/*
=======================================
    把文本转换成 UTF-16
=======================================
*/
CR_API wide_t*
text_to_utf16 (
  __CR_IN__ const ansi_t*   text,
  __CR_OT__ uint_t*         codepage,
  __CR_IN__ bool_t          is_xml
    )
{
    uint_t          page;
    int32u          ucs4;
    wide_t*         rett;
    leng_t          move, len;
    const ansi_t*   strx = text;

    /* UTF-8 */
    if (chr_cmpA(text, BOM_UTF8, 3) == 0)
    {
        /* 计算长度 */
        for (text += 3; *text != CR_AC(NIL); text += move)
        {
            move = utf8_to_ucs4(&ucs4, text);
            if (move == 0)
                goto _local;
        }

        /* 分配内存 */
        rett = str_allocW((leng_t)(text - strx));
        if (rett == NULL)
            return (NULL);
        len = 0;

        /* 转换编码 */
        for (strx += 3; *strx != CR_AC(NIL); strx += move)
        {
            move = utf8_to_ucs4(&ucs4, strx);
            len += ucs4_to_utf16(&rett[len], ucs4) / sizeof(wide_t);
        }

        /* 返回结果 */
        if (codepage != NULL)
            *codepage = CR_UTF8;
        rett[len] = CR_NC(NIL);
        return (rett);
    }

    /* UTF-32LE */
    if (chr_cmpA(text, BOM_UTF32LE, 4) == 0)
    {
        /* 计算长度 */
        for (text += 4; *(int32u*)text != 0; text += move)
        {
            move = utf32le_to_ucs4(&ucs4, text);
            if (move == 0)
                goto _local;
        }

        /* 分配内存 */
        rett = (wide_t*)mem_malloc((leng_t)(text - strx));
        if (rett == NULL)
            return (NULL);
        len = 0;

        /* 转换编码 */
        for (strx += 4; *(int32u*)strx != 0; strx += move)
        {
            move = utf32le_to_ucs4(&ucs4, strx);
            len += ucs4_to_utf16(&rett[len], ucs4) / sizeof(wide_t);
        }

        /* 返回结果 */
        if (codepage != NULL)
            *codepage = CR_UTF32LE;
        rett[len] = CR_NC(NIL);
        return (rett);
    }

    /* UTF-32BE */
    if (chr_cmpA(text, BOM_UTF32BE, 4) == 0)
    {
        /* 计算长度 */
        for (text += 4; *(int32u*)text != 0; text += move)
        {
            move = utf32be_to_ucs4(&ucs4, text);
            if (move == 0)
                goto _local;
        }

        /* 分配内存 */
        rett = (wide_t*)mem_malloc((leng_t)(text - strx));
        if (rett == NULL)
            return (NULL);
        len = 0;

        /* 转换编码 */
        for (strx += 4; *(int32u*)strx != 0; strx += move)
        {
            move = utf32be_to_ucs4(&ucs4, strx);
            len += ucs4_to_utf16(&rett[len], ucs4) / sizeof(wide_t);
        }

        /* 返回结果 */
        if (codepage != NULL)
            *codepage = CR_UTF32BE;
        rett[len] = CR_NC(NIL);
        return (rett);
    }

    /* UTF-16LE */
    if (chr_cmpA(text, BOM_UTF16LE, 2) == 0)
    {
#if defined(_CR_ORDER_LE_)

        /* 直接复制 */
        rett = str_dupW((wide_t*)text + 1);
        if (rett == NULL)
            return (NULL);
#else
        /* 计算长度 */
        for (text += 2; *(int16u*)text != 0; text += move)
        {
            move = utf16le_to_ucs4(&ucs4, text);
            if (move == 0)
                goto _local;
        }

        /* 分配内存 */
        rett = (wide_t*)mem_malloc((leng_t)(text - strx));
        if (rett == NULL)
            return (NULL);
        len = 0;

        /* 转换编码 */
        for (strx += 2; *(int16u*)strx != 0; strx += move)
        {
            move = utf16le_to_ucs4(&ucs4, strx);
            len += ucs4_to_utf16be(&rett[len], ucs4) / sizeof(wide_t);
        }
        rett[len] = CR_NC(NIL);
#endif
        /* 返回结果 */
        if (codepage != NULL)
            *codepage = CR_UTF16LE;
        return (rett);
    }

    /* UTF-16BE */
    if (chr_cmpA(text, BOM_UTF16BE, 2) == 0)
    {
#if defined(_CR_ORDER_BE_)

        /* 直接复制 */
        rett = str_dupW((wide_t*)text + 1);
        if (rett == NULL)
            return (NULL);
#else
        /* 计算长度 */
        for (text += 2; *(int16u*)text != 0; text += move)
        {
            move = utf16be_to_ucs4(&ucs4, text);
            if (move == 0)
                goto _local;
        }

        /* 分配内存 */
        rett = (wide_t*)mem_malloc((leng_t)(text - strx));
        if (rett == NULL)
            return (NULL);
        len = 0;

        /* 转换编码 */
        for (strx += 2; *(int16u*)strx != 0; strx += move)
        {
            move = utf16be_to_ucs4(&ucs4, strx);
            len += ucs4_to_utf16le(&rett[len], ucs4) / sizeof(wide_t);
        }
        rett[len] = CR_NC(NIL);
#endif
        /* 返回结果 */
        if (codepage != NULL)
            *codepage = CR_UTF16BE;
        return (rett);
    }

    /* 无 BOM */
    if (is_xml)
    {
        /* XML 形式 */
        if (chr_cmpA(text, "<!-- encoding:", 14) == 0)
        {
            /* 指定了编码 */
            page = str2intA(text + 14, NULL);

            /* 跳过此注释 */
            text = skip_xml_cmtA(text);
            if (text != NULL)
            {
                /* 转换编码 (需外部库支持) */
                rett = local_to_utf16(page, text);
                if (rett == NULL)
                    return (NULL);

                /* 返回结果 */
                if (codepage != NULL)
                    *codepage = page;
                return (rett);
            }
        }
    }
    else
    {
        /* INI 形式 */
        if (chr_cmpA(text, "#encoding:", 10) == 0)
        {
            /* 指定了编码 */
            text += 10;
            page = str2intA(text, &len);

            /* 转换编码 (需外部库支持) */
            rett = local_to_utf16(page, text + len);
            if (rett == NULL)
                return (NULL);

            /* 返回结果 */
            if (codepage != NULL)
                *codepage = page;
            return (rett);
        }
    }

_local:
    /* 本地编码 (需外部库支持) */
    rett = local_to_utf16(CR_LOCAL, strx);
    if (rett == NULL)
        return (NULL);

    /* 返回结果 */
    if (codepage != NULL)
        *codepage = CR_LOCAL;
    return (rett);
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
