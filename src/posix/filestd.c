/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2009-12-26  */
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
/*  >>>>>>>>>>>>>>>>>>>> CrHack 文件操作函数库 for LIBC <<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "safe.h"
#include "fileio.h"
#if defined(_CR_USE_GLIBC_)
    #include "memlib.h"
    #include "strlib.h"
#endif

#include <stdio.h>

/* 用这个来转换文件偏移类型 */
#if defined(_CR_NO_STDIO64_)
    typedef int32s  foffset_t;
#else
    typedef int64s  foffset_t;
#endif

/* 根据 C 库的支持情况映射函数 */
#if     defined(_CR_USE_GLIBC_)
    #define cr_fseek    fseeko
    #define cr_ftell    ftello

#elif   defined(_CR_MS_STDIO64_)
    #define cr_fseek    _fseeki64
    #define cr_ftell    _ftelli64

    /* VS2005 开始 stdio.h 里才有这两个函数的声明 */
    #if defined(_CR_CC_MSC_) && (_CR_CC_VER_ < 1400)
        CR_C_FUNC   int   __cdecl _fseeki64 (FILE*, __int64, int);
        CR_C_FUNC __int64 __cdecl _ftelli64 (FILE*);
    #endif
#else
    #define cr_fseek    fseek
    #define cr_ftell    ftell
#endif

/*****************************************************************************/
/*                       基本 I/O 操作 (支持二级缓冲)                        */
/*****************************************************************************/

/* 打开文件模式A */
static const ansi_t* s_open_modeA[] =
{
    "rb", "wb", "r+b", "a+b",
};

/* 打开文件模式W */
#if !defined(_CR_USE_GLIBC_)
static const wide_t* s_open_modeW[] =
{
    L"rb", L"wb", L"r+b", L"a+b",
};
#endif

/*
=======================================
    文件打开A
=======================================
*/
CR_API fbuf_t
file_buf_openA (
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ uint_t          mode
    )
{
    mode &= (~CR_FO_SEQ);
    if (mode > CR_FO_AW)
        return (NULL);

/* 有些编译器的 C 库不支持64位文件操作, 过滤之 */
#if defined(_CR_NO_STDIO64_)
    if (mode != CR_FO_WO) {
        if (file_sizeA(name) > ((int32u)-1) / 2)
            return (NULL);
    }
#endif
    return ((fbuf_t)fopen(name, s_open_modeA[mode]));
}

/*
=======================================
    文件打开W
=======================================
*/
CR_API fbuf_t
file_buf_openW (
  __CR_IN__ const wide_t*   name,
  __CR_IN__ uint_t          mode
    )
{
#if defined(_CR_USE_GLIBC_)
    FILE*   file;
    ansi_t* ansi;
#endif
    mode &= (~CR_FO_SEQ);
    if (mode > CR_FO_AW)
        return (NULL);

/* 有些编译器的 C 库不支持64位文件操作, 过滤之 */
#if defined(_CR_NO_STDIO64_)
    if (mode != CR_FO_WO) {
        if (file_sizeW(name) > ((int32u)-1) / 2)
            return (NULL);
    }
#endif
#if defined(_CR_USE_GLIBC_)
    ansi = utf16_to_local(CR_LOCAL, name);
    if (ansi == NULL)
        return (NULL);
    file = fopen(ansi, s_open_modeA[mode]);
    mem_free(ansi);
    return ((fbuf_t)file);
#else
    return ((fbuf_t)_wfopen(name, s_open_modeW[mode]));
#endif
}

/*
=======================================
    文件关闭
=======================================
*/
CR_API void_t
file_buf_close (
  __CR_IN__ fbuf_t  file
    )
{
    fclose((FILE*)file);
}

/*
=======================================
    返回文件指针位置
=======================================
*/
CR_API fsize_t
file_buf_tell (
  __CR_IN__ fbuf_t  file
    )
{
    return ((fsize_t)cr_ftell((FILE*)file));
}

/*
=======================================
    返回文件大小
=======================================
*/
CR_API fsize_t
file_buf_size (
  __CR_IN__ fbuf_t  file
    )
{
    fsize_t size, back;

    back = file_buf_tell(file);
    if (back == CR_F_ERROR)
        return (CR_F_ERROR);
    if (!file_buf_seek(file, 0, SEEK_END))
        size = CR_F_ERROR;
    else
        size = file_buf_tell(file);
    file_buf_seek(file, back, SEEK_SET);
    return (size);
}

/*
=======================================
    文件指针定位
=======================================
*/
CR_API bool_t
file_buf_seek (
  __CR_IN__ fbuf_t  file,
  __CR_IN__ fdist_t offset,
  __CR_IN__ uint_t  whence
    )
{
    if (whence > SEEK_END)
        return (FALSE);

/* 偏移截断检查, 有些编译器的 C 库不支持64位文件操作 */
#if defined(_CR_FILE64_) && defined(_CR_NO_STDIO64_)
    if (cut_int64_32s(NULL, offset))
        return (FALSE);
#endif
    if (cr_fseek((FILE*)file, (foffset_t)offset, whence) != 0)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    刷新文件缓冲区
=======================================
*/
CR_API bool_t
file_buf_flush (
  __CR_IN__ fbuf_t  file
    )
{
    if (fflush((FILE*)file) != 0)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    读取文件数据
=======================================
*/
CR_API leng_t
file_buf_read (
  __CR_OT__ void_t* data,
  __CR_IN__ leng_t  size,
  __CR_IN__ fbuf_t  file
    )
{
    return ((leng_t)fread(data, 1, size, (FILE*)file));
}

/*
=======================================
    写入文件数据
=======================================
*/
CR_API leng_t
file_buf_write (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size,
  __CR_IN__ fbuf_t          file
    )
{
    return ((leng_t)fwrite(data, 1, size, (FILE*)file));
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
