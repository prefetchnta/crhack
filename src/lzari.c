/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-01-14  */
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
/*  >>>>>>>>>>>>>>>>>>>>> CrHack LZARI 压缩解压函数库 <<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "enclib.h"
#include "memlib.h"

/* LZARI 常数 */
#define LZARI_N             4096
#define LZARI_F             60
#define LZARI_THRESHOLD     2
#define LZARI_NIL           LZARI_N
#define LZARI_M             15
#define LZARI_Q1            (1UL << LZARI_M)
#define LZARI_Q2            (2 * LZARI_Q1)
#define LZARI_Q3            (3 * LZARI_Q1)
#define LZARI_Q4            (4 * LZARI_Q1)
#define LZARI_MAXCUM        (LZARI_Q1 - 1)
#define LZARI_NCHAR         (256 - LZARI_THRESHOLD + LZARI_F)
#define LZARI_INIT(handle)  handle->lo = 0UL; \
                            handle->hi = LZARI_Q4; \
                            handle->value = 0UL; \
                            handle->shift = 0; \
                            handle->buff1 = 0; \
                            handle->mask1 = 128; \
                            handle->mask2 = 0; \
                            handle->i_ptr = 0; \
                            handle->o_ptr = 0; \
                            handle->s_len = srclen; \
                            handle->d_len = dstlen; \
                            handle->i_buf = (uchar*)src; \
                            handle->o_buf = (uchar*)dst;
/* LZARI 内部结构 */
typedef struct
{
        /* LZSS 部分 */
        uint_t  matchPos;
        uint_t  matchLen;
        uint_t  mdad[LZARI_N + 1];
        uint_t  lson[LZARI_N + 1];
        uint_t  rson[LZARI_N + 257];
        byte_t  textBuf[LZARI_N + LZARI_F - 1];

        /* 算术编码部分 */
        uint_t  shift;
        uint_t  mask1;
        uint_t  mask2;
        uint_t  buff1;
        uint_t  buff2;
        int32u  value;
        int32u  lo, hi;
        uint_t  posCum[LZARI_N + 1];
        uint_t  symCum[LZARI_NCHAR + 1];
        uint_t  symFreq[LZARI_NCHAR + 1];
        uint_t  charToSym[LZARI_NCHAR];
        uint_t  symToChar[LZARI_NCHAR + 1];

        /* 内存访问部分 */
        uchar*  i_buf;
        uchar*  o_buf;
        leng_t  i_ptr, s_len;
        leng_t  o_ptr, d_len;

} sLZARI;

/*
---------------------------------------
    输出一位
---------------------------------------
*/
static void_t
lzari_put_bit (
  __CR_IN__ uint_t  bit,
  __CR_IO__ sLZARI* handle
    )
{
    if (bit != 0)
        handle->buff1 |= handle->mask1;

    if ((handle->mask1 >>= 1) == 0) {
        if (handle->o_ptr < handle->d_len)
            handle->o_buf[handle->o_ptr] = (uchar)handle->buff1;
        handle->o_ptr++;

        handle->buff1 = 0;
        handle->mask1 = 128;
    }
}

/*
---------------------------------------
    获取一位
---------------------------------------
*/
static uint_t
lzari_get_bit (
  __CR_IO__ sLZARI* handle
    )
{
    if ((handle->mask2 >>= 1) == 0) {
        if (handle->i_ptr < handle->s_len)
            handle->buff2 = handle->i_buf[handle->i_ptr];
        else
            handle->buff2 = 0;
        handle->i_ptr++;
        handle->mask2 = 128;
    }
    return ((handle->buff2 & handle->mask2) != 0);
}

/*
---------------------------------------
    输出一个字节
---------------------------------------
*/
static void_t
lzari_flush_bit_buffer (
  __CR_IO__ sLZARI* handle
    )
{
    uint_t  idx;

    for (idx = 7; idx != 0; idx--)
        lzari_put_bit(0, handle);
}

/*
---------------------------------------
    输出几位
---------------------------------------
*/
static void_t
lzari_output (
  __CR_IN__ uint_t  bit,
  __CR_IO__ sLZARI* handle
    )
{
    lzari_put_bit(bit, handle);

    for (; handle->shift != 0; handle->shift--)
        lzari_put_bit(!bit, handle);
}

/*
---------------------------------------
    二分查找符号
---------------------------------------
*/
static uint_t
lzari_binary_search_sym (
  __CR_IN__ uint_t          x,
  __CR_IN__ const sLZARI*   handle
    )
{
    uint_t  ii = 1, kk;
    uint_t  jj = LZARI_NCHAR;

    while (ii < jj)
    {
        kk = (ii + jj) / 2;

        if (handle->symCum[kk] > x)
            ii = kk + 1;
        else
            jj = kk;
    }
    return (ii);
}

/*
---------------------------------------
    二分查找位置
---------------------------------------
*/
static uint_t
lzari_binary_search_pos (
  __CR_IN__ uint_t          x,
  __CR_IN__ const sLZARI*   handle
    )
{
    uint_t  ii = 1, kk;
    uint_t  jj = LZARI_N;

    while (ii < jj)
    {
        kk = (ii + jj) / 2;

        if (handle->posCum[kk] > x)
            ii = kk + 1;
        else
            jj = kk;
    }
    return (ii - 1);
}

/*
---------------------------------------
    初始化 LZSS 树
---------------------------------------
*/
static void_t
lzari_init_tree (
  __CR_OT__ sLZARI* handle
    )
{
    uint_t  idx;

    for (idx = 0; idx < LZARI_N; idx++)
        handle->mdad[idx] = LZARI_NIL;

    for (idx = LZARI_N + 1; idx <= LZARI_N + 256; idx++)
        handle->rson[idx] = LZARI_NIL;
}

/*
---------------------------------------
    插入 LZSS 树节点
---------------------------------------
*/
static void_t
lzari_insert_node (
  __CR_IN__ uint_t  r,
  __CR_IO__ sLZARI* handle
    )
{
    uchar*  key;
    uint_t  idx;
    uint_t  ppp;
    uint_t  tmp;
    sint_t  cmp;

    cmp = 1;
    key = &handle->textBuf[r];
    ppp = LZARI_N + 1 + key[0];

    handle->matchLen = 0;
    handle->lson[r] = LZARI_NIL;
    handle->rson[r] = LZARI_NIL;

    for (;;)
    {
        if (cmp >= 0)
        {
            if (handle->rson[ppp] == LZARI_NIL)
            {
                handle->mdad[r] = ppp;
                handle->rson[ppp] = r;
                return;
            }
            ppp = handle->rson[ppp];
        }
        else
        {
            if (handle->lson[ppp] == LZARI_NIL)
            {
                handle->mdad[r] = ppp;
                handle->lson[ppp] = r;
                return;
            }
            ppp = handle->lson[ppp];
        }

        for (idx = 1; idx < LZARI_F; idx++)
        {
            cmp = key[idx] - handle->textBuf[ppp + idx];
            if (cmp != 0)
                break;
        }

        if (idx > LZARI_THRESHOLD)
        {
            if (idx > handle->matchLen)
            {
                handle->matchLen = idx;
                handle->matchPos = (r - ppp) & (LZARI_N - 1);

                if (idx >= LZARI_F)
                    break;
            }
            else
            if (idx == handle->matchLen)
            {
                tmp = (r - ppp) & (LZARI_N - 1);
                if (tmp < handle->matchPos)
                    handle->matchPos = tmp;
            }
        }
    }
    handle->mdad[r] = handle->mdad[ppp];
    handle->lson[r] = handle->lson[ppp];
    handle->rson[r] = handle->rson[ppp];

    handle->mdad[handle->lson[ppp]] = r;
    handle->mdad[handle->rson[ppp]] = r;

    if (handle->rson[handle->mdad[ppp]] == ppp)
        handle->rson[handle->mdad[ppp]] = r;
    else
        handle->lson[handle->mdad[ppp]] = r;
    handle->mdad[ppp] = LZARI_NIL;
}

/*
---------------------------------------
    删除 LZSS 树节点
---------------------------------------
*/
static void_t
lzari_delete_node (
  __CR_IN__ uint_t  p,
  __CR_IO__ sLZARI* handle
    )
{
    uint_t  qq;

    if (handle->mdad[p] == LZARI_NIL)
        return;

    if (handle->rson[p] == LZARI_NIL)
    {
        qq = handle->lson[p];
    }
    else
    if (handle->lson[p] == LZARI_NIL)
    {
        qq = handle->rson[p];
    }
    else
    {
        qq = handle->lson[p];
        if (handle->rson[qq] != LZARI_NIL)
        {
            do
            {
                qq = handle->rson[qq];
            } while (handle->rson[qq] != LZARI_NIL);

            handle->rson[handle->mdad[qq]] = handle->lson[qq];
            handle->mdad[handle->lson[qq]] = handle->mdad[qq];
            handle->lson[qq] = handle->lson[p];
            handle->mdad[handle->lson[p]] = qq;
        }
        handle->rson[qq] = handle->rson[p];
        handle->mdad[handle->rson[p]] = qq;
    }
    handle->mdad[qq] = handle->mdad[p];

    if (handle->rson[handle->mdad[p]] == p)
        handle->rson[handle->mdad[p]] = qq;
    else
        handle->lson[handle->mdad[p]] = qq;
    handle->mdad[p] = LZARI_NIL;
}

/*
---------------------------------------
    开始建模
---------------------------------------
*/
static void_t
lzari_start_model (
  __CR_OT__ sLZARI* handle
    )
{
    uint_t  cha, idx, sym;

    handle->symCum[LZARI_NCHAR] = 0;

    for (sym = LZARI_NCHAR; sym >= 1; sym--)
    {
        cha = sym - 1;
        handle->symFreq[sym] = 1;
        handle->symToChar[sym] = cha;
        handle->charToSym[cha] = sym;
        handle->symCum[sym - 1] = handle->symCum[sym] + handle->symFreq[sym];
    }
    handle->symFreq[0]      = 0;
    handle->posCum[LZARI_N] = 0;

    for (idx = LZARI_N; idx >= 1; idx--)
        handle->posCum[idx - 1] = handle->posCum[idx] + 10000 / (idx + 200);
}

/*
---------------------------------------
    更新模型
---------------------------------------
*/
static void_t
lzari_update_model (
  __CR_IN__ uint_t  sym,
  __CR_IO__ sLZARI* handle
    )
{
    uint_t  idx;
    uint_t  cha;
    uint_t  ch_idx;
    uint_t  ch_sym;

    if (handle->symCum[0] >= LZARI_MAXCUM)
    {
        for (cha = 0, idx = LZARI_NCHAR; idx > 0; idx--)
        {
            handle->symCum[idx] = cha;
            cha += (handle->symFreq[idx] = (handle->symFreq[idx] + 1) >> 1);
        }
        handle->symCum[0] = cha;
    }

    for (idx = sym; handle->symFreq[idx] == handle->symFreq[idx - 1]; idx--);

    if (idx < sym)
    {
        ch_idx = handle->symToChar[idx];
        ch_sym = handle->symToChar[sym];
        handle->symToChar[idx] = ch_sym;
        handle->symToChar[sym] = ch_idx;
        handle->charToSym[ch_idx] = sym;
        handle->charToSym[ch_sym] = idx;
    }

    handle->symFreq[idx]++;
    while (idx-- != 0)
        handle->symCum[idx]++;
}

/*
---------------------------------------
    编码一个字符
---------------------------------------
*/
static void_t
lzari_encode_char (
  __CR_IN__ uint_t  ch,
  __CR_IO__ sLZARI* handle
    )
{
    uint_t  sym   = handle->charToSym[ch];
    int32u  range = handle->hi - handle->lo;

    handle->hi = handle->lo + (range * handle->symCum[sym - 1]) /
                 handle->symCum[0];
    handle->lo += (range * handle->symCum[sym]) / handle->symCum[0];

    for (;;)
    {
        if (handle->hi <= LZARI_Q2)
        {
            lzari_output(0, handle);
        }
        else
        if (handle->lo >= LZARI_Q2)
        {
            lzari_output(1, handle);
            handle->lo -= LZARI_Q2;
            handle->hi -= LZARI_Q2;
        }
        else
        if (handle->lo >= LZARI_Q1 && handle->hi <= LZARI_Q3)
        {
            handle->shift++;
            handle->lo -= LZARI_Q1;
            handle->hi -= LZARI_Q1;
        }
        else
        {
            break;
        }
        handle->lo += handle->lo;
        handle->hi += handle->hi;
    }
    lzari_update_model(sym, handle);
}

/*
---------------------------------------
    编码一个位置
---------------------------------------
*/
static void_t
lzari_encode_position (
  __CR_IN__ uint_t  pos,
  __CR_IO__ sLZARI* handle
    )
{
    int32u  range = handle->hi - handle->lo;

    handle->hi = handle->lo + (range * handle->posCum[pos]) /
                 handle->posCum[0];
    handle->lo += (range * handle->posCum[pos + 1]) / handle->posCum[0];

    for (;;)
    {
        if (handle->hi <= LZARI_Q2)
        {
            lzari_output(0, handle);
        }
        else
        if (handle->lo >= LZARI_Q2)
        {
            lzari_output(1, handle);
            handle->lo -= LZARI_Q2;
            handle->hi -= LZARI_Q2;
        }
        else
        if (handle->lo >= LZARI_Q1 && handle->hi <= LZARI_Q3)
        {
            handle->shift++;
            handle->lo -= LZARI_Q1;
            handle->hi -= LZARI_Q1;
        }
        else
        {
            break;
        }
        handle->lo += handle->lo;
        handle->hi += handle->hi;
    }
}

/*
---------------------------------------
    编码结束
---------------------------------------
*/
static void_t
lzari_encode_end (
  __CR_IO__ sLZARI* handle
    )
{
    handle->shift++;
    if (handle->lo < LZARI_Q1)
        lzari_output(0, handle);
    else
        lzari_output(1, handle);
    lzari_flush_bit_buffer(handle);
}

/*
---------------------------------------
    开始解码
---------------------------------------
*/
static void_t
lzari_start_decode (
  __CR_IO__ sLZARI* handle
    )
{
    uint_t  idx;

    for (idx = LZARI_M + 2; idx != 0; idx--)
        handle->value = 2 * handle->value + lzari_get_bit(handle);
}

/*
---------------------------------------
    解码一个字符
---------------------------------------
*/
static uint_t
lzari_decode_char (
  __CR_IO__ sLZARI* handle
    )
{
    uint_t  sym, cha;
    int32u  range = handle->hi - handle->lo;

    sym = lzari_binary_search_sym((uint_t)(((handle->value - handle->lo + 1)
                                * handle->symCum[0] - 1) / range), handle);

    handle->hi = handle->lo + (range * handle->symCum[sym - 1]) /
                 handle->symCum[0];
    handle->lo += (range * handle->symCum[sym]) / handle->symCum[0];

    for (;;)
    {
        if (handle->lo >= LZARI_Q2)
        {
            handle->lo    -= LZARI_Q2;
            handle->hi    -= LZARI_Q2;
            handle->value -= LZARI_Q2;
        }
        else
        if (handle->lo >= LZARI_Q1 && handle->hi <= LZARI_Q3)
        {
            handle->lo    -= LZARI_Q1;
            handle->hi    -= LZARI_Q1;
            handle->value -= LZARI_Q1;
        }
        else
        if (handle->hi > LZARI_Q2)
        {
            break;
        }
        handle->lo += handle->lo;
        handle->hi += handle->hi;
        handle->value = 2 * handle->value + lzari_get_bit(handle);
    }
    cha = handle->symToChar[sym];
    lzari_update_model(sym, handle);
    return (cha);
}

/*
---------------------------------------
    解码一个位置
---------------------------------------
*/
static uint_t
lzari_decode_position (
  __CR_IO__ sLZARI* handle
    )
{
    uint_t  pos;
    int32u  range = handle->hi - handle->lo;

    pos = lzari_binary_search_pos((uint_t)(((handle->value - handle->lo + 1)
                                * handle->posCum[0] - 1) / range), handle);

    handle->hi = handle->lo + (range * handle->posCum[pos]) /
                 handle->posCum[0];
    handle->lo += (range * handle->posCum[pos + 1]) / handle->posCum[0];

    for (;;)
    {
        if (handle->lo >= LZARI_Q2)
        {
            handle->lo    -= LZARI_Q2;
            handle->hi    -= LZARI_Q2;
            handle->value -= LZARI_Q2;
        }
        else
        if (handle->lo >= LZARI_Q1 && handle->hi <= LZARI_Q3)
        {
            handle->lo    -= LZARI_Q1;
            handle->hi    -= LZARI_Q1;
            handle->value -= LZARI_Q1;
        }
        else
        if (handle->hi > LZARI_Q2)
        {
            break;
        }
        handle->lo += handle->lo;
        handle->hi += handle->hi;
        handle->value = 2 * handle->value + lzari_get_bit(handle);
    }
    return (pos);
}

/*
=======================================
    LZARI 压缩
=======================================
*/
CR_API leng_t
compr_lzari (
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen,
  __CR_IN__ uint_t          fill
    )
{
    byte_t  cha;
    uint_t  rrr, sss;
    uint_t  idx, len;
    uint_t  last_match_length;
    sLZARI* handle;

    if (dst == NULL)
        return (srclen + srclen / 8 + 512);

    handle = struct_new(sLZARI);
    if (handle == NULL)
        return (0);
    LZARI_INIT(handle)
    lzari_init_tree(handle);
    lzari_start_model(handle);

    sss = 0;
    rrr = LZARI_N - LZARI_F;

    for (idx = 0; idx < rrr; idx++)
        handle->textBuf[idx] = (uchar)fill;

    for (len = 0; len < LZARI_F && handle->i_ptr < srclen; len++)
        handle->textBuf[rrr + len] = handle->i_buf[handle->i_ptr++];

    for (idx = 1; idx <= LZARI_F; idx++)
        lzari_insert_node(rrr - idx, handle);
    lzari_insert_node(rrr, handle);

    do
    {
        if (handle->matchLen > len)
            handle->matchLen = len;

        if (handle->matchLen <= LZARI_THRESHOLD)
        {
            handle->matchLen = 1;
            lzari_encode_char(handle->textBuf[rrr], handle);
        }
        else
        {
            lzari_encode_char(255 - LZARI_THRESHOLD +
                              handle->matchLen, handle);
            lzari_encode_position(handle->matchPos - 1, handle);
        }
        last_match_length = handle->matchLen;

        for (idx = 0; idx < last_match_length && handle->i_ptr < srclen; idx++)
        {
            cha = handle->i_buf[handle->i_ptr++];

            lzari_delete_node(sss, handle);

            handle->textBuf[sss] = cha;
            if (sss < LZARI_F - 1)
                handle->textBuf[sss + LZARI_N] = cha;

            sss = (sss + 1) & (LZARI_N - 1);
            rrr = (rrr + 1) & (LZARI_N - 1);
            lzari_insert_node(rrr, handle);
        }

        while (idx++ < last_match_length)
        {
            lzari_delete_node(sss, handle);
            sss = (sss + 1) & (LZARI_N - 1);
            rrr = (rrr + 1) & (LZARI_N - 1);

            if (--len != 0)
                lzari_insert_node(rrr, handle);
        }
    } while (len > 0);

    lzari_encode_end(handle);

    if (handle->o_ptr > dstlen) {
        mem_free(handle);
        return (0);
    }
    dstlen = handle->o_ptr;
    mem_free(handle);
    return (dstlen);
}

/*
=======================================
    LZARI 解压
=======================================
*/
CR_API leng_t
uncompr_lzari (
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen,
  __CR_IN__ uint_t          fill
    )
{
    uint_t  ii, jj;
    uint_t  kk, rr, cha;
    sLZARI* handle;

    handle = struct_new(sLZARI);
    if (handle == NULL)
        return (0);
    LZARI_INIT(handle)
    lzari_start_decode(handle);
    lzari_start_model(handle);

    rr = LZARI_N - LZARI_F;
    for (ii = 0; ii < rr; ii++)
        handle->textBuf[ii] = (uchar)fill;

    while (handle->o_ptr < dstlen)
    {
        cha = lzari_decode_char(handle);
        if (cha < 256)
        {
            if (handle->o_ptr >= dstlen)
                goto _failure;
            handle->textBuf[rr++]          = (uchar)cha;
            handle->o_buf[handle->o_ptr++] = (uchar)cha;

            rr &= (LZARI_N - 1);
        }
        else
        {
            jj = cha - 255 + LZARI_THRESHOLD;
            ii = (rr - lzari_decode_position(handle) - 1) & (LZARI_N - 1);

            for (kk = 0; kk < jj; kk++)
            {
                if (handle->o_ptr >= dstlen)
                    goto _failure;
                cha = handle->textBuf[(ii + kk) & (LZARI_N - 1)];

                handle->textBuf[rr++]          = (uchar)cha;
                handle->o_buf[handle->o_ptr++] = (uchar)cha;

                rr &= (LZARI_N - 1);
            }
        }
    }
    dstlen = handle->o_ptr;
    mem_free(handle);
    return (dstlen);

_failure:
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
