/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2011-09-20  */
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
/*  >>>>>>>>>>>>>>>>> LLB3 MLR 流量结算仪目标板配置头文件 <<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_BOARD_H__
#define __CR_BOARD_H__

#include "msclib.h"
#include "intel5x.h"
#include "fw-mcs51.h"

/*****************************************************************************/
/*                              接口 SPI0 配置                               */
/*****************************************************************************/

/* 连接 X5045 */
#if     defined(_LLB3_SPI0_)

    /***** 管脚操作定义 *****/
    #define spi_leng_t          leng_t
    #define SPI_MISO_GETB       b_P1_3
    #define SPI_MISO_SETB       b_P1_3 = 1;
    #define SPI_MISO_CLRB       b_P1_3 = 0;
    #define SPI_SSEL_SETB       b_P1_2 = 1;
    #define SPI_SSEL_CLRB       b_P1_2 = 0;
    #define SPI_MOSI_SETB       b_P1_6 = 1;
    #define SPI_MOSI_CLRB       b_P1_6 = 0;
    #define SPI_SCLK_SETB       b_P1_5 = 1;
    #define SPI_SCLK_CLRB       b_P1_5 = 0;
    #define SPI_SCLK_FLIP       b_P1_5 = ~b_P1_5;
    #define SPI_SCLK_DELAY12

    /***** 函数的重映射 *****/
    #define spi_init            spi0_init
    #define spi_send_data       spi0_send_data
    #define spi_send_recv       spi0_send_recv
    #define spi_send_send       spi0_send_send

#endif  /* _LLB3_SPI0_ */

CR_API void_t   spi0_init (void_t);
CR_API void_t   spi0_send_data (const void_t *data, leng_t size);
CR_API void_t   spi0_send_recv (void_t *recv_data, leng_t recv_size,
                          const void_t *send_data, leng_t send_size);
CR_API void_t   spi0_send_send (const void_t *send1_data, leng_t send1_size,
                                const void_t *send2_data, leng_t send2_size);

/*****************************************************************************/
/*                              接口 SPI1 配置                               */
/*****************************************************************************/

/* 连接 TLC5615 */
#if     defined(_LLB3_SPI1_)

    /***** 管脚操作定义 *****/
    #define SPI_MISO_GETB       0
    #define SPI_MISO_SETB
    #define SPI_MISO_CLRB
    #define SPI_SSEL_SETB       b_P3_5 = 1;
    #define SPI_SSEL_CLRB       b_P3_5 = 0;
    #define SPI_MOSI_SETB       b_P3_3 = 1;
    #define SPI_MOSI_CLRB       b_P3_3 = 0;
    #define SPI_SCLK_SETB       b_P3_4 = 1;
    #define SPI_SCLK_CLRB       b_P3_4 = 0;
    #define SPI_SCLK_FLIP       b_P3_4 = ~b_P3_4;
    #define SPI_SCLK_DELAY12

    /***** 函数的重映射 *****/
    #define spi_init            spi1_init
    #define spi_send_data       spi1_send_data

#endif  /* _LLB3_SPI1_ */

CR_API void_t   spi1_init (void_t);
CR_API void_t   spi1_send_data (const void_t *data, ufast_t size);

/*****************************************************************************/
/*                              接口 IBT0 配置                               */
/*****************************************************************************/

/* 连接 DS18B20 */
#if     defined(_LLB3_IBT0_)

    /***** 管脚操作定义 *****/
    #define WIRE1_DQ_GETB       b_P1_1
    #define WIRE1_DQ_SETB       b_P1_1 = 1;
    #define WIRE1_DQ_CLRB       b_P1_1 = 0;

    /***** 函数的重映射 *****/
    #define wire1_init          ibt0_init
    #define wire1_reset         ibt0_reset
    #define wire1_recv_byte     ibt0_recv_byte
    #define wire1_send_byte     ibt0_send_byte

#endif  /* _LLB3_IBT0_ */

CR_API void_t   ibt0_init  (void_t);
CR_API retc_t   ibt0_reset (void_t);
CR_API byte_t   ibt0_recv_byte (void_t);
CR_API void_t   ibt0_send_byte (byte_t val);

/*****************************************************************************/
/*                              接口 MOT0 配置                               */
/*****************************************************************************/

/* 连接 KS0108 (左) */
#if     defined(_LLB3_MOT0_)

    /***** 管脚操作定义 *****/
    #define _MOTOL_ADDR08_
    #define _MOTOL_DATA08_
    #define MOTOL_DAT_RD()      r_P0
    #define MOTOL_CSE_SETB    { b_P2_0 = 0; \
                                b_P2_1 = 0; }
    #define MOTOL_CSE_CLRB      b_P2_0 = 1;     /* 高电平片选有效 */
    #define MOTOL_DLE_SETB      b_P2_2 = 1;
    #define MOTOL_DLE_CLRB      b_P2_2 = 0;
    #define MOTOL_RNW_SETB      b_P2_7 = 1;
    #define MOTOL_RNW_CLRB      b_P2_7 = 0;
    #define MOTOL_DAT_DIRI      r_P0 = (255);
    #define MOTOL_DAT_WR(dat)   r_P0 = (dat);
    #define MOTOL_ADR_WR(adr)   b_P2_6 = (adr) & 1;

    /***** 函数的重映射 *****/
    #define mbus_init           mbus_init
    #define mbus_chip_sel       mot0_chip_sel
    #define mbus_chip_out       mbus_chip_out
    #define mbus_data_set       mbus_data_set
    #define mbus_data_get       mbus_data_get
    #define mbus_data_write     mbus_data_write

    /***** 函数的重映射 *****/
    #define ks0108_set_dx       ks0108l_set_dx
    #define ks0108_set_dy       ks0108l_set_dy
    #define ks0108_set_xy       ks0108l_set_xy
    #define ks0108_set_start    ks0108l_set_start
    #define ks0108_read_dot     ks0108l_read_dot
    #define ks0108_write_dot    ks0108l_write_dot
    #define ks0108_write_line   ks0108l_write_line
    #define ks0108_pixel_get    ks0108l_pixel_get
    #define ks0108_pixel_set    ks0108l_pixel_set
    #define ks0108_wait_idle    ks0108l_wait_idle
    #define ks0108_get_status   ks0108l_get_status
    #define ks0108_lcd_switch   ks0108l_lcd_switch

#endif  /* _LLB3_MOT0_ */

CR_API void_t   mbus_init (void_t);
CR_API void_t   mot0_chip_sel (void_t);
CR_API void_t   mbus_chip_out (void_t);
CR_API byte_t   mbus_data_get (byte_t addr);
CR_API void_t   mbus_data_set (byte_t addr, byte_t data);
CR_API void_t   mbus_data_write (byte_t addr, const byte_t *data,
                                 ufast_t size);

/*****************************************************************************/
/*                              接口 MOT1 配置                               */
/*****************************************************************************/

/* 连接 KS0108 (右) */
#if     defined(_LLB3_MOT1_)

    /***** 管脚操作定义 *****/
    #define _MOTOL_ADDR08_
    #define _MOTOL_DATA08_
    #define MOTOL_CSE_CLRB      b_P2_1 = 1;     /* 高电平片选有效 */

    /***** 函数的重映射 *****/
    #define mbus_chip_sel       mot1_chip_sel

    /***** 函数的重映射 *****/
    #define ks0108_set_dx       ks0108r_set_dx
    #define ks0108_set_dy       ks0108r_set_dy
    #define ks0108_set_xy       ks0108r_set_xy
    #define ks0108_set_start    ks0108r_set_start
    #define ks0108_read_dot     ks0108r_read_dot
    #define ks0108_write_dot    ks0108r_write_dot
    #define ks0108_write_line   ks0108r_write_line
    #define ks0108_pixel_get    ks0108r_pixel_get
    #define ks0108_pixel_set    ks0108r_pixel_set
    #define ks0108_wait_idle    ks0108r_wait_idle
    #define ks0108_get_status   ks0108r_get_status
    #define ks0108_lcd_switch   ks0108r_lcd_switch

#endif  /* _LLB3_MOT1_ */

CR_API void_t   mot1_chip_sel (void_t);

/*****************************************************************************/
/*                                 公用配置                                  */
/*****************************************************************************/

/* 地址的定义 */
#define KS0108_CMD  0x00
#define KS0108_DAT  0x01

/* 驱动宏定义 */
#define X5XXX_WDT_NOP       { CR_NOP; }
#define MOTOL_DELAY_TM_WSH  { CR_NOP; }
#define MOTOL_DELAY_TM_WSL  { CR_NOP; }
#define MOTOL_DELAY_TM_RSH  { CR_NOP; }
#define MOTOL_DELAY_TM_RSL  { CR_NOP; }
#define WIRE1_DELAY___1_US  { CR_NOP; }
#define WIRE1_DELAY__60_US  delay08(( 60 - 8) / 6 + 1);
#define WIRE1_DELAY_240_US  delay08((240 - 8) / 6 + 1);
#define WIRE1_DELAY_480_US  delay08((480 - 8) / 6 + 1);

/* 函数库裁剪 */
#define __no_uart0_read
#define __no_uart0_write
#define __no_x5xxx_lock
#define __no_x5xxx_set_status
#define __no_ds18b20_ram_write

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
