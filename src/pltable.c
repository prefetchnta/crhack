/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2011-02-15  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>> CrHack 平面哈希表函数库 <<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "hash.h"
#include "safe.h"
#include "datlib.h"

/*
=======================================
    构造函数
=======================================
*/
CR_API bool_t
pltable_init (
  __CR_OT__ sPLTABLE*   that,
  __CR_IN__ leng_t      unit,
  __CR_IN__ leng_t      count
    )
{
    uchar*  data;
    uchar*  info;
    leng_t  size;

    struct_zero(that, sPLTABLE);

    if ((dist_t)count < 0) {
        count = 0 - count;
    }
    else {
        if (count == 0)
            count = _CR_DEF_HASH_SIZE_;
        else
            count =  hash_count(count);
    }
    if (cut_mul(&size, count, unit))
        return (FALSE);

    data = (uchar*)mem_malloc(size);
    if (data == NULL)
        return (FALSE);
    info = (uchar*)mem_malloc(count);
    if (info == NULL) {
        mem_free(data);
        return (FALSE);
    }
    that->__size__ = (leng_t)count;
    that->__buff__ = (uchar*)mem_zero(data, size);
    that->__info__ = (uchar*)mem_zero(info, count);
    return (TRUE);
}

/*
=======================================
    析构函数
=======================================
*/
CR_API void_t
pltable_free (
  __CR_IO__ sPLTABLE*   that,
  __CR_IN__ leng_t      unit
    )
{
    if (that->__buff__ != NULL) {
        pltable_clear(that, unit);
        mem_free(that->__buff__);
        mem_free(that->__info__);
    }
    struct_zero(that, sPLTABLE);
}

/*
=======================================
    清除哈希表
=======================================
*/
CR_API void_t
pltable_clear (
  __CR_IO__ sPLTABLE*   that,
  __CR_IN__ leng_t      unit
    )
{
    leng_t  idx;
    void_t* node;

    if (that->free != NULL) {
        idx = that->__size__;
        while (idx-- != 0) {
            node = pltable_get_unit(that, unit, idx);
            if (node != NULL)
                that->free(node);
        }
    }
    mem_zero(that->__info__, that->__size__);
}

/*
=======================================
    查找哈希表
=======================================
*/
CR_API void_t*
pltable_find (
  __CR_IN__ const sPLTABLE* that,
  __CR_IN__ leng_t          unit,
  __CR_IN__ const void_t*   key
    )
{
    leng_t  idx;
    leng_t  beg;
    void_t* data;

    if (that->find == NULL ||
        that->comp == NULL)
        return (NULL);

    /* 计算起始索引 */
    beg = that->find(key) % that->__size__;

    for (idx = 0; idx < that->__size__; idx++)
    {
        if (that->__info__[beg] != CR_PLTABLE_SKIP) {
            data = pltable_get_unit(that, unit, beg);
            if (data == NULL)
                break;
            if (that->comp(key, data))
                return (data);
        }
        if (++beg >= that->__size__)
            beg = 0;
    }
    return (NULL);
}

/*
=======================================
    删除表成员
=======================================
*/
CR_API bool_t
pltable_delete (
  __CR_IN__ const sPLTABLE* that,
  __CR_IN__ leng_t          unit,
  __CR_IN__ const void_t*   key
    )
{
    leng_t  idx;
    leng_t  beg;
    void_t* data;

    if (that->find == NULL ||
        that->comp == NULL)
        return (FALSE);

    /* 计算起始索引 */
    beg = that->find(key) % that->__size__;

    for (idx = 0; idx < that->__size__; idx++)
    {
        if (that->__info__[beg] != CR_PLTABLE_SKIP) {
            data = pltable_get_unit(that, unit, beg);
            if (data == NULL)
                break;
            if (that->comp(key, data)) {
                if (that->free != NULL)
                    that->free(data);
                that->__info__[beg] = CR_PLTABLE_SKIP;
                return (TRUE);
            }
        }
        if (++beg >= that->__size__)
            beg = 0;
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
pltable_insert (
  __CR_IN__ const sPLTABLE* that,
  __CR_IN__ leng_t          unit,
  __CR_IN__ const void_t*   key,
  __CR_IN__ const void_t*   data
    )
{
    leng_t  idx;
    leng_t  beg;
    void_t* find;

    if (that->find == NULL ||
        that->comp == NULL)
        return (NULL);

    /* 计算起始索引 */
    beg = that->find(key) % that->__size__;

    for (idx = 0; idx < that->__size__; idx++)
    {
        find = pltable_get_unit(that, unit, beg);
        if (find == NULL) {
            that->__info__[beg] = CR_PLTABLE_HAVE;
            find = that->__buff__ + beg * unit;
            return (mem_cpy(find, data, unit));
        }
        if (that->comp(key, find))
        {
            /* 找到目标, 复制数据 */
            if (that->free != NULL)
                that->free(find);
            return (mem_cpy(find, data, unit));
        }
        if (++beg >= that->__size__)
            beg = 0;
    }

    /* 哈希表已经没有空间 */
    return (NULL);
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
