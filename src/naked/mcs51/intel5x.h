/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-09-13  */
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
/*  >>>>>>>>>>>>>>> CrHack INTEL MCS-51 兼容 MCU 定义头文件 <<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_INTEL5X_H__
#define __CR_INTEL5X_H__

#include "defs.h"

/*****************************************************************************/
/*                                字节寄存器                                 */
/*****************************************************************************/

#if     defined(_CR_CC_CX51_)

    sfr     r_P0        =   0x80;
    sfr     r_SP        =   0x81;
    sfr16   r_DPTR      =   0x82;
    sfr     r_DPL       =   0x82;
    sfr     r_DPH       =   0x83;
    sfr     r_PCON      =   0x87;
    sfr     r_TCON      =   0x88;
    sfr     r_TMOD      =   0x89;
    sfr     r_TL0       =   0x8A;
    sfr     r_TL1       =   0x8B;
    sfr     r_TH0       =   0x8C;
    sfr     r_TH1       =   0x8D;
    sfr     r_P1        =   0x90;
    sfr     r_SCON      =   0x98;
    sfr     r_SBUF      =   0x99;
    sfr     r_P2        =   0xA0;
    sfr     r_IE        =   0xA8;
    sfr     r_P3        =   0xB0;
    sfr     r_IP        =   0xB8;
    sfr     r_T2CON     =   0xC8;
    sfr     r_T2MOD     =   0xC9;
    sfr16   r_RCAP2     =   0xCA;
    sfr     r_RCAP2L    =   0xCA;
    sfr     r_RCAP2H    =   0xCB;
    sfr16   r_T2        =   0xCC;
    sfr     r_TL2       =   0xCC;
    sfr     r_TH2       =   0xCD;
    sfr     r_PSW       =   0xD0;
    sfr     r_ACC       =   0xE0;
    sfr     r_B         =   0xF0;

#endif  /* CP TYPE predefines */

/*****************************************************************************/
/*                                寄存器位域                                 */
/*****************************************************************************/

#if     defined(_CR_CC_CX51_)

    /***** B *****/
    sbit    b_B7        =   r_B^7;
    sbit    b_B6        =   r_B^6;
    sbit    b_B5        =   r_B^5;
    sbit    b_B4        =   r_B^4;
    sbit    b_B3        =   r_B^3;
    sbit    b_B2        =   r_B^2;
    sbit    b_B1        =   r_B^1;
    sbit    b_B0        =   r_B^0;

    /***** ACC *****/
    sbit    b_ACC7      =   r_ACC^7;
    sbit    b_ACC6      =   r_ACC^6;
    sbit    b_ACC5      =   r_ACC^5;
    sbit    b_ACC4      =   r_ACC^4;
    sbit    b_ACC3      =   r_ACC^3;
    sbit    b_ACC2      =   r_ACC^2;
    sbit    b_ACC1      =   r_ACC^1;
    sbit    b_ACC0      =   r_ACC^0;

    /***** PSW *****/
    sbit    b_CY        =   r_PSW^7;
    sbit    b_AC        =   r_PSW^6;
    sbit    b_F0        =   r_PSW^5;
    sbit    b_RS1       =   r_PSW^4;
    sbit    b_RS0       =   r_PSW^3;
    sbit    b_OV        =   r_PSW^2;
    sbit    b_F1        =   r_PSW^1;
    sbit    b_P         =   r_PSW^0;

    /***** TCON *****/
    sbit    b_TF1       =   r_TCON^7;
    sbit    b_TR1       =   r_TCON^6;
    sbit    b_TF0       =   r_TCON^5;
    sbit    b_TR0       =   r_TCON^4;
    sbit    b_IE1       =   r_TCON^3;
    sbit    b_IT1       =   r_TCON^2;
    sbit    b_IE0       =   r_TCON^1;
    sbit    b_IT0       =   r_TCON^0;

    /***** T2CON *****/
    sbit    b_TF2       =   r_T2CON^7;
    sbit    b_EXF2      =   r_T2CON^6;
    sbit    b_RCLK      =   r_T2CON^5;
    sbit    b_TCLK      =   r_T2CON^4;
    sbit    b_EXEN2     =   r_T2CON^3;
    sbit    b_TR2       =   r_T2CON^2;
    sbit    b_C_T2      =   r_T2CON^1;
    sbit    b_CP_RL2    =   r_T2CON^0;

    /***** IE *****/
    sbit    b_EA        =   r_IE^7;
    sbit    b_ET2       =   r_IE^5;
    sbit    b_ES        =   r_IE^4;
    sbit    b_ET1       =   r_IE^3;
    sbit    b_EX1       =   r_IE^2;
    sbit    b_ET0       =   r_IE^1;
    sbit    b_EX0       =   r_IE^0;

    /***** IP *****/
    sbit    b_PT2       =   r_IP^5;
    sbit    b_PS        =   r_IP^4;
    sbit    b_PT1       =   r_IP^3;
    sbit    b_PX1       =   r_IP^2;
    sbit    b_PT0       =   r_IP^1;
    sbit    b_PX0       =   r_IP^0;

    /***** P0 *****/
    sbit    b_P0_7      =   r_P0^7;
    sbit    b_P0_6      =   r_P0^6;
    sbit    b_P0_5      =   r_P0^5;
    sbit    b_P0_4      =   r_P0^4;
    sbit    b_P0_3      =   r_P0^3;
    sbit    b_P0_2      =   r_P0^2;
    sbit    b_P0_1      =   r_P0^1;
    sbit    b_P0_0      =   r_P0^0;

    /***** P1 *****/
    sbit    b_P1_7      =   r_P1^7;
    sbit    b_P1_6      =   r_P1^6;
    sbit    b_P1_5      =   r_P1^5;
    sbit    b_P1_4      =   r_P1^4;
    sbit    b_P1_3      =   r_P1^3;
    sbit    b_P1_2      =   r_P1^2;
    sbit    b_P1_1      =   r_P1^1;
    sbit    b_P1_0      =   r_P1^0;

    /***** P2 *****/
    sbit    b_P2_7      =   r_P2^7;
    sbit    b_P2_6      =   r_P2^6;
    sbit    b_P2_5      =   r_P2^5;
    sbit    b_P2_4      =   r_P2^4;
    sbit    b_P2_3      =   r_P2^3;
    sbit    b_P2_2      =   r_P2^2;
    sbit    b_P2_1      =   r_P2^1;
    sbit    b_P2_0      =   r_P2^0;

    /***** P3 *****/
    sbit    b_P3_7      =   r_P3^7;
    sbit    b_P3_6      =   r_P3^6;
    sbit    b_P3_5      =   r_P3^5;
    sbit    b_P3_4      =   r_P3^4;
    sbit    b_P3_3      =   r_P3^3;
    sbit    b_P3_2      =   r_P3^2;
    sbit    b_P3_1      =   r_P3^1;
    sbit    b_P3_0      =   r_P3^0;

    /***** SCON *****/
    sbit    b_SM0       =   r_SCON^7;
    sbit    b_SM1       =   r_SCON^6;
    sbit    b_SM2       =   r_SCON^5;
    sbit    b_REN       =   r_SCON^4;
    sbit    b_TB8       =   r_SCON^3;
    sbit    b_RB8       =   r_SCON^2;
    sbit    b_TI        =   r_SCON^1;
    sbit    b_RI        =   r_SCON^0;

#endif  /* CP TYPE predefines */

#endif  /* !__CR_INTEL5X_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
