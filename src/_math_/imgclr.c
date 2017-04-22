/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2013-04-17  */
/*     #######          ###    ###      [MATH]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 彩色图片函数库 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "blit.h"
#include "memlib.h"
#include "phylib.h"
#include "pixels.h"

#ifndef _CR_NO_STDC_
    #include <math.h>
#endif

/*****************************************************************************/
/*                                 颜色转换                                  */
/*****************************************************************************/

/*
=======================================
    RGB to HSL
=======================================
*/
CR_API void_t
bgr2hsl (
  __CR_OT__ sint_t          hsl[3],
  __CR_IN__ const byte_t    bgr[3]
    )
{
    sint_t  dlt;
    sint_t  bb = bgr[0];
    sint_t  gg = bgr[1];
    sint_t  rr = bgr[2];
    sint_t  max = bb;
    sint_t  min = bb;

    if (max < gg) max = gg;
    if (min > gg) min = gg;
    if (max < rr) max = rr;
    if (min > rr) min = rr;
    dlt = max - min;

    /* 分量-H */
    if (dlt == 0) {
        hsl[0] = 0;
    }
    else
    if (max == rr) {
        hsl[0] = (60 * (gg - bb)) / dlt;
        if (gg < bb)
            hsl[0] += 360;
    }
    else
    if (max == gg) {
        hsl[0] = (60 * (bb - rr)) / dlt + 120;
    }
    else {
        hsl[0] = (60 * (rr - gg)) / dlt + 240;
    }

    /* 分量-L */
    hsl[2] = (min + max) / 2;
    hsl[2] += hsl[2] / 128;

    /* 分量-S */
    if (hsl[2] == 0 || dlt == 0) {
        hsl[1] = 0;
    }
    else
    if (hsl[2] <= 128) {
        hsl[1] = (128 * dlt) / hsl[2];
    }
    else {
        hsl[1] = (128 * dlt) / (256 - hsl[2]);
    }
}

/*
=======================================
    RGB to HSV
=======================================
*/
CR_API void_t
bgr2hsv (
  __CR_OT__ sint_t          hsv[3],
  __CR_IN__ const byte_t    bgr[3]
    )
{
    sint_t  dlt;
    sint_t  bb = bgr[0];
    sint_t  gg = bgr[1];
    sint_t  rr = bgr[2];
    sint_t  max = bb;
    sint_t  min = bb;

    if (max < gg) max = gg;
    if (min > gg) min = gg;
    if (max < rr) max = rr;
    if (min > rr) min = rr;
    dlt = max - min;

    /* 分量-H */
    if (dlt == 0) {
        hsv[0] = 0;
    }
    else
    if (max == rr) {
        hsv[0] = (60 * (gg - bb)) / dlt;
        if (gg < bb)
            hsv[0] += 360;
    }
    else
    if (max == gg) {
        hsv[0] = (60 * (bb - rr)) / dlt + 120;
    }
    else {
        hsv[0] = (60 * (rr - gg)) / dlt + 240;
    }

    /* 分量-S */
    if (max == 0) {
        hsv[1] = 0;
    }
    else {
        hsv[1] = (256 * dlt) / max;
    }

    /* 分量-V */
    hsv[2] = max;
    hsv[2] += hsv[2] / 128;
}

/*
=======================================
    RGB to YCbCr
=======================================
*/
CR_API void_t
bgr2yuv (
  __CR_OT__ byte_t          yuv[3],
  __CR_IN__ const byte_t    bgr[3]
    )
{
    int32u  ib, ig, ir;
    int32u  bb = bgr[0];
    int32u  gg = bgr[1];
    int32u  rr = bgr[2];

    /* 分量-Y */
    ir = rr * 299UL;
    ig = gg * 587UL;
    ib = bb * 114UL;
    ir += ig + ib + 500UL;
    ir /= 1000UL;
    yuv[0] = (byte_t)ir;

    /* 分量-Cb */
    ir = rr * 1687UL;
    ig = gg * 3313UL;
    ib = bb * 5000UL;
    ib += 1280000UL;
    ib -= ig + ir;
    ib /= 10000UL;
    yuv[1] = (byte_t)ib;

    /* 分量-Cr */
    ir = rr * 5000UL;
    ig = gg * 4187UL;
    ib = bb *  813UL;
    ir += 1280000UL;
    ir -= ig + ib;
    ir /= 10000UL;
    yuv[2] = (byte_t)ir;
}

/*****************************************************************************/
/*                                 颜色区分                                  */
/*****************************************************************************/

/* 颜色常数表 */
static const byte_t _rom_ s_cstep_pal[60] =
{
    0x7F, 0x7F, 0x7F, 0xFF, /* 无效 */
    0x00, 0x00, 0x00, 0xFF, /* 黑色 */
    0xFF, 0xFF, 0xFF, 0xFF, /* 白色 */
    0x00, 0x00, 0xFF, 0xFF, /* 红色 */
    0x00, 0x80, 0xFF, 0xFF, /* 橙色 */
    0x00, 0xFF, 0xFF, 0xFF, /* 黄色 */
    0x00, 0xFF, 0x80, 0xFF, /* 黄绿 */
    0x00, 0xFF, 0x00, 0xFF, /* 绿色 */
    0x80, 0xFF, 0x00, 0xFF, /* 青绿 */
    0xFF, 0xFF, 0x00, 0xFF, /* 青色 */
    0xFF, 0x80, 0x00, 0xFF, /* 青蓝 */
    0xFF, 0x00, 0x00, 0xFF, /* 蓝色 */
    0xFF, 0x00, 0x80, 0xFF, /* 蓝紫 */
    0xFF, 0x00, 0xFF, 0xFF, /* 紫色 */
    0x80, 0x00, 0xFF, 0xFF, /* 紫红 */
};

/*
=======================================
    返回颜色常数表
=======================================
*/
CR_API void_t
color_step_pal (
  __CR_OT__ int32u* pal,
  __CR_IN__ uint_t  count
    )
{
    if (count > 15 || count == 0)
        count = 15;
    mem_cpy(pal, s_cstep_pal, count * 4);
}

/* 内部使用的结构 */
typedef struct
{
        sint_t  steps[12];  /* 颜色角度表 */
        byte_t  color[36];  /* 颜色替换表 */

} sCOLOR_STEP;

/*
=======================================
    建立颜色区分对象
=======================================
*/
CR_API cstep_t
color_step_init (void_t)
{
    sCOLOR_STEP*    rett;

    rett = struct_new(sCOLOR_STEP);
    if (rett == NULL)
        return (NULL);

    /* [0 - 15] 红色区 0xFF0000 */
    rett->steps[0] = 15;
    rett->color[0] = 0x00;
    rett->color[1] = 0x00;
    rett->color[2] = 0xFF;

    /* [15 - 45] 橙色区 0xFF8000 */
    rett->steps[1] = 45;
    rett->color[3] = 0x00;
    rett->color[4] = 0x80;
    rett->color[5] = 0xFF;

    /* [45 - 75] 黄色区 0xFFFF00 */
    rett->steps[2] = 75;
    rett->color[6] = 0x00;
    rett->color[7] = 0xFF;
    rett->color[8] = 0xFF;

    /* [75 - 105] 黄绿区 0x80FF00 */
    rett->steps[3] = 105;
    rett->color[9] = 0x00;
    rett->color[10] = 0xFF;
    rett->color[11] = 0x80;

    /* [105 - 135] 绿色区 0x00FF00 */
    rett->steps[4] = 135;
    rett->color[12] = 0x00;
    rett->color[13] = 0xFF;
    rett->color[14] = 0x00;

    /* [135 - 165] 青绿区 0x00FF80 */
    rett->steps[5] = 165;
    rett->color[15] = 0x80;
    rett->color[16] = 0xFF;
    rett->color[17] = 0x00;

    /* [165 - 195] 青色区 0x00FFFF */
    rett->steps[6] = 195;
    rett->color[18] = 0xFF;
    rett->color[19] = 0xFF;
    rett->color[20] = 0x00;

    /* [195 - 225] 青蓝区 0x0080FF */
    rett->steps[7] = 225;
    rett->color[21] = 0xFF;
    rett->color[22] = 0x80;
    rett->color[23] = 0x00;

    /* [225 - 255] 蓝色区 0x0000FF */
    rett->steps[8] = 255;
    rett->color[24] = 0xFF;
    rett->color[25] = 0x00;
    rett->color[26] = 0x00;

    /* [255 - 285] 蓝紫区 0x8000FF */
    rett->steps[9] = 285;
    rett->color[27] = 0xFF;
    rett->color[28] = 0x00;
    rett->color[29] = 0x80;

    /* [285 - 315] 紫色区 0xFF00FF */
    rett->steps[10] = 315;
    rett->color[30] = 0xFF;
    rett->color[31] = 0x00;
    rett->color[32] = 0xFF;

    /* [315 - 345] 紫红区 0xFF0080 */
    rett->steps[11] = 345;
    rett->color[33] = 0x80;
    rett->color[34] = 0x00;
    rett->color[35] = 0xFF;

    /* [345 - 360] 红色区 0xFF0000 */
    return ((cstep_t)rett);
}

/*
=======================================
    释放颜色区分对象
=======================================
*/
CR_API void_t
color_step_kill (
  __CR_IN__ cstep_t cstep
    )
{
    mem_free(cstep);
}

/*
=======================================
    设置颜色区分参数
=======================================
*/
CR_API void_t
color_step_set (
  __CR_IO__ cstep_t         cstep,
  __CR_IN__ const sint_t    steps[12],
  __CR_IN__ const byte_t    color[36]
    )
{
    sCOLOR_STEP*    real;

    real = (sCOLOR_STEP*)cstep;
    if (steps != NULL)
        mem_cpy(real->steps, steps, 12 * sizeof(sint_t));
    if (color != NULL)
        mem_cpy(real->color, color, 36 * sizeof(byte_t));
}

/*
=======================================
    颜色区分主色的角度偏移
=======================================
*/
CR_API void_t
color_step_bias (
  __CR_IO__ cstep_t cstep,
  __CR_IN__ sint_t  bias
    )
{
    ufast_t         idx;
    sCOLOR_STEP*    real;

    real = (sCOLOR_STEP*)cstep;
    for (idx = 0; idx < 12; idx++) {
        real->steps[idx] += bias;
        bias = -bias;
    }
}

/*
=======================================
    颜色区分计算 (输入 HUE 分量)
=======================================
*/
CR_API byte_t
color_step_do (
  __CR_IN__ cstep_t cstep,
  __CR_OT__ byte_t  dst[3],
  __CR_IN__ sint_t  hue
    )
{
    ufast_t         clr;
    ufast_t         idx;
    sCOLOR_STEP*    real;

    real = (sCOLOR_STEP*)cstep;
    for (idx = 0; idx < 12; idx++) {
        if (hue <= real->steps[idx])
            break;
    }
    if (idx == 12)
        idx = 0;
    clr = idx + 3;
    if (dst != NULL) {
        idx *= 3;
        dst[0] = real->color[idx + 0];
        dst[1] = real->color[idx + 1];
        dst[2] = real->color[idx + 2];
    }
    return ((byte_t)clr);
}

/*****************************************************************************/
/*                                 图片转换                                  */
/*****************************************************************************/

/*
=======================================
    转换到 HSL 图
=======================================
*/
CR_API bool_t
image_to_hsl (
  __CR_IO__ const sIMAGE*   img
    )
{
    uint_t  xx;
    uint_t  ww;
    uint_t  hh;
    byte_t* ptr;
    byte_t* line;
    int32u  temp;
    sint_t  hsl[3];

    /* 只支持图像处理的真彩格式 */
    if (img->fmt != CR_INDEX8 &&
        img->fmt != CR_ARGB888 &&
        img->fmt != CR_ARGB8888)
        return (FALSE);

    /* 8位图只处理调色板 */
    ww = img->position.ww;
    hh = img->position.hh;
    if (img->fmt != CR_INDEX8) {
        for (line = img->data; hh != 0; hh--) {
            for (ptr = line, xx = ww; xx != 0; xx--) {
                bgr2hsl(hsl, ptr); temp = hsl[0];
                ptr[0] = (byte_t)((temp * 255UL) / 360UL);
                ptr[1] = (byte_t)(hsl[1] - (hsl[1] >> 8));
                ptr[2] = (byte_t)(hsl[2] - (hsl[2] >> 8));
                ptr += img->bpc;
            }
            line += img->bpl;
        }
    }
    else {
        ptr = (byte_t*)img->pal;
        for (xx = 256; xx != 0; xx--) {
            bgr2hsl(hsl, ptr); temp = hsl[0];
            ptr[0] = (byte_t)((temp * 255UL) / 360UL);
            ptr[1] = (byte_t)(hsl[1] - (hsl[1] >> 8));
            ptr[2] = (byte_t)(hsl[2] - (hsl[2] >> 8));
            ptr += sizeof(cl32_t);
        }
    }
    return (TRUE);
}

/*
=======================================
    转换到 HSV 图
=======================================
*/
CR_API bool_t
image_to_hsv (
  __CR_IO__ const sIMAGE*   img
    )
{
    uint_t  xx;
    uint_t  ww;
    uint_t  hh;
    byte_t* ptr;
    byte_t* line;
    int32u  temp;
    sint_t  hsv[3];

    /* 只支持图像处理的真彩格式 */
    if (img->fmt != CR_INDEX8 &&
        img->fmt != CR_ARGB888 &&
        img->fmt != CR_ARGB8888)
        return (FALSE);

    /* 8位图只处理调色板 */
    ww = img->position.ww;
    hh = img->position.hh;
    if (img->fmt != CR_INDEX8) {
        for (line = img->data; hh != 0; hh--) {
            for (ptr = line, xx = ww; xx != 0; xx--) {
                bgr2hsv(hsv, ptr); temp = hsv[0];
                ptr[0] = (byte_t)((temp * 255UL) / 360UL);
                ptr[1] = (byte_t)(hsv[1] - (hsv[1] >> 8));
                ptr[2] = (byte_t)(hsv[2] - (hsv[2] >> 8));
                ptr += img->bpc;
            }
            line += img->bpl;
        }
    }
    else {
        ptr = (byte_t*)img->pal;
        for (xx = 256; xx != 0; xx--) {
            bgr2hsv(hsv, ptr); temp = hsv[0];
            ptr[0] = (byte_t)((temp * 255UL) / 360UL);
            ptr[1] = (byte_t)(hsv[1] - (hsv[1] >> 8));
            ptr[2] = (byte_t)(hsv[2] - (hsv[2] >> 8));
            ptr += sizeof(cl32_t);
        }
    }
    return (TRUE);
}

/*
=======================================
    转换到 YUV 图
=======================================
*/
CR_API bool_t
image_to_yuv (
  __CR_IO__ const sIMAGE*   img
    )
{
    uint_t  xx;
    uint_t  ww;
    uint_t  hh;
    byte_t* ptr;
    byte_t* line;

    /* 只支持图像处理的真彩格式 */
    if (img->fmt != CR_INDEX8 &&
        img->fmt != CR_ARGB888 &&
        img->fmt != CR_ARGB8888)
        return (FALSE);

    /* 8位图只处理调色板 */
    ww = img->position.ww;
    hh = img->position.hh;
    if (img->fmt != CR_INDEX8) {
        for (line = img->data; hh != 0; hh--) {
            for (ptr = line, xx = ww; xx != 0; xx--) {
                bgr2yuv(ptr, ptr);
                ptr += img->bpc;
            }
            line += img->bpl;
        }
    }
    else {
        ptr = (byte_t*)img->pal;
        for (xx = 256; xx != 0; xx--) {
            bgr2yuv(ptr, ptr);
            ptr += sizeof(cl32_t);
        }
    }
    return (TRUE);
}

/*
=======================================
    转换到灰度图
=======================================
*/
CR_API sIMAGE*
image_graying (
  __CR_IN__ const sIMAGE*   img
    )
{
    uint_t          xx;
    uint_t          ww;
    uint_t          hh;
    sIMAGE*         dst;
    byte_t*         dpt;
    byte_t*         dln;
    const sIMAGE*   src;
    const byte_t*   spt;
    const byte_t*   sln;

    /* 新建一张8位色图 */
    ww = img->position.ww;
    hh = img->position.hh;
    dst = image_new(img->position.x1, img->position.y1, ww, hh,
                    CR_INDEX8, img->gdi, sizeof(int32u));
    if (dst == NULL)
        return (NULL);

    /* 源图统一转换到32位色图 */
    if (img->fmt != CR_ARGB888 && img->fmt != CR_ARGB8888) {
        src = img_auto_to_32(NULL, 0, 0, img);
        if (src != NULL) {
            mem_free(dst);
            return (NULL);
        }
    }
    else {
        src = img;
    }

    /* 转换到灰度图 */
    for (sln = src->data, dln = dst->data; hh != 0; hh--) {
        for (spt = sln, dpt = dln, xx = ww; xx != 0; xx--, spt += src->bpc)
            *dpt++ = (byte_t)rgb2light(spt[2], spt[1], spt[0]);
        sln += src->bpl;
        dln += dst->bpl;
    }

    /* 必须使用固定的调色板 */
    pal_set_gray8(dst->pal, 256);
    if (src != img)
        image_del(src);
    return (dst);
}

/*
=======================================
    转换到索引图
=======================================
*/
CR_API sIMAGE*
image_indexed (
  __CR_IN__ const sIMAGE*   img,
  __CR_IN__ idx_bgr_t       dopix,
  __CR_IN__ const byte_t*   pal,
  __CR_IN__ leng_t          num,
  __CR_IN__ void_t*         param
    )
{
    uint_t          xx;
    uint_t          yy;
    uint_t          ww;
    uint_t          hh;
    sIMAGE*         dst;
    byte_t*         dpt;
    byte_t*         dln;
    const sIMAGE*   src;
    const byte_t*   spt;
    const byte_t*   sln;

    /* 新建一张8位色图 */
    ww = img->position.ww;
    hh = img->position.hh;
    dst = image_new(img->position.x1, img->position.y1, ww, hh,
                    CR_INDEX8, img->gdi, sizeof(int32u));
    if (dst == NULL)
        return (NULL);

    /* 源图统一转换到32位色图 */
    if (img->fmt != CR_ARGB888 && img->fmt != CR_ARGB8888) {
        src = img_auto_to_32(NULL, 0, 0, img);
        if (src != NULL) {
            mem_free(dst);
            return (NULL);
        }
    }
    else {
        src = img;
    }

    /* 转换到索引图 */
    for (sln = src->data, dln = dst->data, yy = 0; hh != 0; hh--, yy++) {
        for (spt = sln, dpt = dln, xx = 0; xx < ww; xx++, spt += src->bpc)
            *dpt++ = dopix(param, xx, yy, spt);
        sln += src->bpl;
        dln += dst->bpl;
    }

    /* 设置索引图的调色板 */
    if (pal != NULL) {
        if (num > 256) num = 256;
        mem_cpy(dst->pal, pal, num * sizeof(int32u));
    }
    if (src != img)
        image_del(src);
    return (dst);
}

/*
=======================================
    灰度图二值化 (自适应)
=======================================
*/
CR_API bool_t
image_binary0 (
  __CR_IO__ const sIMAGE*   gray,
  __CR_IN__ bool_t          is_he
    )
{
    uint_t  ww, hh;
    byte_t  lut[256];
    leng_t  tab[256];

    /* 输入必须为灰度图 */
    if (gray->fmt != CR_INDEX8)
        return (FALSE);

    /* 可选直方图均匀功能 */
    if (is_he) {
        ww = gray->position.ww;
        hh = gray->position.hh;
        image_histo(tab, gray);
        dot_histo_ave(lut, tab, ww, hh);
        image_lookup(gray, lut);
    }
    image_histo(tab, gray);
    image_binary1(gray, histo_otsu(tab));
    return (TRUE);
}

/*
=======================================
    灰度图二值化 (通过单一阈值)
=======================================
*/
CR_API bool_t
image_binary1 (
  __CR_IO__ const sIMAGE*   gray,
  __CR_IN__ byte_t          gate
    )
{
    uint_t  xx;
    uint_t  ww;
    uint_t  hh;
    byte_t* ptr;
    byte_t* line;

    /* 输入必须为灰度图 */
    if (gray->fmt != CR_INDEX8)
        return (FALSE);

    /* 通过灰度阈值分割 */
    ww = gray->position.ww;
    hh = gray->position.hh;
    for (line = gray->data; hh != 0; hh--) {
        for (ptr = line, xx = ww; xx != 0; xx--, ptr++)
            *ptr = (*ptr < gate) ? 0 : 255;
        line += gray->bpl;
    }
    return (TRUE);
}

/*
=======================================
    灰度图二值化 (通过阈值图片)
=======================================
*/
CR_API bool_t
image_binary2 (
  __CR_IO__ const sIMAGE*   gray,
  __CR_IN__ const sIMAGE*   gate,
  __CR_IN__ sint_t          offset
    )
{
    uint_t          xx;
    uint_t          ww;
    uint_t          hh;
    byte_t*         dpt;
    byte_t*         dln;
    const byte_t*   spt;
    const byte_t*   sln;

    /* 输入必须为灰度图 */
    if (gray->fmt != CR_INDEX8 ||
        gate->fmt != CR_INDEX8)
        return (FALSE);

    /* 两张图的宽高必须一致 */
    if (gray->position.ww != gate->position.ww ||
        gray->position.hh != gate->position.hh)
        return (FALSE);

    /* 通过灰度阈值分割 */
    ww = gray->position.ww;
    hh = gray->position.hh;
    for (sln = gate->data, dln = gray->data; hh != 0; hh--) {
        for (spt = sln, dpt = dln, xx = ww; xx != 0; xx--, spt++, dpt++)
            *dpt = (((sint_t)*dpt) < ((sint_t)*spt) - offset) ? 0 : 255;
        sln += gate->bpl;
        dln += gray->bpl;
    }
    return (TRUE);
}

/*****************************************************************************/
/*                                直方图生成                                 */
/*****************************************************************************/

/*
=======================================
    灰度直方图计算
=======================================
*/
CR_API bool_t
image_histo (
  __CR_OT__ leng_t          tab[256],
  __CR_IN__ const sIMAGE*   gray
    )
{
    uint_t          xx;
    uint_t          ww;
    uint_t          hh;
    const byte_t*   ptr;
    const byte_t*   line;

    /* 输入必须为灰度图 */
    mem_zero(tab, 256 * sizeof(leng_t));
    if (gray->fmt != CR_INDEX8)
        return (FALSE);

    /* 生成直方图 */
    ww = gray->position.ww;
    hh = gray->position.hh;
    for (line = gray->data; hh != 0; hh--) {
        for (ptr = line, xx = ww; xx != 0; xx--)
            tab[*ptr++] += 1;
        line += gray->bpl;
    }
    return (TRUE);
}

/*
=======================================
    彩色直方图计算
=======================================
*/
CR_API bool_t
image_histo3 (
  __CR_OT__ leng_t          t_r[256],
  __CR_OT__ leng_t          t_g[256],
  __CR_OT__ leng_t          t_b[256],
  __CR_IN__ const sIMAGE*   img
    )
{
    uint_t          xx;
    uint_t          ww;
    uint_t          hh;
    const byte_t*   ptr;
    const byte_t*   line;

    /* 输入必须为彩色图 */
    mem_zero(t_r, 256 * sizeof(leng_t));
    mem_zero(t_g, 256 * sizeof(leng_t));
    mem_zero(t_b, 256 * sizeof(leng_t));
    if (img->fmt != CR_ARGB888 &&
        img->fmt != CR_ARGB8888)
        return (FALSE);

    /* 生成直方图 */
    ww = img->position.ww;
    hh = img->position.hh;
    for (line = img->data; hh != 0; hh--) {
        for (ptr = line, xx = ww; xx != 0; xx--) {
            t_b[ptr[0]] += 1;
            t_g[ptr[1]] += 1;
            t_r[ptr[2]] += 1;
            ptr += img->bpc;
        }
        line += img->bpl;
    }
    return (TRUE);
}

/*****************************************************************************/
/*                                 像素变换                                  */
/*****************************************************************************/

/*
=======================================
    灰度查表变换
=======================================
*/
CR_API bool_t
image_lookup (
  __CR_IO__ const sIMAGE*   gray,
  __CR_IN__ const byte_t    tab[256]
    )
{
    uint_t  xx;
    uint_t  ww;
    uint_t  hh;
    byte_t* ptr;
    byte_t* line;

    /* 输入必须为灰度图 */
    if (gray->fmt != CR_INDEX8)
        return (FALSE);

    /* 像素查表变换 */
    ww = gray->position.ww;
    hh = gray->position.hh;
    for (line = gray->data; hh != 0; hh--) {
        for (ptr = line, xx = ww; xx != 0; xx--, ptr++)
            *ptr = tab[*ptr];
        line += gray->bpl;
    }
    return (TRUE);
}

/*
=======================================
    彩色查表变换
=======================================
*/
CR_API bool_t
image_lookup3 (
  __CR_IO__ const sIMAGE*   img,
  __CR_IN__ const byte_t    t_r[256],
  __CR_IN__ const byte_t    t_g[256],
  __CR_IN__ const byte_t    t_b[256]
    )
{
    uint_t  xx;
    uint_t  ww;
    uint_t  hh;
    byte_t* ptr;
    byte_t* line;

    /* 输入必须为彩色图 */
    if (img->fmt != CR_ARGB888 &&
        img->fmt != CR_ARGB8888)
        return (FALSE);

    /* 像素查表变换 */
    ww = img->position.ww;
    hh = img->position.hh;
    for (line = img->data; hh != 0; hh--) {
        for (ptr = line, xx = ww; xx != 0; xx--) {
            ptr[0] = t_b[ptr[0]];
            ptr[1] = t_g[ptr[1]];
            ptr[2] = t_r[ptr[2]];
            ptr += img->bpc;
        }
        line += img->bpl;
    }
    return (TRUE);
}

/*
=======================================
    伽玛变换
=======================================
*/
CR_API void_t
dot_gamma (
  __CR_OT__ byte_t  tab[256],
  __CR_IN__ fp32_t  gamma,
  __CR_IN__ fp32_t  comp
    )
{
    fp32_t  val;
    sint_t  idx;
    sint_t  tmp;

    gamma = 1.0f / gamma;
    for (idx = 0; idx < 256; idx++) {
        tmp = idx + (idx >> 7);
        val = tmp / 256.0f;
        val = FPOW(val + comp, gamma);
        if (val <= 0.0f) {
            tab[idx] = 0x00;
        }
        else {
            tmp = (sint_t)(val * 255.0f + 0.5f);
            if (tmp >= 255)
                tab[idx] = 0xFF;
            else
                tab[idx] = (byte_t)tmp;
        }
    }
}

/*
=======================================
    乘加变换
=======================================
*/
CR_API void_t
dot_muladd (
  __CR_OT__ byte_t  tab[256],
  __CR_IN__ fp32_t  fmul,
  __CR_IN__ fp32_t  fadd
    )
{
    fp32_t  val;
    sint_t  idx;
    sint_t  tmp;

    for (idx = 0; idx < 256; idx++) {
        val = fmul * idx + fadd;
        if (val <= 0.0f) {
            tab[idx] = 0x00;
        }
        else {
            tmp = (sint_t)(val + 0.5f);
            if (tmp >= 255)
                tab[idx] = 0xFF;
            else
                tab[idx] = (byte_t)tmp;
        }
    }
}

/*
=======================================
    对比度变换
=======================================
*/
CR_API void_t
dot_contrast (
  __CR_OT__ byte_t  tab[256],
  __CR_IN__ fp32_t  contrast
    )
{
    fp32_t  val;
    sint_t  idx;
    sint_t  tmp;

    for (idx = 0; idx < 256; idx++) {
        tmp = idx + (idx >> 7) - 128;
        val = 128.0f + contrast * tmp;
        if (val <= 0.0f) {
            tab[idx] = 0x00;
        }
        else {
            tmp = (sint_t)(val + 0.5f);
            if (tmp >= 255)
                tab[idx] = 0xFF;
            else
                tab[idx] = (byte_t)tmp;
        }
    }
}

/*
=======================================
    曝光变换
=======================================
*/
CR_API void_t
dot_solarize (
  __CR_OT__ byte_t  tab[256],
  __CR_IN__ sint_t  light
    )
{
    sint_t  idx;

    for (idx = 0; idx < 256; idx++) {
        if (idx <= light)
            tab[idx] = (byte_t)(255 - idx);
        else
            tab[idx] = (byte_t)(idx);
    }
}

/*
=======================================
    直方图均匀
=======================================
*/
CR_API void_t
dot_histo_ave (
  __CR_OT__ byte_t          tab[256],
  __CR_IN__ const leng_t    hist[256],
  __CR_IN__ uint_t          width,
  __CR_IN__ uint_t          height
    )
{
    int64s  sum;
    uint_t  idx;
    sint_t  tmp;
    fp32_t  tot;

    sum  = 0;
    tot  = (fp32_t)width;
    tot *= (fp32_t)height;
    for (idx = 0; idx < 256; idx++) {
        sum += hist[idx];
        tmp = (sint_t)((sum * 255) / tot + 0.5f);
        if (tmp >= 255)
            tab[idx] = 0xFF;
        else
            tab[idx] = (byte_t)tmp;
    }
}

/*
=======================================
    图片亮度平衡
=======================================
*/
CR_API bool_t
pic_white_bl (
  __CR_IO__ const sIMAGE*   img,
  __CR_IN__ sint_t          light
    )
{
    fp32_t  f_r, f_g, f_b;
    sint_t  a_r, a_g, a_b;
    byte_t  c_r[256], c_g[256], c_b[256];
    leng_t  t_r[256], t_g[256], t_b[256];

    /* 灰度图 */
    if (img->fmt == CR_INDEX8) {
        if (light >= 0) {
            image_histo(t_r, img);
            a_r = histo_avge(t_r);
            if (a_r == 0) a_r = 1;
            f_r  = (fp32_t)light;
            f_r /= (fp32_t)a_r;
            dot_muladd(c_r, f_r, 0.0f);
            image_lookup(img, c_r);
        }
        return (TRUE);
    }

    /* 真彩图 */
    if (img->fmt == CR_ARGB888 ||
        img->fmt == CR_ARGB8888) {
        image_histo3(t_r, t_g, t_b, img);
        a_r = histo_avge(t_r);
        a_g = histo_avge(t_g);
        a_b = histo_avge(t_b);
        if (a_r == 0) a_r = 1;
        if (a_g == 0) a_g = 1;
        if (a_b == 0) a_b = 1;
        if (light < 0)
            light = rgb2light(a_r, a_g, a_b);
        f_r  = (fp32_t)light;
        f_r /= (fp32_t)a_r;
        f_g  = (fp32_t)light;
        f_g /= (fp32_t)a_g;
        f_b  = (fp32_t)light;
        f_b /= (fp32_t)a_b;
        dot_muladd(c_r, f_r, 0.0f);
        dot_muladd(c_g, f_g, 0.0f);
        dot_muladd(c_b, f_b, 0.0f);
        image_lookup3(img, c_r, c_g, c_b);
        return (TRUE);
    }

    /* 非法图 */
    return (FALSE);
}

/*
=======================================
    图片饱和度调整
=======================================
*/
CR_API bool_t
pic_saturation (
  __CR_IO__ const sIMAGE*   img,
  __CR_IN__ const sIMAGE*   gray,
  __CR_IN__ fp32_t          fsat
    )
{
    sint_t  light;
    byte_t* c_ptr;
    byte_t* g_ptr;
    byte_t* c_line;
    byte_t* g_line;
    fp32_t  rr, gg, bb;
    uint_t  xx, ww, hh;

    /* 必须输入彩色图与它的灰度图 */
    if (img->fmt != CR_ARGB888 &&
        img->fmt != CR_ARGB8888)
        return (FALSE);
    if (gray->fmt != CR_INDEX8)
        return (FALSE);

    /* 两张图的宽高必须一致 */
    if (img->position.ww != gray->position.ww ||
        img->position.hh != gray->position.hh)
        return (FALSE);
    ww = img->position.ww;
    hh = img->position.hh;

    /* 调整饱和度即为彩色图与灰度图之间的插值 */
    for (c_line = img->data, g_line = gray->data; hh != 0; hh--) {
        for (c_ptr = c_line, g_ptr = g_line, xx = ww; xx != 0; xx--) {
            light = *g_ptr++;
            bb = light + fsat * ((sint_t)c_ptr[0] - light);
            gg = light + fsat * ((sint_t)c_ptr[1] - light);
            rr = light + fsat * ((sint_t)c_ptr[2] - light);

            /* 蓝色通道 */
            if (bb <= 0.0f) {
                c_ptr[0] = 0x00;
            }
            else {
                light = (sint_t)(bb + 0.5f);
                if (light >= 255)
                    c_ptr[0] = 0xFF;
                else
                    c_ptr[0] = (byte_t)light;
            }

            /* 绿色通道 */
            if (gg <= 0.0f) {
                c_ptr[1] = 0x00;
            }
            else {
                light = (sint_t)(gg + 0.5f);
                if (light >= 255)
                    c_ptr[1] = 0xFF;
                else
                    c_ptr[1] = (byte_t)light;
            }

            /* 红色通道 */
            if (rr <= 0.0f) {
                c_ptr[2] = 0x00;
            }
            else {
                light = (sint_t)(rr + 0.5f);
                if (light >= 255)
                    c_ptr[2] = 0xFF;
                else
                    c_ptr[2] = (byte_t)light;
            }
            c_ptr += img->bpc;
        }
        c_line += img->bpl;
        g_line += gray->bpl;
    }
    return (TRUE);
}

/*****************************************************************************/
/*                                 杂项操作                                  */
/*****************************************************************************/

/*
=======================================
    抓出一块图片
=======================================
*/
CR_API sIMAGE*
image_grab (
  __CR_IN__ const sIMAGE*   img,
  __CR_IN__ const sRECT*    box
    )
{
    sBLIT   blt;
    sRECT   rct;
    sIMAGE* dst;

    /* 只支持图像处理的常见格式 */
    if (img->fmt != CR_INDEX8 &&
        img->fmt != CR_ARGB888 &&
        img->fmt != CR_ARGB8888)
        return (NULL);

    /* 判断抓图框是否合法 */
    if (!clip_rect(&rct, box, &img->clip_win))
        return (NULL);

    /* 生成目标图片并复制 */
    dst = image_new(0, 0, rct.ww, rct.hh, img->fmt, FALSE, 4);
    if (dst == NULL)
        return (NULL);
    blt.sx = rct.x1;
    blt.sy = rct.y1;
    blt.sw = rct.ww;
    blt.sh = rct.hh;
    blt.dx = blt.dy = 0;
    dst->keycolor = img->keycolor;
    mem_cpy(dst->pal, img->pal, 1024);
    if (img->fmt == CR_ARGB8888)
        blit_set32_c(dst, img, &blt, NULL);
    else
    if (img->fmt == CR_INDEX8)
        blit_set08_c(dst, img, &blt, NULL);
    else
        blit_set24_c(dst, img, &blt, NULL);
    return (dst);
}

/* 定点数转换常数 */
#define FIXED_SHR           16
#define FIXED_NUM   (1L << FIXED_SHR)

/* 公用代码简化宏 */
#define ROTATE_GET_DEST \
    sIMAGE  *dst; \
    sint_t  box_x, box_y; \
    uint_t  new_w, new_h, xx; \
    fp32_t  rx0, ry0, rx1, ry1, sina, cosa; \
    int32s  iax, iay, ibx, iby, icx, icy, idx, idy; \
    fp32_t  fax, fay, fbx, fby, fcx, fcy, fdx, fdy; \
\
    sina = FSIN(ccw); \
    cosa = FCOS(ccw); \
    box_x = (sint_t)box->ww; \
    box_y = (sint_t)box->hh; \
    rx1 = (fp32_t)box_x; \
    ry1 = (fp32_t)box_y; \
    rx0 = (rx1 - 1.0f) / 2.0f; \
    ry0 = (ry1 - 1.0f) / 2.0f; \
    rx1 *= scale; ry1 *= scale; \
    fax = rx1 * cosa; \
    if (fax < 0.0f) fax = -fax; \
    fay = ry1 * sina; \
    if (fay < 0.0f) fay = -fay; \
    fbx = rx1 * sina; \
    if (fbx < 0.0f) fbx = -fbx; \
    fby = ry1 * cosa; \
    if (fby < 0.0f) fby = -fby; \
    new_w = (sint_t)(fax + fay + 0.999f); \
    new_h = (sint_t)(fbx + fby + 0.999f); \
    box_x = (sint_t)(new_w - box->ww); \
    box_y = (sint_t)(new_h - box->hh); \
    rx1 = rx0 + ((fp32_t)box_x) / 2.0f; \
    ry1 = ry0 + ((fp32_t)box_y) / 2.0f; \
    sina /= scale; cosa /= scale;

/*
---------------------------------------
    旋转一块图片 (bpc = 1)
---------------------------------------
*/
static sIMAGE*
image_rot1 (
  __CR_IN__ const sIMAGE*   img,
  __CR_IN__ const sRECT*    box,
  __CR_IN__ fp32_t          ccw,
  __CR_IN__ fp32_t          scale,
  __CR_IN__ bool_t          fast
    )
{
    byte_t  *line, *pntr;

    ROTATE_GET_DEST

    /* 生成目标图片 */
    dst = image_new(0, 0, new_w, new_h, img->fmt, FALSE, 4);
    if (dst == NULL)
        return (NULL);

    /* 开始旋转复制 */
    line = dst->data;
    if (fast)
    {
        /* 临近采样 */
        iax = (int32s)( cosa * FIXED_NUM);
        iay = (int32s)( sina * FIXED_NUM);
        ibx = (int32s)(-sina * FIXED_NUM);
        iby = (int32s)( cosa * FIXED_NUM);
        icx = (int32s)((rx0 - rx1 * cosa + ry1 * sina) * FIXED_NUM);
        icy = (int32s)((ry0 - rx1 * sina - ry1 * cosa) * FIXED_NUM);
        for (; new_h != 0; new_h--) {
            idx = icx;
            idy = icy;
            for (xx = 0; xx < new_w; xx++) {
                box_x = (sint_t)(idx >> FIXED_SHR);
                box_y = (sint_t)(idy >> FIXED_SHR);
                box_x += box->x1;
                box_y += box->y1;
                if (clip_pixel(box_x, box_y, &img->clip_win)) {
                    if (img->gdi)
                        box_y = img->clip_win.y2 - box_y;
                    pntr = pixel_addr1(img, box_x, box_y);
                    line[xx] = pntr[0];
                }
                else {
                    line[xx] = 0x00;
                }
                idx += iax;
                idy += iay;
            }
            icx += ibx;
            icy += iby;
            line += dst->bpl;
        }
    }
    else
    {
        /* 双线性插值 */
        fax =  cosa;
        fay =  sina;
        fbx = -sina;
        fby =  cosa;
        fcx = rx0 - rx1 * cosa + ry1 * sina;
        fcy = ry0 - rx1 * sina - ry1 * cosa;
        for (; new_h != 0; new_h--) {
            fdx = fcx;
            fdy = fcy;
            for (xx = 0; xx < new_w; xx++) {
                rx0 = box->x1 + fdx;
                ry0 = box->y1 + fdy;
                box_x = (sint_t)rx0;
                box_y = (sint_t)ry0;
                if (clip_pixel(box_x, box_y, &img->clip_win)) {
                    if (rx0 < 0.0f) rx0 = 0.0f;
                    if (ry0 < 0.0f) ry0 = 0.0f;
                    line[xx] = dot_wu_get08(img, rx0, ry0);
                }
                else {
                    line[xx] = 0x00;
                }
                fdx += fax;
                fdy += fay;
            }
            fcx += fbx;
            fcy += fby;
            line += dst->bpl;
        }
    }
    return (dst);
}

/*
---------------------------------------
    旋转一块图片 (bpc = 3)
---------------------------------------
*/
static sIMAGE*
image_rot3 (
  __CR_IN__ const sIMAGE*   img,
  __CR_IN__ const sRECT*    box,
  __CR_IN__ fp32_t          ccw,
  __CR_IN__ fp32_t          scale,
  __CR_IN__ bool_t          fast
    )
{
    cpix_t  color;
    byte_t  *line, *pntr;

    ROTATE_GET_DEST

    /* 生成目标图片 */
    dst = image_new(0, 0, new_w, new_h, img->fmt, FALSE, 4);
    if (dst == NULL)
        return (NULL);

    /* 开始旋转复制 */
    line = dst->data;
    if (fast)
    {
        /* 临近采样 */
        iax = (int32s)( cosa * FIXED_NUM);
        iay = (int32s)( sina * FIXED_NUM);
        ibx = (int32s)(-sina * FIXED_NUM);
        iby = (int32s)( cosa * FIXED_NUM);
        icx = (int32s)((rx0 - rx1 * cosa + ry1 * sina) * FIXED_NUM);
        icy = (int32s)((ry0 - rx1 * sina - ry1 * cosa) * FIXED_NUM);
        for (; new_h != 0; new_h--) {
            idx = icx;
            idy = icy;
            for (xx = 0; xx < new_w; xx++) {
                box_x = (sint_t)(idx >> FIXED_SHR);
                box_y = (sint_t)(idy >> FIXED_SHR);
                box_x += box->x1;
                box_y += box->y1;
                if (clip_pixel(box_x, box_y, &img->clip_win)) {
                    if (img->gdi)
                        box_y = img->clip_win.y2 - box_y;
                    pntr = pixel_addr3(img, box_x, box_y);
                    line[xx * 3 + 0] = pntr[0];
                    line[xx * 3 + 1] = pntr[1];
                    line[xx * 3 + 2] = pntr[2];
                }
                else {
                    line[xx * 3 + 0] = 0x00;
                    line[xx * 3 + 1] = 0x00;
                    line[xx * 3 + 2] = 0x00;
                }
                idx += iax;
                idy += iay;
            }
            icx += ibx;
            icy += iby;
            line += dst->bpl;
        }
    }
    else
    {
        /* 双线性插值 */
        fax =  cosa;
        fay =  sina;
        fbx = -sina;
        fby =  cosa;
        fcx = rx0 - rx1 * cosa + ry1 * sina;
        fcy = ry0 - rx1 * sina - ry1 * cosa;
        for (; new_h != 0; new_h--) {
            fdx = fcx;
            fdy = fcy;
            for (xx = 0; xx < new_w; xx++) {
                rx0 = box->x1 + fdx;
                ry0 = box->y1 + fdy;
                box_x = (sint_t)rx0;
                box_y = (sint_t)ry0;
                if (clip_pixel(box_x, box_y, &img->clip_win)) {
                    if (rx0 < 0.0f) rx0 = 0.0f;
                    if (ry0 < 0.0f) ry0 = 0.0f;
                    color.val = dot_wu_get24(img, rx0, ry0);
                    line[xx * 3 + 0] = color.c32.bbb;
                    line[xx * 3 + 1] = color.c32.ggg;
                    line[xx * 3 + 2] = color.c32.rrr;
                }
                else {
                    line[xx * 3 + 0] = 0x00;
                    line[xx * 3 + 1] = 0x00;
                    line[xx * 3 + 2] = 0x00;
                }
                fdx += fax;
                fdy += fay;
            }
            fcx += fbx;
            fcy += fby;
            line += dst->bpl;
        }
    }
    return (dst);
}

/*
---------------------------------------
    旋转一块图片 (bpc = 4)
---------------------------------------
*/
static sIMAGE*
image_rot4 (
  __CR_IN__ const sIMAGE*   img,
  __CR_IN__ const sRECT*    box,
  __CR_IN__ fp32_t          ccw,
  __CR_IN__ fp32_t          scale,
  __CR_IN__ bool_t          fast
    )
{
    int32u  *line, *pntr;

    ROTATE_GET_DEST

    /* 生成目标图片 */
    dst = image_new(0, 0, new_w, new_h, img->fmt, FALSE, 4);
    if (dst == NULL)
        return (NULL);

    /* 开始旋转复制 */
    line = (int32u*)dst->data;
    if (fast)
    {
        /* 临近采样 */
        iax = (int32s)( cosa * FIXED_NUM);
        iay = (int32s)( sina * FIXED_NUM);
        ibx = (int32s)(-sina * FIXED_NUM);
        iby = (int32s)( cosa * FIXED_NUM);
        icx = (int32s)((rx0 - rx1 * cosa + ry1 * sina) * FIXED_NUM);
        icy = (int32s)((ry0 - rx1 * sina - ry1 * cosa) * FIXED_NUM);
        for (; new_h != 0; new_h--) {
            idx = icx;
            idy = icy;
            for (xx = 0; xx < new_w; xx++) {
                box_x = (sint_t)(idx >> FIXED_SHR);
                box_y = (sint_t)(idy >> FIXED_SHR);
                box_x += box->x1;
                box_y += box->y1;
                if (clip_pixel(box_x, box_y, &img->clip_win)) {
                    if (img->gdi)
                        box_y = img->clip_win.y2 - box_y;
                    pntr = pixel_addr4(img, box_x, box_y);
                    line[xx] = pntr[0];
                }
                else {
                    line[xx] = 0x00000000UL;
                }
                idx += iax;
                idy += iay;
            }
            icx += ibx;
            icy += iby;
            line = (int32u*)((byte_t*)line + dst->bpl);
        }
    }
    else
    {
        /* 双线性插值 */
        fax =  cosa;
        fay =  sina;
        fbx = -sina;
        fby =  cosa;
        fcx = rx0 - rx1 * cosa + ry1 * sina;
        fcy = ry0 - rx1 * sina - ry1 * cosa;
        for (; new_h != 0; new_h--) {
            fdx = fcx;
            fdy = fcy;
            for (xx = 0; xx < new_w; xx++) {
                rx0 = box->x1 + fdx;
                ry0 = box->y1 + fdy;
                box_x = (sint_t)rx0;
                box_y = (sint_t)ry0;
                if (clip_pixel(box_x, box_y, &img->clip_win)) {
                    if (rx0 < 0.0f) rx0 = 0.0f;
                    if (ry0 < 0.0f) ry0 = 0.0f;
                    line[xx] = dot_wu_get32(img, rx0, ry0);
                }
                else {
                    line[xx] = 0x00000000UL;
                }
                fdx += fax;
                fdy += fay;
            }
            fcx += fbx;
            fcy += fby;
            line = (int32u*)((byte_t*)line + dst->bpl);
        }
    }
    return (dst);
}

/*
=======================================
    旋转一块图片
=======================================
*/
CR_API sIMAGE*
image_rotz (
  __CR_IN__ const sIMAGE*   img,
  __CR_IN__ const sRECT*    box,
  __CR_IN__ fp32_t          ccw,
  __CR_IN__ fp32_t          scale,
  __CR_IN__ bool_t          lerp
    )
{
    fp32_t  tmp;
    sint_t  len;
    sIMAGE* dst;

    /* 只支持图像处理的常见格式 */
    if (img->fmt != CR_INDEX8 &&
        img->fmt != CR_ARGB888 &&
        img->fmt != CR_ARGB8888)
        return (NULL);

    /* 判断抓图框是否合法 */
    if (box->ww > img->clip_win.ww ||
        box->hh > img->clip_win.hh ||
        (uint_t)box->x1 > img->clip_win.ww - box->ww ||
        (uint_t)box->y1 > img->clip_win.hh - box->hh)
        return (NULL);

    /* 计算缩放是否有效 */
    len = CR_MIN(box->ww, box->hh);
    tmp = ((fp32_t)len) * scale;
    if (tmp > -1.0f && tmp < 1.0f)
        return (NULL);

    /* 根据图片格式旋转图片 */
    if (img->fmt == CR_ARGB8888)
        dst = image_rot4(img, box, ccw, scale, !lerp);
    else
    if (img->fmt == CR_INDEX8)
        dst = image_rot1(img, box, ccw, scale, !lerp);
    else
        dst = image_rot3(img, box, ccw, scale, !lerp);
    if (dst != NULL) {
        dst->keycolor = img->keycolor;
        mem_cpy(dst->pal, img->pal, 1024);
    }
    return (dst);
}

/*
=======================================
    回写一块图片
=======================================
*/
CR_API void_t
image_back (
  __CR_IO__ const sIMAGE*   dst,
  __CR_IN__ const sIMAGE*   src,
  __CR_IN__ sint_t          left,
  __CR_IN__ sint_t          top
    )
{
    sBLIT   blt;

    if (dst->fmt != src->fmt)
        return;
    blt.dx = left;
    blt.dy = top;
    blt.sx = blt.sy = 0;
    blt.sw = src->position.ww;
    blt.sh = src->position.hh;
    if (src->fmt == CR_ARGB8888)
        blit_set32_c(dst, src, &blt, NULL);
    else
    if (src->fmt == CR_INDEX8)
        blit_set08_c(dst, src, &blt, NULL);
    else
    if (src->fmt == CR_ARGB888)
        blit_set24_c(dst, src, &blt, NULL);
}

/*
=======================================
    扩展图片边框
=======================================
*/
CR_API sIMAGE*
image_bound (
  __CR_IO__ const sIMAGE*   img,
  __CR_IN__ uint_t          xsize,
  __CR_IN__ uint_t          ysize
    )
{
    sBLIT   blt;
    sIMAGE* dst;
    uint_t  idx;
    uint_t  ww, hh;

    /* 只支持图像处理的常见格式 */
    if (img->fmt != CR_INDEX8 &&
        img->fmt != CR_ARGB888 &&
        img->fmt != CR_ARGB8888)
        return (NULL);
    ww = img->position.ww + xsize * 2;
    if (ww < img->position.ww)
        return (NULL);
    hh = img->position.hh + ysize * 2;
    if (hh < img->position.hh)
        return (NULL);

    /* 生成目标图片并用边界像素填充扩展的边 */
    dst = image_new(0, 0, ww, hh, img->fmt, FALSE, 4);
    if (dst == NULL)
        return (NULL);
    dst->keycolor = img->keycolor;
    mem_cpy(dst->pal, img->pal, 1024);
    if (img->fmt == CR_ARGB8888)
    {
        /* 复制原图 */
        blt.dx = xsize;
        blt.dy = ysize;
        blt.sx = blt.sy = 0;
        blt.sw = img->position.ww;
        blt.sh = img->position.hh;
        blit_set32_c(dst, img, &blt, NULL);

        /* 复制左边 */
        blt.sw = 1;
        for (idx = 0; idx < xsize; idx++) {
            blt.dx = idx;
            blit_set32_c(dst, img, &blt, NULL);
        }

        /* 复制右边 */
        idx += img->position.ww;
        blt.sx = img->position.ww - 1;
        for (; idx < dst->position.ww; idx++) {
            blt.dx = idx;
            blit_set32_c(dst, img, &blt, NULL);
        }

        /* 复制上边 */
        blt.dx = 0;
        blt.sx = 0;
        blt.sy = ysize;
        blt.sw = dst->position.ww;
        blt.sh = 1;
        for (idx = 0; idx < ysize; idx++) {
            blt.dy = idx;
            blit_set32_c(dst, dst, &blt, NULL);
        }

        /* 复制下边 */
        idx += img->position.hh;
        blt.sy = idx - 1;
        for (; idx < dst->position.hh; idx++) {
            blt.dy = idx;
            blit_set32_c(dst, dst, &blt, NULL);
        }
    }
    else
    if (img->fmt == CR_INDEX8)
    {
        /* 复制原图 */
        blt.dx = xsize;
        blt.dy = ysize;
        blt.sx = blt.sy = 0;
        blt.sw = img->position.ww;
        blt.sh = img->position.hh;
        blit_set08_c(dst, img, &blt, NULL);

        /* 复制左边 */
        blt.sw = 1;
        for (idx = 0; idx < xsize; idx++) {
            blt.dx = idx;
            blit_set08_c(dst, img, &blt, NULL);
        }

        /* 复制右边 */
        idx += img->position.ww;
        blt.sx = img->position.ww - 1;
        for (; idx < dst->position.ww; idx++) {
            blt.dx = idx;
            blit_set08_c(dst, img, &blt, NULL);
        }

        /* 复制上边 */
        blt.dx = 0;
        blt.sx = 0;
        blt.sy = ysize;
        blt.sw = dst->position.ww;
        blt.sh = 1;
        for (idx = 0; idx < ysize; idx++) {
            blt.dy = idx;
            blit_set08_c(dst, dst, &blt, NULL);
        }

        /* 复制下边 */
        idx += img->position.hh;
        blt.sy = idx - 1;
        for (; idx < dst->position.hh; idx++) {
            blt.dy = idx;
            blit_set08_c(dst, dst, &blt, NULL);
        }
    }
    else
    {
        /* 复制原图 */
        blt.dx = xsize;
        blt.dy = ysize;
        blt.sx = blt.sy = 0;
        blt.sw = img->position.ww;
        blt.sh = img->position.hh;
        blit_set24_c(dst, img, &blt, NULL);

        /* 复制左边 */
        blt.sw = 1;
        for (idx = 0; idx < xsize; idx++) {
            blt.dx = idx;
            blit_set24_c(dst, img, &blt, NULL);
        }

        /* 复制右边 */
        idx += img->position.ww;
        blt.sx = img->position.ww - 1;
        for (; idx < dst->position.ww; idx++) {
            blt.dx = idx;
            blit_set24_c(dst, img, &blt, NULL);
        }

        /* 复制上边 */
        blt.dx = 0;
        blt.sx = 0;
        blt.sy = ysize;
        blt.sw = dst->position.ww;
        blt.sh = 1;
        for (idx = 0; idx < ysize; idx++) {
            blt.dy = idx;
            blit_set24_c(dst, dst, &blt, NULL);
        }

        /* 复制下边 */
        idx += img->position.hh;
        blt.sy = idx - 1;
        for (; idx < dst->position.hh; idx++) {
            blt.dy = idx;
            blit_set24_c(dst, dst, &blt, NULL);
        }
    }
    return (dst);
}

/*
=======================================
    收缩图片边框
=======================================
*/
CR_API bool_t
image_unbound (
  __CR_IN__ const sIMAGE*   dst,
  __CR_IN__ const sIMAGE*   src,
  __CR_IN__ uint_t          xsize,
  __CR_IN__ uint_t          ysize
    )
{
    sBLIT   blt;
    uint_t  ww, hh;

    /* 只支持图像处理的常见格式 */
    if (src->fmt != CR_INDEX8 &&
        src->fmt != CR_ARGB888 &&
        src->fmt != CR_ARGB8888)
        return (FALSE);
    if (dst->fmt != src->fmt)
        return (FALSE);
    ww = src->position.ww - xsize * 2;
    if (ww != dst->position.ww)
        return (FALSE);
    hh = src->position.hh - ysize * 2;
    if (hh != dst->position.hh)
        return (FALSE);

    /* 图片数据回拷 */
    blt.dx = 0;
    blt.dy = 0;
    blt.sx = xsize;
    blt.sy = ysize;
    blt.sw = ww;
    blt.sh = hh;
    if (src->fmt == CR_ARGB8888)
        blit_set32_c(dst, src, &blt, NULL);
    else
    if (src->fmt == CR_INDEX8)
        blit_set08_c(dst, src, &blt, NULL);
    else
        blit_set24_c(dst, src, &blt, NULL);
    return (TRUE);
}

/*
=======================================
    图片自定义运算
=======================================
*/
CR_API bool_t
image_oper (
  __CR_IN__ const sIMAGE*   dst,
  __CR_IN__ const sIMAGE*   src,
  __CR_IN__ pix_ops_t       dopix
    )
{
    uint_t  hh;
    sint_t  val;
    leng_t  idx;
    leng_t  line;
    leng_t  sbpl;
    leng_t  dbpl;
    byte_t* sptr;
    byte_t* dptr;

    /* 只支持图像处理的常见格式 */
    if (src->fmt != CR_INDEX8 &&
        src->fmt != CR_ARGB888 &&
        src->fmt != CR_ARGB8888)
        return (FALSE);
    if (dst->fmt != src->fmt ||
        src->position.ww != dst->position.ww ||
        src->position.hh != dst->position.hh)
        return (FALSE);

    /* 做好运算准备 */
    if (src->gdi) {
        sbpl = 0 - src->bpl;
        sptr = src->data + src->clip_win.y2 * src->bpl;
    }
    else {
        sbpl = src->bpl;
        sptr = src->data;
    }
    if (dst->gdi) {
        dbpl = 0 - dst->bpl;
        dptr = dst->data + dst->clip_win.y2 * dst->bpl;
    }
    else {
        dbpl = dst->bpl;
        dptr = dst->data;
    }
    if (src->fmt == CR_INDEX8) {
        line = src->position.ww;
    }
    else
    if (src->fmt == CR_ARGB888) {
        line = src->position.ww;
        line *= 3;
    }
    else {
        line = src->position.ww;
        line *= 4;
    }

    /* 逐像素运算 */
    for (hh = src->position.hh; hh != 0; hh--) {
        for (idx = 0; idx < line; idx++) {
            val = dopix(dptr[idx], sptr[idx]);
            if (val <= 0)
                dptr[idx] = 0x00;
            else
            if (val >= 255)
                dptr[idx] = 0xFF;
            else
                dptr[idx] = (byte_t)val;
        }
        dptr += dbpl;
        sptr += sbpl;
    }
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
