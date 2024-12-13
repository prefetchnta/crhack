/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2012-07-12  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>> EGUI 界面绘制布局函数库 <<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "util/egui.h"

/*
=======================================
    区域的对齐计算
=======================================
*/
CR_API void_t
egui_rect_align (
  __CR_IO__ sRECT*          dest,
  __CR_IN__ const sRECT*    rect,
  __CR_IN__ uint_t          type
    )
{
    uint_t  flag;

    /* 水平对齐 */
    flag = type & EGUI_ALN_MASKH;
    if (flag == EGUI_ALN_L)
        rect_align_x(dest, rect, RCT_ALIGN_LSB);
    else
    if (flag == EGUI_ALN_R)
        rect_align_x(dest, rect, RCT_ALIGN_MSB);
    else
    if (flag == EGUI_ALN_C)
        rect_align_x(dest, rect, RCT_ALIGN_HSB);

    /* 垂直对齐 */
    flag = type & EGUI_ALN_MASKV;
    if (flag == EGUI_ALN_T)
        rect_align_y(dest, rect, RCT_ALIGN_LSB);
    else
    if (flag == EGUI_ALN_B)
        rect_align_y(dest, rect, RCT_ALIGN_MSB);
    else
    if (flag == EGUI_ALN_M)
        rect_align_y(dest, rect, RCT_ALIGN_HSB);
}

/*
=======================================
    文字抽象输出 (实体)
=======================================
*/
CR_API bool_t
egui_draw_text (
  __CR_IN__ iFONT*          font,
  __CR_IN__ const void_t*   text,
  __CR_IN__ const sRECT*    rect,
  __CR_IN__ uint_t          type,
  __CR_IN__ uint_t          cpage
    )
{
    sRECT   dest;

    /* 计算目标区域 */
    dest.x1 = rect->x1;
    dest.y1 = rect->y1;
    if (!CR_VCALL(font)->calc_rect(font, text, &dest, cpage))
        return (FALSE);

    /* 对齐后区域输出 */
    egui_rect_align(&dest, rect, type);
    return (CR_VCALL(font)->draw_text(font, text, &dest, cpage));
}

/*
=======================================
    文字抽象输出 (透明)
=======================================
*/
CR_API bool_t
egui_draw_tran (
  __CR_IN__ iFONT*          font,
  __CR_IN__ const void_t*   text,
  __CR_IN__ const sRECT*    rect,
  __CR_IN__ uint_t          type,
  __CR_IN__ uint_t          cpage
    )
{
    sRECT   dest;

    /* 计算目标区域 */
    dest.x1 = rect->x1;
    dest.y1 = rect->y1;
    if (!CR_VCALL(font)->calc_rect(font, text, &dest, cpage))
        return (FALSE);

    /* 对齐后区域输出 */
    egui_rect_align(&dest, rect, type);
    return (CR_VCALL(font)->draw_tran(font, text, &dest, cpage));
}

/*
=======================================
    文字抽象输出 (实体)
=======================================
*/
CR_API bool_t
egui_draw_text2 (
  __CR_IN__ iFONT*          font,
  __CR_IN__ const void_t*   text,
  __CR_IN__ sint_t          x,
  __CR_IN__ sint_t          y,
  __CR_IN__ uint_t          cpage
    )
{
    sRECT   dest;

    dest.x1 = x;
    dest.y1 = y;
    if (!CR_VCALL(font)->calc_rect(font, text, &dest, cpage))
        return (FALSE);
    return (CR_VCALL(font)->draw_text(font, text, &dest, cpage));
}

/*
=======================================
    文字抽象输出 (透明)
=======================================
*/
CR_API bool_t
egui_draw_tran2 (
  __CR_IN__ iFONT*          font,
  __CR_IN__ const void_t*   text,
  __CR_IN__ sint_t          x,
  __CR_IN__ sint_t          y,
  __CR_IN__ uint_t          cpage
    )
{
    sRECT   dest;

    dest.x1 = x;
    dest.y1 = y;
    if (!CR_VCALL(font)->calc_rect(font, text, &dest, cpage))
        return (FALSE);
    return (CR_VCALL(font)->draw_tran(font, text, &dest, cpage));
}

/*
=======================================
    进度条区域计算 (整数)
=======================================
*/
CR_API void_t
egui_rect_probar_i (
  __CR_OT__ sRECT*          dest,
  __CR_IN__ const sRECT*    rect,
  __CR_IN__ uint_t          type,
  __CR_IN__ dist_t          vmin,
  __CR_IN__ dist_t          vmax,
  __CR_IN__ dist_t          vnow
    )
{
    sint_t  step, width;
    leng_t  delta, length;

    /* 过滤参数 */
    if (vmax <= vmin)
        vmax = vmin + 100;

    /* 数值钳位 */
    length = vmax - vmin;
    if (vnow <= vmin)
        delta = 0;
    else
    if (vnow >= vmax)
        delta = length;
    else
        delta = vnow - vmin;
    switch (type)
    {
        default:
        case EGUI_PRO_L2R:
            width = rect->x2 - rect->x1;
            step = (sint_t)((width * delta) / length + rect->x1);
            rect_set_xy(dest, rect->x1, rect->y1, step, rect->y2);
            break;

        case EGUI_PRO_R2L:
            width = rect->x2 - rect->x1;
            step = (sint_t)(rect->x2 - (width * delta) / length);
            rect_set_xy(dest, step, rect->y1, rect->x2, rect->y2);
            break;

        case EGUI_PRO_T2B:
            width = rect->y2 - rect->y1;
            step = (sint_t)((width * delta) / length + rect->y1);
            rect_set_xy(dest, rect->x1, rect->y1, rect->x2, step);
            break;

        case EGUI_PRO_B2T:
            width = rect->y2 - rect->y1;
            step = (sint_t)(rect->y2 - (width * delta) / length);
            rect_set_xy(dest, rect->x1, step, rect->x2, rect->y2);
            break;
    }
}

/*
=======================================
    进度条区域计算 (浮点)
=======================================
*/
CR_API void_t
egui_rect_probar_f (
  __CR_OT__ sRECT*          dest,
  __CR_IN__ const sRECT*    rect,
  __CR_IN__ uint_t          type,
  __CR_IN__ real_t          vmin,
  __CR_IN__ real_t          vmax,
  __CR_IN__ real_t          vnow
    )
{
    sint_t  step, width;
    real_t  delta, length;

    /* 过滤参数 */
    if (vmax <= vmin)
        vmax = vmin + 100;

    /* 数值钳位 */
    length = vmax - vmin;
    if (vnow <= vmin)
        delta = 0;
    else
    if (vnow >= vmax)
        delta = length;
    else
        delta = vnow - vmin;
    switch (type)
    {
        default:
        case EGUI_PRO_L2R:
            width = rect->x2 - rect->x1;
            step = (sint_t)((width * delta) / length + rect->x1);
            if (step > rect->x2) step = rect->x2;
            rect_set_xy(dest, rect->x1, rect->y1, step, rect->y2);
            break;

        case EGUI_PRO_R2L:
            width = rect->x2 - rect->x1;
            step = (sint_t)(rect->x2 - (width * delta) / length);
            if (step < rect->x1) step = rect->x1;
            rect_set_xy(dest, step, rect->y1, rect->x2, rect->y2);
            break;

        case EGUI_PRO_T2B:
            width = rect->y2 - rect->y1;
            step = (sint_t)((width * delta) / length + rect->y1);
            if (step > rect->y2) step = rect->y2;
            rect_set_xy(dest, rect->x1, rect->y1, rect->x2, step);
            break;

        case EGUI_PRO_B2T:
            width = rect->y2 - rect->y1;
            step = (sint_t)(rect->y2 - (width * delta) / length);
            if (step < rect->y1) step = rect->y1;
            rect_set_xy(dest, rect->x1, step, rect->x2, rect->y2);
            break;
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
