/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-09-06  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 双向链表函数库 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "safe.h"
#include "datlib.h"

/*
---------------------------------------
    分配一个节点
---------------------------------------
*/
static sLST_UNIT*
slist_node_alloc (
  __CR_IN__ leng_t  unit
    )
{
    leng_t  size;

    if (cut_addu(&size, unit, sizeof(sLST_UNIT)))
        return (NULL);
    return ((sLST_UNIT*)mem_malloc(size));
}

/*
=======================================
    构造函数
=======================================
*/
CR_API void_t
slist_init (
  __CR_OT__ sLIST*  that
    )
{
    struct_zero(that, sLIST);
}

/*
=======================================
    析构函数
=======================================
*/
CR_API void_t
slist_free (
  __CR_IO__ sLIST*  that
    )
{
    sLST_UNIT*  prev;
    sLST_UNIT*  node;

    if (that->__size__ != 0)
    {
        node = that->__tail__;
        do
        {
            prev = node->prev;
            if (that->free != NULL)
                that->free(slist_get_data(node));
            mem_free(node);
            node = prev;
        } while (node != NULL);
    }
    struct_zero(that, sLIST);
}

/*
=======================================
    交换指定节点和上一个节点
=======================================
*/
CR_API sLST_UNIT*
slist_swap (
  __CR_IO__ sLIST*      that,
  __CR_IO__ sLST_UNIT*  node
    )
{
    sLST_UNIT*  prev;

    if (that->__size__ <= 1 ||
        that->__head__ == node)
        return (NULL);
    prev = node->prev;
    if (prev->prev != NULL)
        prev->prev->next = node;
    else
        that->__head__ = node;
    if (node->next != NULL)
        node->next->prev = prev;
    else
        that->__tail__ = prev;
    node->prev = prev->prev;
    prev->prev = node;
    prev->next = node->next;
    node->next = prev;
    return (prev);
}

/*
=======================================
    删除指定节点
=======================================
*/
CR_API void_t
slist_delete (
  __CR_IO__ sLIST*      that,
  __CR_IN__ sLST_UNIT*  node
    )
{
    sLST_UNIT*  prev;
    sLST_UNIT*  next;

    if (that->__size__ == 0)
        return;
    prev = node->prev;
    next = node->next;
    if (prev != NULL)
        prev->next = next;
    else
        that->__head__ = next;
    if (next != NULL)
        next->prev = prev;
    else
        that->__tail__ = prev;

    if (that->free != NULL)
        that->free(slist_get_data(node));
    mem_free(node);
    that->__size__--;
}

/*
=======================================
    在尾部追加一个新节点
=======================================
*/
CR_API sLST_UNIT*
slist_append (
  __CR_IO__ sLIST*          that,
  __CR_IN__ leng_t          unit,
  __CR_IN__ const void_t*   data
    )
{
    sLST_UNIT*  tail;

    /* 分配节点 */
    tail = slist_node_alloc(unit);
    if (tail == NULL)
        return (NULL);
    that->__size__++;
    tail->next = NULL;
    tail->prev = that->__tail__;
    if (that->__tail__ != NULL)
        that->__tail__->next = tail;
    else
        that->__head__ = tail;
    that->__tail__ = tail;
    mem_cpy(&tail[1], data, unit);
    return (tail);
}

/*
=======================================
    在头部插入一个新节点
=======================================
*/
CR_API sLST_UNIT*
slist_sthead (
  __CR_IO__ sLIST*          that,
  __CR_IN__ leng_t          unit,
  __CR_IN__ const void_t*   data
    )
{
    sLST_UNIT*  head;

    /* 分配节点 */
    head = slist_node_alloc(unit);
    if (head == NULL)
        return (NULL);
    that->__size__++;
    head->prev = NULL;
    head->next = that->__head__;
    if (that->__head__ != NULL)
        that->__head__->prev = head;
    else
        that->__tail__ = head;
    that->__head__ = head;
    mem_cpy(&head[1], data, unit);
    return (head);
}

/*
=======================================
    插入一个新节点
=======================================
*/
CR_API sLST_UNIT*
slist_insert (
  __CR_IO__ sLIST*          that,
  __CR_IN__ leng_t          unit,
  __CR_IO__ sLST_UNIT*      node,
  __CR_IN__ const void_t*   data
    )
{
    sLST_UNIT*  nnew;

    /* 分配节点 */
    nnew = slist_node_alloc(unit);
    if (nnew == NULL)
        return (NULL);
    that->__size__++;
    if (node == that->__head__) {
        nnew->prev = NULL;
        nnew->next = that->__head__;
        if (that->__head__ != NULL)
            that->__head__->prev = nnew;
        else
            that->__tail__ = nnew;
        that->__head__ = nnew;
    }
    else {
        nnew->next = node;
        nnew->prev = node->prev;
        node->prev->next = nnew;
        node->prev = nnew;
    }
    mem_cpy(&nnew[1], data, unit);
    return (nnew);
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
