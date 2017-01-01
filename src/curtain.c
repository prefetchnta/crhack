/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2009-12-18  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>> CrHack 窗帘哈希表函数库 <<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "hash.h"
#include "datlib.h"

/*
=======================================
    构造函数
=======================================
*/
CR_API bool_t
curtain_init (
  __CR_OT__ sCURTAIN*   that,
  __CR_IN__ leng_t      unit,
  __CR_IN__ leng_t      count,
  __CR_IN__ leng_t      leng
    )
{
    uint_t  idx;

    struct_zero(that, sCURTAIN);

    if ((dist_t)count < 0) {
        count = 0 - count;
        that->__size__ = (uint_t)count;
    }
    else {
        if (count == 0)
            that->__size__ = _CR_DEF_HASH_SIZE_;
        else
            that->__size__ =  hash_count(count);
    }
    that->__list__ = mem_talloc(that->__size__, sARRAY);
    if (that->__list__ == NULL)
        return (FALSE);

    /* 初始化窗帘 */
    mem_tzero(that->__list__, that->__size__, sARRAY);

    if (leng != 0)
    {
        for (idx = 0; idx < that->__size__; idx++)
        {
            /* 保留 leng 个单位的长度 */
            if (!array_reserve(&that->__list__[idx], unit, leng)) {
                curtain_free(that, unit);
                return (FALSE);
            }
        }
    }
    return (TRUE);
}

/*
=======================================
    析构函数
=======================================
*/
CR_API void_t
curtain_free (
  __CR_IO__ sCURTAIN*   that,
  __CR_IN__ leng_t      unit
    )
{
    uint_t  idx;

    if (that->__list__ != NULL)
    {
        /* 释放每条窗帘 */
        for (idx = 0; idx < that->__size__; idx++) {
            that->__list__[idx].free = that->free;
            array_free(&that->__list__[idx], unit);
        }
        mem_free(that->__list__);
    }
    struct_zero(that, sCURTAIN);
}

/*
=======================================
    查找哈希表
=======================================
*/
CR_API void_t*
curtain_find (
  __CR_IN__ const sCURTAIN* that,
  __CR_IN__ leng_t          unit,
  __CR_IN__ const void_t*   key
    )
{
    leng_t  idx;
    void_t* data;
    sARRAY* node;

    if (that->find == NULL ||
        that->comp == NULL)
        return (NULL);

    /* 计算索引位置 */
    node = &that->__list__[that->find(key) % that->__size__];

    for (idx = 0; idx < node->__cnts__; idx++)
    {
        data = array_get_unit(node, unit, idx);
        if (that->comp(key, data))
            return (data);
    }
    return (NULL);
}

/*
=======================================
    删除表成员
=======================================
*/
CR_API bool_t
curtain_delete (
  __CR_IN__ const sCURTAIN* that,
  __CR_IN__ leng_t          unit,
  __CR_IN__ const void_t*   key
    )
{
    leng_t  idx;
    void_t* data;
    sARRAY* node;

    if (that->find == NULL ||
        that->comp == NULL)
        return (FALSE);

    /* 计算索引位置 */
    node = &that->__list__[that->find(key) % that->__size__];

    for (idx = 0; idx < node->__cnts__; idx++)
    {
        data = array_get_unit(node, unit, idx);
        if (that->comp(key, data))
        {
            node->free = that->free;
            array_delete(node, unit, idx);
            return (TRUE);
        }
    }

    /* 没有找到目标 */
    return (FALSE);
}

/*
=======================================
    插入表成员
=======================================
*/
CR_API void_t*
curtain_insert (
  __CR_IN__ const sCURTAIN* that,
  __CR_IN__ leng_t          unit,
  __CR_IN__ const void_t*   key,
  __CR_IN__ const void_t*   data
    )
{
    leng_t  idx;
    void_t* find;
    sARRAY* node;

    if (that->find == NULL)
        return (NULL);

    /* 计算索引位置 */
    node = &that->__list__[that->find(key) % that->__size__];

    /* 支持直接插入 */
    if (that->comp != NULL)
    {
        for (idx = 0; idx < node->__cnts__; idx++)
        {
            find = array_get_unit(node, unit, idx);
            if (that->comp(key, find))
            {
                /* 找到目标, 复制数据 */
                if (that->free != NULL)
                    that->free(find);
                return (mem_cpy(find, data, unit));
            }
        }
    }

    /* 没有找到目标, 新建数据 */
    return (array_push_grow(node, unit, data));
}

/*
=======================================
    返回冲突情况
=======================================
*/
CR_API leng_t*
curtain_check (
  __CR_IN__ const sCURTAIN* that,
  __CR_OT__ uint_t*         size
    )
{
    uint_t  idx;
    leng_t* list;

    list = mem_talloc(that->__size__, leng_t);
    if (list == NULL)
        return (NULL);
    for (idx = 0; idx < that->__size__; idx++)
        list[idx] = array_get_size(&that->__list__[idx]);
    if (size != NULL)
        *size = idx;
    return (list);
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
