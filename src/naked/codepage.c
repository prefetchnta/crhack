/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2016-03-24  */
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
/*  >>>>>>>>>>>>>>>> CrHack 字符串编码转换函数库 for Naked <<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "applib.h"
#include "memlib.h"
#include "strlib.h"

/* 使用自定义的编码值 */
extern uint_t   g_codepage;

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
    return (g_codepage);
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
    leng_t  cnts;
    ansi_t* utf8;
    wide_t* wide;

    /* 使用外挂 */
    if (g_str_acp2uni != NULL)
        return (g_str_acp2uni(codepage, str, size, use_utf16));

    /* 输入必须是多字节编码 */
    if (is_cr_widechar(codepage))
        return (NULL);
    if (codepage == CR_LOCAL)
        codepage = g_codepage;

    if (use_utf16) {
        if (codepage == CR_LOCAL)
            wide = str_char2wide(str);
        else
        if (codepage == CR_UTF8)
            wide = utf8_to_utf16(str);
        else
            return (NULL);
        if (wide == NULL)
            return (NULL);
        if (size != NULL)
            *size = str_sizeW(wide);
        return (wide);
    }

    if (codepage != CR_LOCAL && codepage != CR_UTF8)
        return (NULL);
    cnts = str_sizeA(str);
    utf8 = (ansi_t*)mem_dup(str, cnts);
    if (utf8 == NULL)
        return (NULL);
    if (size != NULL)
        *size = cnts;
    return (utf8);
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
    leng_t  cnts;
    ansi_t* ansi;

    /* 使用外挂 */
    if (g_str_uni2acp != NULL)
        return (g_str_uni2acp(codepage, str, size, use_utf16));

    /* 输入必须是多字节编码 */
    if (is_cr_widechar(codepage))
        return (NULL);
    if (codepage == CR_LOCAL)
        codepage = g_codepage;

    if (use_utf16) {
        if (codepage == CR_LOCAL)
            ansi = str_wide2char((wide_t*)str);
        else
        if (codepage == CR_UTF8)
            ansi = utf16_to_utf8((wide_t*)str);
        else
            return (NULL);
        if (ansi == NULL)
            return (NULL);
        if (size != NULL)
            *size = str_sizeA(ansi);
        return (ansi);
    }

    if (codepage != CR_LOCAL && codepage != CR_UTF8)
        return (NULL);
    cnts = str_sizeA((ansi_t*)str);
    ansi = (ansi_t*)mem_dup((ansi_t*)str, cnts);
    if (ansi == NULL)
        return (NULL);
    if (size != NULL)
        *size = cnts;
    return (ansi);
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
