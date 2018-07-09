/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2018-07-09  */
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
/*  >>>>>>>>>>>>>> CrHack NUVOTON N76E003 系列 MCU 定义头文件 <<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_N76E003_H__
#define __CR_N76E003_H__

#include "intel5x.h"

/*****************************************************************************/
/*                                字节寄存器                                 */
/*****************************************************************************/

#if     defined(_CR_CC_CX51_)

    sfr     r_RCTRIM0   =   0x84;
    sfr     r_RCTRIM1   =   0x85;
    sfr     r_RWK       =   0x86;
    sfr     r_CKCON     =   0x8E;
    sfr     r_WKCON     =   0x8F;
    sfr     r_SFRS      =   0x91;
    sfr     r_CAPCON0   =   0x92;
    sfr     r_CAPCON1   =   0x93;
    sfr     r_CAPCON2   =   0x94;
    sfr     r_CKDIV     =   0x95;
    sfr     r_CKSWT     =   0x96;
    sfr     r_CKEN      =   0x97;
    sfr     r_SBUF_1    =   0x9A;
    sfr     r_EIE       =   0x9B;
    sfr     r_EIE1      =   0x9C;
    sfr     r_CHPCON    =   0x9F;
    sfr     r_AUXR1     =   0xA2;
    sfr     r_BODCON0   =   0xA3;
    sfr     r_IAPTRG    =   0xA4;
    sfr     r_IAPUEN    =   0xA5;
    sfr16   r_IAPA      =   0xA6;
    sfr     r_IAPAL     =   0xA6;
    sfr     r_IAPAH     =   0xA7;
    sfr     r_SADDR     =   0xA9;
    sfr     r_WDCON     =   0xAA;
    sfr     r_BODCON1   =   0xAB;
    sfr     r_P3M1      =   0xAC;
    sfr     r_P3S       =   0xAC;
    sfr     r_P3M2      =   0xAD;
    sfr     r_P3SR      =   0xAD;
    sfr     r_IAPFD     =   0xAE;
    sfr     r_IAPCN     =   0xAF;
    sfr     r_P0M1      =   0xB1;
    sfr     r_P0S       =   0xB1;
    sfr     r_P0M2      =   0xB2;
    sfr     r_P0SR      =   0xB2;
    sfr     r_P1M1      =   0xB3;
    sfr     r_P1S       =   0xB3;
    sfr     r_P1M2      =   0xB4;
    sfr     r_P1SR      =   0xB4;
    sfr     r_P2S       =   0xB5;
    sfr     r_IPH       =   0xB7;
    sfr     r_PWMINTC   =   0xB7;
    sfr     r_SADEN     =   0xB9;
    sfr     r_SADEN_1   =   0xBA;
    sfr     r_SADDR_1   =   0xBB;
    sfr     r_I2DAT     =   0xBC;
    sfr     r_I2STAT    =   0xBD;
    sfr     r_I2CLK     =   0xBE;
    sfr     r_I2TOC     =   0xBF;
    sfr     r_I2CON     =   0xC0;
    sfr     r_I2ADDR    =   0xC1;
    sfr16   r_ADCR      =   0xC2;
    sfr     r_ADCRL     =   0xC2;
    sfr     r_ADCRH     =   0xC3;
    sfr     r_T3CON     =   0xC4;
    sfr     r_PWM4H     =   0xC4;
    sfr16   r_T3R       =   0xC5;
    sfr     r_RL3       =   0xC5;
    sfr     r_PWM5H     =   0xC5;
    sfr     r_RH3       =   0xC6;
    sfr     r_PIOCON1   =   0xC6;
    sfr     r_TA        =   0xC7;
    sfr     r_PWM4L     =   0xCC;
    sfr     r_PWM5L     =   0xCD;
    sfr16   r_ADCMP     =   0xCE;
    sfr     r_ADCMPL    =   0xCE;
    sfr     r_ADCMPH    =   0xCF;
    sfr     r_PWMPH     =   0xD1;
    sfr     r_PWM0H     =   0xD2;
    sfr     r_PWM1H     =   0xD3;
    sfr     r_PWM2H     =   0xD4;
    sfr     r_PWM3H     =   0xD5;
    sfr     r_PNP       =   0xD6;
    sfr     r_FBD       =   0xD7;
    sfr     r_PWMCON0   =   0xD8;
    sfr     r_PWMPL     =   0xD9;
    sfr     r_PWM0L     =   0xDA;
    sfr     r_PWM1L     =   0xDB;
    sfr     r_PWM2L     =   0xDC;
    sfr     r_PWM3L     =   0xDD;
    sfr     r_PIOCON0   =   0xDE;
    sfr     r_PWMCON1   =   0xDF;
    sfr     r_ADCCON1   =   0xE1;
    sfr     r_ADCCON2   =   0xE2;
    sfr     r_ADCDLY    =   0xE3;
    sfr16   r_C0        =   0xE4;
    sfr     r_C0L       =   0xE4;
    sfr     r_C0H       =   0xE5;
    sfr16   r_C1        =   0xE6;
    sfr     r_C1L       =   0xE6;
    sfr     r_C1H       =   0xE7;
    sfr     r_ADCCON0   =   0xE8;
    sfr     r_PICON     =   0xE9;
    sfr     r_PINEN     =   0xEA;
    sfr     r_PIPEN     =   0xEB;
    sfr     r_PIF       =   0xEC;
    sfr16   r_C2        =   0xED;
    sfr     r_C2L       =   0xED;
    sfr     r_C2H       =   0xEE;
    sfr     r_EIP       =   0xEF;
    sfr     r_CAPCON3   =   0xF1;
    sfr     r_CAPCON4   =   0xF2;
    sfr     r_SPCR      =   0xF3;
    sfr     r_SPCR2     =   0xF3;
    sfr     r_SPSR      =   0xF4;
    sfr     r_SPDR      =   0xF5;
    sfr     r_AINDIDS   =   0xF6;
    sfr     r_EIPH      =   0xF7;
    sfr     r_SCON_1    =   0xF8;
    sfr     r_PDTEN     =   0xF9;
    sfr     r_PDTCNT    =   0xFA;
    sfr     r_PMEN      =   0xFB;
    sfr     r_PMD       =   0xFC;
    sfr     r_PORDIS    =   0xFD;
    sfr     r_EIP1      =   0xFE;
    sfr     r_EIPH1     =   0xFF;

#endif  /* CP TYPE predefines */

/*****************************************************************************/
/*                                寄存器位域                                 */
/*****************************************************************************/

#if     defined(_CR_CC_CX51_)

    /***** IP *****/
    sbit    b_PADC      =   r_IP^6;
    sbit    b_PBOD      =   r_IP^5;

    /***** IE *****/
    sbit    b_EADC      =   r_IE^6;
    sbit    b_EBOD      =   r_IE^5;

    /***** SCON_1 *****/
    sbit    b_SM0_1     =   r_SCON_1^7;
    sbit    b_SM1_1     =   r_SCON_1^6;
    sbit    b_SM2_1     =   r_SCON_1^5;
    sbit    b_REN_1     =   r_SCON_1^4;
    sbit    b_TB8_1     =   r_SCON_1^3;
    sbit    b_RB8_1     =   r_SCON_1^2;
    sbit    b_TI_1      =   r_SCON_1^1;
    sbit    b_RI_1      =   r_SCON_1^0;

    /***** ADCCON0 *****/
    sbit    b_ADCF      =   r_ADCCON0^7;
    sbit    b_ADCS      =   r_ADCCON0^6;
    sbit    b_ETGSEL1   =   r_ADCCON0^5;
    sbit    b_ETGSEL0   =   r_ADCCON0^4;
    sbit    b_ADCHS3    =   r_ADCCON0^3;
    sbit    b_ADCHS2    =   r_ADCCON0^2;
    sbit    b_ADCHS1    =   r_ADCCON0^1;
    sbit    b_ADCHS0    =   r_ADCCON0^0;

    /***** PWMCON0 *****/
    sbit    b_PWMRUN    =   r_PWMCON0^7;
    sbit    b_LOAD      =   r_PWMCON0^6;
    sbit    b_PWMF      =   r_PWMCON0^5;
    sbit    b_CLRPWM    =   r_PWMCON0^4;

    /***** I2CON *****/
    sbit    b_I2CEN     =   r_I2CON^6;
    sbit    b_STA       =   r_I2CON^5;
    sbit    b_STO       =   r_I2CON^4;
    sbit    b_SI        =   r_I2CON^3;
    sbit    b_AA        =   r_I2CON^2;
    sbit    b_I2CPX     =   r_I2CON^0;

#endif  /* CP TYPE predefines */

#endif  /* !__CR_N76E003_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
