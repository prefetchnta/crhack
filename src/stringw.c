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
/*  >>>>>>>>>>>>>>>>>>>>>>> CrHack 字符串操作函数库W <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#define _CR_BUILD_WIDE_
#include "datlib.h"
#include "morder.h"
#include "parser.h"
#include "strlib.h"

#define sINIx   sINIw
#define sXMLx   sXMLw
#define sXNODEx sXNODEw

/* 不支持宽字符串 */
#if defined(_CR_NO_WIDE_)
    #define _CR_NO_WIDE_FUNC_
#endif
#if defined(_CR_NO_WIDE_FUNC_)

/* for L"true" */
static const wide_t _rom_ s_wcs_true[] =
{
    CR_WC('t'), CR_WC('r'), CR_WC('u'),
    CR_WC('e'), CR_WC('\0'),
};

/* for L"false" */
static const wide_t _rom_ s_wcs_false[] =
{
    CR_WC('f'), CR_WC('a'), CR_WC('l'),
    CR_WC('s'), CR_WC('e'), CR_WC('\0'),
};

/* for L"\"\"\"" */
static const wide_t _rom_ s_wcs_raws[] =
{
    CR_WC('\"'), CR_WC('\"'), CR_WC('\"'),
    CR_WC('\0'),
};

/* for L"<!--" */
static const wide_t _rom_ s_wcs_xcmts1[] =
{
    CR_WC('<'), CR_WC('!'), CR_WC('-'),
    CR_WC('-'), CR_WC('\0'),
};

/* for L"-->" */
static const wide_t _rom_ s_wcs_xcmte1[] =
{
    CR_WC('-'), CR_WC('-'), CR_WC('>'),
    CR_WC('\0'),
};

/* for L"<![CDATA[" */
static const wide_t _rom_ s_wcs_xcmts2[] =
{
    CR_WC('<'), CR_WC('!'), CR_WC('['),
    CR_WC('C'), CR_WC('D'), CR_WC('A'),
    CR_WC('T'), CR_WC('A'), CR_WC('['),
    CR_WC('\0'),
};

/* for L"]]>" */
static const wide_t _rom_ s_wcs_xcmte2[] =
{
    CR_WC(']'), CR_WC(']'), CR_WC('>'),
    CR_WC('\0'),
};

/* for L"*-/" */
static const wide_t _rom_ s_wcs_ccmte[] =
{
    CR_WC('*'), CR_WC('/'), CR_WC('\0'),
};

/* for L"://" */
static const wide_t _rom_ s_wcs_scheme[] =
{
    CR_WC(':'), CR_WC('/'), CR_WC('/'),
    CR_WC('\0'),
};

#endif  /* _CR_NO_WIDE_FUNC_ */

/* 字符转换用查表 */
static const wide_t _rom_ s_hex2asc[] =
{
    CR_WC('0'), CR_WC('1'), CR_WC('2'), CR_WC('3'),
    CR_WC('4'), CR_WC('5'), CR_WC('6'), CR_WC('7'),
    CR_WC('8'), CR_WC('9'), CR_WC('A'), CR_WC('B'),
    CR_WC('C'), CR_WC('D'), CR_WC('E'), CR_WC('F'),
};

#include "templ/ctype.inl"
#include "templ/strini.inl"
#include "templ/strxml.inl"
#include "templ/strbase.inl"
#include "templ/strcnvt.inl"
#include "templ/strhtml.inl"
#include "templ/strpath.inl"
#include "templ/strtool.inl"

/*
=======================================
    全角字符转半角字符
=======================================
*/
CR_API void_t*
str_full2half (
  __CR_IO__ const void_t*   full,
  __CR_IN__ uint_t          codepage
    )
{
    wide_t* ptr;
    ansi_t* temp;
    wide_t* utf16;

    if (codepage == CR_UTF16X)
        utf16 = str_dupW((wide_t*)full);
    else
        utf16 = str_acp2uni(codepage, (ansi_t*)full, NULL, TRUE);
    if (utf16 == NULL)
        return (NULL);

    /* 空格比较特殊, 其他一一对应 */
    for (ptr = utf16; *ptr != CR_NC(NIL); ptr++)
    {
        if (*ptr == 0x3000)
            *ptr = CR_WC(' ');
        else
        if (*ptr >= 0xFF01 && *ptr <= 0xFF5E)
            *ptr -= 0xFEE0;
    }

    /* 无需转换, 直接返回 */
    if (codepage == CR_UTF16X)
        return ((void_t*)utf16);

    /* 完成后转换回本地编码 */
    temp = str_uni2acp(codepage, utf16, NULL, TRUE);
    mem_free(utf16);
    return ((void_t*)temp);
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
