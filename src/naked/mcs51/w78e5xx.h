/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-09-14  */
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
/*  >>>>>>>>>>>>> CrHack WINBOND W78E5XXX 系列 MCU 定义头文件 <<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_W78E5XX_H__
#define __CR_W78E5XX_H__

#include "intel5x.h"

/*****************************************************************************/
/*                                字节寄存器                                 */
/*****************************************************************************/

#if     defined(_CR_CC_CX51_)

    sfr16   r_P40A      =   0x84;
    sfr     r_P40AL     =   0x84;
    sfr     r_P40AH     =   0x85;
    sfr     r_P0UPR     =   0x86;
    sfr     r_AUXR      =   0x8E;
    sfr     r_WDTC      =   0x8F;
    sfr16   r_P41A      =   0x94;
    sfr     r_P41AL     =   0x94;
    sfr     r_P41AH     =   0x95;
    sfr16   r_P42A      =   0xAC;
    sfr     r_P42AL     =   0xAC;
    sfr     r_P42AH     =   0xAD;
    sfr     r_P2ECON    =   0xAE;
    sfr16   r_P43A      =   0xB4;
    sfr     r_P43AL     =   0xB4;
    sfr     r_P43AH     =   0xB5;
    sfr     r_CHPCON    =   0xBF;
    sfr     r_XICON     =   0xC0;
    sfr     r_P4CONA    =   0xC2;
    sfr     r_P4CONB    =   0xC3;
    sfr16   r_SFRA      =   0xC4;
    sfr     r_SFRAL     =   0xC4;
    sfr     r_SFRAH     =   0xC5;
    sfr     r_SFRFD     =   0xC6;
    sfr     r_SFRCN     =   0xC7;
    sfr     r_P4        =   0xD8;
    sfr     r_CHPENR    =   0xF6;

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

#endif  /* !__CR_W78E5XX_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
