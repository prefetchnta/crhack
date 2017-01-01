/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-01-13  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack LZSS 压缩解压函数库 <<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "enclib.h"
#include "memlib.h"

/* 标准 LZSS 常数 */
#ifndef LZSS_N
    #define LZSS_N          4096
    #define LZSS_THRESHOLD  2
    #define LZSS_NIL        LZSS_N
    #define LZSS_F          18
    #define LZSS_FILL       (uchar)fill
    #define LZSS_PARAM      , uint_t fill
    #define LZSS_ENCODE     compr_lzss
    #define LZSS_DECODE     uncompr_lzss
#endif

/* LZSS 内部结构 */
typedef struct
{
        uint_t  matchPos;
        uint_t  matchLen;
        uint_t  mdad[LZSS_N + 1];
        uint_t  lson[LZSS_N + 1];
        uint_t  rson[LZSS_N + 257];
        byte_t  textBuf[LZSS_N + LZSS_F - 1];
} sLZSS;

/*
---------------------------------------
    初始化 LZSS 树
---------------------------------------
*/
static void_t
lzss_init_tree (
  __CR_OT__ sLZSS*  handle
    )
{
    uint_t  idx;

    for (idx = 0; idx < LZSS_N; idx++)
        handle->mdad[idx] = LZSS_NIL;

    for (idx = LZSS_N + 1; idx <= LZSS_N + 256; idx++)
        handle->rson[idx] = LZSS_NIL;
}

/*
---------------------------------------
    插入 LZSS 树节点
---------------------------------------
*/
static void_t
lzss_insert_node (
  __CR_IN__ uint_t  r,
  __CR_IO__ sLZSS*  handle
    )
{
    uchar*  key;
    uint_t  idx;
    uint_t  ppp;
    sint_t  cmp;

    cmp = 1;
    key = &handle->textBuf[r];
    ppp = LZSS_N + 1 + key[0];

    handle->matchLen = 0;
    handle->lson[r] = LZSS_NIL;
    handle->rson[r] = LZSS_NIL;

    for (;;)
    {
        if (cmp >= 0)
        {
            if (handle->rson[ppp] == LZSS_NIL)
            {
                handle->mdad[r] = ppp;
                handle->rson[ppp] = r;
                return;
            }
            ppp = handle->rson[ppp];
        }
        else
        {
            if (handle->lson[ppp] == LZSS_NIL)
            {
                handle->mdad[r] = ppp;
                handle->lson[ppp] = r;
                return;
            }
            ppp = handle->lson[ppp];
        }

        for (idx = 1; idx < LZSS_F; idx++)
        {
            cmp = key[idx] - handle->textBuf[ppp + idx];
            if (cmp != 0)
                break;
        }

        if (idx > handle->matchLen)
        {
            handle->matchLen = idx;
            handle->matchPos = ppp;

            if (idx >= LZSS_F)
                break;
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
    handle->mdad[ppp] = LZSS_NIL;
}

/*
---------------------------------------
    删除 LZSS 树节点
---------------------------------------
*/
static void_t
lzss_delete_node (
  __CR_IN__ uint_t  p,
  __CR_IO__ sLZSS*  handle
    )
{
    uint_t  qq;

    if (handle->mdad[p] == LZSS_NIL)
        return;

    if (handle->rson[p] == LZSS_NIL)
    {
        qq = handle->lson[p];
    }
    else
    if (handle->lson[p] == LZSS_NIL)
    {
        qq = handle->rson[p];
    }
    else
    {
        qq = handle->lson[p];
        if (handle->rson[qq] != LZSS_NIL)
        {
            do
            {
                qq = handle->rson[qq];
            } while (handle->rson[qq] != LZSS_NIL);

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
    handle->mdad[p] = LZSS_NIL;
}

/*
=======================================
    LZSS 压缩
=======================================
*/
CR_API leng_t
LZSS_ENCODE (
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen
  __CR_IN__ LZSS_PARAM
    )
{
    uchar*  i_buf;
    uchar*  o_buf;
    leng_t  i_ptr;
    leng_t  o_ptr;
    sLZSS*  handle;
    uint_t  rrr, sss;
    uint_t  idx, len;
    uint_t  code_buf_ptr;
    uint_t  last_match_length;
    byte_t  cha, mask, code_buf[17];

    if (dst == NULL)
        return (srclen + srclen / 8 + 1);

    handle = struct_new(sLZSS);
    if (handle == NULL)
        return (0);
    i_ptr = 0;
    o_ptr = 0;
    i_buf = (uchar*)src;
    o_buf = (uchar*)dst;

    code_buf[0]  = 0;
    code_buf_ptr = mask = 1;

    lzss_init_tree(handle);

    sss = 0;
    rrr = LZSS_N - LZSS_F;
    for (idx = 0; idx < rrr; idx++)
        handle->textBuf[idx] = LZSS_FILL;

    for (len = 0; len < LZSS_F && i_ptr < srclen; len++)
        handle->textBuf[rrr + len] = i_buf[i_ptr++];

    for (idx = 1; idx <= LZSS_F; idx++)
        lzss_insert_node(rrr - idx, handle);
    lzss_insert_node(rrr, handle);

    do
    {
        if (handle->matchLen > len)
            handle->matchLen = len;

        if (handle->matchLen <= LZSS_THRESHOLD)
        {
            code_buf[0] |= mask;
            code_buf[code_buf_ptr++] = handle->textBuf[rrr];

            handle->matchLen = 1;
        }
        else
        {
            code_buf[code_buf_ptr++] = (uchar)   handle->matchPos;
            code_buf[code_buf_ptr++] = (uchar)(((handle->matchPos >> 4)
                          & 0xF0) | (handle->matchLen - (LZSS_THRESHOLD + 1)));
        }

        if ((mask <<= 1) == 0)
        {
            for (idx = 0; idx < code_buf_ptr; idx++)
            {
                if (o_ptr >= dstlen)
                    goto _failure;
                o_buf[o_ptr++] = code_buf[idx];
            }
            code_buf[0]  = 0;
            code_buf_ptr = mask = 1;
        }
        last_match_length = handle->matchLen;

        for (idx = 0; idx < last_match_length && i_ptr < srclen; idx++)
        {
            cha = i_buf[i_ptr++];

            lzss_delete_node(sss, handle);

            handle->textBuf[sss] = cha;
            if (sss < LZSS_F - 1)
                handle->textBuf[sss + LZSS_N] = cha;

            sss = (sss + 1) & (LZSS_N - 1);
            rrr = (rrr + 1) & (LZSS_N - 1);
            lzss_insert_node(rrr, handle);
        }

        while (idx++ < last_match_length)
        {
            lzss_delete_node(sss, handle);
            sss = (sss + 1) & (LZSS_N - 1);
            rrr = (rrr + 1) & (LZSS_N - 1);

            if (--len != 0)
                lzss_insert_node(rrr, handle);
        }
    } while (len > 0);

    if (code_buf_ptr > 1)
    {
        for (idx = 0; idx < code_buf_ptr; idx++)
        {
            if (o_ptr >= dstlen)
                goto _failure;
            o_buf[o_ptr++] = code_buf[idx];
        }
    }
    mem_free(handle);
    return (o_ptr);

_failure:
    mem_free(handle);
    return (0);
}

/*
=======================================
    LZSS 解压
=======================================
*/
CR_API leng_t
LZSS_DECODE (
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen
  __CR_IN__ LZSS_PARAM
    )
{
    byte_t  cha;
    uint_t  flags;
    uchar*  i_buf;
    uchar*  o_buf;
    leng_t  i_ptr;
    leng_t  o_ptr;
    uint_t  ii, jj;
    uint_t  kk, rr;
    sLZSS*  handle;

    handle = struct_new(sLZSS);
    if (handle == NULL)
        return (0);
    flags = 0;
    i_ptr = 0;
    o_ptr = 0;
    i_buf = (uchar*)src;
    o_buf = (uchar*)dst;

    rr = LZSS_N - LZSS_F;
    for (ii = 0; ii < rr; ii++)
        handle->textBuf[ii] = LZSS_FILL;

    for (;;)
    {
        if (((flags >>= 1) & 256) == 0)
        {
            if (i_ptr >= srclen)
                break;
            flags = i_buf[i_ptr++] | 0xFF00;
        }

        if ((flags & 1) != 0)
        {
            if (i_ptr >= srclen || o_ptr >= dstlen)
                goto _failure;
            o_buf[o_ptr++]        = i_buf[i_ptr+0];
            handle->textBuf[rr++] = i_buf[i_ptr++];

            rr &= (LZSS_N - 1);
        }
        else
        {
            if (i_ptr >= srclen || i_ptr + 1 >= srclen)
                break;
            ii  = i_buf[i_ptr++];
            jj  = i_buf[i_ptr++];
            ii |= (jj & 0xF0) << 4;
            jj  = (jj & 0x0F) + LZSS_THRESHOLD;

            for (kk = 0; kk <= jj; kk++)
            {
                if (o_ptr >= dstlen)
                    goto _failure;
                cha = handle->textBuf[(ii + kk) & (LZSS_N - 1)];

                o_buf[o_ptr++]        = cha;
                handle->textBuf[rr++] = cha;

                rr &= (LZSS_N - 1);
            }
        }
    }
    mem_free(handle);
    return (o_ptr);

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
