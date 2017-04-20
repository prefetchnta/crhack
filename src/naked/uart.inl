/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2017-03-14  */
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
/*  >>>>>>>>>>>>>>>>>>>>> CrHack USART 驱动函数库模板 <<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "applib.h"
#include "memlib.h"

/* 默认定义 */
#ifndef USART_YIELD
    #define USART_YIELD
#endif
#ifndef USART_LOCK_ENTER
    #define USART_LOCK_ENTER
#endif
#ifndef USART_LOCK_LEAVE
    #define USART_LOCK_LEAVE
#endif

/* 环形队列 */
typedef struct
{
#if defined(RX_SIZE)
        uint_t  rx_hd, rx_tl;
        byte_t  rx_buf[RX_SIZE];
#endif
#if defined(TX_SIZE)
        uint_t  tx_hd, tx_tl;
        byte_t  tx_buf[TX_SIZE + 1];
#endif
} sFIFO;

static sFIFO    s_fifo;

/*
=======================================
    初始化环形队列
=======================================
*/
CR_API void_t
uart_zero (void_t)
{
    mem_zero(&s_fifo, sizeof(s_fifo));
}

/* 接收区的实现 */
#if defined(RX_SIZE)

#if defined(uart_rx_size)
/*
=======================================
    读取缓冲区内数据长度
=======================================
*/
CR_API uint_t
uart_rx_size (void_t)
{
    uint_t  head, tail;

    head = s_fifo.rx_hd;
    USART_LOCK_ENTER
    tail = s_fifo.rx_tl;
    USART_LOCK_LEAVE

    /* 这里假设处理的很快永远不会满 */
    if (tail >= head)
        return (tail - head);
    return (RX_SIZE - head + tail);
}

#endif  /* uart_rx_size */

#if defined(uart_rx_flush)
/*
=======================================
    清空接收缓冲区
=======================================
*/
CR_API void_t
uart_rx_flush (void_t)
{
    s_fifo.rx_hd = s_fifo.rx_tl;
}

#endif  /* uart_rx_flush */

#if defined(uart_throw)
/*
=======================================
    丢掉接收缓冲一组字节
=======================================
*/
CR_API void_t
uart_throw (
  __CR_IN__ uint_t  size
    )
{
    /* 自己保证不会超过已有字节数 */
    s_fifo.rx_hd += size;
    if (s_fifo.rx_hd >= RX_SIZE)
        s_fifo.rx_hd -= RX_SIZE;
}

#endif  /* uart_throw */

#if defined(uart_peek)
/*
=======================================
    偷窥缓冲区数据
=======================================
*/
CR_API uint_t
uart_peek (
  __CR_OT__ void_t* data,
  __CR_IN__ uint_t  size
    )
{
    uint_t  temp;

    if (size == 0 || size > uart_rx_size())
        return (0);
    temp = RX_SIZE - s_fifo.rx_hd;
    if (size <= temp) {
        mem_cpy(data, &s_fifo.rx_buf[s_fifo.rx_hd], size);
    }
    else {
        mem_cpy(data, &s_fifo.rx_buf[s_fifo.rx_hd], temp);
        data = (byte_t*)data + temp;
        mem_cpy(data, &s_fifo.rx_buf[0], size - temp);
    }
    return (size);
}

#endif  /* uart_peek */

#if defined(uart_read)
/*
=======================================
    读取缓冲区数据
=======================================
*/
CR_API uint_t
uart_read (
  __CR_OT__ void_t* data,
  __CR_IN__ uint_t  size
    )
{
    size = uart_peek(data, size);
    if (size != 0)
        uart_throw(size);
    return (size);
}

#endif  /* uart_read */

#if defined(uart_input)
/*
=======================================
    输入缓冲区数据
=======================================
*/
CR_API void_t
uart_input (
  __CR_IN__ const void_t*   data,
  __CR_IN__ uint_t          size
    )
{
    for (; size != 0; size--) {
        s_fifo.rx_buf[s_fifo.rx_tl++] = *(byte_t*)data;
        if (s_fifo.rx_tl >= RX_SIZE)
            s_fifo.rx_tl = 0;
        data = (byte_t*)data + 1;
    }
}

#endif  /* uart_input */

#if defined(uart_wait)
/*
=======================================
    等待接收数据
=======================================
*/
CR_API uint_t
uart_wait (
  __CR_OT__ void_t* data,
  __CR_IN__ uint_t  step,
  __CR_IN__ uint_t  tout
    )
{
    uint_t  count, cnt_base = 0;
    int32u  stp_base = timer_get32();
    int32u  tot_base = stp_base;

    for (;;) {
        count = uart_rx_size();
        if (count != cnt_base)
        {
            /* 有数据在来更新计时 */
            cnt_base = count;
            stp_base = timer_get32();
            tot_base = stp_base;
        }
        else
        if (count != 0)
        {
            /* 一段时间没有来数据表示断流, 返回之 */
            if (timer_delta32(stp_base) > step) {
                if (data == NULL)
                    return (count);
                return (uart_read(data, count));
            }
        }
        else
        {
            /* 长时间数据为空返回超时 */
            if (timer_delta32(tot_base) > tout)
                break;
        }
        USART_YIELD
    }
    return (0);
}

#endif  /* uart_wait */

#endif  /* RX_SIZE */

/* 发送区的实现 */
#if defined(TX_SIZE)

#if defined(uart_tx_flush)
/*
=======================================
    清空发送缓冲区
=======================================
*/
CR_API void_t
uart_tx_flush (void_t)
{
    s_fifo.tx_tl = s_fifo.tx_hd;
}

#endif  /* uart_tx_flush */

#if defined(uart_tx_free)
/*
=======================================
    返回缓冲区剩余空间
=======================================
*/
CR_API uint_t
uart_tx_free (void_t)
{
    uint_t  count;
    uint_t  head, tail;

    USART_LOCK_ENTER
    head = s_fifo.tx_hd;
    USART_LOCK_LEAVE
    tail = s_fifo.tx_tl;

    if (tail >= head)
        count = tail - head;
    else
        count = sizeof(s_fifo.tx_buf) - head + tail;
    if (count >= TX_SIZE)
        return (0);
    return (TX_SIZE - count);
}

#endif  /* uart_tx_free */

#if defined(uart_commit)
/*
=======================================
    写入缓冲区数据
=======================================
*/
CR_API uint_t
uart_commit (
  __CR_IN__ const void_t*   data,
  __CR_IN__ uint_t          size
    )
{
    uint_t  count;

    /* 能添加多少是多少 */
    if (size == 0)
        return (0);
    count = uart_tx_free();
    if (count == 0)
        return (0);
    if (size > count)
        size = count;
    for (count = size; count != 0; count--) {
        s_fifo.tx_buf[s_fifo.tx_tl++] = *(byte_t*)data;
        if (s_fifo.tx_tl >= sizeof(s_fifo.tx_buf))
            s_fifo.tx_tl = 0;
        data = (byte_t*)data + 1;
    }
    return (size);
}

#endif  /* uart_commit */

#endif  /* TX_SIZE */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
