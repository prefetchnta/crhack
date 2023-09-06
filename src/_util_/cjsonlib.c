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
/*  >>>>>>>>>>>>>>>>>>>>>>>>>>> CJSON 助手函数库 <<<<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "memlib.h"
#include "strlib.h"
#include "util/jsonlib.h"

/*
=======================================
    返回 JSON 对象字段
=======================================
*/
CR_API cJSON*
cjson_object (
  __CR_IN__ cJSON*          node,
  __CR_IN__ const ansi_t*   name
    )
{
    cJSON*  obj;

    obj = cJSON_GetObjectItem(node, name);
    if (!cJSON_IsObject(obj))
        return (NULL);
    return (obj);
}

/*
=======================================
    返回 JSON 字符串字段
=======================================
*/
CR_API ansi_t*
cjson_string (
  __CR_IN__ cJSON*          node,
  __CR_IN__ const ansi_t*   name
    )
{
    cJSON*  obj;

    obj = cJSON_GetObjectItem(node, name);
    if (!cJSON_IsString(obj))
        return (NULL);
    return (obj->valuestring);
}

/*
=======================================
    克隆 JSON 字符串字段
=======================================
*/
CR_API ansi_t*
cjson_str_dup (
  __CR_IN__ cJSON*          node,
  __CR_IN__ const ansi_t*   name
    )
{
    cJSON*  obj;

    obj = cJSON_GetObjectItem(node, name);
    if (!cJSON_IsString(obj))
        return (NULL);
    return (str_dupA(obj->valuestring));
}

/*
=======================================
    复制 JSON 字符串字段
=======================================
*/
CR_API bool_t
cjson_str_cpy (
  __CR_OT__ ansi_t*         dest,
  __CR_IN__ size_t          size,
  __CR_IN__ cJSON*          node,
  __CR_IN__ const ansi_t*   name
    )
{
    cJSON*  obj;
    leng_t  len;

    obj = cJSON_GetObjectItem(node, name);
    if (!cJSON_IsString(obj))
        return (FALSE);
    len = str_sizeA(obj->valuestring);
    if (len > size)
        return (FALSE);
    chr_cpyA(dest, obj->valuestring, len);
    return (TRUE);
}

/*
=======================================
    返回 JSON 数组元素个数
=======================================
*/
CR_API bool_t
cjson_array_cnt (
  __CR_IN__ cJSON*          node,
  __CR_IN__ const ansi_t*   name,
  __CR_OT__ uint_t*         count
    )
{
    cJSON*  obj;

    obj = cJSON_GetObjectItem(node, name);
    if (!cJSON_IsArray(obj))
        return (FALSE);
    *count = cJSON_GetArraySize(obj);
    return (TRUE);
}

/*
=======================================
    返回 JSON 数组元素对象
=======================================
*/
CR_API cJSON*
cjson_array_get (
  __CR_IN__ cJSON*  node,
  __CR_IN__ uint_t  index
    )
{
    cJSON*  obj;

    obj = cJSON_GetArrayItem(node, index);
    if (!cJSON_IsObject(obj))
        return (NULL);
    return (obj);
}

/*
=======================================
    获取 JSON 整数字段
=======================================
*/
CR_API bool_t
cjson_int (
  __CR_IN__ cJSON*          node,
  __CR_IN__ const ansi_t*   name,
  __CR_OT__ sint_t*         numb
    )
{
    cJSON*  obj;
    leng_t  skp;

    obj = cJSON_GetObjectItem(node, name);
    if (cJSON_IsNumber(obj)) {
        *numb = (sint_t)obj->valueint;
        return (TRUE);
    }
    if (!cJSON_IsString(obj))
        return (FALSE);
    skp = 0;
    *numb = str2intxA(obj->valuestring, &skp);
    return ((skp == 0) ? FALSE : TRUE);
}

/*
=======================================
    获取 JSON 整数字段 (32)
=======================================
*/
CR_API bool_t
cjson_int32 (
  __CR_IN__ cJSON*          node,
  __CR_IN__ const ansi_t*   name,
  __CR_OT__ int32s*         numb
    )
{
    cJSON*  obj;
    leng_t  skp;

    obj = cJSON_GetObjectItem(node, name);
    if (cJSON_IsNumber(obj)) {
        *numb = (int32s)obj->valueint;
        return (TRUE);
    }
    if (!cJSON_IsString(obj))
        return (FALSE);
    skp = 0;
    *numb = str2intx32A(obj->valuestring, &skp);
    return ((skp == 0) ? FALSE : TRUE);
}

/*
=======================================
    获取 JSON 整数字段 (64)
=======================================
*/
CR_API bool_t
cjson_int64 (
  __CR_IN__ cJSON*          node,
  __CR_IN__ const ansi_t*   name,
  __CR_OT__ int64s*         numb
    )
{
    cJSON*  obj;
    leng_t  skp;

    obj = cJSON_GetObjectItem(node, name);
    if (cJSON_IsNumber(obj)) {
        *numb = (int64s)obj->valueint;
        return (TRUE);
    }
    if (!cJSON_IsString(obj))
        return (FALSE);
    skp = 0;
    *numb = str2intx64A(obj->valuestring, &skp);
    return ((skp == 0) ? FALSE : TRUE);
}

/*
=======================================
    获取 JSON 浮点字段
=======================================
*/
CR_API bool_t
cjson_real (
  __CR_IN__ cJSON*          node,
  __CR_IN__ const ansi_t*   name,
  __CR_OT__ real_t*         numb
    )
{
    cJSON*  obj;
    leng_t  skp;

    obj = cJSON_GetObjectItem(node, name);
    if (cJSON_IsNumber(obj)) {
        *numb = (real_t)obj->valuedouble;
        return (TRUE);
    }
    if (!cJSON_IsString(obj))
        return (FALSE);
    skp = 0;
    *numb = str2realA(obj->valuestring, &skp);
    return ((skp == 0) ? FALSE : TRUE);
}

/*
=======================================
    获取 JSON 浮点字段 (32)
=======================================
*/
CR_API bool_t
cjson_fp32 (
  __CR_IN__ cJSON*          node,
  __CR_IN__ const ansi_t*   name,
  __CR_OT__ fp32_t*         numb
    )
{
    cJSON*  obj;
    leng_t  skp;

    obj = cJSON_GetObjectItem(node, name);
    if (cJSON_IsNumber(obj)) {
        *numb = (fp32_t)obj->valuedouble;
        return (TRUE);
    }
    if (!cJSON_IsString(obj))
        return (FALSE);
    skp = 0;
    *numb = str2fp32A(obj->valuestring, &skp);
    return ((skp == 0) ? FALSE : TRUE);
}

/*
=======================================
    获取 JSON 浮点字段 (64)
=======================================
*/
CR_API bool_t
cjson_fp64 (
  __CR_IN__ cJSON*          node,
  __CR_IN__ const ansi_t*   name,
  __CR_OT__ fp64_t*         numb
    )
{
    cJSON*  obj;
    leng_t  skp;

    obj = cJSON_GetObjectItem(node, name);
    if (cJSON_IsNumber(obj)) {
        *numb = (fp64_t)obj->valuedouble;
        return (TRUE);
    }
    if (!cJSON_IsString(obj))
        return (FALSE);
    skp = 0;
    *numb = str2fp64A(obj->valuestring, &skp);
    return ((skp == 0) ? FALSE : TRUE);
}

/*
=======================================
    获取 JSON 16进制数组字段 (定长)
=======================================
*/
CR_API bool_t
cjson_hex_dat (
  __CR_IN__ cJSON*          node,
  __CR_IN__ const ansi_t*   name,
  __CR_OT__ void_t*         data,
  __CR_IN__ leng_t          size
    )
{
    cJSON*  obj;
    leng_t  len;

    obj = cJSON_GetObjectItem(node, name);
    if (!cJSON_IsString(obj))
        return (FALSE);
    if (str_lenA(obj->valuestring) != size * 2)
        return (FALSE);
    len = size;
    str2datA(data, &len, obj->valuestring);
    return ((len != size) ? FALSE : TRUE);
}

/*
=======================================
    获取 JSON 16进制数组字段 (不定长)
=======================================
*/
CR_API bool_t
cjson_hex_dat2 (
  __CR_IN__ cJSON*          node,
  __CR_IN__ const ansi_t*   name,
  __CR_OT__ void_t*         data,
  __CR_IO__ leng_t*         size
    )
{
    cJSON*  obj;

    obj = cJSON_GetObjectItem(node, name);
    if (!cJSON_IsString(obj))
        return (FALSE);
    if (str_lenA(obj->valuestring) & 1)
        return (FALSE);
    str2datA(data, size, obj->valuestring);
    return (TRUE);
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
