/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2016-06-23  */
/*     #######          ###    ###      [UTIL]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>>>>>> FILELIB 函数库 <<<<<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "hash.h"
#include "fileio.h"
#include "memlib.h"
#include "util/filelib.h"

/*
=======================================
    把整个文件当字符串读入 (带 MD5)
=======================================
*/
CR_API ansi_t*
file_load_str_md5 (
  __CR_IN__ const ansi_t*   name
    )
{
    fraw_t  file;
    leng_t  back;
    ansi_t* text;
    fsize_t size;
    /* ------- */
    byte_t  shsh[16];
    byte_t  dhsh[16];

    file = file_raw_openA(name, CR_FO_RO | CR_FO_SEQ);
    if (file == NULL)
        return (NULL);

    size = file_raw_size(file);
    if (size == CR_F_ERROR)
        goto _failure1;

    if (size <= sizeof(shsh) ||
        size > ((leng_t)-1) / 2 - sizeof(int64u))
        goto _failure1;
    size -= sizeof(shsh);

    /* 兼容 UTF-32 编码 */
    text = str_allocA((leng_t)(size + sizeof(int64u)));
    if (text == NULL)
        goto _failure1;

    back = file_raw_read(text, (leng_t)size, file);
    if (back != size)
        goto _failure2;
    hash_md5_total(dhsh, text, back);
    mem_zero(&text[back], sizeof(int64u));

    back = file_raw_read(shsh, sizeof(shsh), file);
    if (back != sizeof(shsh))
        goto _failure2;

    if (mem_cmp(dhsh, shsh, sizeof(shsh)) != 0)
        goto _failure2;

    file_raw_close(file);
    return (text);

_failure2:
    mem_free(text);
_failure1:
    file_raw_close(file);
    return (NULL);
}

/*
=======================================
    读入整个文件 (带 MD5)
=======================================
*/
CR_API void_t*
file_load_bin_md5 (
  __CR_IN__ const ansi_t*   name,
  __CR_OT__ leng_t*         size
    )
{
    fraw_t  file;
    leng_t  back;
    void_t* data;
    fsize_t temp;
    /* ------- */
    byte_t  shsh[16];
    byte_t  dhsh[16];

    file = file_raw_openA(name, CR_FO_RO | CR_FO_SEQ);
    if (file == NULL)
        return (NULL);

    temp = file_raw_size(file);
    if (temp == CR_F_ERROR)
        goto _failure1;

    if (temp <= sizeof(shsh) || temp > ((leng_t)-1) / 2)
        goto _failure1;
    temp -= sizeof(shsh);

    data = mem_malloc((leng_t)temp);
    if (data == NULL)
        goto _failure1;

    back = file_raw_read(data, (leng_t)temp, file);
    if (back != temp)
        goto _failure2;

    if (size != NULL)
        *size = back;
    hash_md5_total(dhsh, data, back);

    back = file_raw_read(shsh, sizeof(shsh), file);
    if (back != sizeof(shsh))
        goto _failure2;

    if (mem_cmp(dhsh, shsh, sizeof(shsh)) != 0)
        goto _failure2;

    file_raw_close(file);
    return (data);

_failure2:
    mem_free(data);
_failure1:
    file_raw_close(file);
    return (NULL);
}

/*
=======================================
    生成一个文件 (带 MD5)
=======================================
*/
CR_API bool_t
file_save_md5 (
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    fraw_t  file;
    leng_t  back;
    byte_t  hash[16];

    file = file_raw_openA(name, CR_FO_WO | CR_FO_SEQ);
    if (file == NULL)
        return (FALSE);

    back = file_raw_write(data, size, file);
    if (back != size)
        goto _failure;

    hash_md5_total(hash, data, back);
    back = file_raw_write(hash, sizeof(hash), file);
    if (back != sizeof(hash))
        goto _failure;

    file_raw_close(file);
    return (TRUE);

_failure:
    file_raw_close(file);
    file_deleteA(name);
    return (FALSE);
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
