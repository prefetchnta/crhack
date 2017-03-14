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
/*  >>>>>>>>>>>>>>>>> RFGEO-SRV-2 采集器 RS232 驱动函数库 <<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "device.h"

/* 环形队列大小 */
#define RX_SIZE CR_K2B(12)

/* 函数的重映射 */
#define uart_rx_size    bridge_rs232_rx_size
#define uart_rx_flush   bridge_rs232_rx_flush
#define uart_throw      bridge_rs232_throw
#define uart_peek       bridge_rs232_peek
#define uart_read       bridge_rs232_read

#include "uart.inl"

/* 是否通过桥接板 */
extern bool_t   g_is_bridge;

/*
=======================================
    RS232 设置波特率
=======================================
*/
CR_API bool_t
rs232_baud (
  __CR_IN__ int32u  baud
    )
{
    bool_t  retc;

    if (g_is_bridge)
    {
        /* 走桥接板 */
        retc = bridge_baud(PORT_TYPE_PSS, baud);
    }
    else
    {
        /* 走本地接口 */
        retc = uart0_init(baud);
    }
    return (retc);
}

/*
=======================================
    RS232 发送
=======================================
*/
CR_API void_t
rs232_write (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    if (g_is_bridge)
    {
        /* 走桥接板 */
        bridge_commit(PORT_TYPE_PSS, data, size);
    }
    else
    {
        /* 走本地接口 */
        uart0_write(data, size);
    }
}

/*
=======================================
    RS232 读取缓冲区内数据长度
=======================================
*/
CR_API uint_t
rs232_rx_size (void_t)
{
    uint_t  size;

    if (g_is_bridge)
    {
        /* 走桥接板 */
        size = bridge_rs232_rx_size();
    }
    else
    {
        /* 走本地接口 */
        size = uart0_rx_size();
    }
    return (size);
}

/*
=======================================
    RS232 清空接收缓冲区
=======================================
*/
CR_API void_t
rs232_rx_flush (void_t)
{
    if (g_is_bridge)
    {
        /* 走桥接板 */
        bridge_rs232_rx_flush();
    }
    else
    {
        /* 走本地接口 */
        uart0_rx_flush();
    }
}

/*
=======================================
    RS232 丢掉接收缓冲一组字节
=======================================
*/
CR_API void_t
rs232_throw (
  __CR_IN__ uint_t  size
    )
{
    if (g_is_bridge)
    {
        /* 走桥接板 */
        bridge_rs232_throw(size);
    }
    else
    {
        /* 走本地接口 */
        uart0_throw(size);
    }
}

/*
=======================================
    RS232 偷窥缓冲区数据
=======================================
*/
CR_API uint_t
rs232_peek (
  __CR_OT__ void_t* data,
  __CR_IN__ uint_t  size
    )
{
    uint_t  back;

    if (g_is_bridge)
    {
        /* 走桥接板 */
        back = bridge_rs232_peek(data, size);
    }
    else
    {
        /* 走本地接口 */
        back = uart0_peek(data, size);
    }
    return (back);
}

/*
=======================================
    RS232 读取缓冲区数据
=======================================
*/
CR_API uint_t
rs232_read (
  __CR_OT__ void_t* data,
  __CR_IN__ uint_t  size
    )
{
    uint_t  back;

    if (g_is_bridge)
    {
        /* 走桥接板 */
        back = bridge_rs232_read(data, size);
    }
    else
    {
        /* 走本地接口 */
        back = uart0_read(data, size);
    }
    return (back);
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
