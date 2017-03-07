/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2017-03-05  */
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
/*  >>>>>>>>>>>>>>>>>> YTJ 一体机 LED 屏储存器读写函数库 <<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "ff.h"
#include "device.h"
#include "stm32f10x_conf.h"

/* NOR FLASH 配置 */
#define nor_addr_t          int32u
#define nor_leng_t          leng_t
#define norflash_read       mx25l16_fread
#define norflash_erase      mx25l16_ssec_erase
#define norflash_program    mx25l16_program
/* ------------------------------------- */
#define NORFLASH_ERASE_T1       1
#define NORFLASH_ERASE_T2       0x7FFFFFFFUL
#define NORFLASH_PROGRAM_TOUT   0x7FFFFFFFUL
#define NORFLASH_RD_BACK_SIZE   512
#define NORFLASH_SECTOR_SIZE    MX25LXX_SSEC_SIZE
#include "norflash.inl"

/* FatFS 上下文 */
static FATFS    s_fs;

/*
=======================================
    文件系统初始化
=======================================
*/
CR_API retc_t
store_init (void_t)
{
    byte_t  work[_MAX_SS * 2];

    /* 初始化文件系统 */
    if (f_mount(&s_fs, "", 1) != FR_OK) {
        if (f_mkfs("", FM_FAT, 0, work, sizeof(work)) != FR_OK)
            return (FALSE);
        if (f_mount(&s_fs, "", 1) != FR_OK)
            return (FALSE);
    }
    return (TRUE);
}

/*
=======================================
    检查储存芯片
=======================================
*/
CR_API retc_t
store_check (void_t)
{
    sMX25LXX3   info = { 0 };

    /* 只检查容量对不对 */
    mx25l16_read_id3(&info);
    if (info.cap_sz == 0x15)
        return (TRUE);
    return (FALSE);
}

/*
=======================================
    读取储存器
=======================================
*/
CR_API retc_t
store_read (
  __CR_IN__ int32u  addr,
  __CR_OT__ void_t* data,
  __CR_IN__ leng_t  size
    )
{
    mx25l16_wrdi();
    if (mx25l16_fread(addr, data, size) != 0)
        return (TRUE);
    return (FALSE);
}

/*
=======================================
    写入储存器
=======================================
*/
CR_API retc_t
store_write (
  __CR_IN__ int32u          addr,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    ufast_t idx;

    mx25l16_wren();
    for (idx = 0; idx < 3; idx++) {
        if (norflash_write(addr, data, size)) {
            mx25l16_wrdi();
            return (TRUE);
        }
    }
    mx25l16_wrdi();
    return (FALSE);
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
