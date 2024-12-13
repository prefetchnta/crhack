/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-11-03  */
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
/*  >>>>>>>>>>>>>>>>>>> TOTAL CFG 结构类型配置文件函数库 <<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "hash.h"
#include "safe.h"
#include "datlib.h"
#include "fileio.h"
#include "strlib.h"
#include "util/totcfg.h"

#if !defined(_CR_NO_WIDE_)

/* 常数内部结构 */
typedef struct
{
        bool_t          free;   /* 可释放 */
        leng_t          cnst;   /* 常数值 */
        const wide_t*   name;   /* 常数名 */

} sCONST;

/* 安全内部结构 */
typedef struct
{
        safe_check_t    func;   /* 安全阈 */
        const wide_t*   name;   /* 安全名 */

} sSAFER;

/* 序列化对象内部结构 */
typedef struct
{
        uint_t          local;  /* 本地编码值 */
        sCURTAIN        value;  /* 常数查找表 */
        sCURTAIN        types;  /* 类型查找表 */
        sCURTAIN        check;  /* 安全查找表 */
        const ansi_t*   cpage;  /* 输出的编码 */
        const ansi_t*   style;  /* 输出的风格 */

} sSTRUCT;

/*
---------------------------------------
    常数成员比较回调
---------------------------------------
*/
static bool_t
value_comp (
  __CR_IN__ const void_t*   key,
  __CR_IN__ const void_t*   obj
    )
{
    sCONST* unit = (sCONST*)obj;

    return (str_cmpW(unit->name, (wide_t*)key) == 0);
}

/*
---------------------------------------
    常数成员释放回调
---------------------------------------
*/
static void_t
value_free (
  __CR_IN__ void_t* obj
    )
{
    sCONST* unit = (sCONST*)obj;

    if (unit->free)
        mem_free(unit->name);
}

/*
---------------------------------------
    安全成员比较回调
---------------------------------------
*/
static bool_t
safer_comp (
  __CR_IN__ const void_t*   key,
  __CR_IN__ const void_t*   obj
    )
{
    sSAFER* unit = (sSAFER*)obj;

    return (str_cmpW(unit->name, (wide_t*)key) == 0);
}

/*
---------------------------------------
    类型成员比较回调
---------------------------------------
*/
static bool_t
types_comp (
  __CR_IN__ const void_t*   key,
  __CR_IN__ const void_t*   obj
    )
{
    sSRZ_TYPE*  unit = (sSRZ_TYPE*)obj;

    return (str_cmpW(unit->name, (wide_t*)key) == 0);
}

/*
---------------------------------------
    类型单独释放回调
---------------------------------------
*/
static void_t
membs_free (
  __CR_IN__ void_t* obj
    )
{
    mem_free(((sSRZ_MEMB*)obj)->name);
    mem_free(((sSRZ_MEMB*)obj)->type);
    mem_free(((sSRZ_MEMB*)obj)->show);
}

/*
---------------------------------------
    类型成员释放回调
---------------------------------------
*/
static void_t
types_free (
  __CR_IN__ void_t* obj
    )
{
    leng_t      idx;
    sSRZ_TYPE*  unit = (sSRZ_TYPE*)obj;

    if (unit->free)
        mem_free(unit->name);

    if (!unit->real) {
        if (unit->membs != NULL) {
            for (idx = 0; idx < unit->count; idx++)
                membs_free((void_t*)(&unit->membs[idx]));
            mem_free(unit->membs);
        }
    }
}

/*
---------------------------------------
    键值成员索引回调
---------------------------------------
*/
static uint_t
total_find (
  __CR_IN__ const void_t*   key
    )
{
    return (hash_str_djb2(key, str_lenW((wide_t*)key) * sizeof(wide_t)));
}

/*****************************************************************************/
/*                               内置原生类型                                */
/*****************************************************************************/

#include "cfg_type.inl"

/* 内置原生类型表 */
static const sSRZ_TYPE _rom_ s_srl_types[] =
{
    {
        TRUE, FALSE, sizeof(ansi_t), CR_WS("ansi_t"),
        NULL, ansi_t_bin2str, ansi_t_str2bin, 0, NULL,
    },
    {
        TRUE, FALSE, sizeof(wide_t), CR_WS("wide_t"),
        NULL, wide_t_bin2str, wide_t_str2bin, 0, NULL,
    },
    {
        TRUE, FALSE, sizeof(byte_t), CR_WS("byte_t"),
        NULL, byte_t_bin2str, byte_t_str2bin, 0, NULL,
    },
    {
        TRUE, FALSE, sizeof(byte_t), CR_WS("int08s"),
        NULL, int08s_bin2str, int08s_str2bin, 0, NULL,
    },
    {
        TRUE, FALSE, sizeof(byte_t), CR_WS("int08u"),
        NULL, int08u_bin2str, int08u_str2bin, 0, NULL,
    },
    {
        TRUE, FALSE, sizeof(int16s), CR_WS("int16s"),
        NULL, int16s_bin2str, int16s_str2bin, 0, NULL,
    },
    {
        TRUE, FALSE, sizeof(int16u), CR_WS("int16u"),
        NULL, int16u_bin2str, int16u_str2bin, 0, NULL,
    },
    {
        TRUE, FALSE, sizeof(int32s), CR_WS("int32s"),
        NULL, int32s_bin2str, int32s_str2bin, 0, NULL,
    },
    {
        TRUE, FALSE, sizeof(int32u), CR_WS("int32u"),
        NULL, int32u_bin2str, int32u_str2bin, 0, NULL,
    },
    {
        TRUE, FALSE, sizeof(int64s), CR_WS("int64s"),
        NULL, int64s_bin2str, int64s_str2bin, 0, NULL,
    },
    {
        TRUE, FALSE, sizeof(int64u), CR_WS("int64u"),
        NULL, int64u_bin2str, int64u_str2bin, 0, NULL,
    },
    {
        TRUE, FALSE, sizeof(fp32_t), CR_WS("fp32_t"),
        NULL, fp32_t_bin2str, fp32_t_str2bin, 0, NULL,
    },
    {
        TRUE, FALSE, sizeof(fp64_t), CR_WS("fp64_t"),
        NULL, fp64_t_bin2str, fp64_t_str2bin, 0, NULL,
    },
};

/*****************************************************************************/
/*                                  解析组                                   */
/*****************************************************************************/

/*
=======================================
    初始化序列化对象
=======================================
*/
CR_API stru_t
serial_init (
  __CR_IN__ uint_t          types_n,
  __CR_IN__ uint_t          value_n,
  __CR_IN__ uint_t          safer_n,
  __CR_IN__ uint_t          codepage,
  __CR_IN__ const ansi_t*   encoding
    )
{
    leng_t      idx;
    sSTRUCT*    rett;

    rett = struct_new(sSTRUCT);
    if (rett == NULL)
        return (NULL);
    if (!curtain_initT(&rett->value, sCONST, value_n, 0))
        goto _failure;
    rett->value.find = total_find;
    rett->value.comp = value_comp;
    rett->value.free = value_free;
    if (!curtain_initT(&rett->check, sSAFER, safer_n, 0)) {
        curtain_freeT(&rett->value, sCONST);
        goto _failure;
    }
    rett->check.find = total_find;
    rett->check.comp = safer_comp;
    if (!curtain_initT(&rett->types, sSRZ_TYPE, types_n, 0)) {
        curtain_freeT(&rett->check, sSAFER);
        curtain_freeT(&rett->value, sCONST);
        goto _failure;
    }
    rett->types.find = total_find;
    rett->types.comp = types_comp;
    rett->types.free = types_free;

    /* 注册内置原生类型 */
    for (idx = 0; idx < cntsof(s_srl_types); idx++) {
        if (!serial_set_type((stru_t)rett, &s_srl_types[idx])) {
            serial_free((stru_t)rett);
            return (NULL);
        }
    }
    rett->local = codepage;
    rett->cpage = encoding;
    rett->style = CR_AS("");
    return ((stru_t)rett);

_failure:
    mem_free(rett);
    return (NULL);
}

/*
=======================================
    释放序列化对象
=======================================
*/
CR_API void_t
serial_free (
  __CR_IN__ stru_t  serial
    )
{
    sSTRUCT*    real;

    real = (sSTRUCT*)serial;
    curtain_freeT(&real->types, sSRZ_TYPE);
    curtain_freeT(&real->check, sSAFER);
    curtain_freeT(&real->value, sCONST);
    mem_free((void_t*)real);
}

/*
=======================================
    设置输出风格字符串
=======================================
*/
CR_API void_t
serial_set_style (
  __CR_IN__ stru_t          serial,
  __CR_IN__ const ansi_t*   style
    )
{
    sSTRUCT*    real;

    real = (sSTRUCT*)serial;
    if (style != NULL)
        real->style = style;
    else
        real->style = CR_AS("");
}

/*
=======================================
    返回序列化对象编码
=======================================
*/
CR_API uint_t
serial_codepage (
  __CR_IN__ stru_t  serial
    )
{
    return (((sSTRUCT*)serial)->local);
}

/*
=======================================
    设置自定义原生类型
=======================================
*/
CR_API bool_t
serial_set_type (
  __CR_IO__ stru_t              serial,
  __CR_IN__ const sSRZ_TYPE*    real_type
    )
{
    sSTRUCT*    real;

    if (!real_type->real ||
        real_type->count != 0 || real_type->membs != NULL)
        return (FALSE);
    real = (sSTRUCT*)serial;
    if (curtain_insertT(&real->types, sSRZ_TYPE,
                        real_type->name, real_type) == NULL)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    设置自定义安全阈
=======================================
*/
CR_API bool_t
serial_set_check (
  __CR_IO__ stru_t          serial,
  __CR_IN__ safe_check_t    safer,
  __CR_IN__ const wide_t*   name
    )
{
    sSAFER      temp;
    sSTRUCT*    real;

    temp.name = name;
    temp.func = safer;
    real = (sSTRUCT*)serial;
    if (curtain_insertT(&real->check, sSAFER, name, &temp) == NULL)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    获取自定义常数值
=======================================
*/
CR_API bool_t
serial_get_value (
  __CR_IN__ stru_t          serial,
  __CR_OT__ leng_t*         value,
  __CR_IN__ const wide_t*   name
    )
{
    sCONST*     dfne;
    sSTRUCT*    real;

    if (name[0] != CR_WC('$'))
        return (FALSE);
    real = (sSTRUCT*)serial;
    dfne = curtain_findT(&real->value, sCONST, name);
    if (dfne == NULL)
        return (FALSE);
    if (value != NULL)
        *value = dfne->cnst;
    return (TRUE);
}

/*
=======================================
    设置自定义常数值
=======================================
*/
CR_API bool_t
serial_set_value (
  __CR_IO__ stru_t          serial,
  __CR_IN__ leng_t          value,
  __CR_IN__ const wide_t*   name,
  __CR_IN__ bool_t          is_free
    )
{
    sCONST      temp;
    sSTRUCT*    real;

    if (name[0] != CR_WC('$'))
        return (FALSE);
    temp.name = name;
    temp.cnst = value;
    temp.free = is_free;
    real = (sSTRUCT*)serial;
    if (curtain_insertT(&real->value, sCONST, name, &temp) == NULL)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    建立序列化格式表
=======================================
*/
CR_API bool_t
serial_setup (
  __CR_IO__ stru_t          serial,
  __CR_IN__ const ansi_t*   header
    )
{
    sXMLw*      xml;
    leng_t      idx;
    leng_t      cnst;
    sARRAY      list;
    wide_t*     name;
    wide_t*     safe;
    wide_t*     gots;
    sCONST*     dfne;
    sSAFER*     chks;
    sXNODEw*    node;
    sSTRUCT*    real;
    sSRZ_MEMB   memb;
    sSRZ_TYPE   temp;
    sSRZ_TYPE*  find;

    xml = xml_from_textW(header);
    if (xml == NULL)
        return (FALSE);
    node = xml->nodes;
    real = (sSTRUCT*)serial;

    /* 开始分析格式标签 */
    for (idx = 0; idx < xml->count; idx++, node++)
    {
        /* 结构体节点 */
        if (!node->closed &&
            str_cmpIW(node->name, CR_WS("STRUCT")) == 0)
        {
            /* 注册一个新类型 (必须未使用过) */
            name = xml_attr_stringW(CR_WS("name"), node);
            if (name == NULL)
                goto _failure1;
            find = curtain_findT(&real->types, sSRZ_TYPE, name);
            if (find != NULL)
                goto _failure2;

            /* 超出直接跳出 */
            if (++idx >= xml->count) {
                mem_free(name);
                break;
            }
            struct_zero(&temp, sSRZ_TYPE);
            struct_zero(&memb, sSRZ_MEMB);
            array_initT(&list, sSRZ_MEMB);
            safe = xml_attr_stringW(CR_WS("safe"), node++);
            list.free = membs_free;

            /* 开始分析成员标签 */
            while (str_cmpIW(node->name, CR_WS("/STRUCT")) != 0)
            {
                /* 三种类型的成员节点 */
                if (node->closed &&
                   (str_cmpIW(node->name, CR_WS("MEMBER")) == 0 ||
                    str_cmpIW(node->name, CR_WS("HASHER")) == 0 ||
                    str_cmpIW(node->name, CR_WS("IGNORE")) == 0))
                {
                    /* 抽取成员属性 */
                    memb.name = xml_attr_stringW(CR_WS("name"), node);
                    memb.type = xml_attr_stringW(CR_WS("type"), node);
                    if (memb.name == NULL || memb.type == NULL)
                        goto _failure4;

                    /* 查找是否有这个类型 */
                    find = curtain_findT(&real->types, sSRZ_TYPE, memb.type);
                    if (find == NULL)
                        goto _failure4;
                    memb.skip = memb.hash = FALSE;
                    if (str_cmpIW(node->name, CR_WS("HASHER")) == 0)
                        memb.hash = TRUE;
                    else
                    if (str_cmpIW(node->name, CR_WS("IGNORE")) == 0)
                        memb.skip = TRUE;

                    /* 成员个数可以使用常数宏 */
                    gots = xml_attr_bufferW(CR_WS("count"), node);
                    if (gots == NULL) {
                        memb.cnts = 1;
                    }
                    else
                    if (gots[0] == CR_WC('$')) {
                        gots = xml_attr_stringW(CR_WS("count"), node);
                        if (gots == NULL)
                            goto _failure4;
                        dfne = curtain_findT(&real->value, sCONST, gots);
                        mem_free(gots);
                        if (dfne == NULL)
                            goto _failure4;
                        memb.cnts = dfne->cnst;
                    }
                    else {
                        memb.cnts = str2intxW(gots, NULL);
                    }

                    /* 个数为0表示跳过之 */
                    if (memb.cnts == 0)
                        memb.skip = TRUE;

                    /* 计算此成员的相关属性 */
                    memb.unit = find->size;
                    if (cut_mul(&memb.size, memb.unit, memb.cnts))
                        goto _failure4;

                    /* 转换用于显示的字段名编码 */
                    memb.show = utf16_to_local(real->local, memb.name);
                    if (memb.show == NULL)
                        goto _failure4;

                    /* 压入列表后再增加偏移 */
                    memb.attr = xml_attr_intxW(CR_WS("attr"), 0, node);
                    if (array_push_growT(&list, sSRZ_MEMB, &memb) == NULL) {
                        mem_free(memb.show);
                        goto _failure4;
                    }

                    /* 防止加法溢出 */
                    if (cut_addu(&memb.offs, memb.offs, memb.size)) {
                        TRY_FREE(safe);
                        goto _failure3;
                    }
                }

                /* 超出直接跳出 */
                if (++idx >= xml->count)
                    break;
                node = node + 1;
            }

            /* 设置类型属性 */
            if (safe == NULL) {
                temp.safe = NULL;
            }
            else {
                chks = curtain_findT(&real->check, sSAFER, safe);
                mem_free(safe);
                if (chks == NULL)
                    goto _failure3;
                temp.safe = chks->func;
            }
            temp.free = TRUE;
            temp.name = name;
            temp.real = FALSE;
            temp.size = memb.offs;  /* 类型的大小为最后成员的计算偏移 */
            temp.count = array_get_sizeT(&list, sSRZ_MEMB);
            temp.membs = mem_talloc(temp.count, sSRZ_MEMB);
            if (temp.membs == NULL)
                goto _failure3;
            mem_cpy((void_t*)temp.membs, array_get_dataT(&list, sSRZ_MEMB),
                             temp.count * sizeof(sSRZ_MEMB));
            /* 插入查找表 */
            if (curtain_insertT(&real->types, sSRZ_TYPE,
                                 name, &temp) == NULL) {
                mem_free(temp.membs);
                goto _failure3;
            }
            /* 安全释放列表 */
            list.free = NULL;
            array_freeT(&list, sSRZ_MEMB);
        }
        else
        if (node->closed &&     /* 常数节点 */
            str_cmpIW(node->name, CR_WS("DEFINE")) == 0)
        {
            /* 注册一个新常数 */
            name = xml_attr_stringW(CR_WS("name"), node);
            if (name == NULL)
                goto _failure1;
            cnst = xml_attr_intxW(CR_WS("value"), 0x00, node);
            if (!serial_set_value(serial, cnst, name, TRUE))
                goto _failure2;
        }
    }
    xml_closeW(xml);
    return (TRUE);

_failure4:
    TRY_FREE(safe);
    TRY_FREE(memb.name);
    TRY_FREE(memb.type);
_failure3:
    array_freeT(&list, sSRZ_MEMB);
_failure2:
    mem_free(name);
_failure1:
    xml_closeW(xml);
    return (FALSE);
}

/*
---------------------------------------
    查找当前类型的成员 (递归)
---------------------------------------
*/
static bool_t
serial_search (
  __CR_IN__ stru_t              serial,
  __CR_OT__ sSRZ_MEMB*          member,
  __CR_IN__ wide_t**            list,
  __CR_IN__ uint_t              count,
  __CR_IN__ uint_t              index,
  __CR_IN__ const sSRZ_TYPE*    strct
    )
{
    leng_t      idx;
    leng_t      cmon;
    leng_t      amon;
    wide_t*     cnxt;
    wide_t*     anxt;
    sSTRUCT*    real;
    sSRZ_TYPE*  find;

    /* 虽然不可能, 但还是检查一下 */
    if (strct->real || index >= count)
        return (FALSE);

    /* 是否使用了数组下标 [索引, 数量] */
    anxt = str_chrW(list[index], CR_WC(']'));
    cnxt = str_chrW(list[index], CR_WC('['));
    if (cnxt != NULL &&
        anxt != NULL && anxt[1] == CR_NC(NIL)) {
        cnxt[0] = CR_NC(NIL);
        cmon = (leng_t)str2intxW(&cnxt[1], &idx);
        anxt = str_chrW(&cnxt[idx + 1], CR_WC(','));
        if (anxt != NULL)
            amon = (leng_t)str2intxW(&anxt[1], NULL);
        else
            amon = 1;
    }
    else
    {
        cmon = 0;   /* 默认索引 */
        amon = 0;   /* 默认数量 */
    }

    /* 逐个查找成员名称 */
    for (idx = 0; idx < strct->count; idx++)
    {
        /* 不区分名称的大小写 */
        if (str_cmpIW(strct->membs[idx].name, list[index]) == 0)
        {
            /* 计算数组下标 */
            if (cmon >= strct->membs[idx].cnts)
                return (FALSE);
            member->offs += strct->membs[idx].offs + cmon *
                            strct->membs[idx].unit;

            /* 找到了指定的成员 */
            if (index == count - 1)
            {
                /* 已经是最后的一个节点 */
                if (amon == 0)
                    amon = strct->membs[idx].cnts - cmon;
                else
                if (amon > strct->membs[idx].cnts - cmon)
                    return (FALSE);
                member->idxs = cmon;
                member->cnts = amon;
                member->unit = strct->membs[idx].unit;
                member->skip = strct->membs[idx].skip;
                member->hash = strct->membs[idx].hash;
                member->attr = strct->membs[idx].attr;
                member->name = strct->membs[idx].name;
                member->type = strct->membs[idx].type;
                member->show = strct->membs[idx].show;
                member->size = amon * member->unit;
                return (TRUE);
            }

            /* 判断这个成员的类型 */
            real = (sSTRUCT*)serial;
            find = curtain_findT(&real->types, sSRZ_TYPE,
                                  strct->membs[idx].type);
            if (find == NULL || find->real)
                return (FALSE);

            /* 非最后一个节点, 继续向下查找 */
            return (serial_search(serial, member, list, count,
                                  index + 1, find));
        }
    }

    /* 没有在类型里找到指定成员 */
    return (FALSE);
}

/*
=======================================
    根据成员路径查找成员
=======================================
*/
CR_API bool_t
serial_find_memb (
  __CR_IN__ stru_t          serial,
  __CR_OT__ sSRZ_MEMB*      member,
  __CR_IN__ const wide_t*   type,
  __CR_IN__ const wide_t*   path
    )
{
    uint_t      cnts;
    wide_t*     copy;
    wide_t**    list;
    sSTRUCT*    real;
    sSRZ_TYPE*  find;

    copy = str_dupW(path);
    if (copy == NULL)
        return (FALSE);
    list = path_splitW(copy, &cnts);
    if (list == NULL) {
        mem_free(copy);
        return (FALSE);
    }

    /* 先找到指定的类型 */
    real = (sSTRUCT*)serial;
    struct_zero(member, sSRZ_MEMB);
    find = curtain_findT(&real->types, sSRZ_TYPE, type);
    if (find == NULL || find->real)
        goto _failure;

    /* 逐层递归查找成员 */
    if (!serial_search(serial, member, list, cnts, 0, find))
        goto _failure;
    mem_free(list);
    mem_free(copy);
    return (TRUE);

_failure:
    mem_free(list);
    mem_free(copy);
    return (FALSE);
}

/*
=======================================
    根据类型名查找成员
=======================================
*/
CR_API bool_t
serial_find_type (
  __CR_IN__ stru_t          serial,
  __CR_OT__ sSRZ_TYPE*      stru_type,
  __CR_IN__ const wide_t*   type
    )
{
    sSTRUCT*    real;
    sSRZ_TYPE*  find;

    real = (sSTRUCT*)serial;
    find = curtain_findT(&real->types, sSRZ_TYPE, type);
    if (find == NULL)
        return (FALSE);
    struct_cpy(stru_type, find, sSRZ_TYPE);
    return (TRUE);
}

/*****************************************************************************/
/*                                  数据组                                   */
/*****************************************************************************/

/*
---------------------------------------
    生成校验一个类型数据 (递归)
---------------------------------------
*/
static bool_t
serial_hash_type (
  __CR_IN__ stru_t              serial,
  __CR_IO__ void_t*             data,
  __CR_IN__ leng_t              size,   /* 注意: 类型/单位大小 */
  __CR_IN__ const sSRZ_TYPE*    type,
  __CR_IN__ bin_hash_t          hasher,
  __CR_IN__ const wide_t*       member,
  __CR_IN__ err_hash_t          errors,
  __CR_IN__ leng_t              index,
  __CR_IN__ void_t*             param
    )
{
    leng_t      idx;
    bool_t      rett;
    bool_t      okay;
    leng_t      next;
    byte_t*     pntr;
    sSTRUCT*    real;
    sSRZ_HASH   errs;
    sSRZ_TYPE*  find;

    /* 安全检查 */
    if (size != type->size)
        return (FALSE);
    if (type->real)     /* 原生类型假设校验肯定通过 */
        return (TRUE);
    okay = TRUE;
    real = (sSTRUCT*)serial;

    /* 逐个成员查找是否为非原生类型 */
    for (idx = 0; idx < type->count; idx++)
    {
        /* 查找成员类型 */
        find = curtain_findT(&real->types, sSRZ_TYPE, type->membs[idx].type);
        if (find == NULL)
            return (FALSE);

        /* 非原生类型需要递归处理 */
        if (!find->real)
        {
            /* 注意: 只能一个个单元的进行 */
            pntr = (byte_t*)data + type->membs[idx].offs;
            for (next = 0; next < type->membs[idx].cnts; next++) {
                okay &= serial_hash_type(serial, pntr, type->membs[idx].unit,
                                         find, hasher, type->membs[idx].name,
                                         errors, next, param);
                pntr += type->membs[idx].unit;
            }
        }
    }

    /* 要求最后一个成员为校验字段 */
    if (idx != 0 && type->membs[idx - 1].hash) {
        rett = hasher(data, size, type->membs[idx - 1].size);
        if (!rett && errors != NULL) {
            errs.err_idxs = index;
            errs.err_memb = member;
            errs.err_type = type->name;
            errors(param, &errs);
        }
        okay &= rett;
    }
    return (okay);
}

/*
=======================================
    生成校验一个成员数据
=======================================
*/
CR_API bool_t
serial_hash (
  __CR_IN__ stru_t              serial,
  __CR_IO__ void_t*             data,
  __CR_IN__ leng_t              size,   /* 注意: 成员总大小 (多个) */
  __CR_IN__ const sSRZ_MEMB*    member,
  __CR_IN__ bin_hash_t          hasher,
  __CR_IN__ err_hash_t          errors,
  __CR_IN__ void_t*             param
    )
{
    leng_t      idx;
    bool_t      okay;
    byte_t*     pntr;
    sSTRUCT*    real;
    sSRZ_TYPE*  find;

    real = (sSTRUCT*)serial;
    find = curtain_findT(&real->types, sSRZ_TYPE, member->type);
    if (find == NULL || size != member->size)
        return (FALSE);
    okay = TRUE;
    pntr = (byte_t*)data;

    /* 如果是数组则一个个校验 */
    for (idx = 0; idx < member->cnts; idx++) {
        okay &= serial_hash_type(real, pntr, member->unit, find,
                       hasher, member->name, errors, idx, param);
        pntr += member->unit;
    }
    return (okay);
}

/*
---------------------------------------
    类型数据转换到字符串 (递归)
---------------------------------------
*/
static ansi_t*
serial_bin2str_type (
  __CR_IN__ stru_t              serial,
  __CR_IO__ const void_t*       data,
  __CR_IN__ leng_t              size,   /* 注意: 成员总大小 (多个) */
  __CR_IN__ const sSRZ_TYPE*    type,
  __CR_IN__ const sSRZ_MEMB*    member
    )
{
    leng_t      idx;
    leng_t      next;
    ansi_t*     outp;
    byte_t*     pntr;
    iDATOT*     dato;
    sSTRUCT*    real;
    sSRZ_TYPE*  find;

    if (size != member->size)
        return (NULL);

    /* 忽略和校验不做处理 */
    if (member->skip || member->hash)
        return (str_dupA(CR_AS("\0")));

    /* 生成内存输出流 */
    dato = create_buff_out(0);
    if (dato == NULL)
        return (NULL);
    real = (sSTRUCT*)serial;

    /* 原生类型直接输出 */
    if (type->real)
    {
        /* 转换二进制到文本并输出到流 */
        if (type->bin2str == NULL)
            goto _failure1;
        outp = type->bin2str(data, size, member);
        if (outp == NULL)
            goto _failure1;
        if (!CR_VCALL(dato)->putsA(dato, outp, 0))
            goto _failure2;
        mem_free(outp);
    }
    else
    {
        /* 统一以数组形式输出 */
        for (next = 0; next < member->cnts; next++)
        {
            /* 输出标签头 */
            if (member->cnts == 1) {
                outp = str_fmtA("<%s>", member->show);
            }
            else {
                outp = str_fmtA("<%s index=\"%" CR_SZ "u\">",
                                member->show, next);
            }
            if (outp == NULL)
                goto _failure1;
            if (!CR_VCALL(dato)->putsA(dato, outp, 0))
                goto _failure2;
            mem_free(outp);

            /* 非原生类型输出标签并递归其子成员 */
            for (idx = 0; idx < type->count; idx++) {
                find = curtain_findT(&real->types, sSRZ_TYPE,
                                      type->membs[idx].type);
                if (find == NULL)
                    goto _failure1;
                pntr = (byte_t*)data + type->membs[idx].offs;
                outp = serial_bin2str_type(serial, pntr,
                            type->membs[idx].size, find, &type->membs[idx]);
                if (outp == NULL)
                    goto _failure1;
                if (!CR_VCALL(dato)->putsA(dato, outp, 0))
                    goto _failure2;
                mem_free(outp);
            }

            /* 输出标签尾 */
            outp = str_fmtA("</%s>", member->show);
            if (outp == NULL)
                goto _failure1;
            if (!CR_VCALL(dato)->putsA(dato, outp, 0))
                goto _failure2;
            mem_free(outp);
        }
    }

    /* 输出字符串结束符并返回 */
    if (!CR_VCALL(dato)->putb_no(dato, NIL))
        goto _failure1;
    outp = str_dupA((ansi_t*)(CR_VCALL(dato)->flush(dato)));
    CR_VCALL(dato)->release(dato);
    return (outp);

_failure2:
    mem_free(outp);
_failure1:
    CR_VCALL(dato)->release(dato);
    return (NULL);
}

/*
=======================================
    成员数据转换到字符串
=======================================
*/
CR_API ansi_t*
serial_bin2str (
  __CR_IN__ stru_t              serial,
  __CR_IO__ const void_t*       data,
  __CR_IN__ leng_t              size,   /* 注意: 成员总大小 (多个) */
  __CR_IN__ const sSRZ_MEMB*    member
    )
{
    ansi_t*     text;
    ansi_t*     outp;
    sSTRUCT*    real;
    sSRZ_TYPE*  find;

    real = (sSTRUCT*)serial;
    find = curtain_findT(&real->types, sSRZ_TYPE, member->type);
    if (find == NULL)
        return (NULL);
    outp = serial_bin2str_type(serial, data, size, find, member);
    if (outp == NULL)
        return (NULL);

    /* 加上 XML 文档头 */
    text = str_fmtA("<!-- encoding: %u -->\r\n"
                "<?xml version=\"1.0\" encoding=\"%s\" ?>\r\n"
                "%s\r\n<DATASHEET>\r\n%s\r\n</DATASHEET>\r\n",
                 real->local, real->cpage, real->style, outp);
    mem_free(outp);
    return (text);
}

/*
---------------------------------------
    字符串转换到类型数据 (递归)
---------------------------------------
*/
static leng_t
serial_str2bin_type (
  __CR_IN__ stru_t              serial,
  __CR_IO__ void_t*             data,
  __CR_IN__ sXNODEw*            nodes,
  __CR_IN__ leng_t              index,
  __CR_IN__ leng_t              count,
  __CR_IN__ const sSRZ_TYPE*    type,
  __CR_IN__ const sSRZ_MEMB*    member
    )
{
    leng_t      next;
    byte_t*     pntr;
    sSTRUCT*    real;
    sSRZ_MEMB   memb;
    sSRZ_TYPE*  find;

    /* 非指定成员标签直接跳过标签 */
    if (str_cmpIW(nodes[index].name, CR_WS("/DATASHEET")) == 0)
        return (count);
    if (str_cmpIW(nodes[index].name, member->name) != 0)
        return (xml_skip_tagW(nodes, index, count));

    /* 获取此标签相关信息并进行安全检查 */
    next = xml_attr_intxW(CR_WS("index"), 0, &nodes[index]);
    if (next >= member->cnts)
        return ((leng_t)-1);

    /* 原生类型直接输出并返回 */
    real = (sSTRUCT*)serial;
    if (type->real)
    {
        /* 转换文本到二进制至指定位置 */
        if (type->str2bin == NULL)
            return ((leng_t)-1);

        /* 必须传入成员总大小 */
        if (!type->str2bin(data, member->size, member->attr, next,
                           real->local, &nodes[index]))
            return ((leng_t)-1);
        return (xml_skip_tagW(nodes, index, count));
    }
    else
    {
        /* 非原生类型必须不是封闭节点 */
        if (nodes[index].closed)
            return ((leng_t)-1);
    }

    /* 计算数据偏移地址 */
    if (++index >= count)
        return (index);
    data = (byte_t*)data + next * member->unit;

    /* 处理到成员结束标签为止 */
    while (nodes[index].name[0] != CR_WC('/') ||
           str_cmpIW(&nodes[index].name[1], member->name) != 0)
    {
        /* 先找到指定的成员 (允许找不到的情况) */
        if (!serial_find_memb(serial, &memb, type->name, nodes[index].name))
        {
            /* 跳过这个未识别的标签 */
            index = xml_skip_tagW(nodes, index, count);
            if (index >= count)
                break;
            continue;
        }

        /* 忽略的成员直接跳过标签 */
        if (memb.skip || memb.hash) {
            index = xml_skip_tagW(nodes, index, count);
            if (index >= count)
                break;
            continue;
        }

        /* 递归处理 */
        find = curtain_findT(&real->types, sSRZ_TYPE, memb.type);
        if (find == NULL)
            return ((leng_t)-1);
        pntr = (byte_t*)data + memb.offs;
        index = serial_str2bin_type(serial, pntr, nodes, index, count,
                                    find, &memb);
        if (index == (leng_t)-1)
            return (index);

        /* 超出直接跳出 */
        if (index >= count)
            break;
    }

    /* 对数据的后期处理 */
    if (type->safe != NULL) {
        if (!type->safe(data, type->size))
            return ((leng_t)-1);
    }
    /* 保证返回的索引不会溢出 */
    return ((index < count) ? index + 1 : index);
}

/*
=======================================
    字符串转换到成员数据
=======================================
*/
CR_API bool_t
serial_str2bin (
  __CR_IN__ stru_t              serial,
  __CR_IO__ void_t*             data,
  __CR_IN__ leng_t              size,   /* 注意: 成员总大小 (多个) */
  __CR_IN__ const ansi_t*       text,
  __CR_IN__ const sSRZ_MEMB*    member
    )
{
    sXMLw*      xml;
    leng_t      idx;
    sSTRUCT*    real;
    sSRZ_TYPE*  find;

    if (size != member->size)
        return (FALSE);
    real = (sSTRUCT*)serial;
    find = curtain_findT(&real->types, sSRZ_TYPE, member->type);
    if (find == NULL)
        return (FALSE);
    mem_zero(data, size);

    /* 忽略的成员直接返回成功 */
    if (member->skip || member->hash)
        return (TRUE);

    /* 解析输入的 XML 文档 */
    xml = xml_from_textW(text);
    if (xml == NULL)
        return (FALSE);

    /* 定位到数据开始标签 */
    for (idx = 0; idx < xml->count; idx++) {
        if (str_cmpIW(xml->nodes[idx].name, CR_WS("DATASHEET")) == 0) {
            idx = idx + 1;
            break;
        }
    }
    /* 定位到此成员的标签 */
    while (idx < xml->count) {
        if (str_cmpIW(xml->nodes[idx].name, member->name) == 0)
            break;
        if (str_cmpIW(xml->nodes[idx].name, CR_WS("/DATASHEET")) == 0)
            goto _func_out;
        idx = xml_skip_tagW(xml->nodes, idx, xml->count);
    }
    /* 无当前成员标签也算正常 */
    if (idx < xml->count && idx + 1 < xml->count) {
        do
        {
            /* 直到所有标签处理完为止 */
            idx = serial_str2bin_type(serial, data, xml->nodes, idx,
                                      xml->count, find, member);
            if (idx == (leng_t)-1) {
                xml_closeW(xml);
                return (FALSE);
            }
        } while (idx < xml->count);
    }
_func_out:
    xml_closeW(xml);
    return (TRUE);
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
