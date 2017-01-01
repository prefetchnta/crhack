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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack 图像形态运算函数库 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "phylib.h"

/*
---------------------------------------
    8位色图形态运算
---------------------------------------
*/
static void_t
img_shape_08 (
  __CR_OT__ const sIMAGE*       dst,
  __CR_IN__ const sIMAGE*       src,
  __CR_IN__ const sSHAPE_MAT*   mat,
  __CR_IN__ bool_t              expand
    )
{
    uint_t  width, height;
    uint_t  side_x, side_y;
    uint_t  xx, yy, cx, cy;
    /* ----------------- */
    byte_t*         dx_ptr;
    byte_t*         dy_ptr;
    const byte_t*   sx_ptr;
    const byte_t*   sy_ptr;
    const byte_t*   mat_ptr;

    side_x = (mat->ww - 1) / 2;
    side_y = (mat->hh - 1) / 2;
    width  = src->position.ww - mat->ww + 1;
    height = src->position.hh - mat->hh + 1;

    mat_ptr = mat->dt;
    sy_ptr = src->data;
    dy_ptr = dst->data + side_y * dst->bpl + side_x;
    if (expand)
    {
        /* 膨胀运算 */
        for (yy = 0; yy < height; yy++)
        {
            for (xx = 0; xx < width; xx++)
            {
                sx_ptr = sy_ptr + xx;
                dx_ptr = dy_ptr + xx;

                for (cy = 0; cy < mat->hh; cy++) {
                    for (cx = 0; cx < mat->ww; cx++) {
                        if (mat_ptr[0] == sx_ptr[cx]) {
                            dx_ptr[0] = 0xFF;
                            goto _expand_exit;
                        }
                        mat_ptr++;
                    }
                    sx_ptr += src->bpl;
                }
_expand_exit:
                mat_ptr = mat->dt;
            }
            sy_ptr += src->bpl;
            dy_ptr += dst->bpl;
        }
    }
    else
    {
        /* 腐蚀运算 */
        for (yy = 0; yy < height; yy++)
        {
            for (xx = 0; xx < width; xx++)
            {
                sx_ptr = sy_ptr + xx;
                dx_ptr = dy_ptr + xx;

                for (cy = 0; cy < mat->hh; cy++) {
                    for (cx = 0; cx < mat->ww; cx++) {
                        if (mat_ptr[0] != 0x80 &&
                            mat_ptr[0] != sx_ptr[cx]) {
                            dx_ptr[0] = 0x00;
                            goto _erosion_exit;
                        }
                        mat_ptr++;
                    }
                    sx_ptr += src->bpl;
                }
_erosion_exit:
                mat_ptr = mat->dt;
            }
            sy_ptr += src->bpl;
            dy_ptr += dst->bpl;
        }
    }
}

/*
=======================================
    图像形态运算
=======================================
*/
CR_API sIMAGE*
image_shape (
  __CR_IN__ const sIMAGE*       img,
  __CR_IN__ const sSHAPE_MAT*   mat,
  __CR_IN__ bool_t              expand
    )
{
    sIMAGE* dst;

    /* 矩阵必须有效 */
    if (mat->ww < 1 || mat->hh < 1 ||
        mat->ww > img->position.ww ||
        mat->hh > img->position.hh ||
        mat->ww % 2 != 1 || mat->hh % 2 != 1)
        return (NULL);

    /* 只支持二值图片 */
    if (img->fmt != CR_INDEX8)
        return (NULL);

    /* 克隆出目标图后卷积 */
    dst = image_dup(img);
    if (dst == NULL)
        return (NULL);
    img_shape_08(dst, img, mat, expand);
    return (dst);
}

/*
=======================================
    自定义形态运算
=======================================
*/
CR_API sIMAGE*
image_shape_ex (
  __CR_IN__ const sIMAGE*   img,
  __CR_IN__ shape_core_t    score,
  __CR_IN__ uint_t          mat_w,
  __CR_IN__ uint_t          mat_h,
  __CR_IN__ void_t*         param
    )
{
    sIMAGE* dst;
    byte_t* dline;
    byte_t* sline;
    uint_t  xx, yy;
    uint_t  ww, hh;
    leng_t  offset;

    /* 参数检查 */
    if (mat_w < 1 || mat_w > img->position.ww || mat_w % 2 != 1 ||
        mat_h < 1 || mat_h > img->position.hh || mat_h % 2 != 1)
        return (NULL);

    /* 只支持二值图片 */
    if (img->fmt != CR_INDEX8)
        return (NULL);

    /* 克隆出目标图后卷积 */
    dst = image_dup(img);
    if (dst == NULL)
        return (NULL);

    /* 开始卷积 */
    dline = dst->data;
    sline = img->data;
    ww = img->position.ww - mat_w;
    hh = img->position.hh - mat_h;
    mat_w = (mat_w - 1) / 2;
    mat_h = (mat_h - 1) / 2;
    offset = mat_h * img->bpl + mat_w;
    for (yy = 0; yy < hh; yy++)
    {
        for (xx = 0; xx < ww; xx++)
        {
            /* 根据回调函数决定像素去留 */
            switch (score(param, sline + xx, img->bpl))
            {
                default:
                case SHAPE_OP_NOP:
                    break;

                case SHAPE_OP_DEL:
                    dline[xx + offset] = 0x00;
                    break;

                case SHAPE_OP_SET:
                    dline[xx + offset] = 0xFF;
                    break;
            }
        }
        dline += dst->bpl;
        sline += img->bpl;
    }
    return (dst);
}

/*
=======================================
    图像形态开运算
=======================================
*/
CR_API sIMAGE*
shape_open (
  __CR_IN__ const sIMAGE*       img,
  __CR_IN__ const sSHAPE_MAT*   rotz,
  __CR_IN__ uint_t              rot_times,
  __CR_IN__ const sSHAPE_MAT*   expn,
  __CR_IN__ uint_t              exp_times
    )
{
    sIMAGE* bin1;
    sIMAGE* bin2;

    /* 克隆一份 */
    bin2 = image_dup(img);
    if (bin2 == NULL)
        return (NULL);

    /* 先腐蚀 */
    for (; rot_times != 0; rot_times--) {
        bin1 = image_shape(bin2, rotz, FALSE);
        image_del(bin2);
        if (bin1 == NULL)
            return (NULL);
        bin2 = bin1;
    }

    /* 再膨胀 */
    for (; exp_times != 0; exp_times--) {
        bin1 = image_shape(bin2, expn, TRUE);
        image_del(bin2);
        if (bin1 == NULL)
            return (NULL);
        bin2 = bin1;
    }
    return (bin2);
}

/*
=======================================
    图像形态闭运算
=======================================
*/
CR_API sIMAGE*
shape_close (
  __CR_IN__ const sIMAGE*       img,
  __CR_IN__ const sSHAPE_MAT*   expn,
  __CR_IN__ uint_t              exp_times,
  __CR_IN__ const sSHAPE_MAT*   rotz,
  __CR_IN__ uint_t              rot_times
    )
{
    sIMAGE* bin1;
    sIMAGE* bin2;

    /* 克隆一份 */
    bin2 = image_dup(img);
    if (bin2 == NULL)
        return (NULL);

    /* 先膨胀 */
    for (; exp_times != 0; exp_times--) {
        bin1 = image_shape(bin2, expn, TRUE);
        image_del(bin2);
        if (bin1 == NULL)
            return (NULL);
        bin2 = bin1;
    }

    /* 再腐蚀 */
    for (; rot_times != 0; rot_times--) {
        bin1 = image_shape(bin2, rotz, FALSE);
        image_del(bin2);
        if (bin1 == NULL)
            return (NULL);
        bin2 = bin1;
    }
    return (bin2);
}

/********************/
/* 杂点清除矩阵数据 */
/********************/
static const sint_t _rom_ s_dt_wxy[3] =
{
             1, 1, 1,
};
static const sint_t _rom_ s_dt_wet[9] =
{
    1, 2, 1, 2, 4, 2, 1, 2, 1,
};
static const byte_t _rom_ s_dt_box[9] =
{
    255, 255, 255, 255, 255, 255, 255, 255, 255,
};
static const sSHAPE_MAT _rom_ s_shp_a_x =
{
    3, 1, s_dt_box
};
static const sSHAPE_MAT _rom_ s_shp_a_y =
{
    1, 3, s_dt_box
};
static const sSHAPE_MAT _rom_ s_shp_box =
{
    3, 3, s_dt_box
};
static const sCONVO_MAT _rom_ s_mat_wxy =
{
    2, 3, 3, s_dt_wxy
};
static const sCONVO_MAT _rom_ s_mat_wet =
{
    8, 3, 3, s_dt_wet
};

/* 自定义核上下文 */
typedef struct
{
        const sSHAPE_MAT*   cshp;
        const sCONVO_MAT*   wght;

} sSHAPE_CTX;

/*
---------------------------------------
    杂点清除形态核
---------------------------------------
*/
static uint_t
do_score (
  __CR_IN__ void_t*         param,
  __CR_IN__ const byte_t*   left_top,
  __CR_IN__ leng_t          img_bpl
    )
{
    sSHAPE_CTX* ctx;

    ctx = (sSHAPE_CTX*)param;
    if (shape_match_wet(left_top, img_bpl, ctx->cshp, ctx->wght))
        return (SHAPE_OP_SET);
    return (SHAPE_OP_DEL);
}

/*
=======================================
    图像杂点清除
=======================================
*/
CR_API sIMAGE*
shape_clean (
  __CR_IN__ const sIMAGE*   img,
  __CR_IN__ uint_t          type,
  __CR_IN__ uint_t          times
    )
{
    uint_t      matw;
    uint_t      math;
    sIMAGE*     bin1;
    sIMAGE*     bin2;
    sSHAPE_CTX  parm;

    /* 过滤类型 */
    if (type > SHAPE_CL_XY)
        return (NULL);

    /* 克隆一份 */
    bin2 = image_dup(img);
    if (bin2 == NULL)
        return (NULL);

    /* 清除边框 */
    if (!image_bound(bin2, 3, 3)) {
        image_del(bin2);
        return (NULL);
    }

    /* 设置形态核 */
    if (type == SHAPE_CL_XX) {
        matw = 3;
        math = 1;
        parm.cshp = &s_shp_a_x;
        parm.wght = &s_mat_wxy;
    }
    else
    if (type == SHAPE_CL_YY) {
        matw = 1;
        math = 3;
        parm.cshp = &s_shp_a_y;
        parm.wght = &s_mat_wxy;
    }
    else {
        matw = 3;
        math = 3;
        parm.cshp = &s_shp_box;
        parm.wght = &s_mat_wet;
    }

    /* 自定义形态运算 */
    for (; times != 0; times--) {
        bin1 = image_shape_ex(bin2, do_score, matw, math, &parm);
        image_del(bin2);
        if (bin1 == NULL)
            return (NULL);
        bin2 = bin1;
    }
    return (bin2);
}

/*
=======================================
    形态查找匹配 (AND)
=======================================
*/
CR_API bool_t
shape_match_and (
  __CR_IN__ const byte_t*       left_top,
  __CR_IN__ leng_t              img_bpl,
  __CR_IN__ const sSHAPE_MAT*   shape_mat
    )
{
    uint_t          xx, yy;
    const byte_t*   mat_ptr = shape_mat->dt;

    for (yy = 0; yy < shape_mat->hh; yy++) {
        for (xx = 0; xx < shape_mat->ww; xx++) {
            if (mat_ptr[0] != 0x80 &&
                mat_ptr[0] != left_top[xx])
                return (FALSE);
            mat_ptr += 1;
        }
        left_top += img_bpl;
    }
    return (TRUE);
}

/*
=======================================
    形态查找匹配 (ORR)
=======================================
*/
CR_API bool_t
shape_match_orr (
  __CR_IN__ const byte_t*       left_top,
  __CR_IN__ leng_t              img_bpl,
  __CR_IN__ const sSHAPE_MAT*   shape_mat
    )
{
    uint_t          xx, yy;
    const byte_t*   mat_ptr = shape_mat->dt;

    for (yy = 0; yy < shape_mat->hh; yy++) {
        for (xx = 0; xx < shape_mat->ww; xx++) {
            if (mat_ptr[0] == left_top[xx])
                return (TRUE);
            mat_ptr += 1;
        }
        left_top += img_bpl;
    }
    return (FALSE);
}

/*
=======================================
    形态查找匹配 (CNT)
=======================================
*/
CR_API bool_t
shape_match_cnt (
  __CR_IN__ const byte_t*       left_top,
  __CR_IN__ leng_t              img_bpl,
  __CR_IN__ const sSHAPE_MAT*   shape_mat,
  __CR_IN__ uint_t              gate
    )
{
    uint_t          xx, yy, cnt = 0;
    const byte_t*   mat_ptr = shape_mat->dt;

    for (yy = 0; yy < shape_mat->hh; yy++) {
        for (xx = 0; xx < shape_mat->ww; xx++) {
            if (mat_ptr[0] == left_top[xx]) {
                if (++cnt >= gate)
                    return (TRUE);
            }
            mat_ptr += 1;
        }
        left_top += img_bpl;
    }
    return (FALSE);
}

/*
=======================================
    形态查找匹配 (WET)
=======================================
*/
CR_API bool_t
shape_match_wet (
  __CR_IN__ const byte_t*       left_top,
  __CR_IN__ leng_t              img_bpl,
  __CR_IN__ const sSHAPE_MAT*   shape_mat,
  __CR_IN__ const sCONVO_MAT*   weight
    )
{
    uint_t          xx, yy;
    sint_t          cnt = 0;
    const sint_t*   wet_ptr = weight->dt;
    const byte_t*   mat_ptr = shape_mat->dt;

    for (yy = 0; yy < shape_mat->hh; yy++) {
        for (xx = 0; xx < shape_mat->ww; xx++) {
            if (mat_ptr[0] == left_top[xx]) {
                cnt += wet_ptr[0];
                if (cnt >= weight->kk)
                    return (TRUE);
            }
            wet_ptr += 1;
            mat_ptr += 1;
        }
        left_top += img_bpl;
    }
    return (FALSE);
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
