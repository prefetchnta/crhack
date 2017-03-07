/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2017-03-07  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> CrHack NOR 驱动函数库模板 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "memlib.h"

/* 临时使用的缓冲区 */
static byte_t   s_temp_buffer[NORFLASH_SECTOR_SIZE];

/*
---------------------------------------
    判断是否要擦除
---------------------------------------
*/
static retc_t
norflash_erase_int (
  __CR_IN__ nor_addr_t  addr
    )
{
    nor_leng_t  idx;

    /* 读取一个扇区判断是否需要擦除 */
    if (norflash_read(addr, s_temp_buffer, sizeof(s_temp_buffer)) == 0)
        return (FALSE);
    for (idx = 0; idx < sizeof(s_temp_buffer); idx++) {
        if (s_temp_buffer[idx] != 0xFF)
            break;
    }
    /* 需要先擦除扇区 */
    if (idx != sizeof(s_temp_buffer)) {
        if (!norflash_erase(addr, NORFLASH_ERASE_T1, NORFLASH_ERASE_T2))
            return (FALSE);
    }
    return (TRUE);
}

/*
---------------------------------------
    NOR 扇区编程
---------------------------------------
*/
static retc_t
norflash_write_int (
  __CR_IN__ nor_addr_t      addr,
  __CR_IN__ const void_t*   data
    )
{
#if defined(NORFLASH_RD_BACK_SIZE)
    uint_t  idx;
    byte_t  temp[NORFLASH_RD_BACK_SIZE];
#endif
    /* 写入数据 */
    if (data == NULL)
        data = s_temp_buffer;
    if (norflash_program(addr, data, sizeof(s_temp_buffer),
                         NORFLASH_PROGRAM_TOUT) == 0)
        return (FALSE);

    /* 回读比较 */
#if defined(NORFLASH_RD_BACK_SIZE)
    idx = sizeof(s_temp_buffer) / sizeof(temp);
    for (; idx != 0; idx--) {
        if (norflash_read(addr, temp, sizeof(temp)) == 0)
            return (FALSE);
        if (mem_cmp(temp, data, sizeof(temp)) != 0)
            return (FALSE);
        data = (byte_t*)data + sizeof(temp);
        addr += sizeof(temp);
    }
    idx = sizeof(s_temp_buffer) % sizeof(temp);
    if (idx != 0) {
        if (norflash_read(addr, temp, idx) == 0)
            return (FALSE);
        if (mem_cmp(temp, data, idx) != 0)
            return (FALSE);
    }
#endif
    return (TRUE);
}

/*
=======================================
    NOR 写任意数据
=======================================
*/
CR_API retc_t
norflash_write (
  __CR_IN__ nor_addr_t      addr,
  __CR_IN__ const void_t*   data,
  __CR_IN__ nor_leng_t      size
    )
{
    nor_addr_t  start;
    nor_leng_t  idx, rest;

    /* 对齐到扇区地址 */
    start = (addr / sizeof(s_temp_buffer)) * sizeof(s_temp_buffer);
    if (start != addr)
    {
        /* 改写数据并写入 */
        if (!norflash_erase_int(start))
            return (FALSE);
        addr -= start;
        rest = sizeof(s_temp_buffer) - addr;
        if (rest > size)
            rest = size;
        mem_cpy(&s_temp_buffer[addr], data, rest);
        if (!norflash_write_int(start, NULL))
            return (FALSE);

        /* 写入成功 */
        data = (byte_t*)data + rest;
        size  -= rest;
        start += sizeof(s_temp_buffer);
    }

    /* 是否写完 */
    if (size == 0)
        return (TRUE);

    /* 整扇区写过去 */
    idx = size / sizeof(s_temp_buffer);
    for (; idx != 0; idx--) {
        if (!norflash_erase(start, NORFLASH_ERASE_T1, NORFLASH_ERASE_T2))
            return (FALSE);
        if (!norflash_write_int(start, data))
            return (FALSE);
        data = (byte_t*)data + sizeof(s_temp_buffer);
        size  -= sizeof(s_temp_buffer);
        start += sizeof(s_temp_buffer);
    }

    /* 写入剩下的数据 */
    idx = size % sizeof(s_temp_buffer);
    if (idx != 0)
    {
        /* 改写数据并写入 */
        if (!norflash_erase_int(start))
            return (FALSE);
        mem_cpy(s_temp_buffer, data, idx);
        if (!norflash_write_int(start, NULL))
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
