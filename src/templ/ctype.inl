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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack 字符类型函数库模板 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "cmask.h"

/*
=======================================
    数字字母T
=======================================
*/
CR_API bool_t
CR_FAW(is_alnum) (
  __CR_IN__ XCHAR   ch
    )
{
#if defined(_CR_BUILD_WIDE_)
    if (ch > 255)
        return (FALSE);
#endif
    return (((__ismask(ch) & (_U|_L|_D)) != 0) ? TRUE : FALSE);
}

/*
=======================================
    字母T
=======================================
*/
CR_API bool_t
CR_FAW(is_alpha) (
  __CR_IN__ XCHAR   ch
    )
{
#if defined(_CR_BUILD_WIDE_)
    if (ch > 255)
        return (FALSE);
#endif
    return (((__ismask(ch) & (_U|_L)) != 0) ? TRUE : FALSE);
}

/*
=======================================
    控制字符T
=======================================
*/
CR_API bool_t
CR_FAW(is_cntrl) (
  __CR_IN__ XCHAR   ch
    )
{
#if defined(_CR_BUILD_WIDE_)
    if (ch > 255)
        return (FALSE);
#endif
    return (((__ismask(ch) & (_C)) != 0) ? TRUE : FALSE);
}

/*
=======================================
    数字T
=======================================
*/
CR_API bool_t
CR_FAW(is_digit) (
  __CR_IN__ XCHAR   ch
    )
{
#if defined(_CR_BUILD_WIDE_)
    if (ch > 255)
        return (FALSE);
#endif
    return (((__ismask(ch) & (_D)) != 0) ? TRUE : FALSE);
}

/*
=======================================
    图形字符T
=======================================
*/
CR_API bool_t
CR_FAW(is_graph) (
  __CR_IN__ XCHAR   ch
    )
{
#if defined(_CR_BUILD_WIDE_)
    if (ch > 255)
        return (FALSE);
#endif
    return (((__ismask(ch) & (_P|_U|_L|_D)) != 0) ? TRUE : FALSE);
}

/*
=======================================
    小写字母T
=======================================
*/
CR_API bool_t
CR_FAW(is_lower) (
  __CR_IN__ XCHAR   ch
    )
{
#if defined(_CR_BUILD_WIDE_)
    if (ch > 255)
        return (FALSE);
#endif
    return (((__ismask(ch) & (_L)) != 0) ? TRUE : FALSE);
}

/*
=======================================
    显示字符T
=======================================
*/
CR_API bool_t
CR_FAW(is_print) (
  __CR_IN__ XCHAR   ch
    )
{
#if defined(_CR_BUILD_WIDE_)
    if (ch > 255)
        return (FALSE);
#endif
    return (((__ismask(ch) & (_P|_U|_L|_D|_SP)) != 0) ? TRUE : FALSE);
}

/*
=======================================
    标点字符T
=======================================
*/
CR_API bool_t
CR_FAW(is_punct) (
  __CR_IN__ XCHAR   ch
    )
{
#if defined(_CR_BUILD_WIDE_)
    if (ch > 255)
        return (FALSE);
#endif
    return (((__ismask(ch) & (_P)) != 0) ? TRUE : FALSE);
}

/*
=======================================
    空白字符T
=======================================
*/
CR_API bool_t
CR_FAW(is_space) (
  __CR_IN__ XCHAR   ch
    )
{
#if defined(_CR_BUILD_WIDE_)
    if (ch > 255)
        return (FALSE);
#endif
    return (((__ismask(ch) & (_S)) != 0) ? TRUE : FALSE);
}

/*
=======================================
    大写字母T
=======================================
*/
CR_API bool_t
CR_FAW(is_upper) (
  __CR_IN__ XCHAR   ch
    )
{
#if defined(_CR_BUILD_WIDE_)
    if (ch > 255)
        return (FALSE);
#endif
    return (((__ismask(ch) & (_U)) != 0) ? TRUE : FALSE);
}

/*
=======================================
    十六进制T
=======================================
*/
CR_API bool_t
CR_FAW(is_xnumb) (
  __CR_IN__ XCHAR   ch
    )
{
#if defined(_CR_BUILD_WIDE_)
    if (ch > 255)
        return (FALSE);
#endif
    return (((__ismask(ch) & (_D|_H)) != 0) ? TRUE : FALSE);
}

/*
=======================================
    截断变量T
=======================================
*/
CR_API bool_t
CR_FAW(is_break) (
  __CR_IN__ XCHAR   ch
    )
{
    return (!CR_FAW(is_wordz)(ch));
}

/*
=======================================
    变量字符T
=======================================
*/
CR_API bool_t
CR_FAW(is_wordz) (
  __CR_IN__ XCHAR   ch
    )
{
    if (CR_FAW(is_alnum)(ch))
        return (TRUE);
    return ((ch == CR_XC('_')) ? TRUE : FALSE);
}

/*
=======================================
    时间分割T
=======================================
*/
CR_API bool_t
CR_FAW(is_times) (
  __CR_IN__ XCHAR   ch
    )
{
    return ((ch == CR_XC(':')) ? TRUE : FALSE);
}

/*
=======================================
    日期分割T
=======================================
*/
CR_API bool_t
CR_FAW(is_dates) (
  __CR_IN__ XCHAR   ch
    )
{
    return ((ch == CR_XC('-') ||
             ch == CR_XC('/')) ? TRUE : FALSE);
}

/*
=======================================
    路径分割T
=======================================
*/
CR_API bool_t
CR_FAW(is_slash) (
  __CR_IN__ XCHAR   ch
    )
{
#if !defined(_CR_OS_MACOSH_)
    return ((ch == CR_XC('/') ||
             ch == CR_XC('\\')) ? TRUE : FALSE);
#else
    return ((ch == CR_XC('/') ||
             ch == CR_XC('\\') ||
             ch == CR_XC(':')) ? TRUE : FALSE);
#endif
}

/*
=======================================
    车牌符号T
=======================================
*/
CR_API bool_t
CR_FAW(is_plate) (
  __CR_IN__ XCHAR   ch
    )
{
#if defined(_CR_BUILD_WIDE_)
    if (ch > 255)
        return (FALSE);
#endif
    if ((__ismask(ch) & (_U|_D)) == 0)
        return (FALSE);
    if (ch == CR_XC('I') || ch == CR_XC('O'))
        return (FALSE);
    return (TRUE);
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
