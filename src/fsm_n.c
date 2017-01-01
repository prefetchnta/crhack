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
/*  >>>>>>>>>>>>>>>>>>>> CrHack 整数型有限状态机函数库 <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "msclib.h"

/*
=======================================
    状态机启动
=======================================
*/
CR_API bool_t
fsm_n_start (
  __CR_IO__ sFSM_N*         nfsm,
  __CR_IN__ ufast_t         entry,
  __CR_IN__ ufast_t         count,
  __CR_IN__ sFSM_UNIT_N*    state
    )
{
    if (entry >= count)
        return (FALSE);
    nfsm->crrnt = entry;
    nfsm->count = count;
    nfsm->lists = state;
    return (TRUE);
}

/*
=======================================
    状态机步进
=======================================
*/
CR_API void_t
fsm_n_sstep (
  __CR_IO__ sFSM_N* nfsm,
  __CR_IN__ void_t* param
    )
{
    bool_t          chnge;
    sFSM_UNIT_N*    crrnt;

    chnge = TRUE;
    crrnt = &nfsm->lists[nfsm->crrnt];
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
fsm_n_sgoto (
  __CR_IO__ sFSM_N* nfsm,
  __CR_IN__ ufast_t sname
    )
{
    if (sname >= nfsm->count)
        return (FALSE);
    nfsm->crrnt = sname;
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
