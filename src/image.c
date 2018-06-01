/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2009-12-19  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 位图结构函数库 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#define _CR_USE_PIXELCVT_
#include "safe.h"
#include "memlib.h"
#include "pixels.h"

/*****************************************************************************/
/*                                 位图结构                                  */
/*****************************************************************************/

/*
=======================================
    设置线段数据 (通过四点)
=======================================
*/
CR_API void_t
line_set_xy (
  __CR_OT__ sLINE*  line,
  __CR_IN__ sint_t  x1,
  __CR_IN__ sint_t  y1,
  __CR_IN__ sint_t  x2,
  __CR_IN__ sint_t  y2
    )
{
    line->x1 = x1;
    line->y1 = y1;
    line->x2 = x2;
    line->y2 = y2;
    if (x2 >= x1)
        line->ww = x2 - x1 + 1;
    else
        line->ww = x1 - x2 + 1;
    if (y2 >= y1)
        line->hh = y2 - y1 + 1;
    else
        line->hh = y1 - y2 + 1;
}

/*
=======================================
    设置方框数据 (通过四点)
=======================================
*/
CR_API void_t
rect_set_xy (
  __CR_OT__ sRECT*  rect,
  __CR_IN__ sint_t  x1,
  __CR_IN__ sint_t  y1,
  __CR_IN__ sint_t  x2,
  __CR_IN__ sint_t  y2
    )
{
    rect->x1 = x1;
    rect->y1 = y1;
    rect->x2 = x2;
    rect->y2 = y2;
    rect->ww = x2 - x1 + 1;
    rect->hh = y2 - y1 + 1;
    rect_unique(rect);
}

/*
=======================================
    设置方框数据 (通过宽高)
=======================================
*/
CR_API void_t
rect_set_wh (
  __CR_OT__ sRECT*  rect,
  __CR_IN__ sint_t  x1,
  __CR_IN__ sint_t  y1,
  __CR_IN__ uint_t  ww,
  __CR_IN__ uint_t  hh
    )
{
    if (ww == 0)
        ww = 1;
    if (hh == 0)
        hh = 1;
    rect->x1 = x1;
    rect->y1 = y1;
    rect->x2 = x1 + ww - 1;
    rect->y2 = y1 + hh - 1;
    rect->ww = ww;
    rect->hh = hh;
}

/*
=======================================
    矫正方框的方向
=======================================
*/
CR_API void_t
rect_unique (
  __CR_IO__ sRECT*  rect
    )
{
    sint_t  tmp;

    if (rect->x1 > rect->x2) {
        CR_SWAP(rect->x1, rect->x2, tmp);
        rect->ww = rect->x2 - rect->x1 + 1;
    }
    if (rect->y1 > rect->y2) {
        CR_SWAP(rect->y1, rect->y2, tmp);
        rect->hh = rect->y2 - rect->y1 + 1;
    }
}

/*
=======================================
    方框水平对齐
=======================================
*/
CR_API void_t
rect_align_x (
  __CR_IO__ sRECT*          dest,
  __CR_IN__ const sRECT*    rect,
  __CR_IN__ uint_t          type
    )
{
    uint_t  center;

    /* 超出大小使用源大小 */
    if (dest->ww > rect->ww) {
        dest->x1 = rect->x1;
        dest->x2 = rect->x2;
        dest->ww = rect->ww;
    }
    else
    if (type == RCT_ALIGN_HSB) {
        center = (rect->ww - dest->ww) / 2;
        dest->x1 = rect->x1 + center;
        dest->x2 = dest->x1 + dest->ww - 1;
    }
    else
    if (type == RCT_ALIGN_LSB) {
        dest->x1 = rect->x1;
        dest->x2 = dest->x1 + dest->ww - 1;
    }
    else
    if (type == RCT_ALIGN_MSB) {
        dest->x2 = rect->x2;
        dest->x1 = dest->x2 - dest->ww + 1;
    }
}

/*
=======================================
    方框垂直对齐
=======================================
*/
CR_API void_t
rect_align_y (
  __CR_IO__ sRECT*          dest,
  __CR_IN__ const sRECT*    rect,
  __CR_IN__ uint_t          type
    )
{
    uint_t  center;

    /* 超出大小使用源大小 */
    if (dest->hh > rect->hh) {
        dest->y1 = rect->y1;
        dest->y2 = rect->y2;
        dest->hh = rect->hh;
    }
    else
    if (type == RCT_ALIGN_HSB) {
        center = (rect->hh - dest->hh) / 2;
        dest->y1 = rect->y1 + center;
        dest->y2 = dest->y1 + dest->hh - 1;
    }
    else
    if (type == RCT_ALIGN_LSB) {
        dest->y1 = rect->y1;
        dest->y2 = dest->y1 + dest->hh - 1;
    }
    else
    if (type == RCT_ALIGN_MSB) {
        dest->y2 = rect->y2;
        dest->y1 = dest->y2 - dest->hh + 1;
    }
}

/*
=======================================
    计算位图基本信息
=======================================
*/
CR_API bool_t
image_info (
  __CR_IO__ sIMAGE* img,
  __CR_IN__ uint_t  w,
  __CR_IN__ uint_t  h,
  __CR_IN__ uint_t  fmt,
  __CR_IN__ bool_t  gdi,
  __CR_IN__ uint_t  align
    )
{
    leng_t  bpc;

    if (img->fmt != CR_UNKNOWN)
    {
        /* 已填充了数据, 取现有数据 */
        bpc = img->bpc;
        fmt = img->fmt;
        w = img->position.ww;
        h = img->position.hh;
    }
    else
    {
        /* 新的结构, 根据参数计算数据 */
        switch (fmt)
        {
            case CR_INDEX1:
                if (w % 8 == 0)
                    w = w / 8;
                else
                    w = w / 8 + 1;
                bpc = 1;
                img->bpc = 1;
                break;

            case CR_INDEX2:
                if (w % 4 == 0)
                    w = w / 4;
                else
                    w = w / 4 + 1;
                bpc = 1;
                img->bpc = 1;
                break;

            case CR_INDEX4:
                if (w % 2 == 0)
                    w = w / 2;
                else
                    w = w / 2 + 1;
                bpc = 1;
                img->bpc = 1;
                break;

            case CR_INDEX8:
                bpc = 1;
                img->bpc = 1;
                break;

            case CR_ARGB565:
            case CR_ARGB4444:
            case CR_ARGBX555:
            case CR_ARGB1555:
                bpc = 2;
                img->bpc = 2;
                break;

            case CR_ARGB888:
                bpc = 3;
                img->bpc = 3;
                break;

            case CR_ARGB8888:
                bpc = 4;
                img->bpc = 4;
                break;

            case CR_DXT1:
            case CR_DXT2:
            case CR_DXT3:
            case CR_DXT4:
            case CR_DXT5:
                if (w % 4 != 0 || h % 4 != 0)
                    return (FALSE);
                bpc = 0;
                gdi = FALSE;
                img->bpc = 0;
                break;

            default:
                return (FALSE);
        }

        /* 安全检查 */
        if (cut_image(NULL, w, h, bpc))
            return (FALSE);
        bpc *= w;

        /* 计算对齐和 BPL */
        if (align != 0)
        {
            if (CR_TEST2N(align)) {
                img->align = align;
                align = (uint_t)CR_PADDED(bpc, align);
            }
            else {
                return (FALSE);
            }
        }
        else
        {
            img->align = 0;
        }

        /* 安全检查 */
        if (cut_addu(&img->bpl, bpc, align))
            return (FALSE);
        img->fmt = fmt;
        img->gdi = gdi;
    }

    if (bpc != 0)
    {
        /* 安全检查 */
        if (cut_mul(&img->size, img->bpl, h))
            return (FALSE);
    }
    else
    {
        /* DXTC 格式 */
        if (fmt != CR_DXT1)
            img->size = w * h;
        else
            img->size = w * h / 2;
        img->bpl = 0;
        img->align = 0;
    }
    return (TRUE);
}

/*
=======================================
    新建位图
=======================================
*/
CR_API sIMAGE*
image_new (
  __CR_IN__ sint_t  x,
  __CR_IN__ sint_t  y,
  __CR_IN__ uint_t  w,
  __CR_IN__ uint_t  h,
  __CR_IN__ uint_t  fmt,
  __CR_IN__ bool_t  gdi,
  __CR_IN__ uint_t  align
    )
{
    sIMAGE* ret;

    /* 生成位图结构 */
    ret = struct_new(sIMAGE);
    if (ret == NULL)
        return (NULL);
    struct_zero(ret, sIMAGE);

    /* 计算位图大小相关参数 */
    if (!image_info(ret, w, h, fmt, gdi, align)) {
        mem_free(ret);
        return (NULL);
    }

    /* 分配位图缓存 */
    ret->data = (byte_t*)mem_malloc(ret->size);
    if (ret->data == NULL) {
        mem_free(ret);
        return (NULL);
    }
    rect_set_wh(&ret->clip_win, 0, 0, w, h);
    rect_set_wh(&ret->position, x, y, w, h);
    return (ret);
}

/*
=======================================
    填充位图结构
=======================================
*/
CR_API bool_t
image_set (
  __CR_OT__ sIMAGE*         img,
  __CR_IN__ const void_t*   pixel,
  __CR_IN__ leng_t          size,
  __CR_IN__ sint_t          x,
  __CR_IN__ sint_t          y,
  __CR_IN__ uint_t          w,
  __CR_IN__ uint_t          h,
  __CR_IN__ uint_t          fmt,
  __CR_IN__ bool_t          gdi,
  __CR_IN__ uint_t          align
    )
{
    /* 计算位图大小相关参数 */
    struct_zero(img, sIMAGE);
    if (!image_info(img, w, h, fmt, gdi, align))
        return (FALSE);

    /* 欠缓冲检查 */
    if (size < img->size)
        return (FALSE);
    img->data = (uchar*)pixel;
    rect_set_wh(&img->clip_win, 0, 0, w, h);
    rect_set_wh(&img->position, x, y, w, h);
    return (TRUE);
}

/*
=======================================
    释放位图
=======================================
*/
CR_API void_t
image_del (
  __CR_IN__ const sIMAGE*   img
    )
{
    mem_free(img->data);
    mem_free(img);
}

/*
=======================================
    克隆位图
=======================================
*/
CR_API sIMAGE*
image_dup (
  __CR_IN__ const sIMAGE*   src
    )
{
    sIMAGE* dst;

    dst = struct_dup(src, sIMAGE);
    if (dst == NULL)
        return (NULL);

    dst->data = mem_dup(src->data, src->size);
    if (dst->data == NULL) {
        mem_free(dst);
        return (NULL);
    }
    return (dst);
}

/*
=======================================
    垂直翻转图片 (若需要)
=======================================
*/
CR_API void_t
image_fuck_gdi (
  __CR_IO__ sIMAGE* img
    )
{
    if (img->gdi) {
        image_flp(img, FALSE);
        img->gdi = FALSE;
    }
}

/*
=======================================
    位图垂直翻转
=======================================
*/
CR_API void_t
image_flp (
  __CR_IO__ const sIMAGE*   img,
  __CR_IN__ bool_t          solid
    )
{
    byte_t  tmp;
    uint_t  yyy;
    leng_t  pos;
    leng_t  wtmp;
    byte_t* img1;
    byte_t* img2;

    if (isCrTypeCompr(img->fmt))
        return;
    wtmp = img->bpl;
    img1 = img->data;
    img2 = img->data + img->clip_win.y2 * img->bpl;

    for (yyy = img->position.hh / 2; yyy != 0; yyy--) {
        for (pos = 0; pos < wtmp; pos++) {
            CR_SWAP(img1[pos], img2[pos], tmp);
        }
        img1 += img->bpl;
        img2 -= img->bpl;
    }

    /* 取消 Alpha 通道 */
    if (solid && img->fmt == CR_ARGB8888) {
        img1 = img->data;
        for (pos = img->size / 4; pos != 0; pos--, img1 += 4)
            img1[3] = 0xFF;
    }
}

/*
=======================================
    根据透明色设置 Alpha 通道
=======================================
*/
CR_API void_t
image_alptran32 (
  __CR_IO__ const sIMAGE*   img
    )
{
    int32u  temp;
    leng_t  size;
    byte_t* data;

    if (img->fmt != CR_ARGB8888)
        return;
    if (img->keycolor != 0) {
        size = img->size;
        data = img->data;
        temp = img->keycolor & CDWORD_LE(0x00FFFFFFUL);
        for (size /= 4; size != 0; size--) {
            if (temp == (*(int32u*)data & CDWORD_LE(0x00FFFFFFUL)))
                data[3] = 0x00;
            data += sizeof(int32u);
        }
    }
}

/*
=======================================
    计算32位图片预乘
=======================================
*/
CR_API void_t
image_pre_mul32 (
  __CR_IO__ const sIMAGE*   img
    )
{
    byte_t  temp;
    leng_t  size;
    byte_t* data;

    if (img->fmt != CR_ARGB8888)
        return;
    size = img->size;
    data = img->data;
    for (size /= 4; size != 0; size--) {
        temp = data[3];
        data[0] = (uchar)(((uint_t)data[0] * temp) / 255);
        data[1] = (uchar)(((uint_t)data[1] * temp) / 255);
        data[2] = (uchar)(((uint_t)data[2] * temp) / 255);
        data += sizeof(int32u);
    }
}

/*
=======================================
    32位位图垂直翻转 + RB 互换
=======================================
*/
CR_API void_t
image_gdi_flp32 (
  __CR_IO__ const sIMAGE*   img
    )
{
    int32u  temp;
    uint_t  x, y;
    uint_t  wtmp;
    int32u* img1;
    int32u* img2;

    if (img->fmt != CR_ARGB8888)
        return;
    wtmp = img->position.ww;
    img1 = (int32u*)(img->data);
    img2 = (int32u*)(img->data + img->clip_win.y2 * img->bpl);

    for (y = img->position.hh / 2; y != 0; y--) {
        for (x = 0; x < wtmp; x++) {
            argb32_to_gdi(&img1[x]);
            argb32_to_gdi(&img2[x]);
            CR_SWAP(img1[x], img2[x], temp);
        }
        img1 = (int32u*)((uchar*)img1 + img->bpl);
        img2 = (int32u*)((uchar*)img2 - img->bpl);
    }
}

/*
=======================================
    交换位图的字节顺序
=======================================
*/
CR_API void_t
image_swap (
  __CR_IO__ const sIMAGE*   img
    )
{
    leng_t  nn;
    int16u* ww;
    int32u* dd;
    int64u* qq;

    switch (img->bpc)
    {
        default:
        case 1:
            break;

        case 2:
            nn = img->size / 2;
            ww = (int16u*)(img->data);

            for (; nn != 0; nn--, ww++)
                *ww = xchg_int16u(*ww);
            break;

        case 4:
            nn = img->size / 4;
            dd = (int32u*)(img->data);

            for (; nn != 0; nn--, dd++)
                *dd = xchg_int32u(*dd);
            break;

        case 8:
            nn = img->size / 8;
            qq = (int64u*)(img->data);

            for (; nn != 0; nn--, qq++)
                *qq = xchg_int64u(*qq);
            break;
    }
}

/*
=======================================
    拆分图片透明通道 (返回灰度图)
=======================================
*/
CR_API sIMAGE*
image_get_alpha (
  __CR_IN__ const sIMAGE*   img
    )
{
    uint_t  xx;
    uint_t  yy;
    uint_t  ww;
    uint_t  hh;
    byte_t* src;
    byte_t* dst;
    sIMAGE* alp;

    /* 源图必须要有透明通道 */
    if (img->fmt != CR_ARGB4444 &&
        img->fmt != CR_ARGB1555 &&
        img->fmt != CR_ARGB8888)
        return (NULL);

    /* 生成8位的灰度图 */
    alp = image_new(img->position.x1, img->position.y1,
                    img->position.ww, img->position.hh,
                    CR_INDEX8, img->gdi, img->align);
    if (alp == NULL)
        return (NULL);
    pal_set_gray8(alp->pal, 256);

    /* 开始逐个像素转换 */
    dst = alp->data;
    src = img->data;
    ww = img->position.ww;
    hh = img->position.hh;
    switch (img->fmt)
    {
        default:
        case CR_ARGB8888:
            for (yy = 0; yy < hh; yy++) {
                for (xx = 0; xx < ww; xx++)
                    dst[xx] = src[xx * 4 + 3];
                dst += alp->bpl;
                src += img->bpl;
            }
            break;

        case CR_ARGB4444:
            for (yy = 0; yy < hh; yy++) {
                for (xx = 0; xx < ww; xx++)
                    dst[xx] = g_by04to256[src[xx * 2 + 1] >> 4];
                dst += alp->bpl;
                src += img->bpl;
            }
            break;

        case CR_ARGB1555:
            for (yy = 0; yy < hh; yy++) {
                for (xx = 0; xx < ww; xx++)
                    dst[xx] = g_by02to256[src[xx * 2 + 1] >> 7];
                dst += alp->bpl;
                src += img->bpl;
            }
            break;
    }
    return (alp);
}

/*
=======================================
    合并图片透明通道 (传入灰度图)
=======================================
*/
CR_API void_t
image_set_alpha (
  __CR_IO__ const sIMAGE*   img,
  __CR_IN__ const sIMAGE*   alp
    )
{
    uint_t  xx;
    uint_t  yy;
    uint_t  ww;
    uint_t  hh;
    byte_t* src;
    byte_t* dst;

    /* 必须要是灰度图 */
    if (alp->fmt != CR_INDEX8)
        return;

    /* 两个图的属性要匹配 */
    if (alp->gdi != img->gdi)
        return;

    /* 源图必须要有透明通道 */
    if (img->fmt != CR_ARGB4444 &&
        img->fmt != CR_ARGB1555 &&
        img->fmt != CR_ARGB8888)
        return;

    /* 开始逐个像素转换 */
    src = alp->data;
    dst = img->data;
    if (alp->position.ww > img->position.ww)
        ww = img->position.ww;
    else
        ww = alp->position.ww;
    if (alp->position.hh > img->position.hh)
        hh = img->position.hh;
    else
        hh = alp->position.hh;
    switch (img->fmt)
    {
        default:
        case CR_ARGB8888:
            for (yy = 0; yy < hh; yy++) {
                for (xx = 0; xx < ww; xx++)
                    dst[xx * 4 + 3] = src[xx];
                src += alp->bpl;
                dst += img->bpl;
            }
            break;

        case CR_ARGB4444:
            for (yy = 0; yy < hh; yy++) {
                for (xx = 0; xx < ww; xx++) {
                    dst[xx * 2 + 1] &= 0x0F;
                    dst[xx * 2 + 1] |= src[xx] & 0xF0;
                }
                src += alp->bpl;
                dst += img->bpl;
            }
            break;

        case CR_ARGB1555:
            for (yy = 0; yy < hh; yy++) {
                for (xx = 0; xx < ww; xx++) {
                    dst[xx * 2 + 1] &= 0x7F;
                    dst[xx * 2 + 1] |= src[xx] & 0x80;
                }
                src += alp->bpl;
                dst += img->bpl;
            }
            break;
    }
}

/*
=======================================
    拆分图片指定通道
=======================================
*/
CR_API sIMAGE*
image_get_chn32 (
  __CR_IN__ const sIMAGE*   img,
  __CR_IN__ uint_t          offset
    )
{
    uint_t  xx;
    uint_t  yy;
    uint_t  ww;
    uint_t  hh;
    byte_t* src;
    byte_t* dst;
    sIMAGE* chn;

    /* 参数过滤 */
    if (offset > 3 || img->fmt != CR_ARGB8888)
        return (NULL);

    /* 生成8位的灰度图 */
    chn = image_new(img->position.x1, img->position.y1,
                    img->position.ww, img->position.hh,
                    CR_INDEX8, img->gdi, img->align);
    if (chn == NULL)
        return (NULL);
    pal_set_gray8(chn->pal, 256);

    /* 逐像素抽取 */
    dst = chn->data;
    src = img->data;
    ww = img->position.ww;
    hh = img->position.hh;
    for (yy = 0; yy < hh; yy++) {
        for (xx = 0; xx < ww; xx++)
            dst[xx] = src[xx * 4 + offset];
        dst += chn->bpl;
        src += img->bpl;
    }
    return (chn);
}

/*
=======================================
    合并图片指定通道
=======================================
*/
CR_API void_t
image_set_chn32 (
  __CR_IO__ const sIMAGE*   img,
  __CR_IN__ const sIMAGE*   chn,
  __CR_IN__ uint_t          offset
    )
{
    uint_t  xx;
    uint_t  yy;
    uint_t  ww;
    uint_t  hh;
    byte_t* src;
    byte_t* dst;

    /* 参数过滤 */
    if (offset > 3 || chn->gdi != img->gdi ||
        chn->fmt != CR_INDEX8 || img->fmt != CR_ARGB8888)
        return;

    /* 逐像素设置 */
    src = chn->data;
    dst = img->data;
    if (chn->position.ww > img->position.ww)
        ww = img->position.ww;
    else
        ww = chn->position.ww;
    if (chn->position.hh > img->position.hh)
        hh = img->position.hh;
    else
        hh = chn->position.hh;
    for (yy = 0; yy < hh; yy++) {
        for (xx = 0; xx < ww; xx++)
            dst[xx * 4 + offset] = src[xx];
        src += chn->bpl;
        dst += img->bpl;
    }
}

/*
=======================================
    分块转线性图片
=======================================
*/
CR_API bool_t
img_tile2line (
  __CR_IO__ sIMAGE* img,
  __CR_IN__ uint_t  tile_w,
  __CR_IN__ uint_t  tile_h
    )
{
    uchar*  lines;
    uchar*  image;
    uchar*  start;
    leng_t  tile_bpl;
    uint_t  x, y, idx;

    /* 安全检查 */
    if (img->bpc == 0 ||
        img->position.ww % tile_w != 0 ||
        img->position.hh % tile_h != 0)
        return (FALSE);

    /* bpc 小于1字节的情况 */
    if (img->fmt == CR_INDEX4)
    {
        if (tile_w % 2 != 0)
            return (FALSE);
        tile_w /= 2;
    }
    else
    if (img->fmt == CR_INDEX2)
    {
        if (tile_w % 4 != 0)
            return (FALSE);
        tile_w /= 4;
    }
    else
    if (img->fmt == CR_INDEX1)
    {
        if (tile_w % 8 != 0)
            return (FALSE);
        tile_w /= 8;
    }
    tile_bpl = tile_w * img->bpc;
    image = (uchar*)img->data;
    lines = (uchar*)mem_malloc(img->size);
    if (lines == NULL)
        return (FALSE);

    for (y = 0; y < img->position.hh; y += tile_h)
    for (x = 0; x < img->position.ww; x += tile_w)
    {
        /* 计算行列地址 */
        start = lines + y * img->bpl + x * img->bpc;

        for (idx = tile_h; idx != 0; idx--)
        {
            mem_cpy(start, image, tile_bpl);
            image += tile_bpl;
            start += img->bpl;
        }
    }
    mem_free(img->data);
    img->data = lines;
    return (TRUE);
}

/*
=======================================
    线性转分块图片
=======================================
*/
CR_API bool_t
img_line2tile (
  __CR_IO__ sIMAGE* img,
  __CR_IN__ uint_t  tile_w,
  __CR_IN__ uint_t  tile_h
    )
{
    uchar*  tiled;
    uchar*  ttemp;
    uchar*  image;
    uchar*  start;
    leng_t  tile_bpl;
    uint_t  x, y, idx;

    /* 安全检查 */
    if (img->bpc == 0 ||
        img->position.ww % tile_w != 0 ||
        img->position.hh % tile_h != 0)
        return (FALSE);

    /* bpc 小于1字节的情况 */
    if (img->fmt == CR_INDEX4)
    {
        if (tile_w % 2 != 0)
            return (FALSE);
        tile_w /= 2;
    }
    else
    if (img->fmt == CR_INDEX2)
    {
        if (tile_w % 4 != 0)
            return (FALSE);
        tile_w /= 4;
    }
    else
    if (img->fmt == CR_INDEX1)
    {
        if (tile_w % 8 != 0)
            return (FALSE);
        tile_w /= 8;
    }
    tile_bpl = tile_w * img->bpc;
    image = (uchar*)img->data;
    tiled = ttemp = (uchar*)mem_malloc(img->size);
    if (ttemp == NULL)
        return (FALSE);

    for (y = 0; y < img->position.hh; y += tile_h)
    for (x = 0; x < img->position.ww; x += tile_w)
    {
        /* 计算行列地址 */
        start = image + y * img->bpl + x * img->bpc;

        for (idx = tile_h; idx != 0; idx--)
        {
            mem_cpy(ttemp, start, tile_bpl);
            start += img->bpl;
            ttemp += tile_bpl;
        }
    }
    mem_free(img->data);
    img->data = tiled;
    return (TRUE);
}

/*****************************************************************************/
/*                                 位图绘制                                  */
/*****************************************************************************/

/*
=======================================
    计算填充操作的 BLTer
=======================================
*/
CR_API void_t
blt_use_fill (
  __CR_OT__ sBLTer*         blt,
  __CR_IN__ const sIMAGE*   dst,
  __CR_IN__ leng_t          bpc,
  __CR_IN__ const sFILL*    fill
    )
{
    /* 假设传入的参数正确无误, 自行保证 */
    blt->width = fill->dw * bpc;
    blt->height = fill->dh;

    if (!dst->gdi)
    {
        blt->rest = (dist_t)(dst->bpl - blt->width);
        blt->addr = dst->data + dst->bpl * fill->dy
                              +      bpc * fill->dx;
    }
    else
    {
        blt->rest = (dist_t)(0 - dst->bpl - blt->width);
        blt->addr = dst->data + dst->bpl * (dst->clip_win.y2 - fill->dy)
                              +      bpc *  fill->dx;
    }
}

/*
=======================================
    计算填充操作的 BLTer (支持剪裁)
=======================================
*/
CR_API bool_t
blt_use_fill_clip (
  __CR_OT__ sBLTer*         blt,
  __CR_IN__ const sIMAGE*   dst,
  __CR_IN__ leng_t          bpc,
  __CR_IN__ const sFILL*    fill,
  __CR_IN__ const sRECT*    rect
    )
{
    sFILL   temp;

    if (rect == NULL)
        rect = &dst->clip_win;

    if (!clip_fill(&temp, fill, rect))
        return (FALSE);
    blt_use_fill(blt, dst, bpc, &temp);
    return (TRUE);
}

/*
=======================================
    计算 BLT 操作的 BLTer
=======================================
*/
CR_API void_t
blt_use_blit (
  __CR_OT__ sBLTer*         dst_blt,
  __CR_OT__ sBLTer*         src_blt,
  __CR_IN__ leng_t          dst_bpc,
  __CR_IN__ leng_t          src_bpc,
  __CR_IN__ const sIMAGE*   dst,
  __CR_IN__ const sIMAGE*   src,
  __CR_IN__ const sBLIT*    blit
    )
{
    /* 假设传入的参数正确无误, 自行保证 */
    dst_blt->width = blit->sw * dst_bpc;
    src_blt->width = blit->sw * src_bpc;
    dst_blt->height = blit->sh;
    src_blt->height = blit->sh;

    if (!dst->gdi)
    {
        dst_blt->rest = (dist_t)(dst->bpl - dst_blt->width);
        dst_blt->addr = dst->data + dst->bpl * blit->dy
                                  +  dst_bpc * blit->dx;
    }
    else
    {
        dst_blt->rest = (dist_t)(0 - dst->bpl - dst_blt->width);
        dst_blt->addr = dst->data + dst->bpl * (dst->clip_win.y2 - blit->dy)
                                  +  dst_bpc *  blit->dx;
    }

    if (!src->gdi)
    {
        src_blt->rest = (dist_t)(src->bpl - src_blt->width);
        src_blt->addr = src->data + src->bpl * blit->sy
                                  +  src_bpc * blit->sx;
    }
    else
    {
        src_blt->rest = (dist_t)(0 - src->bpl - src_blt->width);
        src_blt->addr = src->data + src->bpl * (src->clip_win.y2 - blit->sy)
                                  +  src_bpc *  blit->sx;
    }
}

/*
=======================================
    计算 BLT 操作的 BLTer (支持剪裁)
=======================================
*/
CR_API bool_t
blt_use_blit_clip (
  __CR_OT__ sBLTer*         dst_blt,
  __CR_OT__ sBLTer*         src_blt,
  __CR_IN__ leng_t          dst_bpc,
  __CR_IN__ leng_t          src_bpc,
  __CR_IN__ const sIMAGE*   dst,
  __CR_IN__ const sIMAGE*   src,
  __CR_IN__ const sBLIT*    blit,
  __CR_IN__ const sRECT*    rect
    )
{
    sBLIT   temp;

    if (rect == NULL)
        rect = &dst->clip_win;

    if (!clip_blit(&temp, blit, rect))
        return (FALSE);
    blt_use_blit(dst_blt, src_blt,
                 dst_bpc, src_bpc,
                 dst, src, &temp);
    return (TRUE);
}

/*
=======================================
    计算缩放操作的 BLTer
=======================================
*/
CR_API void_t
blt_use_zoom (
  __CR_OT__ sBLTer*         dst_blt,
  __CR_OT__ sBLTer*         src_blt,
  __CR_IN__ leng_t          dst_bpc,
  __CR_IN__ leng_t          src_bpc,
  __CR_IN__ const sIMAGE*   dst,
  __CR_IN__ const sIMAGE*   src,
  __CR_IN__ const sZOOM*    zoom
    )
{
    /* 假设传入的参数正确无误, 自行保证 */
    dst_blt->width = zoom->dw * dst_bpc;
    src_blt->width = zoom->sw * src_bpc;
    dst_blt->height = zoom->dh;
    src_blt->height = zoom->sh;

    if (!dst->gdi)
    {
        dst_blt->rest = (dist_t)(dst->bpl - dst_blt->width);
        dst_blt->addr = dst->data + dst->bpl * zoom->dy
                                  +  dst_bpc * zoom->dx;
    }
    else
    {
        dst_blt->rest = (dist_t)(0 - dst->bpl - dst_blt->width);
        dst_blt->addr = dst->data + dst->bpl * (dst->clip_win.y2 - zoom->dy)
                                  +  dst_bpc *  zoom->dx;
    }

    if (!src->gdi)
    {
        src_blt->rest = (dist_t)(src->bpl - src_blt->width);
        src_blt->addr = src->data + src->bpl * zoom->sy
                                  +  src_bpc * zoom->sx;
    }
    else
    {
        src_blt->rest = (dist_t)(0 - src->bpl - src_blt->width);
        src_blt->addr = src->data + src->bpl * (src->clip_win.y2 - zoom->sy)
                                  +  src_bpc *  zoom->sx;
    }
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
