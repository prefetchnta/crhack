/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2011-03-12  */
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
/*  >>>>>>>>>>>>>>>> CrHack NXP LPC214X 系列 CPU 定义头文件 <<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_LPC214X_H__
#define __CR_LPC214X_H__

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
#define r_PLL0CON           IO_INT08(0xE01FC080)    /* PLL 控制 */
#define r_PLL0CFG           IO_INT08(0xE01FC084)
#define r_PLL0STAT          IO_INT16(0xE01FC088)
#define r_PLL0FEED          IO_INT08(0xE01FC08C)
#define r_PLL1CON           IO_INT08(0xE01FC0A0)    /* PLL 控制 (USB) */
#define r_PLL1CFG           IO_INT08(0xE01FC0A4)
#define r_PLL1STAT          IO_INT16(0xE01FC0A8)
#define r_PLL1FEED          IO_INT08(0xE01FC0AC)
#define r_PCON              IO_INT08(0xE01FC0C0)    /* 电源控制 */
#define r_PCONP             IO_INT32(0xE01FC0C4)
#define r_VPBDIV            IO_INT08(0xE01FC100)    /* VPB 分频器 */
#define r_RSIR              IO_INT08(0xE01FC180)    /* 复位 */
#define r_CSPR              IO_INT08(0xE01FC184)    /* 代码安全与调试 */
#define r_SCS               IO_INT32(0xE01FC1A0)    /* 其他系统控制 */

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

#define r_IO0PIN            IO_INT32(0xE0028000)    /* 慢速型 */
#define r_IO0SET            IO_INT32(0xE0028004)
#define r_IO0DIR            IO_INT32(0xE0028008)
#define r_IO0CLR            IO_INT32(0xE002800C)
#define r_IO1PIN            IO_INT32(0xE0028010)
#define r_IO1SET            IO_INT32(0xE0028014)
#define r_IO1DIR            IO_INT32(0xE0028018)
#define r_IO1CLR            IO_INT32(0xE002801C)
#define r_FIO0DIR           IO_INT32(0x3FFFC000)    /* 增强型 */
#define r_FIO0DIR0          IO_INT08(0x3FFFC000)
#define r_FIO0DIR1          IO_INT08(0x3FFFC001)
#define r_FIO0DIR2          IO_INT08(0x3FFFC002)
#define r_FIO0DIR3          IO_INT08(0x3FFFC003)
#define r_FIO0DIRL          IO_INT16(0x3FFFC000)
#define r_FIO0DIRU          IO_INT16(0x3FFFC002)
#define r_FIO0MASK          IO_INT32(0x3FFFC010)
#define r_FIO0MASK0         IO_INT08(0x3FFFC010)
#define r_FIO0MASK1         IO_INT08(0x3FFFC011)
#define r_FIO0MASK2         IO_INT08(0x3FFFC012)
#define r_FIO0MASK3         IO_INT08(0x3FFFC013)
#define r_FIO0MASKL         IO_INT16(0x3FFFC010)
#define r_FIO0MASKU         IO_INT16(0x3FFFC012)
#define r_FIO0PIN           IO_INT32(0x3FFFC014)
#define r_FIO0PIN0          IO_INT08(0x3FFFC014)
#define r_FIO0PIN1          IO_INT08(0x3FFFC015)
#define r_FIO0PIN2          IO_INT08(0x3FFFC016)
#define r_FIO0PIN3          IO_INT08(0x3FFFC017)
#define r_FIO0PINL          IO_INT16(0x3FFFC014)
#define r_FIO0PINU          IO_INT16(0x3FFFC016)
#define r_FIO0SET           IO_INT32(0x3FFFC018)
#define r_FIO0SET0          IO_INT08(0x3FFFC018)
#define r_FIO0SET1          IO_INT08(0x3FFFC019)
#define r_FIO0SET2          IO_INT08(0x3FFFC01A)
#define r_FIO0SET3          IO_INT08(0x3FFFC01B)
#define r_FIO0SETL          IO_INT16(0x3FFFC018)
#define r_FIO0SETU          IO_INT16(0x3FFFC01A)
#define r_FIO0CLR           IO_INT32(0x3FFFC01C)
#define r_FIO0CLR0          IO_INT08(0x3FFFC01C)
#define r_FIO0CLR1          IO_INT08(0x3FFFC01D)
#define r_FIO0CLR2          IO_INT08(0x3FFFC01E)
#define r_FIO0CLR3          IO_INT08(0x3FFFC01F)
#define r_FIO0CLRL          IO_INT16(0x3FFFC01C)
#define r_FIO0CLRU          IO_INT16(0x3FFFC01E)
#define r_FIO1DIR           IO_INT32(0x3FFFC020)
#define r_FIO1DIR0          IO_INT08(0x3FFFC020)
#define r_FIO1DIR1          IO_INT08(0x3FFFC021)
#define r_FIO1DIR2          IO_INT08(0x3FFFC022)
#define r_FIO1DIR3          IO_INT08(0x3FFFC023)
#define r_FIO1DIRL          IO_INT16(0x3FFFC020)
#define r_FIO1DIRU          IO_INT16(0x3FFFC022)
#define r_FIO1MASK          IO_INT32(0x3FFFC030)
#define r_FIO1MASK0         IO_INT08(0x3FFFC030)
#define r_FIO1MASK1         IO_INT08(0x3FFFC031)
#define r_FIO1MASK2         IO_INT08(0x3FFFC032)
#define r_FIO1MASK3         IO_INT08(0x3FFFC033)
#define r_FIO1MASKL         IO_INT16(0x3FFFC030)
#define r_FIO1MASKU         IO_INT16(0x3FFFC032)
#define r_FIO1PIN           IO_INT32(0x3FFFC034)
#define r_FIO1PIN0          IO_INT08(0x3FFFC034)
#define r_FIO1PIN1          IO_INT08(0x3FFFC035)
#define r_FIO1PIN2          IO_INT08(0x3FFFC036)
#define r_FIO1PIN3          IO_INT08(0x3FFFC037)
#define r_FIO1PINL          IO_INT16(0x3FFFC034)
#define r_FIO1PINU          IO_INT16(0x3FFFC036)
#define r_FIO1SET           IO_INT32(0x3FFFC038)
#define r_FIO1SET0          IO_INT08(0x3FFFC038)
#define r_FIO1SET1          IO_INT08(0x3FFFC039)
#define r_FIO1SET2          IO_INT08(0x3FFFC03A)
#define r_FIO1SET3          IO_INT08(0x3FFFC03B)
#define r_FIO1SETL          IO_INT16(0x3FFFC038)
#define r_FIO1SETU          IO_INT16(0x3FFFC03A)
#define r_FIO1CLR           IO_INT32(0x3FFFC03C)
#define r_FIO1CLR0          IO_INT08(0x3FFFC03C)
#define r_FIO1CLR1          IO_INT08(0x3FFFC03D)
#define r_FIO1CLR2          IO_INT08(0x3FFFC03E)
#define r_FIO1CLR3          IO_INT08(0x3FFFC03F)
#define r_FIO1CLRL          IO_INT16(0x3FFFC03C)
#define r_FIO1CLRU          IO_INT16(0x3FFFC03E)

/*****************************************************************************/
/*                             UART0 设备寄存器                              */
/*****************************************************************************/

#define r_U0RBR             IO_INT08(0xE000C000)
#define r_U0THR             IO_INT08(0xE000C000)
#define r_U0DLL             IO_INT08(0xE000C000)
#define r_U0DLM             IO_INT08(0xE000C004)
#define r_U0IER             IO_INT32(0xE000C004)
#define r_U0IIR             IO_INT32(0xE000C008)
#define r_U0FCR             IO_INT08(0xE000C008)
#define r_U0LCR             IO_INT08(0xE000C00C)
#define r_U0LSR             IO_INT08(0xE000C014)
#define r_U0SCR             IO_INT08(0xE000C01C)
#define r_U0ACR             IO_INT32(0xE000C020)
#define r_U0FDR             IO_INT32(0xE000C028)
#define r_U0TER             IO_INT08(0xE000C030)

/*****************************************************************************/
/*                             UART1 设备寄存器                              */
/*****************************************************************************/

#define r_U1RBR             IO_INT08(0xE0010000)
#define r_U1THR             IO_INT08(0xE0010000)
#define r_U1DLL             IO_INT08(0xE0010000)
#define r_U1DLM             IO_INT08(0xE0010004)
#define r_U1IER             IO_INT32(0xE0010004)
#define r_U1IIR             IO_INT32(0xE0010008)
#define r_U1FCR             IO_INT08(0xE0010008)
#define r_U1LCR             IO_INT08(0xE001000C)
#define r_U1MCR             IO_INT08(0xE0010010)
#define r_U1LSR             IO_INT08(0xE0010014)
#define r_U1MSR             IO_INT08(0xE0010018)
#define r_U1SCR             IO_INT08(0xE001001C)
#define r_U1ACR             IO_INT32(0xE0010020)
#define r_U1FDR             IO_INT32(0xE0010028)
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
/*                              USB 设备寄存器                               */
/*****************************************************************************/

#define r_USBIntSt          IO_INT32(0xE01FC1C0)    /* 设备中断寄存器 */
#define r_USBDevIntSt       IO_INT32(0xE0090000)
#define r_USBDevIntEn       IO_INT32(0xE0090004)
#define r_USBDevIntClr      IO_INT32(0xE0090008)
#define r_USBDevIntSet      IO_INT32(0xE009000C)
#define r_USBDevIntPri      IO_INT08(0xE009002C)
#define r_USBEpIntSt        IO_INT32(0xE0090030)    /* 端点中断寄存器 */
#define r_USBEpIntEn        IO_INT32(0xE0090034)
#define r_USBEpIntClr       IO_INT32(0xE0090038)
#define r_USBEpIntSet       IO_INT32(0xE009003C)
#define r_USBEpIntPri       IO_INT32(0xE0090040)
#define r_USBReEp           IO_INT32(0xE0090044)    /* 端点实现寄存器 */
#define r_USBEpInd          IO_INT32(0xE0090048)
#define r_USBMaxPSize       IO_INT32(0xE009004C)
#define r_USBRxData         IO_INT32(0xE0090018)    /* USB 传输寄存器 */
#define r_USBRxPLen         IO_INT32(0xE0090020)
#define r_USBTxData         IO_INT32(0xE009001C)
#define r_USBTxPLen         IO_INT32(0xE0090024)
#define r_USBCtrl           IO_INT32(0xE0090028)
#define r_USBCmdCode        IO_INT32(0xE0090010)    /* 命令寄存器 */
#define r_USBCmdData        IO_INT32(0xE0090014)
#define r_USBDMARSt         IO_INT32(0xE0090050)    /* DMA 寄存器 */
#define r_USBDMARClr        IO_INT32(0xE0090054)
#define r_USBDMARSet        IO_INT32(0xE0090058)
#define r_USBUDCAH          IO_INT32(0xE0090080)
#define r_USBEpDMASt        IO_INT32(0xE0090084)
#define r_USBEpDMAEn        IO_INT32(0xE0090088)
#define r_USBEpDMADis       IO_INT32(0xE009008C)
#define r_USBDMAIntSt       IO_INT32(0xE0090090)
#define r_USBDMAIntEn       IO_INT32(0xE0090094)
#define r_USBEoTIntSt       IO_INT32(0xE00900A0)
#define r_USBEoTIntClr      IO_INT32(0xE00900A4)
#define r_USBEoTIntSet      IO_INT32(0xE00900A8)
#define r_USBNDDRIntSt      IO_INT32(0xE00900AC)
#define r_USBNDDRIntClr     IO_INT32(0xE00900B0)
#define r_USBNDDRIntSet     IO_INT32(0xE00900B4)
#define r_USBSysErrIntSt    IO_INT32(0xE00900B8)
#define r_USBSysErrIntClr   IO_INT32(0xE00900BC)
#define r_USBSysErrIntSet   IO_INT32(0xE00900C0)

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
#define r_AD0GDR            IO_INT32(0xE0034004)
#define r_ADGSR             IO_INT32(0xE0034008)
#define r_AD0STAT           IO_INT32(0xE0034030)
#define r_AD0INTEN          IO_INT32(0xE003400C)
#define r_AD0DR0            IO_INT32(0xE0034010)
#define r_AD0DR1            IO_INT32(0xE0034014)
#define r_AD0DR2            IO_INT32(0xE0034018)
#define r_AD0DR3            IO_INT32(0xE003401C)
#define r_AD0DR4            IO_INT32(0xE0034020)
#define r_AD0DR5            IO_INT32(0xE0034024)
#define r_AD0DR6            IO_INT32(0xE0034028)
#define r_AD0DR7            IO_INT32(0xE003402C)
#define r_AD1CR             IO_INT32(0xE0060000)
#define r_AD1GDR            IO_INT32(0xE0060004)
#define r_AD1STAT           IO_INT32(0xE0060030)
#define r_AD1INTEN          IO_INT32(0xE006000C)
#define r_AD1DR0            IO_INT32(0xE0060010)
#define r_AD1DR1            IO_INT32(0xE0060014)
#define r_AD1DR2            IO_INT32(0xE0060018)
#define r_AD1DR3            IO_INT32(0xE006001C)
#define r_AD1DR4            IO_INT32(0xE0060020)
#define r_AD1DR5            IO_INT32(0xE0060024)
#define r_AD1DR6            IO_INT32(0xE0060028)
#define r_AD1DR7            IO_INT32(0xE006002C)

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

#endif  /* !__CR_LPC214X_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
