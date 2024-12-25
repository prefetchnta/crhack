/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-11-11  */
/*     #######          ###    ###      [SOFT]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>>>>> DTM_NEXT ICCARD 设备驱动头文件 <<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_ICCARD_H__
#define __CR_ICCARD_H__

#include "devlib.h"

/*****************************************************************************/
/*                                驱动控制码                                 */
/*****************************************************************************/

#define ICC_UNIV_SET_PORT   0x0000      /* 设置端口 (默认0x1) */
#define ICC_DCIC_SET_PORT   0x0000      /* 设置端口 (默认100) */
#define ICC_DCRF_SET_PORT   0x0000      /* 设置端口 (默认100) */
#define ICC_DCRF_SET_BAUD   0x0001      /* 设置速度 (默认9600) */
#define ICC_DCRF_SET_RKEY   0x0002      /* 设置 M1 卡访问密钥 */
#define ICC_DCRF_SET_SKEY   0x0003      /* 设置 M1 卡扇区密钥 */
#define ICC_AUTO_SET_PORT   0xFFFF      /* 自动设置端口和速度 */

/* M1 卡密钥控制结构 */
typedef struct
{
        uint_t  sec_id;         /* 扇区编号 */
        byte_t  akey[6];        /* 扇区密钥A */
        byte_t  bkey[6];        /* 扇区密钥B */
        byte_t  b0, b1;         /* 扇区控制字 */
        byte_t  b2, b3, bk;     /* 扇区控制字 */

} sICC_DCRF_M1_KEY;

/*****************************************************************************/
/*                                驱动错误码                                 */
/*****************************************************************************/

/* DISK ICC 驱动出错代码值 */
#define CR_DEVB_ERR_ICC_DISK_SEEK   CR_ERRS(0x5000UL)   /* 文件定位出错 */
#define CR_DEVB_ERR_ICC_DISK_FLUSH  CR_ERRS(0x5001UL)   /* 文件刷新出错 */

/* DCRF ICC 驱动出错代码值 */
#define CR_DEVB_ERR_ICC_DCRF_OPEN   CR_ERRS(0x5100UL)   /* 设备打开失败 */
#define CR_DEVB_ERR_ICC_DCRF_FIND   CR_ERRS(0x5101UL)   /* 设备寻卡失败 */
#define CR_DEVB_ERR_ICC_DCRF_PASS   CR_ERRS(0x5102UL)   /* 密码认证失败 */
#define CR_DEVB_ERR_ICC_DCRF_NSEC   CR_ERRS(0x5103UL)   /* 非法的扇区号 */
#define CR_DEVB_ERR_ICC_DCRF_SKEY   CR_ERRS(0x5104UL)   /* 修改密码失败 */
#define CR_DEVB_ERR_ICC_DCRF_READ   CR_ERRS(0x5105UL)   /* 设备读取失败 */
#define CR_DEVB_ERR_ICC_DCRF_WRITE  CR_ERRS(0x5106UL)   /* 设备写入失败 */
#define CR_DEVB_ERR_ICC_DCRF_STYPE  CR_ERRS(0x5107UL)   /* 设置卡型失败 */

/* DCIC ICC 驱动出错代码值 */
#define CR_DEVB_ERR_ICC_DCIC_OPEN   CR_ERRS(0x5100UL)   /* 设备打开失败 */
#define CR_DEVB_ERR_ICC_DCIC_FIND   CR_ERRS(0x5101UL)   /* 设备寻卡失败 */
#define CR_DEVB_ERR_ICC_DCIC_PASS   CR_ERRS(0x5102UL)   /* 密码认证失败 */
#define CR_DEVB_ERR_ICC_DCIC_NSEC   CR_ERRS(0x5103UL)   /* 非法的扇区号 */
#define CR_DEVB_ERR_ICC_DCIC_SKEY   CR_ERRS(0x5104UL)   /* 修改密码失败 */
#define CR_DEVB_ERR_ICC_DCIC_READ   CR_ERRS(0x5105UL)   /* 设备读取失败 */
#define CR_DEVB_ERR_ICC_DCIC_WRITE  CR_ERRS(0x5106UL)   /* 设备写入失败 */
#define CR_DEVB_ERR_ICC_DCIC_STYPE  CR_ERRS(0x5107UL)   /* 设置卡型失败 */

/* UNIV ICC 驱动出错代码值 */
#define CR_DEVB_ERR_ICC_UNIV_OPEN   CR_ERRS(0x5100UL)   /* 设备打开失败 */
#define CR_DEVB_ERR_ICC_UNIV_READ   CR_ERRS(0x5105UL)   /* 设备读取失败 */
#define CR_DEVB_ERR_ICC_UNIV_WRITE  CR_ERRS(0x5106UL)   /* 设备写入失败 */
#define CR_DEVB_ERR_ICC_UNIV_STYPE  CR_ERRS(0x5107UL)   /* 设置卡型失败 */
#define CR_DEVB_ERR_ICC_UNIV_FLMEM  CR_ERRS(0x5108UL)   /* 内存分配失败 */

/*****************************************************************************/
/*                                驱动注册表                                 */
/*****************************************************************************/

/* DISK ICC 驱动注册表 */
CR_API const sDEV_BNODE _rom_ g_icc_disk_s50;       /* S50 */
CR_API const sDEV_BNODE _rom_ g_icc_disk_s70;       /* S70 */
CR_API const sDEV_BNODE _rom_ g_icc_disk_24c64;     /* 24C64 */
CR_API const sDEV_BNODE _rom_ g_icc_disk_24c128;    /* 24C128 */

/* DRAM ICC 驱动注册表 */
CR_API const sDEV_BNODE _rom_ g_icc_dram_s50;       /* S50 */
CR_API const sDEV_BNODE _rom_ g_icc_dram_s70;       /* S70 */
CR_API const sDEV_BNODE _rom_ g_icc_dram_24c64;     /* 24C64 */
CR_API const sDEV_BNODE _rom_ g_icc_dram_24c128;    /* 24C128 */

/* DCRF ICC 驱动注册表 */
CR_API const sDEV_BNODE _rom_ g_icc_dcrf_s50;       /* S50 */
CR_API const sDEV_BNODE _rom_ g_icc_dcrf_s70;       /* S70 */
CR_API const sDEV_BNODE _rom_ g_icc_dcrf_24c64;     /* 24C64 */
CR_API const sDEV_BNODE _rom_ g_icc_dcrf_24c128;    /* 24C128 */

/* DCIC ICC 驱动注册表 */
CR_API const sDEV_BNODE _rom_ g_icc_dcic_24c64;     /* 24C64 */
CR_API const sDEV_BNODE _rom_ g_icc_dcic_24c128;    /* 24C128 */

/* UNIV ICC 驱动注册表 */
CR_API const sDEV_BNODE _rom_ g_icc_univ_s50;       /* S50 */
CR_API const sDEV_BNODE _rom_ g_icc_univ_s70;       /* S70 */
CR_API const sDEV_BNODE _rom_ g_icc_univ_24c64;     /* 24C64 */
CR_API const sDEV_BNODE _rom_ g_icc_univ_24c128;    /* 24C128 */

#endif  /* !__CR_ICCARD_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
