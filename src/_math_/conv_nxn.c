/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2013-04-18  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack 图像卷积运算函数库 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "phylib.h"

/*
---------------------------------------
    8位色图卷积运算
---------------------------------------
*/
static void_t
img_convo_08 (
  __CR_OT__ const sIMAGE*       dst,
  __CR_IN__ const sIMAGE*       src,
  __CR_IN__ const sCONVO_MAT*   mat
    )
{
    int32s  core;
    uint_t  width, height;
    uint_t  side_x, side_y;
    uint_t  xx, yy, cx, cy;
    /* ----------------- */
    byte_t*         dx_ptr;
    byte_t*         dy_ptr;
    const byte_t*   sx_ptr;
    const byte_t*   sy_ptr;
    const sint_t*   mat_ptr;

    side_x = (mat->ww - 1) / 2;
    side_y = (mat->hh - 1) / 2;
    width  = src->position.ww - mat->ww + 1;
    height = src->position.hh - mat->hh + 1;

    sy_ptr = src->data;
    dy_ptr = dst->data + side_y * dst->bpl + side_x;

    for (yy = 0; yy < height; yy++)
    {
        for (xx = 0; xx < width; xx++)
        {
            core = 0;
            mat_ptr = mat->dt;
            sx_ptr = sy_ptr + xx;
            dx_ptr = dy_ptr + xx;

            for (cy = 0; cy < mat->hh; cy++) {
                for (cx = 0; cx < mat->ww; cx++) {
                    core += sx_ptr[cx] * mat_ptr[0];
                    mat_ptr++;
                }
                sx_ptr += src->bpl;
            }
            core /= mat->kk;
            if (core <= 0)
                dx_ptr[0] = 0x00;
            else
            if (core >= 255)
                dx_ptr[0] = 0xFF;
            else
                dx_ptr[0] = (byte_t)core;
        }
        sy_ptr += src->bpl;
        dy_ptr += dst->bpl;
    }
}

/*
---------------------------------------
    24位色图卷积运算
---------------------------------------
*/
static void_t
img_convo_24 (
  __CR_OT__ const sIMAGE*       dst,
  __CR_IN__ const sIMAGE*       src,
  __CR_IN__ const sCONVO_MAT*   mat
    )
{
    int32s  rr, gg, bb;
    uint_t  width, height;
    uint_t  side_x, side_y;
    uint_t  xx, yy, cx, cy;
    /* ----------------- */
    byte_t*         dx_ptr;
    byte_t*         dy_ptr;
    const byte_t*   sx_ptr;
    const byte_t*   sy_ptr;
    const sint_t*   mat_ptr;

    side_x = (mat->ww - 1) / 2;
    side_y = (mat->hh - 1) / 2;
    width  = src->position.ww - mat->ww + 1;
    height = src->position.hh - mat->hh + 1;

    sy_ptr = src->data;
    dy_ptr = dst->data + side_y * dst->bpl + side_x * 3;

    for (yy = 0; yy < height; yy++)
    {
        for (xx = 0; xx < width; xx++)
        {
            rr = gg = bb = 0;
            mat_ptr = mat->dt;
            sx_ptr = sy_ptr + xx * 3;
            dx_ptr = dy_ptr + xx * 3;

            for (cy = 0; cy < mat->hh; cy++) {
                for (cx = 0; cx < mat->ww; cx++) {
                    bb += sx_ptr[cx * 3 + 0] * mat_ptr[0];
                    gg += sx_ptr[cx * 3 + 1] * mat_ptr[0];
                    rr += sx_ptr[cx * 3 + 2] * mat_ptr[0];
                    mat_ptr++;
                }
                sx_ptr += src->bpl;
            }
            bb /= mat->kk;
            if (bb <= 0)
                dx_ptr[0] = 0x00;
            else
            if (bb >= 255)
                dx_ptr[0] = 0xFF;
            else
                dx_ptr[0] = (byte_t)bb;

            gg /= mat->kk;
            if (gg <= 0)
                dx_ptr[1] = 0x00;
            else
            if (gg >= 255)
                dx_ptr[1] = 0xFF;
            else
                dx_ptr[1] = (byte_t)gg;

            rr /= mat->kk;
            if (rr <= 0)
                dx_ptr[2] = 0x00;
            else
            if (rr >= 255)
                dx_ptr[2] = 0xFF;
            else
                dx_ptr[2] = (byte_t)rr;
        }
        sy_ptr += src->bpl;
        dy_ptr += dst->bpl;
    }
}

/*
---------------------------------------
    32位色图卷积运算
---------------------------------------
*/
static void_t
img_convo_32 (
  __CR_OT__ const sIMAGE*       dst,
  __CR_IN__ const sIMAGE*       src,
  __CR_IN__ const sCONVO_MAT*   mat
    )
{
    int32s  rr, gg, bb;
    uint_t  width, height;
    uint_t  side_x, side_y;
    uint_t  xx, yy, cx, cy;
    /* ----------------- */
    byte_t*         dx_ptr;
    byte_t*         dy_ptr;
    const byte_t*   sx_ptr;
    const byte_t*   sy_ptr;
    const sint_t*   mat_ptr;

    side_x = (mat->ww - 1) / 2;
    side_y = (mat->hh - 1) / 2;
    width  = src->position.ww - mat->ww + 1;
    height = src->position.hh - mat->hh + 1;

    sy_ptr = src->data;
    dy_ptr = dst->data + side_y * dst->bpl + side_x * 4;

    for (yy = 0; yy < height; yy++)
    {
        for (xx = 0; xx < width; xx++)
        {
            rr = gg = bb = 0;
            mat_ptr = mat->dt;
            sx_ptr = sy_ptr + xx * 4;
            dx_ptr = dy_ptr + xx * 4;

            for (cy = 0; cy < mat->hh; cy++) {
                for (cx = 0; cx < mat->ww; cx++) {
                    bb += sx_ptr[cx * 4 + 0] * mat_ptr[0];
                    gg += sx_ptr[cx * 4 + 1] * mat_ptr[0];
                    rr += sx_ptr[cx * 4 + 2] * mat_ptr[0];
                    mat_ptr++;
                }
                sx_ptr += src->bpl;
            }
            bb /= mat->kk;
            if (bb <= 0)
                dx_ptr[0] = 0x00;
            else
            if (bb >= 255)
                dx_ptr[0] = 0xFF;
            else
                dx_ptr[0] = (byte_t)bb;

            gg /= mat->kk;
            if (gg <= 0)
                dx_ptr[1] = 0x00;
            else
            if (gg >= 255)
                dx_ptr[1] = 0xFF;
            else
                dx_ptr[1] = (byte_t)gg;

            rr /= mat->kk;
            if (rr <= 0)
                dx_ptr[2] = 0x00;
            else
            if (rr >= 255)
                dx_ptr[2] = 0xFF;
            else
                dx_ptr[2] = (byte_t)rr;
            dx_ptr[3] = 0xFF;
        }
        sy_ptr += src->bpl;
        dy_ptr += dst->bpl;
    }
}

/*
=======================================
    图像卷积运算
=======================================
*/
CR_API sIMAGE*
image_convo (
  __CR_IN__ const sIMAGE*       img,
  __CR_IN__ const sCONVO_MAT*   mat
    )
{
    sIMAGE* dst;

    /* 矩阵必须有效 */
    if (mat->kk == 0 ||
        mat->ww < 1 || mat->hh < 1 ||
        mat->ww > img->position.ww ||
        mat->hh > img->position.hh ||
        mat->ww % 2 != 1 || mat->hh % 2 != 1)
        return (NULL);

    /* 只支持这几种格式 */
    if (img->fmt != CR_INDEX8 &&
        img->fmt != CR_ARGB888 &&
        img->fmt != CR_ARGB8888)
        return (NULL);

    /* 克隆出目标图后卷积 */
    dst = image_dup(img);
    if (dst == NULL)
        return (NULL);
    if (img->fmt == CR_ARGB8888)
        img_convo_32(dst, img, mat);
    else
    if (img->fmt == CR_INDEX8)
        img_convo_08(dst, img, mat);
    else
        img_convo_24(dst, img, mat);
    return (dst);
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
