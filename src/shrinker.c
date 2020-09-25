/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2020-09-25  */
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
/*  >>>>>>>>>>>>>>>>>>> CrHack SHRINKER 压缩解压函数库 <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "enclib.h"
#include "memlib.h"

/* SHRINKER 常数 */
#define SHRINKER_MINMATCH   4
#define SHRINKER_HASH_BITS  15

/* COPY 函数的优化 */
#if defined(_CR_SYS64_)
    #define SHRINKER_T  int64u
#else
    #define SHRINKER_T  int32u
#endif
#define SHRINKER_MEMCPY_NOOVERLAP(a, b, c) \
    { \
        do { \
            if (a + sizeof(SHRINKER_T) > dst_end) \
                goto _failure; \
            *(SHRINKER_T*)a = *(SHRINKER_T*)b; \
            a += sizeof(SHRINKER_T); \
            b += sizeof(SHRINKER_T); \
        } while (b < c); \
        a -= (b - c); \
        b = c; \
    }
#define SHRINKER_MEMCPY_NOOVERLAP_NOSURPASS(a, b, c) \
    { \
        c -= sizeof(SHRINKER_T); \
        while (b < c) { \
            if (a + sizeof(SHRINKER_T) > dst_end) \
                goto _failure; \
            *(SHRINKER_T*)a = *(SHRINKER_T*)b; \
            a += sizeof(SHRINKER_T); \
            b += sizeof(SHRINKER_T); \
        } \
        c += sizeof(SHRINKER_T); \
        while (b < c) { \
            if (a >= dst_end) \
                goto _failure; \
            *a++ = *b++; \
        } \
    }
#define SHRINKER_MEMCPY(a, b, c) \
    { \
        if (a > b + sizeof(SHRINKER_T)) \
            SHRINKER_MEMCPY_NOOVERLAP(a, b, c) \
        else { \
            while (b < c) { \
                if (a >= dst_end) \
                    goto _failure; \
                *a++ = *b++; \
            } \
        } \
    }

/* 哈希计算 */
#define SHRINKER_HASH(a)    (((a) * 21788233UL) >> (32 - SHRINKER_HASH_BITS))

/*
=======================================
    SHRINKER 压缩
=======================================
*/
CR_API leng_t
compr_shrinker (
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen
    )
{
#ifndef SHRINKER_USE_STACK
    int32u  *ht;
#else
    int32u  ht[1 << SHRINKER_HASH_BITS];
#endif
    byte_t  cache, flag, *pflag, *pfind, *pcur;
    int32u  tmp, cpy_len, match_dist, cur_hash, cur_u32;
    byte_t  *p_src = (byte_t*)src, *p_dst = (byte_t*)dst;
    byte_t  *dst_end = p_dst + dstlen, *p_last_lit = p_src;
    byte_t  *src_end = p_src + srclen - SHRINKER_MINMATCH - 8;

    if ((srclen < 32) || (srclen > (1 << 27) - 1))
        return (0);
    if (dst == NULL)
        return (srclen + SHRINKER_MINMATCH + 8);

#ifndef SHRINKER_USE_STACK
    ht = mem_talloc(1 << SHRINKER_HASH_BITS, int32u);
    if (ht == NULL)
        return (0);
#endif
    mem_zero(ht, 4UL << SHRINKER_HASH_BITS);
    cur_u32 = *(int32u*)p_src;
    while ((p_src < src_end) && (p_dst < dst_end))
    {
        tmp = (int32u)(p_src - (byte_t*)src);
        pcur = p_src;
        cur_u32 = *(int32u*)pcur;
        cur_hash = SHRINKER_HASH(cur_u32);
        cache = ht[cur_hash] >> 27;
        pfind = (byte_t*)src + (ht[cur_hash] & 0x07FFFFFFUL);
        ht[cur_hash] = tmp | (int32u)(*p_src << 27);

        if (cache == (*pcur & 0x1F) && pfind + 0xFFFF >= pcur &&
            pfind < pcur && *(int32u*)pfind == *(int32u*)pcur)
        {
            pfind += sizeof(int32u);
            pcur  += sizeof(int32u);
            while (pcur < src_end && *(int32u*)pfind == *(int32u*)pcur) {
                pfind += sizeof(int32u);
                pcur  += sizeof(int32u);
            }
            if (pcur < src_end) {
                if (*(int16u*)pfind == *(int16u*)pcur) {
                    pfind += sizeof(int16u);
                    pcur  += sizeof(int16u);
                }
            }
            if (*pfind == *pcur) {
                pfind += sizeof(byte_t);
                pcur  += sizeof(byte_t);
            }

            if (p_dst >= dst_end)
                goto _failure;
            pflag = p_dst++;
            cpy_len = (int32u)(p_src - p_last_lit);
            if (cpy_len < 7) {
                flag = (byte_t)(cpy_len << 5);
            }
            else {
                cpy_len -= 7;
                flag = 7 << 5;
                while (cpy_len >= 255) {
                    if (p_dst >= dst_end)
                        goto _failure;
                    *p_dst++ = 255;
                    cpy_len -= 255;
                }
                if (p_dst >= dst_end)
                    goto _failure;
                *p_dst++ = (byte_t)cpy_len;
            }

            cpy_len = (int32u)(pcur - p_src - SHRINKER_MINMATCH);
            if (cpy_len < 15) {
                flag |= (byte_t)cpy_len;
            }
            else {
                cpy_len -= 15;
                flag |= 15;
                while (cpy_len >= 255) {
                    if (p_dst >= dst_end)
                        goto _failure;
                    *p_dst++ = 255;
                    cpy_len -= 255;
                }
                if (p_dst >= dst_end)
                    goto _failure;
                *p_dst++ = (byte_t)cpy_len;
            }
            match_dist = (int32u)(pcur - pfind - 1);
            *pflag = flag;
            if (p_dst >= dst_end)
                goto _failure;
            *p_dst++ = (byte_t)match_dist;
            if (match_dist > 0xFF) {
                *pflag |= 16;
                if (p_dst >= dst_end)
                    goto _failure;
                *p_dst++ = (byte_t)(match_dist >> 8);
            }
            SHRINKER_MEMCPY_NOOVERLAP(p_dst, p_last_lit, p_src)

            cur_u32 = *(int32u*)(p_src + 1);
            ht[SHRINKER_HASH(cur_u32)] = (int32u)
                            ((p_src - (byte_t*)src + 1) | (p_src[1] << 27));
            cur_u32 = *(int32u*)(p_src + 3);
            ht[SHRINKER_HASH(cur_u32)] = (int32u)
                            ((p_src - (byte_t*)src + 3) | (p_src[3] << 27));
            p_src = pcur;
            p_last_lit = p_src;
            continue;
        }
        p_src++;
    }

    p_src = (byte_t*)src + srclen;
    if (p_dst >= dst_end)
        goto _failure;
    pflag = p_dst++;
    cpy_len = (int32u)(p_src - p_last_lit);
    if (cpy_len < 7) {
        flag = (byte_t)(cpy_len << 5);
    }
    else {
        cpy_len -= 7;
        flag = 7 << 5;
        while (cpy_len >= 255) {
            if (p_dst >= dst_end)
                goto _failure;
            *p_dst++ = 255;
            cpy_len -= 255;
        }
        if (p_dst >= dst_end)
            goto _failure;
        *p_dst++ = (byte_t)cpy_len;
    }

    flag |= 7 + 16;
    *pflag = flag;
    if (p_dst + 2 > dst_end)
        goto _failure;
    *p_dst++ = 0xFF;
    *p_dst++ = 0xFF;
    SHRINKER_MEMCPY_NOOVERLAP_NOSURPASS(p_dst, p_last_lit, p_src)
#ifndef SHRINKER_USE_STACK
    mem_free(ht);
#endif
    return ((leng_t)(p_dst - (byte_t*)dst));

_failure:
#ifndef SHRINKER_USE_STACK
    mem_free(ht);
#endif
    return (0);
}

/*
=======================================
    SHRINKER 解压
=======================================
*/
CR_API leng_t
uncompr_shrinker (
  __CR_OT__ void_t*         dst,
  __CR_IN__ leng_t          dstlen,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          srclen
    )
{
    byte_t  *p_src = (byte_t*)src;
    byte_t  *p_dst = (byte_t*)dst;
    byte_t  *dst_end = p_dst + dstlen;
    byte_t  *pcpy, *pend, flag, long_dist;
    int32u  literal_len, match_len, match_dist;

    for (;;)
    {
        if (srclen-- == 0)
            goto _failure;
        flag = *p_src++;
        literal_len = flag >> 5;
        match_len = flag & 0x0F;
        long_dist = flag & 0x10;

        if (literal_len == 7) {
            for (;;) {
                if (srclen-- == 0)
                    goto _failure;
                flag = *p_src++;
                if (flag != 255)
                    break;
                literal_len += 255;
            }
            literal_len += flag;
        }

        if (match_len == 15) {
            for (;;) {
                if (srclen-- == 0)
                    goto _failure;
                flag = *p_src++;
                if (flag != 255)
                    break;
                match_len += 255;
            }
            match_len += flag;
        }

        if (srclen-- == 0)
            goto _failure;
        match_dist = *p_src++;
        if (long_dist) {
            if (srclen-- == 0)
                goto _failure;
            match_dist |= ((*p_src++) << 8);
            if (match_dist == 0xFFFF) {
                pend = p_src + literal_len;
                SHRINKER_MEMCPY_NOOVERLAP_NOSURPASS(p_dst, p_src, pend)
                break;
            }
        }

        pend = p_src + literal_len;
        SHRINKER_MEMCPY_NOOVERLAP(p_dst, p_src, pend)
        pcpy = p_dst - match_dist - 1;
        pend = pcpy + match_len + SHRINKER_MINMATCH;
        if (pcpy < (byte_t*)dst)
            goto _failure;
        SHRINKER_MEMCPY(p_dst, pcpy, pend);
    }
    return (p_dst - (byte_t*)dst);

_failure:
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
