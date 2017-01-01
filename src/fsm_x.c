/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-12-30  */
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
/*  >>>>>>>>>>>>>>>>>>>> CrHack 名称型有限状态机函数库 <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "hash.h"
#include "datlib.h"
#include "msclib.h"
#include "strlib.h"

/* 名称型状态机 */
typedef struct
{
        sCURBEAD        lists;  /* 状态列表 */
        sFSM_UNIT_X*    crrnt;  /* 当前状态 */

} sFSM_X;

/*
---------------------------------------
    成员比较回调
---------------------------------------
*/
static bool_t
function_comp (
  __CR_IN__ const void_t*   key,
  __CR_IN__ const void_t*   obj
    )
{
    sFSM_UNIT_X**   unit = (sFSM_UNIT_X**)obj;

    return (str_cmpA(unit[0]->name, (ansi_t*)key) == 0);
}

/*
---------------------------------------
    成员索引回调
---------------------------------------
*/
static uint_t
function_find (
  __CR_IN__ const void_t*   key
    )
{
    return (hash_str_djb2(key, str_lenA((ansi_t*)key)));
}

/*
=======================================
    状态机启动
=======================================
*/
CR_API xfsm_t
fsm_x_start (
  __CR_IN__ xnfsm_t         entry,
  __CR_IN__ leng_t          count,
  __CR_IN__ sFSM_UNIT_X*    state
    )
{
    sFSM_X*         rett;
    sFSM_UNIT_X**   temp;

    rett = struct_new(sFSM_X);
    if (rett == NULL)
        return (NULL);
    struct_zero(rett, sFSM_X);
    if (!curbead_initT(&rett->lists, sFSM_UNIT_X*, count)) {
        mem_free(rett);
        return (NULL);
    }
    rett->lists.find = function_find;
    rett->lists.comp = function_comp;

    /* 注册状态指针到哈希表 */
    for (; count != 0; count--, state++)
    {
        if (curbead_insertT(&rett->lists, sFSM_UNIT_X*,
                            state->name, &state) == NULL)
            goto _failure;
    }

    /* 进入指定的开始状态 */
    temp = curbead_findT(&rett->lists, sFSM_UNIT_X*, entry);
    if (temp == NULL)
        goto _failure;
    rett->crrnt = temp[0];
    return ((xfsm_t)rett);

_failure:
    curbead_freeT(&rett->lists, sFSM_UNIT_X*);
    mem_free(rett);
    return (NULL);
}

/*
=======================================
    状态机释放
=======================================
*/
CR_API void_t
fsm_x_free (
  __CR_IN__ xfsm_t  xfsm
    )
{
    sFSM_X* real;

    real = (sFSM_X*)xfsm;
    curbead_freeT(&real->lists, sFSM_UNIT_X*);
    mem_free((void_t*)real);
}

/*
=======================================
    状态机步进
=======================================
*/
CR_API void_t
fsm_x_sstep (
  __CR_IN__ xfsm_t  xfsm,
  __CR_IN__ void_t* param
    )
{
    bool_t          chnge;
    sFSM_UNIT_X*    crrnt;

    chnge = TRUE;
    crrnt = ((sFSM_X*)xfsm)->crrnt;
    if (crrnt->fsm_handle != NULL)
        chnge = crrnt->fsm_handle(crrnt, param);

    if (chnge &&
        crrnt->fsm_render != NULL)
        crrnt->fsm_render(crrnt, param);
}

/*
=======================================
    状态机跳转
=======================================
*/
CR_API bool_t
fsm_x_sgoto (
  __CR_IN__ xfsm_t  xfsm,
  __CR_IN__ xnfsm_t sname
    )
{
    sFSM_X*         real;
    sFSM_UNIT_X**   temp;

    real = (sFSM_X*)xfsm;
    temp = curbead_findT(&real->lists, sFSM_UNIT_X*, sname);
    if (temp == NULL)
        return (FALSE);
    real->crrnt = temp[0];
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
