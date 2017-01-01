/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-10-04  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>> CrHack 树形数据表函数库 <<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "safe.h"
#include "datlib.h"

/*
---------------------------------------
    分配一个节点
---------------------------------------
*/
static sATR_UNIT*
atree_node_alloc (
  __CR_IN__ leng_t  unit,
  __CR_IN__ leng_t  count
    )
{
    leng_t      size;
    sATR_UNIT*  node;

    if (cut_addu(&size, unit, sizeof(sATR_UNIT)))
        return (NULL);
    node = (sATR_UNIT*)mem_malloc(size);
    if (node == NULL)
        return (NULL);

    /* 预先分配的子节点个数 */
    mem_zero(node, size);
    if (count != 0) {
        if (!array_reserveT(&node->next, sATR_UNIT*, count)) {
            mem_free(node);
            return (NULL);
        }
        node->next.__cnts__ = count;
        mem_tzero(node->next.__buff__, count, sATR_UNIT*);
    }
    return ((sATR_UNIT*)node);
}

/*
=======================================
    构造函数
=======================================
*/
CR_API bool_t
atree_init (
  __CR_OT__ sATREE* that,
  __CR_IN__ leng_t  unit,
  __CR_IN__ leng_t  count
    )
{
    sATR_UNIT*  root;

    /* 必须生成根节点 */
    struct_zero(that, sATREE);
    root = atree_node_alloc(unit, count);
    if (root == NULL)
        return (FALSE);
    that->__root__ = root;
    that->__chld__ = count;
    return (TRUE);
}

/*
=======================================
    析构函数
=======================================
*/
CR_API void_t
atree_free (
  __CR_IO__ sATREE* that
    )
{
    if (that->__root__ != NULL)
        atree_delete(that, that->__root__);
    struct_zero(that, sATREE);
}

/*
=======================================
    添加一个节点的子节点
=======================================
*/
CR_API sATR_UNIT*
atree_append (
  __CR_IN__ const sATREE*   that,
  __CR_IN__ leng_t          unit,
  __CR_IO__ sATR_UNIT*      node,
  __CR_IN__ const void_t*   data
    )
{
    leng_t      npos;
    sATR_UNIT*  nnew = NULL;

    /* 增加此节点的分枝数 */
    npos = node->next.__cnts__;
    if (array_push_growT(&node->next, sATR_UNIT*, &nnew) == NULL)
        return (NULL);

    /* 插入这个新的节点到新分枝 */
    nnew = atree_insert(that, unit, node, data, npos);
    if (nnew == NULL)
    {
        /* 回滚分枝数 */
        node->next.__cnts__--;
    }
    return ((sATR_UNIT*)nnew);
}

/*
=======================================
    插入一个节点的子节点
=======================================
*/
CR_API sATR_UNIT*
atree_insert (
  __CR_IN__ const sATREE*   that,
  __CR_IN__ leng_t          unit,
  __CR_IO__ sATR_UNIT*      node,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          index
    )
{
    sATR_UNIT*  nnew;
    sATR_UNIT** next;

    /* 只能在已有的空节点处插入 */
    if (index >= node->next.__cnts__)
        return (NULL);
    next = array_get_unitT(&node->next, sATR_UNIT*, index);
    if (next[0] != NULL)
        return (NULL);

    /* 生成一个下级节点 */
    nnew = atree_node_alloc(unit, that->__chld__);
    if (nnew == NULL)
        return (NULL);
    next[0] = nnew;
    nnew->prev = node;
    nnew->npos = index;
    mem_cpy(&nnew[1], data, unit);
    return (nnew);
}

/*
=======================================
    删除一个节点及其子节点
=======================================
*/
CR_API void_t
atree_delete (
  __CR_IN__ const sATREE*   that,
  __CR_IN__ sATR_UNIT*      node
    )
{
    leng_t      idx;
    sATR_UNIT*  prev;
    sATR_UNIT** next;

    /* 递归删除子节点 */
    idx = node->next.__cnts__;
    while (idx-- != 0) {
        next = array_get_unitT(&node->next, sATR_UNIT*, idx);
        if (next[0] != NULL)
            atree_delete(that, next[0]);
    }

    /* 释放当前节点数据 */
    array_freeT(&node->next, sATR_UNIT*);
    if (that->free != NULL)
        that->free(atree_get_data(node));

    /* 处理上层节点的指针指向 */
    if (node->prev != NULL) {
        prev = node->prev;
        next = array_get_unitT(&prev->next, sATR_UNIT*, node->npos);
        next[0] = NULL;
    }
    mem_free(node);
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
