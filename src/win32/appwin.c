/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-03-02  */
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
/*  >>>>>>>>>>>>>>>>>> CrHack 应用程序函数库 for Windows <<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "applib.h"
#include "strlib.h"

/* WinCE 无此部分 LIBC */
#if defined(_CR_GUI_ONLY_)
    #define _CR_NO_STDC_
#endif

#ifndef _CR_NO_STDC_
    #include <conio.h>
    #include <stdio.h>
#endif

#include <windows.h>

/* CUI 输出句柄 */
HANDLE  g_cui_out = NULL;

extern uint_t       g_app_type;
extern quit_t       g_quit_now;
extern hwnd_t       g_gui_hwnd;
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
    if (g_app_type != CR_APP_GUI)
        ExitProcess(0x0UL);
    else
        PostQuitMessage(0);
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
#ifndef _CR_GUI_ONLY_
    HANDLE  cui;
#endif
    if (type == CR_APP_GUI) {
        g_cui_out  = NULL;
        g_app_type = CR_APP_GUI;
        return (TRUE);
    }

#ifndef _CR_GUI_ONLY_
    cui = GetStdHandle(STD_OUTPUT_HANDLE);
    if (cui == INVALID_HANDLE_VALUE)
        return (FALSE);
    g_cui_out  = cui;
    g_app_type = CR_APP_CUI;
    return (TRUE);
#else
    return (FALSE);
#endif
}

#if defined(_CR_OS_WINCE_)
/*
---------------------------------------
    枚举窗口的回调
---------------------------------------
*/
static BOOL CALLBACK
window_enum (
  __CR_IN__ HWND    hwnd,
  __CR_IN__ LPARAM  param
    )
{
    wide_t  name[256];

    if (!GetClassNameW(hwnd, name, 256))
        return (FALSE);

    if (str_cmpW(name, L"HHTaskBar") == 0)
        ShowWindow(hwnd, (int)param);
    else
    if (str_cmpW(name, L"menuworker") == 0)
        ShowWindow(hwnd, (int)param);
    else
    if (str_cmpW(name, L"menu_worker") == 0)
        ShowWindow(hwnd, (int)param);
    else
    if (str_cmpW(name, L"SipWndClass") == 0)
        ShowWindow(hwnd, (int)param);
    else
    if (str_cmpW(name, L"MS_SIPBUTTON") == 0)
        ShowWindow(hwnd, (int)param);

    return (TRUE);
}

#endif  /* _CR_OS_WINCE_ */

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
    int param = (full) ? SW_HIDE : SW_SHOW;

#if defined(_CR_OS_WINCE_)
    if (!EnumWindows(window_enum, (LPARAM)param))
        return (FALSE);
    return (TRUE);

#else
    HWND    task;

    task = FindWindow(_CR_TS("Shell_TrayWnd"), NULL);
    if (task == NULL)
        return (FALSE);
    ShowWindow(task, param);
    return (TRUE);
#endif
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
    if (g_msg_boxA != NULL) {
        g_msg_boxA(text, title, CR_STOP);
    }
#ifndef _CR_NO_STDC_
    else
    if (g_app_type != CR_APP_GUI) {
        printf("STOP - %s: %s [OK]\n", title, text);
        getch();
    }
#endif
    else {
        MessageBoxA((HWND)g_gui_hwnd, text, title,
                     MB_OK | MB_ICONSTOP);
    }
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
    if (g_msg_boxW != NULL) {
        g_msg_boxW(text, title, CR_STOP);
    }
#ifndef _CR_NO_STDC_
    else
    if (g_app_type != CR_APP_GUI) {
        printf("STOP - %ls: %ls [OK]\n", title, text);
        getch();
    }
#endif
    else {
        MessageBoxW((HWND)g_gui_hwnd, text, title,
                     MB_OK | MB_ICONSTOP);
    }
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
    if (g_msg_boxA != NULL) {
        g_msg_boxA(text, title, CR_WARN);
    }
#ifndef _CR_NO_STDC_
    else
    if (g_app_type != CR_APP_GUI) {
        printf("WARN - %s: %s [OK]\n", title, text);
        getch();
    }
#endif
    else {
        MessageBoxA((HWND)g_gui_hwnd, text, title,
                     MB_OK | MB_ICONWARNING);
    }
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
    if (g_msg_boxW != NULL) {
        g_msg_boxW(text, title, CR_WARN);
    }
#ifndef _CR_NO_STDC_
    else
    if (g_app_type != CR_APP_GUI) {
        printf("WARN - %ls: %ls [OK]\n", title, text);
        getch();
    }
#endif
    else {
        MessageBoxW((HWND)g_gui_hwnd, text, title,
                     MB_OK | MB_ICONWARNING);
    }
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
    if (g_msg_boxA != NULL) {
        g_msg_boxA(text, title, CR_DONE);
    }
#ifndef _CR_NO_STDC_
    else
    if (g_app_type != CR_APP_GUI) {
        printf("DONE - %s: %s [OK]\n", title, text);
        getch();
    }
#endif
    else {
        MessageBoxA((HWND)g_gui_hwnd, text, title,
                     MB_OK | MB_ICONINFORMATION);
    }
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
    if (g_msg_boxW != NULL) {
        g_msg_boxW(text, title, CR_DONE);
    }
#ifndef _CR_NO_STDC_
    else
    if (g_app_type != CR_APP_GUI) {
        printf("DONE - %ls: %ls [OK]\n", title, text);
        getch();
    }
#endif
    else {
        MessageBoxW((HWND)g_gui_hwnd, text, title,
                     MB_OK | MB_ICONINFORMATION);
    }
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
    sint_t  answer;

    if (g_msg_boxA != NULL)
        return (g_msg_boxA(text, title, CR_ASKU));

#ifndef _CR_NO_STDC_
    if (g_app_type != CR_APP_GUI) {
_repeat:
        printf("ASKU - %s: %s [Y/N]\n", title, text);
        answer = getch();
        if (answer == CR_AC('y') || answer == CR_AC('Y'))
            return (TRUE);
        if (answer != CR_AC('n') && answer != CR_AC('N'))
            goto _repeat;
    }
    else {
#endif
        answer = MessageBoxA((HWND)g_gui_hwnd, text, title,
                              MB_YESNO | MB_ICONQUESTION);
        if (answer == IDYES)
            return (TRUE);

#ifndef _CR_NO_STDC_
    }
#endif
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
    sint_t  answer;

    if (g_msg_boxW != NULL)
        return (g_msg_boxW(text, title, CR_ASKU));

#ifndef _CR_NO_STDC_
    if (g_app_type != CR_APP_GUI) {
_repeat:
        printf("ASKU - %ls: %ls [Y/N]\n", title, text);
        answer = getch();
        if (answer == CR_AC('y') || answer == CR_AC('Y'))
            return (TRUE);
        if (answer != CR_AC('n') && answer != CR_AC('N'))
            goto _repeat;
    }
    else {
#endif
        answer = MessageBoxW((HWND)g_gui_hwnd, text, title,
                              MB_YESNO | MB_ICONQUESTION);
        if (answer == IDYES)
            return (TRUE);

#ifndef _CR_NO_STDC_
    }
#endif
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
