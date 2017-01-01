/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-09-08  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>> CrHack 珠帘哈希表函数库 <<<<<<<<<<<<<<<<<<<<<<<  */
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
curbead_init (
  __CR_OT__ sCURBEAD*   that,
  __CR_IN__ leng_t      count
    )
{
    struct_zero(that, sCURBEAD);

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
    that->__list__ = mem_talloc(that->__size__, sLIST);
    if (that->__list__ == NULL)
        return (FALSE);

    /* 初始化珠帘 */
    mem_tzero(that->__list__, that->__size__, sLIST);
    return (TRUE);
}

/*
=======================================
    析构函数
=======================================
*/
CR_API void_t
curbead_free (
  __CR_IO__ sCURBEAD*   that
    )
{
    uint_t  idx;

    if (that->__list__ != NULL)
    {
        /* 释放每条珠帘 */
        for (idx = 0; idx < that->__size__; idx++)
        {
            that->__list__[idx].free = that->free;
            slist_free(&that->__list__[idx]);
        }
        mem_free(that->__list__);
    }
    struct_zero(that, sCURBEAD);
}

/*
=======================================
    查找哈希表
=======================================
*/
CR_API void_t*
curbead_find (
  __CR_IN__ const sCURBEAD* that,
  __CR_IN__ const void_t*   key
    )
{
    sLIST*      list;
    leng_t      size;
    void_t*     data;
    sLST_UNIT*  node;

    if (that->find == NULL ||
        that->comp == NULL)
        return (NULL);

    /* 计算索引位置 */
    list = &that->__list__[that->find(key) % that->__size__];
    size =  list->__size__;
    node =  list->__head__;

    for (; size != 0; size--)
    {
        data = slist_get_data(node);
        if (that->comp(key, data))
            return (data);
        node = node->next;
    }
    return (NULL);
}

/*
=======================================
    删除表成员
=======================================
*/
CR_API bool_t
curbead_delete (
  __CR_IN__ const sCURBEAD* that,
  __CR_IN__ const void_t*   key
    )
{
    sLIST*      list;
    leng_t      size;
    void_t*     data;
    sLST_UNIT*  node;

    if (that->find == NULL ||
        that->comp == NULL)
        return (FALSE);

    /* 计算索引位置 */
    list = &that->__list__[that->find(key) % that->__size__];
    size =  list->__size__;
    node =  list->__head__;

    for (; size != 0; size--)
    {
        data = slist_get_data(node);
        if (that->comp(key, data))
        {
            list->free = that->free;
            slist_delete(list, node);
            return (TRUE);
        }
        node = node->next;
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
curbead_insert (
  __CR_IN__ const sCURBEAD* that,
  __CR_IN__ leng_t          unit,
  __CR_IN__ const void_t*   key,
  __CR_IN__ const void_t*   data
    )
{
    sLIST*      list;
    leng_t      size;
    void_t*     find;
    sLST_UNIT*  node;

    if (that->find == NULL)
        return (NULL);

    /* 计算索引位置 */
    list = &that->__list__[that->find(key) % that->__size__];

    /* 支持直接插入 */
    if (that->comp != NULL)
    {
        size = list->__size__;
        node = list->__head__;
        for (; size != 0; size--)
        {
            find = slist_get_data(node);
            if (that->comp(key, find))
            {
                /* 找到目标, 复制数据 */
                if (that->free != NULL)
                    that->free(find);
                return (mem_cpy(find, data, unit));
            }
            node = node->next;
        }
    }

    /* 没有找到目标, 新建数据 */
    node = slist_append(list, unit, data);
    if (node == NULL)
        return (NULL);
    return (slist_get_data(node));
}

/*
=======================================
    返回冲突情况
=======================================
*/
CR_API leng_t*
curbead_check (
  __CR_IN__ const sCURBEAD* that,
  __CR_OT__ uint_t*         size
    )
{
    uint_t  idx;
    leng_t* list;

    list = mem_talloc(that->__size__, leng_t);
    if (list == NULL)
        return (NULL);
    for (idx = 0; idx < that->__size__; idx++)
        list[idx] = slist_get_size(&that->__list__[idx]);
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
