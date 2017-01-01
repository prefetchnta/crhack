/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2011-04-21  */
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
/*  >>>>>>>>>>>>>>>> CrHack SAMSUNG S3C2440A CPU 定义头文件 <<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_S3C2440A_H__
#define __CR_S3C2440A_H__

#include "defs.h"

/*****************************************************************************/
/*                              内存控制寄存器                               */
/*****************************************************************************/

#define r_BWSCON                IO_INT32(0x48000000)
#define r_BANKCON0              IO_INT32(0x48000004)
#define r_BANKCON1              IO_INT32(0x48000008)
#define r_BANKCON2              IO_INT32(0x4800000C)
#define r_BANKCON3              IO_INT32(0x48000010)
#define r_BANKCON4              IO_INT32(0x48000014)
#define r_BANKCON5              IO_INT32(0x48000018)
#define r_BANKCON6              IO_INT32(0x4800001C)
#define r_BANKCON7              IO_INT32(0x48000020)
#define r_REFRESH               IO_INT32(0x48000024)
#define r_BANKSIZE              IO_INT32(0x48000028)
#define r_MRSRB6                IO_INT32(0x4800002C)
#define r_MRSRB7                IO_INT32(0x48000030)

/*****************************************************************************/
/*                              USB 主机寄存器                               */
/*****************************************************************************/

#define r_HcRevision            IO_INT32(0x49000000)
#define r_HcControl             IO_INT32(0x49000004)
#define r_HcCommonStatus        IO_INT32(0x49000008)
#define r_HcInterruptStatus     IO_INT32(0x4900000C)
#define r_HcInterruptEnable     IO_INT32(0x49000010)
#define r_HcInterruptDisable    IO_INT32(0x49000014)
#define r_HcHCCA                IO_INT32(0x49000018)
#define r_HcPeriodCurrentED     IO_INT32(0x4900001C)
#define r_HcControlHeadED       IO_INT32(0x49000020)
#define r_HcControlCurrentED    IO_INT32(0x49000024)
#define r_HcBulkHeadED          IO_INT32(0x49000028)
#define r_HcBulkCurrentED       IO_INT32(0x4900002C)
#define r_HcDoneHead            IO_INT32(0x49000030)
#define r_HcRmInterval          IO_INT32(0x49000034)
#define r_HcFmRemaining         IO_INT32(0x49000038)
#define r_HcFmNumber            IO_INT32(0x4900003C)
#define r_HcPeriodicStart       IO_INT32(0x49000040)
#define r_HcLSThreshold         IO_INT32(0x49000044)
#define r_HcRhDescriptorA       IO_INT32(0x49000048)
#define r_HcRhDescriptorB       IO_INT32(0x4900004C)
#define r_HcRhStatus            IO_INT32(0x49000050)
#define r_HcRhPortStatus1       IO_INT32(0x49000054)
#define r_HcRhPortStatus2       IO_INT32(0x49000058)

/*****************************************************************************/
/*                              中断控制寄存器                               */
/*****************************************************************************/

#define r_SRCPND                IO_INT32(0x4A000000)
#define r_INTMOD                IO_INT32(0x4A000004)
#define r_INTMSK                IO_INT32(0x4A000008)
#define r_PRIORITY              IO_INT32(0x4A00000C)
#define r_INTPND                IO_INT32(0x4A000010)
#define r_INTOFFSET             IO_INT32(0x4A000014)
#define r_SUBSRCPND             IO_INT32(0x4A000018)
#define r_INTSUBMSK             IO_INT32(0x4A00001C)

/*****************************************************************************/
/*                              DMA 控制寄存器                               */
/*****************************************************************************/

#define r_DISRC0                IO_INT32(0x4B000000)
#define r_DISRCC0               IO_INT32(0x4B000004)
#define r_DIDST0                IO_INT32(0x4B000008)
#define r_DIDSTC0               IO_INT32(0x4B00000C)
#define r_DCON0                 IO_INT32(0x4B000010)
#define r_DSTAT0                IO_INT32(0x4B000014)
#define r_DCSRC0                IO_INT32(0x4B000018)
#define r_DCDST0                IO_INT32(0x4B00001C)
#define r_DMASKTRIG0            IO_INT32(0x4B000020)
#define r_DISRC1                IO_INT32(0x4B000040)
#define r_DISRCC1               IO_INT32(0x4B000044)
#define r_DIDST1                IO_INT32(0x4B000048)
#define r_DIDSTC1               IO_INT32(0x4B00004C)
#define r_DCON1                 IO_INT32(0x4B000050)
#define r_DSTAT1                IO_INT32(0x4B000054)
#define r_DCSRC1                IO_INT32(0x4B000058)
#define r_DCDST1                IO_INT32(0x4B00005C)
#define r_DMASKTRIG1            IO_INT32(0x4B000060)
#define r_DISRC2                IO_INT32(0x4B000080)
#define r_DISRCC2               IO_INT32(0x4B000084)
#define r_DIDST2                IO_INT32(0x4B000088)
#define r_DIDSTC2               IO_INT32(0x4B00008C)
#define r_DCON2                 IO_INT32(0x4B000090)
#define r_DSTAT2                IO_INT32(0x4B000094)
#define r_DCSRC2                IO_INT32(0x4B000098)
#define r_DCDST2                IO_INT32(0x4B00009C)
#define r_DMASKTRIG2            IO_INT32(0x4B0000A0)
#define r_DISRC3                IO_INT32(0x4B0000C0)
#define r_DISRCC3               IO_INT32(0x4B0000C4)
#define r_DIDST3                IO_INT32(0x4B0000C8)
#define r_DIDSTC3               IO_INT32(0x4B0000CC)
#define r_DCON3                 IO_INT32(0x4B0000D0)
#define r_DSTAT3                IO_INT32(0x4B0000D4)
#define r_DCSRC3                IO_INT32(0x4B0000D8)
#define r_DCDST3                IO_INT32(0x4B0000DC)
#define r_DMASKTRIG3            IO_INT32(0x4B0000E0)

/*****************************************************************************/
/*                              时钟电源寄存器                               */
/*****************************************************************************/

#define r_LOCKTIME              IO_INT32(0x4C000000)
#define r_MPLLCON               IO_INT32(0x4C000004)
#define r_UPLLCON               IO_INT32(0x4C000008)
#define r_CLKCON                IO_INT32(0x4C00000C)
#define r_CLKSLOW               IO_INT32(0x4C000010)
#define r_CLKDIVN               IO_INT32(0x4C000014)
#define r_CAMDIVN               IO_INT32(0x4C000018)

/*****************************************************************************/
/*                              LCD 设备寄存器                               */
/*****************************************************************************/

#define r_LCDCON1               IO_INT32(0x4D000000)
#define r_LCDCON2               IO_INT32(0x4D000004)
#define r_LCDCON3               IO_INT32(0x4D000008)
#define r_LCDCON4               IO_INT32(0x4D00000C)
#define r_LCDCON5               IO_INT32(0x4D000010)
#define r_LCDSADDR1             IO_INT32(0x4D000014)
#define r_LCDSADDR2             IO_INT32(0x4D000018)
#define r_LCDSADDR3             IO_INT32(0x4D00001C)
#define r_REDLUT                IO_INT32(0x4D000020)
#define r_GREENLUT              IO_INT32(0x4D000024)
#define r_BLUELUT               IO_INT32(0x4D000028)
#define r_DITHMODE              IO_INT32(0x4D00004C)
#define r_TPAL                  IO_INT32(0x4D000050)
#define r_LCDINTPND             IO_INT32(0x4D000054)
#define r_LCDSRCPND             IO_INT32(0x4D000058)
#define r_LCDINTMSK             IO_INT32(0x4D00005C)
#define r_TCONSEL               IO_INT32(0x4D000060)

/*****************************************************************************/
/*                             NAND 设备寄存器                               */
/*****************************************************************************/

#define r_NFCONF                IO_INT32(0x4E000000)
#define r_NFCONT                IO_INT32(0x4E000004)
#define r_NFCMD                 IO_INT32(0x4E000008)
#define r_NFADDR                IO_INT32(0x4E00000C)
#define r_NFDATA                IO_INT32(0x4E000010)
#define r_NFMECC0               IO_INT32(0x4E000014)
#define r_NFMECC1               IO_INT32(0x4E000018)
#define r_NFSECC                IO_INT32(0x4E00001C)
#define r_NFSTAT                IO_INT32(0x4E000020)
#define r_NFESTAT0              IO_INT32(0x4E000024)
#define r_NFESTAT1              IO_INT32(0x4E000028)
#define r_NFMECC0               IO_INT32(0x4E00002C)
#define r_NFMECC1               IO_INT32(0x4E000030)
#define r_NFSECC                IO_INT32(0x4E000034)
#define r_NFSBLK                IO_INT32(0x4E000038)
#define r_NFEBLK                IO_INT32(0x4E00003C)

/*****************************************************************************/
/*                              CAM 设备寄存器                               */
/*****************************************************************************/

#define r_CISRCFMT              IO_INT32(0x4F000000)
#define r_CIWDOFST              IO_INT32(0x4F000004)
#define r_CIGCTRL               IO_INT32(0x4F000008)
#define r_CICOYSA1              IO_INT32(0x4F000018)
#define r_CICOYSA2              IO_INT32(0x4F00001C)
#define r_CICOYSA3              IO_INT32(0x4F000020)
#define r_CICOYSA4              IO_INT32(0x4F000024)
#define r_CICOCBSA1             IO_INT32(0x4F000028)
#define r_CICOCBSA2             IO_INT32(0x4F00002C)
#define r_CICOCBSA3             IO_INT32(0x4F000030)
#define r_CICOCBSA4             IO_INT32(0x4F000034)
#define r_CICOCRSA1             IO_INT32(0x4F000038)
#define r_CICOCRSA2             IO_INT32(0x4F00003C)
#define r_CICOCRSA3             IO_INT32(0x4F000040)
#define r_CICOCRSA4             IO_INT32(0x4F000044)
#define r_CICOTRGFMT            IO_INT32(0x4F000048)
#define r_CICOCTRL              IO_INT32(0x4F00004C)
#define r_CICOSCPRERATIO        IO_INT32(0x4F000050)
#define r_CICOSCPREDST          IO_INT32(0x4F000054)
#define r_CICOSCCTRL            IO_INT32(0x4F000058)
#define r_CICOTAREA             IO_INT32(0x4F00005C)
#define r_CICOSTATUS            IO_INT32(0x4F000064)
#define r_CIPRCLRSA1            IO_INT32(0x4F00006C)
#define r_CIPRCLRSA2            IO_INT32(0x4F000070)
#define r_CIPRCLRSA3            IO_INT32(0x4F000074)
#define r_CIPRCLRSA4            IO_INT32(0x4F000078)
#define r_CIPRTRGFMT            IO_INT32(0x4F00007C)
#define r_CIPRCTRL              IO_INT32(0x4F000080)
#define r_CIPRSCPRERATIO        IO_INT32(0x4F000084)
#define r_CIPRSCPREDST          IO_INT32(0x4F000088)
#define r_CIPRSCCTRL            IO_INT32(0x4F00008C)
#define r_CIPRTAREA             IO_INT32(0x4F000090)
#define r_CIPRSTATUS            IO_INT32(0x4F000098)
#define r_CIIMGCPT              IO_INT32(0x4F0000A0)

/*****************************************************************************/
/*                             UART 设备寄存器                               */
/*****************************************************************************/

#define r_ULCON0                IO_INT32(0x50000000)
#define r_UCON0                 IO_INT32(0x50000004)
#define r_UFCON0                IO_INT32(0x50000008)
#define r_UMCON0                IO_INT32(0x5000000C)
#define r_UTRSTAT0              IO_INT32(0x50000010)
#define r_UERSTAT0              IO_INT32(0x50000014)
#define r_UFSTAT0               IO_INT32(0x50000018)
#define r_UMSTAT0               IO_INT32(0x5000001C)
#define r_UTXH0                 IO_INT08(0x50000020 + oREG08)
#define r_URXH0                 IO_INT08(0x50000024 + oREG08)
#define r_UBRDIV0               IO_INT32(0x50000028)
#define r_ULCON1                IO_INT32(0x50004000)
#define r_UCON1                 IO_INT32(0x50004004)
#define r_UFCON1                IO_INT32(0x50004008)
#define r_UMCON1                IO_INT32(0x5000400C)
#define r_UTRSTAT1              IO_INT32(0x50004010)
#define r_UERSTAT1              IO_INT32(0x50004014)
#define r_UFSTAT1               IO_INT32(0x50004018)
#define r_UMSTAT1               IO_INT32(0x5000401C)
#define r_UTXH1                 IO_INT08(0x50004020 + oREG08)
#define r_URXH1                 IO_INT08(0x50004024 + oREG08)
#define r_UBRDIV1               IO_INT32(0x50004028)
#define r_ULCON2                IO_INT32(0x50008000)
#define r_UCON2                 IO_INT32(0x50008004)
#define r_UFCON2                IO_INT32(0x50008008)
#define r_UTRSTAT2              IO_INT32(0x50008010)
#define r_UERSTAT2              IO_INT32(0x50008014)
#define r_UFSTAT2               IO_INT32(0x50008018)
#define r_UTXH2                 IO_INT08(0x50008020 + oREG08)
#define r_URXH2                 IO_INT08(0x50008024 + oREG08)
#define r_UBRDIV2               IO_INT32(0x50008028)

/*****************************************************************************/
/*                              PWM 定时寄存器                               */
/*****************************************************************************/

#define r_TCFG0                 IO_INT32(0x51000000)
#define r_TCFG1                 IO_INT32(0x51000004)
#define r_TCON                  IO_INT32(0x51000008)
#define r_TCNTB0                IO_INT32(0x5100000C)
#define r_TCMPB0                IO_INT32(0x51000010)
#define r_TCNTO0                IO_INT32(0x51000014)
#define r_TCNTB1                IO_INT32(0x51000018)
#define r_TCMPB1                IO_INT32(0x5100001C)
#define r_TCNTO1                IO_INT32(0x51000020)
#define r_TCNTB2                IO_INT32(0x51000024)
#define r_TCMPB2                IO_INT32(0x51000028)
#define r_TCNTO2                IO_INT32(0x5100002C)
#define r_TCNTB3                IO_INT32(0x51000030)
#define r_TCMPB3                IO_INT32(0x51000034)
#define r_TCNTO3                IO_INT32(0x51000038)
#define r_TCNTB4                IO_INT32(0x5100003C)
#define r_TCNTO4                IO_INT32(0x51000040)

/*****************************************************************************/
/*                              USB 设备寄存器                               */
/*****************************************************************************/

#define r_FUNC_ADDR_REG         IO_INT08(0x52000140 + oREG08)
#define r_PWR_REG               IO_INT08(0x52000144 + oREG08)
#define r_EP_INT_REG            IO_INT08(0x52000148 + oREG08)
#define r_USB_INT_REG           IO_INT08(0x52000158 + oREG08)
#define r_EP_INT_EN_REG         IO_INT08(0x5200015C + oREG08)
#define r_USB_INT_EN_REG        IO_INT08(0x5200016C + oREG08)
#define r_FRAME_NUM1_REG        IO_INT08(0x52000170 + oREG08)
#define r_FRAME_NUM2_REG        IO_INT08(0x52000174 + oREG08)
#define r_INDEX_REG             IO_INT08(0x52000178 + oREG08)
#define r_EP0_CSR               IO_INT08(0x52000184 + oREG08)
#define r_IN_CSR1_REG           IO_INT08(0x52000184 + oREG08)
#define r_IN_CSR2_REG           IO_INT08(0x52000188 + oREG08)
#define r_MAXP_REG              IO_INT08(0x52000180 + oREG08)
#define r_OUT_CSR1_REG          IO_INT08(0x52000190 + oREG08)
#define r_OUT_CSR2_REG          IO_INT08(0x52000194 + oREG08)
#define r_OUT_FIFO_CNT1_REG     IO_INT08(0x52000198 + oREG08)
#define r_OUT_FIFO_CNT2_REG     IO_INT08(0x5200019C + oREG08)
#define r_EP0_FIFO              IO_INT08(0x520001C0 + oREG08)
#define r_EP1_FIFO              IO_INT08(0x520001C4 + oREG08)
#define r_EP2_FIFO              IO_INT08(0x520001C8 + oREG08)
#define r_EP3_FIFO              IO_INT08(0x520001CC + oREG08)
#define r_EP4_FIFO              IO_INT08(0x520001D0 + oREG08)
#define r_EP1_DMA_CON           IO_INT08(0x52000200 + oREG08)
#define r_EP1_DMA_UNIT          IO_INT08(0x52000204 + oREG08)
#define r_EP1_DMA_FIFO          IO_INT08(0x52000208 + oREG08)
#define r_EP1_DMA_TTC_L         IO_INT08(0x5200020C + oREG08)
#define r_EP1_DMA_TTC_M         IO_INT08(0x52000210 + oREG08)
#define r_EP1_DMA_TTC_H         IO_INT08(0x52000214 + oREG08)
#define r_EP2_DMA_CON           IO_INT08(0x52000218 + oREG08)
#define r_EP2_DMA_UNIT          IO_INT08(0x5200021C + oREG08)
#define r_EP2_DMA_FIFO          IO_INT08(0x52000220 + oREG08)
#define r_EP2_DMA_TTC_L         IO_INT08(0x52000224 + oREG08)
#define r_EP2_DMA_TTC_M         IO_INT08(0x52000228 + oREG08)
#define r_EP2_DMA_TTC_H         IO_INT08(0x5200022C + oREG08)
#define r_EP3_DMA_CON           IO_INT08(0x52000240 + oREG08)
#define r_EP3_DMA_UNIT          IO_INT08(0x52000244 + oREG08)
#define r_EP3_DMA_FIFO          IO_INT08(0x52000248 + oREG08)
#define r_EP3_DMA_TTC_L         IO_INT08(0x5200024C + oREG08)
#define r_EP3_DMA_TTC_M         IO_INT08(0x52000250 + oREG08)
#define r_EP3_DMA_TTC_H         IO_INT08(0x52000254 + oREG08)
#define r_EP4_DMA_CON           IO_INT08(0x52000258 + oREG08)
#define r_EP4_DMA_UNIT          IO_INT08(0x5200025C + oREG08)
#define r_EP4_DMA_FIFO          IO_INT08(0x52000260 + oREG08)
#define r_EP4_DMA_TTC_L         IO_INT08(0x52000264 + oREG08)
#define r_EP4_DMA_TTC_M         IO_INT08(0x52000268 + oREG08)
#define r_EP4_DMA_TTC_H         IO_INT08(0x5200026C + oREG08)

/*****************************************************************************/
/*                              WDT 设备寄存器                               */
/*****************************************************************************/

#define r_WTCON                 IO_INT32(0x53000000)
#define r_WTDAT                 IO_INT32(0x53000004)
#define r_WTCNT                 IO_INT32(0x53000008)

/*****************************************************************************/
/*                              I2C 设备寄存器                               */
/*****************************************************************************/

#define r_IICCON                IO_INT32(0x54000000)
#define r_IICSTAT               IO_INT32(0x54000004)
#define r_IICADD                IO_INT32(0x54000008)
#define r_IICDS                 IO_INT32(0x5400000C)
#define r_IICLC                 IO_INT32(0x54000010)

/*****************************************************************************/
/*                              I2S 设备寄存器                               */
/*****************************************************************************/

#define r_IISCON                IO_INT16(0x55000000 + oREG16)
#define r_IISMOD                IO_INT16(0x55000004 + oREG16)
#define r_IISPSR                IO_INT16(0x55000008 + oREG16)
#define r_IISFCON               IO_INT16(0x5500000C + oREG16)
#define r_IISFIFO               IO_INT16(0x55000010 + oREG16)

/*****************************************************************************/
/*                             GPIO 设备寄存器                               */
/*****************************************************************************/

#define r_GPACON                IO_INT32(0x56000000)
#define r_GPADAT                IO_INT32(0x56000004)
#define r_GPBCON                IO_INT32(0x56000010)
#define r_GPBDAT                IO_INT32(0x56000014)
#define r_GPBUP                 IO_INT32(0x56000018)
#define r_GPCCON                IO_INT32(0x56000020)
#define r_GPCDAT                IO_INT32(0x56000024)
#define r_GPCUP                 IO_INT32(0x56000028)
#define r_GPDCON                IO_INT32(0x56000030)
#define r_GPDDAT                IO_INT32(0x56000034)
#define r_GPDUP                 IO_INT32(0x56000038)
#define r_GPECON                IO_INT32(0x56000040)
#define r_GPEDAT                IO_INT32(0x56000044)
#define r_GPEUP                 IO_INT32(0x56000048)
#define r_GPFCON                IO_INT32(0x56000050)
#define r_GPFDAT                IO_INT32(0x56000054)
#define r_GPFUP                 IO_INT32(0x56000058)
#define r_GPGCON                IO_INT32(0x56000060)
#define r_GPGDAT                IO_INT32(0x56000064)
#define r_GPGUP                 IO_INT32(0x56000068)
#define r_GPHCON                IO_INT32(0x56000070)
#define r_GPHDAT                IO_INT32(0x56000074)
#define r_GPHUP                 IO_INT32(0x56000078)
#define r_GPJCON                IO_INT32(0x560000D0)
#define r_GPJDAT                IO_INT32(0x560000D4)
#define r_GPJUP                 IO_INT32(0x560000D8)
#define r_MISCCR                IO_INT32(0x56000080)
#define r_DCLKCON               IO_INT32(0x56000084)
#define r_EXTINT0               IO_INT32(0x56000088)
#define r_EXTINT1               IO_INT32(0x5600008C)
#define r_EXTINT2               IO_INT32(0x56000090)
#define r_EINTFLT0              IO_INT32(0x56000094)
#define r_EINTFLT1              IO_INT32(0x56000098)
#define r_EINTFLT2              IO_INT32(0x5600009C)
#define r_EINTFLT3              IO_INT32(0x560000A0)
#define r_EINTMASK              IO_INT32(0x560000A4)
#define r_EINTPEND              IO_INT32(0x560000A8)
#define r_GSTATUS0              IO_INT32(0x560000AC)
#define r_GSTATUS1              IO_INT32(0x560000B0)
#define r_GSTATUS2              IO_INT32(0x560000B4)
#define r_GSTATUS3              IO_INT32(0x560000B8)
#define r_GSTATUS4              IO_INT32(0x560000BC)
#define r_MSLCON                IO_INT32(0x560000CC)

/*****************************************************************************/
/*                              RTC 设备寄存器                               */
/*****************************************************************************/

#define r_RTCCON                IO_INT08(0x57000040 + oREG08)
#define r_TICNT                 IO_INT08(0x57000044 + oREG08)
#define r_RTCALM                IO_INT08(0x57000050 + oREG08)
#define r_ALMSEC                IO_INT08(0x57000054 + oREG08)
#define r_ALMMIN                IO_INT08(0x57000058 + oREG08)
#define r_ALMHOUR               IO_INT08(0x5700005C + oREG08)
#define r_ALMDATE               IO_INT08(0x57000060 + oREG08)
#define r_ALMMON                IO_INT08(0x57000064 + oREG08)
#define r_ALMYEAR               IO_INT08(0x57000068 + oREG08)
#define r_BCDSEC                IO_INT08(0x57000070 + oREG08)
#define r_BCDMIN                IO_INT08(0x57000074 + oREG08)
#define r_BCDHOUR               IO_INT08(0x57000078 + oREG08)
#define r_BCDDATE               IO_INT08(0x5700007C + oREG08)
#define r_BCDDAY                IO_INT08(0x57000080 + oREG08)
#define r_BCDMON                IO_INT08(0x57000084 + oREG08)
#define r_BCDYEAR               IO_INT08(0x57000088 + oREG08)

/*****************************************************************************/
/*                              ADC 设备寄存器                               */
/*****************************************************************************/

#define r_ADCCON                IO_INT32(0x58000000)
#define r_ADCTSC                IO_INT32(0x58000004)
#define r_ADCDLY                IO_INT32(0x58000008)
#define r_ADCDAT0               IO_INT32(0x5800000C)
#define r_ADCDAT1               IO_INT32(0x58000010)
#define r_ADCUPDN               IO_INT32(0x58000014)

/*****************************************************************************/
/*                              SPI 设备寄存器                               */
/*****************************************************************************/

#define r_SPCON0                IO_INT32(0x59000000)
#define r_SPSTA0                IO_INT32(0x59000004)
#define r_SPPIN0                IO_INT32(0x59000008)
#define r_SPPRE0                IO_INT32(0x5900000C)
#define r_SPTDAT0               IO_INT32(0x59000010)
#define r_SPRDAT0               IO_INT32(0x59000014)
#define r_SPCON1                IO_INT32(0x59000020)
#define r_SPSTA1                IO_INT32(0x59000024)
#define r_SPPIN1                IO_INT32(0x59000028)
#define r_SPPRE1                IO_INT32(0x5900002C)
#define r_SPTDAT1               IO_INT32(0x59000030)
#define r_SPRDAT1               IO_INT32(0x59000034)

/*****************************************************************************/
/*                             SD 卡接口寄存器                               */
/*****************************************************************************/

#define r_SDICON                IO_INT32(0x5A000000)
#define r_SDIPRE                IO_INT32(0x5A000004)
#define r_SDICARG               IO_INT32(0x5A000008)
#define r_SDICCON               IO_INT32(0x5A00000C)
#define r_SDICSTA               IO_INT32(0x5A000010)
#define r_SDIRSP0               IO_INT32(0x5A000014)
#define r_SDIRSP1               IO_INT32(0x5A000018)
#define r_SDIRSP2               IO_INT32(0x5A00001C)
#define r_SDIRSP3               IO_INT32(0x5A000020)
#define r_SDIDTIMER             IO_INT32(0x5A000024)
#define r_SDIBSIZE              IO_INT32(0x5A000028)
#define r_SDIDCON               IO_INT32(0x5A00002C)
#define r_SDIDCNT               IO_INT32(0x5A000030)
#define r_SDIDSTA               IO_INT32(0x5A000034)
#define r_SDIFSTA               IO_INT32(0x5A000038)
#define r_SDIIMSK               IO_INT32(0x5A00003C)
#define r_SDIDAT                IO_INT08(0x5A000040 + oREG08)

/*****************************************************************************/
/*                             AC97 设备寄存器                               */
/*****************************************************************************/

#define r_AC_GLBCTRL            IO_INT32(0x5B000000)
#define r_AC_GLBSTAT            IO_INT32(0x5B000004)
#define r_AC_CODEC_CMD          IO_INT32(0x5B000008)
#define r_AC_CODEC_STAT         IO_INT32(0x5B00000C)
#define r_AC_PCMADDR            IO_INT32(0x5B000010)
#define r_AC_MICADDR            IO_INT32(0x5B000014)
#define r_AC_PCMDATA            IO_INT32(0x5B000018)
#define r_AC_MICDATA            IO_INT32(0x5B00001C)

#endif  /* !__CR_S3C2440A_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
