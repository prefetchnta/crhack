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

#include "memlib.h"
#include "msclib.h"

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
/*                              接口 SPI1 配置                               */
/*****************************************************************************/

/* 连接 SX1278 (M1) */
#if     defined(_SRV2_SPI1_)

    /***** 管脚操作定义 *****/
    #define spi_leng_t          leng_t

    /***** 函数的重映射 *****/
    #define spi_init            spi1_init
    #define spi_send_data       spi1_send_data
    #define spi_send_recv       spi1_send_recv
    #define spi_send_send       spi1_send_send

#endif  /* _SRV2_SPI1_ */

CR_API void_t   spi1_init (void_t);
CR_API void_t   spi1_send_data (const void_t *data, leng_t size);
CR_API void_t   spi1_send_recv (void_t *recv_data, leng_t recv_size,
                          const void_t *send_data, leng_t send_size);
CR_API void_t   spi1_send_send (const void_t *send1_data, leng_t send1_size,
                                const void_t *send2_data, leng_t send2_size);

/*****************************************************************************/
/*                              接口 SPI2 配置                               */
/*****************************************************************************/

/* 连接 SX1278 (M2) */
#if     defined(_SRV2_SPI2_)

    /***** 管脚操作定义 *****/
    #define spi_leng_t          leng_t

    /***** 函数的重映射 *****/
    #define spi_init            spi2_init
    #define spi_send_data       spi2_send_data
    #define spi_send_recv       spi2_send_recv
    #define spi_send_send       spi2_send_send

#endif  /* _SRV2_SPI2_ */

CR_API void_t   spi2_init (void_t);
CR_API void_t   spi2_send_data (const void_t *data, leng_t size);
CR_API void_t   spi2_send_recv (void_t *recv_data, leng_t recv_size,
                          const void_t *send_data, leng_t send_size);
CR_API void_t   spi2_send_send (const void_t *send1_data, leng_t send1_size,
                                const void_t *send2_data, leng_t send2_size);

/*****************************************************************************/
/*                              接口 I2C0 配置                               */
/*****************************************************************************/

/* 连接 RX-8025 */
#if     defined(_SRV2_I2C0_)

    /***** 管脚操作定义 *****/
    #define _I2C_256B_
    #define _I2C_INIT_          i2c0_gpio_init();
    #define I2C_SDA_DIRO        GPIOB->CRH |=  (0x06 << 12);
    #define I2C_SDA_DIRI        GPIOB->CRH &= ~(0x03 << 12);
    #define I2C_SCL_DIRO        GPIOB->CRH |=  (0x06 <<  8);
    #define I2C_SDA_GETB        GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11)
    #define I2C_SDA_SETB        GPIOB->BSRR = GPIO_Pin_11;
    #define I2C_SDA_CLRB        GPIOB->BRR = GPIO_Pin_11;
    #define I2C_SCL_SETB        GPIOB->BSRR = GPIO_Pin_10;
    #define I2C_SCL_CLRB        GPIOB->BRR = GPIO_Pin_10;

    /***** 函数的重映射 *****/
    #define i2c_init            i2c0_init
    #define i2c_enter           i2c0_enter
    #define i2c_leave           i2c0_leave
    #define i2c_send_ack        i2c0_send_ack
    #define i2c_send_nack       i2c0_send_nack
    #define i2c_test_ack        i2c0_test_ack
    #define i2c_recv_byte       i2c0_recv_byte
    #define i2c_send_byte       i2c0_send_byte
    #define i2c_send_slave      i2c0_send_slave
    #define i2c_send_address    i2c0_send_address
    #define i2c_read_device     i2c0_read_device
    #define i2c_write_device    i2c0_write_device

#endif  /* _SRV2_I2C0_ */

CR_API void_t   i2c0_init (void_t);
CR_API byte_t   i2c0_read_device (byte_t devs, byte_t addr,
                                  void_t *data, byte_t size);
CR_API byte_t   i2c0_write_device (byte_t devs, byte_t addr,
                            const void_t *data, byte_t size);

/*****************************************************************************/
/*                              接口 GPIO 配置                               */
/*****************************************************************************/

#define ctl_set()   GPIOA->BSRR = GPIO_Pin_1
#define ctl_clr()   GPIOA->BRR = GPIO_Pin_1
#define oc1_set()   GPIOE->BSRR = GPIO_Pin_2
#define oc1_clr()   GPIOE->BRR = GPIO_Pin_2
#define oc2_set()   GPIOE->BSRR = GPIO_Pin_3
#define oc2_clr()   GPIOE->BRR = GPIO_Pin_3
#define oc3_set()   GPIOE->BSRR = GPIO_Pin_4
#define oc3_clr()   GPIOE->BRR = GPIO_Pin_4
#define oc4_set()   GPIOE->BSRR = GPIO_Pin_5
#define oc4_clr()   GPIOE->BRR = GPIO_Pin_5
#define led_xon()   GPIOE->BSRR = GPIO_Pin_6
#define led_off()   GPIOE->BRR = GPIO_Pin_6
CR_API void_t   gpio_init (void_t);
CR_API byte_t   gpio_input (void_t);
CR_API void_t   gpio_output (byte_t out);

/*****************************************************************************/
/*                                 公用配置                                  */
/*****************************************************************************/

CR_API void_t   nvic_init (void_t);

/* 异步串口0 */
CR_API bool_t   uart0_init (int32u baud);
CR_API void_t   uart0_write (const void_t *data, leng_t size);
#define uart0_send_str(str) uart0_write(str, str_lenA(str))
CR_API void_t   uart0_zero (void_t);
CR_API uint_t   uart0_rx_size (void_t);
CR_API void_t   uart0_rx_flush (void_t);
CR_API void_t   uart0_throw (uint_t size);
CR_API uint_t   uart0_peek (void_t *data, uint_t size);
CR_API uint_t   uart0_read (void_t *data, uint_t size);
CR_API uint_t   uart0_wait (void_t *data, uint_t step, uint_t tout);

/* 驱动宏定义 */
#define I2C_DELAY_GET_EX    delay32(20);
#define I2C_DELAY_4___US    delay32(20);
#define I2C_DELAY_4_7_US    delay32(20);

/* 调试相关的配置 */
#include <stdio.h>
#if defined(SRV2_DEBUG)
    #define WDT_FEED
    #define WDT_TOUT(t)
    #define DBG_PRINT   printf
#else
    #define WDT_FEED    wdg_feed(0)
    #define WDT_TOUT(t) wdg_timeout(0, t)
    #define DBG_PRINT(...)
#endif

/* LED & 喂狗 */
CR_API void_t   wdt_task (void_t);

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
