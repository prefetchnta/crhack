/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-12-29  */
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
/*  >>>>>>>>>>>>>>>>>>> CrHack 应用程序函数库 for Naked <<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "applib.h"

#ifndef _CR_NO_STDC_
    #include <stdlib.h>
#endif

extern uint_t       g_app_type;
extern quit_t       g_quit_now;
extern msgboxA_t    g_msg_boxA;
extern msgboxW_t    g_msg_boxW;

/*
=======================================
    退出应用程序
=======================================
*/
CR_API void_t
quit_now (void_t)
{
    if (g_quit_now != NULL)
        g_quit_now();
    else
#ifndef _CR_NO_STDC_
        abort();
#else
        for (;;);
#endif
}

/*
=======================================
    设置应用程序类型
=======================================
*/
CR_API bool_t
set_app_type (
  __CR_IN__ uint_t  type
    )
{
    if (type == CR_APP_GUI)
        g_app_type = CR_APP_GUI;
    else
        g_app_type = CR_APP_CUI;
    return (TRUE);
}

/*
=======================================
    切换全屏/桌面功能
=======================================
*/
CR_API bool_t
set_app_mode (
  __CR_IN__ bool_t  full
    )
{
    CR_NOUSE(full);
    return (FALSE);
}

/*
=======================================
    显示严重错误提示A
=======================================
*/
CR_API void_t
msg_stopA (
  __CR_IN__ const ansi_t*   text,
  __CR_IN__ const ansi_t*   title
    )
{
    if (g_msg_boxA != NULL)
        g_msg_boxA(text, title, CR_STOP);
    quit_now();
}

/*
=======================================
    显示严重错误提示W
=======================================
*/
CR_API void_t
msg_stopW (
  __CR_IN__ const wide_t*   text,
  __CR_IN__ const wide_t*   title
    )
{
    if (g_msg_boxW != NULL)
        g_msg_boxW(text, title, CR_STOP);
    quit_now();
}

/*
=======================================
    显示警告错误提示A
=======================================
*/
CR_API void_t
msg_warnA (
  __CR_IN__ const ansi_t*   text,
  __CR_IN__ const ansi_t*   title
    )
{
    if (g_msg_boxA != NULL)
        g_msg_boxA(text, title, CR_WARN);
}

/*
=======================================
    显示警告错误提示W
=======================================
*/
CR_API void_t
msg_warnW (
  __CR_IN__ const wide_t*   text,
  __CR_IN__ const wide_t*   title
    )
{
    if (g_msg_boxW != NULL)
        g_msg_boxW(text, title, CR_WARN);
}

/*
=======================================
    显示成功提示A
=======================================
*/
CR_API void_t
msg_doneA (
  __CR_IN__ const ansi_t*   text,
  __CR_IN__ const ansi_t*   title
    )
{
    if (g_msg_boxA != NULL)
        g_msg_boxA(text, title, CR_DONE);
}

/*
=======================================
    显示成功提示W
=======================================
*/
CR_API void_t
msg_doneW (
  __CR_IN__ const wide_t*   text,
  __CR_IN__ const wide_t*   title
    )
{
    if (g_msg_boxW != NULL)
        g_msg_boxW(text, title, CR_DONE);
}

/*
=======================================
    显示是否选择框A
=======================================
*/
CR_API bool_t
msg_askuA (
  __CR_IN__ const ansi_t*   text,
  __CR_IN__ const ansi_t*   title
    )
{
    if (g_msg_boxA != NULL)
        return (g_msg_boxA(text, title, CR_ASKU));
    return (FALSE);
}

/*
=======================================
    显示是否选择框W
=======================================
*/
CR_API bool_t
msg_askuW (
  __CR_IN__ const wide_t*   text,
  __CR_IN__ const wide_t*   title
    )
{
    if (g_msg_boxW != NULL)
        return (g_msg_boxW(text, title, CR_ASKU));
    return (FALSE);
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
