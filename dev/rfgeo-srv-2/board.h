/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2017-03-09  */
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
/*  >>>>>>>>>>>>>>>>> RFGEO-SRV-2 采集器目标板配置头文件 <<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_BOARD_H__
#define __CR_BOARD_H__

#include "defs.h"

/*****************************************************************************/
/*                              接口 SPI0 配置                               */
/*****************************************************************************/

/* 连接 MX25L1606E */
#if     defined(_SRV2_SPI0_)

    /***** 管脚操作定义 *****/
    #define spi_leng_t          leng_t

    /***** 函数的重映射 *****/
    #define spi_init            spi0_init
    #define spi_send_data       spi0_send_data
    #define spi_send_recv       spi0_send_recv
    #define spi_send_send       spi0_send_send

#endif  /* _SRV2_SPI0_ */

CR_API void_t   spi0_init (void_t);
CR_API void_t   spi0_send_data (const void_t *data, leng_t size);
CR_API void_t   spi0_send_recv (void_t *recv_data, leng_t recv_size,
                          const void_t *send_data, leng_t send_size);
CR_API void_t   spi0_send_send (const void_t *send1_data, leng_t send1_size,
                                const void_t *send2_data, leng_t send2_size);

/*****************************************************************************/
/*                              接口 GPIO 配置                               */
/*****************************************************************************/

CR_API void_t   gpio_init (void_t);

/*****************************************************************************/
/*                                 公用配置                                  */
/*****************************************************************************/

CR_API void_t   nvic_init (void_t);

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
