/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2017-03-19  */
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
/*  >>>>>>>>>>>>>>>>>>>>> CrHack 桥接板驱动函数库模板 <<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "hash.h"
#include "applib.h"
#include "morder.h"
#include "memlib.h"

/* 默认定义 */
#ifndef BRIDGE_YIELD
    #define BRIDGE_YIELD
#endif
#ifndef BRIDGE_MAX
    #define BRIDGE_MAX      4096
#endif
#ifndef BRIDGE_TIMEOUT
    #define BRIDGE_TIMEOUT  1000
#endif
#ifndef BRIDGE_CLR_TOUT
    #define BRIDGE_CLR_TOUT 1000
#endif

/* 临时缓冲区 */
static byte_t   s_okay[2];
static byte_t   s_recv[BRIDGE_MAX + 2];
static byte_t   s_send[BRIDGE_MAX + 2];

/* 两个需要实现的函数 */
CR_API void_t   bridge_rs232_input (const void_t *data, uint_t size);
CR_API void_t   bridge_rs485_input (const void_t *data, uint_t size);

/*
---------------------------------------
    查找一帧桥接数据
---------------------------------------
*/
static uint_t
bridge_recv (
  __CR_OT__ void_t* data
    )
{
    byte_t  hd[3], *pntr;
    int16u  crc1, crc2, length;
    uint_t  count = uart_rx_size();

    /* 查找有效的头 */
    if (count <= sizeof(hd))
        return (0);
    uart_peek(hd, sizeof(hd));
    if (hd[0] != 0xAA) {
        uart_throw(1);
        return (0);
    }
    length  = hd[1];
    length <<= 8;
    length |= hd[2];
    if (length <= 2 ||
        length > BRIDGE_MAX - sizeof(hd)) {
        uart_throw(1);
        return (0);
    }

    /* 后续数据没有跟上 */
    if (length > count - sizeof(hd))
        return (0);

    /* 校验 CRC16 是否正确 */
    pntr = (byte_t*)data;
    uart_peek(data, length + sizeof(hd));
    pntr += 1;
    crc1  = hash_crc16h_total(pntr, length);
    pntr += length;
    crc2  = pntr[0];
    crc2 <<= 8;
    crc2 |= pntr[1];
    if (crc1 != crc2) {
        uart_throw(1);
        return (0);
    }

    /* 一包正常的数据 */
    uart_throw(length + sizeof(hd));
    length -= 2;
    mem_mov(data, (byte_t*)data + sizeof(hd), length);
    return (length);
}

/*
---------------------------------------
    发送一帧桥接数据
---------------------------------------
*/
static void_t
bridge_send (
  __CR_IN__ const void_t*   data,
  __CR_IN__ uint_t          size
    )
{
    byte_t  hd[3];
    int16u  crc = 0xFFFF;

    size += 2;
    hd[0] = 0xAA;
    hd[1] = (byte_t)(size >> 8);
    hd[2] = (byte_t)(size);
    size -= 2;
    crc = hash_crc16h_update(crc, &hd[1], 2);
    crc = hash_crc16h_update(crc, data, size);
    crc = WORD_BE(crc);
    uart_write(hd, sizeof(hd));
    uart_write(data, size);
    uart_write(&crc, sizeof(crc));
}

/*
---------------------------------------
    桥接板等待回应
---------------------------------------
*/
static bool_t
bridge_wait (
  __CR_IN__ uint_t  timeout
    )
{
    int32u  base = timer_get32();

    for (;;)
    {
        /* 等待结果和超时 */
        if (s_okay[1] == CR_BRIDGE_ERR_OKAY)
            return (TRUE);
        if (timer_delta32(base) >= timeout)
            break;
        BRIDGE_YIELD
    }
    return (FALSE);
}

/*
---------------------------------------
    桥接板分发任务
---------------------------------------
*/
static void_t
bridge_task (void_t)
{
    uint_t          back;
    uint_t          temp;
    static int32u   base = 0;
    static uint_t   cnts = 0;

    back = bridge_recv(s_recv);
    if (back > 2 && s_recv[0] == CR_BRIDGE_CMD_SEND)
    {
        /* 向不同的缓冲区转发数据 */
        back -= 2;
        switch (s_recv[1])
        {
            default:
            case CR_BRIDGE_PORT_BRD:    /* 主板 */
                break;

            case CR_BRIDGE_PORT_CAN:    /* CAN */
                break;

            case CR_BRIDGE_PORT_485:    /* 485 */
                bridge_rs485_input(&s_recv[2], back);
                break;

            case CR_BRIDGE_PORT_PSS:    /* 桥接 */
                bridge_rs232_input(&s_recv[2], back);
                break;
        }
    }
    else
    if (back == 2)
    {
        /* 命令的响应 */
        if (s_recv[0] == s_okay[0] && s_recv[0] == CR_BRIDGE_ERR_OKAY)
            s_okay[1] = CR_BRIDGE_ERR_OKAY;
    }

    /* 长时间没接收到命令就清缓存 */
    temp = uart_rx_size();
    if (temp != cnts) {
        cnts = temp;
        base = timer_get32();
    }
    else
    if (temp != 0) {
        if (timer_delta32(base) > BRIDGE_CLR_TOUT) {
            uart_throw(1);
            cnts = uart_rx_size();
            base = timer_get32();
        }
    }
}

/*
=======================================
    桥接板波特率设置
=======================================
*/
CR_API bool_t
bridge_baud (
  __CR_IN__ byte_t  port,
  __CR_IN__ int32u  baud
    )
{
    uint_t  idx;
    byte_t  buf[6];

    /* 填充命令 */
    buf[0] = CR_BRIDGE_CMD_BAUD;
    buf[1] = port;
    buf[2] = (byte_t)(baud >> 24);
    buf[3] = (byte_t)(baud >> 16);
    buf[4] = (byte_t)(baud >>  8);
    buf[5] = (byte_t)(baud);
    s_okay[0] = CR_BRIDGE_CMD_BAUD;

    /* 重试三次 */
    for (idx = 0; idx < 3; idx++)
    {
        /* 发送命令 */
        s_okay[1] = 0xFF;
        bridge_send(buf, sizeof(buf));

        /* 等待响应 */
        if (bridge_wait(BRIDGE_TIMEOUT)) {
            if (port == CR_BRIDGE_PORT_BRD)
                uart_init(baud);
            return (TRUE);
        }
    }
    return (FALSE);
}

/*
=======================================
    桥接板复位
=======================================
*/
CR_API bool_t
bridge_reset (void_t)
{
    uint_t  idx;
    byte_t  cmd;

    /* 填充命令 */
    cmd = CR_BRIDGE_CMD_RESET;
    s_okay[0] = CR_BRIDGE_CMD_RESET;

    /* 重试三次 */
    for (idx = 0; idx < 3; idx++)
    {
        /* 发送命令 */
        s_okay[1] = 0xFF;
        bridge_send(&cmd, sizeof(cmd));

        /* 等待响应 */
        if (bridge_wait(BRIDGE_TIMEOUT))
            return (TRUE);
    }
    return (FALSE);
}

/*
=======================================
    桥接板电平控制
=======================================
*/
CR_API bool_t
bridge_gpio (
  __CR_IN__ byte_t  level
    )
{
    uint_t  idx;
    byte_t  buf[2];

    /* 填充命令 */
    buf[0] = CR_BRIDGE_CMD_GPIO;
    buf[1] = level;
    s_okay[0] = CR_BRIDGE_CMD_GPIO;

    /* 重试三次 */
    for (idx = 0; idx < 3; idx++)
    {
        /* 发送命令 */
        s_okay[1] = 0xFF;
        bridge_send(buf, sizeof(buf));

        /* 等待响应 */
        if (bridge_wait(BRIDGE_TIMEOUT))
            return (TRUE);
    }
    return (FALSE);
}

/*
=======================================
    桥接板透传
=======================================
*/
CR_API void_t
bridge_commit (
  __CR_IN__ byte_t          port,
  __CR_IN__ const void_t*   data,
  __CR_IN__ uint_t          size
    )
{
    if (size == 0 || size >= BRIDGE_MAX)
        return;
    s_send[0] = CR_BRIDGE_CMD_SEND;
    s_send[1] = port;
    mem_cpy(&s_send[2], data, size);
    bridge_send(s_send, size + 2);
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
