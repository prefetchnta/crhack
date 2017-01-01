/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-01-11  */
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
/*  >>>>>>>>>>>>>>> CrHack 字符串编码转换函数库 for Windows <<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "applib.h"
#include "memlib.h"
#include "strlib.h"

#include <windows.h>

/* 外挂的编码转换函数 */
extern cr_acp2uni_t g_str_acp2uni;
extern cr_uni2acp_t g_str_uni2acp;

/*
=======================================
    获取系统本地编码值
=======================================
*/
CR_API uint_t
get_sys_codepage (void_t)
{
    return (GetACP());
}

/*
=======================================
    多字节转 UNICODE
=======================================
*/
CR_API void_t*
str_acp2uni (
  __CR_IN__ uint_t          codepage,
  __CR_IN__ const ansi_t*   str,
  __CR_OT__ leng_t*         size,
  __CR_IN__ bool_t          use_utf16
    )
{
    sint_t  temp;
    leng_t  cnts;
    ansi_t* utf8;
    wide_t* wide;

    /* 使用外挂 */
    if (g_str_acp2uni != NULL)
        return (g_str_acp2uni(codepage, str, size, use_utf16));

    /* 输入必须是多字节编码 */
    if (is_cr_widechar(codepage))
        return (NULL);

    /* 转换到 UTF-16 编码 */
    temp = MultiByteToWideChar(codepage, 0, str, -1, NULL, 0);
    if (temp == 0)
        return (NULL);
    wide = str_allocW(temp);
    if (wide == NULL)
        return (NULL);
    cnts = MultiByteToWideChar(codepage, 0, str, -1, wide, temp);
    if (use_utf16) {
        if (size != NULL)
            *size = cnts * sizeof(wide_t);
        return (wide);
    }

    /* 转换到 UTF-8 编码 */
    temp = WideCharToMultiByte(CP_UTF8, 0, wide, -1, NULL, 0, NULL, NULL);
    if (temp == 0)
        goto _failure;
    utf8 = str_allocA(temp);
    if (utf8 == NULL)
        goto _failure;
    cnts = WideCharToMultiByte(CP_UTF8, 0, wide, -1, utf8, temp, NULL, NULL);
    if (size != NULL)
        *size = cnts * sizeof(ansi_t);
    mem_free(wide);
    return (utf8);

_failure:
    mem_free(wide);
    return (NULL);
}

/*
=======================================
    UNICODE 转多字节
=======================================
*/
CR_API ansi_t*
str_uni2acp (
  __CR_IN__ uint_t          codepage,
  __CR_IN__ const void_t*   str,
  __CR_OT__ leng_t*         size,
  __CR_IN__ bool_t          use_utf16
    )
{
    sint_t  temp;
    leng_t  cnts;
    ansi_t* ansi;
    wide_t* wide;

    /* 使用外挂 */
    if (g_str_uni2acp != NULL)
        return (g_str_uni2acp(codepage, str, size, use_utf16));

    /* 输入必须是多字节编码 */
    if (is_cr_widechar(codepage))
        return (NULL);

    if (!use_utf16)
    {
        /* 从 UTF-8 转出 */
        temp = MultiByteToWideChar(CP_UTF8, 0, (ansi_t*)str, -1, NULL, 0);
        if (temp == 0)
            return (NULL);
        wide = str_allocW(temp);
        if (wide == NULL)
            return (NULL);
        MultiByteToWideChar(CP_UTF8, 0, (ansi_t*)str, -1, wide, temp);
    }
    else
    {
        /* 从 UTF-16 转出 */
        wide = (wide_t*)str;
    }

    temp = WideCharToMultiByte(codepage, 0, wide, -1, NULL, 0, NULL, NULL);
    if (temp == 0)
        goto _failure;
    ansi = str_allocA(temp);
    if (ansi == NULL)
        goto _failure;
    cnts = WideCharToMultiByte(codepage, 0, wide, -1, ansi, temp, NULL, NULL);
    if (size != NULL)
        *size = cnts * sizeof(ansi_t);
    if (!use_utf16)
        mem_free(wide);
    return (ansi);

_failure:
    if (!use_utf16)
        mem_free(wide);
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
