/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2024-11-26  */
/*     #######          ###    ###      [GFX2]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 形状填充函数库 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "blit.h"
#include "datlib.h"

/* 内部结构 */
typedef struct
{
        sARRAY* stk;
        byte_t* chk;
        leng_t  start;
        sint_t  sx;
        sint_t  sy;
        cpix_t  color;
        cpix_t  cback;
        int32u  mask;

} sSCAN_PARAM;

/*
---------------------------------------
    向左向右扫描到边界
---------------------------------------
*/
static bool_t
scan_left2right (
  __CR_IN__ const sIMAGE*       dst,
  __CR_IN__ const sSCAN_PARAM*  prm
    )
{
    sPNT2   tt;
    sint_t  ii;
    sint_t  ww;
    leng_t  bpc;
    leng_t  ptr;
    int32u  mask;
    int32u  nmsk;
    cpix_t  ctmp;
    byte_t* sets;

    tt.y = prm->sy;
    ww = dst->clip_win.ww;
    sets = prm->chk + tt.y * ww;
    bpc = dst->bpc;
    mask = prm->mask;
    nmsk = ~mask;

    /* 判断当前点 */
    mem_cpy(&ctmp, &dst->data[prm->start], bpc);
    if (prm->cback.val != (ctmp.val & mask))
        return (TRUE);
    ctmp.val &= nmsk;
    ctmp.val |= prm->color.val;
    mem_cpy(&dst->data[prm->start], &ctmp, bpc);
    sets[prm->sx] = TRUE;
    tt.x = prm->sx;
    if (array_push_growT(prm->stk, sPNT2, &tt) == NULL)
        return (FALSE);

    /* 向左扫描 */
    ptr = prm->start - bpc;
    for (ii = prm->sx - 1; ii >= 0; ii--, ptr -= bpc)
    {
        /* 不是背景值直接跳出 */
        mem_cpy(&ctmp, &dst->data[ptr], bpc);
        if (prm->cback.val != (ctmp.val & mask))
            break;

        /* 背景值则填充后入栈 */
        ctmp.val &= nmsk;
        ctmp.val |= prm->color.val;
        mem_cpy(&dst->data[ptr], &ctmp, bpc);
        sets[ii] = TRUE;
        tt.x = ii;
        if (array_push_growT(prm->stk, sPNT2, &tt) == NULL)
            return (FALSE);
    }

    /* 向右扫描 */
    ptr = prm->start + bpc;
    for (ii = prm->sx + 1; ii < ww; ii++, ptr += bpc)
    {
        /* 不是背景值直接跳出 */
        mem_cpy(&ctmp, &dst->data[ptr], bpc);
        if (prm->cback.val != (ctmp.val & mask))
            break;

        /* 背景值则填充后入栈 */
        ctmp.val &= nmsk;
        ctmp.val |= prm->color.val;
        mem_cpy(&dst->data[ptr], &ctmp, bpc);
        sets[ii] = TRUE;
        tt.x = ii;
        if (array_push_growT(prm->stk, sPNT2, &tt) == NULL)
            return (FALSE);
    }

    /* 一行扫完 */
    return (TRUE);
}

/*
=======================================
    封闭区域颜色填充
=======================================
*/
CR_API void_t
fill_shape (
  __CR_IN__ const sIMAGE*   dst,
  __CR_IN__ uint_t          sx,
  __CR_IN__ uint_t          sy,
  __CR_IN__ cpix_t          color
    )
{
    sPNT2   pnt;
    sint_t  nxt;
    int32u  mask;
    cpix_t  cback;
    sARRAY  stack;
    leng_t  start;
    byte_t* chked;
    sint_t  width;
    sint_t  height;
    /* --------- */
    sSCAN_PARAM prm;

    /* 参数检查 */
    if (dst->bpc == 0 || dst->bpc > 4 ||
        sx >= dst->clip_win.ww || sy >= dst->clip_win.hh)
        return;
    switch (dst->fmt)
    {
        default: return;
        case CR_INDEX8: mask = 0xFF; break;
        case CR_ARGB565: mask = 0xFFFF; break;
        case CR_ARGB888: mask = 0xFFFFFF; break;
        case CR_ARGB4444: mask = 0xFFF; break;
        case CR_ARGBX555: mask = 0x7FFF; break;
        case CR_ARGB1555: mask = 0x7FFF; break;
        case CR_ARGB8888: mask = 0xFFFFFF; break;
    }
    width  = (sint_t)dst->clip_win.ww;
    height = (sint_t)dst->clip_win.hh;

    /* 创建已扫描标志图 */
    chked = (byte_t*)mem_calloc(width, height);
    if (chked == NULL)
        return;
    mem_zero(chked, width * height);

    /* 创建扫描点的栈 */
    array_initT(&stack, sPNT2);
    if (!array_reserveT(&stack, sPNT2, width * 4))
        goto _func_out1;

    /* 取背景点色 */
    height -= 1;
    if (!dst->gdi)
        start = dst->bpl * sy + dst->bpc * sx;
    else
        start = dst->bpl * (height - sy) + dst->bpc * sx;
    mem_cpy(&cback, &dst->data[start], dst->bpc);

    /* 扫描起始行 */
    prm.stk = &stack;
    prm.chk = chked;
    prm.start = start;
    prm.sx = sx;
    prm.sy = sy;
    prm.color.val = color.val & mask;
    prm.cback.val = cback.val & mask;
    prm.mask = mask;
    if (!scan_left2right(dst, &prm))
        goto _func_out2;
    for (;;)
    {
        /* 取出一点扫描他的上下行直到没有点为止 */
        if (!array_popT(&stack, sPNT2, &pnt))
            break;
        if (pnt.y > 0) {
            nxt = pnt.y - 1;
            if (!chked[nxt * width + pnt.x]) {
                prm.sx = pnt.x;
                prm.sy = nxt;
                if (!dst->gdi)
                    start = dst->bpl * nxt + dst->bpc * pnt.x;
                else
                    start = dst->bpl * (height - nxt) + dst->bpc * pnt.x;
                if (!scan_left2right(dst, &prm))
                    goto _func_out2;
            }
        }
        if (pnt.y < height) {
            nxt = pnt.y + 1;
            if (!chked[nxt * width + pnt.x]) {
                prm.sx = pnt.x;
                prm.sy = nxt;
                if (!dst->gdi)
                    start = dst->bpl * nxt + dst->bpc * pnt.x;
                else
                    start = dst->bpl * (height - nxt) + dst->bpc * pnt.x;
                if (!scan_left2right(dst, &prm))
                    goto _func_out2;
            }
        }
    }
_func_out2:
    array_freeT(&stack, sPNT2);
_func_out1:
    mem_free(chked);
    return;
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
