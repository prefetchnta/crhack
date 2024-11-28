/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2012-03-22  */
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
/*  >>>>>>>>>>>>>>>>>>>>> CrHack 软件 BLIT 函数库 (C) <<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "blit.h"
#include "blends.h"
#include "memlib.h"

/*****************************************************************************/
/*                                 填充操作                                  */
/*****************************************************************************/

/*
=======================================
    逻辑与填充
=======================================
*/
#define CR_BLT_MODE CR_BLT_AND
#define CR_NAME_FILLZ(n) fill_and##n
#include "pixels.inl"
#undef  CR_BLT_MODE
#undef  CR_NAME_FILLZ

/*
=======================================
    逻辑或填充
=======================================
*/
#define CR_BLT_MODE CR_BLT_ORR
#define CR_NAME_FILLZ(n) fill_orr##n
#include "pixels.inl"
#undef  CR_BLT_MODE
#undef  CR_NAME_FILLZ

/*
=======================================
    逻辑非填充
=======================================
*/
#define CR_BLT_MODE CR_BLT_NOT
#define CR_NAME_FILLZ(n) fill_not##n
#include "pixels.inl"
#undef  CR_BLT_MODE
#undef  CR_NAME_FILLZ

/*
=======================================
    逻辑异或填充
=======================================
*/
#define CR_BLT_MODE CR_BLT_XOR
#define CR_NAME_FILLZ(n) fill_xor##n
#include "pixels.inl"
#undef  CR_BLT_MODE
#undef  CR_NAME_FILLZ

/*
=======================================
    直接绘制填充 (高彩)
=======================================
*/
#define CR_BLT_MODE CR_BLT_SET
#define CR_NAME_FILLZ(n) fill_set##n
#include "pixels.inl"
#undef  CR_BLT_MODE
#undef  CR_NAME_FILLZ

/*
=======================================
    直接绘制填充 (08位色)
=======================================
*/
CR_API void_t
fill_set08_c (
  __CR_IN__ const sIMAGE*   dst,
  __CR_IN__ const sFILL*    fill,
  __CR_IN__ cpix_t          color,
  __CR_IN__ const sRECT*    rect
    )
{
    sBLTer  oper;

    /* 过滤输入参数 */
    if (dst->bpc != 1 ||
        !blt_use_fill_clip(&oper, dst, 1, fill, rect))
        return;
    while (oper.height-- != 0) {
        mem_set(oper.addr, color.c08.idx, oper.width);
        oper.addr += oper.rest;
        oper.addr += oper.width;
    }
}

/*
=======================================
    饱和加法填充
=======================================
*/
#define CR_BLT_MODE CR_BLT_ADD
#define CR_NAME_FILLZ(n) fill_add##n
#include "blends.inl"
#undef  CR_BLT_MODE
#undef  CR_NAME_FILLZ

/*
=======================================
    饱和减法填充
=======================================
*/
#define CR_BLT_MODE CR_BLT_SUB
#define CR_NAME_FILLZ(n) fill_sub##n
#include "blends.inl"
#undef  CR_BLT_MODE
#undef  CR_NAME_FILLZ

/*
=======================================
    插值混合填充
=======================================
*/
#define CR_BLT_MODE CR_BLT_ALP
#define CR_NAME_FILLZ(n) fill_lrp##n
#include "blends.inl"
#undef  CR_BLT_MODE
#undef  CR_NAME_FILLZ

/*****************************************************************************/
/*                                BLIT 操作                                  */
/*****************************************************************************/

/*
=======================================
    逻辑与 BLIT
=======================================
*/
#define CR_BLT_MODE CR_BLT_AND
#define CR_NAME_BLITZ(n) blit_and##n
#include "pixels.inl"
#undef  CR_BLT_MODE
#undef  CR_NAME_BLITZ

/*
=======================================
    逻辑或 BLIT
=======================================
*/
#define CR_BLT_MODE CR_BLT_ORR
#define CR_NAME_BLITZ(n) blit_orr##n
#include "pixels.inl"
#undef  CR_BLT_MODE
#undef  CR_NAME_BLITZ

/*
=======================================
    逻辑异或 BLIT
=======================================
*/
#define CR_BLT_MODE CR_BLT_XOR
#define CR_NAME_BLITZ(n) blit_xor##n
#include "pixels.inl"
#undef  CR_BLT_MODE
#undef  CR_NAME_BLITZ

/*
=======================================
    直接绘制 BLIT (08位色)
=======================================
*/
CR_API void_t
blit_set08_c (
  __CR_IN__ const sIMAGE*   dst,
  __CR_IN__ const sIMAGE*   src,
  __CR_IN__ const sBLIT*    blit,
  __CR_IN__ const sRECT*    rect
    )
{
    sBLTer  doper;
    sBLTer  soper;

    /* 过滤输入参数 */
    if (dst->bpc != 1 || src->bpc != 1 ||
        !blt_use_blit_clip(&doper, &soper, 1, 1, dst, src, blit, rect))
        return;
    while (soper.height-- != 0) {
        mem_cpy(doper.addr, soper.addr, soper.width);
        doper.addr += doper.rest;
        soper.addr += soper.rest;
        doper.addr += doper.width;
        soper.addr += soper.width;
    }
}

/*
=======================================
    直接绘制 BLIT (16位色)
=======================================
*/
CR_API void_t
blit_set16_c (
  __CR_IN__ const sIMAGE*   dst,
  __CR_IN__ const sIMAGE*   src,
  __CR_IN__ const sBLIT*    blit,
  __CR_IN__ const sRECT*    rect
    )
{
    sBLTer  doper;
    sBLTer  soper;

    /* 过滤输入参数 */
    if (dst->bpc != 2 || src->bpc != 2 ||
        !blt_use_blit_clip(&doper, &soper, 2, 2, dst, src, blit, rect))
        return;
    while (soper.height-- != 0) {
        mem_cpy(doper.addr, soper.addr, soper.width);
        doper.addr += doper.rest;
        soper.addr += soper.rest;
        doper.addr += doper.width;
        soper.addr += soper.width;
    }
}

/*
=======================================
    直接绘制 BLIT (24位色)
=======================================
*/
CR_API void_t
blit_set24_c (
  __CR_IN__ const sIMAGE*   dst,
  __CR_IN__ const sIMAGE*   src,
  __CR_IN__ const sBLIT*    blit,
  __CR_IN__ const sRECT*    rect
    )
{
    sBLTer  doper;
    sBLTer  soper;

    /* 过滤输入参数 */
    if (dst->bpc != 3 || src->bpc != 3 ||
        !blt_use_blit_clip(&doper, &soper, 3, 3, dst, src, blit, rect))
        return;
    while (soper.height-- != 0) {
        mem_cpy(doper.addr, soper.addr, soper.width);
        doper.addr += doper.rest;
        soper.addr += soper.rest;
        doper.addr += doper.width;
        soper.addr += soper.width;
    }
}

/*
=======================================
    直接绘制 BLIT (32位色)
=======================================
*/
CR_API void_t
blit_set32_c (
  __CR_IN__ const sIMAGE*   dst,
  __CR_IN__ const sIMAGE*   src,
  __CR_IN__ const sBLIT*    blit,
  __CR_IN__ const sRECT*    rect
    )
{
    sBLTer  doper;
    sBLTer  soper;

    /* 过滤输入参数 */
    if (dst->bpc != 4 || src->bpc != 4 ||
        !blt_use_blit_clip(&doper, &soper, 4, 4, dst, src, blit, rect))
        return;
    while (soper.height-- != 0) {
        mem_cpy(doper.addr, soper.addr, soper.width);
        doper.addr += doper.rest;
        soper.addr += soper.rest;
        doper.addr += doper.width;
        soper.addr += soper.width;
    }
}

/*
=======================================
    源透明色 BLIT (08位色)
=======================================
*/
CR_API void_t
blit_str08_c (
  __CR_IN__ const sIMAGE*   dst,
  __CR_IN__ const sIMAGE*   src,
  __CR_IN__ const sBLIT*    blit,
  __CR_IN__ cpix_t          trans,
  __CR_IN__ const sRECT*    rect
    )
{
    leng_t  idx;
    leng_t  len;
    sBLTer  doper;
    sBLTer  soper;

    /* 过滤输入参数 */
    if (dst->bpc != 1 || src->bpc != 1 ||
        !blt_use_blit_clip(&doper, &soper, 1, 1, dst, src, blit, rect))
        return;
    len = soper.width;
    while (soper.height-- != 0) {
        for (idx = len; idx != 0; idx--) {
            if (soper.addr[0] != trans.c08.idx)
                doper.addr[0] = soper.addr[0];
            doper.addr += 1;
            soper.addr += 1;
        }
        doper.addr += doper.rest;
        soper.addr += soper.rest;
    }
}

/*
=======================================
    源透明色 BLIT (16位色)
=======================================
*/
CR_API void_t
blit_str16_c (
  __CR_IN__ const sIMAGE*   dst,
  __CR_IN__ const sIMAGE*   src,
  __CR_IN__ const sBLIT*    blit,
  __CR_IN__ cpix_t          trans,
  __CR_IN__ const sRECT*    rect
    )
{
    leng_t  idx;
    leng_t  len;
    sBLTer  doper;
    sBLTer  soper;

    /* 过滤输入参数 */
    if (dst->bpc != 2 || src->bpc != 2 ||
        !blt_use_blit_clip(&doper, &soper, 2, 2, dst, src, blit, rect))
        return;
    len = soper.width / 2;
    while (soper.height-- != 0) {
        for (idx = len; idx != 0; idx--) {
            if (mem_cmp(soper.addr, &trans, 2) != 0)
                mem_cpy(doper.addr, soper.addr, 2);
            doper.addr += 2;
            soper.addr += 2;
        }
        doper.addr += doper.rest;
        soper.addr += soper.rest;
    }
}

/*
=======================================
    源透明色 BLIT (24位色)
=======================================
*/
CR_API void_t
blit_str24_c (
  __CR_IN__ const sIMAGE*   dst,
  __CR_IN__ const sIMAGE*   src,
  __CR_IN__ const sBLIT*    blit,
  __CR_IN__ cpix_t          trans,
  __CR_IN__ const sRECT*    rect
    )
{
    leng_t  idx;
    leng_t  len;
    sBLTer  doper;
    sBLTer  soper;

    /* 过滤输入参数 */
    if (dst->bpc != 3 || src->bpc != 3 ||
        !blt_use_blit_clip(&doper, &soper, 3, 3, dst, src, blit, rect))
        return;
    len = soper.width / 3;
    while (soper.height-- != 0) {
        for (idx = len; idx != 0; idx--) {
            if (mem_cmp(soper.addr, &trans, 3) != 0)
                mem_cpy(doper.addr, soper.addr, 3);
            doper.addr += 3;
            soper.addr += 3;
        }
        doper.addr += doper.rest;
        soper.addr += soper.rest;
    }
}

/*
=======================================
    源透明色 BLIT (32位色)
=======================================
*/
CR_API void_t
blit_str32_c (
  __CR_IN__ const sIMAGE*   dst,
  __CR_IN__ const sIMAGE*   src,
  __CR_IN__ const sBLIT*    blit,
  __CR_IN__ cpix_t          trans,
  __CR_IN__ const sRECT*    rect
    )
{
    leng_t  idx;
    leng_t  len;
    sBLTer  doper;
    sBLTer  soper;

    /* 过滤输入参数 */
    if (dst->bpc != 4 || src->bpc != 4 ||
        !blt_use_blit_clip(&doper, &soper, 4, 4, dst, src, blit, rect))
        return;
    len = soper.width / 4;
    while (soper.height-- != 0) {
        for (idx = len; idx != 0; idx--) {
            if (mem_cmp(soper.addr, &trans, 3) != 0)
                mem_cpy(doper.addr, soper.addr, 4);
            doper.addr += 4;
            soper.addr += 4;
        }
        doper.addr += doper.rest;
        soper.addr += soper.rest;
    }
}

/*
=======================================
    目标关键 BLIT (08位色)
=======================================
*/
CR_API void_t
blit_dtr08_c (
  __CR_IN__ const sIMAGE*   dst,
  __CR_IN__ const sIMAGE*   src,
  __CR_IN__ const sBLIT*    blit,
  __CR_IN__ cpix_t          trans,
  __CR_IN__ const sRECT*    rect
    )
{
    leng_t  idx;
    leng_t  len;
    sBLTer  doper;
    sBLTer  soper;

    /* 过滤输入参数 */
    if (dst->bpc != 1 || src->bpc != 1 ||
        !blt_use_blit_clip(&doper, &soper, 1, 1, dst, src, blit, rect))
        return;
    len = soper.width;
    while (soper.height-- != 0) {
        for (idx = len; idx != 0; idx--) {
            if (doper.addr[0] == trans.c08.idx)
                doper.addr[0] = soper.addr[0];
            doper.addr += 1;
            soper.addr += 1;
        }
        doper.addr += doper.rest;
        soper.addr += soper.rest;
    }
}

/*
=======================================
    目标关键 BLIT (16位色)
=======================================
*/
CR_API void_t
blit_dtr16_c (
  __CR_IN__ const sIMAGE*   dst,
  __CR_IN__ const sIMAGE*   src,
  __CR_IN__ const sBLIT*    blit,
  __CR_IN__ cpix_t          trans,
  __CR_IN__ const sRECT*    rect
    )
{
    leng_t  idx;
    leng_t  len;
    sBLTer  doper;
    sBLTer  soper;

    /* 过滤输入参数 */
    if (dst->bpc != 2 || src->bpc != 2 ||
        !blt_use_blit_clip(&doper, &soper, 2, 2, dst, src, blit, rect))
        return;
    len = soper.width / 2;
    while (soper.height-- != 0) {
        for (idx = len; idx != 0; idx--) {
            if (mem_cmp(doper.addr, &trans, 2) == 0)
                mem_cpy(doper.addr, soper.addr, 2);
            doper.addr += 2;
            soper.addr += 2;
        }
        doper.addr += doper.rest;
        soper.addr += soper.rest;
    }
}

/*
=======================================
    目标关键 BLIT (24位色)
=======================================
*/
CR_API void_t
blit_dtr24_c (
  __CR_IN__ const sIMAGE*   dst,
  __CR_IN__ const sIMAGE*   src,
  __CR_IN__ const sBLIT*    blit,
  __CR_IN__ cpix_t          trans,
  __CR_IN__ const sRECT*    rect
    )
{
    leng_t  idx;
    leng_t  len;
    sBLTer  doper;
    sBLTer  soper;

    /* 过滤输入参数 */
    if (dst->bpc != 3 || src->bpc != 3 ||
        !blt_use_blit_clip(&doper, &soper, 3, 3, dst, src, blit, rect))
        return;
    len = soper.width / 3;
    while (soper.height-- != 0) {
        for (idx = len; idx != 0; idx--) {
            if (mem_cmp(doper.addr, &trans, 3) == 0)
                mem_cpy(doper.addr, soper.addr, 3);
            doper.addr += 3;
            soper.addr += 3;
        }
        doper.addr += doper.rest;
        soper.addr += soper.rest;
    }
}

/*
=======================================
    目标关键 BLIT (32位色)
=======================================
*/
CR_API void_t
blit_dtr32_c (
  __CR_IN__ const sIMAGE*   dst,
  __CR_IN__ const sIMAGE*   src,
  __CR_IN__ const sBLIT*    blit,
  __CR_IN__ cpix_t          trans,
  __CR_IN__ const sRECT*    rect
    )
{
    leng_t  idx;
    leng_t  len;
    sBLTer  doper;
    sBLTer  soper;

    /* 过滤输入参数 */
    if (dst->bpc != 4 || src->bpc != 4 ||
        !blt_use_blit_clip(&doper, &soper, 4, 4, dst, src, blit, rect))
        return;
    len = soper.width / 4;
    while (soper.height-- != 0) {
        for (idx = len; idx != 0; idx--) {
            if (mem_cmp(doper.addr, &trans, 3) == 0)
                mem_cpy(doper.addr, soper.addr, 4);
            doper.addr += 4;
            soper.addr += 4;
        }
        doper.addr += doper.rest;
        soper.addr += soper.rest;
    }
}

/*
=======================================
    饱和加法 BLIT
=======================================
*/
#define CR_BLT_MODE CR_BLT_ADD
#define CR_NAME_BLITZ(n) blit_add##n
#include "blends.inl"
#undef  CR_BLT_MODE
#undef  CR_NAME_BLITZ

/*
=======================================
    饱和减法 BLIT
=======================================
*/
#define CR_BLT_MODE CR_BLT_SUB
#define CR_NAME_BLITZ(n) blit_sub##n
#include "blends.inl"
#undef  CR_BLT_MODE
#undef  CR_NAME_BLITZ

/*
=======================================
    插值混合 BLIT
=======================================
*/
#define CR_BLT_MODE CR_BLT_ALP
#define CR_NAME_BLITZ(n) blit_lrp##n
#include "blends.inl"
#undef  CR_BLT_MODE
#undef  CR_NAME_BLITZ

/*
=======================================
    通道混合 BLIT (08位色)
=======================================
*/
CR_API void_t
blit_alp08_c (
  __CR_IN__ const sIMAGE*   dst,
  __CR_IN__ const sIMAGE*   src,
  __CR_IN__ const sIMAGE*   chn,
  __CR_IN__ const sBLIT*    blit,
  __CR_IN__ const sRECT*    rect
    )
{
    leng_t  idx;
    leng_t  len;
    sBLTer  doper;
    sBLTer  soper;
    sBLTer  aoper;

    CR_BLEND_LRP_INIT08

    /* alp 大小要与 src 一致 */
    if (chn == NULL)
        return;
    if (chn->bpc != 1 ||
        dst->bpc != 1 || src->bpc != 1 ||
        chn->position.ww < src->position.ww ||
        chn->position.hh < src->position.hh ||
        !blt_use_blit_clip(&doper, &soper, 1, 1, dst, src, blit, rect))
        return;
    blt_use_blit_clip(&doper, &aoper, 1, 1, dst, chn, blit, rect);

    /* 开始像素混合 */
    len = soper.width;
    while (soper.height-- != 0) {
        for (idx = len; idx != 0; idx--) {
            CR_BLEND_LRP_LOAD08(aoper.addr[0])
            CR_BLEND_LRP_MAIN08(doper.addr[0], soper.addr[0])
            doper.addr += 1;
            soper.addr += 1;
            aoper.addr += 1;
        }
        doper.addr += doper.rest;
        soper.addr += soper.rest;
        aoper.addr += aoper.rest;
    }
}

/*
=======================================
    通道混合 BLIT (12位色)
=======================================
*/
CR_API void_t
blit_alp12_c (
  __CR_IN__ const sIMAGE*   dst,
  __CR_IN__ const sIMAGE*   src,
  __CR_IN__ const sIMAGE*   chn,
  __CR_IN__ const sBLIT*    blit,
  __CR_IN__ const sRECT*    rect
    )
{
    leng_t  idx;
    leng_t  len;
    sBLTer  doper;
    sBLTer  soper;
    sBLTer  aoper;

    CR_BLEND_LRP_INIT12
    if (dst->bpc != 2 || src->bpc != 2 ||
        !blt_use_blit_clip(&doper, &soper, 2, 2, dst, src, blit, rect))
        return;
    len = soper.width / 2;

    /* 可以选择外部通道 */
    if (chn == NULL) {
        while (soper.height-- != 0) {
            for (idx = len; idx != 0; idx--) {
                CR_BLEND_LRP_LOAD12(soper.addr[1] >> 4)
                CR_BLEND_LRP_MAIN12(*(int16u*)doper.addr,
                                    *(int16u*)soper.addr)
                doper.addr += 2;
                soper.addr += 2;
            }
            doper.addr += doper.rest;
            soper.addr += soper.rest;
        }
    }
    else {
        if (chn->bpc != 1 ||
            chn->position.ww < src->position.ww ||
            chn->position.hh < src->position.hh)
            return;
        blt_use_blit_clip(&doper, &aoper, 2, 1, dst, chn, blit, rect);

        /* 开始像素混合 */
        while (soper.height-- != 0) {
            for (idx = len; idx != 0; idx--) {
                CR_BLEND_LRP_LOAD12(aoper.addr[0] >> 4)
                CR_BLEND_LRP_MAIN12(*(int16u*)doper.addr,
                                    *(int16u*)soper.addr)
                doper.addr += 2;
                soper.addr += 2;
                aoper.addr += 1;
            }
            doper.addr += doper.rest;
            soper.addr += soper.rest;
            aoper.addr += aoper.rest;
        }
    }
}

/*
=======================================
    通道混合 BLIT (15位色)
=======================================
*/
CR_API void_t
blit_alp15_c (
  __CR_IN__ const sIMAGE*   dst,
  __CR_IN__ const sIMAGE*   src,
  __CR_IN__ const sIMAGE*   chn,
  __CR_IN__ const sBLIT*    blit,
  __CR_IN__ const sRECT*    rect
    )
{
    leng_t  idx;
    leng_t  len;
    sBLTer  doper;
    sBLTer  soper;
    sBLTer  aoper;

    CR_BLEND_LRP_INIT15
    if (dst->bpc != 2 || src->bpc != 2 ||
        !blt_use_blit_clip(&doper, &soper, 2, 2, dst, src, blit, rect))
        return;
    len = soper.width / 2;

    /* 可以选择外部通道 */
    if (chn == NULL) {
        while (soper.height-- != 0) {
            for (idx = len; idx != 0; idx--) {
                alp = (sint_t)((soper.addr[1] >> 7) * 32);
                CR_BLEND_LRP_MAIN15(*(int16u*)doper.addr,
                                    *(int16u*)soper.addr)
                doper.addr += 2;
                soper.addr += 2;
            }
            doper.addr += doper.rest;
            soper.addr += soper.rest;
        }
    }
    else {
        if (chn->bpc != 1 ||
            chn->position.ww < src->position.ww ||
            chn->position.hh < src->position.hh)
            return;
        blt_use_blit_clip(&doper, &aoper, 2, 1, dst, chn, blit, rect);

        /* 开始像素混合 */
        while (soper.height-- != 0) {
            for (idx = len; idx != 0; idx--) {
                CR_BLEND_LRP_LOAD15(aoper.addr[0] >> 3)
                CR_BLEND_LRP_MAIN15(*(int16u*)doper.addr,
                                    *(int16u*)soper.addr)
                doper.addr += 2;
                soper.addr += 2;
                aoper.addr += 1;
            }
            doper.addr += doper.rest;
            soper.addr += soper.rest;
            aoper.addr += aoper.rest;
        }
    }
}

/*
=======================================
    通道混合 BLIT (16位色)
=======================================
*/
CR_API void_t
blit_alp16_c (
  __CR_IN__ const sIMAGE*   dst,
  __CR_IN__ const sIMAGE*   src,
  __CR_IN__ const sIMAGE*   chn,
  __CR_IN__ const sBLIT*    blit,
  __CR_IN__ const sRECT*    rect
    )
{
    leng_t  idx;
    leng_t  len;
    sBLTer  doper;
    sBLTer  soper;
    sBLTer  aoper;

    CR_BLEND_LRP_INIT16

    /* alp 大小要与 src 一致 */
    if (chn == NULL)
        return;
    if (chn->bpc != 1 ||
        dst->bpc != 2 || src->bpc != 2 ||
        chn->position.ww < src->position.ww ||
        chn->position.hh < src->position.hh ||
        !blt_use_blit_clip(&doper, &soper, 2, 2, dst, src, blit, rect))
        return;
    blt_use_blit_clip(&doper, &aoper, 2, 1, dst, chn, blit, rect);

    /* 开始像素混合 */
    len = soper.width / 2;
    while (soper.height-- != 0) {
        for (idx = len; idx != 0; idx--) {
            CR_BLEND_LRP_LOAD16(aoper.addr[0] >> 2)
            CR_BLEND_LRP_MAIN16(*(int16u*)doper.addr,
                                *(int16u*)soper.addr)
            doper.addr += 2;
            soper.addr += 2;
            aoper.addr += 1;
        }
        doper.addr += doper.rest;
        soper.addr += soper.rest;
        aoper.addr += aoper.rest;
    }
}

/*
=======================================
    通道混合 BLIT (24位色)
=======================================
*/
CR_API void_t
blit_alp24_c (
  __CR_IN__ const sIMAGE*   dst,
  __CR_IN__ const sIMAGE*   src,
  __CR_IN__ const sIMAGE*   chn,
  __CR_IN__ const sBLIT*    blit,
  __CR_IN__ const sRECT*    rect
    )
{
    leng_t  idx;
    leng_t  len;
    sBLTer  doper;
    sBLTer  soper;
    sBLTer  aoper;

    CR_BLEND_LRP_INIT24

    /* alp 大小要与 src 一致 */
    if (chn == NULL)
        return;
    if (chn->bpc != 1 ||
        dst->bpc != 3 || src->bpc != 3 ||
        chn->position.ww < src->position.ww ||
        chn->position.hh < src->position.hh ||
        !blt_use_blit_clip(&doper, &soper, 3, 3, dst, src, blit, rect))
        return;
    blt_use_blit_clip(&doper, &aoper, 3, 1, dst, chn, blit, rect);

    /* 开始像素混合 */
    len = soper.width / 3;
    while (soper.height-- != 0) {
        for (idx = len; idx != 0; idx--) {
            CR_BLEND_LRP_LOAD24(aoper.addr[0])
            CR_BLEND_LRP_MAIN24(doper.addr[0], doper.addr[1],
                    doper.addr[2], soper.addr[0], soper.addr[1],
                                soper.addr[2])
            doper.addr += 3;
            soper.addr += 3;
            aoper.addr += 1;
        }
        doper.addr += doper.rest;
        soper.addr += soper.rest;
        aoper.addr += aoper.rest;
    }
}

/*
=======================================
    通道混合 BLIT (32位色)
=======================================
*/
CR_API void_t
blit_alp32_c (
  __CR_IN__ const sIMAGE*   dst,
  __CR_IN__ const sIMAGE*   src,
  __CR_IN__ const sIMAGE*   chn,
  __CR_IN__ const sBLIT*    blit,
  __CR_IN__ const sRECT*    rect
    )
{
    leng_t  idx;
    leng_t  len;
    sBLTer  doper;
    sBLTer  soper;
    sBLTer  aoper;

    CR_BLEND_LRP_INIT32
    if (dst->bpc != 4 || src->bpc != 4 ||
        !blt_use_blit_clip(&doper, &soper, 4, 4, dst, src, blit, rect))
        return;
    len = soper.width / 4;

    /* 可以选择外部通道 */
    if (chn == NULL) {
        while (soper.height-- != 0) {
            for (idx = len; idx != 0; idx--) {
                CR_BLEND_LRP_LOAD32(soper.addr[3])
                CR_BLEND_LRP_MAIN32(doper.addr[0], doper.addr[1],
                        doper.addr[2], soper.addr[0], soper.addr[1],
                                    soper.addr[2])
                doper.addr += 4;
                soper.addr += 4;
            }
            doper.addr += doper.rest;
            soper.addr += soper.rest;
        }
    }
    else {
        if (chn->bpc != 1 ||
            chn->position.ww < src->position.ww ||
            chn->position.hh < src->position.hh)
            return;
        blt_use_blit_clip(&doper, &aoper, 4, 1, dst, chn, blit, rect);

        /* 开始像素混合 */
        while (soper.height-- != 0) {
            for (idx = len; idx != 0; idx--) {
                CR_BLEND_LRP_LOAD32(aoper.addr[0])
                CR_BLEND_LRP_MAIN32(doper.addr[0], doper.addr[1],
                        doper.addr[2], soper.addr[0], soper.addr[1],
                                    soper.addr[2])
                doper.addr += 4;
                soper.addr += 4;
                aoper.addr += 1;
            }
            doper.addr += doper.rest;
            soper.addr += soper.rest;
            aoper.addr += aoper.rest;
        }
    }
}

/*
=======================================
    掩码绘制 BLIT (08位色)
=======================================
*/
CR_API void_t
blit_msk08_c (
  __CR_IN__ const sIMAGE*   dst,
  __CR_IN__ const sIMAGE*   src,
  __CR_IN__ const sIMAGE*   msk,
  __CR_IN__ const sBLIT*    blit,
  __CR_IN__ const sRECT*    rect,
  __CR_IN__ byte_t          index
    )
{
    leng_t  idx;
    leng_t  len;
    sBLTer  doper;
    sBLTer  soper;
    sBLTer  moper;

    /* msk 大小要与 src 一致 */
    if (msk->bpc != 1 ||
        dst->bpc != 1 || src->bpc != 1 ||
        msk->position.ww < src->position.ww ||
        msk->position.hh < src->position.hh ||
        !blt_use_blit_clip(&doper, &soper, 1, 1, dst, src, blit, rect))
        return;
    blt_use_blit_clip(&doper, &moper, 1, 1, dst, msk, blit, rect);

    /* 开始像素绘制 */
    len = soper.width;
    while (soper.height-- != 0) {
        for (idx = len; idx != 0; idx--) {
            if (moper.addr[0] == index)
                doper.addr[0] = soper.addr[0];
            doper.addr += 1;
            soper.addr += 1;
            moper.addr += 1;
        }
        doper.addr += doper.rest;
        soper.addr += soper.rest;
        moper.addr += moper.rest;
    }
}

/*
=======================================
    掩码绘制 BLIT (16位色)
=======================================
*/
CR_API void_t
blit_msk16_c (
  __CR_IN__ const sIMAGE*   dst,
  __CR_IN__ const sIMAGE*   src,
  __CR_IN__ const sIMAGE*   msk,
  __CR_IN__ const sBLIT*    blit,
  __CR_IN__ const sRECT*    rect,
  __CR_IN__ byte_t          index
    )
{
    leng_t  idx;
    leng_t  len;
    sBLTer  doper;
    sBLTer  soper;
    sBLTer  moper;

    /* msk 大小要与 src 一致 */
    if (msk->bpc != 1 ||
        dst->bpc != 2 || src->bpc != 2 ||
        msk->position.ww < src->position.ww ||
        msk->position.hh < src->position.hh ||
        !blt_use_blit_clip(&doper, &soper, 2, 2, dst, src, blit, rect))
        return;
    blt_use_blit_clip(&doper, &moper, 2, 1, dst, msk, blit, rect);

    /* 开始像素绘制 */
    len = soper.width / 2;
    while (soper.height-- != 0) {
        for (idx = len; idx != 0; idx--) {
            if (moper.addr[0] == index)
                mem_cpy(doper.addr, soper.addr, 2);
            doper.addr += 2;
            soper.addr += 2;
            moper.addr += 1;
        }
        doper.addr += doper.rest;
        soper.addr += soper.rest;
        moper.addr += moper.rest;
    }
}

/*
=======================================
    掩码绘制 BLIT (24位色)
=======================================
*/
CR_API void_t
blit_msk24_c (
  __CR_IN__ const sIMAGE*   dst,
  __CR_IN__ const sIMAGE*   src,
  __CR_IN__ const sIMAGE*   msk,
  __CR_IN__ const sBLIT*    blit,
  __CR_IN__ const sRECT*    rect,
  __CR_IN__ byte_t          index
    )
{
    leng_t  idx;
    leng_t  len;
    sBLTer  doper;
    sBLTer  soper;
    sBLTer  moper;

    /* msk 大小要与 src 一致 */
    if (msk->bpc != 1 ||
        dst->bpc != 3 || src->bpc != 3 ||
        msk->position.ww < src->position.ww ||
        msk->position.hh < src->position.hh ||
        !blt_use_blit_clip(&doper, &soper, 3, 3, dst, src, blit, rect))
        return;
    blt_use_blit_clip(&doper, &moper, 3, 1, dst, msk, blit, rect);

    /* 开始像素绘制 */
    len = soper.width / 3;
    while (soper.height-- != 0) {
        for (idx = len; idx != 0; idx--) {
            if (moper.addr[0] == index)
                mem_cpy(doper.addr, soper.addr, 3);
            doper.addr += 3;
            soper.addr += 3;
            moper.addr += 1;
        }
        doper.addr += doper.rest;
        soper.addr += soper.rest;
        moper.addr += moper.rest;
    }
}

/*
=======================================
    掩码绘制 BLIT (32位色)
=======================================
*/
CR_API void_t
blit_msk32_c (
  __CR_IN__ const sIMAGE*   dst,
  __CR_IN__ const sIMAGE*   src,
  __CR_IN__ const sIMAGE*   msk,
  __CR_IN__ const sBLIT*    blit,
  __CR_IN__ const sRECT*    rect,
  __CR_IN__ byte_t          index
    )
{
    leng_t  idx;
    leng_t  len;
    sBLTer  doper;
    sBLTer  soper;
    sBLTer  moper;

    /* msk 大小要与 src 一致 */
    if (msk->bpc != 1 ||
        dst->bpc != 4 || src->bpc != 4 ||
        msk->position.ww < src->position.ww ||
        msk->position.hh < src->position.hh ||
        !blt_use_blit_clip(&doper, &soper, 4, 4, dst, src, blit, rect))
        return;
    blt_use_blit_clip(&doper, &moper, 4, 1, dst, msk, blit, rect);

    /* 开始像素绘制 */
    len = soper.width / 4;
    while (soper.height-- != 0) {
        for (idx = len; idx != 0; idx--) {
            if (moper.addr[0] == index)
                mem_cpy(doper.addr, soper.addr, 4);
            doper.addr += 4;
            soper.addr += 4;
            moper.addr += 1;
        }
        doper.addr += doper.rest;
        soper.addr += soper.rest;
        moper.addr += moper.rest;
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
