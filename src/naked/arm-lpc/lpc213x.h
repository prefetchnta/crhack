/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2011-03-10  */
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
/*  >>>>>>>>>>>>>>>> CrHack NXP LPC213X 系列 CPU 定义头文件 <<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_LPC213X_H__
#define __CR_LPC213X_H__

#include "defs.h"

/*****************************************************************************/
/*                               CPU 频率配置                                */
/*****************************************************************************/

/* 系统频率 (x1-x32, <= 60MHz) */
#ifndef LPC_CCLK
    #define LPC_CCLK    (CPU_FOSC * 4)
#endif

/* CCO 频率 (x2/x4/x8/x16, 156-320MHz) */
#ifndef LPC_FCCO
    #define LPC_FCCO    (LPC_CCLK * 4)
#endif

/* VPB 外设频率 (x1/x2/x4) */
#ifndef LPC_PCLK
    #define LPC_PCLK    (LPC_CCLK / 4)
#endif

/*****************************************************************************/
/*                              系统控制寄存器                               */
/*****************************************************************************/

#define r_EXTINT            IO_INT08(0xE01FC140)    /* 外部中断 */
#define r_INTWAKE           IO_INT16(0xE01FC144)
#define r_EXTMODE           IO_INT08(0xE01FC148)
#define r_EXTPOLAR          IO_INT08(0xE01FC14C)
#define r_MEMMAP            IO_INT08(0xE01FC040)    /* 内存映射控制 */
#define r_PLLCON            IO_INT08(0xE01FC080)    /* PLL 控制 */
#define r_PLLCFG            IO_INT08(0xE01FC084)
#define r_PLLSTAT           IO_INT16(0xE01FC088)
#define r_PLLFEED           IO_INT08(0xE01FC08C)
#define r_PCON              IO_INT08(0xE01FC0C0)    /* 电源控制 */
#define r_PCONP             IO_INT32(0xE01FC0C4)
#define r_VPBDIV            IO_INT08(0xE01FC100)    /* VPB 分频器 */
#define r_RSIR              IO_INT08(0xE01FC180)    /* 复位 */
#define r_CSPR              IO_INT08(0xE01FC184)    /* 代码安全与调试 */

/*****************************************************************************/
/*                              MAM 控制寄存器                               */
/*****************************************************************************/

#define r_MAMCR             IO_INT08(0xE01FC000)
#define r_MAMTIM            IO_INT08(0xE01FC004)

/*****************************************************************************/
/*                              VIC 控制寄存器                               */
/*****************************************************************************/

#define r_VICIRQStatus      IO_INT32(0xFFFFF000)
#define r_VICFIQStatus      IO_INT32(0xFFFFF004)
#define r_VICRawIntr        IO_INT32(0xFFFFF008)
#define r_VICIntSelect      IO_INT32(0xFFFFF00C)
#define r_VICIntEnable      IO_INT32(0xFFFFF010)
#define r_VICIntEnClr       IO_INT32(0xFFFFF014)
#define r_VICSoftInt        IO_INT32(0xFFFFF018)
#define r_VICSoftIntClear   IO_INT32(0xFFFFF01C)
#define r_VICProtection     IO_INT32(0xFFFFF020)
#define r_VICVectAddr       IO_INT32(0xFFFFF030)
#define r_VICDefVectAddr    IO_INT32(0xFFFFF034)
#define r_VICVectAddr0      IO_INT32(0xFFFFF100)
#define r_VICVectAddr1      IO_INT32(0xFFFFF104)
#define r_VICVectAddr2      IO_INT32(0xFFFFF108)
#define r_VICVectAddr3      IO_INT32(0xFFFFF10C)
#define r_VICVectAddr4      IO_INT32(0xFFFFF110)
#define r_VICVectAddr5      IO_INT32(0xFFFFF114)
#define r_VICVectAddr6      IO_INT32(0xFFFFF118)
#define r_VICVectAddr7      IO_INT32(0xFFFFF11C)
#define r_VICVectAddr8      IO_INT32(0xFFFFF120)
#define r_VICVectAddr9      IO_INT32(0xFFFFF124)
#define r_VICVectAddr10     IO_INT32(0xFFFFF128)
#define r_VICVectAddr11     IO_INT32(0xFFFFF12C)
#define r_VICVectAddr12     IO_INT32(0xFFFFF130)
#define r_VICVectAddr13     IO_INT32(0xFFFFF134)
#define r_VICVectAddr14     IO_INT32(0xFFFFF138)
#define r_VICVectAddr15     IO_INT32(0xFFFFF13C)
#define r_VICVectCntl0      IO_INT32(0xFFFFF200)
#define r_VICVectCntl1      IO_INT32(0xFFFFF204)
#define r_VICVectCntl2      IO_INT32(0xFFFFF208)
#define r_VICVectCntl3      IO_INT32(0xFFFFF20C)
#define r_VICVectCntl4      IO_INT32(0xFFFFF210)
#define r_VICVectCntl5      IO_INT32(0xFFFFF214)
#define r_VICVectCntl6      IO_INT32(0xFFFFF218)
#define r_VICVectCntl7      IO_INT32(0xFFFFF21C)
#define r_VICVectCntl8      IO_INT32(0xFFFFF220)
#define r_VICVectCntl9      IO_INT32(0xFFFFF224)
#define r_VICVectCntl10     IO_INT32(0xFFFFF228)
#define r_VICVectCntl11     IO_INT32(0xFFFFF22C)
#define r_VICVectCntl12     IO_INT32(0xFFFFF230)
#define r_VICVectCntl13     IO_INT32(0xFFFFF234)
#define r_VICVectCntl14     IO_INT32(0xFFFFF238)
#define r_VICVectCntl15     IO_INT32(0xFFFFF23C)

/*****************************************************************************/
/*                              管脚配置寄存器                               */
/*****************************************************************************/

#define r_PINSEL0           IO_INT32(0xE002C000)
#define r_PINSEL1           IO_INT32(0xE002C004)
#define r_PINSEL2           IO_INT32(0xE002C014)

/*****************************************************************************/
/*                             GPIO 设备寄存器                               */
/*****************************************************************************/

#define r_IO0PIN            IO_INT32(0xE0028000)
#define r_IO0SET            IO_INT32(0xE0028004)
#define r_IO0DIR            IO_INT32(0xE0028008)
#define r_IO0CLR            IO_INT32(0xE002800C)
#define r_IO1PIN            IO_INT32(0xE0028010)
#define r_IO1SET            IO_INT32(0xE0028014)
#define r_IO1DIR            IO_INT32(0xE0028018)
#define r_IO1CLR            IO_INT32(0xE002801C)

/*****************************************************************************/
/*                             UART0 设备寄存器                              */
/*****************************************************************************/

#define r_U0RBR             IO_INT08(0xE000C000)
#define r_U0THR             IO_INT08(0xE000C000)
#define r_U0DLL             IO_INT08(0xE000C000)
#define r_U0DLM             IO_INT08(0xE000C004)
#define r_U0IER             IO_INT08(0xE000C004)
#define r_U0IIR             IO_INT08(0xE000C008)
#define r_U0FCR             IO_INT08(0xE000C008)
#define r_U0LCR             IO_INT08(0xE000C00C)
#define r_U0LSR             IO_INT08(0xE000C014)
#define r_U0SCR             IO_INT08(0xE000C01C)
#define r_U0TER             IO_INT08(0xE000C030)

/*****************************************************************************/
/*                             UART1 设备寄存器                              */
/*****************************************************************************/

#define r_U1RBR             IO_INT08(0xE0010000)
#define r_U1THR             IO_INT08(0xE0010000)
#define r_U1DLL             IO_INT08(0xE0010000)
#define r_U1DLM             IO_INT08(0xE0010004)
#define r_U1IER             IO_INT08(0xE0010004)
#define r_U1IIR             IO_INT08(0xE0010008)
#define r_U1FCR             IO_INT08(0xE0010008)
#define r_U1LCR             IO_INT08(0xE001000C)
#define r_U1MCR             IO_INT08(0xE0010010)
#define r_U1LSR             IO_INT08(0xE0010014)
#define r_U1MSR             IO_INT08(0xE0010018)
#define r_U1SCR             IO_INT08(0xE001001C)
#define r_U1TER             IO_INT08(0xE0010030)

/*****************************************************************************/
/*                              I2C 设备寄存器                               */
/*****************************************************************************/

#define r_I2C0CONSET        IO_INT08(0xE001C000)
#define r_I2C0STAT          IO_INT08(0xE001C004)
#define r_I2C0DAT           IO_INT08(0xE001C008)
#define r_I2C0ADR           IO_INT08(0xE001C00C)
#define r_I2C0SCLH          IO_INT16(0xE001C010)
#define r_I2C0SCLL          IO_INT16(0xE001C014)
#define r_I2C0CONCLR        IO_INT08(0xE001C018)
#define r_I2C1CONSET        IO_INT08(0xE005C000)
#define r_I2C1STAT          IO_INT08(0xE005C004)
#define r_I2C1DAT           IO_INT08(0xE005C008)
#define r_I2C1ADR           IO_INT08(0xE005C00C)
#define r_I2C1SCLH          IO_INT16(0xE005C010)
#define r_I2C1SCLL          IO_INT16(0xE005C014)
#define r_I2C1CONCLR        IO_INT08(0xE005C018)

/*****************************************************************************/
/*                              SPI 设备寄存器                               */
/*****************************************************************************/

#define r_S0SPCR            IO_INT16(0xE0020000)
#define r_S0SPSR            IO_INT08(0xE0020004)
#define r_S0SPDR            IO_INT16(0xE0020008)
#define r_S0SPCCR           IO_INT08(0xE002000C)
#define r_S0SPINT           IO_INT08(0xE002001C)

/*****************************************************************************/
/*                              SSP 设备寄存器                               */
/*****************************************************************************/

#define r_SSPCR0            IO_INT16(0xE0068000)
#define r_SSPCR1            IO_INT08(0xE0068004)
#define r_SSPDR             IO_INT16(0xE0068008)
#define r_SSPSR             IO_INT08(0xE006800C)
#define r_SSPCPSR           IO_INT08(0xE0068010)
#define r_SSPIMSC           IO_INT08(0xE0068014)
#define r_SSPRIS            IO_INT08(0xE0068018)
#define r_SSPMIS            IO_INT08(0xE006801C)
#define r_SSPICR            IO_INT08(0xE0068020)

/*****************************************************************************/
/*                             TIMER 设备寄存器                              */
/*****************************************************************************/

#define r_T0IR              IO_INT08(0xE0004000)
#define r_T0TCR             IO_INT08(0xE0004004)
#define r_T0TC              IO_INT32(0xE0004008)
#define r_T0PR              IO_INT32(0xE000400C)
#define r_T0PC              IO_INT32(0xE0004010)
#define r_T0MCR             IO_INT16(0xE0004014)
#define r_T0MR0             IO_INT32(0xE0004018)
#define r_T0MR1             IO_INT32(0xE000401C)
#define r_T0MR2             IO_INT32(0xE0004020)
#define r_T0MR3             IO_INT32(0xE0004024)
#define r_T0CCR             IO_INT16(0xE0004028)
#define r_T0CR0             IO_INT32(0xE000402C)
#define r_T0CR1             IO_INT32(0xE0004030)
#define r_T0CR2             IO_INT32(0xE0004034)
#define r_T0CR3             IO_INT32(0xE0004038)
#define r_T0EMR             IO_INT16(0xE000403C)
#define r_T0CTCR            IO_INT08(0xE0004070)
#define r_T1IR              IO_INT08(0xE0008000)
#define r_T1TCR             IO_INT08(0xE0008004)
#define r_T1TC              IO_INT32(0xE0008008)
#define r_T1PR              IO_INT32(0xE000800C)
#define r_T1PC              IO_INT32(0xE0008010)
#define r_T1MCR             IO_INT16(0xE0008014)
#define r_T1MR0             IO_INT32(0xE0008018)
#define r_T1MR1             IO_INT32(0xE000801C)
#define r_T1MR2             IO_INT32(0xE0008020)
#define r_T1MR3             IO_INT32(0xE0008024)
#define r_T1CCR             IO_INT16(0xE0008028)
#define r_T1CR0             IO_INT32(0xE000802C)
#define r_T1CR1             IO_INT32(0xE0008030)
#define r_T1CR2             IO_INT32(0xE0008034)
#define r_T1CR3             IO_INT32(0xE0008038)
#define r_T1EMR             IO_INT16(0xE000803C)
#define r_T1CTCR            IO_INT08(0xE0008070)

/*****************************************************************************/
/*                              PWM 设备寄存器                               */
/*****************************************************************************/

#define r_PWMIR             IO_INT16(0xE0014000)
#define r_PWMTCR            IO_INT08(0xE0014004)
#define r_PWMTC             IO_INT32(0xE0014008)
#define r_PWMPR             IO_INT32(0xE001400C)
#define r_PWMPC             IO_INT32(0xE0014010)
#define r_PWMMCR            IO_INT32(0xE0014014)
#define r_PWMMR0            IO_INT32(0xE0014018)
#define r_PWMMR1            IO_INT32(0xE001401C)
#define r_PWMMR2            IO_INT32(0xE0014020)
#define r_PWMMR3            IO_INT32(0xE0014024)
#define r_PWMMR4            IO_INT32(0xE0014040)
#define r_PWMMR5            IO_INT32(0xE0014044)
#define r_PWMMR6            IO_INT32(0xE0014048)
#define r_PWMPCR            IO_INT16(0xE001404C)
#define r_PWMLER            IO_INT08(0xE0014050)

/*****************************************************************************/
/*                              ADC 设备寄存器                               */
/*****************************************************************************/

#define r_AD0CR             IO_INT32(0xE0034000)
#define r_AD0DR             IO_INT32(0xE0034004)
#define r_ADGSR             IO_INT32(0xE0034008)
#define r_AD1CR             IO_INT32(0xE0060000)
#define r_AD1DR             IO_INT32(0xE0060004)

/*****************************************************************************/
/*                              DAC 设备寄存器                               */
/*****************************************************************************/

#define r_DACR              IO_INT32(0xE006C000)

/*****************************************************************************/
/*                              RTC 设备寄存器                               */
/*****************************************************************************/

#define r_ILR               IO_INT08(0xE0024000)
#define r_CTC               IO_INT16(0xE0024004)
#define r_CCR               IO_INT08(0xE0024008)
#define r_CIIR              IO_INT08(0xE002400C)
#define r_AMR               IO_INT08(0xE0024010)
#define r_CTIME0            IO_INT32(0xE0024014)
#define r_CTIME1            IO_INT32(0xE0024018)
#define r_CTIME2            IO_INT32(0xE002401C)
#define r_SEC               IO_INT08(0xE0024020)
#define r_MIN               IO_INT08(0xE0024024)
#define r_HOUR              IO_INT08(0xE0024028)
#define r_DOM               IO_INT08(0xE002402C)
#define r_DOW               IO_INT08(0xE0024030)
#define r_DOY               IO_INT16(0xE0024034)
#define r_MONTH             IO_INT08(0xE0024038)
#define r_YEAR              IO_INT16(0xE002403C)
#define r_ALSEC             IO_INT08(0xE0024060)
#define r_ALMIN             IO_INT08(0xE0024064)
#define r_ALHOUR            IO_INT08(0xE0024068)
#define r_ALDOM             IO_INT08(0xE002406C)
#define r_ALDOW             IO_INT08(0xE0024070)
#define r_ALDOY             IO_INT16(0xE0024074)
#define r_ALMON             IO_INT08(0xE0024078)
#define r_ALYEAR            IO_INT16(0xE002407C)
#define r_PREINT            IO_INT16(0xE0024080)
#define r_PREFRAC           IO_INT16(0xE0024084)

/*****************************************************************************/
/*                              WDT 设备寄存器                               */
/*****************************************************************************/

#define r_WDMOD             IO_INT08(0xE0000000)
#define r_WDTC              IO_INT32(0xE0000004)
#define r_WDFEED            IO_INT08(0xE0000008)
#define r_WDTV              IO_INT32(0xE000000C)

#endif  /* !__CR_LPC213X_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
