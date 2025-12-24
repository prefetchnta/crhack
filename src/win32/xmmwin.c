/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2012-06-21  */
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
/*  >>>>>>>>>>>>>>>>> CrHack 系统多媒体函数库 for Windows <<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "extz.h"
#if defined(_CR_API_WIDE_ONLY_)
    #include "memlib.h"
    #include "strlib.h"
#else
    #define _WIN32_DCOM
#endif

#include <windows.h>
#include <objbase.h>
#include <mmsystem.h>

/* Windows 多媒体导入库 */
#ifndef _CR_NO_PRAGMA_LIB_
    #if !defined(_CR_CC_BCC_)
        #pragma comment (lib, "winmm.lib")
    #endif
#endif

/*****************************************************************************/
/*                               Windows COM                                 */
/*****************************************************************************/

/*
=======================================
    COM 加载
=======================================
*/
CR_API bool_t
com_init (void_t)
{
    HRESULT rett;

    rett = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (FAILED(rett))
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    COM 释放
=======================================
*/
CR_API void_t
com_kill (void_t)
{
    CoUninitialize();
}

/*****************************************************************************/
/*                               Windows WAV                                 */
/*****************************************************************************/

/*
---------------------------------------
    解析设置标志
---------------------------------------
*/
static DWORD
win_wav_flags (
  __CR_IN__ int32u  flags
    )
{
    DWORD   rett = 0;

    /* 循环播放 */
    if (flags & XMM_PLAY_LOOP)
        rett |= SND_LOOP;

    /* 同步播放 */
    if (flags & XMM_PLAY_SYNC)
        rett |= SND_SYNC;
    else
        rett |= SND_ASYNC;
    return (rett);
}

/*
=======================================
    停止播放 WAV 音频
=======================================
*/
CR_API bool_t
win_wav_stop (
  __CR_IN__ int32u  flags
    )
{
    DWORD   used;

    used = win_wav_flags(flags);
    if (!PlaySound(NULL, NULL, used))
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    播放 WAV 数据
=======================================
*/
CR_API bool_t
win_wav_play (
  __CR_IN__ const void_t*   data,
  __CR_IN__ int32u          flags
    )
{
    DWORD   used;

    used = win_wav_flags(flags);
    used |= SND_MEMORY;
    if (!PlaySound((LPCTSTR)data, NULL, used))
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    播放 WAV 文件A
=======================================
*/
CR_API bool_t
win_wav_playA (
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ int32u          flags
    )
{
    DWORD   used;

#ifndef _CR_API_WIDE_ONLY_
    used = win_wav_flags(flags);
    used |= SND_FILENAME;
    if (!PlaySoundA(name, NULL, used))
        return (FALSE);
    return (TRUE);
#else
    wide_t* wname;

    wname = local_to_utf16(CR_LOCAL, name);
    if (wname == NULL)
        return (FALSE);
    used = win_wav_flags(flags);
    used |= SND_FILENAME;
    if (!PlaySoundW(wname, NULL, used)) {
        mem_free(wname);
        return (FALSE);
    }
    mem_free(wname);
    return (TRUE);
#endif
}

/*
=======================================
    播放 WAV 文件W
=======================================
*/
CR_API bool_t
win_wav_playW (
  __CR_IN__ const wide_t*   name,
  __CR_IN__ int32u          flags
    )
{
    DWORD   used;

    used = win_wav_flags(flags);
    used |= SND_FILENAME;
    if (!PlaySoundW(name, NULL, used))
        return (FALSE);
    return (TRUE);
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
