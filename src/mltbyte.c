/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2012-07-13  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>> CrHack 多字节编码函数库 <<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "memlib.h"
#include "strlib.h"

/*****************************************************************************/
/*                               字符长度计算                                */
/*****************************************************************************/

/*
=======================================
    GBK 字符长度计算
=======================================
*/
CR_API uint_t
offset_gbk (
  __CR_IN__ const ansi_t*   str
    )
{
    byte_t  cha;

    cha = str[0];
    if (cha <= 0x7F)
        return (1);
    if (cha >= 0x81 && cha <= 0xFE) {
        cha = str[1];
        if (cha >= 0x40 && cha <= 0xFE)
            return (2);
    }
    return (0);
}

/*
=======================================
    UTF-8 字符长度计算
=======================================
*/
CR_API uint_t
offset_utf8 (
  __CR_IN__ const ansi_t*   str
    )
{
    uint_t  offs;
    int32u  ucs4;

    offs = utf8_to_ucs4(&ucs4, str);
    return (offs);
}

/*
=======================================
    BIG-5 字符长度计算
=======================================
*/
CR_API uint_t
offset_big5 (
  __CR_IN__ const ansi_t*   str
    )
{
    byte_t  cha;

    cha = str[0];
    if (cha <= 0x7F)
        return (1);
    if (cha >= 0x81 && cha <= 0xFE) {
        cha = str[1];
        if ((cha >= 0x40 && cha <= 0x7E) ||
            (cha >= 0xA1 && cha <= 0xFE))
            return (2);
    }
    return (0);
}

/*
=======================================
    Shift-JIS 字符长度计算
=======================================
*/
CR_API uint_t
offset_sjis (
  __CR_IN__ const ansi_t*   str
    )
{
    byte_t  cha;

    cha = str[0];
    if (cha <= 0x7E)
        return (1);
    if (cha >= 0xA1 && cha <= 0xDF)
        return (1);
    if ((cha >= 0x81 && cha <= 0x9F) ||
        (cha >= 0xE0 && cha <= 0xEF)) {
        cha = str[1];
        if ((cha >= 0x40 && cha <= 0x7E) ||
            (cha >= 0x80 && cha <= 0xFC))
            return (2);
    }
    return (0);
}

/*
=======================================
    EUC-JP 字符长度计算
=======================================
*/
CR_API uint_t
offset_eucjp (
  __CR_IN__ const ansi_t*   str
    )
{
    byte_t  cha;

    cha = str[0];
    if (cha <= 0x7E)
        return (1);
    if (cha >= 0xA1 && cha <= 0xFE) {
        cha = str[1];
        if (cha >= 0xA1 && cha <= 0xFE)
            return (2);
        return (0);
    }
    if (cha == 0x8E) {
        cha = str[1];
        if (cha >= 0xA1 && cha <= 0xDF)
            return (2);
        return (0);
    }
    if (cha == 0x8F) {
        cha = str[1];
        if (cha >= 0xA1 && cha <= 0xFE) {
            cha = str[2];
            if (cha >= 0xA1 && cha <= 0xFE)
                return (3);
        }
    }
    return (0);
}

/*
=======================================
    ASCII 字符长度计算
=======================================
*/
CR_API uint_t
offset_ascii (
  __CR_IN__ const ansi_t*   str
    )
{
    byte_t  cha = str[0];

    return ((cha <= 0x7F) ? 1 : 0);
}

/*
=======================================
    GB2312 字符长度计算
=======================================
*/
CR_API uint_t
offset_gb2312 (
  __CR_IN__ const ansi_t*   str
    )
{
    byte_t  cha;

    cha = str[0];
    if (cha <= 0x7F)
        return (1);
    if (cha >= 0xA1 && cha <= 0xFE) {
        cha = str[1];
        if (cha >= 0xA1 && cha <= 0xFE)
            return (2);
    }
    return (0);
}

/*
=======================================
    GB18030
=======================================
*/
CR_API uint_t
offset_gb18030 (
  __CR_IN__ const ansi_t*   str
    )
{
    byte_t  cha;

    cha = str[0];
    if (cha <= 0x7F)
        return (1);
    if (cha >= 0x81 && cha <= 0xFE) {
        cha = str[1];
        if (cha >= 0x40 && cha <= 0xFE)
            return (2);
        if (cha >= 0x30 && cha <= 0x39) {
            cha = str[2];
            if (cha >= 0x81 && cha <= 0xFE) {
                cha = str[3];
                if (cha >= 0x30 && cha <= 0x39)
                    return (4);
            }
        }
    }
    return (0);
}

/*****************************************************************************/
/*                               其他杂项功能                                */
/*****************************************************************************/

/*
=======================================
    获取字符的个数
=======================================
*/
CR_API leng_t
str_length (
  __CR_IN__ const ansi_t*   str,
  __CR_IN__ offs_cha_t      get_offs
    )
{
    uint_t  step;
    leng_t  len = 0;

    while (*str != CR_AC(NIL)) {
        step = get_offs(str);
        if (step == 0)
            step = 1;
        len += 1;
        str += step;
    }
    return (len);
}

/*
=======================================
    文本自动断行 (单一宽度字体)
=======================================
*/
CR_API ansi_t*
text_wrap1 (
  __CR_IN__ const ansi_t*   text,
  __CR_IN__ uint_t          count,
  __CR_IN__ offs_cha_t      get_offs
    )
{
    leng_t  after = 0;
    ansi_t* string, cha;
    uint_t  step, line = 0;
    ansi_t* ptr = (ansi_t*)text;
    leng_t  carry = str_lenA(CR_NL);

    /* 预计算空间大小 */
    while (*ptr != CR_AC(NIL))
    {
        /* 获取字符个数 */
        step = get_offs(ptr);
        if (step == 0)
            step = 1;

        /* 达到指定个数后插入换行 */
        if (*ptr != CR_AC('\n') &&
            *ptr != CR_AC('\r')) {
            line += 1;
            if (line >= count) {
                line = 0;
                after += carry;
            }
        }
        else {
            line = 0;
        }
        ptr   += step;
        after += step;
    }

    /* 创建一个新的字符串 */
    string = str_allocA(after + 1);
    if (string == NULL)
        return (NULL);

    /* 开始复制字符串 */
    line = 0;
    ptr = string;
    while (*text != CR_AC(NIL))
    {
        /* 获取字符个数 */
        cha = text[0];
        step = get_offs(text);
        if (step == 0)
            step = 1;
        mem_cpy(ptr, text, step);
        ptr  += step;
        text += step;

        /* 达到指定个数后插入换行 */
        if (cha != CR_AC('\n') &&
            cha != CR_AC('\r')) {
            line += 1;
            if (line >= count) {
                line = 0;
                mem_cpy(ptr, CR_NL, carry);
                ptr += carry;
            }
        }
        else {
            line = 0;
        }
    }
    *ptr = CR_AC(NIL);
    return (string);
}

/*
=======================================
    文本自动断行 (单/双倍宽度字体)
=======================================
*/
CR_API ansi_t*
text_wrap2 (
  __CR_IN__ const ansi_t*   text,
  __CR_IN__ uint_t          count,
  __CR_IN__ offs_cha_t      get_offs
    )
{
    leng_t  after = 0;
    ansi_t* string, cha;
    uint_t  step, line = 0;
    ansi_t* ptr = (ansi_t*)text;
    leng_t  carry = str_lenA(CR_NL);

    /* 预计算空间大小 */
    while (*ptr != CR_AC(NIL))
    {
        /* 获取字符个数 */
        step = get_offs(ptr);
        if (step == 0)
            step = 1;

        /* 达到指定个数后插入换行 */
        if (*ptr != CR_AC('\n') &&
            *ptr != CR_AC('\r')) {
            if (step == 1)
                line += 1;
            else
                line += 2;
            if (line >= count) {
                line = 0;
                after += carry;
            }
        }
        else {
            line = 0;
        }
        ptr   += step;
        after += step;
    }

    /* 创建一个新的字符串 */
    string = str_allocA(after + 1);
    if (string == NULL)
        return (NULL);

    /* 开始复制字符串 */
    line = 0;
    ptr = string;
    while (*text != CR_AC(NIL))
    {
        /* 获取字符个数 */
        cha = text[0];
        step = get_offs(text);
        if (step == 0)
            step = 1;
        mem_cpy(ptr, text, step);
        ptr  += step;
        text += step;

        /* 达到指定个数后插入换行 */
        if (cha != CR_AC('\n') &&
            cha != CR_AC('\r')) {
            if (step == 1)
                line += 1;
            else
                line += 2;
            if (line >= count) {
                line = 0;
                mem_cpy(ptr, CR_NL, carry);
                ptr += carry;
            }
        }
        else {
            line = 0;
        }
    }
    *ptr = CR_AC(NIL);
    return (string);
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
