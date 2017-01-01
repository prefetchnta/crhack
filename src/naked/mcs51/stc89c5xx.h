/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2011-03-29  */
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
/*  >>>>>>>>>>>>>> CrHack 宏晶 STC89C5XX 系列 MCU 定义头文件 <<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_STC89C5XX_H__
#define __CR_STC89C5XX_H__

#include "intel5x.h"

/*****************************************************************************/
/*                                字节寄存器                                 */
/*****************************************************************************/

#if     defined(_CR_CC_CX51_)

    sfr     r_AUXR      =   0x8E;
    sfr     r_AUXR1     =   0xA2;
    sfr     r_SADDR     =   0xA9;
    sfr     r_IPH       =   0xB7;
    sfr     r_SADEN     =   0xB9;
    sfr     r_XICON     =   0xC0;
    sfr     r_WDT_CONTR =   0xE1;
    sfr     r_ISP_DATA  =   0xE2;
    sfr     r_ISP_ADDRH =   0xE3;
    sfr     r_ISP_ADDRL =   0xE4;
    sfr     r_ISP_CMD   =   0xE5;
    sfr     r_ISP_TRIG  =   0xE6;
    sfr     r_ISP_CONTR =   0xE7;
    sfr     r_P4        =   0xE8;

#endif  /* CP TYPE predefines */

/*****************************************************************************/
/*                                寄存器位域                                 */
/*****************************************************************************/

#if     defined(_CR_CC_CX51_)

    /***** XICON *****/
    sbit    b_PX3   =   r_XICON^7;
    sbit    b_EX3   =   r_XICON^6;
    sbit    b_IE3   =   r_XICON^5;
    sbit    b_IT3   =   r_XICON^4;
    sbit    b_PX2   =   r_XICON^3;
    sbit    b_EX2   =   r_XICON^2;
    sbit    b_IE2   =   r_XICON^1;
    sbit    b_IT2   =   r_XICON^0;

    /***** P4 *****/
    sbit    b_P4_3  =   r_P4^3;
    sbit    b_P4_2  =   r_P4^2;
    sbit    b_P4_1  =   r_P4^1;
    sbit    b_P4_0  =   r_P4^0;

#endif  /* CP TYPE predefines */

#endif  /* !__CR_STC89C5XX_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
