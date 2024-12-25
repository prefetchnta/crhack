/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-11-30  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> DTM_NEXT 手持机操作头文件 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_MACHINE_H__
#define __CR_MACHINE_H__

#include "defs.h"

/*****************************************************************************/
/*                                 公用部分                                  */
/*****************************************************************************/

/* 南昌 GPS 设备数据结构 */
typedef struct
{
        int32u  longitude;      /* 经度 (分) */
        int32u  latitude;       /* 纬度 (分) */
        int16u  direction;      /* 方向 (度) */
        byte_t  velocity;       /* 速度 (km/h) */
        byte_t  gps_locate;     /* GPS 是否定位 */
        byte_t  gps_time[3];    /* 时间 (时分秒) */

} sGPS_NC;

CR_API bool_t   rs232_init (uint_t port, int32u baud, uint_t timeout);
CR_API bool_t   power_play_voice (uint_t port, byte_t index);
CR_API bool_t   gps_nc_get_data (uint_t port, sGPS_NC *gps_data);

/*****************************************************************************/
/*                              DTM_NEW 手持机                               */
/*****************************************************************************/

/* DTM_NEW 手持机串口命令字 */
#define NCMD_STATE      0x30    /* 手持机状态读取/设置 */
    #define NCMD_STATE_ONLINE       0x30            /* 手持机是否在线 */
    #define NCMD_STATE_VERSION      0x31            /* 回手持机版本串 */
    #define NCMD_STATE_ARESET       0x32            /* 手持机复位操作 */
    #define NCMD_STATE_ACCESS       0x33            /* 手持机密码验证 */
    #define NCMD_STATE_SETTIME      0x34            /* 手持机时间校准 */
    #define NCMD_STATE_GETTIME      0x35            /* 手持机时间读取 */
    #define NCMD_STATE_TCLEAR       0x36            /* 清设备训练数据 */
    #define NCMD_STATE_FNG_PIC      0x37            /* 获取指纹的图像 */
    #define NCMD_STATE_FNG_DES      0x38            /* 获取指纹的模板 */
    #define NCMD_STATE_FNG_CMP      0x39            /* 比对指纹的模板 */
/* ------------------------------------------------------------------------- */
#define NCMD_RAWEE      0x31    /* 手持机裸读/裸写机内地址 */
    #define NCMD_RAWEE_RD           0x30            /* 裸读机内的地址 */
    #define NCMD_RAWEE_WR           0x31            /* 裸写机内的地址 */
/* ------------------------------------------------------------------------- */
#define NCMD_RAWIC      0x32    /* 手持机裸读/裸写 IC 地址 */
    #define NCMD_RAWIC_RD           0x30            /* 裸读 IC 的地址 */
    #define NCMD_RAWIC_WR           0x31            /* 裸写 IC 的地址 */
    #define NCMD_RAWIC_CARD_24C     0x32            /* 设置为接触式卡 */
    #define NCMD_RAWIC_CARD_RFC     0x33            /* 设置为感应式卡 */
    #define NCMD_RAWIC_CARD_AUTO    0x34            /* 自动识别卡类型 */
/* ------------------------------------------------------------------------- */
#define NCMD_BINEE_RD   0x33    /* 读手持机内结构, 数据模式 */
#define NCMD_BINEE_WR   0x34    /* 写手持机内结构, 数据模式 */
#define NCMD_TXTEE_RD   0x35    /* 读手持机内结构, 文本模式 (包括 IC 卡) */
#define NCMD_TXTEE_WR   0x36    /* 写手持机内结构, 文本模式 (包括 IC 卡) */
#define NCMD_BINIC_RD   0x37    /* 读 IC 卡内结构, 直接模式 */
#define NCMD_BINIC_WR   0x38    /* 写 IC 卡内结构, 直接模式 */
/* ------------------------------------------------------------------------- */
#define NCMD_FLASH      0x39    /* 手持机的程序下载 */
    #define NCMD_FLASH_PARAM        0x30            /* 下载程序文件参数 */
    #define NCMD_FLASH_BLOCK        0x31            /* 下载程序的数据块 */
    #define NCMD_FLASH_BURNIT       0x32            /* 执行烧写程序指令 */
/* ------------------------------------------------------------------------- */
#define NCMD_MISCX      0x3B    /* 手持机的杂项功能 */
    #define NCMD_MISCX_INITCARD     0x30            /* 执行初始化卡的脚本 */
    #define NCMD_MISCX_SETMODES     0x31            /* 设置手持机工作状态 */
/* ------------------------------------------------------------------------- */
#define NCMD_LOG_READ   0x3F    /* 手持机读内部日志 */
    #define NCMD_LOG_READ_TOTAL     0x30            /* 读取手持机日志汇总 */
    #define NCMD_LOG_READ_ITEMS     0x31            /* 读取手持机日志明细 */
/* ------------------------------------------------------------------------- */

CR_API bool_t   dtm_new_online (uint_t port);
CR_API bool_t   dtm_new_areset (uint_t port);
CR_API bool_t   dtm_new_tclear (uint_t port);
CR_API ansi_t*  dtm_new_version (uint_t port);
CR_API bool_t   dtm_new_bin_read (uint_t port, byte_t type, int16u index,
                                  void_t *data, uint_t size);
CR_API bool_t   dtm_new_bin_write (uint_t port, byte_t type, int16u index,
                                   const void_t *data, int16u size);
CR_API bool_t   dtm_new_txt_read (uint_t port, const ansi_t *name,
                                  void_t *data, uint_t size);
CR_API bool_t   dtm_new_txt_write (uint_t port, const ansi_t *name,
                                   const void_t *data, int16u size);
CR_API uint_t   dtm_new_search (uint_t port_max, uint_t timeout, int32u *baud);
CR_API bool_t   dtm_new_download (uint_t port, int32u addr, const void_t *data,
                                  leng_t size, const sPROBAR *probar);

/*****************************************************************************/
/*                              DTM_OLD 手持机                               */
/*****************************************************************************/

/* DTM_OLD 手持机串口命令字 */
#define OCMD_BYTE1_ONLINE       0x61        /* 手持机是否在线1 */
#define OCMD_BYTE2_ONLINE       0x31        /* 手持机是否在线2 */
/* ------------------------------------------------------------------------- */
#define OCMD_BYTE1_VERSION      0x10        /* 回手持机版本串1 */
#define OCMD_BYTE2_VERSION      0x30        /* 回手持机版本串2 */
/* ------------------------------------------------------------------------- */
#define OCMD_BYTE1_ARESET       0x10        /* 手持机复位操作1 */
#define OCMD_BYTE2_ARESET       0x38        /* 手持机复位操作2 */
/* ------------------------------------------------------------------------- */
#define OCMD_BYTE1_ACCESS       0x30        /* 手持机密码验证1 */
#define OCMD_BYTE2_ACCESS       0x30        /* 手持机密码验证2 */
/* ------------------------------------------------------------------------- */
#define OCMD_BYTE1_FNG_PIC      0x80        /* 获取指纹的图像1 */
#define OCMD_BYTE2_FNG_PIC      0x31        /* 获取指纹的图像2 */
/* ------------------------------------------------------------------------- */
#define OCMD_BYTE1_FNG_CMP      0x82        /* 比对指纹的模板1 */
#define OCMD_BYTE2_FNG_CMP      0x33        /* 比对指纹的模板2 */
/* ------------------------------------------------------------------------- */
#define OCMD_BYTE1_FNG_DES      0x83        /* 获取指纹的模板1 */
#define OCMD_BYTE2_FNG_DES      0x33        /* 获取指纹的模板2 */
/* ------------------------------------------------------------------------- */
#define OCMD_BYTE1_BOOTLDR      0x20        /* 进入下载的状态 */
#define OCMD_BYTE2_BOOTLDR      0x00        /* 进入下载的状态 */
#define OCMD_BYTE1_BOOTLDR1     0x2A        /* 进入下载的状态1 */
#define OCMD_BYTE2_BOOTLDR1     0xA0        /* 进入下载的状态2 */
#define OCMD_BYTE1_BOOTLDR2     0x20        /* 进入下载的状态1 */
#define OCMD_BYTE2_BOOTLDR2     0x30        /* 进入下载的状态2 */
/* ------------------------------------------------------------------------- */
#define OCMD_BYTE1_DPACKET      0x20        /* 下载程序分块包 */
#define OCMD_BYTE2_DPACKET      0x01        /* 下载程序分块包 */
#define OCMD_BYTE1_DPACKET1     0x2A        /* 下载程序分块包1 */
#define OCMD_BYTE2_DPACKET1     0xA1        /* 下载程序分块包2 */
#define OCMD_BYTE1_DPACKET2     0x20        /* 下载程序分块包1 */
#define OCMD_BYTE2_DPACKET2     0x31        /* 下载程序分块包2 */
/* ------------------------------------------------------------------------- */
#define OCMD_BYTE1_WRFLASH      0x20        /* 执行程序的烧写 */
#define OCMD_BYTE2_WRFLASH      0x02        /* 执行程序的烧写 */
#define OCMD_BYTE1_WRFLASH1     0x2A        /* 执行程序的烧写1 */
#define OCMD_BYTE2_WRFLASH1     0xA2        /* 执行程序的烧写2 */
#define OCMD_BYTE1_WRFLASH2     0x20        /* 执行程序的烧写1 */
#define OCMD_BYTE2_WRFLASH2     0x32        /* 执行程序的烧写2 */
/* ------------------------------------------------------------------------- */

/* DTM_OLD 手持机下载类型 */
#define DTM_OLD_DOWN_AT9_BOOT   0x01    /* AT91 系列启动代码 */
#define DTM_OLD_DOWN_AT9_MAIN   0x02    /* AT91 系列程序字库 */
#define DTM_OLD_DOWN_DSP_MAIN   0x03    /* BLACKFIN 系列程序 */
#define DTM_OLD_DOWN_DSP_ZK12   0x04    /* BLACKFIN 系列字库 */

CR_API bool_t   dtm_old_areset (uint_t port);
CR_API ansi_t*  dtm_old_version (uint_t port);
CR_API uint_t   dtm_old_search (uint_t port_max, uint_t timeout, int32u *baud);
CR_API bool_t   dtm_old_download (uint_t port, byte_t type, const void_t *data,
                              leng_t size, leng_t shzk, const sPROBAR *probar);

#endif  /* !__CR_MACHINE_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
