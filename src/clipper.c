/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2009-12-23  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 二维剪裁函数库 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "gfx2.h"

/*
=======================================
    检查点是否在方框内
=======================================
*/
CR_API bool_t
clip_pixel (
  __CR_IN__ sint_t          x,
  __CR_IN__ sint_t          y,
  __CR_IN__ const sRECT*    win
    )
{
    return ((x < win->x1 || y < win->y1 ||
             x > win->x2 || y > win->y2) ? FALSE : TRUE);
}

/*
---------------------------------------
    线段剪裁内部函数
---------------------------------------
*/
static bool_t
clip_line_test (
  __CR_IN__ const uint_t*   size,
  __CR_IO__ sint_t          pt[4]
    )
{
    sint_t  aa;
    sint_t  x1, y1, x2, y2;
    sint_t  c1, c2, rr, bb;

    x1 = pt[0];
    y1 = pt[1];
    x2 = pt[2];
    y2 = pt[3];
    rr = size[0] - 1;
    bb = size[1] - 1;
    c1 = (x1 < 0) + (x1 > rr) * 2 + (y1 < 0) * 4 + (y1 > bb) * 8;
    c2 = (x2 < 0) + (x2 > rr) * 2 + (y2 < 0) * 4 + (y2 > bb) * 8;
    if (((c1 & c2) == 0) && ((c1 | c2) != 0))
    {
        if (c1 & 12) {
            aa  = (c1 < 8) ? 0 : bb;
            x1 += (sint_t)(((int32s)(aa - y1)) * (x2 - x1) / (y2 - y1));
            y1  = aa;
            c1  = (x1 < 0) + (x1 > rr) * 2;
        }
        if (c2 & 12) {
            aa  = (c2 < 8) ? 0 : bb;
            x2 += (sint_t)(((int32s)(aa - y2)) * (x2 - x1) / (y2 - y1));
            y2  = aa;
            c2  = (x2 < 0) + (x2 > rr) * 2;
        }
        if (((c1 & c2) == 0) && ((c1 | c2) != 0))
        {
            if (c1 != 0) {
                aa  = (c1 == 1) ? 0 : rr;
                y1 += (sint_t)(((int32s)(aa - x1)) * (y2 - y1) / (x2 - x1));
                x1  = aa;
                c1  = 0;
            }
            if (c2 != 0) {
                aa  = (c2 == 1) ? 0 : rr;
                y2 += (sint_t)(((int32s)(aa - x2)) * (y2 - y1) / (x2 - x1));
                x2  = aa;
                c2  = 0;
            }
        }
        pt[0] = x1;
        pt[1] = y1;
        pt[2] = x2;
        pt[3] = y2;
    }
    return (((c1 | c2) == 0) ? TRUE : FALSE);
}

/*
=======================================
    线段剪裁 (使用前需保证宽高为正)
=======================================
*/
CR_API bool_t
clip_line (
  __CR_OT__ sLINE*          dst,
  __CR_IN__ const sLINE*    src,
  __CR_IN__ const sRECT*    win
    )
{
    sint_t  pt[4];

    pt[0] = src->x1 - win->x1;
    pt[1] = src->y1 - win->y1;
    pt[2] = src->x2 - win->x1;
    pt[3] = src->y2 - win->y1;
    if (!clip_line_test(&win->ww, pt))
        return (FALSE);
    pt[0] += win->x1;
    pt[1] += win->y1;
    pt[2] += win->x1;
    pt[3] += win->y1;
    line_set_xy(dst, pt[0], pt[1], pt[2], pt[3]);
    return (TRUE);
}

/*
=======================================
    填充剪裁 (使用前需保证宽高为正)
=======================================
*/
CR_API bool_t
clip_fill (
  __CR_OT__ sFILL*          dst,
  __CR_IN__ const sFILL*    src,
  __CR_IN__ const sRECT*    win
    )
{
    sint_t  dx1, dx2;
    sint_t  dy1, dy2;

    /* X 轴 */
    dx1 = src->dx;
    dx2 = src->dx + src->dw - 1;
    if (win->x1 > dx1) {
        if (win->x1 > dx2)
            return (FALSE);

        if (win->x2 < dx2)
            dx2 = win->x2;
        dx1 = win->x1;
    }
    else {
        if (win->x2 < dx1)
            return (FALSE);

        if (win->x2 < dx2)
            dx2 = win->x2;
    }

    /* Y 轴 */
    dy1 = src->dy;
    dy2 = src->dy + src->dh - 1;
    if (win->y1 > dy1) {
        if (win->y1 > dy2)
            return (FALSE);

        if (win->y2 < dy2)
            dy2 = win->y2;
        dy1 = win->y1;
    }
    else {
        if (win->y2 < dy1)
            return (FALSE);

        if (win->y2 < dy2)
            dy2 = win->y2;
    }
    dst->dx = dx1;
    dst->dy = dy1;
    dst->dw = dx2 - dx1 + 1;
    dst->dh = dy2 - dy1 + 1;
    return (TRUE);
}

/*
=======================================
    BLT 剪裁 (使用前需保证宽高为正)
=======================================
*/
CR_API bool_t
clip_blit (
  __CR_OT__ sBLIT*          dst,
  __CR_IN__ const sBLIT*    src,
  __CR_IN__ const sRECT*    win
    )
{
    sint_t  dx1, dx2;
    sint_t  dy1, dy2;

    /* X 轴 */
    dx1 = src->dx;
    dx2 = src->dx + src->sw - 1;
    if (win->x1 > dx1) {
        if (win->x1 > dx2)
            return (FALSE);

        if (win->x2 < dx2)
            dx2 = win->x2;
        dx1 = win->x1;
    }
    else {
        if (win->x2 < dx1)
            return (FALSE);

        if (win->x2 < dx2)
            dx2 = win->x2;
    }

    /* Y 轴 */
    dy1 = src->dy;
    dy2 = src->dy + src->sh - 1;
    if (win->y1 > dy1) {
        if (win->y1 > dy2)
            return (FALSE);

        if (win->y2 < dy2)
            dy2 = win->y2;
        dy1 = win->y1;
    }
    else {
        if (win->y2 < dy1)
            return (FALSE);

        if (win->y2 < dy2)
            dy2 = win->y2;
    }
    dst->dx = dx1;
    dst->dy = dy1;
    dst->sx = src->sx + dx1 - src->dx;
    dst->sy = src->sy + dy1 - src->dy;
    dst->sw = dx2 - dx1 + 1;
    dst->sh = dy2 - dy1 + 1;
    return (TRUE);
}

/*
=======================================
    矩形剪裁 (使用前需保证宽高为正)
=======================================
*/
CR_API bool_t
clip_rect (
  __CR_OT__ sRECT*          dst,
  __CR_IN__ const sRECT*    src,
  __CR_IN__ const sRECT*    win
    )
{
    sint_t  x1, y1;
    sint_t  x2, y2;

    x1 = CR_MAX(src->x1, win->x1);
    x2 = CR_MIN(src->x2, win->x2);
    if (x1 > x2)
        return (FALSE);

    y1 = CR_MAX(src->y1, win->y1);
    y2 = CR_MIN(src->y2, win->y2);
    if (y1 > y2)
        return (FALSE);

    rect_set_xy(dst, x1, y1, x2, y2);
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
