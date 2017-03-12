/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2017-03-11  */
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
/*  >>>>>>>>>>>>>>>>>>> CrHack 文件操作函数库 for FatFS <<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "fileio.h"
#include "memlib.h"
#include "strlib.h"

#include <ff.h>

/*****************************************************************************/
/*                      基本 I/O 操作 (不支持二级缓冲)                       */
/*****************************************************************************/

/* 文件内部结构 */
typedef struct
{
        FIL     file;   /* 文件句柄 */
        uint_t  mode;   /* 打开模式 */

} sWFILE;

/* 打开文件模式 */
static const BYTE _rom_ s_open_mode[] =
{
    FA_READ | FA_OPEN_EXISTING,
    FA_WRITE | FA_CREATE_ALWAYS,
    FA_READ | FA_WRITE | FA_OPEN_EXISTING,
    FA_READ | FA_WRITE | FA_OPEN_ALWAYS,
};

/*
=======================================
    文件打开A
=======================================
*/
CR_API fraw_t
file_raw_openA (
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ uint_t          mode
    )
{
    sWFILE* rett;

    /* 过滤打开模式 */
    mode &= (~CR_FO_SEQ);
    if (mode > CR_FO_AW)
        return (NULL);

    /* 分配结构内存 */
    rett = struct_new(sWFILE);
    if (rett == NULL)
        return (NULL);

    /* 打开文件 */
    if (f_open(&rett->file, name, s_open_mode[mode]) != FR_OK) {
        mem_free(rett);
        return (NULL);
    }
    rett->mode = mode;
    return (rett);
}

/*
=======================================
    文件打开W
=======================================
*/
CR_API fraw_t
file_raw_openW (
  __CR_IN__ const wide_t*   name,
  __CR_IN__ uint_t          mode
    )
{
    fraw_t  file;
    ansi_t* ansi;

    ansi = utf16_to_local(CR_LOCAL, name);
    if (ansi == NULL)
        return (NULL);
    file = file_raw_openA(ansi, mode);
    mem_free(ansi);
    return (file);
}

/*
=======================================
    文件关闭
=======================================
*/
CR_API void_t
file_raw_close (
  __CR_IN__ fraw_t  file
    )
{
    sWFILE* real;

    real = (sWFILE*)file;
    f_close(&real->file);
    mem_free(real);
}

/*
=======================================
    返回文件指针位置
=======================================
*/
CR_API fsize_t
file_raw_tell (
  __CR_IN__ fraw_t  file
    )
{
    sWFILE* real = (sWFILE*)file;

    return ((fsize_t)f_tell(&real->file));
}

/*
=======================================
    返回文件大小
=======================================
*/
CR_API fsize_t
file_raw_size (
  __CR_IN__ fraw_t  file
    )
{
    sWFILE* real = (sWFILE*)file;

    return ((fsize_t)f_size(&real->file));
}

/*
=======================================
    文件指针定位
=======================================
*/
CR_API bool_t
file_raw_seek (
  __CR_IN__ fraw_t  file,
  __CR_IN__ fdist_t offset,
  __CR_IN__ uint_t  whence
    )
{
    sWFILE* real = (sWFILE*)file;

    switch (whence)
    {
        default:
            return (FALSE);

        case SEEK_SET:
            break;

        case SEEK_CUR:
            if (offset == 0)
                return (TRUE);
            offset += file_raw_tell(file);
            break;

        case SEEK_END:
            offset += file_raw_size(file);
            break;
    }
    if (f_lseek(&real->file, (FSIZE_t)offset) != FR_OK)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    刷新文件缓冲区
=======================================
*/
CR_API bool_t
file_raw_flush (
  __CR_IN__ fraw_t  file
    )
{
    sWFILE* real = (sWFILE*)file;

    if (f_sync(&real->file) != FR_OK)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    读取文件数据
=======================================
*/
CR_API leng_t
file_raw_read (
  __CR_OT__ void_t* data,
  __CR_IN__ leng_t  size,
  __CR_IN__ fraw_t  file
    )
{
    UINT    red = 0;
    sWFILE* real = (sWFILE*)file;

    f_read(&real->file, data, size, &red);
    return (red);
}

/*
=======================================
    写入文件数据
=======================================
*/
CR_API leng_t
file_raw_write (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size,
  __CR_IN__ fraw_t          file
    )
{
    UINT    wrt = 0;
    sWFILE* real = (sWFILE*)file;

    /* 定位到文件最后 */
    if (real->mode == CR_FO_AW) {
        if (!file_raw_seek(file, 0, SEEK_END))
            return (0);
    }
    f_write(&real->file, data, size, &wrt);
    return (wrt);
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
