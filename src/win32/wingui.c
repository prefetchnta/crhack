/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-01-10  */
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
/*  >>>>>>>>>>>>>>>> CrHack 系统窗口外壳函数库 for Windows <<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "extz.h"
#include "memlib.h"

#include <windows.h>

/* 简化常数的宏 */
#if defined(_CR_OS_WINCE_)
    #define WINGUI_STYLE    (CS_DBLCLKS | \
                             CS_HREDRAW | CS_VREDRAW)
#else
    #define WINGUI_STYLE    (CS_DBLCLKS | CS_OWNDC | \
                             CS_HREDRAW | CS_VREDRAW)
#endif

/* 本地窗口风格 */
static const DWORD s_win_style[] =
{
    WS_POPUP | WS_VISIBLE,

#if defined(_CR_OS_WINCE_)
    WS_OVERLAPPED | WS_SYSMENU,
#else
    WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,
#endif
    /* 兼容 Windows CE */
    WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION |
    WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
};

/*
=======================================
    应用程序窗口创建
=======================================
*/
CR_API hwnd_t
window_open (
  __CR_IN__ void_t*         instance,
  __CR_IN__ void_t*         msg_proc,
  __CR_IN__ sint_t          x,
  __CR_IN__ sint_t          y,
  __CR_IN__ uint_t          w,
  __CR_IN__ uint_t          h,
  __CR_IN__ const ansi_t*   title,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ const ansi_t*   icon,
  __CR_IN__ uint_t          style
    )
{
    RECT        full;
    HWND        hwnd;
    HICON       hicon;
    DWORD       flags;
    HBRUSH      hbrush;
    HCURSOR     hcursor;
    WNDCLASSA   winclass;

    /* 处理额外的标志 */
    if (style & CR_WSTYLE_TOPMOST) {
        flags = WS_EX_TOPMOST;
        style &= (~CR_WSTYLE_TOPMOST);
    }
    else {
        flags = 0;
    }

    /* 过滤窗口风格 */
    if (style > CR_WSTYLE_MAXVV)
        return (NULL);
    hicon = LoadIconA(instance, icon);
    if (hicon == NULL)
        return (NULL);
    hcursor = LoadCursor(NULL, (LPCTSTR)IDC_ARROW);
    if (hcursor == NULL)
        return (NULL);
    hbrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
    if (hbrush == NULL)
        return (NULL);

    /* 是否生成全屏程序 */
    if (w == 0 || h == 0) {
        if (w == 0 && h == 0) {     /* 全屏幕 */
            x = 0;
            y = 0;
            w = GetSystemMetrics(SM_CXSCREEN);
            h = GetSystemMetrics(SM_CYSCREEN);
        }
        else {                      /* 全桌面 */
            if (!SystemParametersInfo(SPI_GETWORKAREA, 0, &full, 0))
                return (NULL);
            x = full.left;
            y = full.top;
            w = full.right - full.left;
            h = full.bottom - full.top;
        }
    }

    /* 注册窗口类别 */
    winclass.style = WINGUI_STYLE;
    winclass.lpfnWndProc = (WNDPROC)msg_proc;
    winclass.cbClsExtra = 0;
    winclass.cbWndExtra = 0;
    winclass.hInstance = (HINSTANCE)instance;
    winclass.hIcon = hicon;
    winclass.hCursor = hcursor;
    winclass.hbrBackground = hbrush;
    winclass.lpszMenuName = NULL;
    winclass.lpszClassName = name;
    if (!RegisterClassA(&winclass))
        return (NULL);

    /* 生成并显示窗口 */
    hwnd = CreateWindowExA(flags, name, title, s_win_style[style],
                           x, y, w, h, NULL, NULL, instance, NULL);
    if (hwnd == NULL) {
        UnregisterClassA(name, (HINSTANCE)instance);
        return (NULL);
    }
    ShowWindow(hwnd, SW_SHOWNORMAL);
    UpdateWindow(hwnd);
    return ((hwnd_t)hwnd);
}

/*
=======================================
    应用程序窗口创建A
=======================================
*/
CR_API hwnd_t
window_openA (
  __CR_IN__ void_t*         instance,
  __CR_IN__ void_t*         msg_proc,
  __CR_IN__ sint_t          x,
  __CR_IN__ sint_t          y,
  __CR_IN__ uint_t          w,
  __CR_IN__ uint_t          h,
  __CR_IN__ const ansi_t*   title,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ const ansi_t*   icon,
  __CR_IN__ uint_t          style
    )
{
    return (window_open(instance, msg_proc, x, y, w, h,
                        title, name, icon, style));
}

/*
=======================================
    应用程序窗口创建W
=======================================
*/
CR_API hwnd_t
window_openW (
  __CR_IN__ void_t*         instance,
  __CR_IN__ void_t*         msg_proc,
  __CR_IN__ sint_t          x,
  __CR_IN__ sint_t          y,
  __CR_IN__ uint_t          w,
  __CR_IN__ uint_t          h,
  __CR_IN__ const wide_t*   title,
  __CR_IN__ const wide_t*   name,
  __CR_IN__ const ansi_t*   icon,
  __CR_IN__ uint_t          style
    )
{
    RECT        full;
    HWND        hwnd;
    HICON       hicon;
    DWORD       flags;
    HBRUSH      hbrush;
    HCURSOR     hcursor;
    WNDCLASSW   winclass;

    /* 处理额外的标志 */
    if (style & CR_WSTYLE_TOPMOST) {
        flags = WS_EX_TOPMOST;
        style &= (~CR_WSTYLE_TOPMOST);
    }
    else {
        flags = 0;
    }

    /* 过滤窗口风格 */
    if (style > CR_WSTYLE_MAXVV)
        return (NULL);
    hicon = LoadIconA(instance, icon);
    if (hicon == NULL)
        return (NULL);
    hcursor = LoadCursor(NULL, (LPCTSTR)IDC_ARROW);
    if (hcursor == NULL)
        return (NULL);
    hbrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
    if (hbrush == NULL)
        return (NULL);

    /* 是否生成全屏程序 */
    if (w == 0 || h == 0) {
        if (w == 0 && h == 0) {     /* 全屏幕 */
            x = 0;
            y = 0;
            w = GetSystemMetrics(SM_CXSCREEN);
            h = GetSystemMetrics(SM_CYSCREEN);
        }
        else {                      /* 全桌面 */
            if (!SystemParametersInfo(SPI_GETWORKAREA, 0, &full, 0))
                return (NULL);
            x = full.left;
            y = full.top;
            w = full.right - full.left;
            h = full.bottom - full.top;
        }
    }

    /* 注册窗口类别 */
    winclass.style = WINGUI_STYLE;
    winclass.lpfnWndProc = (WNDPROC)msg_proc;
    winclass.cbClsExtra = 0;
    winclass.cbWndExtra = 0;
    winclass.hInstance = (HINSTANCE)instance;
    winclass.hIcon = hicon;
    winclass.hCursor = hcursor;
    winclass.hbrBackground = hbrush;
    winclass.lpszMenuName = NULL;
    winclass.lpszClassName = name;
    if (!RegisterClassW(&winclass))
        return (NULL);

    /* 生成并显示窗口 */
    hwnd = CreateWindowExW(flags, name, title, s_win_style[style],
                           x, y, w, h, NULL, NULL, instance, NULL);
    if (hwnd == NULL) {
        UnregisterClassW(name, (HINSTANCE)instance);
        return (NULL);
    }
    ShowWindow(hwnd, SW_SHOWNORMAL);
    UpdateWindow(hwnd);
    return ((hwnd_t)hwnd);
}

/*
=======================================
    应用程序窗口销毁
=======================================
*/
CR_API bool_t
window_kill (
  __CR_IN__ hwnd_t          hwnd,
  __CR_IN__ void_t*         instance,
  __CR_IN__ const ansi_t*   name
    )
{
    if (!DestroyWindow((HWND)hwnd))
        return (FALSE);
    if (!UnregisterClassA(name, (HINSTANCE)instance))
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    应用程序窗口销毁A
=======================================
*/
CR_API bool_t
window_killA (
  __CR_IN__ hwnd_t          hwnd,
  __CR_IN__ void_t*         instance,
  __CR_IN__ const ansi_t*   name
    )
{
    return (window_kill(hwnd, instance, name));
}

/*
=======================================
    应用程序窗口销毁W
=======================================
*/
CR_API bool_t
window_killW (
  __CR_IN__ hwnd_t          hwnd,
  __CR_IN__ void_t*         instance,
  __CR_IN__ const wide_t*   name
    )
{
    if (!DestroyWindow((HWND)hwnd))
        return (FALSE);
    if (!UnregisterClassW(name, (HINSTANCE)instance))
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    检测按键按下
=======================================
*/
CR_API bool_t
key_input_test (
  __CR_IN__ uint_t  vkey
    )
{
    /* CUI 下面也能使用 (函数 GetKeyState() 也可以使用) */
    return ((GetAsyncKeyState((int)vkey) & 0x8000) ? TRUE : FALSE);
}

/*
=======================================
    检测按键释放
=======================================
*/
CR_API bool_t
key_input_click (
  __CR_IN__ uint_t  vkey
    )
{
    /* 等待按钮松开后返回 */
    if (key_input_test(vkey)) {
        while (key_input_test(vkey))
            Sleep(0);
        return (TRUE);
    }
    return (FALSE);
}

/*
=======================================
    鼠标光标开关
=======================================
*/
CR_API void_t
mouse_toggle (
  __CR_IN__ bool_t  show
    )
{
    ShowCursor(show);
}

/*
=======================================
    鼠标位置设置
=======================================
*/
CR_API bool_t
mouse_set_pos (
  __CR_IN__ sint_t  x,
  __CR_IN__ sint_t  y
    )
{
    /* 设置屏幕系的坐标 */
    if (!SetCursorPos((int)x, (int)y))
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    鼠标位置返回
=======================================
*/
CR_API bool_t
mouse_get_pos (
  __CR_OT__ sint_t* x,
  __CR_OT__ sint_t* y
    )
{
    POINT   pnt;

    /* 获取屏幕系的坐标 */
    if (!GetCursorPos(&pnt))
        return (FALSE);
    if (x != NULL)
        *x = (sint_t)pnt.x;
    if (y != NULL)
        *y = (sint_t)pnt.y;
    return (TRUE);
}

#if !defined(_CR_OS_WINCE_)
#if !defined(_CR_CC_MSC_) || (_WIN32_WINNT >= 0x0500)
/*
=======================================
    获取上次输入时间
=======================================
*/
CR_API int32u
input_tick32 (void_t)
{
    LASTINPUTINFO   lii;

    lii.cbSize = sizeof(lii);
    if (!GetLastInputInfo(&lii))
        return (0);
    return (lii.dwTime);
}
#endif  /* !_CR_CC_MSC_ || (_WIN32_WINNT >= 0x0500) */

/*
=======================================
    获取当前显示模式
=======================================
*/
CR_API dispmode_t
disp_mode_get (void_t)
{
    DEVMODE*    rett;

    rett = struct_new(DEVMODE);
    if (rett == NULL)
        return (NULL);
    rett->dmSize = sizeof(DEVMODE);
    rett->dmDriverExtra = 0;
    if (!EnumDisplaySettingsW(NULL, ENUM_CURRENT_SETTINGS, rett)) {
        mem_free(rett);
        return (NULL);
    }
    return ((dispmode_t)rett);
}

/*
=======================================
    设置当前显示模式
=======================================
*/
CR_API bool_t
disp_mode_set (
  __CR_IN__ dispmode_t  mode,
  __CR_IN__ uint_t      width,
  __CR_IN__ uint_t      height,
  __CR_IN__ uint_t      bpp
    )
{
    LONG    retc;
    DEVMODE temp;

    struct_cpy(&temp, mode, DEVMODE);
    if (bpp == 8 || bpp == 16 || bpp == 32) {
        temp.dmBitsPerPel = bpp;
        if (width != 0 && height != 0) {
            temp.dmPelsWidth  = width;
            temp.dmPelsHeight = height;
        }
    }
    retc = ChangeDisplaySettingsW(&temp, 0);
    if (retc != DISP_CHANGE_SUCCESSFUL)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    释放获取的显示模式信息
=======================================
*/
CR_API void_t
disp_mode_del (
  __CR_IN__ dispmode_t  mode
    )
{
    mem_free(mode);
}

#endif  /* !_CR_OS_WINCE_ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
