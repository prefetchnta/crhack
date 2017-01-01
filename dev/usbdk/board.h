/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2012-02-01  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> USBDK U盘目标板配置头文件 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_BOARD_H__
#define __CR_BOARD_H__

#include "defs.h"

/*****************************************************************************/
/*                              接口 USB0 配置                               */
/*****************************************************************************/

CR_API void_t   usb0_init (void_t);

/*****************************************************************************/
/*                              接口 ITL0 配置                               */
/*****************************************************************************/

/* 连接 K9F1208X0C */
#if     defined(_USBDK_ITL0_)

    /***** 接口配置定义 *****/
    #define _INTEL_ADDR32_
    #define _INTEL_DATA08_
    #ifndef _USBDK_ECC_SOFT_
        #define ADBUS_HARDWARE      512
    #endif
    #define ADBUS_READ_CHECK    ibus_ecc_check();
    #define ADBUS_WRITE_CHECK   ibus_ecc_check();
    #define ADBUS_READ_START    ibus_ecc_start(size);
    #define ADBUS_WRITE_START   ibus_ecc_start(size);

#endif  /* _USBDK_ITL0_ */

CR_API void_t   ibus_init (void_t);
CR_API void_t   ibus_ecc_start (ufast_t size);
CR_API void_t   ibus_ecc_check (void_t);

#if defined(_USBDK_ITL0_)
    #include "adbus.h"
#endif

/*****************************************************************************/
/*                                 公用配置                                  */
/*****************************************************************************/

/* 地址的定义 */
#define SRAM16_SIZE     CR_M2B(1)
#define SRAM16_ADDR     0x68000000UL
#define K9F1208C_ADR    0x70020000UL
#define K9F1208C_CMD    0x70010000UL
#define K9F1208C_DAT    0x70000000UL

#endif  /* !__CR_BOARD_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
