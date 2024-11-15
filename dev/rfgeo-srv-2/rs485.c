/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2017-03-14  */
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
/*  >>>>>>>>>>>>>>>>> RFGEO-SRV-2 采集器 RS485 驱动函数库 <<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "device.h"
#include "stm32f10x_conf.h"

/* 环形队列大小 */
#define RX_SIZE CR_K2B(4)

/* 函数的重映射 */
#define uart_zero       rs485_zero
#define uart_rx_size    bridge_rs485_rx_size
#define uart_rx_flush   bridge_rs485_rx_flush
#define uart_throw      bridge_rs485_throw
#define uart_peek       bridge_rs485_peek
#define uart_read       bridge_rs485_read
#define uart_input      bridge_rs485_input
#define uart_wait       bridge_rs485_wait

/* 等待中的空闲 */
#define USART_YIELD     wdt_task();

#include "uart.inl"

/* 是否通过桥接板 */
extern bool_t   g_is_bridge;

/*
=======================================
    RS485 设置波特率
=======================================
*/
CR_API bool_t
rs485_baud (
  __CR_IN__ int32u  baud
    )
{
    /* 只支持桥接板 */
    if (!g_is_bridge)
        return (FALSE);
    return (bridge_baud(CR_BRIDGE_PORT_485, baud));
}

/*
=======================================
    RS485 发送
=======================================
*/
CR_API void_t
rs485_write (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    /* 只支持桥接板 */
    if (!g_is_bridge)
        return;
    bridge_commit(CR_BRIDGE_PORT_485, data, size);
}

/*
=======================================
    RS485 读取缓冲区内数据长度
=======================================
*/
CR_API uint_t
rs485_rx_size (void_t)
{
    /* 只支持桥接板 */
    if (!g_is_bridge)
        return (0);
    return (bridge_rs485_rx_size());
}

/*
=======================================
    RS485 清空接收缓冲区
=======================================
*/
CR_API void_t
rs485_rx_flush (void_t)
{
    /* 只支持桥接板 */
    if (!g_is_bridge)
        return;
    bridge_rs485_rx_flush();
}

/*
=======================================
    RS485 丢掉接收缓冲一组字节
=======================================
*/
CR_API void_t
rs485_throw (
  __CR_IN__ uint_t  size
    )
{
    /* 只支持桥接板 */
    if (!g_is_bridge)
        return;
    bridge_rs485_throw(size);
}

/*
=======================================
    RS485 偷窥缓冲区数据
=======================================
*/
CR_API uint_t
rs485_peek (
  __CR_OT__ void_t* data,
  __CR_IN__ uint_t  size
    )
{
    /* 只支持桥接板 */
    if (!g_is_bridge)
        return (0);
    return (bridge_rs485_peek(data, size));
}

/*
=======================================
    RS485 读取缓冲区数据
=======================================
*/
CR_API uint_t
rs485_read (
  __CR_OT__ void_t* data,
  __CR_IN__ uint_t  size
    )
{
    /* 只支持桥接板 */
    if (!g_is_bridge)
        return (0);
    return (bridge_rs485_read(data, size));
}

/*
=======================================
    RS485 等待接收数据
=======================================
*/
CR_API uint_t
rs485_wait (
  __CR_OT__ void_t* data,
  __CR_IN__ uint_t  step,
  __CR_IN__ uint_t  tout
    )
{
    /* 只支持桥接板 */
    if (!g_is_bridge)
        return (0);
    return (bridge_rs485_wait(data, step, tout));
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
