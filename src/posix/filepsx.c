/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-03-03  */
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
/*  >>>>>>>>>>>>>>>>>>> CrHack 文件操作函数库 for POSIX <<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "fileio.h"
#include "memlib.h"
#include "strlib.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/* 根据文件系统位数映射函数 */
#if !defined(_CR_FILE64_) || defined(_CR_OS_MACOSX_)
    #define cr_stat     stat
    #define cr_lseek    lseek
    #define cr_fstat    fstat
#else
    #define cr_stat     stat64
    #define cr_lseek    lseek64
    #define cr_fstat    fstat64
#endif

/*****************************************************************************/
/*                      基本 I/O 操作 (不支持二级缓冲)                       */
/*****************************************************************************/

/* 打开文件模式 */
static const int s_open_mode[] =
{
    O_RDONLY,
    O_WRONLY | O_CREAT | O_TRUNC,
    O_RDWR,
    O_RDWR | O_CREAT | O_APPEND,
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
    sint_t  fd;

    mode &= (~CR_FO_SEQ);
    if (mode > CR_FO_AW)
        return (NULL);
#if defined(O_CLOEXEC)
    fd = open(name, s_open_mode[mode] | O_CLOEXEC, 0777);
#else
    fd = open(name, s_open_mode[mode], 0777);
#endif
    if (fd < 0)
        return (NULL);
    if (fd == 0) fd = -1;
    return ((fraw_t)((leng_t)fd));
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
    sint_t  fd;
    ansi_t* ansi;

    mode &= (~CR_FO_SEQ);
    if (mode > CR_FO_AW)
        return (NULL);
    ansi = utf16_to_local(CR_LOCAL, name);
    if (ansi == NULL)
        return (NULL);
#if defined(O_CLOEXEC)
    fd = open(ansi, s_open_mode[mode] | O_CLOEXEC, 0777);
#else
    fd = open(ansi, s_open_mode[mode], 0777);
#endif
    mem_free(ansi);
    if (fd < 0)
        return (NULL);
    if (fd == 0) fd = -1;
    return ((fraw_t)((leng_t)fd));
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
    sint_t  fd;

    fd = (sint_t)((leng_t)file);
    if (fd == -1) fd = 0;
    close(fd);
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
    sint_t  fd;

    fd = (sint_t)((leng_t)file);
    if (fd == -1) fd = 0;
    return ((fsize_t)cr_lseek(fd, 0, SEEK_CUR));
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
    sint_t          fd;
    struct cr_stat  info;

    fd = (sint_t)((leng_t)file);
    if (fd == -1) fd = 0;
    if (cr_fstat(fd, &info) != 0)
        return (CR_F_ERROR);
    return ((fsize_t)info.st_size);
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
    sint_t  fd;

    if (whence > SEEK_END)
        return (FALSE);
    fd = (sint_t)((leng_t)file);
    if (fd == -1) fd = 0;
    if (cr_lseek(fd, offset, whence) == -1)
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
    sint_t  fd;

    fd = (sint_t)((leng_t)file);
    if (fd == -1) fd = 0;
    if (fsync(fd) == -1)
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
    sint_t  fd;
    leng_t  back;

    fd = (sint_t)((leng_t)file);
    if (fd == -1) fd = 0;
    back = (leng_t)read(fd, data, size);
    return ((back == (leng_t)-1) ? 0 : back);
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
    sint_t  fd;
    leng_t  back;

    fd = (sint_t)((leng_t)file);
    if (fd == -1) fd = 0;
    back = (leng_t)write(fd, data, size);
    return ((back == (leng_t)-1) ? 0 : back);
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
