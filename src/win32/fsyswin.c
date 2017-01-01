/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2009-12-28  */
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
/*  >>>>>>>>>>>>>>>>>> CrHack 文件系统函数库 for Windows <<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "datlib.h"
#include "fileio.h"
#include "strlib.h"

#include <windows.h>

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
    if (!DeleteFileA(name))
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
    if (!DeleteFileW(name))
        return (FALSE);
    return (TRUE);
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
    if (!MoveFileA(old_name, new_name))
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
    if (!MoveFileW(old_name, new_name))
        return (FALSE);
    return (TRUE);
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
    DWORD   hi;
    DWORD   lo;
    DWORD   err;
    HANDLE  fmap;
    HANDLE  file;
    void_t* data;
    fsize_t size;

    /* 打开只读文件 */
    file = CreateFileA(name, GENERIC_READ, FILE_SHARE_READ, NULL,
                       OPEN_EXISTING, FILE_FLAG_RANDOM_ACCESS, NULL);
    if (file == INVALID_HANDLE_VALUE)
        return (NULL);

    /* 获取文件大小 */
    lo = GetFileSize(file, &hi);
    err = GetLastError();
    if (lo == INVALID_FILE_SIZE && err != NO_ERROR)
        goto _failure;

    /* 生成文件映射 */
    fmap = CreateFileMapping(file, NULL, PAGE_READONLY, hi, lo, NULL);
    if (fmap == NULL)
        goto _failure;

    /* 获取映射地址 */
    data = MapViewOfFile(fmap, FILE_MAP_READ, 0, 0, 0);
    if (data == NULL) {
        CloseHandle(fmap);
        goto _failure;
    }
    size = mk_size(hi, lo);
    vfile->size = (leng_t )size;
    vfile->data = (void_t*)data;
    vfile->fmap = (void_t*)fmap;
    vfile->file = (void_t*)file;
    return (data);

_failure:
    CloseHandle(file);
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
    DWORD   hi;
    DWORD   lo;
    DWORD   err;
    HANDLE  fmap;
    HANDLE  file;
    void_t* data;
    fsize_t size;

    /* 打开只读文件 */
    file = CreateFileW(name, GENERIC_READ, FILE_SHARE_READ, NULL,
                       OPEN_EXISTING, FILE_FLAG_RANDOM_ACCESS, NULL);
    if (file == INVALID_HANDLE_VALUE)
        return (NULL);

    /* 获取文件大小 */
    lo = GetFileSize(file, &hi);
    err = GetLastError();
    if (lo == INVALID_FILE_SIZE && err != NO_ERROR)
        goto _failure;

    /* 生成文件映射 */
    fmap = CreateFileMapping(file, NULL, PAGE_READONLY, hi, lo, NULL);
    if (fmap == NULL)
        goto _failure;

    /* 获取映射地址 */
    data = MapViewOfFile(fmap, FILE_MAP_READ, 0, 0, 0);
    if (data == NULL) {
        CloseHandle(fmap);
        goto _failure;
    }
    size = mk_size(hi, lo);
    vfile->size = (leng_t )size;
    vfile->data = (void_t*)data;
    vfile->fmap = (void_t*)fmap;
    vfile->file = (void_t*)file;
    return (data);

_failure:
    CloseHandle(file);
    return (NULL);
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
    UnmapViewOfFile(vfile->data);
    CloseHandle((HANDLE)vfile->fmap);
    CloseHandle((HANDLE)vfile->file);
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

    /* 跳过盘符 */
    if (dir[1] == CR_AC(':') && is_slashA(dir[2]))
        ptr += 3;
    for (; *ptr != CR_AC(NIL); ptr++) {
        if (is_slashA(*ptr)) {
            *ptr = CR_AC('\0');
            CreateDirectoryA(dir, NULL);
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
    wide_t* ptr;
    wide_t* dir = str_dupW(path);

    if (dir == NULL)
        return (FALSE);
    ptr = dir;

    /* 跳过盘符 */
    if (dir[1] == CR_WC(':') && is_slashW(dir[2]))
        ptr += 3;
    for (; *ptr != CR_NC(NIL); ptr++) {
        if (is_slashW(*ptr)) {
            *ptr = CR_WC('\0');
            CreateDirectoryW(dir, NULL);
            *ptr = CR_WC('\\');
        }
    }
    mem_free(dir);
    return (TRUE);
}

/*
---------------------------------------
    目录释放回调
---------------------------------------
*/
static void_t
dir_free (
  __CR_IN__ void_t* obj
    )
{
    mem_free(*(void_t**)obj);
}

/*
---------------------------------------
    判断是否为特殊目录A
---------------------------------------
*/
static bool_t
is_spec_dirA (
  __CR_IN__ const ansi_t*   name
    )
{
    if (str_cmpA(name, CR_AS(".")) == 0)
        return (TRUE);
    if (str_cmpA(name, CR_AS("..")) == 0)
        return (TRUE);
    return (FALSE);
}

/*
---------------------------------------
    判断是否为特殊目录W
---------------------------------------
*/
static bool_t
is_spec_dirW (
  __CR_IN__ const wide_t*   name
    )
{
    if (str_cmpW(name, CR_WS(".")) == 0)
        return (TRUE);
    if (str_cmpW(name, CR_WS("..")) == 0)
        return (TRUE);
    return (FALSE);
}

/*
---------------------------------------
    目录属性过滤
---------------------------------------
*/
cr_inline bool_t
is_okay_dirs (
  __CR_IN__ bool_t  hide,
  __CR_IN__ int32u  attr
    )
{
    if (attr & FILE_ATTRIBUTE_DIRECTORY) {
        if (hide) {
            if (attr & FILE_ATTRIBUTE_HIDDEN)
                return (FALSE);
        }
        return (TRUE);
    }
    return (FALSE);
}

/*
---------------------------------------
    文件属性过滤
---------------------------------------
*/
cr_inline bool_t
is_okay_file (
  __CR_IN__ bool_t  hide,
  __CR_IN__ bool_t  fdir,
  __CR_IN__ int32u  attr
    )
{
    if (hide) {
        if (attr & FILE_ATTRIBUTE_HIDDEN)
            return (FALSE);
    }
    if (attr & FILE_ATTRIBUTE_DIRECTORY)
        return (fdir);
    return (TRUE);
}

/*
---------------------------------------
    搜索指定扩展名的文件A (递归使用)
---------------------------------------
*/
static bool_t
search_filesA (
  __CR_IN__ const ansi_t*   root,
  __CR_IN__ bool_t          sub,
  __CR_IN__ bool_t          hide,
  __CR_IN__ bool_t          fdir,
  __CR_IN__ const ansi_t*   match,
  __CR_IN__ search_filesA_t handler,
  __CR_IN__ void_t*         param,
  __CR_IN__ uint_t          level
    )
{
    leng_t              len;
    leng_t              beg;
    bool_t              rett;
    HANDLE              find;
    sARRAY              dirs;
    ansi_t*             full;
    ansi_t*             temp;
    sSEARCHa            info;
    WIN32_FIND_DATAA    data;

    /* 安全检查 */
    beg = str_lenA(root);
    if (beg >= MAX_PATHA)
        return (FALSE);
    len = str_lenA(match);
    if (len >= MAX_PATHA)
        return (FALSE);

    /* 把通配符处的路径合到根路径上来 */
    full = str_allocA(MAX_PATHW);
    if (full == NULL)
        return (FALSE);
    for (; len != 0; len--) {
        if (is_slashA(match[len - 1]))
            break;
    }
    chr_cpyA(full, root, beg);
    if (len != 0) {
        chr_cpyA(full + beg, match, len);
        match += len;
        beg += len;
    }

    /* 初始化列表 */
    array_initT(&dirs, ansi_t*);
    dirs.free = dir_free;

    /* 不需要处理目录 */
    if (!sub)
        goto _find_file;

    /* 查找所有目录 */
    chr_cpyA(full + beg, CR_AS("*.*"), 4);
    struct_zero(&data, WIN32_FIND_DATAA);
    find = FindFirstFileA(full, &data);
    if (find == INVALID_HANDLE_VALUE)
        goto _find_file;
    do
    {
        /* 根据参数过滤目录 */
        if (is_okay_dirs(hide, data.dwFileAttributes) &&
            !is_spec_dirA(data.cFileName))
        {
            /* 加入目录列表 */
            str_cpyA(full + beg, data.cFileName);
            len = str_lenA(full);
            full[len++] = CR_AC('\\');
            full[len++] = CR_AC('\0');
            temp = str_allocA(len);
            if (temp == NULL)
                goto _failure;
            chr_cpyA(temp, full, len);
            if (array_push_growT(&dirs, ansi_t*, &temp) == NULL) {
                mem_free(temp);
                goto _failure;
            }
        }
    } while (FindNextFileA(find, &data));

    FindClose(find);

    /* 查找指定文件 */
_find_file:
    str_cpyA(full + beg, match);
    struct_zero(&data, WIN32_FIND_DATAA);
    find = FindFirstFileA(full, &data);
    if (find == INVALID_HANDLE_VALUE)
        goto _next_level;
    do
    {
        /* 根据参数过滤文件 */
        if (is_okay_file(hide, fdir, data.dwFileAttributes) &&
            !is_spec_dirA(data.cFileName))
        {
            /* 填充数据 */
            info.level = level;
            info.attr = data.dwFileAttributes;
            str_cpyA(full + beg, data.cFileName);
            str_cpyNA(info.name, full, MAX_PATHA);
            info.size = mk_size(data.nFileSizeHigh, data.nFileSizeLow);
            if (!handler(param, &info))
                goto _failure;
        }
    } while (FindNextFileA(find, &data));

    FindClose(find);

    /* 对子目录遍列 */
_next_level:
    rett = TRUE;
    if (sub) {
        beg = array_get_sizeT(&dirs, ansi_t*);
        for (len = 0; len < beg; len++) {
            temp = array_get_unitT(&dirs, ansi_t*, len)[0];
            rett = search_filesA(temp, TRUE, hide, fdir, match,
                                 handler, param, level + 1);
            if (!rett) break;
        }
    }
    mem_free(full);
    array_freeT(&dirs, ansi_t*);
    return (rett);

_failure:
    mem_free(full);
    array_freeT(&dirs, ansi_t*);
    return (FALSE);
}

/*
---------------------------------------
    搜索指定扩展名的文件W (递归使用)
---------------------------------------
*/
static bool_t
search_filesW (
  __CR_IN__ const wide_t*   root,
  __CR_IN__ bool_t          sub,
  __CR_IN__ bool_t          hide,
  __CR_IN__ bool_t          fdir,
  __CR_IN__ const wide_t*   match,
  __CR_IN__ search_filesW_t handler,
  __CR_IN__ void_t*         param,
  __CR_IN__ uint_t          level
    )
{
    leng_t              len;
    leng_t              beg;
    bool_t              rett;
    HANDLE              find;
    sARRAY              dirs;
    wide_t*             full;
    wide_t*             temp;
    sSEARCHw            info;
    WIN32_FIND_DATAW    data;

    /* 安全检查 */
    beg = str_lenW(root);
    if (beg >= MAX_PATHA)
        return (FALSE);
    len = str_lenW(match);
    if (len >= MAX_PATHA)
        return (FALSE);

    /* 把通配符处的路径合到根路径上来 */
    full = str_allocW(MAX_PATHW);
    if (full == NULL)
        return (FALSE);
    for (; len != 0; len--) {
        if (is_slashW(match[len - 1]))
            break;
    }
    chr_cpyW(full, root, beg);
    if (len != 0) {
        chr_cpyW(full + beg, match, len);
        match += len;
        beg += len;
    }

    /* 初始化列表 */
    array_initT(&dirs, wide_t*);
    dirs.free = dir_free;

    /* 不需要处理目录 */
    if (!sub)
        goto _find_file;

    /* 查找所有目录 */
    chr_cpyW(full + beg, CR_WS("*.*"), 4);
    struct_zero(&data, WIN32_FIND_DATAW);
    find = FindFirstFileW(full, &data);
    if (find == INVALID_HANDLE_VALUE)
        goto _find_file;
    do
    {
        /* 根据参数过滤目录 */
        if (is_okay_dirs(hide, data.dwFileAttributes) &&
            !is_spec_dirW(data.cFileName))
        {
            /* 加入目录列表 */
            str_cpyW(full + beg, data.cFileName);
            len = str_lenW(full);
            full[len++] = CR_WC('\\');
            full[len++] = CR_WC('\0');
            temp = str_allocW(len);
            if (temp == NULL)
                goto _failure;
            chr_cpyW(temp, full, len);
            if (array_push_growT(&dirs, wide_t*, &temp) == NULL) {
                mem_free(temp);
                goto _failure;
            }
        }
    } while (FindNextFileW(find, &data));

    FindClose(find);

    /* 查找指定文件 */
_find_file:
    str_cpyW(full + beg, match);
    struct_zero(&data, WIN32_FIND_DATAW);
    find = FindFirstFileW(full, &data);
    if (find == INVALID_HANDLE_VALUE)
        goto _next_level;
    do
    {
        /* 根据参数过滤文件 */
        if (is_okay_file(hide, fdir, data.dwFileAttributes) &&
            !is_spec_dirW(data.cFileName))
        {
            /* 填充数据 */
            info.level = level;
            info.attr = data.dwFileAttributes;
            str_cpyW(full + beg, data.cFileName);
            str_cpyNW(info.name, full, MAX_PATHA);
            info.size = mk_size(data.nFileSizeHigh, data.nFileSizeLow);
            if (!handler(param, &info))
                goto _failure;
        }
    } while (FindNextFileW(find, &data));

    FindClose(find);

    /* 对子目录遍列 */
_next_level:
    rett = TRUE;
    if (sub) {
        beg = array_get_sizeT(&dirs, wide_t*);
        for (len = 0; len < beg; len++) {
            temp = array_get_unitT(&dirs, wide_t*, len)[0];
            rett = search_filesW(temp, TRUE, hide, fdir, match,
                                 handler, param, level + 1);
            if (!rett) break;
        }
    }
    mem_free(full);
    array_freeT(&dirs, wide_t*);
    return (rett);

_failure:
    mem_free(full);
    array_freeT(&dirs, wide_t*);
    return (FALSE);
}

/*
=======================================
    搜索指定扩展名的文件A
=======================================
*/
CR_API bool_t
file_searchA (
  __CR_IN__ const ansi_t*   root,
  __CR_IN__ bool_t          sub,
  __CR_IN__ bool_t          hide,
  __CR_IN__ bool_t          fdir,
  __CR_IN__ const ansi_t*   match,
  __CR_IN__ search_filesA_t handler,
  __CR_IN__ void_t*         param
    )
{
    bool_t  ret;
    ansi_t* dir = NULL;
    leng_t  len = str_lenA(root);

    /* 加上目录结尾 */
    if (len != 0 && !is_slashA(root[len - 1])) {
        dir = str_allocA(len + 2);
        if (dir == NULL)
            return (FALSE);
        root = chr_cpyA(dir, root, len);
        dir[len + 0] = CR_AC('\\');
        dir[len + 1] = CR_AC('\0');
    }
    ret = search_filesA(root, sub, hide, fdir, match, handler, param, 0);
    TRY_FREE(dir);
    return (ret);
}

/*
=======================================
    搜索指定扩展名的文件W
=======================================
*/
CR_API bool_t
file_searchW (
  __CR_IN__ const wide_t*   root,
  __CR_IN__ bool_t          sub,
  __CR_IN__ bool_t          hide,
  __CR_IN__ bool_t          fdir,
  __CR_IN__ const wide_t*   match,
  __CR_IN__ search_filesW_t handler,
  __CR_IN__ void_t*         param
    )
{
    bool_t  ret;
    wide_t* dir = NULL;
    leng_t  len = str_lenW(root);

    /* 加上目录结尾 */
    if (len != 0 && !is_slashW(root[len - 1])) {
        dir = str_allocW(len + 2);
        if (dir == NULL)
            return (FALSE);
        root = chr_cpyW(dir, root, len);
        dir[len + 0] = CR_WC('\\');
        dir[len + 1] = CR_WC('\0');
    }
    ret = search_filesW(root, sub, hide, fdir, match, handler, param, 0);
    TRY_FREE(dir);
    return (ret);
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
