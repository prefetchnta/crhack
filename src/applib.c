/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2009-12-16  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 应用程序函数库 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "applib.h"
#include "msclib.h"

/* 应用程序相关全局变量 */
quit_t      g_quit_now = NULL;          /* 自定义的退出函数 */
uint_t      g_app_type = CR_APP_GUI;    /* 应用程序类型指定 */
hwnd_t      g_gui_hwnd = NULL;          /* 应用程序窗口句柄 */
msgboxA_t   g_msg_boxA = NULL;          /* 自定义消息窗口回调A */
msgboxW_t   g_msg_boxW = NULL;          /* 自定义消息窗口回调W */
uint_t      g_codepage = CR_UTF8;       /* 默认使用 UTF-8 编码 */

/* 外挂的编码转换函数 */
cr_acp2uni_t    g_str_acp2uni = NULL;
cr_uni2acp_t    g_str_uni2acp = NULL;

/*
=======================================
    设置退出回调
=======================================
*/
CR_API void_t
quit_set (
  __CR_IN__ quit_t  func
    )
{
    g_quit_now = func;
}

/*
=======================================
    设置 GUI 窗口句柄
=======================================
*/
CR_API void_t
set_gui_hwnd (
  __CR_IN__ hwnd_t  hwnd
    )
{
    g_gui_hwnd = hwnd;
}

/*
=======================================
    设置消息提示调用A
=======================================
*/
CR_API void_t
set_msg_callA (
  __CR_IN__ msgboxA_t   func
    )
{
    g_msg_boxA = func;
}

/*
=======================================
    设置消息提示调用W
=======================================
*/
CR_API void_t
set_msg_callW (
  __CR_IN__ msgboxW_t   func
    )
{
    g_msg_boxW = func;
}

/*
=======================================
    设置系统本地编码值
=======================================
*/
CR_API void_t
set_sys_codepage (
  __CR_IN__ uint_t  cpage
    )
{
    if (cpage != CR_LOCAL)
        g_codepage = cpage;
}

/*
=======================================
    设置 str_acp2uni 外挂
=======================================
*/
CR_API void_t
set_str_acp2uni (
  __CR_IN__ cr_acp2uni_t    func
    )
{
    g_str_acp2uni = func;
}

/*
=======================================
    设置 str_uni2acp 外挂
=======================================
*/
CR_API void_t
set_str_uni2acp (
  __CR_IN__ cr_uni2acp_t    func
    )
{
    g_str_uni2acp = func;
}

/*
=======================================
    计算 Tick 时间差
=======================================
*/
CR_API int32u
timer_delta32 (
  __CR_IN__ int32u  base
    )
{
    int32u  now = timer_get32();

    if (now < base)
        return (0xFFFFFFFFUL - base + now + 1);
    return (now - base);
}

/*
=======================================
    延时一段时间
=======================================
*/
CR_API void_t
delayms (
  __CR_IN__ byte_t  num
    )
{
    int32u  time = (int32u)num;
    int32u  base = timer_get32();

    while (timer_delta32(base) < time);
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
