/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2016-04-03  */
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
/*  >>>>>>>>>>>>>>>>>>> CrHack 文件系统函数库 for POSIX <<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "safe.h"
#include "fileio.h"
#include "memlib.h"
#include "strlib.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

/*
=======================================
    删除一个文件A
=======================================
*/
CR_API bool_t
file_deleteA (
  __CR_IN__ const ansi_t*   name
    )
{
    if (unlink(name) != 0)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    删除一个文件W
=======================================
*/
CR_API bool_t
file_deleteW (
  __CR_IN__ const wide_t*   name
    )
{
    sint_t  retc;
    ansi_t* ansi;

    ansi = utf16_to_local(CR_LOCAL, name);
    if (ansi == NULL)
        return (FALSE);
    retc = unlink(ansi);
    mem_free(ansi);
    return ((retc != 0) ? FALSE : TRUE);
}

/*
=======================================
    重命名一个文件A
=======================================
*/
CR_API bool_t
file_renameA (
  __CR_IN__ const ansi_t*   new_name,
  __CR_IN__ const ansi_t*   old_name
    )
{
    if (rename(old_name, new_name) != 0)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    重命名一个文件W
=======================================
*/
CR_API bool_t
file_renameW (
  __CR_IN__ const wide_t*   new_name,
  __CR_IN__ const wide_t*   old_name
    )
{
    sint_t  retc;
    ansi_t* ansi_new;
    ansi_t* ansi_old;

    ansi_new = utf16_to_local(CR_LOCAL, new_name);
    if (ansi_new == NULL)
        return (FALSE);

    ansi_old = utf16_to_local(CR_LOCAL, old_name);
    if (ansi_old == NULL) {
        mem_free(ansi_new);
        return (FALSE);
    }

    retc = rename(ansi_old, ansi_new);
    mem_free(ansi_old);
    mem_free(ansi_new);
    return ((retc != 0) ? FALSE : TRUE);
}

/*
=======================================
    建立内存映射文件A
=======================================
*/
CR_API void_t*
file_mappingA (
  __CR_IN__ const ansi_t*   name,
  __CR_OT__ sVFILE*         vfile
    )
{
    sint_t  file;
    void_t* data;
    fsize_t size;

    file = open(name, O_RDONLY);
    if (file < 0)
        return (NULL);
    size = file_raw_size((fraw_t)((leng_t)file));
    if (size == CR_F_ERROR)
        goto _failure;
#if defined(_CR_FILE64_) && defined(_CR_SYS32_)
    if (cut_int64u(NULL, size))
        goto _failure;
#endif
    data = mmap(0, (size_t)size, PROT_READ, MAP_SHARED, file, 0);
    if (data == MAP_FAILED)
        goto _failure;
    vfile->size = (leng_t )size;
    vfile->data = (void_t*)data;
    vfile->fmap = (void_t*)NULL;
    vfile->file = (void_t*)((leng_t)file);
    return (data);

_failure:
    close(file);
    return (NULL);
}

/*
=======================================
    建立内存映射文件W
=======================================
*/
CR_API void_t*
file_mappingW (
  __CR_IN__ const wide_t*   name,
  __CR_OT__ sVFILE*         vfile
    )
{
    void_t* data;
    ansi_t* ansi;

    ansi = utf16_to_local(CR_LOCAL, name);
    if (ansi == NULL)
        return (NULL);
    data = file_mappingA(ansi, vfile);
    mem_free(ansi);
    return (data);
}

/*
=======================================
    释放内存映射文件
=======================================
*/
CR_API void_t
file_release (
  __CR_IN__ const sVFILE*   vfile
    )
{
    munmap(vfile->data, vfile->size);
    close((int)((leng_t)vfile->file));
}

/*
=======================================
    创建一串目录A
=======================================
*/
CR_API bool_t
path_buildA (
  __CR_IN__ const ansi_t*   path
    )
{
    ansi_t* ptr;
    ansi_t* dir = str_dupA(path);

    if (dir == NULL)
        return (FALSE);
    ptr = dir;

    /* 跳过根目录 */
    if (is_slashA(*dir))
        ptr += 1;
    for (; *ptr != CR_AC(NIL); ptr++) {
        if (is_slashA(*ptr)) {
            *ptr = CR_AC('\0');
            mkdir(dir, S_IRWXG);
            *ptr = CR_AC('\\');
        }
    }
    mem_free(dir);
    return (TRUE);
}

/*
=======================================
    创建一串目录W
=======================================
*/
CR_API bool_t
path_buildW (
  __CR_IN__ const wide_t*   path
    )
{
    bool_t  retc;
    ansi_t* ansi;

    ansi = utf16_to_local(CR_LOCAL, path);
    if (ansi == NULL)
        return (FALSE);
    retc = path_buildA(ansi);
    mem_free(ansi);
    return (retc);
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
