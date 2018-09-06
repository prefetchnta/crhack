/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2013-05-08  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 黑白图片函数库 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "phylib.h"

/*****************************************************************************/
/*                                  直方图                                   */
/*****************************************************************************/

/*
=======================================
    直方图阈值计算 (平均值)
=======================================
*/
CR_API byte_t
histo_avge (
  __CR_IN__ const leng_t    tab[256]
    )
{
    uint_t  idx;
    int64u  val;
    int64u  count = 0;
    int64u  total = 0;

    /* 计算平均亮度 */
    for (idx = 0; idx < 256; idx++) {
        count += tab[idx];
        val = tab[idx];
        total += val * idx;
    }
    return ((byte_t)(total / count));
}

/*
=======================================
    直方图阈值计算 (大津法)
=======================================
*/
CR_API byte_t
histo_otsu (
  __CR_IN__ const leng_t    tab[256]
    )
{
    sint_t  delta;
    uint_t  ii, jj;
    int64s  u1, u2;
    int64s  cnts = 0;
    uint_t  idx = 127;
    double  omax = -2.0;
    int64s  n1, n2, val;
    double  w1, w2, otsu;

    /* 统计总像素个数 */
    for (ii = 0; ii < 256; ii++)
        cnts += tab[ii];

    /* 穷举灰度阈值求出最大类间差异 */
    for (ii = 1; ii < 255; ii++)
    {
        /* 计算类间差异 */
        n1 = u1 = 0;
        for (jj = 0; jj < ii; jj++) {
            n1 += tab[jj];
            val = tab[jj];
            u1 += val * jj;
        }
        if (n1 == 0)
            continue;
        n2 = cnts - n1;
        if (n2 == 0)
            continue;
        for (u2 = 0; jj < 256; jj++) {
            val = tab[jj];
            u2 += val * jj;
        }
        u1 /= n1;
        u2 /= n2;
        w1 = (double)n1;
        w1 /= cnts;
        w2 = 1.0 - w1;
        delta = (sint_t)(u1 - u2);
        otsu = w1 * w2 * delta * delta;
        if (otsu > omax) {
            omax = otsu;
            idx = ii;
        }
    }
    return ((byte_t)idx);
}

/*****************************************************************************/
/*                                 骨架抽取                                  */
/*****************************************************************************/

/*
=======================================
    骨架抽取 (Zhang-Suen)
=======================================
*/
CR_API sIMAGE*
skeleton_zhang (
  __CR_IN__ const sIMAGE*   img
    )
{
    sPNT2   spnt;
    sPNT2*  sptr;
    bool_t  odds;
    leng_t  size;
    sARRAY  list;
    sIMAGE* dest;
    byte_t* line0;
    byte_t* line1;
    byte_t* line2;
    uint_t  count;
    uint_t  ap, kk;
    uint_t  xx, yy;
    uint_t  ww, hh;
    uint_t  crss[9];

    /* 输入必须为二值图 */
    if (img->fmt != CR_INDEX8)
        return (NULL);

    /* 图片宽高过滤 */
    ww = img->position.ww;
    hh = img->position.hh;
    if (ww <= 2 || hh <= 2)
        return (NULL);

    /* 清除源图边框 */
    dest = image_dup(img);
    if (dest == NULL)
        return (NULL);
    hh -= 1;
    line0 = dest->data;
    line1 = dest->data + hh * dest->bpl;
    for (xx = 0; xx < ww; xx++) {
        line0[xx] = 0x00;
        line1[xx] = 0x00;
    }
    ww -= 1;
    line1 = line0 + ww;
    for (yy = 1; yy < hh; yy++) {
        *line0 = 0x00;
        *line1 = 0x00;
        line0 += dest->bpl;
        line1 += dest->bpl;
    }

    /* 开始骨架抽取 */
    odds = TRUE;
    array_initT(&list, sPNT2);
    for (;;)
    {
        line0 = dest->data;
        line1 = line0 + dest->bpl;
        line2 = line1 + dest->bpl;

        for (yy = 1; yy < hh; yy++)
        {
            for (xx = 1; xx < ww; xx++)
            {
                if (line1[xx] == 0x00)
                    continue;
                crss[1] = (line0[xx + 0] != 0x00) ? 1 : 0;
                crss[2] = (line0[xx + 1] != 0x00) ? 1 : 0;
                crss[3] = (line1[xx + 1] != 0x00) ? 1 : 0;
                crss[4] = (line2[xx + 1] != 0x00) ? 1 : 0;
                crss[5] = (line2[xx + 0] != 0x00) ? 1 : 0;
                crss[6] = (line2[xx - 1] != 0x00) ? 1 : 0;
                crss[7] = (line1[xx - 1] != 0x00) ? 1 : 0;
                crss[8] = (line0[xx - 1] != 0x00) ? 1 : 0;
                for (count = 0, kk = 1; kk < 9; kk++)
                    count += crss[kk];
                if (!(count >= 2 && count <= 6))
                    continue;
                ap = 0;
                if ((crss[1] == 0) && (crss[2] == 1)) ap++;
                if ((crss[2] == 0) && (crss[3] == 1)) ap++;
                if ((crss[3] == 0) && (crss[4] == 1)) ap++;
                if ((crss[4] == 0) && (crss[5] == 1)) ap++;
                if ((crss[5] == 0) && (crss[6] == 1)) ap++;
                if ((crss[6] == 0) && (crss[7] == 1)) ap++;
                if ((crss[7] == 0) && (crss[8] == 1)) ap++;
                if ((crss[8] == 0) && (crss[1] == 1)) ap++;
                if (ap != 1)
                    continue;
                if ( odds && (crss[3] * crss[5] * crss[7] == 0)
                          && (crss[1] * crss[3] * crss[5] == 0))
                {
                    spnt.x = xx;
                    spnt.y = yy;
                    if (array_push_growT(&list, sPNT2, &spnt) == NULL)
                        goto _failure;
                }
                else
                if (!odds && (crss[1] * crss[5] * crss[7] == 0)
                          && (crss[1] * crss[3] * crss[7] == 0))
                {
                    spnt.x = xx;
                    spnt.y = yy;
                    if (array_push_growT(&list, sPNT2, &spnt) == NULL)
                        goto _failure;
                }
            }
            line0 += dest->bpl;
            line1 += dest->bpl;
            line2 += dest->bpl;
        }

        /* 没有可删除点退出抽取 */
        size = array_get_sizeT(&list, sPNT2);
        if (size == 0)
            break;
        sptr = array_get_dataT(&list, sPNT2);

        /* 清除列表里的点 */
        for (; size != 0; size--, sptr++) {
            line0 = pixel_addr1(dest, sptr->x, sptr->y);
            *line0 = 0x00;
        }
        array_clearT(&list, sPNT2);
        odds = !odds;
    }
    array_freeT(&list, sPNT2);
    return (dest);

_failure:
    array_freeT(&list, sPNT2);
    image_del(dest);
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
