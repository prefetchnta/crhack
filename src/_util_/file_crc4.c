/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2023-07-31  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>>> FILELIB-CRC32 函数库 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "hash.h"
#include "fileio.h"
#include "memlib.h"
#include "util/filelib.h"

/*
=======================================
    把整个文件当字符串读入 (带 CRC32)
=======================================
*/
CR_API ansi_t*
file_load_str_crc4 (
  __CR_IN__ const ansi_t*   name
    )
{
    int32u  shsh;
    int32u  dhsh;
    fraw_t  file;
    leng_t  back;
    ansi_t* text;
    fsize_t size;

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
    dhsh = hash_crc32i_total(text, back);
    mem_zero(&text[back], sizeof(int64u));

    back = file_raw_read(&shsh, sizeof(shsh), file);
    if (back != sizeof(shsh))
        goto _failure2;

    shsh = DWORD_LE(shsh) ^ 0xFFFFFFFFUL;
    if (dhsh != shsh)
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
    读入整个文件 (带 CRC32)
=======================================
*/
CR_API void_t*
file_load_bin_crc4 (
  __CR_IN__ const ansi_t*   name,
  __CR_OT__ leng_t*         size
    )
{
    int32u  shsh;
    int32u  dhsh;
    fraw_t  file;
    leng_t  back;
    void_t* data;
    fsize_t temp;

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
    dhsh = hash_crc32i_total(data, back);

    back = file_raw_read(&shsh, sizeof(shsh), file);
    if (back != sizeof(shsh))
        goto _failure2;

    shsh = DWORD_LE(shsh) ^ 0xFFFFFFFFUL;
    if (dhsh != shsh)
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
    生成一个文件 (带 CRC32)
=======================================
*/
CR_API bool_t
file_save_crc4 (
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    fraw_t  file;
    leng_t  back;
    int32u  hash;

    file = file_raw_openA(name, CR_FO_WO | CR_FO_SEQ);
    if (file == NULL)
        return (FALSE);

    back = file_raw_write(data, size, file);
    if (back != size)
        goto _failure;

    hash = hash_crc32i_total(data, back);
    hash = DWORD_LE(hash) ^ 0xFFFFFFFFUL;
    back = file_raw_write(&hash, sizeof(hash), file);
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
