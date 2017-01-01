/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2011-01-07  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>> CrHack XML 调用器函数库 <<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "hash.h"
#include "datlib.h"
#include "parser.h"
#include "plugin.h"
#include "strlib.h"

/* XML 调用器结构 */
typedef struct
{
        sbin_t      plugin;     /* 插件的句柄 */
        void_t*     aparam;     /* 额外的参数 */
        sCURTAIN    xtable;     /* 哈希查找表 */

} sXMLCALL;

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
    sXC_PORT*   unit = (sXC_PORT*)obj;

    return (str_cmpA(unit->name, (ansi_t*)key) == 0);
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
    加载 XML 调用插件模块
=======================================
*/
CR_API xmlcaller_t
xmlcall_load (
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ leng_t          count,
  __CR_IN__ void_t*         aparam
    )
{
    leng_t      idx;
    leng_t      cnts;
    sXC_PORT*   port;
    sXMLCALL*   rett;

    /* 创建结构 */
    rett = struct_new(sXMLCALL);
    if (rett == NULL)
        return (NULL);
    struct_zero(rett, sXMLCALL);

    /* 初始化接口表 */
    if (name != NULL)
    {
        /* 加载指定接口的模块 */
        rett->plugin = sbin_loadA(name);
        if (rett->plugin == NULL)
            goto _failure1;
        port = sbin_exportT(rett->plugin, CR_AS("__xmlca__"), sXC_PORT*);
        if (port == NULL)
            goto _failure2;

        /* 统计 XML 接口的个数 */
        for (cnts = 0; ; cnts++) {
            if (port[cnts].name == NULL)
                break;
        }
        if (cnts == 0)
            goto _failure2;
        if (count < cnts)
            count = cnts;
        if (!curtain_initT(&rett->xtable, sXC_PORT, count, 0))
            goto _failure2;
        rett->xtable.comp = function_comp;
        rett->xtable.find = function_find;

        /* 注册 XML 接口到查找表 */
        for (idx = 0; idx < cnts; idx++) {
            if (curtain_insertT(&rett->xtable, sXC_PORT,
                        port[idx].name, &port[idx]) == NULL) {
                curtain_freeT(&rett->xtable, sXC_PORT);
                goto _failure2;
            }
        }
    }
    else
    {
        /* 不指定文件名表示不加载模块 */
        if (!curtain_initT(&rett->xtable, sXC_PORT, count, 0))
            goto _failure1;
        rett->xtable.comp = function_comp;
        rett->xtable.find = function_find;
    }
    rett->aparam = aparam;
    return ((xmlcaller_t)rett);

_failure2:
    sbin_unload(rett->plugin);
_failure1:
    mem_free(rett);
    return (NULL);
}

/*
=======================================
    加载 XML 调用接口模块
=======================================
*/
CR_API bool_t
xmlcall_setup (
  __CR_IN__ xmlcaller_t     xmlcall,
  __CR_IN__ const sXC_PORT* port
    )
{
    leng_t      idx;
    leng_t      cnts;
    sXMLCALL*   real;

    /* 统计 XML 接口的个数 */
    real = (sXMLCALL*)xmlcall;
    for (cnts = 0; ; cnts++) {
        if (port[cnts].name == NULL)
            break;
    }

    /* 注册 XML 接口到查找表 */
    for (idx = 0; idx < cnts; idx++) {
        if (curtain_insertT(&real->xtable, sXC_PORT,
                    port[idx].name, &port[idx]) == NULL)
            return (FALSE);
    }
    return (TRUE);
}

/*
=======================================
    卸载 XML 调用插件模块
=======================================
*/
CR_API void_t
xmlcall_unload (
  __CR_IN__ xmlcaller_t xmlcall
    )
{
    sXMLCALL*   real;

    real = (sXMLCALL*)xmlcall;
    curtain_freeT(&real->xtable, sXC_PORT);
    if (real->plugin != NULL)
        sbin_unload(real->plugin);
    mem_free((void_t*)xmlcall);
}

/*
=======================================
    运行 XML 调用器脚本
=======================================
*/
CR_API bool_t
xmlcall_exec (
  __CR_IN__ xmlcaller_t     xmlcall,
  __CR_IN__ void_t*         uparam,
  __CR_IN__ const ansi_t*   script,
  __CR_IN__ sXMLu*          preload
    )
{
    sXMLu*      xml;
    leng_t      cnts;
    void_t*     parm;
    sXNODEu*    node;
    sXC_PORT*   port;
    sXMLCALL*   real;

    real = (sXMLCALL*)xmlcall;
    if (preload == NULL) {
        xml = xml_parseU(script);
        if (xml == NULL)
            return (FALSE);
    }
    else {
        xml = preload;      /* 使用预加载的脚本 */
    }
    cnts = xml->count;
    node = xml->nodes;
    parm = real->aparam;
    for (; cnts != 0; cnts--, node++)
    {
        if (node->name[0] == CR_AC('/') ||
            node->name[0] == CR_AC('?'))
            continue;
        port = curtain_findT(&real->xtable, sXC_PORT, node->name);
        if (mostly(port != NULL)) {
            if (mostly(port->func != NULL)) {
                if (!port->func(parm, uparam, node)) {
                    if (preload == NULL)
                        xml_closeU(xml);
                    return (FALSE);
                }
            }
        }
    }
    if (preload == NULL)
        xml_closeU(xml);
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
