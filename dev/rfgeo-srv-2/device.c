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
#include "device.h"
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
    byte_t  work[FF_MAX_SS * 2];

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
    wdt_task();
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
        wdt_task();
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
#define uart_input      uart0_input
#define uart_wait       uart0_wait

/* 等待中的空闲 */
#define USART_YIELD     wdt_task();

#include "uart.inl"

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
    if (USART_GetFlagStatus(USART2, USART_FLAG_ORE)) {
        s_fifo.rx_buf[s_fifo.rx_tl] = USART_ReceiveData(USART2);
        USART_ClearFlag(USART2, USART_FLAG_ORE);
    }
}

/*****************************************************************************/
/*                                    桥接                                   */
/*****************************************************************************/

/* 函数的重映射 */
#define uart_init       uart0_init
#define uart_write      uart0_write

/* 等待中的空闲 */
#define BRIDGE_YIELD    wdt_task();

#include "bridge.bsp"

/* 是否通过桥接板 */
bool_t  g_is_bridge = FALSE;

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
    g_is_bridge = TRUE;
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
}

/*
=======================================
    桥接板关闭
=======================================
*/
CR_API void_t
bridge_kill (void_t)
{
    g_is_bridge = FALSE;
    TIM_Cmd(TIM2, DISABLE);
    TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
}

/*
=======================================
    TIM2 中断处理
=======================================
*/
CR_API void_t
TIM2_IRQHandler (void_t)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update)) {
        TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);
        bridge_task();
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
