/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2017-09-09  */
/*     #######          ###    ###      [MATH]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>>>>>> CrHack Dijkstra 寻路函数库 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "applib.h"
#include "phylib.h"

/*
=======================================
    Dijkstra 初始化对象
=======================================
*/
CR_API void_t
dijkstra_init (
  __CR_OT__ sDIJKSTRA*  dj
    )
{
    struct_zero(dj, sDIJKSTRA);
}

/*
---------------------------------------
    Dijkstra 节点释放
---------------------------------------
*/
static void_t
dijkstra_node_free (
  __CR_IN__ djNode* node
    )
{
    array_freeT(&node->children, djNode*);
    mem_free(node);
}

/*
---------------------------------------
    Dijkstra 清除节点
---------------------------------------
*/
static void_t
dijkstra_clear_nodes (
  __CR_IO__ sDIJKSTRA*  dj
    )
{
    djNode* temp;

    while (dj->pOpen != NULL) {
        temp = dj->pOpen->next;
        dijkstra_node_free(dj->pOpen);
        dj->pOpen = temp;
    }
    while (dj->pClosed != NULL) {
        temp = dj->pClosed->next;
        dijkstra_node_free(dj->pClosed);
        dj->pClosed = temp;
    }
}

/*
=======================================
    Dijkstra 释放对象
=======================================
*/
CR_API void_t
dijkstra_free (
  __CR_IO__ sDIJKSTRA*  dj
    )
{
    dijkstra_clear_nodes(dj);
}

/*
=======================================
    Dijkstra 复位
=======================================
*/
CR_API void_t
dijkstra_reset (
  __CR_IN__ sDIJKSTRA*  dj
    )
{
    dj->pBest = NULL;
}

/*
---------------------------------------
    Dijkstra 节点初始化
---------------------------------------
*/
static void_t
dijkstra_node_init (
  __CR_OT__ djNode* node
    )
{
    struct_zero(node, djNode);
}

/*
=======================================
    Dijkstra 开始路径搜索
=======================================
*/
CR_API bool_t
dijkstra_step_init (
  __CR_IO__ sDIJKSTRA*  dj,
  __CR_IN__ sint_t      beg,
  __CR_IN__ sint_t      end
    )
{
    djNode* temp;

    if (dj->udCost == NULL || dj->udValid == NULL)
        return (FALSE);
    temp = struct_new(djNode);
    if (temp == NULL)
        return (FALSE);
    dijkstra_node_init(temp);

    dj->iDNum = end;

    temp->g = 0;
    temp->number = beg;

    dijkstra_clear_nodes(dj);
    dj->pOpen = temp;
#if defined(_CR_DIJKSTRA_SXS_)
    if (dj->udNotifyList != NULL)
        dj->udNotifyList(NULL, dj->pOpen, DJNL_STARTOPEN, dj->ncData);
    if (dj->udNotifyChild != NULL)
        dj->udNotifyChild(NULL, temp, DJNC_INITIALADD, dj->ncData);
#endif
    return (TRUE);
}

/*
---------------------------------------
    Dijkstra 返回最佳节点
---------------------------------------
*/
static djNode*
dijkstra_get_best (
  __CR_IO__ sDIJKSTRA*  dj
    )
{
    djNode* temp1;
    djNode* temp2;

    if (dj->pOpen == NULL)
        return (NULL);
    temp1 = dj->pOpen;
    temp2 = dj->pClosed;
    dj->pOpen = temp1->next;
#if defined(_CR_DIJKSTRA_SXS_)
    if (dj->udNotifyList != NULL)
        dj->udNotifyList(NULL, temp1, DJNL_DELETEOPEN, dj->ncData);
#endif
    dj->pClosed = temp1;
    dj->pClosed->next = temp2;
#if defined(_CR_DIJKSTRA_SXS_)
    if (dj->udNotifyList != NULL)
        dj->udNotifyList(NULL, dj->pClosed, DJNL_ADDCLOSED, dj->ncData);
#endif
    return (temp1);
}

/*
---------------------------------------
    Dijkstra 检查列表
---------------------------------------
*/
static djNode*
dijkstra_check_list (
  __CR_IN__ djNode* node,
  __CR_IN__ sint_t  num
    )
{
    while (node != NULL) {
        if (node->number == num)
            return (node);
        node = node->next;
    }
    return (NULL);
}

/*
---------------------------------------
    Dijkstra 把节点加入 Open 列表
---------------------------------------
*/
static void_t
dijkstra_add2open (
  __CR_IO__ sDIJKSTRA*  dj,
  __CR_IN__ djNode*     addnode
    )
{
    djNode* temp;
    djNode* prev = NULL;
    djNode* node = dj->pOpen;

    if (dj->pOpen == NULL) {
        dj->pOpen = addnode;
        dj->pOpen->next = NULL;
#if defined(_CR_DIJKSTRA_SXS_)
        if (dj->udNotifyList != NULL)
            dj->udNotifyList(NULL, addnode, DJNL_STARTOPEN, dj->ncData);
#endif
        return;
    }

    while (node != NULL) {
        if (addnode->g > node->g) {
            prev = node;
            node = node->next;
        }
        else {
            if (prev != NULL) {
                prev->next = addnode;
                addnode->next = node;
#if defined(_CR_DIJKSTRA_SXS_)
                if (dj->udNotifyList != NULL)
                    dj->udNotifyList(prev, addnode, DJNL_ADDOPEN, dj->ncData);
#endif
                return;
            }
            temp = dj->pOpen;
            dj->pOpen = addnode;
            dj->pOpen->next = temp;
#if defined(_CR_DIJKSTRA_SXS_)
            if (dj->udNotifyList != NULL)
                dj->udNotifyList(temp, addnode, DJNL_STARTOPEN, dj->ncData);
#endif
            return;
        }
    }

    prev->next = addnode;
#if defined(_CR_DIJKSTRA_SXS_)
    if (dj->udNotifyList != NULL)
        dj->udNotifyList(prev, addnode, DJNL_ADDOPEN, dj->ncData);
#endif
}

/*
---------------------------------------
    Dijkstra 压入节点
---------------------------------------
*/
static void_t
dijkstra_push (
  __CR_IO__ sDIJKSTRA*  dj,
  __CR_IN__ djNode*     node
    )
{
    djStack*    temp;

    if (dj->pStack == NULL) {
        dj->pStack = struct_new(djStack);
        if (dj->pStack == NULL)
            msg_stopA("struct_new() failure", "crhack");
        dj->pStack->data = node;
        dj->pStack->next = NULL;
    }
    else {
        temp = struct_new(djStack);
        if (temp == NULL)
            msg_stopA("struct_new() failure", "crhack");
        temp->data = node;
        temp->next = dj->pStack;
        dj->pStack = temp;
    }
}

/*
---------------------------------------
    Dijkstra 弹出节点
---------------------------------------
*/
static djNode*
dijkstra_pop (
  __CR_IO__ sDIJKSTRA*  dj
    )
{
    djNode*     data;
    djStack*    temp;

    data = dj->pStack->data;
    temp = dj->pStack;
    dj->pStack = temp->next;
    mem_free(temp);
    return (data);
}

/*
---------------------------------------
    Dijkstra 更新父节点
---------------------------------------
*/
static void_t
dijkstra_update_parents (
  __CR_IO__ sDIJKSTRA*  dj,
  __CR_IN__ djNode*     node
    )
{
    djNode  *kid, *parent;
    sint_t  cost, ii, gg = node->g, cc = node->numchildren;
    djNode  **list = array_get_dataT(&node->children, djNode*);

    for (ii = 0; ii < cc; ii++) {
        kid = list[ii];
        cost = dj->udCost(node, kid, ii, dj->cbData);
        if (gg + cost < kid->g) {
            kid->g = gg + cost;
            kid->parent = node;
            dijkstra_push(dj, kid);
        }
    }

    while (dj->pStack != NULL) {
        parent = dijkstra_pop(dj);
        cc = parent->numchildren;
        list = array_get_dataT(&parent->children, djNode*);
        for (ii = 0; ii < cc; ii++) {
            kid = list[ii];
            cost = dj->udCost(parent, kid, ii, dj->cbData);
            if (parent->g + cost < kid->g) {
                kid->g = parent->g + cost;
                kid->parent = parent;
                dijkstra_push(dj, kid);
            }
        }
    }
}

/*
---------------------------------------
    Dijkstra 子节点添加
---------------------------------------
*/
static void_t
dijkstra_add_child (
  __CR_IN__ djNode* node,
  __CR_IN__ djNode* child
    )
{
    if (array_push_growT(&node->children, djNode*, &child) == NULL)
        msg_stopA("array_push_growT() failure", "crhack");
    node->numchildren++;
}

/*
---------------------------------------
    Dijkstra 连接子节点
---------------------------------------
*/
static void_t
dijkstra_link_child (
  __CR_IO__ sDIJKSTRA*  dj,
  __CR_IN__ djNode*     node,
  __CR_IN__ djNode*     temp,
  __CR_IN__ sint_t      index
    )
{
    djNode  *check, *newnode;
    sint_t  num = temp->number;
    sint_t  gg = node->g + dj->udCost(node, temp, index, dj->cbData);

    if ((check = dijkstra_check_list(dj->pOpen, num)) != NULL) {
        dijkstra_add_child(node, check);
        if (gg < check->g) {
            check->parent = node;
            check->g = gg;
#if defined(_CR_DIJKSTRA_SXS_)
            if (dj->udNotifyChild != NULL)
                dj->udNotifyChild(node, check, DJNC_OPENADD_UP, dj->ncData);
#endif
        }
#if defined(_CR_DIJKSTRA_SXS_)
        else {
            if (dj->udNotifyChild != NULL)
                dj->udNotifyChild(node, check, DJNC_OPENADD, dj->ncData);
        }
#endif
    }
    else
    if ((check = dijkstra_check_list(dj->pClosed, num)) != NULL) {
        dijkstra_add_child(node, check);
        if (gg < check->g) {
            check->parent = node;
            check->g = gg;
#if defined(_CR_DIJKSTRA_SXS_)
            if (dj->udNotifyChild != NULL)
                dj->udNotifyChild(node, check, DJNC_CLOSEDADD_UP, dj->ncData);
#endif
            dijkstra_update_parents(dj, check);
        }
#if defined(_CR_DIJKSTRA_SXS_)
        else {
            if (dj->udNotifyChild != NULL)
                dj->udNotifyChild(node, check, DJNC_CLOSEDADD, dj->ncData);
        }
#endif
    }
    else {
        newnode = struct_new(djNode);
        if (newnode == NULL)
            msg_stopA("struct_new() failure", "crhack");
        dijkstra_node_init(newnode);
        newnode->parent = node;
        newnode->g = gg;
        newnode->number = num;
        dijkstra_add2open(dj, newnode);
        dijkstra_add_child(node, newnode);
#if defined(_CR_DIJKSTRA_SXS_)
        if (dj->udNotifyChild != NULL)
            dj->udNotifyChild(node, newnode, DJNC_NEWADD, dj->ncData);
#endif
    }
}

/*
---------------------------------------
    Dijkstra 生成子节点
---------------------------------------
*/
static void_t
dijkstra_create_children (
  __CR_IO__ sDIJKSTRA*  dj,
  __CR_IN__ djNode*     node
    )
{
    sint_t  idx;
    djNode  temp;

    /* 找到没有节点为止 */
    dijkstra_node_init(&temp);
    for (idx = 0; ; idx++)
    {
        /* 这个回调必须填写 temp.number 成员 */
        if (!dj->udValid(node, &temp, idx, dj->cbData))
            break;
        dijkstra_link_child(dj, node, &temp, idx);
    }
}

/*
=======================================
    Dijkstra 路径搜索下一步
=======================================
*/
CR_API sint_t
dijkstra_step_next (
  __CR_IO__ sDIJKSTRA*  dj
    )
{
    dj->pBest = dijkstra_get_best(dj);
    if (dj->pBest == NULL)
        return (-1);
    if (dj->pBest->number == dj->iDNum)
        return (1);
    dijkstra_create_children(dj, dj->pBest);
    return (0);
}

/*
=======================================
    Dijkstra 路径搜索
=======================================
*/
CR_API bool_t
dijkstra_find_path (
  __CR_IO__ sDIJKSTRA*  dj,
  __CR_IN__ sint_t      beg,
  __CR_IN__ sint_t      end
    )
{
    sint_t  retc;

    if (!dijkstra_step_init(dj, beg, end))
        return (FALSE);
    for (retc = 0; retc == 0; retc = dijkstra_step_next(dj));
    if (retc < 0 || dj->pBest == NULL) {
        dj->pBest = NULL;
        return (FALSE);
    }
    return (TRUE);
}

/*
=======================================
    Dijkstra 返回路径 (终点2起点)
=======================================
*/
CR_API sint_t*
dijkstra_get_path (
  __CR_IN__ const sDIJKSTRA*    dj,
  __CR_IN__ leng_t*             count
    )
{
    leng_t  cnts;
    sint_t* list;
    djNode* best;

    if (count != NULL)
        *count = 0;
    if (dj->pBest == NULL)
        return (NULL);
    cnts = 0;
    best = dj->pBest;
    do {
        cnts += 1;
        best = best->parent;
    } while (best != NULL);

    if (count != NULL)
        *count = cnts;
    list = mem_talloc(cnts, sint_t);
    if (list == NULL)
        return (NULL);

    cnts = 0;
    best = dj->pBest;
    do {
        list[cnts] = best->number;
        cnts += 1;
        best = best->parent;
    } while (best != NULL);

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
