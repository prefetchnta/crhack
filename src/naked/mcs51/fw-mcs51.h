/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-10-19  */
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
/*  >>>>>>>>>>>>>>>>>>>>> CrHack MCS-51 片上功能头文件 <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_FW_MCS51_H__
#define __CR_FW_MCS51_H__

/*********************************************/
/* +---------------------------------------+ */
/* | 这个头文件必须在相应 CPU 头文件后包含 | */
/* +---------------------------------------+ */
/*********************************************/

/*****************************************************************************/
/*                                 系统设备                                  */
/*****************************************************************************/

/* 整个系统的中断开关 */
#define irqx_enabled()  b_EA = 1
#define irqx_disable()  b_EA = 0

/* 串口设备的中断开关 */
#define uart0_enabled() b_ES = 1
#define uart0_disable() b_ES = 0

/* 定时设备的中断开关 */
#define time0_enabled() b_ET0 = 1
#define time0_disable() b_ET0 = 0
#define time1_enabled() b_ET1 = 1
#define time1_disable() b_ET1 = 0
#define time2_enabled() b_ET2 = 1
#define time2_disable() b_ET2 = 0

/* 引脚设备的中断开关 */
#define intx0_enabled() b_EX0 = 1
#define intx0_disable() b_EX0 = 0
#define intx1_enabled() b_EX1 = 1
#define intx1_disable() b_EX1 = 0

/* ALE 脉冲开关 (部分) */
#define ale_enabled() r_AUXR &= 0xFE
#define ale_disable() r_AUXR |= 0x01

/* AUX-RAM 开关 (部分) */
#define aux_enabled() { r_CHPENR  = 0x87; \
                        r_CHPENR  = 0x59; \
                        r_CHPCON |= 0x10; \
                        r_CHPENR  = 0x00; }
#define aux_disable() { r_CHPENR  = 0x87; \
                        r_CHPENR  = 0x59; \
                        r_CHPCON &= 0xEF; \
                        r_CHPENR  = 0x00; }

/*****************************************************************************/
/*                                UART 设备                                  */
/*****************************************************************************/

/* 默认使用 12T 频率来计算 */
#ifndef UART0_FOSC
    #define UART0_FOSC  (CPU_FOSC / 12)
#endif

/* 波特率值 */
#if     (UART0_FOSC == 500000UL)
    #define CR_0_B110       0   /*    110 */
    #define CR_0_B300       1   /*    300 */
    #define CR_0_B600       2   /*    600 */
    #define CR_0_B1200      3   /*   1200 */
    #define CR_0_B2400      4   /*   2400 */
#elif   (UART0_FOSC == 921600UL)
    #define CR_0_B300       0   /*    300 */
    #define CR_0_B600       1   /*    600 */
    #define CR_0_B1200      2   /*   1200 */
    #define CR_0_B2400      3   /*   2400 */
    #define CR_0_B4800      4   /*   4800 */
    #define CR_0_B9600      5   /*   9600 */
    #define CR_0_B14400     6   /*  14400 */
    #define CR_0_B19200     7   /*  19200 */
    #define CR_0_B57600     8   /*  57600 */
#elif   (UART0_FOSC == 1000000UL)
    #define CR_0_B300       0   /*    300 */
    #define CR_0_B600       1   /*    600 */
    #define CR_0_B1200      2   /*   1200 */
    #define CR_0_B2400      3   /*   2400 */
    #define CR_0_B4800      4   /*   4800 */
#elif   (UART0_FOSC == 1843200UL)
    #define CR_0_B300       0   /*    300 */
    #define CR_0_B600       1   /*    600 */
    #define CR_0_B1200      2   /*   1200 */
    #define CR_0_B2400      3   /*   2400 */
    #define CR_0_B4800      4   /*   4800 */
    #define CR_0_B9600      5   /*   9600 */
    #define CR_0_B14400     6   /*  14400 */
    #define CR_0_B19200     7   /*  19200 */
    #define CR_0_B38400     8   /*  38400 */
    #define CR_0_B57600     9   /*  57600 */
    #define CR_0_B115200    10  /* 115200 */
#elif   (UART0_FOSC == 2000000UL)
    #define CR_0_B300       0   /*    300 */
    #define CR_0_B600       1   /*    600 */
    #define CR_0_B1200      2   /*   1200 */
    #define CR_0_B2400      3   /*   2400 */
    #define CR_0_B4800      4   /*   4800 */
    #define CR_0_B9600      5   /*   9600 */
#elif   (UART0_FOSC == 3686400UL)
    #define CR_0_B600       0   /*    600 */
    #define CR_0_B1200      1   /*   1200 */
    #define CR_0_B2400      2   /*   2400 */
    #define CR_0_B4800      3   /*   4800 */
    #define CR_0_B9600      4   /*   9600 */
    #define CR_0_B14400     5   /*  14400 */
    #define CR_0_B19200     6   /*  19200 */
    #define CR_0_B38400     7   /*  38400 */
    #define CR_0_B57600     8   /*  57600 */
    #define CR_0_B115200    9   /* 115200 */
#elif   (UART0_FOSC == 6000000UL)
    #define CR_0_B1200      0   /*   1200 */
    #define CR_0_B2400      1   /*   2400 */
    #define CR_0_B4800      2   /*   4800 */
    #define CR_0_B9600      3   /*   9600 */
    #define CR_0_B14400     4   /*  14400 */
#elif   (UART0_FOSC == 11059200UL)
    #define CR_0_B2400      0   /*   2400 */
    #define CR_0_B4800      1   /*   4800 */
    #define CR_0_B9600      2   /*   9600 */
    #define CR_0_B14400     3   /*  14400 */
    #define CR_0_B19200     4   /*  19200 */
    #define CR_0_B38400     5   /*  38400 */
    #define CR_0_B57600     6   /*  57600 */
    #define CR_0_B115200    7   /* 115200 */
#elif   (UART0_FOSC == 12000000UL)
    #define CR_0_B2400      0   /*   2400 */
    #define CR_0_B4800      1   /*   4800 */
    #define CR_0_B9600      2   /*   9600 */
    #define CR_0_B14400     3   /*  14400 */
    #define CR_0_B19200     4   /*  19200 */
    #define CR_0_B57600     5   /*  57600 */
#elif   (UART0_FOSC == 22118400UL)
    #define CR_0_B4800      0   /*   4800 */
    #define CR_0_B9600      1   /*   9600 */
    #define CR_0_B14400     2   /*  14400 */
    #define CR_0_B19200     3   /*  19200 */
    #define CR_0_B38400     4   /*  38400 */
    #define CR_0_B57600     5   /*  57600 */
    #define CR_0_B115200    6   /* 115200 */
    #define CR_0_B128000    7   /* 128000 */
#elif   (UART0_FOSC == 24000000UL)
    #define CR_0_B4800      0   /*   4800 */
    #define CR_0_B9600      1   /*   9600 */
    #define CR_0_B14400     2   /*  14400 */
    #define CR_0_B19200     3   /*  19200 */
    #define CR_0_B38400     4   /*  38400 */
    #define CR_0_B57600     5   /*  57600 */
    #define CR_0_B115200    6   /* 115200 */
#else
    #error "fw-mcs51.h: UART0_FOSC not supported yet!"
#endif

/* 串口0接收 */
#define uart0_recv(ch)  { while (!b_RI); \
                          b_RI = 0; \
                          (ch) = r_SBUF; }
/* 串口0发送 */
#define uart0_send(ch)  { r_SBUF = (ch); \
                          while (!b_TI); \
                          b_TI = 0; }
/* 异步串口0 */
CR_API bool_t   uart0_init (byte_t baud, bool_t parity);
CR_API void_t   uart0_read (void_t *data, leng_t size);
CR_API void_t   uart0_write (const void_t *data, leng_t size);

#endif  /* !__CR_FW_MCS51_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
