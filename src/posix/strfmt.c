/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-01-13  */
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
/*  >>>>>>>>>>>>>>>>>> CrHack 字符串格式化函数库 for LIBC <<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "memlib.h"
#include "strlib.h"

#include <stdio.h>
#include <stdarg.h>

/* VC8 以下 vsnprintf 要加上下划线
   (Windows CE 是用的 VC6 的库?????) */
#if defined(_CR_CC_MSC_)
    #if (_CR_CC_VER_ < 1400) || defined(_CR_OS_WINCE_)
        #define vsnprintf   _vsnprintf
    #endif
#endif

/* 代码复制宏 */
#define STR_VFMT_DO \
    leng_t  idx; \
    leng_t  len; \
    leng_t  size; \
    sint_t  back; \
    ansi_t* temp; \
    va_list args; \
\
    /* 估算初始大小 */ \
    len = str_lenA(format); \
    size = len + 1; \
    for (idx = 0; idx < len; idx++) { \
        if (format[idx + 0] == CR_AC('%') && \
            format[idx + 1] != CR_AC('%')) \
            size += 32; \
    } \
\
    /* 计算最终大小 */ \
    for (;;) { \
        temp = str_allocA(size); \
        if (temp == NULL) \
            return (NULL); \
        va_start(args, format); \
        back = vsnprintf(temp, size, format, args); \
        va_end(args); \
        if (back < 0) { \
            size *= 2; \
        } \
        else { \
            len = (leng_t)back; \
            if (len == size || len == size - 1) \
                size *= 2; \
            else if (len > size) \
                size = len + 2; \
            else \
                break; \
        } \
        mem_free(temp); \
    }
/*
=======================================
    字符串格式编码转换A
=======================================
*/
CR_API ansi_t*
str_fmtA (
  __CR_IN__ const ansi_t*   format,
  __CR_IN__ ...
    )
{
    STR_VFMT_DO

    /* 直接返回字符串 */
    return (temp);
}

/*
=======================================
    字符串格式编码转换U
=======================================
*/
CR_API ansi_t*
str_fmtU (
  __CR_IN__ const ansi_t*   format,
  __CR_IN__ ...
    )
{
    ansi_t* buff;

    STR_VFMT_DO

    /* 转换到 UTF-8 */
    buff = local_to_utf8(CR_LOCAL, temp);
    mem_free(temp);
    return (buff);
}

/*
=======================================
    字符串格式编码转换W
=======================================
*/
CR_API wide_t*
str_fmtW (
  __CR_IN__ const ansi_t*   format,
  __CR_IN__ ...
    )
{
    wide_t* buff;

    STR_VFMT_DO

    /* 转换到 UTF-16 */
    buff = local_to_utf16(CR_LOCAL, temp);
    mem_free(temp);
    return (buff);
}

/*
=======================================
    字符串格式编码转换
=======================================
*/
CR_API void_t*
str_fmtX (
  __CR_IN__ uint_t          codepage,
  __CR_OT__ leng_t*         dst_size,
  __CR_IN__ const ansi_t*   format,
  __CR_IN__ ...
    )
{
    wide_t* wide;
    void_t* buff;

    STR_VFMT_DO

    /* 转换到目标编码 */
    if (codepage == CR_UTF8) {
        buff = str_acp2uni(CR_LOCAL, temp, dst_size, FALSE);
        wide = NULL;
    }
    else
    if (codepage == CR_UTF16X) {
        buff = str_acp2uni(CR_LOCAL, temp, dst_size, TRUE);
        wide = NULL;
    }
    else
    if (codepage != CR_LOCAL) {
        buff = str_acp2uni(CR_LOCAL, temp, dst_size, TRUE);
        wide = (wide_t*)buff;
    }
    else {
        if (dst_size != NULL)
            *dst_size = back + 1;
        return (temp);
    }
    mem_free(temp);
    if (buff == NULL)
        return (NULL);

    /* 再转回指定的多字节编码 */
    if (wide != NULL) {
        buff = str_uni2acp(codepage, wide, dst_size, TRUE);
        mem_free(wide);
    }
    return (buff);
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
