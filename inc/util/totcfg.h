/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2011-08-15  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> TOTAL CFG 项目总头文件 <<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_TOTCFG_H__
#define __CR_TOTCFG_H__

#include "../defs.h"

/*****************************************************************************/
/*                              多语言配置文件                               */
/*****************************************************************************/

/* 多语言配置对象类型 */
typedef void_t*     mlang_t;

/* 多语言配置 API 函数 */
CR_API mlang_t          mlang_load (const ansi_t *name,
                                    const wide_t *defs, uint_t cpage);
CR_API void_t           mlang_free (mlang_t xlang);
CR_API const ansi_t*    mlang_get_txtA (mlang_t xlang, const ansi_t *id);
CR_API const wide_t*    mlang_get_txtW (mlang_t xlang, const ansi_t *id);

/*****************************************************************************/
/*                               报警配置文件                                */
/*****************************************************************************/

/* 报警配置对象类型 */
typedef void_t*     alarm_t;

/* 报警配置输出结构 */
typedef struct
{
        int32u      id;     /* 报警编号 */
        leng_t      cnts;   /* 报警序号 */
        ansi_t*     txta;   /* 报警消息A */
        wide_t*     txtw;   /* 报警消息W */
        int32u      attr1;  /* 报警属性 */
        int32u      rv[2];  /* 保留字段 */

} sAL_NODE;

/* 报警属性位值 */
#define AL_REPEAT       (1UL << 0)  /* 是否重复 */
#define AL_ENABLE       (1UL << 1)  /* 是否启用 */
#define AL_IGNORE       (1UL << 2)  /* 可否关闭 */
#define AL_ALTYPE       (3UL << 3)  /* 报警类型 */

/* 报警类型的值 (最大可13位) */
#define AL_ICO_HINT     (1UL << 3)  /* 提示类型 */
#define AL_ICO_WARN     (2UL << 3)  /* 警告类型 */
#define AL_ICO_ERRO     (3UL << 3)  /* 错误类型 */

/*
=======================================
    取报警类型
=======================================
*/
cr_inline uint_t
alarm_get_type (
  __CR_IN__ const sAL_NODE* node
    )
{
    return ((uint_t)(node->attr1 & AL_ALTYPE));
}

/*
=======================================
    是否可重复
=======================================
*/
cr_inline bool_t
alarm_is_repeat (
  __CR_IN__ const sAL_NODE* node
    )
{
    return ((node->attr1 & AL_REPEAT) ? TRUE : FALSE);
}

/*
=======================================
    是否已启用
=======================================
*/
cr_inline bool_t
alarm_is_enable (
  __CR_IN__ const sAL_NODE* node
    )
{
    return ((node->attr1 & AL_ENABLE) ? TRUE : FALSE);
}

/*
=======================================
    是否可关闭
=======================================
*/
cr_inline bool_t
alarm_is_ignore (
  __CR_IN__ const sAL_NODE* node
    )
{
    return ((node->attr1 & AL_IGNORE) ? TRUE : FALSE);
}

/* 报警配置 API 函数 */
CR_API alarm_t      alarm_load (const ansi_t *name, uint_t cpage);
CR_API void_t       alarm_free (alarm_t alert);
CR_API leng_t       alarm_get_count (alarm_t alert);
CR_API sAL_NODE*    alarm_find (alarm_t alert, int32u id);
CR_API bool_t       alarm_get_node (alarm_t alert, leng_t idx,
                                    sAL_NODE *node);
CR_API bool_t       alarm_set_node (alarm_t alert, leng_t idx,
                                    const sAL_NODE *node);
CR_API bool_t       alarm_flags_save (alarm_t alert, const ansi_t *name);
CR_API bool_t       alarm_flags_load (alarm_t alert, const ansi_t *name);

#endif  /* !__CR_TOTCFG_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
