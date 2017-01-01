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
/*  >>>>>>>>>>>>>>>> CrHack CUI 输出控制函数库 for Windows <<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "extz.h"

#ifndef _CR_GUI_ONLY_

#include <windows.h>

extern HANDLE   g_cui_out;

/* MinGW 的 PSDK wincon.h 里没有这两个常数 */
#ifndef COMMON_LVB_UNDERSCORE
    #define COMMON_LVB_UNDERSCORE       0x8000
#endif
#ifndef COMMON_LVB_REVERSE_VIDEO
    #define COMMON_LVB_REVERSE_VIDEO    0x4000
#endif

/*
=======================================
    设置 CUI 当前颜色
=======================================
*/
CR_API bool_t
cui_set_color (
  __CR_IN__ int16u  color
    )
{
    if (g_cui_out == NULL)
        return (FALSE);
    if (!SetConsoleTextAttribute(g_cui_out, color))
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    获取 CUI 当前颜色
=======================================
*/
CR_API bool_t
cui_get_color (
  __CR_OT__ int16u* color
    )
{
    CONSOLE_SCREEN_BUFFER_INFO  info;

    if (g_cui_out == NULL)
        return (FALSE);
    if (!GetConsoleScreenBufferInfo(g_cui_out, &info))
        return (FALSE);
    *color = info.wAttributes;
    return (TRUE);
}

/*
=======================================
    合成 CUI 输出属性
=======================================
*/
CR_API int16u
cui_make_attr (
  __CR_IN__ int16u  color,
  __CR_IN__ uint_t  attrib
    )
{
/*  低8位的属性是一致的
    if ((attrib & CR_CUI_TEXT_BLUE)  != 0)
        color |= FOREGROUND_BLUE;
    if ((attrib & CR_CUI_TEXT_GREEN) != 0)
        color |= FOREGROUND_GREEN;
    if ((attrib & CR_CUI_TEXT_RED)   != 0)
        color |= FOREGROUND_RED;
    if ((attrib & CR_CUI_TEXT_LIGHT) != 0)
        color |= FOREGROUND_INTENSITY;

    if ((attrib & CR_CUI_BACK_BLUE)  != 0)
        color |= BACKGROUND_BLUE;
    if ((attrib & CR_CUI_BACK_GREEN) != 0)
        color |= BACKGROUND_GREEN;
    if ((attrib & CR_CUI_BACK_RED)   != 0)
        color |= BACKGROUND_RED;
    if ((attrib & CR_CUI_BACK_LIGHT) != 0)
        color |= BACKGROUND_INTENSITY;
*/
    if ((attrib & CR_CUI_REVERSE)    != 0)
        color |= COMMON_LVB_REVERSE_VIDEO;
    if ((attrib & CR_CUI_UNDERSCORE) != 0)
        color |= COMMON_LVB_UNDERSCORE;
    return (color | (int16u)attrib);
}

/*
=======================================
    设置 CUI 当前位置
=======================================
*/
CR_API bool_t
cui_set_xy (
  __CR_IN__ int16u  x,
  __CR_IN__ int16u  y
    )
{
    COORD   xy;

    if (g_cui_out == NULL)
        return (FALSE);
    xy.X = x;
    xy.Y = y;
    if (!SetConsoleCursorPosition(g_cui_out, xy))
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    获取 CUI 当前位置
=======================================
*/
CR_API bool_t
cui_get_xy (
  __CR_OT__ int16u* x,
  __CR_OT__ int16u* y
    )
{
    CONSOLE_SCREEN_BUFFER_INFO  info;

    if (g_cui_out == NULL)
        return (FALSE);
    if (!GetConsoleScreenBufferInfo(g_cui_out, &info))
        return (FALSE);
    if (x != NULL)
        *x = info.dwCursorPosition.X;
    if (y != NULL)
        *y = info.dwCursorPosition.Y;
    return (TRUE);
}

#endif  /* !_CR_GUI_ONLY_ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
