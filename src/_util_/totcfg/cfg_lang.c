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
/*  >>>>>>>>>>>>>>>>>>>> TOTAL CFG 多语言配置文件函数库 <<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "hash.h"
#include "datlib.h"
#include "fileio.h"
#include "parser.h"
#include "strlib.h"
#include "util/totcfg.h"

#if !defined(_CR_NO_WIDE_)

/* 内部使用的结构 */
typedef struct
{
        uint_t          page;   /* 本地语言编码 */
        sCURBEAD        list;   /* 节点查找对象 */
        const ansi_t*   defa;   /* 默认的词条值A */
        const wide_t*   defw;   /* 默认的词条值W */

} sMLANG;

/* 语言词条节点结构 */
typedef struct
{
        const ansi_t*   id;     /* 索引编号 */
        const ansi_t*   txta;   /* 词条文本A */
        const wide_t*   txtw;   /* 词条文本W */

} sML_NODE;

/*
---------------------------------------
    成员释放回调
---------------------------------------
*/
static void_t
node_free (
  __CR_IN__ void_t* obj
    )
{
    sML_NODE*   unit;

    unit = (sML_NODE*)obj;
    TRY_FREE(unit->txta);
    mem_free(unit->txtw);
    mem_free(unit->id);
}

/*
---------------------------------------
    成员比较回调
---------------------------------------
*/
static bool_t
node_comp (
  __CR_IN__ const void_t*   key,
  __CR_IN__ const void_t*   obj
    )
{
    sML_NODE*   unit = (sML_NODE*)obj;

    return (str_cmpA(unit->id, (ansi_t*)key) == 0);
}

/*
---------------------------------------
    成员索引回调
---------------------------------------
*/
static uint_t
node_find (
  __CR_IN__ const void_t*   key
    )
{
    return (hash_str_djb2(key, str_lenA((ansi_t*)key)));
}

/*
=======================================
    加载多语言配置文件
=======================================
*/
CR_API mlang_t
mlang_load (
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ const wide_t*   defs,
  __CR_IN__ uint_t          cpage
    )
{
    sXMLw*      xml;
    leng_t      idx;
    ansi_t*     str;
    wide_t*     idw;
    sMLANG*     rett;
    sML_NODE    temp;

    /* 加载文本文件 */
    str = file_load_as_strA(name);
    if (str == NULL)
        return (NULL);
    xml = xml_from_textW(str);
    mem_free(str);
    if (xml == NULL)
        return (NULL);

    /* 检查首标签 */
    if (xml->nodes[0].name[0] == CR_WC('?'))
        idx = 1;
    else
        idx = 0;
    if (xml->nodes[idx].closed ||
        str_cmpIW(xml->nodes[idx].name, CR_WS("LANGUAGE")) != 0)
        goto _failure;

    /* 生成目标对象 */
    rett = struct_new(sMLANG);
    if (rett == NULL)
        goto _failure;
    rett->page = cpage;
    if (defs != NULL) {
        rett->defw = str_dupW(defs);
        if (rett->defw == NULL)
            rett->defa = NULL;
        else
            rett->defa = utf16_to_local(cpage, defs);
    }
    else {
        rett->defa = NULL;
        rett->defw = NULL;
    }
    if (!curbead_initT(&rett->list, sML_NODE, xml->count)) {
        TRY_FREE(rett->defa);
        TRY_FREE(rett->defw);
        mem_free(rett);
        goto _failure;
    }
    temp.txta = NULL;
    rett->list.find = node_find;
    rett->list.comp = node_comp;
    rett->list.free = node_free;

    /* 逐个解析标签 */
    for (idx++; idx < xml->count; idx++) {
        if (str_cmpIW(xml->nodes[idx].name, CR_WS("/LANGUAGE")) == 0)
            break;
        if (!xml->nodes[idx].closed) {
            idx = xml_skip_tagW(xml->nodes, idx, xml->count) - 1;
            continue;
        }

        /* 只支持这一种标签类型 */
        if (str_cmpIW(xml->nodes[idx].name, CR_WS("ENTRY")) == 0) {
            idw = xml_attr_stringW(CR_WS("id"), &xml->nodes[idx]);
            if (idw == NULL)
                continue;
            temp.id = utf16_to_local(cpage, idw);
            mem_free(idw);
            if (temp.id == NULL)
                continue;
            temp.txtw = xml_attr_stringW(CR_WS("text"), &xml->nodes[idx]);
            if (temp.txtw == NULL && xml->nodes[idx].found) {
                mem_free(temp.id);
                continue;
            }

            /* 插入珠帘查找表 */
            if (curbead_insertT(&rett->list, sML_NODE,
                        temp.id, &temp) == NULL) {
                mem_free(temp.txtw);
                mem_free(temp.id);
                mlang_free(rett);
                goto _failure;
            }
        }
    }
    xml_closeW(xml);
    return ((mlang_t)rett);

_failure:
    xml_closeW(xml);
    return (NULL);
}

/*
=======================================
    释放多语言配置对象
=======================================
*/
CR_API void_t
mlang_free (
  __CR_IN__ mlang_t xlang
    )
{
    sMLANG* real = (sMLANG*)xlang;

    curbead_freeT(&real->list, sML_NODE);
    TRY_FREE(real->defa);
    TRY_FREE(real->defw);
    mem_free(real);
}

/*
=======================================
    获取配置的词条A
=======================================
*/
CR_API const ansi_t*
mlang_get_txtA (
  __CR_IN__ mlang_t         xlang,
  __CR_IN__ const ansi_t*   id
    )
{
    sMLANG*     real;
    sML_NODE*   unit;

    real = (sMLANG*)xlang;
    unit = curbead_findT(&real->list, sML_NODE, id);
    if (unit == NULL) {
        if (real->defa == NULL)
            return (CR_AS("<null>"));
        return (real->defa);
    }
    if (unit->txta != NULL)
        return (unit->txta);
    if (unit->txtw == NULL) {
        if (real->defa == NULL)
            return (CR_AS("<empty>"));
        return (real->defa);
    }

    /* 转换格式后返回 */
    unit->txta = utf16_to_local(real->page, unit->txtw);
    if (unit->txta == NULL)
        return (CR_AS("<invw>"));
    return (unit->txta);
}

/*
=======================================
    获取配置的词条W
=======================================
*/
CR_API const wide_t*
mlang_get_txtW (
  __CR_IN__ mlang_t         xlang,
  __CR_IN__ const ansi_t*   id
    )
{
    sMLANG*     real;
    sML_NODE*   unit;

    real = (sMLANG*)xlang;
    unit = curbead_findT(&real->list, sML_NODE, id);
    if (unit == NULL) {
        if (real->defw == NULL)
            return (CR_WS("<null>"));
        return (real->defw);
    }
    if (unit->txtw == NULL) {
        if (real->defw == NULL)
            return (CR_WS("<empty>"));
        return (real->defw);
    }
    return (unit->txtw);
}

#endif  /* !_CR_NO_WIDE_ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
