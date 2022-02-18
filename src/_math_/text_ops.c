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

#include "msclib.h"
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

/*
=======================================
    多维直方图初始化
=======================================
*/
CR_API bool_t
histo_xd_init (
  __CR_IO__ sHISTO_XD*  hist
    )
{
    uint_t  idx, bpl;

    /* 参数校验 */
    if (hist->dim == 0)
        return (FALSE);
    if (hist->sze != 1 && hist->sze != 2 && hist->sze != 4)
        return (FALSE);
    hist->len = 1;
    for (idx = 0; idx < hist->dim; idx++)
    {
        /* 参数校验 */
        if (hist->dwn[idx] == 0)
            return (FALSE);
        if (hist->min[idx] >= hist->max[idx])
            return (FALSE);

        /* 计算参数 */
        bpl = (uint_t)(hist->max[idx] - hist->min[idx]);
        hist->bpd[idx] = (leng_t)(bpl / hist->dwn[idx]);
        if (bpl % hist->dwn[idx] != 0)
            hist->bpd[idx] += 1;
        hist->len *= hist->bpd[idx];
        if (idx == 0)
            hist->stt[idx] = 1;
        else
            hist->stt[idx] = hist->stt[idx - 1] * hist->bpd[idx - 1];
    }
    hist->map = mem_calloc(hist->len, hist->sze);
    if (hist->map == NULL)
        return (FALSE);
    mem_zero(hist->map, hist->len * hist->sze);
    return (TRUE);
}

/*
=======================================
    多维直方图释放
=======================================
*/
CR_API void_t
histo_xd_free (
  __CR_IO__ sHISTO_XD*  hist
    )
{
    SAFE_FREE(hist->map);
}

/*
=======================================
    多维直方图复位
=======================================
*/
CR_API void_t
histo_xd_reset (
  __CR_IO__ sHISTO_XD*  hist
    )
{
    mem_zero(hist->map, hist->len * hist->sze);
}

/*
=======================================
    多维直方图返回索引
=======================================
*/
CR_API bool_t
histo_xd_index (
  __CR_IN__ sHISTO_XD*      hist,
  __CR_OT__ leng_t*         index,
  __CR_IN__ const sint_t*   value
    )
{
    uint_t  idx, cnt;

    index[0] = 0;
    for (idx = 0; idx < hist->dim; idx++)
    {
        /* 判断范围 */
        if (value[idx] <  hist->min[idx] ||
            value[idx] >= hist->max[idx])
            return (FALSE);

        /* 降采样后计算偏移 */
        cnt = (uint_t)(value[idx] - hist->min[idx]);
        cnt /= hist->dwn[idx];
        if (cnt >= hist->bpd[idx])
            return (FALSE);
        index[0] += cnt * hist->stt[idx];
    }
    return (TRUE);
}

/*
=======================================
    多维直方图计数 (通过索引)
=======================================
*/
CR_API void_t
histo_xd_inc (
  __CR_IN__ sHISTO_XD*  hist,
  __CR_IN__ leng_t      index
    )
{
    leng_t  idx;
    byte_t* ptr1;
    int16u* ptr2;
    int32u* ptr4;

    switch (hist->sze)
    {
        default:
            break;

        case 1:
            ptr1 = (byte_t*)hist->map;
            if (ptr1[index] != 0xFF) {
                ptr1[index] += 1;
            }
            else {
                for (idx = 0; idx < hist->len; idx++) {
                    if (idx == index)
                        continue;
                    if (ptr1[idx] != 0)
                        ptr1[idx] -= 1;
                }
            }
            break;

        case 2:
            ptr2 = (int16u*)hist->map;
            if (ptr2[index] != 0xFFFF) {
                ptr2[index] += 1;
            }
            else {
                for (idx = 0; idx < hist->len; idx++) {
                    if (idx == index)
                        continue;
                    if (ptr2[idx] != 0)
                        ptr2[idx] -= 1;
                }
            }
            break;

        case 4:
            ptr4 = (int32u*)hist->map;
            if (ptr4[index] != 0xFFFFFFFFUL) {
                ptr4[index] += 1;
            }
            else {
                for (idx = 0; idx < hist->len; idx++) {
                    if (idx == index)
                        continue;
                    if (ptr4[idx] != 0)
                        ptr4[idx] -= 1;
                }
            }
            break;
    }
}

/*
=======================================
    多维直方图计数 (通过坐标)
=======================================
*/
CR_API bool_t
histo_xd_inc_ex (
  __CR_IN__ sHISTO_XD*      hist,
  __CR_IN__ const sint_t*   value
    )
{
    leng_t  index;

    if (!histo_xd_index(hist, &index, value))
        return (FALSE);
    histo_xd_inc(hist, index);
    return (TRUE);
}

/*
=======================================
    多维直方图反算坐标
=======================================
*/
CR_API bool_t
histo_xd_get_back (
  __CR_IN__ sHISTO_XD*  hist,
  __CR_OT__ sint_t*     value,
  __CR_IN__ leng_t      index
    )
{
    uint_t  ii, jj, tmp;

    if (index >= hist->len)
        return (FALSE);
    for (ii = hist->dim; ii != 0; ii--) {
        jj = ii - 1;
        tmp = (uint_t)(index / hist->stt[jj]);
        index %= hist->stt[jj];
        value[jj] = tmp * hist->dwn[jj] + hist->dwn[jj] / 2;
        value[jj] += hist->min[jj];
    }
    return (TRUE);
}

/*
---------------------------------------
    节点降序排序
---------------------------------------
*/
static sint_t
histo_comp (
  __CR_IN__ const void_t*   elem1,
  __CR_IN__ const void_t*   elem2
    )
{
    sHISTO_XD_NODE* val1 = (sHISTO_XD_NODE*)elem1;
    sHISTO_XD_NODE* val2 = (sHISTO_XD_NODE*)elem2;

    /* 降序 */
    if (val1->freq < val2->freq) return ( 1);
    if (val1->freq > val2->freq) return (-1);
    return (0);
}

/*
=======================================
    多维直方图降序排序
=======================================
*/
CR_API sHISTO_XD_NODE*
histo_xd_sort (
  __CR_IN__ sHISTO_XD*  hist,
  __CR_OT__ leng_t*     size
    )
{
    leng_t          idx;
    leng_t          len;
    byte_t*         ptr1;
    int16u*         ptr2;
    int32u*         ptr4;
    sHISTO_XD_NODE* list;

    *size = 0;

    switch (hist->sze)
    {
        default:
            return (NULL);

        case 1:
            ptr1 = (byte_t*)hist->map;
            for (len = idx = 0; idx < hist->len; idx++) {
                if (ptr1[idx] != 0)
                    len++;
            }
            if (len == 0) return (NULL);
            list = mem_talloc(len, sHISTO_XD_NODE);
            if (list == NULL)
                return (NULL);
            for (len = idx = 0; idx < hist->len; idx++) {
                if (ptr1[idx] != 0) {
                    list[len].freq = ptr1[idx];
                    list[len++].index = idx;
                }
            }
            break;

        case 2:
            ptr2 = (int16u*)hist->map;
            for (len = idx = 0; idx < hist->len; idx++) {
                if (ptr2[idx] != 0)
                    len++;
            }
            if (len == 0) return (NULL);
            list = mem_talloc(len, sHISTO_XD_NODE);
            if (list == NULL)
                return (NULL);
            for (len = idx = 0; idx < hist->len; idx++) {
                if (ptr2[idx] != 0) {
                    list[len].freq = ptr2[idx];
                    list[len++].index = idx;
                }
            }
            break;

        case 4:
            ptr4 = (int32u*)hist->map;
            for (len = idx = 0; idx < hist->len; idx++) {
                if (ptr4[idx] != 0)
                    len++;
            }
            if (len == 0) return (NULL);
            list = mem_talloc(len, sHISTO_XD_NODE);
            if (list == NULL)
                return (NULL);
            for (len = idx = 0; idx < hist->len; idx++) {
                if (ptr4[idx] != 0) {
                    list[len].freq = ptr4[idx];
                    list[len++].index = idx;
                }
            }
            break;
    }
    *size = len;
    quick_sort(list, len, sizeof(sHISTO_XD_NODE), histo_comp);
    return (list);
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
