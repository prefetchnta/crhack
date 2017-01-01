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
/*  >>>>>>>>>>>>>>>>>> CrHack 字符串工具函数库转换组模板 <<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

/*
=======================================
    数据转16进制串T
=======================================
*/
CR_API XCHAR*
CR_FAW(hex2str) (
  __CR_OT__ XCHAR*          str,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    byte_t  temp;
    leng_t  idx = 0;

    for (size *= 2; idx < size; idx += 2)
    {
        temp = *(uchar*)data;
        data =  (uchar*)data + 1;
        str[idx + 0] = s_hex2asc[temp >>   4];
        str[idx + 1] = s_hex2asc[temp & 0x0F];
    }
    str[idx] = CR_XN(NIL);
    return (str);
}

/*
=======================================
    16进制串转数据T
=======================================
*/
CR_API void_t*
CR_FAW(str2dat) (
  __CR_OT__ void_t*         data,
  __CR_IO__ leng_t*         size,
  __CR_IN__ const XCHAR*    str
    )
{
    byte_t  val;
    leng_t  length = 0;
    leng_t  limit = (leng_t)-1;
    void_t* dest = data;

    if (size != NULL)
        limit = *size;

    while (*str != CR_XN(NIL))
    {
        if (length >= limit)
            break;
        str = CR_FAW(skip_space)(str);
        if (CR_FAW(is_digit)(*str))
            val = (byte_t)((0x0F & *str++));
        else
        if (CR_FAW(is_xnumb)(*str))
            val = (byte_t)((0x0F & *str++) + 9);
        else
            break;

        if (CR_FAW(is_digit)(*str)) {
            val <<= 4;
            val |= (byte_t)((0x0F & *str++));
        }
        else
        if (CR_FAW(is_xnumb)(*str)) {
            val <<= 4;
            val |= (byte_t)((0x0F & *str++) + 9);
        }
        length++;
        *(byte_t*)dest = val;
        dest = (byte_t*)dest + 1;
    }

    if (size != NULL)
        *size = length;
    return (data);
}

/*
=======================================
    获取一个整数集合T
=======================================
*/
CR_API XCHAR*
CR_FAW(str2lst) (
  __CR_OT__ uint_t*         list,
  __CR_IN__ leng_t          count,
  __CR_IN__ const XCHAR*    str,
  __CR_IN__ const XCHAR     head_tail_block[3]
    )
{
    leng_t  idx, len;

    str = CR_FAW(skip_space)(str);
    if (*str++ != head_tail_block[0])
        return (NULL);

    for (idx = 0; idx < count - 1; idx++)
    {
        str = CR_FAW(skip_space)(str);
        list[idx] = CR_FAW(str2intx)(str, &len);
        if (len == 0)
            return (NULL);

        str = CR_FAW(skip_space)(str + len);
        if (*str++ != head_tail_block[2])
            return (NULL);
    }

    str = CR_FAW(skip_space)(str);
    list[idx] = CR_FAW(str2intx)(str, &len);
    if (len == 0)
        return (NULL);

    str = CR_FAW(skip_space)(str + len);
    if (*str++ != head_tail_block[1])
        return (NULL);
    return ((XCHAR*)str);
}

/*
=======================================
    获取一个整数集合T (不定长)
=======================================
*/
CR_API uint_t*
CR_FAW(str2lst2) (
  __CR_OT__ leng_t*         count,
  __CR_IN__ const XCHAR*    str,
  __CR_IN__ const XCHAR     head_tail_block[3],
  __CR_OT__ leng_t*         skip
    )
{
    sARRAY  list;
    uint_t  value;
    leng_t  length;
    XCHAR*  bak = (XCHAR*)str;

    str = CR_FAW(skip_space)(str);
    if (*str++ != head_tail_block[0])
        return (NULL);
    array_initT(&list, uint_t);

    for (;;) {
        str = CR_FAW(skip_space)(str);
        value = CR_FAW(str2intx)(str, &length);
        if (length == 0)
            goto _failure;
        if (array_push_growT(&list, uint_t, &value) == NULL)
            goto _failure;

        str = CR_FAW(skip_space)(str + length);
        if (*str == head_tail_block[1]) {
            if (skip != NULL)
                *skip = (leng_t)(str + 1 - bak);
            break;
        }
        if (*str++ != head_tail_block[2])
            goto _failure;
    }

    if (!array_no_growT(&list, uint_t))
        goto _failure;
    if (count != NULL)
        *count = list.__cnts__;
    return (array_get_dataT(&list, uint_t));

_failure:
    array_freeT(&list, uint_t);
    return (NULL);
}

/*
=======================================
    获取一个矢量集合T
=======================================
*/
CR_API XCHAR*
CR_FAW(str2vec) (
  __CR_OT__ fp32_t*         list,
  __CR_IN__ leng_t          count,
  __CR_IN__ const XCHAR*    str,
  __CR_IN__ const XCHAR     head_tail_block[3]
    )
{
    leng_t  idx, len;

    str = CR_FAW(skip_space)(str);
    if (*str++ != head_tail_block[0])
        return (NULL);

    for (idx = 0; idx < count - 1; idx++)
    {
        str = CR_FAW(skip_space)(str);
        list[idx] = CR_FAW(str2fp32)(str, &len);
        if (len == 0)
            return (NULL);

        str = CR_FAW(skip_space)(str + len);
        if (*str++ != head_tail_block[2])
            return (NULL);
    }

    str = CR_FAW(skip_space)(str);
    list[idx] = CR_FAW(str2fp32)(str, &len);
    if (len == 0)
        return (NULL);

    str = CR_FAW(skip_space)(str + len);
    if (*str++ != head_tail_block[1])
        return (NULL);
    return ((XCHAR*)str);
}

/*
=======================================
    获取一个矢量集合T (不定长)
=======================================
*/
CR_API fp32_t*
CR_FAW(str2vec2) (
  __CR_OT__ leng_t*         count,
  __CR_IN__ const XCHAR*    str,
  __CR_IN__ const XCHAR     head_tail_block[3],
  __CR_OT__ leng_t*         skip
    )
{
    sARRAY  list;
    fp32_t  value;
    leng_t  length;
    XCHAR*  bak = (XCHAR*)str;

    str = CR_FAW(skip_space)(str);
    if (*str++ != head_tail_block[0])
        return (NULL);
    array_initT(&list, fp32_t);

    for (;;) {
        str = CR_FAW(skip_space)(str);
        value = CR_FAW(str2fp32)(str, &length);
        if (length == 0)
            goto _failure;
        if (array_push_growT(&list, fp32_t, &value) == NULL)
            goto _failure;

        str = CR_FAW(skip_space)(str + length);
        if (*str == head_tail_block[1]) {
            if (skip != NULL)
                *skip = (leng_t)(str + 1 - bak);
            break;
        }
        if (*str++ != head_tail_block[2])
            goto _failure;
    }

    if (!array_no_growT(&list, fp32_t))
        goto _failure;
    if (count != NULL)
        *count = list.__cnts__;
    return (array_get_dataT(&list, fp32_t));

_failure:
    array_freeT(&list, fp32_t);
    return (NULL);
}

#define TYPE_T  32
#define REAL_T  fp32_t
#include "strreal.inl"
#undef  TYPE_T
#undef  REAL_T

#define TYPE_T  64
#define REAL_T  fp64_t
#include "strreal.inl"
#undef  TYPE_T
#undef  REAL_T

#define TYPE_T  16
#define REAL_T  real_t
#include "strreal.inl"
#undef  TYPE_T
#undef  REAL_T

#define TYPE_T  16
#define SINT_T  sint_t
#define UINT_T  uint_t
#include "strintx.inl"
#undef  TYPE_T
#undef  SINT_T
#undef  UINT_T

#define TYPE_T  32
#define SINT_T  int32s
#define UINT_T  int32u
#include "strintx.inl"
#undef  TYPE_T
#undef  SINT_T
#undef  UINT_T

#define TYPE_T  64
#define SINT_T  int64s
#define UINT_T  int64u
#include "strintx.inl"
#undef  TYPE_T
#undef  SINT_T
#undef  UINT_T

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
