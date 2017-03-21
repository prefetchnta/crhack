/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-01-06  */
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
/*  >>>>>>>>>>>>>>>>>>>> CrHack 文件读取接口生成函数库 <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "fileio.h"
#include "memlib.h"

/*
=======================================
    可选择生成文件读取接口A
=======================================
*/
CR_API iDATIN*
create_file_inA (
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ msize_t         param
    )
{
    leng_t  size;
    void_t* data;
    iDATIN* datin;
    fsize_t fsize;

    if (param != 0)
    {
        /* 指定为内存接口 */
        if (param == (msize_t)-1)
        {
            data = file_load_as_binA(name, &size);
            if (data == NULL)
                return (NULL);

            datin = create_buff_in(data, size, TRUE);
            if (datin == NULL)
                mem_free(data);
            return (datin);
        }

        /* 根据内存和文件大小选择接口 */
        fsize = file_sizeA(name);
        if (fsize == CR_F_ERROR)
            return (NULL);

        if (fsize <= param / 2)
        {
#ifdef  _CR_NO_VPAGE_
            /* 不支持页面接口使用内存接口 */
            data = file_load_as_binA(name, &size);
            if (data != NULL)
            {
                datin = create_buff_in(data, size, TRUE);
#else
            datin = create_page_inA(name);
#endif
                if (datin != NULL)
                    return (datin);
#ifdef  _CR_NO_VPAGE_
                mem_free(data);
            }
#endif
        }
    }

    /* 使用磁盘接口 */
    return (create_disk_inA(name));
}

#if !defined(_CR_NAKED_NO_WSTRING_)

/*
=======================================
    可选择生成文件读取接口W
=======================================
*/
CR_API iDATIN*
create_file_inW (
  __CR_IN__ const wide_t*   name,
  __CR_IN__ msize_t         param
    )
{
    leng_t  size;
    void_t* data;
    iDATIN* datin;
    fsize_t fsize;

    if (param != 0)
    {
        /* 指定为内存接口 */
        if (param == (msize_t)-1)
        {
            data = file_load_as_binW(name, &size);
            if (data == NULL)
                return (NULL);

            datin = create_buff_in(data, size, TRUE);
            if (datin == NULL)
                mem_free(data);
            return (datin);
        }

        /* 根据内存和文件大小选择接口 */
        fsize = file_sizeW(name);
        if (fsize == CR_F_ERROR)
            return (NULL);

        if (fsize <= param / 2)
        {
#ifdef  _CR_NO_VPAGE_
            /* 不支持页面接口使用内存接口 */
            data = file_load_as_binW(name, &size);
            if (data != NULL)
            {
                datin = create_buff_in(data, size, TRUE);
#else
            datin = create_page_inW(name);
#endif
                if (datin != NULL)
                    return (datin);
#ifdef  _CR_NO_VPAGE_
                mem_free(data);
            }
#endif
        }
    }

    /* 使用磁盘接口 */
    return (create_disk_inW(name));
}

#endif  /* !_CR_NAKED_NO_WSTRING_ */

/*
=======================================
    通用的文件加载
=======================================
*/
CR_API iDATIN*
create_file_inX (
  __CR_IN__ const sLOADER*  loader
    )
{
    iDATIN* datin;

    switch (loader->type)
    {
        case CR_LDR_ANSI:
            datin = create_file_inA(loader->name.ansi, loader->smem);
            break;

#if !defined(_CR_NAKED_NO_WSTRING_)
        case CR_LDR_WIDE:
            datin = create_file_inW(loader->name.wide, loader->smem);
            break;
#endif  /* !_CR_NAKED_NO_WSTRING_ */

        case CR_LDR_BUFF:
            datin = create_buff_in(loader->buff.data, loader->buff.size,FALSE);
            break;

        default:
            return (NULL);
    }
    if (datin == NULL)
        return (NULL);

    /* 设置文件的逻辑区段 */
    if (!CR_VCALL(datin)->setArea(datin, loader->head, loader->tail)) {
        CR_VCALL(datin)->release(datin);
        return (NULL);
    }
    return (datin);
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
