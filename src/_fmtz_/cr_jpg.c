/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2013-08-30  */
/*     #######          ###    ###      [FMTZ]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack JPG 图片读取函数库 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "fmtz.h"
#include "tjpgdec/tjpgd.h"

/* 用户定义设备结构 */
typedef struct
{
        iDATIN* datin;  /* 文件读取对象 */
        sIMAGE* image;  /* 图片输出对象 */

} sIODEV;

/*
---------------------------------------
    用户定义输入函数
---------------------------------------
*/
static unsigned int
jpg_in_func (
  __CR_IO__ JDEC*           jd,
  __CR_OT__ uint8_t*        buff,
  __CR_IN__ unsigned int    nbyte
    )
{
    iDATIN* datin;

    datin = ((sIODEV*)jd->device)->datin;
    if (buff != NULL)
        return ((uint16_t)(CR_VCALL(datin)->read(datin, buff, nbyte)));

    if (!CR_VCALL(datin)->seek(datin, nbyte, SEEK_CUR))
        return (0);
    return (nbyte);
}

/*
---------------------------------------
    用户定义输出函数
---------------------------------------
*/
static int
jpg_out_func (
  __CR_IO__ JDEC*   jd,
  __CR_IN__ void*   bitmap,
  __CR_IN__ JRECT*  rect
    )
{
    sIMAGE*     img;
    uint8_t*    src;
    uint8_t*    dst;
    uint16_t    yyy;
    uint32_t    idx;
    uint32_t    bws;

    src = (uint8_t*)bitmap;
    img = ((sIODEV*)jd->device)->image;
    dst = pixel_addr3(img, rect->left, rect->top);
    bws = 3UL * (rect->right - rect->left + 1);
    for (yyy = rect->top; yyy <= rect->bottom; yyy++) {
        for (idx = 0; idx < bws; idx += 3) {
            dst[idx + 0] = src[idx + 2];
            dst[idx + 1] = src[idx + 1];
            dst[idx + 2] = src[idx + 0];
        }
        src += bws;
        dst += img->bpl;
    }
    return (1);
}

/*
=======================================
    JPG 文件读取
=======================================
*/
CR_API sFMT_PIC*
load_cr_jpg (
  __CR_IO__ iDATIN*         datin,
  __CR_IN__ const sLOADER*  param
    )
{
    JDEC    jdec;
    sIODEV  devid;
    byte_t  head[3100];
    /* ------------- */
    JRESULT     retc;
    sFMT_PIC*   rett;
    sFMT_FRAME  temp;

    /* 这个参数可能为空 */
    if (datin == NULL)
        return (NULL);

    /* 读取 & 检查头部 */
    if (CR_VCALL(datin)->read(datin, head, 11) != 11)
        return (NULL);
    if (mem_cmp2(head, "\xFF\xD8\xFF\xE0**JFIF", 11) != 0)
        return (NULL);
    if (!CR_VCALL(datin)->rewind(datin))
        return (NULL);

    /* 准备解压图片 */
    devid.datin = datin;
    retc = jd_prepare(&jdec, jpg_in_func, head, sizeof(head), &devid);
    if (retc != JDR_OK)
        return (NULL);

    /* 生成图片对象 */
    mem_zero(temp.wh, sizeof(temp.wh));
    temp.fmt = CR_PIC_ARGB;
    temp.bpp = 24;
    temp.clr = "BGR";
    temp.wh[0] = 8;
    temp.wh[1] = 8;
    temp.wh[2] = 8;
    temp.pic = image_new(0, 0, jdec.width, jdec.height,
                         CR_ARGB888, FALSE, 4);
    if (temp.pic == NULL)
        return (NULL);

    /* 读取图片数据 */
    devid.image = temp.pic;
    retc = jd_decomp(&jdec, jpg_out_func, 0);
    if (retc != JDR_OK)
        goto _failure;

    /* 返回读取的文件数据 */
    rett = struct_new(sFMT_PIC);
    if (rett == NULL)
        goto _failure;
    rett->frame = struct_dup(&temp, sFMT_FRAME);
    if (rett->frame == NULL) {
        mem_free(rett);
        goto _failure;
    }
    CR_NOUSE(param);
    rett->type = CR_FMTZ_PIC;
    rett->count = 1;
    rett->infor = "Independent JPEG Group (JPG)";
    return (rett);

_failure:
    image_del(temp.pic);
    return (NULL);
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
