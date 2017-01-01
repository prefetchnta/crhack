/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-01-11  */
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
/*  >>>>>>>>>>>>>>> CrHack OS API 编码包装函数库 for Windows <<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "memlib.h"

#include <windows.h>

/* Windows CE 有声明但没实现 */
#if     defined(_CR_API_WIDE_ONLY_)

/*
---------------------------------------
    转换字符串编码
---------------------------------------
*/
static LPCWSTR
ansi2wide (
  __CR_IN__ const ansi_t*   ansi
    )
{
    HANDLE  heap;
    sint_t  cnts;
    leng_t  size;
    wide_t* wide;

    cnts = MultiByteToWideChar(CP_ACP, 0, ansi, -1, NULL, 0);
    if (cnts == 0)
        return (NULL);

    heap = GetProcessHeap();
    if (heap == NULL)
        return (NULL);

    size = cnts;
    size *= sizeof(wide_t);
    wide = HeapAlloc(heap, 0, size);
    if (wide == NULL) {
        SetLastError(ERROR_OUTOFMEMORY);
        return (NULL);
    }
    MultiByteToWideChar(CP_ACP, 0, ansi, -1, wide, cnts);
    return ((LPCWSTR)wide);
}

/*
---------------------------------------
    释放转换的字符串
---------------------------------------
*/
static void_t
string_free (
  __CR_IN__ const wide_t*   wide
    )
{
    DWORD   last_error;

    /* 保存错误信息 */
    last_error = GetLastError();
    HeapFree(GetProcessHeap(), 0, (LPVOID)wide);
    SetLastError(last_error);
}

/*
=======================================
    LoadLibraryA
=======================================
*/
HMODULE WINAPI
LoadLibraryA (
  __CR_IN__ LPCSTR  lpFileName
    )
{
    HMODULE rett;
    LPCWSTR name;

    name = ansi2wide(lpFileName);
    if (name == NULL)
        return (NULL);

    rett = LoadLibraryW(name);
    string_free(name);
    return (rett);
}

/*
=======================================
    GetModuleHandleA
=======================================
*/
HMODULE WINAPI
GetModuleHandleA (
  __CR_IN__ LPCSTR  lpModuleName
    )
{
    HMODULE rett;
    LPCWSTR name;

    name = ansi2wide(lpModuleName);
    if (name == NULL)
        return (NULL);

    rett = GetModuleHandleW(name);
    string_free(name);
    return (rett);
}

/*
=======================================
    MessageBoxA
=======================================
*/
int WINAPI
MessageBoxA (
  __CR_IN__ HWND    hWnd,
  __CR_IN__ LPCSTR  lpText,
  __CR_IN__ LPCSTR  lpCaption,
  __CR_IN__ UINT    uType
    )
{
    int     rett;
    LPCWSTR text;
    LPCWSTR cptn;

    text = ansi2wide(lpText);
    cptn = ansi2wide(lpCaption);
    rett = MessageBoxW(hWnd, text, cptn, uType);

    if (cptn != NULL)
        string_free(cptn);
    if (text != NULL)
        string_free(text);
    return (rett);
}

/*
=======================================
    RegisterClassA
=======================================
*/
ATOM WINAPI
RegisterClassA (
  __CR_IN__ CONST WNDCLASSA*    lpWndClass
    )
{
    ATOM        rett;
    BOOL        free1;
    BOOL        free2;
    WNDCLASSW   winclass;

    struct_cpy(&winclass, lpWndClass, WNDCLASSA);

    /* 是否需要转换字符串 */
    if ((DWORD)lpWndClass->lpszMenuName > 0xFFFFUL) {
        free1 = TRUE;
        winclass.lpszMenuName = ansi2wide(lpWndClass->lpszMenuName);
        if (winclass.lpszMenuName == NULL)
            return (NULL);
    }
    else {
        free1 = FALSE;
    }

    /* 是否需要转换字符串 */
    if ((DWORD)lpWndClass->lpszClassName > 0xFFFFUL) {
        free2 = TRUE;
        winclass.lpszClassName = ansi2wide(lpWndClass->lpszClassName);
        if (winclass.lpszClassName == NULL) {
            if (free1)
                string_free(winclass.lpszMenuName);
            return (NULL);
        }
    }
    else {
        free2 = FALSE;
    }

    rett = RegisterClassW(&winclass);
    if (free2)
        string_free(winclass.lpszClassName);
    if (free1)
        string_free(winclass.lpszMenuName);
    return (rett);
}

/*
=======================================
    CreateWindowExA
=======================================
*/
HWND WINAPI
CreateWindowExA (
  __CR_IN__ DWORD       dwExStyle,
  __CR_IN__ LPCSTR      lpClassName,
  __CR_IN__ LPCSTR      lpWindowName,
  __CR_IN__ DWORD       dwStyle,
  __CR_IN__ int         x,
  __CR_IN__ int         y,
  __CR_IN__ int         nWidth,
  __CR_IN__ int         nHeight,
  __CR_IN__ HWND        hWndParent,
  __CR_IN__ HMENU       hMenu,
  __CR_IN__ HINSTANCE   hInstance,
  __CR_IN__ LPVOID      lpParam
    )
{
    HWND    rett;
    BOOL    class_free;
    LPCWSTR class_name;
    LPCWSTR title_name;

    if ((DWORD)lpClassName > 0xFFFFUL) {
        class_free = TRUE;
        class_name = ansi2wide(lpClassName);
        if (class_name == NULL)
            return (NULL);
    }
    else {
        class_free = FALSE;
        class_name = (LPCWSTR)lpClassName;
    }

    title_name = ansi2wide(lpWindowName);
    if (title_name == NULL) {
        if (class_free)
            string_free(class_name);
        return (NULL);
    }

    rett = CreateWindowExW(dwExStyle, class_name, title_name,
                           dwStyle, x, y, nWidth, nHeight, hWndParent,
                           hMenu, hInstance, lpParam);
    string_free(title_name);
    if (class_free)
        string_free(class_name);
    return (rett);
}

/*
=======================================
    UnregisterClassA
=======================================
*/
BOOL WINAPI
UnregisterClassA (
  __CR_IN__ LPCSTR      lpClassName,
  __CR_IN__ HINSTANCE   hInstance
    )
{
    BOOL    rett;
    LPCWSTR class_name;

    if ((DWORD)lpClassName > 0xFFFFUL) {
        class_name = ansi2wide(lpClassName);
        if (class_name == NULL)
            return (FALSE);

        rett = UnregisterClassW(class_name, hInstance);
        string_free(class_name);
        return (rett);
    }
    return (UnregisterClassW((LPCWSTR)lpClassName, hInstance));
}

/*
=======================================
    LoadIconA
=======================================
*/
HICON WINAPI
LoadIconA (
  __CR_IN__ HINSTANCE   hInstance,
  __CR_IN__ LPCSTR      lpIconName
    )
{
    HICON   rett;
    LPCWSTR icon_name;

    if ((DWORD)lpIconName > 0xFFFFUL) {
        icon_name = ansi2wide(lpIconName);
        if (icon_name == NULL)
            return (NULL);

        rett = LoadIconW(hInstance, icon_name);
        string_free(icon_name);
        return (rett);
    }
    return (LoadIconW(hInstance, (LPCWSTR)lpIconName));
}

/*
=======================================
    CreateFileA
=======================================
*/
HANDLE WINAPI
CreateFileA (
  __CR_IN__ LPCSTR                  lpFileName,
  __CR_IN__ DWORD                   dwDesiredAccess,
  __CR_IN__ DWORD                   dwShareMode,
  __CR_IN__ LPSECURITY_ATTRIBUTES   lpSecurityAttributes,
  __CR_IN__ DWORD                   dwCreationDisposition,
  __CR_IN__ DWORD                   dwFlagsAndAttributes,
  __CR_IN__ HANDLE                  hTemplateFile
    )
{
    HANDLE  rett;
    LPCWSTR name;

    name = ansi2wide(lpFileName);
    if (name == NULL)
        return (INVALID_HANDLE_VALUE);

    rett = CreateFileW(name, dwDesiredAccess, dwShareMode,
                       lpSecurityAttributes, dwCreationDisposition,
                       dwFlagsAndAttributes, hTemplateFile);
    string_free(name);
    return (rett);
}

/*
=======================================
    CreateDirectoryA
=======================================
*/
BOOL WINAPI
CreateDirectoryA (
  __CR_IN__ LPCSTR                  lpPathName,
  __CR_IN__ LPSECURITY_ATTRIBUTES   lpSecurityAttributes
    )
{
    BOOL    rett;
    LPCWSTR path;

    path = ansi2wide(lpPathName);
    if (path == NULL)
        return (FALSE);

    rett = CreateDirectoryW(path, lpSecurityAttributes);
    string_free(path);
    return (rett);
}

/*
=======================================
    FindFirstFileA
=======================================
*/
HANDLE WINAPI
FindFirstFileA (
  __CR_IN__ LPCSTR              lpFileName,
  __CR_OT__ LPWIN32_FIND_DATAA  lpFindFileData
    )
{
    HANDLE              rett;
    LPCWSTR             name;
    WIN32_FIND_DATAW    data;

    name = ansi2wide(lpFileName);
    if (name == NULL)
        return (INVALID_HANDLE_VALUE);

    rett = FindFirstFileW(name, &data);
    string_free(name);

    if (rett == INVALID_HANDLE_VALUE)
        return (INVALID_HANDLE_VALUE);

    mem_cpy(lpFindFileData, &data, sizeof(data) - MAX_PATH * sizeof(wide_t));
    if (WideCharToMultiByte(CP_ACP, 0, data.cFileName, -1,
                            lpFindFileData->cFileName, MAX_PATH,
                            NULL, NULL) == 0)
        return (INVALID_HANDLE_VALUE);
    return (rett);
}

/*
=======================================
    MoveFileA
=======================================
*/
BOOL WINAPI
MoveFileA (
  __CR_IN__ LPCSTR  lpExistingFileName,
  __CR_IN__ LPCSTR  lpNewFileName
    )
{
    BOOL    rett;
    LPCWSTR new_name;
    LPCWSTR ext_name;

    ext_name = ansi2wide(lpExistingFileName);
    if (ext_name == NULL)
        return (FALSE);

    new_name = ansi2wide(lpNewFileName);
    if (new_name == NULL) {
        string_free(ext_name);
        return (FALSE);
    }

    rett = MoveFileW(ext_name, new_name);
    string_free(new_name);
    string_free(ext_name);
    return (rett);
}

/*
=======================================
    FindNextFileA
=======================================
*/
BOOL WINAPI
FindNextFileA (
  __CR_IN__ HANDLE              hFindFile,
  __CR_OT__ LPWIN32_FIND_DATAA  lpFindFileData
    )
{
    WIN32_FIND_DATAW    data;

    if (!FindNextFileW(hFindFile, &data))
        return (FALSE);

    mem_cpy(lpFindFileData, &data, sizeof(data) - MAX_PATH * sizeof(wide_t));
    if (WideCharToMultiByte(CP_ACP, 0, data.cFileName, -1,
                            lpFindFileData->cFileName, MAX_PATH,
                            NULL, NULL) == 0)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    DeleteFileA
=======================================
*/
BOOL WINAPI
DeleteFileA (
  __CR_IN__ LPCSTR  lpFileName
    )
{
    BOOL    rett;
    LPCWSTR name;

    name = ansi2wide(lpFileName);
    if (name == NULL)
        return (FALSE);

    rett = DeleteFileW(name);
    string_free(name);
    return (rett);
}

/*
=======================================
    CreateFontIndirectA
=======================================
*/
HFONT WINAPI
CreateFontIndirectA (
  __CR_IN__ CONST LOGFONTA* lplf
    )
{
    LOGFONTW    info;

    /* 转换字体名的编码 */
    mem_cpy(&info, lplf, sizeof(info) - LF_FACESIZE * sizeof(wide_t));
    if (MultiByteToWideChar(CP_ACP, 0, lplf->lfFaceName, -1,
                            info.lfFaceName, LF_FACESIZE) == 0)
        return (NULL);
    return (CreateFontIndirectW(&info));
}

/* Windows 9x 不支持宽字符版本 */
#elif   defined(_CR_API_ANSI_ONLY_)
    #error "wrapper.c: OS API CODEPAGE not supported yet!"

#endif  /* OS API CODEPAGE predefines */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
