/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2019-11-11  */
/*     #######          ###    ###      [CORE]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack DMC 压缩解压函数库 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "enclib.h"
#include "memlib.h"

/* 标准 DMC 常数 */
#ifndef DMC_MEMSIZE
    #define DMC_MEMSIZE     0x1000000UL
    #define DMC_BIGTHRESH       2
    #define DMC_THRESHOLD       2
#endif

/* DMC 内部结构 */
typedef struct  _sDMC_NODE
{
        fp32_t              count[2];
        struct _sDMC_NODE   *next[2];

} sDMC_NODE;

typedef struct
{
        sDMC_NODE   *p;
        sDMC_NODE   *newn;
        sDMC_NODE   nodes[256][256];

        sDMC_NODE   *nodebuf;
        sDMC_NODE   *nodemaxp;
        sDMC_NODE   *nodesptr;
} sDMC;

/*
---------------------------------------
    DMC 复位
---------------------------------------
*/
static void_t
dmc_reset (
  __CR_IO__ sDMC*   handle
    )
{
    handle->p = &handle->nodes[0][0];
}

/*
---------------------------------------
    DMC 输出刷新
---------------------------------------
*/
static void_t
dmc_flush (
  __CR_IO__ sDMC*   handle
    )
{
    uint_t  ii, jj;

    for (jj = 0; jj < 256; jj++) {
        for (ii = 0; ii < 127; ii++) {
            handle->nodes[jj][ii].count[0] = 0.2f;
            handle->nodes[jj][ii].count[1] = 0.2f;
            handle->nodes[jj][ii].next[0] = &handle->nodes[jj][2 * ii + 1];
            handle->nodes[jj][ii].next[1] = &handle->nodes[jj][2 * ii + 2];
        }
        for (; ii < 255; ii++) {
            handle->nodes[jj][ii].count[0] = 0.2f;
            handle->nodes[jj][ii].count[1] = 0.2f;
            handle->nodes[jj][ii].next[0] = &handle->nodes[ii + 1][0];
            handle->nodes[jj][ii].next[1] = &handle->nodes[ii - 127][0];
        }
    }
    handle->nodesptr = handle->nodebuf;
    dmc_reset(handle);
}

/*
---------------------------------------
    DMC 初始化
---------------------------------------
*/
static bool_t
dmc_init (
  __CR_OT__ sDMC*   handle
    )
{
    handle->nodebuf = (sDMC_NODE*)mem_malloc(DMC_MEMSIZE);
    if (handle->nodebuf == NULL)
        return (FALSE);
    handle->nodemaxp  = handle->nodebuf;
    handle->nodemaxp += DMC_MEMSIZE / sizeof(sDMC_NODE);
    handle->nodemaxp -= 20;
    dmc_flush(handle);
    return (TRUE);
}

/*
---------------------------------------
    DMC 释放
---------------------------------------
*/
static void_t
dmc_free (
  __CR_IN__ const sDMC* handle
    )
{
    mem_free(handle->nodebuf);
}

/*
---------------------------------------
    DMC 预测
---------------------------------------
*/
static fp32_t
dmc_predict (
  __CR_IO__ sDMC*   handle
    )
{
    return (handle->p->count[0] / (handle->p->count[0] + handle->p->count[1]));
}

/*
---------------------------------------
    DMC 更新
---------------------------------------
*/
static void_t
dmc_update (
  __CR_IO__ sDMC*   handle,
  __CR_IN__ uint_t  b
    )
{
    fp32_t  rr;

    if (handle->p->count[b] >= DMC_THRESHOLD &&
        handle->p->next[b]->count[0] + handle->p->next[b]->count[1] >=
        DMC_BIGTHRESH + handle->p->count[b]) {
        handle->newn = handle->nodesptr++;
        handle->p->next[b]->count[0] -= handle->newn->count[0] =
                handle->p->next[b]->count[0] * (rr = handle->p->count[b] /
               (handle->p->next[b]->count[1] + handle->p->next[b]->count[0]));
        handle->p->next[b]->count[1] -= handle->newn->count[1] =
                handle->p->next[b]->count[1] * rr;
        handle->newn->next[0] = handle->p->next[b]->next[0];
        handle->newn->next[1] = handle->p->next[b]->next[1];
        handle->p->next[b] = handle->newn;
    }
    handle->p->count[b]++;
    handle->p = handle->p->next[b];
    if (handle->nodesptr > handle->nodemaxp)
        dmc_flush(handle);
}

/*
=======================================
    DMC 压缩
=======================================
*/
CR_API leng_t
compr_dmc (
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen
    )
{
    uint_t  idx;
    leng_t  pot;
    uchar*  i_buf;
    uchar*  o_buf;
    leng_t  i_ptr;
    leng_t  o_ptr;
    /* -------- */
    sDMC*   handle;
    byte_t  cha, bit;
    int32u  max, min, mid;

    if (dst == NULL)
        return (srclen * 2 + 3);

    handle = struct_new(sDMC);
    if (handle == NULL)
        return (0);
    if (!dmc_init(handle))
        goto _failure1;
    pot = 0;
    min = 0UL;
    max = 0x1000000UL;
    i_ptr = 0;
    o_ptr = 0;
    i_buf = (uchar*)src;
    o_buf = (uchar*)dst;

    for (;;) {
        if (i_ptr >= srclen) {
             min = max - 1;
             break;
        }
        cha = i_buf[i_ptr++];
        for (idx = 0; idx < 8; idx++) {
            bit = (cha << idx) & 0x80;
            mid = (int32u)(min + (max - min - 1) * dmc_predict(handle));
            dmc_update(handle, bit != 0);
            if (mid == min)
                mid++;
            if (mid == max - 1)
                mid--;
            if (bit) min = mid;
            else     max = mid;
            while (max - min < 256) {
                if (bit) max--;
                if (o_ptr >= dstlen)
                    goto _failure2;
                o_buf[o_ptr++] = (byte_t)(min >> 16);
                min = (min << 8) & 0xFFFF00UL;
                max = (max << 8) & 0xFFFF00UL;
                if (min >= max)
                    max = 0x1000000UL;
            }
        }
        if (!(i_ptr & 0xFF)) {
            if (o_ptr - pot > 256)
                dmc_flush(handle);
            pot = o_ptr;
        }
    }
    if (o_ptr + 3 > dstlen)
        goto _failure2;
    o_buf[o_ptr]     = (byte_t)(min >> 16);
    o_buf[o_ptr + 1] = (byte_t)(min >>  8);
    o_buf[o_ptr + 2] = (byte_t)(min);
    dmc_free(handle);
    mem_free(handle);
    return (o_ptr + 3);

_failure2:
    dmc_free(handle);
_failure1:
    mem_free(handle);
    return (0);
}

/*
=======================================
    DMC 解压
=======================================
*/
CR_API leng_t
uncompr_dmc (
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen
    )
{
    uint_t  idx;
    leng_t  pin;
    uchar*  i_buf;
    uchar*  o_buf;
    leng_t  i_ptr;
    leng_t  o_ptr;
    /* -------- */
    sDMC*   handle;
    byte_t  cha, bit;
    int32u  max, min, mid, val;

    if (srclen < 3)
        return (0);
    handle = struct_new(sDMC);
    if (handle == NULL)
        return (0);
    if (!dmc_init(handle))
        goto _failure1;
    pin = 3;
    min = 0UL;
    max = 0x1000000UL;
    i_ptr = 0;
    o_ptr = 0;
    i_buf = (uchar*)src;
    o_buf = (uchar*)dst;

    val  = (int32u)i_buf[i_ptr] << 16;
    val |= (int32u)i_buf[i_ptr + 1] << 8;
    val |= (int32u)i_buf[i_ptr + 2];
    i_ptr += 3;
    for (;;) {
        cha = 0;
        if (val == max - 1)
            break;
        for (idx = 0; idx < 8; idx++) {
            mid = (int32u)(min + (max - min - 1) * dmc_predict(handle));
            if (mid == min)
                mid++;
            if (mid == max - 1)
                mid--;
            if (val >= mid) {
                bit = 1;
                min = mid;
            }
            else {
                bit = 0;
                max = mid;
            }
            dmc_update(handle, bit != 0);
            cha = cha + cha + bit;
            while (max - min < 256) {
                if (bit) max--;
                if (i_ptr >= srclen)
                    goto _failure2;
                val = ((val << 8) & 0xFFFF00UL) | i_buf[i_ptr++];
                min = ((min << 8) & 0xFFFF00UL);
                max = ((max << 8) & 0xFFFF00UL);
                if (min >= max)
                    max = 0x1000000UL;
            }
        }

        if (o_ptr >= dstlen)
            goto _failure2;
        o_buf[o_ptr++] = cha;
        if (!(o_ptr & 0xFF)) {
            if (i_ptr - pin > 256)
                dmc_flush(handle);
            pin = i_ptr;
        }
    }
    dmc_free(handle);
    mem_free(handle);
    return (o_ptr);

_failure2:
    dmc_free(handle);
_failure1:
    mem_free(handle);
    return (0);
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
