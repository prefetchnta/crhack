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
/*  >>>>>>>>>>>>>>>>>>>> CrHack 字符串工具函数库解析组W <<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#define _CR_BUILD_WIDE_
#include "memlib.h"
#include "morder.h"
#include "strlib.h"

/* 不支持宽字符串 */
#if defined(_CR_NO_WIDE_)
    #define _CR_NO_WIDE_FUNC_
#endif
#if defined(_CR_NO_WIDE_FUNC_)

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

#include "templ/strtool.inl"

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
