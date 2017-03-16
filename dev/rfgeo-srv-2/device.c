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
/*  >>>>>>>>>>>>>>>>>>>>> RFGEO-SRV-2 设备驱动函数库 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "ff.h"
#include "hash.h"
#include "applib.h"
#include "device.h"
#include "morder.h"
#include "stm32f10x_conf.h"

/*****************************************************************************/
/*                                    储存                                   */
/*****************************************************************************/

/* NOR FLASH 配置 */
#define nor_addr_t          int32u
#define nor_leng_t          leng_t
#define norflash_read       mx25l16_fread
#define norflash_erase      mx25l16_ssec_erase
#define norflash_program    mx25l16_program
/* ------------------------------------- */
#define NORFLASH_ERASE_T1       1
#define NORFLASH_ERASE_T2       0x7FFFFFFFUL
#define NORFLASH_PROGRAM_TOUT   0x7FFFFFFFUL
#define NORFLASH_RD_BACK_SIZE   512
#define NORFLASH_SECTOR_SIZE    MX25LXX_SSEC_SIZE
#include "norflash.inl"

/* FatFS 上下文 */
static FATFS    s_fs;

/*
=======================================
    文件系统初始化
=======================================
*/
CR_API retc_t
store_init (void_t)
{
    byte_t  work[_MAX_SS * 2];

    /* 初始化文件系统 */
    if (f_mount(&s_fs, "", 1) != FR_OK) {
        if (f_mkfs("", FM_FAT, 0, work, sizeof(work)) != FR_OK)
            return (FALSE);
        if (f_mount(&s_fs, "", 1) != FR_OK)
            return (FALSE);
    }
    return (TRUE);
}

/*
=======================================
    检查储存芯片
=======================================
*/
CR_API retc_t
store_check (void_t)
{
    sMX25LXX3   info = { 0 };

    /* 只检查容量对不对 */
    mx25l16_read_id3(&info);
    if (info.cap_sz == 0x15)
        return (TRUE);
    return (FALSE);
}

/*
=======================================
    读取储存器
=======================================
*/
CR_API retc_t
store_read (
  __CR_IN__ int32u  addr,
  __CR_OT__ void_t* data,
  __CR_IN__ leng_t  size
    )
{
    if (mx25l16_fread(addr, data, size) != 0)
        return (TRUE);
    return (FALSE);
}

/*
=======================================
    写入储存器
=======================================
*/
CR_API retc_t
store_write (
  __CR_IN__ int32u          addr,
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    ufast_t idx;

    for (idx = 0; idx < 3; idx++) {
        if (norflash_write(addr, data, size))
            return (TRUE);
    }
    return (FALSE);
}

/*****************************************************************************/
/*                                    串口                                   */
/*****************************************************************************/

/* 环形队列大小 */
#define RX_SIZE CR_K2B(20)

/* 函数的重映射 */
#define uart_zero       uart0_zero
#define uart_rx_size    uart0_rx_size
#define uart_rx_flush   uart0_rx_flush
#define uart_throw      uart0_throw
#define uart_peek       uart0_peek
#define uart_read       uart0_read

#include "uart.inl"

/*
=======================================
    异步串口0等待接收数据
=======================================
*/
CR_API uint_t
uart0_wait (
  __CR_OT__ void_t* data,
  __CR_IN__ uint_t  step,
  __CR_IN__ uint_t  tout
    )
{
    WDT_DECL
    int32u  stp_base = led_base;
    int32u  tot_base = led_base;
    uint_t  count, cnt_base = 0;

    for (;;) {
        count = uart0_rx_size();
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
                return (uart0_read(data, count));
            }
        }
        else
        {
            /* 长时间数据为空返回超时 */
            if (timer_delta32(tot_base) > tout)
                break;
        }
        WDT_FUNC
    }
    return (0);
}

/*
=======================================
    UART2 中断处理
=======================================
*/
CR_API void_t
USART2_IRQHandler (void_t)
{
    if (USART_GetITStatus(USART2, USART_IT_RXNE)) {
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
        s_fifo.rx_buf[s_fifo.rx_tl++] = USART_ReceiveData(USART2);
        if (s_fifo.rx_tl >= RX_SIZE)
            s_fifo.rx_tl = 0;
    }
    if (USART_GetITStatus(USART2, USART_IT_ORE)) {
        s_fifo.rx_buf[s_fifo.rx_tl++] = USART_ReceiveData(USART2);
        if (s_fifo.rx_tl >= RX_SIZE)
            s_fifo.rx_tl = 0;
    }
    if (USART_GetITStatus(USART2, USART_IT_LBD))
        USART_ClearITPendingBit(USART2, USART_IT_LBD);
}

/*****************************************************************************/
/*                                    桥接                                   */
/*****************************************************************************/

/* 桥接最大数据包 */
#define BRIDGE_MAX  4096

/* 临时缓冲区 */
static byte_t   s_okay[2];
static byte_t   s_recv[BRIDGE_MAX + 2];
static byte_t   s_send[BRIDGE_MAX + 2];

/* 内部使用的函数 */
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
    uint_t  count = uart0_rx_size();

    /* 查找有效的头 */
    if (count <= sizeof(hd))
        return (0);
    uart0_peek(hd, sizeof(hd));
    if (hd[0] != 0xAA) {
        uart0_throw(1);
        return (0);
    }
    length  = hd[1];
    length <<= 8;
    length |= hd[2];
    if (length <= 2 ||
        length > BRIDGE_MAX - sizeof(hd)) {
        uart0_throw(1);
        return (0);
    }

    /* 后续数据没有跟上 */
    if (length > count - sizeof(hd))
        return (0);

    /* 校验 CRC16 是否正确 */
    pntr = (byte_t*)data;
    uart0_peek(data, length + sizeof(hd));
    pntr += 1;
    crc1  = hash_crc16h_total(pntr, length);
    pntr += length;
    crc2  = pntr[0];
    crc2 <<= 8;
    crc2 |= pntr[1];
    if (crc1 != crc2) {
        uart0_throw(1);
        return (0);
    }

    /* 一包正常的数据 */
    uart0_throw(length + sizeof(hd));
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
    uart0_write(hd, sizeof(hd));
    uart0_write(data, size);
    uart0_write(&crc, sizeof(crc));
}

/*
---------------------------------------
    桥接板分发任务
---------------------------------------
*/
static void_t
bridge_task (void_t)
{
    uint_t  back;

    back = bridge_recv(s_recv);
    if (back > 2 && s_recv[0] == BRIDGE_SEND)
    {
        /* 向不同的缓冲区转发数据 */
        back -= 2;
        switch (s_recv[1])
        {
            default:
            case PORT_TYPE_BRD: /* 主板 */
                break;

            case PORT_TYPE_CAN: /* CAN */
                break;

            case PORT_TYPE_485: /* 485 */
                bridge_rs485_input(&s_recv[2], back);
                break;

            case PORT_TYPE_PSS: /* 桥接 */
                bridge_rs232_input(&s_recv[2], back);
                break;
        }
    }
    else
    if (back == 2)
    {
        /* 命令的响应 */
        if (s_recv[0] == s_okay[0] && s_recv[0] == RETURN_OKAY)
            s_okay[1] = RETURN_OKAY;
    }
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
    WDT_DECL
    int32u  base = led_base;

    for (;;)
    {
        /* 等待结果和超时 */
        if (s_okay[1] == RETURN_OKAY)
            return (TRUE);
        if (timer_delta32(base) >= timeout)
            break;
        WDT_FUNC
    }
    return (FALSE);
}

/*
=======================================
    桥接板初始化
=======================================
*/
CR_API void_t
bridge_init (void_t)
{
    TIM_TimeBaseInitTypeDef sttb;

    /* 标志清空 */
    s_okay[0] = 0xFF;
    s_okay[1] = 0xFF;

    /* 每 50ms 执行一次桥接板任务 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    TIM_DeInit(TIM2);
    sttb.TIM_Period = 3599;
    sttb.TIM_Prescaler = 999;
    sttb.TIM_ClockDivision = TIM_CKD_DIV1;
    sttb.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &sttb);
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
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
    buf[0] = BRIDGE_BAUD;
    buf[1] = port;
    buf[2] = (byte_t)(baud >> 24);
    buf[3] = (byte_t)(baud >> 16);
    buf[4] = (byte_t)(baud >>  8);
    buf[5] = (byte_t)(baud);
    s_okay[0] = BRIDGE_BAUD;

    /* 重试三次 */
    for (idx = 0; idx < 3; idx++)
    {
        /* 发送命令 */
        s_okay[1] = 0xFF;
        bridge_send(buf, sizeof(buf));

        /* 等待响应 */
        if (bridge_wait(1000)) {
            if (port == PORT_TYPE_BRD)
                uart0_init(baud);
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
    cmd = BRIDGE_RESET;
    s_okay[0] = BRIDGE_RESET;

    /* 重试三次 */
    for (idx = 0; idx < 3; idx++)
    {
        /* 发送命令 */
        s_okay[1] = 0xFF;
        bridge_send(&cmd, sizeof(cmd));

        /* 等待响应 */
        if (bridge_wait(1000))
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
    buf[0] = BRIDGE_GPIO;
    buf[1] = level;
    s_okay[0] = BRIDGE_GPIO;

    /* 重试三次 */
    for (idx = 0; idx < 3; idx++)
    {
        /* 发送命令 */
        s_okay[1] = 0xFF;
        bridge_send(buf, sizeof(buf));

        /* 等待响应 */
        if (bridge_wait(1000))
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
    s_send[0] = BRIDGE_SEND;
    s_send[1] = port;
    mem_cpy(&s_send[2], data, size);
    bridge_send(s_send, size + 2);
}

/*
=======================================
    TIM2 中断处理
=======================================
*/
CR_API void_t
TIM2_IRQHandler (void_t)
{
    uint_t          temp;
    static int32u   base = 0;
    static uint_t   cnts = 0;

    if (TIM_GetITStatus(TIM2, TIM_IT_Update))
    {
        /* 桥接板任务 */
        TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);
        bridge_task();

        /* 长时间没接收到命令就清缓存 */
        temp = uart0_rx_size();
        if (temp != cnts) {
            cnts = temp;
            base = timer_get32();
        }
        else
        if (temp != 0) {
            if (timer_delta32(base) > 1000) {
                uart0_throw(temp);
                cnts = uart0_rx_size();
                base = timer_get32();
            }
        }
    }
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
