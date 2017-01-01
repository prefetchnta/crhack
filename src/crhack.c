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
/*  >>>>>>>>>>>>>>>>>>>>>>> CRHACK/CORE 主程序入口点 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "crhack.h"

#ifndef _CR_NO_STDC_
    #include <stdlib.h>
#endif

/* CRHACK/CORE 是否已初始化 */
static bool_t   s_crh_okay = FALSE;

/*------------------------------------------------*/

/* DLL 自定义入口点 */
#if defined(_CR_BUILD_DLL_)
#if defined(_CR_OS_MSWIN_)
/*
=======================================
    CRHACK/CORE DLL 入口点
=======================================
*/
#if defined(_CR_CC_BCC_)
int  WINAPI
DllEntryPoint (
  __CR_IN__ HINSTANCE       hinst,
  __CR_IN__ unsigned long   reason,
  __CR_UU__ void*           reserved
    )
#else
BOOL WINAPI
DllMain (
  __CR_IN__ HANDLE  hinst,
  __CR_IN__ DWORD   reason,
  __CR_UU__ LPVOID  reserved
    )
#endif
{
    switch (reason)
    {
        case DLL_PROCESS_ATTACH:
            s_crh_okay = FALSE;
            crhack_core_init();
            break;

        case DLL_PROCESS_DETACH:
            crhack_core_free();
            break;
    }
    CR_NOUSE(hinst);
    CR_NOUSE(reserved);
    return (TRUE);
}
#elif   defined(_CR_CC_GCC_)
void_t  so_core_init (void_t) __attribute__((constructor));
void_t  so_core_init (void_t)
{
    s_crh_okay = FALSE;
    crhack_core_init();
}
void_t  so_core_fini (void_t) __attribute__((destructor));
void_t  so_core_fini (void_t)
{
    crhack_core_free();
}
#endif  /* _CR_OS_MSWIN_ */
#endif  /* _CR_BUILD_DLL_ */

/*------------------------------------------------*/

/* 线程模型标识串 */
#ifndef _CR_NO_MT_
    #define _CR_THREAD_STR_ "-MT"
#else
    #define _CR_THREAD_STR_ "-ST"
#endif

/*
=======================================
    返回编译器版本值
=======================================
*/
CR_API uint_t
crhack_comp_vers (void_t)
{
    return (_CR_CC_VER_);
}

/*
=======================================
    返回编译器名称串
=======================================
*/
CR_API const ansi_t*
crhack_comp_name (void_t)
{
    return (_CR_CC_STR_ _CR_OS_STR_
            _CR_THREAD_STR_ "-REL::"
            _CR_AR_STR_ _CR_SYS_STR_
            _CR_MM_STR_ _CR_ORDER_STR_);
}

/*
=======================================
    返回编译日期时间
=======================================
*/
CR_API const ansi_t*
crhack_date_time (void_t)
{
    return (__DATE__ " @ " __TIME__);
}

/*
=======================================
    CRHACK/CORE 初始化
=======================================
*/
CR_API bool_t
crhack_core_init (void_t)
{
    if (!s_crh_okay)
    {
        mem_init(malloc, free);
        mem_reset();

        sio_init();
        rand_seed(0);
        error_hook(NULL);

        quit_set(NULL);
        set_msg_callA(NULL);
        set_msg_callW(NULL);

        set_gui_hwnd(NULL);
        set_app_type(CR_APP_GUI);
        set_sys_codepage(CR_UTF8);
        set_str_acp2uni(NULL);
        set_str_uni2acp(NULL);

        s_crh_okay = TRUE;
        return (TRUE);
    }
    return (FALSE);
}

/*
=======================================
    CRHACK/CORE 释放
=======================================
*/
CR_API void_t
crhack_core_free (void_t)
{
    if (s_crh_okay) {
        sio_free();
        s_crh_okay = FALSE;
    }
}

/*------------------------------------------------*/

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
