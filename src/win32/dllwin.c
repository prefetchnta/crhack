/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2009-12-17  */
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
/*  >>>>>>>>>>>>>>>>>> CrHack 动态模块函数库 for Windows <<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "plugin.h"

#include <windows.h>

/*
=======================================
    加载动态模块A
=======================================
*/
CR_API sbin_t
sbin_loadA (
  __CR_IN__ const ansi_t*   name
    )
{
    return ((sbin_t)LoadLibraryA(name));
}

/*
=======================================
    加载动态模块W
=======================================
*/
CR_API sbin_t
sbin_loadW (
  __CR_IN__ const wide_t*   name
    )
{
    return ((sbin_t)LoadLibraryW(name));
}

/*
=======================================
    卸载动态模块
=======================================
*/
CR_API bool_t
sbin_unload (
  __CR_IN__ sbin_t  sbin
    )
{
    if (!FreeLibrary((HMODULE)sbin))
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    获取模块导出项地址
=======================================
*/
CR_API void_t*
sbin_export (
  __CR_IN__ sbin_t          sbin,
  __CR_IN__ const ansi_t*   name
    )
{
#if defined(_CR_OS_WINCE_)
    return ((void_t*)GetProcAddressA((HMODULE)sbin, name));
#else
    return ((void_t*)GetProcAddress((HMODULE)sbin, name));
#endif
}

/*
=======================================
    获取已加载的模块A
=======================================
*/
CR_API sbin_t
sbin_testA (
  __CR_IN__ const ansi_t*   name
    )
{
    return ((sbin_t)GetModuleHandleA(name));
}

/*
=======================================
    获取已加载的模块W
=======================================
*/
CR_API sbin_t
sbin_testW (
  __CR_IN__ const wide_t*   name
    )
{
    return ((sbin_t)GetModuleHandleW(name));
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
