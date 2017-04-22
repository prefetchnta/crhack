/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-03-04  */
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
/*  >>>>>>>>>>>>>>>>>>> CrHack 动态模块函数库 for POSIX <<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "memlib.h"
#include "plugin.h"
#include "strlib.h"

#include <dlfcn.h>

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
    return ((sbin_t)dlopen(name, RTLD_NOW));
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
    void_t* so;
    ansi_t* ansi;

    ansi = utf16_to_local(CR_LOCAL, name);
    if (ansi == NULL)
        return (NULL);
    so = dlopen(ansi, RTLD_NOW);
    mem_free(ansi);
    return ((sbin_t)so);
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
    if (dlclose((void_t*)sbin) != 0)
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
    return (dlsym((void_t*)sbin, name));
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
#if (defined(_CR_NDK_LOW_)) || \
    (!defined(_CR_OS_ANDROID_) && !defined(RTLD_NOLOAD))
    CR_NOUSE(name);
    return (NULL);
#else
    return ((sbin_t)dlopen(name, RTLD_NOLOAD));
#endif
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
#if (defined(_CR_NDK_LOW_)) || \
    (!defined(_CR_OS_ANDROID_) && !defined(RTLD_NOLOAD))
    CR_NOUSE(name);
    return (NULL);
#else
    void_t* so;
    ansi_t* ansi;

    ansi = utf16_to_local(CR_LOCAL, name);
    if (ansi == NULL)
        return (NULL);
    so = dlopen(ansi, RTLD_NOLOAD);
    mem_free(ansi);
    return ((sbin_t)so);
#endif
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
