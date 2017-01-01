/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2013-05-24  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack 图片纹理运算函数库 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "memlib.h"
#include "phylib.h"

/*
=======================================
    像素投影直方图 (X 轴)
=======================================
*/
CR_API bool_t
tex_project_x (
  __CR_OT__ uint_t*         prj,
  __CR_IN__ const sIMAGE*   img,
  __CR_IN__ byte_t          idx
    )
{
    uint_t  xx, yy;
    uint_t  ww, hh;
    byte_t* img_line;

    /* 只支持索引图 */
    if (img->fmt != CR_INDEX8)
        return (FALSE);

    /* 计算指定像素投射到 X 轴上的直方图 */
    ww = img->position.ww;
    hh = img->position.hh;
    for (xx = 0; xx < ww; xx++) {
        prj[xx] = 0;
        img_line = img->data + xx;
        for (yy = 0; yy < hh; yy++) {
            if (img_line[0] == idx)
                prj[xx] += 1;
            img_line += img->bpl;
        }
    }
    return (TRUE);
}

/*
=======================================
    像素投影直方图 (Y 轴)
=======================================
*/
CR_API bool_t
tex_project_y (
  __CR_OT__ uint_t*         prj,
  __CR_IN__ const sIMAGE*   img,
  __CR_IN__ byte_t          idx
    )
{
    uint_t  xx, yy;
    uint_t  ww, hh;
    byte_t* img_line;

    /* 只支持索引图 */
    if (img->fmt != CR_INDEX8)
        return (FALSE);

    /* 计算指定像素投射到 Y 轴上的直方图 */
    img_line = img->data;
    ww = img->position.ww;
    hh = img->position.hh;
    for (yy = 0; yy < hh; yy++) {
        prj[yy] = 0;
        for (xx = 0; xx < ww; xx++) {
            if (img_line[xx] == idx)
                prj[yy] += 1;
        }
        img_line += img->bpl;
    }
    return (TRUE);
}

/*
=======================================
    形态统计 (分割 X 轴方向)
=======================================
*/
CR_API bool_t
tex_altern_x (
  __CR_IO__ sTEX_PATTERN*   patt
    )
{
    sRECT   clip;
    uchar*  line;
    sint_t  flag;
    byte_t  idx1;
    byte_t  idx2;
    sint_t  xx, yy;
    uint_t  n1, n2;

    /* 参数过滤 */
    if (patt->img->fmt != CR_INDEX8 || patt->step <= 0)
        return (FALSE);
    if (!clip_rect(&clip, patt->win, &patt->img->clip_win))
        return (FALSE);

    /* 最大也只有宽度个结果 */
    patt->result = (byte_t*)mem_malloc(clip.ww);
    if (patt->result == NULL)
        return (FALSE);

    /* 开始统计形态的个数 */
    patt->nhit = patt->ntot = 0;
    if (!patt->altr)
    {
        /* 直接统计 */
        for (xx = clip.x1 + patt->step; xx < clip.x2; xx += patt->step) {
            n1 = n2 = 0;
            line = patt->img->data + xx;
            for (yy = clip.y1; yy < clip.y2; yy++) {
                idx1 = line[(yy + 0) * patt->img->bpl];
                idx2 = line[(yy + 1) * patt->img->bpl];
                if (idx1 == patt->idx1 && idx2 == patt->idx2)
                    n1 += 1;
                else
                if (idx1 == patt->idx2 && idx2 == patt->idx1)
                    n2 += 1;
            }
            if (n1 >= patt->gmin && n2 >= patt->gmin &&
                n1 <= patt->gmax && n2 <= patt->gmax) {
                patt->nhit += 1;
                patt->result[patt->ntot] = TRUE;
            }
            else {
                patt->result[patt->ntot] = FALSE;
            }
            patt->ntot += 1;
        }
    }
    else
    {
        /* 交替统计 */
        flag = 1;
        for (xx = clip.x1 + patt->step; xx < clip.x2; xx += patt->step) {
            n1 = n2 = 0;
            line = patt->img->data + xx;
            for (yy = clip.y1; yy < clip.y2; yy++) {
                idx1 = line[(yy + 0) * patt->img->bpl];
                idx2 = line[(yy + 1) * patt->img->bpl];
                if (flag > 0) {
                    if (idx1 == patt->idx1 && idx2 == patt->idx2) {
                        n1 += 1;
                        flag = -flag;
                    }
                }
                else {
                    if (idx1 == patt->idx2 && idx2 == patt->idx1) {
                        n2 += 1;
                        flag = -flag;
                    }
                }
            }
            if (n1 >= patt->gmin && n2 >= patt->gmin &&
                n1 <= patt->gmax && n2 <= patt->gmax) {
                patt->nhit += 1;
                patt->result[patt->ntot] = TRUE;
            }
            else {
                patt->result[patt->ntot] = FALSE;
            }
            patt->ntot += 1;
        }
    }
    return (TRUE);
}

/*
=======================================
    形态统计 (分割 Y 轴方向)
=======================================
*/
CR_API bool_t
tex_altern_y (
  __CR_IO__ sTEX_PATTERN*   patt
    )
{
    sRECT   clip;
    uchar*  line;
    sint_t  flag;
    byte_t  idx1;
    byte_t  idx2;
    sint_t  xx, yy;
    uint_t  n1, n2;

    /* 参数过滤 */
    if (patt->img->fmt != CR_INDEX8 || patt->step <= 0)
        return (FALSE);
    if (!clip_rect(&clip, patt->win, &patt->img->clip_win))
        return (FALSE);

    /* 最大也只有高度个结果 */
    patt->result = (byte_t*)mem_malloc(clip.hh);
    if (patt->result == NULL)
        return (FALSE);

    /* 开始统计形态的个数 */
    patt->nhit = patt->ntot = 0;
    if (!patt->altr)
    {
        /* 直接统计 */
        for (yy = clip.y1 + patt->step; yy < clip.y2; yy += patt->step) {
            n1 = n2 = 0;
            line = patt->img->data + yy * patt->img->bpl;
            for (xx = clip.x1; xx < clip.x2; xx++) {
                idx1 = line[xx + 0];
                idx2 = line[xx + 1];
                if (idx1 == patt->idx1 && idx2 == patt->idx2)
                    n1 += 1;
                else
                if (idx1 == patt->idx2 && idx2 == patt->idx1)
                    n2 += 1;
            }
            if (n1 >= patt->gmin && n2 >= patt->gmin &&
                n1 <= patt->gmax && n2 <= patt->gmax) {
                patt->nhit += 1;
                patt->result[patt->ntot] = TRUE;
            }
            else {
                patt->result[patt->ntot] = FALSE;
            }
            patt->ntot += 1;
        }
    }
    else
    {
        /* 交替统计 */
        flag = 1;
        for (yy = clip.y1 + patt->step; yy < clip.y2; yy += patt->step) {
            n1 = n2 = 0;
            line = patt->img->data + yy * patt->img->bpl;
            for (xx = clip.x1; xx < clip.x2; xx++) {
                idx1 = line[xx + 0];
                idx2 = line[xx + 1];
                if (flag > 0) {
                    if (idx1 == patt->idx1 && idx2 == patt->idx2) {
                        n1 += 1;
                        flag = -flag;
                    }
                }
                else {
                    if (idx1 == patt->idx2 && idx2 == patt->idx1) {
                        n2 += 1;
                        flag = -flag;
                    }
                }
            }
            if (n1 >= patt->gmin && n2 >= patt->gmin &&
                n1 <= patt->gmax && n2 <= patt->gmax) {
                patt->nhit += 1;
                patt->result[patt->ntot] = TRUE;
            }
            else {
                patt->result[patt->ntot] = FALSE;
            }
            patt->ntot += 1;
        }
    }
    return (TRUE);
}

/*
=======================================
    释放交替统计参数结果
=======================================
*/
CR_API void_t
tex_patt_free (
  __CR_IN__ sTEX_PATTERN*   patt
    )
{
    SAFE_FREE(patt->result);
}

/*
=======================================
    纹理网格压缩
=======================================
*/
CR_API sIMAGE*
tex_compress (
  __CR_IN__ const sIMAGE*   img,
  __CR_IN__ uint_t          tile_w,
  __CR_IN__ uint_t          tile_h,
  __CR_IN__ byte_t          gate
    )
{
    leng_t  idx;
    uint_t  xx, yy;
    uint_t  ww, hh;
    byte_t* pnt_freq;
    byte_t* img_line;
    sIMAGE* tex_freq;

    /* 参数过滤 */
    if (img->fmt != CR_INDEX8 ||
        tile_w <= 1 || tile_w >= img->position.ww ||
        tile_h <= 1 || tile_h >= img->position.hh ||
        tile_w * tile_h > 255)
        return (NULL);

    /* 生成一张新的灰度图 */
    ww = img->position.ww / tile_w;
    hh = img->position.hh / tile_h;
    tex_freq = image_new(0, 0, ww, hh, CR_INDEX8, FALSE, 4);
    if (tex_freq == NULL)
        return (NULL);
    ww *= tile_w;
    hh *= tile_h;
    mem_cpy(tex_freq->pal, img->pal, 1024);
    mem_zero(tex_freq->data, tex_freq->size);

    /* 开始统计点的频率 */
    img_line = img->data;
    for (yy = 0; yy < hh; yy++) {
        for (xx = 0; xx < ww; xx++) {
            if (img_line[xx] != 0x00) {
                pnt_freq = pixel_addr1(tex_freq, xx / tile_w, yy / tile_h);
                pnt_freq[0] += 1;
            }
        }
        img_line += img->bpl;
    }

    /* 是否进行二值化 */
    if (gate != 0) {
        for (idx = 0; idx < tex_freq->size; idx++) {
            if (tex_freq->data[idx] < gate)
                tex_freq->data[idx] = 0x00;
            else
                tex_freq->data[idx] = 0xFF;
        }
    }
    return (tex_freq);
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
