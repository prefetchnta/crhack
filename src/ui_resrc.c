/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2012-03-21  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack 界面资源加载函数库 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "egui.h"
#include "hash.h"
#include "parser.h"
#include "pixels.h"
#include "strlib.h"

/* 资源单元结构 */
typedef struct
{
        ansi_t* name;   /* 资源名称 */
        sFMTZ*  resrc;  /* 资源数据 */
        void_t* fdata;  /* 附加数据 */

} sEGUI_RES;

/* 资源表内部结构 */
typedef struct
{
        uint_t      cdpage;     /* 编码值 */
        ansi_t*     ldroot;     /* 根目录 */
        sCURBEAD    resrcs;     /* 资源表 */
} sEGUI;

/*
---------------------------------------
    资源对象释放
---------------------------------------
*/
static void_t
egui_res_kill (
  __CR_IN__ void_t* obj
    )
{
    sEGUI_RES*  unit;

    unit = (sEGUI_RES*)obj;
    fmtz_free(unit->resrc);
    TRY_FREE(unit->fdata);
    mem_free(unit->name);
}

/*
---------------------------------------
    资源对象查找
---------------------------------------
*/
static uint_t
egui_res_find (
  __CR_IN__ const void_t*   key
    )
{
    return (hash_str_djb2(key, str_lenA((ansi_t*)key)));
}

/*
---------------------------------------
    资源对象比较
---------------------------------------
*/
static bool_t
egui_res_comp (
  __CR_IN__ const void_t*   key,
  __CR_IN__ const void_t*   obj
    )
{
    sEGUI_RES*  unit = (sEGUI_RES*)obj;

    return (str_cmpA(unit->name, (ansi_t*)key) == 0);
}

/*
=======================================
    建立资源表
=======================================
*/
CR_API egui_t
egui_res_init (
  __CR_IN__ leng_t  count
    )
{
    sEGUI*  rett;

    /* 生成资源表对象 */
    rett = struct_new(sEGUI);
    if (rett == NULL)
        return (NULL);
    struct_zero(rett, sEGUI);

    /* 初始化哈希表 */
    if (!curbead_initT(&rett->resrcs, sEGUI_RES, count)) {
        mem_free(rett);
        return (NULL);
    }
    rett->resrcs.free = egui_res_kill;
    rett->resrcs.find = egui_res_find;
    rett->resrcs.comp = egui_res_comp;
    return ((egui_t)rett);
}

/*
=======================================
    释放资源表
=======================================
*/
CR_API void_t
egui_res_free (
  __CR_IN__ egui_t  egui
    )
{
    sEGUI*  real = (sEGUI*)egui;

    TRY_FREE(real->ldroot);
    curbead_freeT(&real->resrcs, sEGUI_RES);
    mem_free((void_t*)egui);
}

/*
---------------------------------------
    从封包文件内加载文件
---------------------------------------
*/
static bool_t
egui_res_pack_file (
  __CR_IN__ const sEGUI*    real,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ const ansi_t*   fpak,
  __CR_OT__ sLOADER*        ldrs
    )
{
    leng_t      size;
    sBUFFER     buff;
    iPACKAGE*   pack;

    pack = egui_res_get_pak((egui_t)real, fpak);
    if (pack == NULL)
        return (FALSE);
    if (real->cdpage == CR_UTF8) {
        if (!pack_file_dataU(pack, &buff, name, TRUE))
            return (FALSE);
    }
    else {
        if (!pack_file_dataA(pack, &buff, name, real->cdpage, TRUE))
            return (FALSE);
    }
    size = buffer_size(&buff);
    set_ldrM(ldrs, buff.data, size, "", 0, 0);
    return (TRUE);
}

/*
=======================================
    从内存加载资源文件
=======================================
*/
CR_API bool_t
egui_res_load_s (
  __CR_IO__ egui_t          egui,
  __CR_IN__ const ansi_t*   text,
  __CR_IN__ uint_t          type,   /* 图片转换格式 */
  __CR_IN__ engine_init_t   port
    )
{
    sXMLu*      xml;
    leng_t      idx;
    leng_t      len;
    int32u      cnt;
    sEGUI*      real;
    sbin_t      sbin;
    uint_t      rc[4];
    byte_t*     data;
    ansi_t*     str1;
    ansi_t*     str2;
    ansi_t*     full;
    sIMAGE*     frms;
    sIMAGE*     imgs;
    sLOADER     ldrs;
    sENGINE*    load;
    sFMT_DAT*   fdat;
    sFMT_PIC*   fpic;
    sFMT_RCT*   frct;
    sFMT_TXT*   ftxt;
    sEGUI_RES   sres;
    sEGUI_RES*  find;

    /* 解析资源 XML 字符串 */
    xml = xml_parseU(text);
    if (xml == NULL)
        return (FALSE);

    /* 检查首标签 */
    if (xml->nodes[0].name[0] == CR_AC('?'))
        idx = 1;
    else
        idx = 0;
    if (xml->nodes[idx].closed ||
        str_cmpIA(xml->nodes[idx].name, CR_AS("RESRC")) != 0)
        goto _failure1;

    if (port == NULL)
    {
        /* 加载引擎插件 (文件名 & 接口名可选) */
        str1 = xml_attr_stringU(CR_AS("sbin"), &xml->nodes[idx]);
        if (str1 == NULL)
            goto _failure1;
        sbin = sbin_loadA(str1);
        mem_free(str1);
        if (sbin == NULL)
            goto _failure1;
        str1 = xml_attr_stringU(CR_AS("port"), &xml->nodes[idx]);
        if (str1 == NULL) {
            port = sbin_exportT(sbin, "engine_get", engine_init_t);
        }
        else {
            port = sbin_exportT(sbin, str1, engine_init_t);
            mem_free(str1);
        }
        if (port == NULL)
            goto _failure2;
    }
    else
    {
        sbin = NULL;    /* 使用静态的引擎插件 */
    }

    /* 生成引擎对象 */
    if ((load = port()) == NULL)
        goto _failure2;
    real = (sEGUI*)egui;

    /* 逐个解析标签 */
    for (idx++; idx < xml->count; idx++)
    {
        /* 处理一些特殊的标签 */
        if (str_cmpIA(xml->nodes[idx].name, CR_AS("/RESRC")) == 0)
            break;
        if (!xml->nodes[idx].closed) {
            idx = xml_skip_tagU(xml->nodes, idx, xml->count) - 1;
            continue;
        }

        /* 名称, 必须要的属性, 没有直接过之 */
        str1 = xml_attr_stringU(CR_AS("name"), &xml->nodes[idx]);
        if (str1 == NULL)
            continue;
        struct_zero(&sres, sEGUI_RES);

        /* 解析对象标签 (出错也要继续) */
        if (str_cmpIA(xml->nodes[idx].name, CR_AS("LDR")) == 0)
        {
            /* 引擎加载 */
            str2 = xml_attr_stringU(CR_AS("file"), &xml->nodes[idx]);
            if (str2 == NULL) {
                mem_free(str1);
                continue;
            }

            /* 包内文件 */
            full = xml_attr_stringU(CR_AS("pack"), &xml->nodes[idx]);
            if (full != NULL) {
                if (!egui_res_pack_file(real, str2, full, &ldrs)) {
                    mem_free(full);
                    mem_free(str2);
                    mem_free(str1);
                    continue;
                }
                /* 保存一份内存数据备用 */
                sres.fdata = (void_t*)ldrs.buff.data;
                mem_free(full);
            }
            else {
                if (real->ldroot != NULL) {
                    full = path_appendA(real->ldroot, str2);
                    mem_free(str2);
                    if (full == NULL) {
                        mem_free(str1);
                        continue;
                    }
                    str2 = full;
                }
                set_ldrA(&ldrs, str2, "", 0, 0);
            }

            /* 附加参数 */
            full = xml_attr_stringU(CR_AS("param"), &xml->nodes[idx]);
            if (full != NULL)
                ldrs.aprm = (void_t*)full;

            /* 生成对象 */
            sres.name  = str1;
            sres.resrc = engine_do(load, &ldrs);
            mem_free(str2);
            if (ldrs.aprm == (void_t*)full)
                mem_free(full);
            if (sres.resrc == NULL) {
                TRY_FREE(sres.fdata);
                mem_free(str1);
                continue;
            }

            /* 如果不是封包文件就释放加载的内存数据 */
            if (sres.fdata != NULL) {
                if (fmtz_get_pack(sres.resrc) == NULL) {
                    mem_free(sres.fdata);
                    sres.fdata = NULL;
                }
            }

            /* 转换格式 */
            if (type != CR_UNKNOWN &&
                sres.resrc->type == CR_FMTZ_PIC)
            {
                /* 图片转换到统一指定的格式 */
                fpic = (sFMT_PIC*)sres.resrc;
                for (cnt = 0; cnt < fpic->count; cnt++)
                {
                    /* 逐帧转换格式 */
                    frms = fpic->frame[cnt].pic;
                    if (frms->fmt == type)
                        continue;
                    imgs = image_new(frms->position.x1, frms->position.y1,
                                     frms->position.ww, frms->position.hh,
                                     type, frms->gdi, frms->align);
                    if (imgs == NULL)
                        goto _failure3;

                    /* 转换图片格式 */
                    if (img_auto_to_xx(imgs, frms) == NULL) {
                        image_del(imgs);
                        goto _failure3;
                    }

                    /* 替换原来图片 */
                    image_del(frms);
                    fpic->frame[cnt].pic = imgs;
                }
            }
        }
        else
        if (str_cmpIA(xml->nodes[idx].name, CR_AS("TXT")) == 0)
        {
            /* 加载文本 */
            str2 = xml_attr_stringU(CR_AS("text"), &xml->nodes[idx]);
            if (str2 == NULL) {
                mem_free(str1);
                continue;
            }

            /* 是否为路径串 */
            if (real->ldroot != NULL &&
                xml_attr_intxU(CR_AS("path"), FALSE, &xml->nodes[idx])) {
                full = path_appendA(real->ldroot, str2);
                mem_free(str2);
                if (full == NULL) {
                    mem_free(str1);
                    continue;
                }
                str2 = full;
            }

            /* 生成对象 */
            ftxt = struct_new(sFMT_TXT);
            if (ftxt == NULL) {
                mem_free(str2);
                mem_free(str1);
                continue;
            }
            sres.name   = str1;
            sres.resrc  = (sFMTZ*)ftxt;
            ftxt->type  = CR_FMTZ_TXT;
            ftxt->size  = str_sizeA(str2);
            ftxt->text  = (byte_t*)str2;
            ftxt->infor = "eGUI Resource String";
        }
        else
        if (str_cmpIA(xml->nodes[idx].name, CR_AS("SPR")) == 0)
        {
            /* 加载精灵 */
            str2 = xml_attr_stringU(CR_AS("rect"), &xml->nodes[idx]);
            if (str2 == NULL) {
                mem_free(str1);
                continue;
            }

            /* 解析数据 */
            if (str2lstA(rc, 4, str2, "[],") == NULL) {
                mem_free(str2);
                mem_free(str1);
                continue;
            }
            mem_free(str2);
            str2 = xml_attr_stringU(CR_AS("pict"), &xml->nodes[idx]);
            if (str2 == NULL) {
                mem_free(str1);
                continue;
            }
            cnt = xml_attr_intx32U(CR_AS("index"), 0UL, &xml->nodes[idx]);

            /* 查找图片对象 */
            find = curbead_findT(&real->resrcs, sEGUI_RES, str2);
            if (find == NULL) {
                mem_free(str2);
                mem_free(str1);
                continue;
            }
            imgs = fmtz_get_imgo(find->resrc, cnt);
            if (imgs == NULL) {
                mem_free(str2);
                mem_free(str1);
                continue;
            }

            /* 生成对象 */
            frct = struct_new(sFMT_RCT);
            if (frct == NULL) {
                mem_free(str2);
                mem_free(str1);
                continue;
            }
            sres.name   = str1;
            sres.resrc  = (sFMTZ*)frct;
            frct->type  = CR_FMTZ_RCT;
            frct->pict  = str2;
            frct->index = cnt;
            frct->image = imgs;
            frct->infor = "eGUI Resource Sprite";
            rect_set_wh(&frct->rect, rc[0], rc[1], rc[2], rc[3]);
        }
        else
        if (str_cmpIA(xml->nodes[idx].name, CR_AS("RCT")) == 0)
        {
            /* 加载区域 */
            str2 = xml_attr_stringU(CR_AS("rect"), &xml->nodes[idx]);
            if (str2 == NULL) {
                mem_free(str1);
                continue;
            }

            /* 解析数据 */
            if (str2lstA(rc, 4, str2, "[],") == NULL) {
                mem_free(str2);
                mem_free(str1);
                continue;
            }
            mem_free(str2);

            /* 生成对象 */
            frct = struct_new(sFMT_RCT);
            if (frct == NULL) {
                mem_free(str1);
                continue;
            }
            sres.name   = str1;
            sres.resrc  = (sFMTZ*)frct;
            frct->type  = CR_FMTZ_RCT;
            frct->pict  = NULL;
            frct->index = 0UL;
            frct->image = NULL;
            frct->infor = "eGUI Resource Rectangle";
            rect_set_wh(&frct->rect, rc[0], rc[1], rc[2], rc[3]);
        }
        else
        if (str_cmpIA(xml->nodes[idx].name, CR_AS("BIN")) == 0)
        {
            /* 加载数据 */
            str2 = xml_attr_stringU(CR_AS("data"), &xml->nodes[idx]);
            if (str2 == NULL) {
                mem_free(str1);
                continue;
            }

            /* 解析数据 */
            len = (str_lenA(str2) + 1) / 2;
            data = mem_malloc(len);
            if (data == NULL) {
                mem_free(str2);
                mem_free(str1);
                continue;
            }
            str2datA(data, &len, str2);
            mem_free(str2);

            /* 生成对象 */
            fdat = struct_new(sFMT_DAT);
            if (fdat == NULL) {
                mem_free(data);
                mem_free(str1);
                continue;
            }
            sres.name   = str1;
            sres.resrc  = (sFMTZ*)fdat;
            fdat->type  = CR_FMTZ_NOP;
            fdat->unsz  = len;
            fdat->pksz  = len;
            fdat->data  = data;
            fdat->infor = "eGUI Resource Binary Data";
        }
        else
        if (str_cmpIA(xml->nodes[idx].name, CR_AS("DAT")) == 0)
        {
            /* 加载文件 */
            str2 = xml_attr_stringU(CR_AS("file"), &xml->nodes[idx]);
            if (str2 == NULL) {
                mem_free(str1);
                continue;
            }

            /* 包内文件 */
            full = xml_attr_stringU(CR_AS("pack"), &xml->nodes[idx]);
            if (full != NULL) {
                if (!egui_res_pack_file(real, str2, full, &ldrs)) {
                    mem_free(full);
                    mem_free(str2);
                    mem_free(str1);
                    continue;
                }
                len  = (leng_t )ldrs.buff.size;
                data = (void_t*)ldrs.buff.data;
                mem_free(full);
                mem_free(str2);
            }
            else
            {
                /* 是否为相对路径 */
                if (real->ldroot != NULL &&
                    !xml_attr_intxU(CR_AS("fabs"), FALSE, &xml->nodes[idx])) {
                    full = path_appendA(real->ldroot, str2);
                    mem_free(str2);
                    if (full == NULL) {
                        mem_free(str1);
                        continue;
                    }
                    str2 = full;
                }

                /* 加载文件 */
                data = file_load_as_binA(str2, &len);
                mem_free(str2);
                if (data == NULL) {
                    mem_free(str1);
                    continue;
                }
            }

            /* 生成对象 */
            fdat = struct_new(sFMT_DAT);
            if (fdat == NULL) {
                mem_free(data);
                mem_free(str1);
                continue;
            }
            sres.name   = str1;
            sres.resrc  = (sFMTZ*)fdat;
            fdat->type  = CR_FMTZ_NOP;
            fdat->unsz  = len;
            fdat->pksz  = len;
            fdat->data  = data;
            fdat->infor = "eGUI Resource File Data";
        }
        else
        {
            /* 未知标签 */
            mem_free(str1);
            continue;
        }

        /* 插入哈希表 (失败直接退出) */
        if (curbead_insertT(&real->resrcs, sEGUI_RES,
                            sres.name, &sres) == NULL)
            goto _failure3;
    }
    load->sbin = sbin;
    engine_free(load);
    xml_closeU(xml);
    return (TRUE);

_failure3:
    egui_res_kill(&sres);
    engine_free(load);
_failure2:
    if (sbin != NULL)
        sbin_unload(sbin);
_failure1:
    xml_closeU(xml);
    return (FALSE);
}

/*
=======================================
    从文件加载资源文件
=======================================
*/
CR_API bool_t
egui_res_load_f (
  __CR_IO__ egui_t          egui,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ uint_t          type,   /* 图片转换格式 */
  __CR_IN__ engine_init_t   port
    )
{
    bool_t  rett;
    ansi_t* text;

    text = file_load_as_strA(name);
    if (text == NULL)
        return (FALSE);
    rett = egui_res_load_s(egui, text, type, port);
    mem_free(text);
    return (rett);
}

/*
=======================================
    设置加载文件根目录
=======================================
*/
CR_API bool_t
egui_res_set_root (
  __CR_IO__ egui_t          egui,
  __CR_IN__ const ansi_t*   root
    )
{
    sEGUI*  real;

    real = (sEGUI*)egui;
    SAFE_FREE(real->ldroot);
    if (root != NULL) {
        real->ldroot = str_dupA(root);
        if (real->ldroot == NULL)
            return (FALSE);
    }
    return (TRUE);
}

/*
=======================================
    设置资源文件的编码
=======================================
*/
CR_API void_t
egui_res_set_cpage (
  __CR_IO__ egui_t  egui,
  __CR_IN__ uint_t  cpage
    )
{
    ((sEGUI*)egui)->cdpage = cpage;
}

/*
=======================================
    查找资源对象
=======================================
*/
CR_API sFMTZ*
egui_res_get (
  __CR_IN__ egui_t          egui,
  __CR_IN__ const ansi_t*   name
    )
{
    sEGUI*      real;
    sEGUI_RES*  find;

    real = (sEGUI*)egui;
    find = curbead_findT(&real->resrcs, sEGUI_RES, name);
    if (find == NULL)
        return (NULL);
    return (find->resrc);
}

/*
=======================================
    注册资源对象
=======================================
*/
CR_API bool_t
egui_res_set (
  __CR_IN__ egui_t          egui,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ sFMTZ*          resz
    )
{
    sEGUI*      real;
    sEGUI_RES   temp;

    real = (sEGUI*)egui;
    temp.resrc = resz;  /* 注意: resz 必须是可释放的 */
    temp.name = str_dupA(name);
    if (temp.name == NULL)
        return (FALSE);
    if (curbead_insertT(&real->resrcs, sEGUI_RES,
                        temp.name, &temp) == NULL) {
        mem_free(temp.name);
        return (FALSE);
    }
    return (TRUE);
}

/*
=======================================
    查找数据对象
=======================================
*/
CR_API void_t*
egui_res_get_dat (
  __CR_IN__ egui_t          egui,
  __CR_IN__ const ansi_t*   name,
  __CR_OT__ leng_t*         size
    )
{
    const sFMTZ*    fmtz;

    fmtz = egui_res_get(egui, name);
    if (fmtz == NULL)
        return (NULL);
    if (fmtz->type != CR_FMTZ_NOP &&
        fmtz->type != CR_FMTZ_DEC)
        return (NULL);
    if (size != NULL)
        *size = ((sFMT_DAT*)fmtz)->unsz;
    return (((sFMT_DAT*)fmtz)->data);
}

/*
=======================================
    查找图片对象
=======================================
*/
CR_API sIMAGE*
egui_res_get_img (
  __CR_IN__ egui_t          egui,
  __CR_IN__ const ansi_t*   name,
  __CR_IN__ int32u          index,
  __CR_OT__ sRECT*          rect
    )
{
    sIMAGE*         imgs;
    const sFMTZ*    fmtz;

    fmtz = egui_res_get(egui, name);
    if (fmtz == NULL)
        return (NULL);
    if (fmtz->type == CR_FMTZ_PIC) {
        imgs = fmtz_get_imgo(fmtz, index);
        if (imgs == NULL)
            return (NULL);
        if (rect != NULL)
            struct_cpy(rect, &imgs->clip_win, sRECT);
        return (imgs);
    }
    if (fmtz->type == CR_FMTZ_RCT &&
        ((sFMT_RCT*)fmtz)->image != NULL) {
        imgs = ((sFMT_RCT*)fmtz)->image;
        if (rect != NULL)
            struct_cpy(rect, &(((sFMT_RCT*)fmtz)->rect), sRECT);
        return (imgs);
    }
    return (NULL);
}

/*
=======================================
    查找区域对象
=======================================
*/
CR_API sRECT*
egui_res_get_rct (
  __CR_IN__ egui_t          egui,
  __CR_IN__ const ansi_t*   name
    )
{
    const sFMTZ*    fmtz;

    fmtz = egui_res_get(egui, name);
    if (fmtz == NULL)
        return (NULL);
    if (fmtz->type != CR_FMTZ_RCT)
        return (NULL);
    return (&(((sFMT_RCT*)fmtz)->rect));
}

/*
=======================================
    查找文本对象
=======================================
*/
CR_API ansi_t*
egui_res_get_txt (
  __CR_IN__ egui_t          egui,
  __CR_IN__ const ansi_t*   name
    )
{
    const sFMTZ*    fmtz;

    fmtz = egui_res_get(egui, name);
    if (fmtz == NULL)
        return (NULL);
    if (fmtz->type != CR_FMTZ_TXT)
        return (NULL);
    return ((ansi_t*)(((sFMT_TXT*)fmtz)->text));
}

/*
=======================================
    查找动画对象
=======================================
*/
CR_API iPICTURE*
egui_res_get_anm (
  __CR_IN__ egui_t          egui,
  __CR_IN__ const ansi_t*   name
    )
{
    const sFMTZ*    fmtz;

    fmtz = egui_res_get(egui, name);
    if (fmtz == NULL)
        return (NULL);
    return (fmtz_get_pict(fmtz));
}

/*
=======================================
    查找读包对象
=======================================
*/
CR_API iPACKAGE*
egui_res_get_pak (
  __CR_IN__ egui_t          egui,
  __CR_IN__ const ansi_t*   name
    )
{
    const sFMTZ*    fmtz;

    fmtz = egui_res_get(egui, name);
    if (fmtz == NULL)
        return (NULL);
    return (fmtz_get_pack(fmtz));
}

/*
=======================================
    查找媒体对象
=======================================
*/
CR_API iXMMEDIA*
egui_res_get_xmm (
  __CR_IN__ egui_t          egui,
  __CR_IN__ const ansi_t*   name
    )
{
    const sFMTZ*    fmtz;

    fmtz = egui_res_get(egui, name);
    if (fmtz == NULL)
        return (NULL);
    return (fmtz_get_xmms(fmtz));
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
