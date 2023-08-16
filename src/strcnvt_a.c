/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-01-06  */
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
/*  >>>>>>>>>>>>>>>>>>>> CrHack 字符串工具函数库转换组A <<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#define _CR_BUILD_ANSI_
#include "datlib.h"
#include "strlib.h"

/* 字符转换用查表 */
static const ansi_t _rom_ s_hex2asc[] =
{
    CR_AC('0'), CR_AC('1'), CR_AC('2'), CR_AC('3'),
    CR_AC('4'), CR_AC('5'), CR_AC('6'), CR_AC('7'),
    CR_AC('8'), CR_AC('9'), CR_AC('A'), CR_AC('B'),
    CR_AC('C'), CR_AC('D'), CR_AC('E'), CR_AC('F'),
};

#include "templ/strcnvt.inl"

/*
=======================================
    URL 编码
=======================================
*/
CR_API ansi_t*
url_encode (
  __CR_IN__ const ansi_t*   str,
  __CR_IN__ bool_t          slash
    )
{
    byte_t  cha;
    leng_t  idx, len;
    ansi_t  *url, *ptr;

    /* 分配足够多的空间 */
    len = str_lenA(str);
    url = str_allocA(len * 3 + 1);
    if (url == NULL)
        return (NULL);
    ptr = url;

    /* 开始转换编码 */
    for (idx = 0; idx < len; idx++) {
        cha = *str++;   /* ~ 也转义掉 */
        if (is_alnumA(cha) || cha == CR_AC('-') ||
            cha == CR_AC('_') || cha == CR_AC('.') ||
            (cha == CR_AC('/') && !slash)) {
            *ptr++ = (ansi_t)cha;
        }
        else {
            *ptr++ = CR_AC('%');
            *ptr++ = s_hex2asc[cha >>   4];
            *ptr++ = s_hex2asc[cha & 0x0F];
        }
    }
    *ptr++ = CR_AC(NIL);
    len = (leng_t)(ptr - url);

    /* 重新分配字符串 */
    ptr = str_allocA(len);
    if (ptr == NULL) {
        mem_free(url);
        return (NULL);
    }
    mem_cpy(ptr, url, len);
    mem_free(url);
    return (ptr);
}

/*
=======================================
    URL 解码
=======================================
*/
CR_API ansi_t*
url_decode (
  __CR_IN__ const ansi_t*   str
    )
{
    byte_t  val;
    leng_t  idx, len;
    ansi_t  cha, *url, *ptr;

    /* 分配足够多的空间 */
    len = str_lenA(str);
    url = str_allocA(len + 1);
    if (url == NULL)
        return (NULL);
    ptr = url;

    /* 开始转换编码 */
    for (idx = 0; idx < len; idx++) {
        cha = *str++;
        if (cha == CR_AC('+')) {
            *ptr++ = CR_AC(' ');
        }
        else
        if (cha == CR_AC('%')) {
            if (idx + 3 > len)
                goto _failure;
            if (is_digitA(*str)) {
                val = (byte_t)((0x0F & *str++));
            }
            else
            if (is_xnumbA(*str)) {
                val = (byte_t)((0x0F & *str++) + 9);
            }
            else {
                goto _failure;
            }

            if (is_digitA(*str)) {
                val <<= 4;
                val |= (byte_t)((0x0F & *str++));
            }
            else
            if (is_xnumbA(*str)) {
                val <<= 4;
                val |= (byte_t)((0x0F & *str++) + 9);
            }
            else {
                goto _failure;
            }
            *ptr++ = (ansi_t)val;
            idx += 2;
        }
        else {
            *ptr++ = cha;
        }
    }
    *ptr++ = CR_AC(NIL);
    len = (leng_t)(ptr - url);

    /* 重新分配字符串 */
    ptr = str_allocA(len);
    if (ptr == NULL)
        goto _failure;
    mem_cpy(ptr, url, len);
    mem_free(url);
    return (ptr);

_failure:
    mem_free(url);
    return (NULL);
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
