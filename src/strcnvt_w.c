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
/*  >>>>>>>>>>>>>>>>>>>> CrHack 字符串工具函数库转换组W <<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#define _CR_BUILD_WIDE_
#include "datlib.h"
#include "strlib.h"

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

#endif  /* _CR_NO_WIDE_FUNC_ */

/* 字符转换用查表 */
static const wide_t _rom_ s_hex2asc[] =
{
    CR_WC('0'), CR_WC('1'), CR_WC('2'), CR_WC('3'),
    CR_WC('4'), CR_WC('5'), CR_WC('6'), CR_WC('7'),
    CR_WC('8'), CR_WC('9'), CR_WC('A'), CR_WC('B'),
    CR_WC('C'), CR_WC('D'), CR_WC('E'), CR_WC('F'),
};

#include "templ/strcnvt.inl"

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
