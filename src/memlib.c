/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2009-12-16  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 内存操作函数库 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "safe.h"
#include "applib.h"
#include "memlib.h"
#include "mtplib.h"

/* 内存的分配器 */
static cr_alloc_t   s_alloc = NULL;
static cr_mfree_t   s_mfree = NULL;

/* 指针前结构体 */
typedef struct
{
        leng_t  alloc_size;     /* 分配大小 */
        leng_t  check_sums;     /* 反码校验 */
} ffunc;

/* 调试用的计数 */
#if !defined(_CR_NO_MT_)
static lock_t   s_dbg_lock;     /* 数值设置的锁 */
#endif
static leng_t   s_dbg_size = 0; /* 已分配的大小 */
static leng_t   s_dbg_maxs = 0; /* 单个分配峰值 */
static leng_t   s_dbg_tots = 0; /* 已分配的峰值 */
static sint_t   s_dbg_cnts = 0; /* 已分配的次数 */

/*
=======================================
    初始化内存分配器
=======================================
*/
CR_API void_t
mem_init (
  __CR_IN__ cr_alloc_t  alloc,
  __CR_IN__ cr_mfree_t  mfree
    )
{
    if (alloc == NULL ||
        mfree == NULL)
        return;
    s_alloc = alloc;
    s_mfree = mfree;
    splock_init(&s_dbg_lock);
}

/*
=======================================
    清零调试计数
=======================================
*/
CR_API void_t
mem_reset (void_t)
{
    splock_acquire(&s_dbg_lock);
    s_dbg_size = 0;
    s_dbg_maxs = 0;
    s_dbg_tots = 0;
    s_dbg_cnts = 0;
    splock_release(&s_dbg_lock);
}

/*
=======================================
    获取调试计数
=======================================
*/
CR_API void_t
mem_count (
  __CR_OT__ dist_t* size,
  __CR_OT__ leng_t* maxs,
  __CR_OT__ leng_t* tots,
  __CR_OT__ sint_t* count
    )
{
    splock_acquire(&s_dbg_lock);
    if (size  != NULL) *size  = s_dbg_size;
    if (maxs  != NULL) *maxs  = s_dbg_maxs;
    if (tots  != NULL) *tots  = s_dbg_tots;
    if (count != NULL) *count = s_dbg_cnts;
    splock_release(&s_dbg_lock);
}

/*
=======================================
    内存分配
=======================================
*/
CR_API void_t*
mem_malloc (
  __CR_IN__ leng_t  size
    )
{
    ffunc*  hdr;
    uchar*  buf;
    leng_t  off;

    /* 必须先初始化 */
    if (s_alloc == NULL)
        return (NULL);

    /* 排除大小0, 防溢出 */
    if ((dist_t)size <= 0)
        return (NULL);

    /* 保存原始分配大小 */
    /* 头数据 + 16B 对齐 */
    off = size;
    size += sizeof(ffunc) + 16;

    /* 分配内存 */
    hdr = (ffunc*)s_alloc(size);
    if (hdr == NULL)
        return (NULL);
    hdr->alloc_size =  off;
    hdr->check_sums = ~off;

    /* 设置调试计数 */
#ifndef _CR_NO_MM_DEBUG_
    splock_acquire(&s_dbg_lock);
    s_dbg_cnts++;
    s_dbg_size += size;
    if (s_dbg_maxs < size)
        s_dbg_maxs = size;
    if (s_dbg_tots < s_dbg_size)
        s_dbg_tots = s_dbg_size;
    splock_release(&s_dbg_lock);
#endif
    /* 16B 对齐计算 */
    buf = (uchar*)(&hdr[1]);
    off = (leng_t)CR_EXTEND(buf, 16);
    buf += off;

    /* 存放偏移数据 */
    *(buf - 1) = (uchar)(off + sizeof(ffunc));
    return ((void_t*)buf);
}

/*
=======================================
    内存释放
=======================================
*/
CR_API void_t
mem_free (
  __CR_IN__ const void_t*   ptr
    )
{
    ffunc*  hdr;
    uint_t  off;

    /* 必须先初始化 */
    if (s_mfree == NULL)
        return;

    /* 清除偏移防重入 */
    if (ptr == NULL)
        return;
    off = *((uchar*)ptr - 1);
    *((uchar*)ptr - 1) = 0xFF;
    if (off <= sizeof(ffunc) || off > 16 + sizeof(ffunc)) {
        msg_stopA("invalid memory chunk", "crhack");
        return;
    }
    hdr = (ffunc*)((uchar*)ptr - off);
    if (hdr->alloc_size != ~hdr->check_sums) {
        msg_stopA("invalid memory chunk", "crhack");
        return;
    }

    /* 设置调试计数 */
#ifndef _CR_NO_MM_DEBUG_
    splock_acquire(&s_dbg_lock);
    s_dbg_cnts--;
    s_dbg_size -= hdr->alloc_size;
    s_dbg_size -= sizeof(ffunc) + 16;
    splock_release(&s_dbg_lock);
#endif
    /* 释放内存 */
    s_mfree((void_t*)hdr);
}

/*
=======================================
    内存大小
=======================================
*/
CR_API void_t*
mem_size (
  __CR_IN__ const void_t*   ptr,
  __CR_OT__ leng_t*         size
    )
{
    ffunc*  hdr;
    uint_t  off;

    if (ptr == NULL)
        return (NULL);
    off = *((uchar*)ptr - 1);
    if (off <= sizeof(ffunc) || off > 16 + sizeof(ffunc))
        return (NULL);
    hdr = (ffunc*)((uchar*)ptr - off);
    if (hdr->alloc_size != ~hdr->check_sums)
        return (NULL);
    if (size != NULL)
        *size = hdr->alloc_size;
    return ((void_t*)hdr);
}

/*
=======================================
    乘法内存分配器
=======================================
*/
CR_API void_t*
mem_calloc (
  __CR_IN__ leng_t  num,
  __CR_IN__ leng_t  size
    )
{
    leng_t  tmp;

    if (cut_mul(&tmp, num, size))
        return (NULL);
    return (mem_malloc(tmp));
}

/*
=======================================
    内存分配 (32位数包装)
=======================================
*/
CR_API void_t*
mem_malloc32 (
  __CR_IN__ int32u  size
    )
{
    leng_t  tmp;

    if (cut_int32u(&tmp, size))
        return (NULL);
    return (mem_malloc(tmp));
}

/*
=======================================
    内存分配 (64位数包装)
=======================================
*/
CR_API void_t*
mem_malloc64 (
  __CR_IN__ int64u  size
    )
{
    leng_t  tmp;

    if (cut_int64u(&tmp, size))
        return (NULL);
    return (mem_malloc(tmp));
}

/*
=======================================
    乘法内存分配器 (32位数包装)
=======================================
*/
CR_API void_t*
mem_calloc32 (
  __CR_IN__ int32u  num,
  __CR_IN__ leng_t  size
    )
{
    leng_t  tmp;

    if (cut_int32u(&tmp, num))
        return (NULL);
    return (mem_calloc(tmp, size));
}

/*
=======================================
    乘法内存分配器 (64位数包装)
=======================================
*/
CR_API void_t*
mem_calloc64 (
  __CR_IN__ int64u  num,
  __CR_IN__ leng_t  size
    )
{
    leng_t  tmp;

    if (cut_int64u(&tmp, num))
        return (NULL);
    return (mem_calloc(tmp, size));
}

/*
=======================================
    内存重分配
=======================================
*/
CR_API void_t*
mem_realloc (
  __CR_IN__ void_t* ptr,
  __CR_IN__ leng_t  new_size
    )
{
    ffunc*  hdr;
    uchar*  buf;
    uint_t  off;

    if (ptr == NULL)
        return (mem_malloc(new_size));

    /* 安全检查 */
    off = *((uchar*)ptr - 1);
    if (off <= sizeof(ffunc) || off > 16 + sizeof(ffunc)) {
        msg_stopA("invalid memory chunk", "crhack");
        return (NULL);
    }
    hdr = (ffunc*)((uchar*)ptr - off);
    if (hdr->alloc_size != ~hdr->check_sums) {
        msg_stopA("invalid memory chunk", "crhack");
        return (NULL);
    }
    if (new_size == hdr->alloc_size)
        return (ptr);
    if (new_size == 0) {
        mem_free(ptr);
        return (NULL);
    }
    buf = (uchar*)mem_malloc(new_size);
    if (buf != NULL)
        mem_cpy(buf, ptr, CR_MIN(new_size, hdr->alloc_size));
    mem_free(ptr);
    return ((void_t*)buf);
}

/*
=======================================
    内存克隆
=======================================
*/
CR_API void_t*
mem_dup (
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          len
    )
{
    void_t* tmp = mem_malloc(len);

    if (tmp == NULL)
        return (NULL);
    return (mem_cpy(tmp, src, len));
}

/*
=======================================
    支持通配符的内存比较
=======================================
*/
CR_API sint_t
mem_cmp2 (
  __CR_IN__ const void_t*   dst,
  __CR_IN__ const void_t*   src,
  __CR_IN__ leng_t          len
    )
{
    ansi_t  dv, sv;

    for (; len != 0; len--)
    {
        dv = *(ansi_t*)dst;
        sv = *(ansi_t*)src;
        if (dv != sv && sv != CR_AC('*'))
            return ((sint_t)(dv - sv));

        dst = (uchar*)dst + 1;
        src = (uchar*)src + 1;
    }
    return ((sint_t)0x0000);
}

/*
=======================================
    替换内存内容
=======================================
*/
CR_API void_t*
mem_replace (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          srclen,
  __CR_OT__ leng_t*         dstlen,
  __CR_IN__ leng_t          count,
  __CR_IN__ const leng_t    offsets[],
  __CR_IN__ const leng_t    blocks[],
  __CR_IN__ const void_t*   rplcs[],
  __CR_IN__ const leng_t    rp_sizes[]
    )
{
    byte_t  *new_data, *ptr;
    leng_t  copyed, idx, last;
    dist_t  temp, total = srclen;

    /* 生成替换后的内存块 */
    for (idx = 0; idx < count; idx++)
    {
        temp = rp_sizes[idx];
        temp -=  blocks[idx];

        if (cut_adds(&total, total, temp))
            return (NULL);
    }

    ptr = (byte_t*)mem_malloc((leng_t)total);
    if ((new_data = ptr) == NULL)
        return (NULL);

    /* 逐块输出替换 */
    for (idx = 0, last = 0; idx < count; idx++)
    {
        /* 安全检查, 偏移必须是升序不重叠的, 替换块不能超过原大小 */
        if (offsets[idx] < last  || offsets[idx] > srclen ||
             blocks[idx] > srclen - offsets[idx]) {
            mem_free(new_data);
            return (NULL);
        }

        /* 复制到替换区段开始处 */
        copyed = offsets[idx] - last;
        mem_cpy(ptr, (uchar*)data + last, copyed);
        ptr += copyed;

        /* NULL 表示删除处理 */
        if (rplcs[idx] != NULL)
        {
            /* 替换指定的数据段 */
            mem_cpy(ptr, rplcs[idx], rp_sizes[idx]);
            ptr += rp_sizes[idx];
        }
        last = offsets[idx] + blocks[idx];
    }

    /* 复制剩余的数据 */
    mem_cpy(ptr, (uchar*)data + last, srclen - last);

    /* 返回新数据信息 */
    if (dstlen != NULL)
        *dstlen = (leng_t)total;
    return (new_data);
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
