/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2013-05-16  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack 矩形几何运算函数库 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "msclib.h"
#include "phylib.h"

/*
=======================================
    矩形整理
=======================================
*/
CR_API leng_t
rect_clean (
  __CR_IO__ sRECT*  result,
  __CR_IN__ leng_t  count
    )
{
    leng_t  ii, jj, size;

    /* 删除尾部无效矩形 */
    for (; count != 0; count--) {
        if (result[count - 1].ww != 0)
            break;
    }

    /* 从头查找无效矩形 */
    for (ii = 0; ii < count; ii++)
    {
        /* 查找已删除矩形 */
        if (result[ii].ww != 0)
            continue;

        /* 查找下一个有效矩形 */
        for (jj = ii + 1; jj < count; jj++) {
            if (result[jj].ww != 0)
                break;
        }
        size = (count - jj) * sizeof(sRECT);
        mem_mov(&result[ii], &result[jj], size);
        count -= (jj - ii);
    }
    return (count);
}

/*
=======================================
    矩形合并
=======================================
*/
CR_API leng_t
rect_merge (
  __CR_IO__ sRECT*              result,
  __CR_IN__ leng_t              count,
  __CR_IN__ const sRECT_FILTER* param
    )
{
    sRECT   temp;
    sRECT*  dest;
    sRECT*  srce;
    fp32_t  scale;
    leng_t  area1;
    leng_t  area2;
    leng_t  ii, jj;

    /* 遍历矩形合并掉相交的矩形 */
    for (ii = 0; ii < count; ii++)
    {
        dest = &result[ii];
        if (dest->ww == 0)
            continue;
        for (jj = 0; jj < count; jj++)
        {
            /* 跳过自己和无效矩形 */
            if (jj == ii)
                continue;
            srce = &result[jj];
            if (srce->ww == 0)
                continue;

            /* 检测矩形是否相交 */
            if (!clip_rect(&temp, dest, srce))
                continue;
            area1  = dest->ww;
            area1 *= dest->hh;
            area2  = srce->ww;
            area2 *= srce->hh;
            area2 += area1;
            area1  =  temp.ww;
            area1 *=  temp.hh;
            area2 -= area1;

            /* 相交面积大于一定比例合并矩形 */
            if ((fp32_t)area1 < (fp32_t)area2 * param->merge)
                continue;
            temp.x1 = CR_MIN(dest->x1, srce->x1);
            temp.y1 = CR_MIN(dest->y1, srce->y1);
            temp.x2 = CR_MAX(dest->x2, srce->x2);
            temp.y2 = CR_MAX(dest->y2, srce->y2);

            /* 新矩形必须符合宽高约束条件 */
            temp.ww = temp.x2 - temp.x1 + 1;
            if (temp.ww < param->min_w || temp.ww > param->max_w)
                continue;
            temp.hh = temp.y2 - temp.y1 + 1;
            if (temp.hh < param->min_h || temp.hh > param->max_h)
                continue;
            scale  = (fp32_t)temp.ww;
            scale /= (fp32_t)temp.hh;
            if (scale < param->min_s || scale > param->max_s)
                continue;

            /* 置目标矩形并删除源矩形 */
            struct_cpy(dest, &temp, sRECT);
            srce->ww = 0;

            /* 重新开始搜索 */
            ii -= 1;
            break;
        }
    }
    return (rect_clean(result, count));
}

/*
=======================================
    矩形过滤 (左上 - 右下)
=======================================
*/
CR_API leng_t
rect_get_lt_rb (
  __CR_OT__ sRECT*              result,
  __CR_IN__ leng_t              limit,
  __CR_IN__ const sPNT2*        pnt_lt,
  __CR_IN__ leng_t              cnt_lt,
  __CR_IN__ const sPNT2*        pnt_rb,
  __CR_IN__ leng_t              cnt_rb,
  __CR_IN__ const sRECT_FILTER* param
    )
{
    fp32_t  scale;
    sint_t  x1, y1;
    sint_t  x2, y2;
    uint_t  ww, hh;
    leng_t  ii, jj;
    leng_t  count = 0;

    if (count >= limit)
        return (0);
    for (ii = 0; ii < cnt_lt; ii++)
    for (jj = 0; jj < cnt_rb; jj++)
    {
        /* 宽高条件过滤 */
        x1 = pnt_lt[ii].x;
        y1 = pnt_lt[ii].y;
        x2 = pnt_rb[jj].x;
        y2 = pnt_rb[jj].y;
        if (x1 >= x2 || y1 >= y2)
            continue;
        ww = x2 - x1 + 1;
        if (ww < param->min_w || ww > param->max_w)
            continue;
        hh = y2 - y1 + 1;
        if (hh < param->min_h || hh > param->max_h)
            continue;
        scale  = (fp32_t)ww;
        scale /= (fp32_t)hh;
        if (scale < param->min_s || scale > param->max_s)
            continue;
        result[count].x1 = x1;
        result[count].y1 = y1;
        result[count].x2 = x2;
        result[count].y2 = y2;
        result[count].ww = ww;
        result[count].hh = hh;
        if (++count >= limit)
            goto _find_out;
    }
_find_out:
    return (rect_merge(result, count, param));
}

/*
=======================================
    矩形过滤 (左下 - 右上)
=======================================
*/
CR_API leng_t
rect_get_lb_rt (
  __CR_OT__ sRECT*              result,
  __CR_IN__ leng_t              limit,
  __CR_IN__ const sPNT2*        pnt_lb,
  __CR_IN__ leng_t              cnt_lb,
  __CR_IN__ const sPNT2*        pnt_rt,
  __CR_IN__ leng_t              cnt_rt,
  __CR_IN__ const sRECT_FILTER* param
    )
{
    fp32_t  scale;
    sint_t  x1, y1;
    sint_t  x2, y2;
    uint_t  ww, hh;
    leng_t  ii, jj;
    leng_t  count = 0;

    if (count >= limit)
        return (0);
    for (ii = 0; ii < cnt_lb; ii++)
    for (jj = 0; jj < cnt_rt; jj++)
    {
        /* 宽高条件过滤 */
        x1 = pnt_lb[ii].x;
        y1 = pnt_rt[jj].y;
        x2 = pnt_rt[jj].x;
        y2 = pnt_lb[ii].y;
        if (x1 >= x2 || y1 >= y2)
            continue;
        ww = x2 - x1 + 1;
        if (ww < param->min_w || ww > param->max_w)
            continue;
        hh = y2 - y1 + 1;
        if (hh < param->min_h || hh > param->max_h)
            continue;
        scale  = (fp32_t)ww;
        scale /= (fp32_t)hh;
        if (scale < param->min_s || scale > param->max_s)
            continue;
        result[count].x1 = x1;
        result[count].y1 = y1;
        result[count].x2 = x2;
        result[count].y2 = y2;
        result[count].ww = ww;
        result[count].hh = hh;
        if (++count >= limit)
            goto _find_out;
    }
_find_out:
    return (rect_merge(result, count, param));
}

/*
=======================================
    取点密度最大矩形
=======================================
*/
CR_API bool_t
rect_max_density (
  __CR_OT__ sRECT*          result,
  __CR_IN__ const sRECT*    list,
  __CR_IN__ leng_t          count,
  __CR_IN__ const sPNT2*    pnts1,
  __CR_IN__ leng_t          cnts1,
  __CR_IN__ const sPNT2*    pnts2,
  __CR_IN__ leng_t          cnts2
    )
{
    double  tmp, density = -2.0;
    leng_t  area, ii, jj, tt, kk = 0;

    for (ii = 0; ii < count; ii++) {
        area  = list[ii].ww;
        area *= list[ii].hh;
        area *= area;   /* 加强面积的影响力 */
        for (tt = 0, jj = 0; jj < cnts1; jj++) {
            if (clip_pixel(pnts1[jj].x, pnts1[jj].y, &list[ii]))
                tt++;
        }
        for (jj = 0; jj < cnts2; jj++) {
            if (clip_pixel(pnts2[jj].x, pnts2[jj].y, &list[ii]))
                tt++;
        }
        tmp  = (double)tt;
        tmp /= (double)area;
        if (tmp > density) {
            density = tmp;
            kk = ii;
        }
    }
    if (density < 0.0)
        return (FALSE);
    struct_cpy(result, &list[kk], sRECT);
    return (TRUE);
}

/*
=======================================
    计算点集的重心
=======================================
*/
CR_API leng_t
rect_get_focus (
  __CR_OT__ sPNT2*          pnt,
  __CR_IN__ const sIMAGE*   img,
  __CR_IN__ const sRECT*    roi,
  __CR_IN__ bool_t          gray
    )
{
    sRECT   clip;
    sFILL   fill;
    sBLTer  oper;
    leng_t  count;
    uint_t  xx, yy;
    int64u  sx, sy;
    int64u  temp, cnts;

    /* 参数过滤 */
    if (img->fmt != CR_INDEX8)
        return (0);
    if (roi == NULL) {
        roi = &img->clip_win;
    }
    else {
        if (!clip_rect(&clip, roi, &img->clip_win))
            return (0);
        roi = &clip;
    }

    /* 计算 ROI 结构 */
    fill.dx = roi->x1;
    fill.dy = roi->y1;
    fill.dw = roi->ww;
    fill.dh = roi->hh;
    blt_use_fill(&oper, img, 1, &fill);

    /* 支持以灰度为权重 */
    count = 0;
    sx = sy = cnts = 0;
    if (gray) {
        for (yy = 0; yy < fill.dh; yy++) {
            for (xx = 0; xx < fill.dw; xx++) {
                if (oper.addr[0] != 0x00) {
                    temp = oper.addr[0];
                    cnts += temp;
                    sx += xx * temp;
                    sy += yy * temp;
                    count++;
                }
                oper.addr += 1;
            }
            oper.addr += oper.rest;
        }
    }
    else {
        for (yy = 0; yy < fill.dh; yy++) {
            for (xx = 0; xx < fill.dw; xx++) {
                if (oper.addr[0] != 0x00) {
                    sx += xx;
                    sy += yy;
                    count++;
                }
                oper.addr += 1;
            }
            oper.addr += oper.rest;
        }
        cnts = count;
    }

    /* 计算重心坐标 */
    if (count != 0) {
        pnt->x = fill.dx + (sint_t)(sx / cnts);
        pnt->y = fill.dy + (sint_t)(sy / cnts);
    }
    return (count);
}

/*
---------------------------------------
    十字泛滥填充
---------------------------------------
*/
static void_t
rect_flood (
  __CR_IO__ byte_t* pixel,
  __CR_IN__ leng_t  bpl,
  __CR_IN__ uint_t  cx,
  __CR_IN__ uint_t  cy,
  __CR_IN__ uint_t  width,
  __CR_IN__ uint_t  height,
  __CR_IN__ byte_t  type
    )
{
    *pixel = type;
    if (cx != 0 && *(pixel - 1) == 0xFF)
        rect_flood(pixel - 1, bpl, cx - 1, cy, width, height, type);
    if (cx != width && *(pixel + 1) == 0xFF)
        rect_flood(pixel + 1, bpl, cx + 1, cy, width, height, type);
    if (cy != 0 && *(pixel - bpl) == 0xFF)
        rect_flood(pixel - bpl, bpl, cx, cy - 1, width, height, type);
    if (cy != height && *(pixel + bpl) == 0xFF)
        rect_flood(pixel + bpl, bpl, cx, cy + 1, width, height, type);
}

/*
=======================================
    连通区域的标识
=======================================
*/
CR_API byte_t
rect_area_marks (
  __CR_IO__ const sIMAGE*   img
    )
{
    byte_t  type, *line;
    uint_t  xx, yy, ww, hh;

    /* 参数过滤 */
    if (img->fmt != CR_INDEX8)
        return (0x00);
    type = 0x01;
    line = img->data;
    ww = img->position.ww;
    hh = img->position.hh;

    /* 使用泛滥填充标识连通区域 */
    for (yy = 0; yy < hh; yy++) {
        for (xx = 0; xx < ww; xx++) {
            if (line[xx] == 0xFF) {
                rect_flood(&line[xx], img->bpl, xx, yy,
                            ww - 1, hh - 1, type);
                if (type == 0xFE)
                    return (type);
                type += 1;
            }
        }
        line += img->bpl;
    }
    return (type - 1);
}

/*
=======================================
    连通区域的提取
=======================================
*/
CR_API bool_t
rect_area_bound (
  __CR_OT__ sRECT*          bound,
  __CR_IN__ const sIMAGE*   img,
  __CR_IN__ byte_t          type
    )
{
    byte_t  *line;
    sint_t  min_x, min_y;
    sint_t  max_x, max_y;
    sint_t  xx, yy, ww, hh;

    /* 参数过滤 */
    if (img->fmt != CR_INDEX8 ||
        type == 0x00 || type == 0xFF)
        return (FALSE);
    ww = img->position.ww;
    hh = img->position.hh;

    min_x = ww;
    min_y = hh;
    max_x = -1;
    max_y = -1;

    /* 搜索最小最大坐标 */
    line = img->data;
    for (yy = 0; yy < hh; yy++) {
        for (xx = 0; xx < ww; xx++) {
            if (line[xx] == type) {
                if (xx < min_x) min_x = xx;
                if (xx > max_x) max_x = xx;
                if (yy < min_y) min_y = yy;
                if (yy > max_y) max_y = yy;
            }
        }
        line += img->bpl;
    }

    /* 合成边界矩形 */
    if (max_x < min_x || max_y < min_y)
        return (FALSE);
    rect_set_xy(bound, min_x, min_y, max_x, max_y);
    return (TRUE);
}

/*
---------------------------------------
    置信度排序比较
---------------------------------------
*/
static sint_t
prob_comp (
  __CR_IN__ const void_t*   elem1,
  __CR_IN__ const void_t*   elem2
    )
{
    fp32_t  val1 = ((sRECT_OBJECT*)elem1)->prob;
    fp32_t  val2 = ((sRECT_OBJECT*)elem2)->prob;

    /* 降序 */
    if (val1 < val2) return ( 1);
    if (val1 > val2) return (-1);
    return (0);
}


/*
=======================================
    矩形目标 NMS 操作
=======================================
*/
CR_API leng_t*
rect_sort_nms (
  __CR_IO__ sRECT_OBJECT*   objects,
  __CR_IN__ leng_t          count,
  __CR_IN__ bool_t          agnostic,
  __CR_OT__ leng_t*         selected,
  __CR_IN__ fp32_t          gate
    )
{
    sRECT   temp;
    bool_t  keep;
    leng_t  ii, jj, sz, *pick;
    fp32_t  intr, unon, *area;
    /* -------------------- */
    sRECT_OBJECT    *aa, *bb;

    /* 分配选择列表和面积缓存 */
    pick = mem_talloc(count, leng_t);
    if (pick == NULL)
        return (NULL);
    area = mem_talloc(count, fp32_t);
    if (area == NULL) {
        mem_free(pick);
        return (NULL);
    }

    /* 根据置信度降序排序 */
    quick_sort(objects, count, sizeof(sRECT_OBJECT), prob_comp);

    /* 先计算好所有的面积 */
    for (ii = 0; ii < count; ii++) {
        area[ii]  = (fp32_t)(objects[ii].rect.ww);
        area[ii] *= (fp32_t)(objects[ii].rect.hh);
    }

    /* 执行 NMS 操作 */
    for (sz = ii = 0; ii < count; ii++) {
        aa = &objects[ii];
        keep = TRUE;
        for (jj = 0; jj < sz; jj++) {
            bb = &objects[pick[jj]];
            if (!agnostic && aa->type != bb->type)
                continue;
            if (!clip_rect(&temp, &aa->rect, &bb->rect))
                continue;
            intr = ((fp32_t)temp.ww) * temp.hh;
            unon = area[ii] + area[pick[jj]] - intr;
            if (intr / unon > gate) {
                keep = FALSE;
                break;
            }
        }
        if (keep)
            pick[sz++] = ii;
    }
    mem_free(area);
    if (selected != NULL)
        *selected = sz;
    if (sz < count)
        pick[sz] = count;
    return (pick);
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
