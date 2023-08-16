/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2023-08-16  */
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
/*  >>>>>>>>>>>>>>>>>>>> CrHack 半/全角字符串转换函数库 <<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "memlib.h"
#include "strlib.h"

/*
=======================================
    半角字符转全角字符
=======================================
*/
CR_API void_t*
str_half2full (
  __CR_IO__ const void_t*   half,
  __CR_IN__ uint_t          codepage
    )
{
    wide_t* ptr;
    ansi_t* temp;
    wide_t* utf16;

    if (codepage == CR_UTF16X)
        utf16 = str_dupW((wide_t*)half);
    else
        utf16 = str_acp2uni(codepage, (ansi_t*)half, NULL, TRUE);
    if (utf16 == NULL)
        return (NULL);

    /* 空格比较特殊, 其他一一对应 */
    for (ptr = utf16; *ptr != CR_NC(NIL); ptr++)
    {
        if (*ptr == CR_WC(' '))
            *ptr  = 0x3000;
        else
        if (*ptr >= CR_WC('!') && *ptr <= CR_WC('~'))
            *ptr += 0xFEE0;
    }

    /* 无需转换, 直接返回 */
    if (codepage == CR_UTF16X)
        return ((void_t*)utf16);

    /* 完成后转换回本地编码 */
    temp = str_uni2acp(codepage, utf16, NULL, TRUE);
    mem_free(utf16);
    return ((void_t*)temp);
}

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
