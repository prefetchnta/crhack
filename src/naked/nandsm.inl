/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2011-06-10  */
/*     #######          ###    ###      [KRNL]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>>>> CrHack 简单 NAND 驱动函数库模板 <<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "memlib.h"

/* 交换的分区数量 */
#if !defined(NANDS_ZONE_NUM)
    #error "nandsm.inl: missing NANDS_ZONE_NUM define!"
#elif   (NANDS_ZONE_NUM <= 1)
    #error "nandsm.inl: invalid NANDS_ZONE_NUM define!"
#endif

/* 块首的 OOB 结构 */
typedef struct
{
/*000*/ byte_t  ecc[3];     /*  ECC 纠错码  */
/*003*/ byte_t  noused;     /* 不使用的成员 */
/*004*/ byte_t  is_dit;     /* 0x00 表示脏块 */
/*005*/ byte_t  is_bad;     /* 0x00 表示坏块 */
/*006*/
} sNANDS_OOB;

/* 快速全局查找表 */
#ifndef _pos_
    #define _pos_
#endif
static byte_t _pos_ s_nandsm[NANDS_BLOCK_NUM / NANDS_ZONE_NUM];

/* 需要外部实现的 NAND 操作接口函数声明 (页大小不包括 OOB 数据) */
CR_API retc_t   nandsm_block_erase (uint_t ablock);
CR_API retc_t   nandsm_block_check (uint_t ablock, int32u *page_addr);
CR_API retc_t   nandsm_page_read (int32u apage, void_t *buffer);
CR_API retc_t   nandsm_csec_read (int32u apage, void_t *buffer);
CR_API retc_t   nandsm_page_write (int32u apage, const void_t *buffer);
CR_API retc_t   nandsm_csec_write (int32u apage, const void_t *buffer);

/*
=======================================
    NANDSM 块参数
=======================================
*/
CR_API uint_t
nandsm_info (void_t)
{
    return (NANDS_BLOCK_NUM / NANDS_ZONE_NUM);
}

/*
---------------------------------------
    NANDSM 获取块信息
---------------------------------------
*/
static uint_t
nandsm_get_zone (
  __CR_IN__ uint_t  azone,
  __CR_IN__ uint_t  ablock,
  __CR_OT__ int32u* page_addr
    )
{
    int32u  apage;

    ablock += azone * nandsm_info();
    apage  = (int32u)ablock;
    apage *= NANDS_PAGE_PBLK;
    if (page_addr != NULL)
       *page_addr  = apage;
    return (ablock);
}

/*
=======================================
    NANDSM 初始化
=======================================
*/
CR_API retc_t
nandsm_init (void_t)
{
    int32u  apage;
    retc_t  retcode = TRUE;
    byte_t  csec[NANDS_CSEC_SIZE];
    uint_t  idx, blk, end = nandsm_info();
    sNANDS_OOB* oob = (sNANDS_OOB*)csec;

    /* 记录每个块的起始区 */
    for (blk = 0; blk < end; blk++)
    {
        /* 逐个分区查找非脏块 */
        for (idx = 0; idx < NANDS_ZONE_NUM; idx++)
        {
            nandsm_get_zone(idx, blk, &apage);
            if (nandsm_csec_read(apage, csec))
            {
                /* 是否是坏块或脏块 */
                if (oob->is_bad == 0xFF &&
                    oob->is_dit == 0xFF)
                    break;
            }
        }

        /* 已经没有可用的块了 */
        if (idx >= NANDS_ZONE_NUM)
            retcode = FALSE;
        s_nandsm[blk] = (byte_t)idx;
    }
    return (retcode);
}

/*
=======================================
    NANDSM 格式化
=======================================
*/
CR_API uint_t
nandsm_format (void_t)
{
    int32u  apage;
    uint_t  idx, bad_cnts = 0;
    byte_t  csec[NANDS_CSEC_SIZE];

    /* 整个芯片刷一遍 */
    mem_zero(csec, sizeof(csec));
    for (idx = 0; idx < NANDS_BLOCK_NUM; idx++)
    {
        if (nandsm_block_check(idx, &apage))
        {
            /* 非出厂坏块, 可以擦除 */
            if (!nandsm_block_erase(idx))
            {
                /* 擦写失败, 标为坏块 */
                nandsm_csec_write(apage, csec);
                bad_cnts += 1;
            }
        }
        else
        {
            bad_cnts += 1;
        }
    }
    return (bad_cnts);
}

/*
---------------------------------------
    NANDSM 读页数据 (内部)
---------------------------------------
*/
static void_t*
nandsm_read_int (
  __CR_OT__ void_t* data,
  __CR_IN__ uint_t  idx_b,
  __CR_IN__ uint_t  pge_s,  /* 块内页号 */
  __CR_IN__ uint_t  pge_e   /* 块内页号 */
    )
{
    uint_t  idx;
    int32u  apage;

    /* 找到块使用的分区 */
    idx = (uint_t)s_nandsm[idx_b];
    if (idx >= NANDS_ZONE_NUM)
        return (NULL);
    nandsm_get_zone(idx, idx_b, &apage);

    /* 读取块内指定的页数据 */
    for (; pge_s < pge_e; pge_s++) {
        if (!nandsm_page_read(apage + pge_s, data))
            return (NULL);
        data = (byte_t*)data + NANDS_PAGE_SIZE;
    }
    return (data);
}

/*
=======================================
    NANDSM 读页数据
=======================================
*/
CR_API retc_t
nandsm_read (
  __CR_IN__ int32u  apage,  /* 页地址 */
  __CR_OT__ void_t* data,
  __CR_IN__ leng_t  count   /* 页大小 */
    )
{
    uint_t  idx;
    uint_t  ablk1, rblk1;
    uint_t  ablk2, rblk2;

    /* 参数过滤 */
    if (count == 0)
        return (TRUE);
    ablk1 = (uint_t)(apage / NANDS_PAGE_PBLK);
    rblk1 = (uint_t)(apage % NANDS_PAGE_PBLK);
    if (ablk1 >= nandsm_info())
        return (FALSE);
    apage = (int32u)(apage + count);
    ablk2 = (uint_t)(apage / NANDS_PAGE_PBLK);
    rblk2 = (uint_t)(apage % NANDS_PAGE_PBLK);
    if ((ablk2 >  nandsm_info()) ||
        (ablk2 == nandsm_info() && rblk2 != 0))
        return (FALSE);

    /* 在同一块中 */
    if (ablk1 == ablk2) {
        data = nandsm_read_int(data, ablk1, rblk1, rblk2);
        return ((data == NULL) ? FALSE : TRUE);
    }

    /* 读取起始块数据 */
    data = nandsm_read_int(data, ablk1, rblk1, NANDS_PAGE_PBLK);
    if (data == NULL)
        return (FALSE);

    /* 读中间整块数据 */
    for (idx = ablk1 + 1; idx <= ablk2 - 1; idx++) {
        data = nandsm_read_int(data, idx, 0, NANDS_PAGE_PBLK);
        if (data == NULL)
            return (FALSE);
    }

    /* 读取结束块数据 */
    if (rblk2 != 0) {
        data = nandsm_read_int(data, ablk2, 0, rblk2);
        if (data == NULL)
            return (FALSE);
    }
    return (TRUE);
}

/*
---------------------------------------
    NANDSM 写页数据 (内部)
---------------------------------------
*/
static const void_t*
nandsm_write_int (
  __CR_IN__ const void_t*   data,
  __CR_IN__ uint_t          idx_b,
  __CR_IN__ uint_t          pge_s,  /* 块内页号 */
  __CR_IN__ uint_t          pge_e   /* 块内页号 */
    )
{
    uchar*  buffer;
    int32u  apage, xpage = 0;
    byte_t  cpys[NANDS_PAGE_SIZE];
    byte_t  csec[NANDS_CSEC_SIZE];
    uint_t  tmp, aidx, xidx, xblock = 0;
    sNANDS_OOB* oob = (sNANDS_OOB*)csec;

    /* 找到块使用的分区 */
    aidx = (uint_t)s_nandsm[idx_b];
    if (aidx >= NANDS_ZONE_NUM)
        return (NULL);
    nandsm_get_zone(aidx, idx_b, &apage);

    /* 定位下一个轮转的分区 */
    xidx = (aidx + 1) % NANDS_ZONE_NUM;

_retry_find:
    while (xidx != aidx)
    {
        xblock = nandsm_get_zone(xidx, idx_b, &xpage);
        if (nandsm_csec_read(xpage, csec))
        {
            /* 是否是坏块 */
            if (oob->is_bad == 0xFF)
                break;
        }
        xidx = (xidx + 1) % NANDS_ZONE_NUM;
    }

    /* 已经没有可轮转的块了 */
    if (xidx == aidx)
        return (NULL);

    /* 擦除新的轮转块 */
    buffer = (byte_t*)data;
    if (oob->is_dit != 0xFF) {
        if (!nandsm_block_erase(xblock))
            goto _failure;
    }

    /* 复制当前数据到新轮转的块 */
    for (tmp = 0; tmp < pge_s; tmp++)
    {
        /* 复制原始块的页数据 */
        if (!nandsm_page_read(apage + tmp, cpys))
            return (NULL);
        if (!nandsm_page_write(xpage + tmp, cpys))
            goto _failure;
    }
    for (; tmp < pge_e; tmp++)
    {
        /* 写入目标块的页数据 */
        if (!nandsm_page_write(xpage + tmp, buffer))
            goto _failure;
        buffer += NANDS_PAGE_SIZE;
    }
    for (; tmp < NANDS_PAGE_PBLK; tmp++)
    {
        /* 复制原始块的页数据 */
        if (!nandsm_page_read(apage + tmp, cpys))
            return (NULL);
        if (!nandsm_page_write(xpage + tmp, cpys))
            goto _failure;
    }

    /* 更新原始块的 OOB 信息 */
    /* 置脏标志, ECC 等无所谓了, 因为下次要擦除 */
    oob->is_dit = 0x00;
    if (!nandsm_csec_write(apage, csec)) {
        mem_zero(csec, sizeof(csec));
        nandsm_csec_write(apage, csec);
    }
    s_nandsm[idx_b] = (byte_t)xidx;
    return ((const void_t*)buffer);

_failure:
    mem_zero(csec, sizeof(csec));
    nandsm_csec_write(xpage, csec);

    /* 新出现的坏块, 再次进行轮转 */
    xidx = (xidx + 1) % NANDS_ZONE_NUM;
    goto _retry_find;
}

/*
=======================================
    NANDSM 写页数据
=======================================
*/
CR_API retc_t
nandsm_write (
  __CR_IN__ int32u          apage,  /* 页地址 */
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          count   /* 页大小 */
    )
{
    uint_t  idx;
    uint_t  ablk1, rblk1;
    uint_t  ablk2, rblk2;

    /* 参数过滤 */
    if (count == 0)
        return (TRUE);
    ablk1 = (uint_t)(apage / NANDS_PAGE_PBLK);
    rblk1 = (uint_t)(apage % NANDS_PAGE_PBLK);
    if (ablk1 >= nandsm_info())
        return (FALSE);
    apage = (int32u)(apage + count);
    ablk2 = (uint_t)(apage / NANDS_PAGE_PBLK);
    rblk2 = (uint_t)(apage % NANDS_PAGE_PBLK);
    if ((ablk2 >  nandsm_info()) ||
        (ablk2 == nandsm_info() && rblk2 != 0))
        return (FALSE);

    /* 在同一块中 */
    if (ablk1 == ablk2) {
        data = nandsm_write_int(data, ablk1, rblk1, rblk2);
        return ((data == NULL) ? FALSE : TRUE);
    }

    /* 写入起始块数据 */
    data = nandsm_write_int(data, ablk1, rblk1, NANDS_PAGE_PBLK);
    if (data == NULL)
        return (FALSE);

    /* 写中间整块数据 */
    for (idx = ablk1 + 1; idx <= ablk2 - 1; idx++) {
        data = nandsm_write_int(data, idx, 0, NANDS_PAGE_PBLK);
        if (data == NULL)
            return (FALSE);
    }

    /* 写入结束块数据 */
    if (rblk2 != 0) {
        data = nandsm_write_int(data, ablk2, 0, rblk2);
        if (data == NULL)
            return (FALSE);
    }
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
