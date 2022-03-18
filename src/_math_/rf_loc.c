/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2018-12-04  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>>>>> 无线定位函数库 <<<<<<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "msclib.h"
#include "phylib.h"

#ifndef _CR_NO_STDC_
    #include <math.h>
#endif

/*
=======================================
    LQI 到 RSSI 的换算
=======================================
*/
CR_API fp32_t
rfloc_lqi2rssi (
  __CR_IN__ fp32_t  lqi
    )
{
    return (-(81 - (lqi * 91) / 255));
}

/*
=======================================
    RSSI 到距离的换算
=======================================
*/
CR_API fp32_t
rfloc_rssi2meter (
  __CR_IN__ fp32_t  rssi,
  __CR_IN__ fp32_t  a,
  __CR_IN__ fp32_t  n
    )
{
    fp32_t  pw;

    pw = (-rssi - a) / (10 * n);
    return (FPOW(10, pw));
}

/* 接近0的数 */
#define CR_CIRCLE_ABIT  0.001f

/*
=======================================
    计算两个圆的交点 (外接圆)
=======================================
*/
CR_API bool_t
circle_intersect (
  __CR_OT__ vec2d_t*        pnts,
  __CR_IN__ const vec3d_t*  circle1,
  __CR_IN__ const vec3d_t*  circle2
    )
{
    fp32_t  x1, y1, r1;
    fp32_t  x2, y2, r2;
    fp32_t  xc, yc, xd, yd;
    fp32_t  ae, ef, x0, y0;
    fp32_t  ll, k2, dx, dy;

    /* 准备计算参数 */
    x1 = circle1->x;
    y1 = circle1->y;
    r1 = circle1->z;
    x2 = circle2->x;
    y2 = circle2->y;
    r2 = circle2->z;

    /* 判断圆是否相交 */
    dx = x2 - x1;
    dy = y2 - y1;
    ll = FSQRT(dx * dx + dy * dy);
    if (ll <= r1 || ll <= r2)
        return (FALSE);
    if (ll >= r1 + r2 || ll <= FABS(r1 - r2))
        return (FALSE);

    /* 计算必要参数 */
    ae = r1 * r1 - r2 * r2 + ll * ll;
    ae /= 2 * ll;
    ef = ae / ll;
    x0 = x1 + ef * dx;
    y0 = y1 + ef * dy;
    if (FABS(dy) < CR_CIRCLE_ABIT) {
        xc = xd = x0;
        ef = FSQRT(r1 * r1 - ae * ae);
        yc = y0 - ef;
        yd = y0 + ef;
    }
    else {
        k2 = -dx / dy;
        ef = FSQRT((r1 * r1 - ae * ae) / (1 + k2 * k2));
        xc = x0 - ef;
        xd = x0 + ef;
        yc = y0 + k2 * (xc - x0);
        yd = y0 + k2 * (xd - x0);
    }

    /* 返回结果 */
    pnts[0].x = xc;
    pnts[0].y = yc;
    pnts[1].x = xd;
    pnts[1].y = yd;
    return (TRUE);
}

/*
=======================================
    RSSI 定位初始化
=======================================
*/
CR_API bool_t
rfloc_init (
  __CR_IO__ sRF_LOC*    param,
  __CR_IN__ leng_t      max_count
    )
{
    leng_t  mm, mn, kk, nn;

    /* 参数校验 */
    if (param->num < 4)
        return (FALSE);

    /* 使用最大信标个数 */
    param->buffer = mem_talloc(max_count, sRF_LOC_BKEN);
    if (param->buffer == NULL)
        return (FALSE);
    param->max_sz = max_count;

    /* 计算最大的组合个数 */
    nn = param->num;
    for (mm = 1, kk = 2; kk <= nn; kk++)
        mm *= kk;
    nn -= 3;
    for (mn = 1, kk = 2; kk <= nn; kk++)
        mn *= kk;
    kk = mm / (3 * 2 * mn) + 1;
    param->list = mem_talloc(kk, vec3d_t);
    if (param->list == NULL) {
        mem_free(param->buffer);
        return (FALSE);
    }
    param->size = 0;
    return (TRUE);
}

/*
=======================================
    RSSI 定位释放资源
=======================================
*/
CR_API void_t
rfloc_free (
  __CR_IO__ sRF_LOC*    param
    )
{
    param->size = 0;
    param->max_sz = 0;
    SAFE_FREE(param->list);
    SAFE_FREE(param->buffer);
}

/*
---------------------------------------
    RSSI 排序比较
---------------------------------------
*/
static sint_t
rssi_comp (
  __CR_IN__ const void_t*   elem1,
  __CR_IN__ const void_t*   elem2
    )
{
    sRF_LOC_BKEN*   node1 = (sRF_LOC_BKEN*)elem1;
    sRF_LOC_BKEN*   node2 = (sRF_LOC_BKEN*)elem2;

    /* 降序 */
    if (node1->rssi < node2->rssi)
        return (1);
    if (node1->rssi > node2->rssi)
        return (-1);
    return (0);
}

/*
---------------------------------------
    根据误差的权重合成
---------------------------------------
*/
static void_t
rfloc_make_by_error (
  __CR_IO__ sRF_LOC*    param,
  __CR_IO__ vec3d_t*    points,
  __CR_IN__ uint_t      count
    )
{
    uint_t  idx;
    fp32_t  total;
    vec2d_t result;

    /* 只有一个直接返回 */
    if (count == 1) {
        param->list[param->size].x = points[0].x;
        param->list[param->size].y = points[0].y;
        param->size += 1;
        return;
    }
    total = 0.0f;
    for (idx = 0; idx < count; idx++)
    {
        /* 误差足够小直接返回 */
        if (points[idx].z <= param->sml) {
            param->list[param->size].x = points[idx].x;
            param->list[param->size].y = points[idx].y;
            param->size += 1;
            return;
        }
        points[idx].z = 1.0f / points[idx].z;
        total += points[idx].z;
    }
    result.x = result.y = 0.0f;
    for (idx = 0; idx < count; idx++) {
        result.x += points[idx].x * points[idx].z;
        result.y += points[idx].y * points[idx].z;
    }
    total = 1.0f / total;
    param->list[param->size].x = result.x * total;
    param->list[param->size].y = result.y * total;
    param->size += 1;
}

/*
---------------------------------------
    估算三个圆的交点 (外接圆)
---------------------------------------
*/
static uint_t
rfloc_3_circles (
  __CR_OT__ vec3d_t*        back,
  __CR_IN__ const vec3d_t*  circle1,
  __CR_IN__ const vec3d_t*  circle2,
  __CR_IN__ const vec3d_t*  circle3,
  __CR_IN__ fp32_t          max_error
    )
{
    fp32_t  total;
    fp32_t  x0, y0, ll;
    fp32_t  x1, y1, r1;
    fp32_t  x2, y2, r2;
    fp32_t  x3, y3, r3;
    fp32_t  r12, delta;

    /* 准备计算参数 */
    x1 = circle1->x;
    y1 = circle1->y;
    r1 = circle1->z;
    x2 = circle2->x;
    y2 = circle2->y;
    r2 = circle2->z;
    x3 = circle3->x;
    y3 = circle3->y;
    r3 = circle3->z;
    r12 = r1 + r2;

    /* 计算出两圆的误差 */
    x2 -= x1;   y2 -= y1;
    delta = FSQRT(x2 * x2 + y2 * y2);
    if (delta <= r1 || delta <= r2 || delta <= FABS(r1 - r2))
        return (0);
    delta -= r12;
    ll = FABS(delta);
    if (ll >= max_error)
        return (0);
    total = ll;

    /* 分摊误差 */
    ll = delta * r1 / r12;

    /* 假设两圆相切, 计算出切点 */
    delta = (r1 + ll) / (r12 + delta);
    x0 = x1 + x2 * delta;
    y0 = y1 + y2 * delta;

    /* 计算这个切点与第三圆的距离误差 */
    x3 -= x0;   y3 -= y0;
    delta = FSQRT(x3 * x3 + y3 * y3) - r3;
    ll = FABS(delta);
    if (ll >= max_error)
        return (0);
    total += ll;

    /* 返回估算点 */
    back->x = x0;
    back->y = y0;
    back->z = total;
    return (1);
}

/*
---------------------------------------
    三圆定位内部计算
---------------------------------------
*/
static uint_t
rfloc_seek_int (
  __CR_OT__ vec3d_t*        back,
  __CR_IN__ const sRF_LOC*  param,
  __CR_IN__ const vec3d_t*  circle1,
  __CR_IN__ const vec3d_t*  circle2,
  __CR_IN__ const vec3d_t*  circle3
    )
{
    uint_t  idx;
    fp32_t  ll[2];
    fp32_t  dx, dy;
    vec2d_t pnts[2];

    /* 计算出2个交点 */
    if (circle_intersect(pnts, circle1, circle2)) {
        for (idx = 0; idx < 2; idx++) {
            dx = pnts[idx].x - circle3->x;
            dy = pnts[idx].y - circle3->y;
            ll[idx] = FSQRT(dx * dx + dy * dy);
            ll[idx] = FABS(ll[idx] - circle3->z);
        }

        /* 选择可能性大的那个交点 */
        idx = (ll[0] <= ll[1]) ? 0 : 1;
        if (ll[idx] < param->err) {
            back->z = ll[idx];
            back->x = pnts[idx].x;
            back->y = pnts[idx].y;
            return (1);
        }

        /* 不符合误差要求 */
        return (0);
    }

    /* 无交点使用估算交点的方式 */
    return (rfloc_3_circles(back, circle1, circle2, circle3, param->err));
}

/*
---------------------------------------
    三圆定位算法
---------------------------------------
*/
static bool_t
rfloc_seek (
  __CR_IO__ sRF_LOC*        param,
  __CR_IN__ const vec3d_t*  circle1,
  __CR_IN__ const vec3d_t*  circle2,
  __CR_IN__ const vec3d_t*  circle3
    )
{
    uint_t  cnt = 0;
    vec3d_t back[3];

    /* 选出最多3个可能的点 */
    cnt += rfloc_seek_int(&back[cnt], param, circle1, circle2, circle3);
    cnt += rfloc_seek_int(&back[cnt], param, circle1, circle3, circle2);
    cnt += rfloc_seek_int(&back[cnt], param, circle2, circle3, circle1);
    if (cnt == 0)
        return (FALSE);

    /* 误差的倒数作为权重 */
    rfloc_make_by_error(param, back, cnt);
    return (TRUE);
}

/*
---------------------------------------
    选3个圆计算定位
---------------------------------------
*/
static void_t
rfloc_combine (
  __CR_IO__ sRF_LOC*    param,
  __CR_IN__ leng_t      total,
  __CR_IN__ leng_t      index,
  __CR_IO__ leng_t*     select
    )
{
    /* 选出了3个圆则执行计算 */
    if (select[3] == 3) {
        rfloc_seek(param, (vec3d_t*)(&param->buffer[select[0]]),
                          (vec3d_t*)(&param->buffer[select[1]]),
                          (vec3d_t*)(&param->buffer[select[2]]));
        return;
    }

    /* 没有圆可取了 */
    if (index >= total)
        return;

    /* 递归组合算法 */
    select[select[3]] = index;
    select[3] += 1;
    rfloc_combine(param, total, index + 1, select);
    select[3] -= 1;
    rfloc_combine(param, total, index + 1, select);
}

/*
=======================================
    RSSI 定位算法
=======================================
*/
CR_API bool_t
rfloc_doit (
  __CR_OT__ vec2d_t*            pos,
  __CR_IN__ const sRF_LOC_BKEN* beacon,
  __CR_IN__ leng_t              count,
  __CR_IN__ bool_t              direct,
  __CR_IO__ sRF_LOC*            param
    )
{
    fp32_t  dd, zz;
    fp32_t  dx, dy;
    leng_t  idx, num;
    leng_t  select[4];

    /* 参数过滤 */
    if (count < param->num)
        return (FALSE);
    if (count > param->max_sz)
        count = param->max_sz;

    /* 统计符合要求的信标 */
    if (direct)
    {
        /* 直接模式 RSSI 值即为距离, 负值存放 */
        for (num = idx = 0; idx < count; idx++) {
            if (beacon[idx].rssi >= 0 && beacon[idx].rssi > param->min) {
                struct_cpy(&param->buffer[num], &beacon[idx], sRF_LOC_BKEN);
                param->buffer[num].rssi = -param->buffer[num].rssi;
                num += 1;
            }
        }
    }
    else
    {
        /* 间接模式 RSSI 值需转换成距离 */
        for (num = idx = 0; idx < count; idx++) {
            if (beacon[idx].rssi < 0 && beacon[idx].rssi > param->min) {
                struct_cpy(&param->buffer[num], &beacon[idx], sRF_LOC_BKEN);
                if (param->buffer[num].a <= 0)
                    param->buffer[num].a = param->a;
                num += 1;
            }
        }
    }
    if (num < param->num)
        return (FALSE);

    /* 根据 RSSI 降序排序 (间接模式) or 距离升序排序 (直接模式) */
    quick_sort(param->buffer, num, sizeof(sRF_LOC_BKEN), rssi_comp);

    /* 计算实际的距离 */
    for (idx = 0; idx < param->num; idx++)
    {
        /* 三种计算模式 */
        if (direct) {
            dd = -param->buffer[idx].rssi;
        }
        else
        if (param->rssi2meter != NULL) {
            dd = param->rssi2meter(&param->buffer[idx], param);
        }
        else {
            dd = rfloc_rssi2meter(param->buffer[idx].rssi,
                        param->buffer[idx].a, param->n);
        }

        /* 计算垂直距离 */
        zz = param->buffer[idx].z;
        if (dd <= zz)
        {
            /* 直接认为就在这里 */
            pos->x = param->buffer[idx].x;
            pos->y = param->buffer[idx].y;
            return (TRUE);
        }
        param->buffer[idx].d = dd;
        param->buffer[idx].r = FSQRT(dd * dd - zz * zz);
        if (param->buffer[idx].r <= param->sml)
        {
            /* 直接认为就在这里 */
            pos->x = param->buffer[idx].x;
            pos->y = param->buffer[idx].y;
            return (TRUE);
        }
    }

    /* 执行3个圆的组合定位 */
    param->size = select[3] = 0;
    rfloc_combine(param, param->num, 0, select);

    /* 无法定位 */
    if (param->size == 0)
        return (FALSE);

    /* 只有一个点直接返回 */
    if (param->size == 1) {
        pos->x = param->list[0].x;
        pos->y = param->list[0].y;
        return (TRUE);
    }

    /* 根据得到的点按距离权重合成 */
    for (zz = 0.0f, num = 0; num < param->size; num++) {
        for (dd = 0.0f, idx = 0; idx < param->num; idx++) {
            dx = param->list[num].x - param->buffer[idx].x;
            dy = param->list[num].y - param->buffer[idx].y;
            dd += FSQRT(dx * dx + dy * dy);
        }
        param->list[num].z = 1.0f / dd;
        zz += param->list[num].z;
    }
    pos->x = pos->y = 0.0f;
    for (idx = 0; idx < param->size; idx++) {
        pos->x += param->list[idx].x * param->list[idx].z;
        pos->y += param->list[idx].y * param->list[idx].z;
    }
    zz = 1.0f / zz;
    pos->x *= zz;
    pos->y *= zz;
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
