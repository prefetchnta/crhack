/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2011-08-16  */
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
/*  >>>>>>>>>>>>>>>>>>>>> TOTAL CFG 报警配置文件函数库 <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "hash.h"
#include "datlib.h"
#include "fileio.h"
#include "strlib.h"
#include "util/totcfg.h"

#if !defined(_CR_NO_WIDE_)

/* 内部使用的结构 */
typedef struct
{
        uint_t      page;   /* 本地语言编码 */
        sARRAY      list;   /* 实际节点对象 */
        sCURBEAD    find;   /* 节点查找对象 */

} sALARM;

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
    sAL_NODE*   unit;

    /* 可增长数组用 */
    unit = (sAL_NODE*)obj;
    TRY_FREE(unit->txta);
    mem_free(unit->txtw);
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
    int32u      id;
    sAL_NODE**  unit;

    /* 珠帘查找表用 */
    id = *(const int32u*)key;
    unit = (sAL_NODE**)obj;
    if (id == unit[0]->id)
        return (TRUE);
    return (FALSE);
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
    int32u  id;

    /* 珠帘查找表用 */
    id = *(const int32u*)key;
    return ((uint_t)id);
}

/*
---------------------------------------
    解析报警属性
---------------------------------------
*/
static int32u
alarm_get_attr (
  __CR_IO__ sXNODEw*    node
    )
{
    wide_t  *str;
    int32u  attr = 0UL;

    /* 只有在可忽略的情况下才可设置是否启用 */
    if (xml_attr_intxW(CR_WS("repeatable"), TRUE, node))
        attr |= AL_REPEAT;
    if (xml_attr_intxW(CR_WS("ignorable"), FALSE, node)) {
        attr |= AL_IGNORE;
        if (xml_attr_intxW(CR_WS("enabled"), TRUE, node))
            attr |= AL_ENABLE;
    }
    else {
        attr |= AL_ENABLE;  /* 不可忽略必定启用 */
    }

    /* 报警属性为0表示非法值 */
    str = xml_attr_stringW(CR_WS("type"), node);
    if (str == NULL)
        return (0UL);
    if (str_cmpW(str, CR_WS("ICO_ERRO")) == 0)
        attr |= AL_ICO_ERRO;
    else
    if (str_cmpW(str, CR_WS("ICO_WARN")) == 0)
        attr |= AL_ICO_WARN;
    else
    if (str_cmpW(str, CR_WS("ICO_HINT")) == 0)
        attr |= AL_ICO_HINT;
    else
        attr = 0UL;
    mem_free(str);
    return (attr);
}

/*
=======================================
    加载报警配置文件
=======================================
*/
CR_API alarm_t
alarm_load (
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ uint_t          cpage
    )
{
    sXMLw*      xml;
    leng_t      idx;
    leng_t      cnt;
    ansi_t*     str;
    sALARM*     rett;
    sAL_NODE    temp;
    sAL_NODE*   pntr;

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
        str_cmpIW(xml->nodes[idx].name, CR_WS("ALARMS")) != 0)
        goto _failure1;

    /* 生成目标对象 */
    rett = struct_new(sALARM);
    if (rett == NULL)
        goto _failure1;
    rett->page = cpage;
    temp.rv[0] = temp.rv[1] = 0UL;
    array_initT(&rett->list, sAL_NODE);
    rett->list.free = node_free;

    /* 逐个解析标签 */
    for (idx++; idx < xml->count; idx++) {
        if (str_cmpIW(xml->nodes[idx].name, CR_WS("/ALARMS")) == 0)
            break;
        if (!xml->nodes[idx].closed) {
            idx = xml_skip_tagW(xml->nodes, idx, xml->count) - 1;
            continue;
        }

        /* 只支持这一种标签类型 */
        if (str_cmpIW(xml->nodes[idx].name, CR_WS("INFOR")) == 0) {
            temp.id = xml_attr_intx32W(CR_WS("id"), 0, &xml->nodes[idx]);
            if (temp.id == 0 && !xml->nodes[idx].found)
                goto _failure2;
            temp.txtw = xml_attr_stringW(CR_WS("text"), &xml->nodes[idx]);
            if (temp.txtw == NULL)
                goto _failure2;
            temp.attr1 = alarm_get_attr(&xml->nodes[idx]);
            if (temp.attr1 == 0) {
                mem_free(temp.txtw);
                goto _failure2;
            }
            temp.txta = utf16_to_local(cpage, temp.txtw);
            if (temp.txta == NULL)
                temp.txta = str_dupA(CR_AS("<invw>"));

            /* 压入可增长数组 */
            temp.cnts = array_get_sizeT(&rett->list, sAL_NODE);
            if (array_push_growT(&rett->list, sAL_NODE, &temp) == NULL) {
                TRY_FREE(temp.txta);
                mem_free(temp.txtw);
                goto _failure2;
            }
        }
    }

    /* 注册指针到珠帘查找表 */
    cnt  = array_get_sizeT(&rett->list, sAL_NODE);
    pntr = array_get_dataT(&rett->list, sAL_NODE);
    if (!curbead_initT(&rett->find, sAL_NODE*, cnt))
        goto _failure2;
    rett->find.find = node_find;
    rett->find.comp = node_comp;
    for (idx = 0; idx < cnt; idx++, pntr++) {
        if (curbead_insertT(&rett->find, sAL_NODE*,
                    &pntr->id, &pntr) == NULL) {
            alarm_free(rett);
            goto _failure1;
        }
    }
    xml_closeW(xml);
    return ((alarm_t)rett);

_failure2:
    array_freeT(&rett->list, sAL_NODE);
    mem_free(rett);
_failure1:
    xml_closeW(xml);
    return (NULL);
}

/*
=======================================
    释放报警配置对象
=======================================
*/
CR_API void_t
alarm_free (
  __CR_IN__ alarm_t alert
    )
{
    sALARM* real = (sALARM*)alert;

    curbead_freeT(&real->find, sAL_NODE*);
    array_freeT(&real->list, sAL_NODE);
    mem_free((void_t*)real);
}

/*
=======================================
    获取报警节点个数
=======================================
*/
CR_API leng_t
alarm_get_count (
  __CR_IN__ alarm_t alert
    )
{
    return (array_get_sizeT(&(((sALARM*)alert)->list), sAL_NODE));
}

/*
=======================================
    查找报警配置节点
=======================================
*/
CR_API sAL_NODE*
alarm_find (
  __CR_IN__ alarm_t     alert,
  __CR_IN__ int32u      id
    )
{
    sALARM*     real;
    sAL_NODE**  unit;

    real = (sALARM*)alert;
    unit = curbead_findT(&real->find, sAL_NODE*, &id);
    return ((unit == NULL) ? NULL : unit[0]);
}

/*
=======================================
    获取报警配置节点
=======================================
*/
CR_API bool_t
alarm_get_node (
  __CR_IN__ alarm_t     alert,
  __CR_IN__ leng_t      idx,
  __CR_OT__ sAL_NODE*   node
    )
{
    sALARM*     real;
    sAL_NODE*   unit;

    real = (sALARM*)alert;
    unit = array_get_unit_safeT(&real->list, sAL_NODE, idx);
    if (unit == NULL)
        return (FALSE);
    struct_cpy(node, unit, sAL_NODE);
    return (TRUE);
}

/*
=======================================
    设置报警配置节点
=======================================
*/
CR_API bool_t
alarm_set_node (
  __CR_IN__ alarm_t         alert,
  __CR_IN__ leng_t          idx,
  __CR_IN__ const sAL_NODE* node
    )
{
    sALARM*     real;
    sAL_NODE    temp;
    sAL_NODE*   unit;

    real = (sALARM*)alert;
    unit = array_get_unit_safeT(&real->list, sAL_NODE, idx);
    if (unit == NULL)
        return (FALSE);
    struct_cpy(&temp, node, sAL_NODE);
    temp.txtw = str_dupW(node->txtw);
    if (temp.txtw == NULL)
        return (FALSE);
    if (temp.txta != NULL)
        temp.txta = str_dupA(node->txta);
    if (temp.txta == NULL)
        temp.txta = utf16_to_local(real->page, node->txtw);
    if (temp.txta == NULL)
        temp.txta = str_dupA(CR_AS("<invw>"));
    node_free(unit);
    struct_cpy(unit, &temp, sAL_NODE);
    return (TRUE);
}

/*
---------------------------------------
    保存一个双字数据
---------------------------------------
*/
static bool_t
alarm_save_value (
  __CR_IN__ fbuf_t  file,
  __CR_IO__ int32u* hash,
  __CR_IN__ int32u  value
    )
{
    /* 写入数据并计算校验 */
    if (!file_buf_putd(value, file))
        return (FALSE);
    *hash = hash_crc32i_update(*hash, &value, sizeof(int32u));
    return (TRUE);
}

/*
=======================================
    保存报警标志参数
=======================================
*/
CR_API bool_t
alarm_flags_save (
  __CR_IN__ alarm_t         alert,
  __CR_IN__ const ansi_t*   name
    )
{
    leng_t      idx;
    leng_t      cnt;
    int32u      hash;
    int32u      temp;
    fbuf_t      file;
    sALARM*     real;
    sAL_NODE*   pntr;

    /* 保存文件头 + 节点个数 */
    file = file_buf_openA(name, CR_FO_WO);
    if (file == NULL)
        return (FALSE);
    temp = mk_tag4("ERRS");
    hash = hash_crc32i_init();
    if (!alarm_save_value(file, &hash, temp))
        goto _failure;
    real = (sALARM*)alert;
    cnt  = array_get_sizeT(&real->list, sAL_NODE);
    pntr = array_get_dataT(&real->list, sAL_NODE);
    temp = DWORD_BE((int32u)cnt);
    if (!alarm_save_value(file, &hash, temp))
        goto _failure;

    /* 保存逐个节点的属性数据 */
    for (idx = 0; idx < cnt; idx++, pntr++) {
        temp = DWORD_BE(pntr->id);
        if (!alarm_save_value(file, &hash, temp))
            goto _failure;
        temp = DWORD_BE(pntr->attr1);
        if (!alarm_save_value(file, &hash, temp))
            goto _failure;
        temp = DWORD_BE(pntr->rv[0]);
        if (!alarm_save_value(file, &hash, temp))
            goto _failure;
        temp = DWORD_BE(pntr->rv[1]);
        if (!alarm_save_value(file, &hash, temp))
            goto _failure;
    }

    /* 输出数据的 CRC-32 校验 */
    hash = hash_crc32i_finish(hash);
    temp = DWORD_BE(hash);
    if (!alarm_save_value(file, &hash, temp))
        goto _failure;
    file_buf_close(file);
    return (TRUE);

_failure:
    file_buf_close(file);
    file_deleteA(name);
    return (FALSE);
}

/* 内部使用的结构 */
#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (push, 1)
#endif

/* 数据项结构 */
CR_TYPEDEF struct
{
/*000*/ int32u  id;     /* 报警编号 (BE) */
/*004*/ int32u  attr1;  /* 报警属性 (BE) */
/*008*/ int32u  rv[2];  /* 保留字段 (BE) */
/*016*/
} CR_PACKED sERRSAVE;

/* 文件的结构 */
CR_TYPEDEF struct
{
        int32u      magic;  /* 标志 "ERRS" */
        int32u      count;  /* 数据个数 (BE) */
#if 0
        sERRSAVE    item[count];
#endif
        int32u      crc32;  /* 文件校验 (BE) */

} CR_PACKED sERRFILE;

#ifndef _CR_NO_PRAGMA_PACK_
    #pragma pack (pop)
#endif

/*
=======================================
    加载报警标志参数
=======================================
*/
CR_API bool_t
alarm_flags_load (
  __CR_IN__ alarm_t         alert,
  __CR_IN__ const ansi_t*   name
    )
{
    leng_t      idx;
    leng_t      cnt;
    int32u      hash;
    int32u      temp;
    byte_t*     data;
    sALARM*     real;
    fsize_t     fsze;
    sERRSAVE*   pntr;
    sAL_NODE*   setx;
    sAL_NODE**  unit;

    /* 过滤文件大小 */
    fsze = file_sizeA(name);
    if (fsze == (fsize_t)-1 || fsze < sizeof(sERRFILE))
        return (FALSE);
    fsze -= sizeof(sERRFILE);
    if (fsze % sizeof(sERRSAVE) != 0)
        return (FALSE);
    fsze /= sizeof(sERRSAVE);
    data = (byte_t*)file_load_as_binA(name, &idx);
    if (data == NULL)
        return (FALSE);
    temp = ((int32u*)data)[0];
    if (temp != mk_tag4("ERRS"))
        goto _failure;
    temp = ((int32u*)data)[1];
    cnt = (leng_t)DWORD_BE(temp);
    if (fsze != (fsize_t)cnt)
        goto _failure;

    /* 校验文件 CRC-32 值 */
    idx -= sizeof(int32u);
    hash = hash_crc32i_total(data, idx);
    hash = DWORD_BE(hash);
    if (hash != *(int32u*)(data + idx))
        goto _failure;

    /* 加载所有用到的属性值 */
    real = (sALARM*)alert;
    pntr = (sERRSAVE*)(data + sizeof(sERRFILE) - sizeof(int32u));
    for (idx = 0; idx < cnt; idx++, pntr++)
    {
        /* 没找到则继续 */
        temp = DWORD_BE(pntr->id);
        unit = curbead_findT(&real->find, sAL_NODE*, &temp);
        if (unit == NULL)
            continue;
        setx = unit[0];
        setx->rv[0]  = DWORD_BE(pntr->rv[0]);
        setx->rv[1]  = DWORD_BE(pntr->rv[1]);
        setx->attr1 |= DWORD_BE(pntr->attr1);
    }
    mem_free(data);
    return (TRUE);

_failure:
    mem_free(data);
    return (FALSE);
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
