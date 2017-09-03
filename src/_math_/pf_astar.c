/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2016-07-05  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>> CrHack AStar 寻路函数库 <<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "applib.h"
#include "memlib.h"
#include "phylib.h"

/* 坐标转编号 */
#define ASTAR_COORD2NUM(x, y)   (as->iRows * (x) + (y))

/*
=======================================
    AStar 初始化对象
=======================================
*/
CR_API void_t
astar_init (
  __CR_OT__ sASTAR* as,
  __CR_IN__ sint_t  cols,
  __CR_IN__ sint_t  rows
    )
{
    struct_zero(as, sASTAR);
    as->iCols = cols;
    as->iRows = rows;
}

/*
---------------------------------------
    AStar 清除节点
---------------------------------------
*/
static void_t
astar_clear_nodes (
  __CR_IO__ sASTAR* as
    )
{
    asNode* temp;

    while (as->pOpen != NULL) {
        temp = as->pOpen->next;
        mem_free(as->pOpen);
        as->pOpen = temp;
    }
    while (as->pClosed != NULL) {
        temp = as->pClosed->next;
        mem_free(as->pClosed);
        as->pClosed = temp;
    }
}

/*
=======================================
    AStar 释放对象
=======================================
*/
CR_API void_t
astar_free (
  __CR_IO__ sASTAR* as
    )
{
    astar_clear_nodes(as);
}

/*
=======================================
    AStar 复位
=======================================
*/
CR_API void_t
astar_reset (
  __CR_IN__ sASTAR* as
    )
{
    as->pBest = NULL;
}

/*
---------------------------------------
    AStar 节点初始化
---------------------------------------
*/
static void_t
astar_node_init (
  __CR_OT__ asNode* node,
  __CR_IN__ sint_t  x,
  __CR_IN__ sint_t  y
    )
{
    struct_zero(node, asNode);
    node->x = x;  node->y = y;
}

/*
=======================================
    AStar 开始路径搜索
=======================================
*/
CR_API bool_t
astar_step_init (
  __CR_IO__ sASTAR* as,
  __CR_IN__ sint_t  sx,
  __CR_IN__ sint_t  sy,
  __CR_IN__ sint_t  dx,
  __CR_IN__ sint_t  dy
    )
{
    asNode* temp;

    if (as->udHeuristic == NULL ||
        as->udCost == NULL || as->udValid == NULL)
        return (FALSE);
    temp = struct_new(asNode);
    if (temp == NULL)
        return (FALSE);
    astar_node_init(temp, sx, sy);

    as->iSX = sx;
    as->iSY = sy;
    as->iDX = dx;
    as->iDY = dy;
    as->iDNum = ASTAR_COORD2NUM(dx, dy);

    temp->g = 0;
    temp->h = as->udHeuristic(NULL, temp, 0, as->cbData);
    temp->f = temp->g + temp->h;
    temp->number = ASTAR_COORD2NUM(sx, sy);

    astar_clear_nodes(as);
    as->pOpen = temp;
#if defined(_CR_ASTAR_SXS_)
    if (as->udNotifyList != NULL)
        as->udNotifyList(NULL, as->pOpen, ASNL_STARTOPEN, as->ncData);
    if (as->udNotifyChild != NULL)
        as->udNotifyChild(NULL, temp, ASNC_INITIALADD, as->ncData);
#endif
    return (TRUE);
}

/*
---------------------------------------
    AStar 返回最佳节点
---------------------------------------
*/
static asNode*
astar_get_best (
  __CR_IO__ sASTAR* as
    )
{
    asNode* temp1;
    asNode* temp2;

    if (as->pOpen == NULL)
        return (NULL);
    temp1 = as->pOpen;
    temp2 = as->pClosed;
    as->pOpen = temp1->next;
#if defined(_CR_ASTAR_SXS_)
    if (as->udNotifyList != NULL)
        as->udNotifyList(NULL, temp1, ASNL_DELETEOPEN, as->ncData);
#endif
    as->pClosed = temp1;
    as->pClosed->next = temp2;
#if defined(_CR_ASTAR_SXS_)
    if (as->udNotifyList != NULL)
        as->udNotifyList(NULL, as->pClosed, ASNL_ADDCLOSED, as->ncData);
#endif
    return (temp1);
}

/*
---------------------------------------
    AStar 检查列表
---------------------------------------
*/
static asNode*
astar_check_list (
  __CR_IN__ asNode* node,
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
    AStar 把节点加入 Open 列表
---------------------------------------
*/
static void_t
astar_add2open (
  __CR_IO__ sASTAR* as,
  __CR_IN__ asNode* addnode
    )
{
    asNode* temp;
    asNode* prev = NULL;
    asNode* node = as->pOpen;

    if (as->pOpen == NULL) {
        as->pOpen = addnode;
        as->pOpen->next = NULL;
#if defined(_CR_ASTAR_SXS_)
        if (as->udNotifyList != NULL)
            as->udNotifyList(NULL, addnode, ASNL_STARTOPEN, as->ncData);
#endif
        return;
    }

    while (node != NULL) {
        if (addnode->f > node->f) {
            prev = node;
            node = node->next;
        }
        else {
            if (prev != NULL) {
                prev->next = addnode;
                addnode->next = node;
#if defined(_CR_ASTAR_SXS_)
                if (as->udNotifyList != NULL)
                    as->udNotifyList(prev, addnode, ASNL_ADDOPEN, as->ncData);
#endif
                return;
            }
            temp = as->pOpen;
            as->pOpen = addnode;
            as->pOpen->next = temp;
#if defined(_CR_ASTAR_SXS_)
            if (as->udNotifyList != NULL)
                as->udNotifyList(temp, addnode, ASNL_STARTOPEN, as->ncData);
#endif
            return;
        }
    }

    prev->next = addnode;
#if defined(_CR_ASTAR_SXS_)
    if (as->udNotifyList != NULL)
        as->udNotifyList(prev, addnode, ASNL_ADDOPEN, as->ncData);
#endif
}

/*
---------------------------------------
    AStar 压入节点
---------------------------------------
*/
static void_t
astar_push (
  __CR_IO__ sASTAR* as,
  __CR_IN__ asNode* node
    )
{
    asStack*    temp;

    if (as->pStack == NULL) {
        as->pStack = struct_new(asStack);
        if (as->pStack == NULL)
            msg_stopA("struct_new() failure", "crhack");
        as->pStack->data = node;
        as->pStack->next = NULL;
    }
    else {
        temp = struct_new(asStack);
        if (temp == NULL)
            msg_stopA("struct_new() failure", "crhack");
        temp->data = node;
        temp->next = as->pStack;
        as->pStack = temp;
    }
}

/*
---------------------------------------
    AStar 弹出节点
---------------------------------------
*/
static asNode*
astar_pop (
  __CR_IO__ sASTAR* as
    )
{
    asNode*     data;
    asStack*    temp;

    data = as->pStack->data;
    temp = as->pStack;
    as->pStack = temp->next;
    mem_free(temp);
    return (data);
}

/*
---------------------------------------
    AStar 更新父节点
---------------------------------------
*/
static void_t
astar_update_parents (
  __CR_IO__ sASTAR* as,
  __CR_IN__ asNode* node
    )
{
    asNode  *kid, *parent;
    sint_t  ii, gg = node->g, cc = node->numchildren;

    for (ii = 0; ii < cc; ii++) {
        kid = node->children[ii];
        if (gg + 1 < kid->g) {
            kid->g = gg + 1;
            kid->f = kid->g + kid->h;
            kid->parent = node;
            astar_push(as, kid);
        }
    }

    while (as->pStack != NULL) {
        parent = astar_pop(as);
        cc = parent->numchildren;
        for (ii = 0; ii < cc; ii++) {
            kid = parent->children[ii];
            if (parent->g + 1 < kid->g) {
                kid->g = parent->g + as->udCost(parent, kid, 0, as->cbData);
                kid->f = kid->g + kid->h;
                kid->parent = parent;
                astar_push(as, kid);
            }
        }
    }
}

/*
---------------------------------------
    AStar 连接子节点
---------------------------------------
*/
static void_t
astar_link_child (
  __CR_IO__ sASTAR* as,
  __CR_IN__ asNode* node,
  __CR_IN__ asNode* temp
    )
{
    sint_t  xx = temp->x;
    sint_t  yy = temp->y;
    sint_t  gg = node->g + as->udCost(node, temp, 0, as->cbData);
    sint_t  num = ASTAR_COORD2NUM(xx, yy);
    asNode  *check, *newnode;

    if ((check = astar_check_list(as->pOpen, num)) != NULL) {
        node->children[node->numchildren++] = check;
        if (gg < check->g) {
            check->parent = node;
            check->g = gg;
            check->f = gg + check->h;
#if defined(_CR_ASTAR_SXS_)
            if (as->udNotifyChild != NULL)
                as->udNotifyChild(node, check, ASNC_OPENADD_UP, as->ncData);
#endif
        }
#if defined(_CR_ASTAR_SXS_)
        else {
            if (as->udNotifyChild != NULL)
                as->udNotifyChild(node, check, ASNC_OPENADD, as->ncData);
        }
#endif
    }
    else
    if ((check = astar_check_list(as->pClosed, num)) != NULL) {
        node->children[node->numchildren++] = check;
        if (gg < check->g) {
            check->parent = node;
            check->g = gg;
            check->f = gg + check->h;
#if defined(_CR_ASTAR_SXS_)
            if (as->udNotifyChild != NULL)
                as->udNotifyChild(node, check, ASNC_CLOSEDADD_UP, as->ncData);
#endif
            astar_update_parents(as, check);
        }
#if defined(_CR_ASTAR_SXS_)
        else {
            if (as->udNotifyChild != NULL)
                as->udNotifyChild(node, check, ASNC_CLOSEDADD, as->ncData);
        }
#endif
    }
    else {
        newnode = struct_new(asNode);
        if (newnode == NULL)
            msg_stopA("struct_new() failure", "crhack");
        astar_node_init(newnode, xx, yy);
        newnode->parent = node;
        newnode->g = gg;
        newnode->h = as->udHeuristic(node, newnode, 0, as->cbData);
        newnode->f = newnode->g + newnode->h;
        newnode->number = num;
        astar_add2open(as, newnode);
        node->children[node->numchildren++] = newnode;
#if defined(_CR_ASTAR_SXS_)
        if (as->udNotifyChild != NULL)
            as->udNotifyChild(node, newnode, ASNC_NEWADD, as->ncData);
#endif
    }
}

/*
---------------------------------------
    AStar 生成子节点
---------------------------------------
*/
static void_t
astar_create_children (
  __CR_IO__ sASTAR* as,
  __CR_IN__ asNode* node
    )
{
    asNode  temp;
    sint_t  ii, jj;
    sint_t  xx = node->x;
    sint_t  yy = node->y;

    astar_node_init(&temp, -1, -1);
    for (ii = -1; ii < 2; ii++) {
        for (jj = -1; jj < 2; jj++) {
            if (ii == 0 && jj == 0)
                continue;
            temp.x = xx + ii;
            temp.y = yy + jj;
            if (temp.x < 0 || temp.x >= as->iCols ||
                temp.y < 0 || temp.y >= as->iRows)
                continue;
            if (!as->udValid(node, &temp, 0, as->cbData))
                continue;
            astar_link_child(as, node, &temp);
        }
    }
}

/*
=======================================
    AStar 路径搜索下一步
=======================================
*/
CR_API sint_t
astar_step_next (
  __CR_IO__ sASTAR* as
    )
{
    as->pBest = astar_get_best(as);
    if (as->pBest == NULL)
        return (-1);
    if (as->pBest->number == as->iDNum)
        return (1);
    astar_create_children(as, as->pBest);
    return (0);
}

/*
=======================================
    AStar 路径搜索
=======================================
*/
CR_API bool_t
astar_find_path (
  __CR_IO__ sASTAR* as,
  __CR_IN__ sint_t  sx,
  __CR_IN__ sint_t  sy,
  __CR_IN__ sint_t  dx,
  __CR_IN__ sint_t  dy
    )
{
    sint_t  retc;

    if (!astar_step_init(as, sx, sy, dx, dy))
        return (FALSE);
    for (retc = 0; retc == 0; retc = astar_step_next(as));
    if (retc < 0 || as->pBest == NULL) {
        as->pBest = NULL;
        return (FALSE);
    }
    return (TRUE);
}

/*
=======================================
    AStar 返回路径 (终点2起点)
=======================================
*/
CR_API sPNT2*
astar_get_path (
  __CR_IN__ const sASTAR*   as,
  __CR_IN__ leng_t*         count
    )
{
    sPNT2*  list;
    leng_t  cnts;
    asNode* best;

    if (as->pBest == NULL)
        return (NULL);
    cnts = 0;
    best = as->pBest;
    do {
        cnts += 1;
        best = best->parent;
    } while (best != NULL);

    list = mem_talloc(cnts, sPNT2);
    if (list == NULL)
        return (NULL);

    cnts = 0;
    best = as->pBest;
    do {
        list[cnts].x = best->x;
        list[cnts].y = best->y;
        cnts += 1;
        best = best->parent;
    } while (best != NULL);

    if (count != NULL)
        *count = cnts;
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
