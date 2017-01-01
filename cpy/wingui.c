
/* 头文件组 */
#include "crhack.h"

/* 窗口类名 */
#define WIN_CLASS   "winclass"

/* WinMain 的区别 */
#ifndef _CR_OS_WINCE_
    typedef LPSTR   cmd_line_t;
#else
    typedef LPWSTR  cmd_line_t;
#endif

/*
=======================================
    Windows 窗口消息处理
=======================================
*/
LRESULT CALLBACK
WindowProc (
  __CR_IN__ HWND    hwnd,
  __CR_IN__ UINT    message,
  __CR_IN__ WPARAM  wparam,
  __CR_IN__ LPARAM  lparam
    )
{
    switch (message)
    {
        default:
            break;
/*
        case WM_CREATE:
            return (0);

        case WM_DESTROY:
            return (0);

        case WM_PAINT:
            return (0);

        case WM_SETCURSOR:
            SetCursor(NULL);
            return (TRUE);
*/
        case WM_CLOSE:
            PostQuitMessage(0);
            return (0);
    }
#ifndef _CR_OS_WINCE_
    return (DefWindowProcA(hwnd, message, wparam, lparam));
#else
    return (DefWindowProcW(hwnd, message, wparam, lparam));
#endif
}

/*
---------------------------------------
    出错处理的回调
---------------------------------------
*/
static void_t
errs_hook (
  __CR_IN__ const sERROR*   errs
    )
{
    CR_NOUSE(errs);
}

/*
=======================================
    WinMain 程序入口
=======================================
*/
int WINAPI
WinMain (
  __CR_IN__ HINSTANCE   curt_app,
  __CR_IN__ HINSTANCE   prev_app,
  __CR_IN__ cmd_line_t  cmd_line,
  __CR_IN__ int         cmd_show
    )
{
    MSG     msg;
    HWND    hwnd;

    CR_NOUSE(cmd_line);
    CR_NOUSE(cmd_show);
    CR_NOUSE(prev_app);

    /* 生成系统 */
    crhack_core_init();
    error_hook(errs_hook);

    /* 生成窗口 */
    hwnd = (HWND)window_open(curt_app, (void_t*)WindowProc,
                             0, 0, 1, 0, "window title", WIN_CLASS,
                            (ansi_t*)101, CR_WSTYLE_FIXED);
    if (hwnd == NULL) {
        return (-1);
    }
    set_gui_hwnd((hwnd_t)hwnd);

    /************/
    /* 程序内容 */
    /************/

    /* 消息循环 */
    for (;;)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT)
                break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    /* 释放窗口 */
    window_kill(hwnd, curt_app, WIN_CLASS);

    /* 释放系统 */
    crhack_core_free();
    return ((int)msg.wParam);
}
