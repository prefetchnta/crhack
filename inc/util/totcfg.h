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

#include "../parser.h"

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


/*****************************************************************************/
/*                             结构类型配置文件                              */
/*****************************************************************************/

/* 结构字段结构 */
typedef struct
{
        leng_t          offs;   /* 字段的偏移 */
        leng_t          idxs;   /* 数组的起始 */
        leng_t          cnts;   /* 数组的个数 */
        leng_t          unit;   /* 单元的大小 */
        leng_t          size;   /* 字段的大小 */
        bool_t          skip;   /* 忽略的成员 */
        bool_t          hash;   /* 是否为校验 */
        uint_t          attr;   /* 附加的属性 */
        const wide_t*   name;   /* 字段的名称 */
        const wide_t*   type;   /* 字段类型名 */
        const ansi_t*   show;   /* 字段的名称 */

} sSRZ_MEMB;

/* 校验出错结构 */
typedef struct
{
        leng_t          err_idxs;   /* 出错的索引值 */
        const wide_t*   err_type;   /* 出错的类型名 */
        const wide_t*   err_memb;   /* 出错的成员名 */

} sSRZ_HASH;

/* 有用的类型 */
typedef void_t*   stru_t;   /* 序列化对象类型 */
typedef bool_t  (*safe_check_t) (void_t*, leng_t);
typedef bool_t  (*bin_hash_t) (void_t*, leng_t, leng_t);
typedef void_t  (*err_hash_t) (void_t*, const sSRZ_HASH*);
typedef ansi_t* (*bin2str_t) (const void_t*, leng_t, const sSRZ_MEMB*);
typedef bool_t  (*str2bin_t) (void_t*, leng_t, uint_t, leng_t,
                              uint_t, sXNODEw*);
/* 数据类型结构 */
typedef struct
{
        bool_t          real;   /* 是否为原生 */
        bool_t          free;   /* 是否要释放 */
        leng_t          size;   /* 类型的大小 */
        const wide_t*   name;   /* 类型的名称 */
        safe_check_t    safe;   /* 类型的限制 */

        /* 原生类型的相关属性 */
        bin2str_t   bin2str;    /* 二进制转字符串 */
        str2bin_t   str2bin;    /* 字符串转二进制 */

        /* 嵌套类型的相关属性 */
        leng_t              count;  /* 子成员个数 */
        const sSRZ_MEMB*    membs;  /* 子成员列表 */

} sSRZ_TYPE;

/***** 解析组 *****/
CR_API void_t   serial_free (stru_t serial);
CR_API uint_t   serial_codepage (stru_t serial);
CR_API void_t   serial_set_style (stru_t serial, const ansi_t *style);
CR_API stru_t   serial_init (uint_t types_n, uint_t value_n, uint_t safer_n,
                             uint_t codepage, const ansi_t *encoding);
CR_API bool_t   serial_set_type (stru_t serial, const sSRZ_TYPE *real_type);
CR_API bool_t   serial_set_check (stru_t serial, safe_check_t safer,
                                  const wide_t *name);
CR_API bool_t   serial_get_value (stru_t serial, leng_t *value,
                                  const wide_t *name);
CR_API bool_t   serial_set_value (stru_t serial, leng_t value,
                                  const wide_t *name, bool_t is_free);
CR_API bool_t   serial_setup (stru_t serial, const ansi_t *header);
CR_API bool_t   serial_find_type (stru_t serial, sSRZ_TYPE *stru_type,
                                  const wide_t *type);
CR_API bool_t   serial_find_memb (stru_t serial, sSRZ_MEMB *member,
                                  const wide_t *type, const wide_t *path);
/***** 数据组 *****/
CR_API bool_t   serial_hash (stru_t serial, void_t *data, leng_t size,
                             const sSRZ_MEMB *member, bin_hash_t hasher,
                             err_hash_t errors, void_t *param);
CR_API ansi_t*  serial_bin2str (stru_t serial, const void_t *data,
                                leng_t size, const sSRZ_MEMB *member);
CR_API bool_t   serial_str2bin (stru_t serial, void_t *data, leng_t size,
                                const ansi_t *text, const sSRZ_MEMB *member);

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
