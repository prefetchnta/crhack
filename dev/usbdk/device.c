/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2012-02-03  */
/*     #######          ###    ###      [HARD]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> USBDK U盘 NAND 读写函数库 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "hash.h"
#include "device.h"

#include <stdint.h>
#include <string.h>

/*****************************************************************************/
/*                               算法实现部分                                */
/*****************************************************************************/

/* 算法常数的映射 */
#define NANDS_ZONE_NUM              4
#define NANDS_WAIT_WRT          0x0400000
#define NANDS_WAIT_ERS          0x1000000
#define NANDS_PAGE_SIZE     K9F1208C_PAGE_SIZE
#define NANDS_CSEC_SIZE     K9F1208C_CSEC_SIZE
#define NANDS_ABCA_SIZE     K9F1208C_ABCA_SIZE
#define NANDS_PAGE_PBLK     K9F1208C_PAGE_PBLK
#define NANDS_BLOCK_NUM     K9F1208C_BLOCK_NUM

#if     1
    #include "nandsm.inl"
#endif

/* 三个字节的 ECC 值 */
extern byte_t   g_nand_ecc[3];

/* USB MASS 那里的总长度 */
extern uint32_t SCSI_BlkLen;

/*
=======================================
    NAND 块擦除
=======================================
*/
CR_API retc_t
nandsm_block_erase (
  __CR_IN__ uint_t  ablock
    )
{
    return (k9f1208c_block_erase(ablock, NANDS_WAIT_ERS));
}

/*
=======================================
    NAND 坏块检查
=======================================
*/
CR_API retc_t
nandsm_block_check (
  __CR_IN__ uint_t  ablock,
  __CR_OT__ int32u* page_addr
    )
{
    return (k9f1208c_block_check(ablock, page_addr));
}

/*
=======================================
    NAND 数据页读取
=======================================
*/
CR_API retc_t
nandsm_page_read (
  __CR_IN__ int32u  apage,
  __CR_OT__ void_t* buffer
    )
{
#ifndef _USBDK_ECC_SOFT_
    int32u  ecc1;
    int32u  ecc2;
#else
    byte_t  ecc[3];
#endif
    byte_t  tmp[NANDS_ABCA_SIZE];

    /* 可选硬件或软件 ECC 计算 */
    if (!k9f1208c_page_read(apage, tmp))
        return (FALSE);
#ifndef _USBDK_ECC_SOFT_
    ecc1  = ((int32u)g_nand_ecc[0] <<  0);
    ecc1 |= ((int32u)g_nand_ecc[1] <<  8);
    ecc1 |= ((int32u)g_nand_ecc[2] << 16);
    ecc2  = ((int32u)tmp[NANDS_PAGE_SIZE + 0] <<  0);
    ecc2 |= ((int32u)tmp[NANDS_PAGE_SIZE + 1] <<  8);
    ecc2 |= ((int32u)tmp[NANDS_PAGE_SIZE + 2] << 16);
    parity_ecc1_comp_st(tmp, ecc1, ecc2, NANDS_PAGE_SIZE);
#else
    parity_ecc1_make(ecc, tmp, NANDS_PAGE_SIZE);
    parity_ecc1_comp(tmp, ecc, tmp + NANDS_PAGE_SIZE,
                     NANDS_PAGE_SIZE);
#endif
    memcpy(buffer, tmp, NANDS_PAGE_SIZE);
    return (TRUE);
}

/*
=======================================
    NAND 附加页读取
=======================================
*/
CR_API retc_t
nandsm_csec_read (
  __CR_IN__ int32u  apage,
  __CR_OT__ void_t* buffer
    )
{
    return (k9f1208c_csec_read(apage, buffer));
}

/*
=======================================
    NAND 数据页写入
=======================================
*/
CR_API retc_t
nandsm_page_write (
  __CR_IN__ int32u          apage,
  __CR_IN__ const void_t*   buffer
    )
{
    byte_t  tmp[NANDS_ABCA_SIZE];

    /* 可选硬件或软件 ECC 计算 */
    memcpy(tmp, buffer, NANDS_PAGE_SIZE);
#ifndef _USBDK_ECC_SOFT_
    memset(tmp + NANDS_PAGE_SIZE, 0xFF, NANDS_CSEC_SIZE);
    if (!k9f1208c_page_write(apage, tmp, NANDS_WAIT_WRT))
        return (FALSE);
    memcpy(tmp + NANDS_PAGE_SIZE, g_nand_ecc, 3);
    return (k9f1208c_csec_write(apage, tmp + NANDS_PAGE_SIZE,
                                NANDS_WAIT_WRT));
#else
    memset(tmp + NANDS_PAGE_SIZE + 3, 0xFF, NANDS_CSEC_SIZE - 3);
    parity_ecc1_make(tmp + NANDS_PAGE_SIZE, tmp, NANDS_PAGE_SIZE);
    return (k9f1208c_page_write(apage, tmp, NANDS_WAIT_WRT));
#endif
}

/*
=======================================
    NAND 附加页写入
=======================================
*/
CR_API retc_t
nandsm_csec_write (
  __CR_IN__ int32u          apage,
  __CR_IN__ const void_t*   buffer
    )
{
    return (k9f1208c_csec_write(apage, buffer, NANDS_WAIT_WRT));
}

/*****************************************************************************/
/*                               读写实现部分                                */
/*****************************************************************************/

/* 高速缓冲大小 */
#undef  X_SIZE
#define X_SIZE  CR_K2B(32)  /* 大于等于 NAND 一块的大小比较合适 */

/* 页写的高速缓存数据 */
static int32u   s_ptr;              /* 统计 cache 里的数据量 */
static int32u   s_total;            /* 统计1次操作总的写入量 */
static int32s   s_start_page;       /* 数据连续写入的起始页号 */
static int32s   s_last_page = -1;   /* 上一次数据写入的页号 */
static byte_t   s_cache[X_SIZE];    /* 数据连续写入的高速缓存 */

/*
=======================================
    NAND 初始化
=======================================
*/
CR_API retc_t
nand_init (void_t)
{
    ibus_init();
    return (nandsm_init());
}

/*
=======================================
    NAND 获取容量
=======================================
*/
CR_API retc_t
nand_state (
  __CR_OT__ int32u* size
    )
{
    sK9F1208C   id;

    k9f1208c_get_devid(&id);
    if (id.mak_id != 0xEC || id.dev_id != 0x76)
        return (FALSE);
    *size = nandsm_info() * NANDS_PAGE_PBLK * NANDS_PAGE_SIZE;
    return (TRUE);
}

/*
=======================================
    NAND 格式化
=======================================
*/
CR_API uint_t
nand_format (void_t)
{
    uint_t  bads;

    bads = nandsm_format();
    return (bads);
}

/*
=======================================
    NAND 读取数据
=======================================
*/
CR_API retc_t
nand_read (
  __CR_IN__ int32u  addr,
  __CR_OT__ void_t* data,
  __CR_IN__ int16u  size
    )
{
    /* 直接一页一页地读 */
    addr /= NANDS_PAGE_SIZE;
    size /= NANDS_PAGE_SIZE;
    return (nandsm_read(addr, data, size));
}

/*
=======================================
    NAND 写入数据
=======================================
*/
CR_API retc_t
nand_write (
  __CR_IN__ int32u          addr,
  __CR_IN__ const void_t*   data,
  __CR_IN__ int16u          size
    )
{
    retc_t  rett;
    int32s  page = (int32s)(addr / NANDS_PAGE_SIZE);
    int16u  cnts = (int16u)(size / NANDS_PAGE_SIZE);

    if (cnts == 1)
    {
        /* 一页的做高速缓存 */
        if (s_last_page == -1)
        {
            /* 第一次发起连续写操作 */
            s_last_page  = page;
            s_start_page = page;
            memcpy(s_cache, data, size);
            s_total = s_ptr = size;
        }
        else
        {
            /* 判断是不是连续写操作 */
            if (page == s_last_page + 1)
            {
                /* 是连续写 */
                s_last_page += 1;
                memcpy(&s_cache[s_ptr], data, size);
                s_ptr   += size;
                s_total += size;
            }
            else
            {
                /* 不是连续写, 刷高速缓存 */
                rett = nandsm_write(s_start_page, s_cache,
                            s_ptr / NANDS_PAGE_SIZE);

                /* 一切从头开始 */
                s_last_page  = page;
                s_start_page = page;
                memcpy(s_cache, data, size);
                s_total = s_ptr = size;
                if (!rett)
                    return (FALSE);
            }
        }

        /* 判断是否要刷缓存 */
        if (s_total / NANDS_PAGE_SIZE == SCSI_BlkLen)
        {
            /* 一次连续写操作已经完成, 刷之 */
            s_last_page = -1;
            rett = nandsm_write(s_start_page, s_cache,
                        s_ptr / NANDS_PAGE_SIZE);
        }
        else if (s_ptr >= sizeof(s_cache))
        {
            /* 缓存已经放满, 但是写操作没有完成, 刷之 */
            rett = nandsm_write(s_start_page, s_cache,
                        s_ptr / NANDS_PAGE_SIZE);
            s_ptr = 0;  /* 缓存指针复位 */
            s_start_page = s_last_page + 1;
        }
        else
        {
            rett = TRUE;    /* 不用刷高速缓存 */
        }
    }
    else
    {
        /* 两页以上的直接写 */
        rett = nandsm_write(page, data, cnts);
    }
    return (rett);
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
