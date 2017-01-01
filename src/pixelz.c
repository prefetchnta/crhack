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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 像素操作函数库 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#define _CR_USE_PIXELCVT_
#include "img2d.h"
#include "memlib.h"
#include "pixels.h"

/*****************************************************************************/
/*                              单像素计算操作                               */
/*****************************************************************************/

/*
=======================================
    计算像素的亮度值
=======================================
*/
CR_API uint_t
rgb2light (
  __CR_IN__ uint_t  r,
  __CR_IN__ uint_t  g,
  __CR_IN__ uint_t  b
    )
{
    int32u  ir, ig, ib;

    ir = r * 299UL;
    ig = g * 587UL;
    ib = b * 114UL;
    ir += ig + ib + 500UL;
    ir /= 1000UL;
    return ((uint_t)ir);
}

/*****************************************************************************/
/*                              调色板格式转换                               */
/*****************************************************************************/

/*
=======================================
    调色板 666to888
=======================================
*/
CR_API void_t
pal_vga_to_3b (
  __CR_IO__ void_t* vga3,
  __CR_IN__ uint_t  len
    )
{
    for (; len != 0; len--) {
        ((uchar*)vga3)[0] = g_by64to256[((uchar*)vga3)[0] & 0x3F];
        ((uchar*)vga3)[1] = g_by64to256[((uchar*)vga3)[1] & 0x3F];
        ((uchar*)vga3)[2] = g_by64to256[((uchar*)vga3)[2] & 0x3F];
        vga3 = (uchar*)vga3 + 3;
    }
}

/*
=======================================
    调色板 888to666
=======================================
*/
CR_API void_t
pal_3b_to_vga (
  __CR_IO__ void_t* pal3,
  __CR_IN__ uint_t  len
    )
{
    for (; len != 0; len--) {
        ((uchar*)pal3)[0] >>= 2;
        ((uchar*)pal3)[1] >>= 2;
        ((uchar*)pal3)[2] >>= 2;
        pal3 = (uchar*)pal3 + 3;
    }
}

/*
=======================================
    调色板 888to8888
=======================================
*/
CR_API void_t
pal_3b_to_4b (
  __CR_OT__ void_t*         pal4,
  __CR_IN__ const void_t*   pal3,
  __CR_IN__ uint_t          len
    )
{
    for (; len != 0; len--) {
        ((uchar*)pal4)[0] = ((uchar*)pal3)[0];
        ((uchar*)pal4)[1] = ((uchar*)pal3)[1];
        ((uchar*)pal4)[2] = ((uchar*)pal3)[2];
        ((uchar*)pal4)[3] = 0xFF;
        pal3 = (uchar*)pal3 + 3;
        pal4 = (uchar*)pal4 + 4;
    }
}

/*
=======================================
    调色板 8888to888 (支持自操作)
=======================================
*/
CR_API void_t
pal_4b_to_3b (
  __CR_OT__ void_t*         pal3,
  __CR_IN__ const void_t*   pal4,
  __CR_IN__ uint_t          len
    )
{
    for (; len != 0; len--) {
        ((uchar*)pal3)[0] = ((uchar*)pal4)[0];
        ((uchar*)pal3)[1] = ((uchar*)pal4)[1];
        ((uchar*)pal3)[2] = ((uchar*)pal4)[2];
        pal4 = (uchar*)pal4 + 4;
        pal3 = (uchar*)pal3 + 3;
    }
}

/*
=======================================
    调色板 888to8888 (RB 交换)
=======================================
*/
CR_API void_t
pal_3b_to_4b_sw (
  __CR_OT__ void_t*         pal4,
  __CR_IN__ const void_t*   pal3,
  __CR_IN__ uint_t          len
    )
{
    for (; len != 0; len--) {
        ((uchar*)pal4)[0] = ((uchar*)pal3)[2];
        ((uchar*)pal4)[1] = ((uchar*)pal3)[1];
        ((uchar*)pal4)[2] = ((uchar*)pal3)[0];
        ((uchar*)pal4)[3] = 0xFF;
        pal3 = (uchar*)pal3 + 3;
        pal4 = (uchar*)pal4 + 4;
    }
}

/*
=======================================
    调色板 8888to888 (支持自操作 & RB 交换)
=======================================
*/
CR_API void_t
pal_4b_to_3b_sw (
  __CR_OT__ void_t*         pal3,
  __CR_IN__ const void_t*   pal4,
  __CR_IN__ uint_t          len
    )
{
    for (; len != 0; len--) {
        ((uchar*)pal3)[0] = ((uchar*)pal4)[2];
        ((uchar*)pal3)[1] = ((uchar*)pal4)[1];
        ((uchar*)pal3)[2] = ((uchar*)pal4)[0];
        pal4 = (uchar*)pal4 + 4;
        pal3 = (uchar*)pal3 + 3;
    }
}

/*
=======================================
    调色板 8888to565 (支持自操作)
=======================================
*/
CR_API void_t
pal_4b_to_2b_16 (
  __CR_OT__ void_t*         pal2,
  __CR_IN__ const void_t*   pal4,
  __CR_IN__ uint_t          len
    )
{
    for (; len != 0; len--) {
        pixel32to565((int16u*)pal2, *(int32u*)pal4);
        pal4 = (byte_t*)pal4 + sizeof(int32u);
        pal2 = (byte_t*)pal2 + sizeof(int16u);
    }
}

/*
=======================================
    调色板 8888toX555 (支持自操作)
=======================================
*/
CR_API void_t
pal_4b_to_2b_15 (
  __CR_OT__ void_t*         pal2,
  __CR_IN__ const void_t*   pal4,
  __CR_IN__ uint_t          len
    )
{
    for (; len != 0; len--) {
        pixel32toX555((int16u*)pal2, *(int32u*)pal4);
        pal4 = (byte_t*)pal4 + sizeof(int32u);
        pal2 = (byte_t*)pal2 + sizeof(int16u);
    }
}

/*
=======================================
    调色板 8888to1555 (支持自操作)
=======================================
*/
CR_API void_t
pal_4b_to_2b_17 (
  __CR_OT__ void_t*         pal2,
  __CR_IN__ const void_t*   pal4,
  __CR_IN__ uint_t          len
    )
{
    for (; len != 0; len--) {
        pixel32to1555((int16u*)pal2, *(int32u*)pal4);
        pal4 = (byte_t*)pal4 + sizeof(int32u);
        pal2 = (byte_t*)pal2 + sizeof(int16u);
    }
}

/*
=======================================
    调色板 8888to4444 (支持自操作)
=======================================
*/
CR_API void_t
pal_4b_to_2b_12 (
  __CR_OT__ void_t*         pal2,
  __CR_IN__ const void_t*   pal4,
  __CR_IN__ uint_t          len
    )
{
    for (; len != 0; len--) {
        pixel32to4444((int16u*)pal2, *(int32u*)pal4);
        pal4 = (byte_t*)pal4 + sizeof(int32u);
        pal2 = (byte_t*)pal2 + sizeof(int16u);
    }
}

/*
=======================================
    调色板设置8位灰度等级
=======================================
*/
CR_API void_t
pal_set_gray8 (
  __CR_OT__ void_t* pal4,
  __CR_IN__ uint_t  len
    )
{
    byte_t  clr = 0x00;

    for (; len != 0; len--, clr++) {
        ((uchar*)pal4)[0] = clr;
        ((uchar*)pal4)[1] = clr;
        ((uchar*)pal4)[2] = clr;
        ((uchar*)pal4)[3] = 0xFF;
        pal4 = (uchar*)pal4 + 4;
    }
}

/*
=======================================
    调色板 8888 RB 交换 & Alpha 设置
=======================================
*/
CR_API void_t
pal_4b_alp_sw (
  __CR_IO__ void_t* pal4,
  __CR_IN__ bool_t  swap,
  __CR_IN__ uint_t  alp,
  __CR_IN__ uint_t  len
    )
{
    byte_t  tmp;

    if (alp < 256) {
        if (swap) {
            for (; len != 0; len--) {
                tmp = ((uchar*)pal4)[0];
                ((uchar*)pal4)[0] = ((uchar*)pal4)[2];
                ((uchar*)pal4)[2] = tmp;
                ((uchar*)pal4)[3] = (byte_t)alp;
                pal4 = (uchar*)pal4 + 4;
            }
        }
        else {
            for (; len != 0; len--) {
                ((uchar*)pal4)[3] = (byte_t)alp;
                pal4 = (uchar*)pal4 + 4;
            }
        }
    }
    else {
        if (swap) {
            for (; len != 0; len--) {
                tmp = ((uchar*)pal4)[0];
                ((uchar*)pal4)[0] = ((uchar*)pal4)[2];
                ((uchar*)pal4)[2] = tmp;
                pal4 = (uchar*)pal4 + 4;
            }
        }
    }
}

/*****************************************************************************/
/*                              像素块格式转换                               */
/*****************************************************************************/

/*
=======================================
    图块转换安全检查
=======================================
*/
extern sIMAGE*
safe_convert (
  __CR_IO__ sIMAGE*         dst,
  __CR_IN__ uint_t          dst_fmt,
  __CR_IN__ const sIMAGE*   src,
  __CR_IN__ uint_t          src_fmt,
  __CR_IO__ uint_t*         dx,
  __CR_IO__ uint_t*         dy,
  __CR_OT__ uint_t*         width,
  __CR_OT__ uint_t*         height
    )
{
    uint_t  xx, yy;

    if (src->fmt != src_fmt)
        return (NULL);

    if (dst == NULL)
    {
        dst = image_new(src->position.x1, src->position.y1,
                        src->position.ww, src->position.hh,
                        dst_fmt, src->gdi, src->align);
        if (dst == NULL)
            return (NULL);
        *dx = 0;
        *dy = 0;
        *width  = src->position.ww;
        *height = src->position.hh;
        dst->keycolor = src->keycolor;
        mem_cpy(dst->pal, src->pal, 1024);
    }
    else
    {
        if (dst->fmt != dst_fmt)
            return (NULL);

        xx = *dx;
        if (xx >= dst->position.ww)
            return (NULL);
        yy = *dy;
        if (yy >= dst->position.hh)
            return (NULL);

        /* 传递透明色 */
        if (dst->keycolor == 0)
            dst->keycolor = src->keycolor;

        /* 剪裁到指定宽 (若需要) */
        if (src->position.ww > dst->position.ww - xx)
            *width = dst->position.ww - xx;
        else
            *width = src->position.ww;

        /* 剪裁到指定高 (若需要) */
        if (src->position.hh > dst->position.hh - yy)
            *height = dst->position.hh - yy;
        else
            *height = src->position.hh;
    }
    return ((sIMAGE*)dst);
}

/*
=======================================
    8888to565
=======================================
*/
CR_API sIMAGE*
img_32_to_565 (
  __CR_IO__ sIMAGE*         dst,
  __CR_IN__ uint_t          dx,
  __CR_IN__ uint_t          dy,
  __CR_IN__ const sIMAGE*   src
    )
{
    uint_t  x, y;
    dist_t  dbpl;
    dist_t  sbpl;
    uint_t  wtmp;
    uint_t  htmp;
    int16u* dtmp;
    int32u* stmp;

    dst = safe_convert(dst, CR_ARGB565, src, CR_ARGB8888,
                       &dx, &dy, &wtmp, &htmp);
    if (dst == NULL)
        return (NULL);

    CR_MAKE_SRC(00, 00, 4);
    CR_MAKE_DST(dx, dy, 2);

    for (y = htmp; y != 0; y--) {
        for (x = 0; x < wtmp; x++) {
            pixel32to565(&dtmp[x], stmp[x]);
        }
        dtmp = (int16u*)((uchar*)dtmp + dbpl);
        stmp = (int32u*)((uchar*)stmp + sbpl);
    }
    return ((sIMAGE*)dst);
}

/*
=======================================
    8888to888
=======================================
*/
CR_API sIMAGE*
img_32_to_888 (
  __CR_IO__ sIMAGE*         dst,
  __CR_IN__ uint_t          dx,
  __CR_IN__ uint_t          dy,
  __CR_IN__ const sIMAGE*   src
    )
{
    uint_t  x, y;
    dist_t  dbpl;
    dist_t  sbpl;
    uint_t  wtmp;
    uint_t  htmp;
    byte_t* dtmp;
    int32u* stmp;
    byte_t* dptr;
    byte_t* sptr;

    dst = safe_convert(dst, CR_ARGB888, src, CR_ARGB8888,
                       &dx, &dy, &wtmp, &htmp);
    if (dst == NULL)
        return (NULL);

    CR_MAKE_SRC(00, 00, 4);
    CR_MAKE_DST(dx, dy, 3);

    for (y = htmp; y != 0; y--) {
        dptr = (byte_t*)dtmp;
        sptr = (byte_t*)stmp;
        for (x = 0; x < wtmp; x++) {
            dptr[0] = sptr[0];
            dptr[1] = sptr[1];
            dptr[2] = sptr[2];
            dptr += 3;
            sptr += 4;
        }
        dtmp = (byte_t*)((uchar*)dtmp + dbpl);
        stmp = (int32u*)((uchar*)stmp + sbpl);
    }
    return ((sIMAGE*)dst);
}

/*
=======================================
    8888toX555
=======================================
*/
CR_API sIMAGE*
img_32_to_X555 (
  __CR_IO__ sIMAGE*         dst,
  __CR_IN__ uint_t          dx,
  __CR_IN__ uint_t          dy,
  __CR_IN__ const sIMAGE*   src
    )
{
    uint_t  x, y;
    dist_t  dbpl;
    dist_t  sbpl;
    uint_t  wtmp;
    uint_t  htmp;
    int16u* dtmp;
    int32u* stmp;

    dst = safe_convert(dst, CR_ARGBX555, src, CR_ARGB8888,
                       &dx, &dy, &wtmp, &htmp);
    if (dst == NULL)
        return (NULL);

    CR_MAKE_SRC(00, 00, 4);
    CR_MAKE_DST(dx, dy, 2);

    for (y = htmp; y != 0; y--) {
        for (x = 0; x < wtmp; x++) {
            pixel32toX555(&dtmp[x], stmp[x]);
        }
        dtmp = (int16u*)((uchar*)dtmp + dbpl);
        stmp = (int32u*)((uchar*)stmp + sbpl);
    }
    return ((sIMAGE*)dst);
}

/*
=======================================
    8888to1555
=======================================
*/
CR_API sIMAGE*
img_32_to_1555 (
  __CR_IO__ sIMAGE*         dst,
  __CR_IN__ uint_t          dx,
  __CR_IN__ uint_t          dy,
  __CR_IN__ const sIMAGE*   src
    )
{
    uint_t  x, y;
    dist_t  dbpl;
    dist_t  sbpl;
    uint_t  wtmp;
    uint_t  htmp;
    int16u* dtmp;
    int32u* stmp;

    dst = safe_convert(dst, CR_ARGB1555, src, CR_ARGB8888,
                       &dx, &dy, &wtmp, &htmp);
    if (dst == NULL)
        return (NULL);

    CR_MAKE_SRC(00, 00, 4);
    CR_MAKE_DST(dx, dy, 2);

    for (y = htmp; y != 0; y--) {
        for (x = 0; x < wtmp; x++) {
            pixel32to1555(&dtmp[x], stmp[x]);
        }
        dtmp = (int16u*)((uchar*)dtmp + dbpl);
        stmp = (int32u*)((uchar*)stmp + sbpl);
    }
    return ((sIMAGE*)dst);
}

/*
=======================================
    8888to4444
=======================================
*/
CR_API sIMAGE*
img_32_to_4444 (
  __CR_IO__ sIMAGE*         dst,
  __CR_IN__ uint_t          dx,
  __CR_IN__ uint_t          dy,
  __CR_IN__ const sIMAGE*   src
    )
{
    uint_t  x, y;
    dist_t  dbpl;
    dist_t  sbpl;
    uint_t  wtmp;
    uint_t  htmp;
    int16u* dtmp;
    int32u* stmp;

    dst = safe_convert(dst, CR_ARGB4444, src, CR_ARGB8888,
                       &dx, &dy, &wtmp, &htmp);
    if (dst == NULL)
        return (NULL);

    CR_MAKE_SRC(00, 00, 4);
    CR_MAKE_DST(dx, dy, 2);

    for (y = htmp; y != 0; y--) {
        for (x = 0; x < wtmp; x++) {
            pixel32to4444(&dtmp[x], stmp[x]);
        }
        dtmp = (int16u*)((uchar*)dtmp + dbpl);
        stmp = (int32u*)((uchar*)stmp + sbpl);
    }
    return ((sIMAGE*)dst);
}

/*
=======================================
    索引1to8888
=======================================
*/
CR_API sIMAGE*
img_idx1_to_32 (
  __CR_IO__ sIMAGE*         dst,
  __CR_IN__ uint_t          dx,
  __CR_IN__ uint_t          dy,
  __CR_IN__ const sIMAGE*   src
    )
{
    byte_t  bits;
    uint_t  x, y;
    dist_t  dbpl;
    dist_t  sbpl;
    uint_t  cnts;
    uint_t  rest;
    uint_t  wtmp;
    uint_t  htmp;
    int32u* dtmp;
    byte_t* stmp;

    dst = safe_convert(dst, CR_ARGB8888, src, CR_INDEX1,
                       &dx, &dy, &wtmp, &htmp);
    if (dst == NULL)
        return (NULL);

    CR_MAKE_SRC(00, 00, 1);
    CR_MAKE_DST(dx, dy, 4);

    rest = wtmp % 8;
    wtmp = wtmp - rest;
    for (y = htmp; y != 0; y--) {
        for (x = 0; x < wtmp; x += 8) {
            bits = stmp[x / 8];
            dtmp[x + 0] = src->pal[(bits >> 7) & 1];
            dtmp[x + 1] = src->pal[(bits >> 6) & 1];
            dtmp[x + 2] = src->pal[(bits >> 5) & 1];
            dtmp[x + 3] = src->pal[(bits >> 4) & 1];
            dtmp[x + 4] = src->pal[(bits >> 3) & 1];
            dtmp[x + 5] = src->pal[(bits >> 2) & 1];
            dtmp[x + 6] = src->pal[(bits >> 1) & 1];
            dtmp[x + 7] = src->pal[(bits >> 0) & 1];
        }

        /* 处理尾部一个字节 */
        bits = stmp[x / 8];
        for (cnts = 7; rest != 0; rest--, cnts--, x++)
            dtmp[x] = src->pal[(bits >> cnts) & 1];

        dtmp = (int32u*)((uchar*)dtmp + dbpl);
        stmp = (byte_t*)((uchar*)stmp + sbpl);
    }
    return ((sIMAGE*)dst);
}

/*
=======================================
    索引4to8888
=======================================
*/
CR_API sIMAGE*
img_idx4_to_32 (
  __CR_IO__ sIMAGE*         dst,
  __CR_IN__ uint_t          dx,
  __CR_IN__ uint_t          dy,
  __CR_IN__ const sIMAGE*   src
    )
{
    byte_t  bits;
    uint_t  x, y;
    dist_t  dbpl;
    dist_t  sbpl;
    uint_t  rest;
    uint_t  wtmp;
    uint_t  htmp;
    int32u* dtmp;
    byte_t* stmp;

    dst = safe_convert(dst, CR_ARGB8888, src, CR_INDEX4,
                       &dx, &dy, &wtmp, &htmp);
    if (dst == NULL)
        return (NULL);

    CR_MAKE_SRC(00, 00, 1);
    CR_MAKE_DST(dx, dy, 4);

    rest = wtmp % 2;
    wtmp = wtmp - rest;
    for (y = htmp; y != 0; y--) {
        for (x = 0; x < wtmp; x += 2) {
            bits = stmp[x / 2];
            dtmp[x + 0] = src->pal[(bits >> 4) & 15];
            dtmp[x + 1] = src->pal[(bits >> 0) & 15];
        }

        /* 处理尾部一个字节 */
        if (rest != 0)
            dtmp[x] = src->pal[(stmp[x / 2] >> 4) & 15];

        dtmp = (int32u*)((uchar*)dtmp + dbpl);
        stmp = (byte_t*)((uchar*)stmp + sbpl);
    }
    return ((sIMAGE*)dst);
}

/*
=======================================
    索引8to8888
=======================================
*/
CR_API sIMAGE*
img_idx8_to_32 (
  __CR_IO__ sIMAGE*         dst,
  __CR_IN__ uint_t          dx,
  __CR_IN__ uint_t          dy,
  __CR_IN__ const sIMAGE*   src
    )
{
    uint_t  x, y;
    dist_t  dbpl;
    dist_t  sbpl;
    uint_t  wtmp;
    uint_t  htmp;
    int32u* dtmp;
    byte_t* stmp;

    dst = safe_convert(dst, CR_ARGB8888, src, CR_INDEX8,
                       &dx, &dy, &wtmp, &htmp);
    if (dst == NULL)
        return (NULL);

    CR_MAKE_SRC(00, 00, 1);
    CR_MAKE_DST(dx, dy, 4);

    for (y = htmp; y != 0; y--) {
        for (x = 0; x < wtmp; x++) {
            dtmp[x] = src->pal[stmp[x]];
        }
        dtmp = (int32u*)((uchar*)dtmp + dbpl);
        stmp = (byte_t*)((uchar*)stmp + sbpl);
    }
    return ((sIMAGE*)dst);
}

/*
=======================================
    565to8888
=======================================
*/
CR_API sIMAGE*
img_565_to_32 (
  __CR_IO__ sIMAGE*         dst,
  __CR_IN__ uint_t          dx,
  __CR_IN__ uint_t          dy,
  __CR_IN__ const sIMAGE*   src
    )
{
    uint_t  x, y;
    dist_t  dbpl;
    dist_t  sbpl;
    uint_t  wtmp;
    uint_t  htmp;
    int32u* dtmp;
    int16u* stmp;

    dst = safe_convert(dst, CR_ARGB8888, src, CR_ARGB565,
                       &dx, &dy, &wtmp, &htmp);
    if (dst == NULL)
        return (NULL);

    CR_MAKE_SRC(00, 00, 2);
    CR_MAKE_DST(dx, dy, 4);

    for (y = htmp; y != 0; y--) {
        for (x = 0; x < wtmp; x++) {
            pixel565to32(&dtmp[x], stmp[x]);
        }
        dtmp = (int32u*)((uchar*)dtmp + dbpl);
        stmp = (int16u*)((uchar*)stmp + sbpl);
    }
    return ((sIMAGE*)dst);
}

/*
=======================================
    888to8888
=======================================
*/
CR_API sIMAGE*
img_888_to_32 (
  __CR_IO__ sIMAGE*         dst,
  __CR_IN__ uint_t          dx,
  __CR_IN__ uint_t          dy,
  __CR_IN__ const sIMAGE*   src
    )
{
    uint_t  x, y;
    dist_t  dbpl;
    dist_t  sbpl;
    uint_t  wtmp;
    uint_t  htmp;
    int32u* dtmp;
    byte_t* stmp;
    byte_t* dptr;
    byte_t* sptr;

    dst = safe_convert(dst, CR_ARGB8888, src, CR_ARGB888,
                       &dx, &dy, &wtmp, &htmp);
    if (dst == NULL)
        return (NULL);

    CR_MAKE_SRC(00, 00, 3);
    CR_MAKE_DST(dx, dy, 4);

    for (y = htmp; y != 0; y--) {
        dptr = (byte_t*)dtmp;
        sptr = (byte_t*)stmp;
        for (x = 0; x < wtmp; x++) {
            dptr[0] = sptr[0];
            dptr[1] = sptr[1];
            dptr[2] = sptr[2];
            dptr[3] = 0xFF;
            dptr += 4;
            sptr += 3;
        }
        dtmp = (int32u*)((uchar*)dtmp + dbpl);
        stmp = (byte_t*)((uchar*)stmp + sbpl);
    }
    return ((sIMAGE*)dst);
}

/*
=======================================
    X555to8888
=======================================
*/
CR_API sIMAGE*
img_X555_to_32 (
  __CR_IO__ sIMAGE*         dst,
  __CR_IN__ uint_t          dx,
  __CR_IN__ uint_t          dy,
  __CR_IN__ const sIMAGE*   src
    )
{
    uint_t  x, y;
    dist_t  dbpl;
    dist_t  sbpl;
    uint_t  wtmp;
    uint_t  htmp;
    int32u* dtmp;
    int16u* stmp;

    dst = safe_convert(dst, CR_ARGB8888, src, CR_ARGBX555,
                       &dx, &dy, &wtmp, &htmp);
    if (dst == NULL)
        return (NULL);

    CR_MAKE_SRC(00, 00, 2);
    CR_MAKE_DST(dx, dy, 4);

    for (y = htmp; y != 0; y--) {
        for (x = 0; x < wtmp; x++) {
            pixelX555to32(&dtmp[x], stmp[x]);
        }
        dtmp = (int32u*)((uchar*)dtmp + dbpl);
        stmp = (int16u*)((uchar*)stmp + sbpl);
    }
    return ((sIMAGE*)dst);
}

/*
=======================================
    1555to8888
=======================================
*/
CR_API sIMAGE*
img_1555_to_32 (
  __CR_IO__ sIMAGE*         dst,
  __CR_IN__ uint_t          dx,
  __CR_IN__ uint_t          dy,
  __CR_IN__ const sIMAGE*   src
    )
{
    uint_t  x, y;
    dist_t  dbpl;
    dist_t  sbpl;
    uint_t  wtmp;
    uint_t  htmp;
    int32u* dtmp;
    int16u* stmp;

    dst = safe_convert(dst, CR_ARGB8888, src, CR_ARGB1555,
                       &dx, &dy, &wtmp, &htmp);
    if (dst == NULL)
        return (NULL);

    CR_MAKE_SRC(00, 00, 2);
    CR_MAKE_DST(dx, dy, 4);

    for (y = htmp; y != 0; y--) {
        for (x = 0; x < wtmp; x++) {
            pixel1555to32(&dtmp[x], stmp[x]);
        }
        dtmp = (int32u*)((uchar*)dtmp + dbpl);
        stmp = (int16u*)((uchar*)stmp + sbpl);
    }
    return ((sIMAGE*)dst);
}

/*
=======================================
    4444to8888
=======================================
*/
CR_API sIMAGE*
img_4444_to_32 (
  __CR_IO__ sIMAGE*         dst,
  __CR_IN__ uint_t          dx,
  __CR_IN__ uint_t          dy,
  __CR_IN__ const sIMAGE*   src
    )
{
    uint_t  x, y;
    dist_t  dbpl;
    dist_t  sbpl;
    uint_t  wtmp;
    uint_t  htmp;
    int32u* dtmp;
    int16u* stmp;

    dst = safe_convert(dst, CR_ARGB8888, src, CR_ARGB4444,
                       &dx, &dy, &wtmp, &htmp);
    if (dst == NULL)
        return (NULL);

    CR_MAKE_SRC(00, 00, 2);
    CR_MAKE_DST(dx, dy, 4);

    for (y = htmp; y != 0; y--) {
        for (x = 0; x < wtmp; x++) {
            pixel4444to32(&dtmp[x], stmp[x]);
        }
        dtmp = (int32u*)((uchar*)dtmp + dbpl);
        stmp = (int16u*)((uchar*)stmp + sbpl);
    }
    return ((sIMAGE*)dst);
}

/*
=======================================
    DXTCto8888
=======================================
*/
typedef struct
{
        byte_t  r, g, b, a;

} color4;

CR_API sIMAGE*
img_dxtc_to_32 (
  __CR_IO__ sIMAGE*         dst,
  __CR_IN__ uint_t          dx,
  __CR_IN__ uint_t          dy,
  __CR_IN__ const sIMAGE*   src
    )
{
    uchar*  sptr;
    uchar*  dptr;
    uint_t  x, y;
    uint_t  wtmp;
    uint_t  htmp;
    uint_t  type;
    leng_t  dbpl;
    int64u  alpha;
    byte_t  index;
    uint_t  ii, jj;
    int16u  c0, c1;
    byte_t  alp[8];
    color4  clr[4];

    /* 安全检查 */
    if (src->fmt == CR_UNKNOWN || src->fmt > CR_DXT5 ||
        src->position.ww % 4 != 0 || src->position.hh % 4 != 0)
        return (NULL);

    dst = safe_convert(dst, CR_ARGB8888, src, src->fmt,
                       &dx, &dy, &wtmp, &htmp);
    if (dst == NULL)
        return (NULL);

    /* DXT24 to DXT135 */
    if (src->fmt % 2 == 0)
        type = src->fmt + 1;
    else
        type = src->fmt;

    sptr = src->data;
    alpha = CR_ULL(0);
    if (!dst->gdi)
        dbpl = dst->bpl;
    else
        dbpl = 0 - dst->bpl;

    for (y = 0; y < htmp; y += 4)
    for (x = 0; x < wtmp; x += 4)
    {
        if (type != 1)
        {
            /* DXT3/DXT5 */
            alpha = QWORD_LE(*(int64u*)sptr);
            sptr += 8;

            if (type == 5)
            {
                /* DXT5 */
                alp[0] = (byte_t)(alpha >> 0);
                alp[1] = (byte_t)(alpha >> 8);
                alpha >>= 16;

                if (alp[0] > alp[1])
                {
                    alp[2] = (byte_t)((6 * alp[0] + 1 * alp[1]) / 7);
                    alp[3] = (byte_t)((5 * alp[0] + 2 * alp[1]) / 7);
                    alp[4] = (byte_t)((4 * alp[0] + 3 * alp[1]) / 7);
                    alp[5] = (byte_t)((3 * alp[0] + 4 * alp[1]) / 7);
                    alp[6] = (byte_t)((2 * alp[0] + 5 * alp[1]) / 7);
                    alp[7] = (byte_t)((1 * alp[0] + 6 * alp[1]) / 7);
                }
                else
                {
                    alp[2] = (byte_t)((4 * alp[0] + 1 * alp[1] + 2) / 5);
                    alp[3] = (byte_t)((3 * alp[0] + 2 * alp[1] + 2) / 5);
                    alp[4] = (byte_t)((2 * alp[0] + 3 * alp[1] + 2) / 5);
                    alp[5] = (byte_t)((1 * alp[0] + 4 * alp[1] + 2) / 5);
                    alp[6] = 0x00;
                    alp[7] = 0xFF;
                }
            }
        }

        c0 = WORD_LE(((int16u*)sptr)[0]);
        c1 = WORD_LE(((int16u*)sptr)[1]);
        sptr += 4;

        clr[0].r = g_by32to256[(c0 >> 11) & 0x1F];
        clr[0].g = g_by64to256[(c0 >>  5) & 0x3F];
        clr[0].b = g_by32to256[(c0 >>  0) & 0x1F];
        clr[0].a = 0xFF;

        clr[1].r = g_by32to256[(c1 >> 11) & 0x1F];
        clr[1].g = g_by64to256[(c1 >>  5) & 0x3F];
        clr[1].b = g_by32to256[(c1 >>  0) & 0x1F];
        clr[1].a = 0xFF;

        if (type != 1 || c0 > c1)
        {
            clr[2].r = (byte_t)((clr[0].r * 2 + clr[1].r) / 3);
            clr[2].g = (byte_t)((clr[0].g * 2 + clr[1].g) / 3);
            clr[2].b = (byte_t)((clr[0].b * 2 + clr[1].b) / 3);
            clr[2].a = 0xFF;

            clr[3].r = (byte_t)((clr[0].r + clr[1].r * 2) / 3);
            clr[3].g = (byte_t)((clr[0].g + clr[1].g * 2) / 3);
            clr[3].b = (byte_t)((clr[0].b + clr[1].b * 2) / 3);
            clr[3].a = 0xFF;
        }
        else
        {
            clr[2].r = (byte_t)((clr[0].r + clr[1].r) / 2);
            clr[2].g = (byte_t)((clr[0].g + clr[1].g) / 2);
            clr[2].b = (byte_t)((clr[0].b + clr[1].b) / 2);
            clr[2].a = 0xFF;

            clr[3].r = 0x00;
            clr[3].g = 0x00;
            clr[3].b = 0x00;
            clr[3].a = 0x00;
        }

        if (!dst->gdi)
            dptr = (byte_t*)pixel_addr4(dst, dx+x, dy+y);
        else
            dptr = (byte_t*)pixel_addr4(dst, dx+x, dst->clip_win.y2-dy-y);

        if (type == 1)
        {
            /* DXT1 */
            for (ii = 0; ii < 4; ii++)
            {
                index = *sptr++;
                if (y + ii < htmp)
                {
                    for (jj = 0; jj < 4; jj++)
                    {
                        if (x + jj < wtmp) {
                            dptr[jj*4+0] = clr[index & 0x03].b;
                            dptr[jj*4+1] = clr[index & 0x03].g;
                            dptr[jj*4+2] = clr[index & 0x03].r;
                            dptr[jj*4+3] = clr[index & 0x03].a;
                            index >>= 2;
                        }
                    }
                }
                dptr += dbpl;
            }
        }
        else
        if (type == 3)
        {
            /* DXT3 */
            for (ii = 0; ii < 4; ii++)
            {
                index = *sptr++;
                if (y + ii < htmp)
                {
                    for (jj = 0; jj < 4; jj++)
                    {
                        if (x + jj < wtmp) {
                            dptr[jj*4+0] = clr[index & 0x03].b;
                            dptr[jj*4+1] = clr[index & 0x03].g;
                            dptr[jj*4+2] = clr[index & 0x03].r;
                            dptr[jj*4+3] = g_by16to256[(leng_t)alpha & 0x0F];
                            index >>= 2;
                        }
                        alpha >>= 4;
                    }
                }
                dptr += dbpl;
            }
        }
        else
        {
            /* DXT5 */
            for (ii = 0; ii < 4; ii++)
            {
                index = *sptr++;
                if (y + ii < htmp)
                {
                    for (jj = 0; jj < 4; jj++)
                    {
                        if (x + jj < wtmp) {
                            dptr[jj*4+0] = clr[index & 0x03].b;
                            dptr[jj*4+1] = clr[index & 0x03].g;
                            dptr[jj*4+2] = clr[index & 0x03].r;
                            dptr[jj*4+3] = alp[(leng_t)alpha & 0x07];
                            index >>= 2;
                        }
                        alpha >>= 3;
                    }
                }
                dptr += dbpl;
            }
        }
    }

    if (src->fmt % 2 == 0)
    {
        /* 处理预乘 DXT2/DXT4 */
        for (dptr = dst->data, dbpl = 0; dbpl < dst->size; dbpl += 4)
        {
            ii = dptr[dbpl + 3];
            if (ii == 0)
                continue;
            dptr[dbpl + 0] = (byte_t)(((uint_t)dptr[dbpl + 0] * 255) / ii);
            dptr[dbpl + 1] = (byte_t)(((uint_t)dptr[dbpl + 1] * 255) / ii);
            dptr[dbpl + 2] = (byte_t)(((uint_t)dptr[dbpl + 2] * 255) / ii);
        }
    }
    return ((sIMAGE*)dst);
}

/*
=======================================
    所有to8888
=======================================
*/
CR_API sIMAGE*
img_auto_to_32 (
  __CR_IO__ sIMAGE*         dst,
  __CR_IN__ uint_t          dx,
  __CR_IN__ uint_t          dy,
  __CR_IN__ const sIMAGE*   src
    )
{
    leng_t  line;
    uint_t  wtmp;
    uint_t  htmp;
    dist_t  dbpl;
    dist_t  sbpl;
    int32u* dtmp;
    int32u* stmp;

    switch (src->fmt)
    {
        default:
            break;

        case CR_DXT1:
        case CR_DXT2:
        case CR_DXT3:
        case CR_DXT4:
        case CR_DXT5:
            return (img_dxtc_to_32(dst, dx, dy, src));

        case CR_INDEX1:
            return (img_idx1_to_32(dst, dx, dy, src));

        case CR_INDEX4:
            return (img_idx4_to_32(dst, dx, dy, src));

        case CR_INDEX8:
            return (img_idx8_to_32(dst, dx, dy, src));

        case CR_ARGB565:
            return (img_565_to_32(dst, dx, dy, src));

        case CR_ARGB888:
            return (img_888_to_32(dst, dx, dy, src));

        case CR_ARGB4444:
            return (img_4444_to_32(dst, dx, dy, src));

        case CR_ARGBX555:
            return (img_X555_to_32(dst, dx, dy, src));

        case CR_ARGB1555:
            return (img_1555_to_32(dst, dx, dy, src));

        case CR_ARGB8888:
            dst = safe_convert(dst, CR_ARGB8888, src, CR_ARGB8888,
                               &dx, &dy, &wtmp, &htmp);
            if (dst == NULL)
                return (NULL);

            CR_MAKE_SRC(00, 00, 4);
            CR_MAKE_DST(dx, dy, 4);

            line = wtmp;
            line *= 4;
            for (; htmp != 0; htmp--) {
                mem_cpy(dtmp, stmp, line);
                dtmp = (int32u*)((uchar*)dtmp + dbpl);
                stmp = (int32u*)((uchar*)stmp + sbpl);
            }
            return ((sIMAGE*)dst);
    }

    return (NULL);
}

/*
=======================================
    自由格式转换 (用32位色中转)
=======================================
*/
CR_API sIMAGE*
img_auto_to_xx (
  __CR_IO__ sIMAGE*         dst,
  __CR_IN__ const sIMAGE*   src
    )
{
    uint_t          xxxx;
    uint_t          wtmp;
    uint_t          htmp;
    dist_t          dbpl;
    dist_t          sbpl;
    leng_t          line;
    byte_t*         dtmp;
    byte_t*         stmp;
    const sIMAGE*   temp;

    /* 两者格式一致, 直接复制 */
    if (src->fmt == dst->fmt)
    {
        xxxx = 0;
        dst = safe_convert(dst, dst->fmt, src, src->fmt,
                           &xxxx, &xxxx, &wtmp, &htmp);
        if (dst == NULL)
            return (NULL);

        CR_MAKE_SRC(0, 0, N);
        CR_MAKE_DST(0, 0, N);

        /* 计算复制的长度 */
        if (src->fmt == CR_INDEX1) {
            if (wtmp % 8 == 0)
                line = wtmp / 8;
            else
                line = wtmp / 8 + 1;
        }
        else
        if (src->fmt == CR_INDEX4) {
            if (wtmp % 2 == 0)
                line = wtmp / 2;
            else
                line = wtmp / 2 + 1;
        }
        else {
            line = src->bpc;
            line *= wtmp;
        }

        for (; htmp != 0; htmp--) {
            mem_cpy(dtmp, stmp, line);
            dtmp = dtmp + dbpl;
            stmp = stmp + sbpl;
        }
        return ((sIMAGE*)dst);
    }

    /* 目标为32位色, 直接转过去 */
    if (dst->fmt == CR_ARGB8888)
        return (img_auto_to_32(dst, 0, 0, src));

    /* 统一转换到 ARGB8888 */
    if (src->fmt == CR_ARGB8888) {
        temp = (const sIMAGE*)src;
    }
    else {
        /* 转到临时32位位图 */
        temp = img_auto_to_32(NULL, 0, 0, src);
        if (temp == NULL)
            return (NULL);
    }

    /* 转换到指定的格式 */
    switch (dst->fmt)
    {
        case CR_ARGB565:
            if (img_32_to_565(dst, 0, 0, temp) == NULL)
                goto _failure;
            break;

        case CR_ARGB888:
            if (img_32_to_888(dst, 0, 0, temp) == NULL)
                goto _failure;
            break;

        case CR_ARGBX555:
            if (img_32_to_X555(dst, 0, 0, temp) == NULL)
                goto _failure;
            break;

        case CR_ARGB1555:
            if (img_32_to_1555(dst, 0, 0, temp) == NULL)
                goto _failure;
            break;

        case CR_ARGB4444:
            if (img_32_to_4444(dst, 0, 0, temp) == NULL)
                goto _failure;
            break;

        default:
            goto _failure;
    }

    /* 释放临时位图 */
    if (src != temp)
        image_del(temp);
    return (dst);

_failure:
    if (src != temp)
        image_del(temp);
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
