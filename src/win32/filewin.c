/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2009-12-23  */
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
/*  >>>>>>>>>>>>>>>>>> CrHack 文件操作函数库 for Windows <<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "fileio.h"
#include "memlib.h"

#include <windows.h>

/*****************************************************************************/
/*                      基本 I/O 操作 (不支持二级缓冲)                       */
/*****************************************************************************/

/* 文件内部结构 */
typedef struct
{
        HANDLE  file;   /* 文件句柄 */
        uint_t  mode;   /* 打开模式 */

} sWFILE;

/* 打开文件模式 */
static const DWORD s_create_mode[] =
{
    OPEN_EXISTING, CREATE_ALWAYS, OPEN_EXISTING, OPEN_ALWAYS,
};

/* 文件读写模式 */
static const DWORD s_access_mode[] =
{
    GENERIC_READ, GENERIC_WRITE, GENERIC_READ | GENERIC_WRITE,
    GENERIC_READ | GENERIC_WRITE,
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
    int32u  flag;
    HANDLE  file;
    sWFILE* rett;

    /* 处理额外的标志 */
    if (mode & CR_FO_SEQ) {
        mode &= (~CR_FO_SEQ);
        flag = FILE_FLAG_SEQUENTIAL_SCAN;
    }
    else {
        flag = FILE_FLAG_RANDOM_ACCESS;
    }

    /* 过滤打开模式 */
    if (mode > CR_FO_AW)
        return (NULL);

    /* 分配结构内存 */
    rett = struct_new(sWFILE);
    if (rett == NULL)
        return (NULL);

    /* 打开文件 */
    file = CreateFileA(name, s_access_mode[mode], FILE_SHARE_READ,
                       NULL, s_create_mode[mode], flag, NULL);
    if (file == INVALID_HANDLE_VALUE) {
        mem_free(rett);
        return (NULL);
    }
    rett->file = file;
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
    int32u  flag;
    HANDLE  file;
    sWFILE* rett;

    /* 处理额外的标志 */
    if (mode & CR_FO_SEQ) {
        mode &= (~CR_FO_SEQ);
        flag = FILE_FLAG_SEQUENTIAL_SCAN;
    }
    else {
        flag = FILE_FLAG_RANDOM_ACCESS;
    }

    /* 过滤打开模式 */
    if (mode > CR_FO_AW)
        return (NULL);

    /* 分配结构内存 */
    rett = struct_new(sWFILE);
    if (rett == NULL)
        return (NULL);

    /* 打开文件 */
    file = CreateFileW(name, s_access_mode[mode], FILE_SHARE_READ,
                       NULL, s_create_mode[mode], flag, NULL);
    if (file == INVALID_HANDLE_VALUE) {
        mem_free(rett);
        return (NULL);
    }
    rett->file = file;
    rett->mode = mode;
    return (rett);
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
    CloseHandle(real->file);
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
    LONG    hi = 0;
    DWORD   err, lo = 0;
    sWFILE* real = (sWFILE*)file;

    lo = SetFilePointer(real->file, lo, &hi, FILE_CURRENT);
    err = GetLastError();
    if (lo == INVALID_SET_FILE_POINTER && err != NO_ERROR)
        return (CR_F_ERROR);
    return (mk_size(hi, lo));
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
    DWORD   hi, lo, err;
    sWFILE* real = (sWFILE*)file;

    lo = GetFileSize(real->file, &hi);
    err = GetLastError();
    if (lo == INVALID_FILE_SIZE && err != NO_ERROR)
        return (CR_F_ERROR);
    return (mk_size(hi, lo));
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
    LONG*   ptr;
    DWORD   err, ret;
    sWFILE* real = (sWFILE*)file;

    union {
        int64s  u;
        struct {
            LONG    a, b;
        } s;
    } tmp;

    if (whence > SEEK_END)
        return (FALSE);
    tmp.u = offset;
    ptr = (LONG*)(&tmp.s.a);

#if defined(_CR_ORDER_LE_)
    ret = SetFilePointer(real->file, ptr[0], &ptr[1], whence);
#else   /* (_CR_ORDER_BE_) */
    ret = SetFilePointer(real->file, ptr[1], &ptr[0], whence);
#endif
    err = GetLastError();
    if (ret == INVALID_SET_FILE_POINTER && err != NO_ERROR)
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

    if (!FlushFileBuffers(real->file))
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
    DWORD   tmp;
    sWFILE* real = (sWFILE*)file;

#ifndef _CR_SYS64_
    ReadFile(real->file, data, size, &tmp, NULL);
    return (tmp);
#else
    leng_t  red = 0;
    uchar*  buf = (uchar*)(data);
    leng_t  blk = (leng_t)(size / CR_SPLITE);
    uint_t  rst = (uint_t)(size % CR_SPLITE);

    for (; blk != 0; blk--, buf += CR_SPLITE, red += tmp) {
        if (!ReadFile(real->file, buf, CR_SPLITE, &tmp, NULL))
            return (red + tmp);
        if (tmp != CR_SPLITE)
            return (red + tmp);
    }
    if (rst != 0) {
        ReadFile(real->file, buf, rst, &tmp, NULL);
        red += tmp;
    }
    return (red);
#endif
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
#ifdef  _CR_SYS64_
    uchar*  buf;
    leng_t  wrt;
    leng_t  blk;
    uint_t  rst;
#endif
    DWORD   err, tmp;
    sWFILE* real = (sWFILE*)file;

    /* 定位到文件最后 */
    if (real->mode == CR_FO_AW) {
        tmp = SetFilePointer(real->file, 0, NULL, FILE_END);
        err = GetLastError();
        if (tmp == INVALID_SET_FILE_POINTER && err != NO_ERROR)
            return (0);
    }

#ifndef _CR_SYS64_
    WriteFile(real->file, data, size, &tmp, NULL);
    return (tmp);
#else
    wrt = 0;
    buf = (uchar*)(data);
    blk = (leng_t)(size / CR_SPLITE);
    rst = (uint_t)(size % CR_SPLITE);
    for (; blk != 0; blk--, buf += CR_SPLITE, wrt += tmp) {
        if (!WriteFile(real->file, buf, CR_SPLITE, &tmp, NULL))
            return (wrt + tmp);
        if (tmp != CR_SPLITE)
            return (wrt + tmp);
    }
    if (rst != 0) {
        WriteFile(real->file, buf, rst, &tmp, NULL);
        wrt += tmp;
    }
    return (wrt);
#endif
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
