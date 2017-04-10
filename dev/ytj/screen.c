/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2017-03-05  */
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
/*  >>>>>>>>>>>>>>>>>>> YTJ 一体机 LED 屏显示驱动函数库 <<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#define _YTJ_HS08_
#include "device.h"
#include "memlib.h"
#include "stm32f10x_conf.h"

/* 前后台缓冲区 */
static byte_t*  s_front;
static byte_t*  s_backs;

/* 三色 64x64 显示区 */
static byte_t   s_screen1[SCREEN_SIZE];
static byte_t   s_screen2[SCREEN_SIZE];

/*
=======================================
    屏幕初始化
=======================================
*/
CR_API void_t
screen_init (void_t)
{
    TIM_TimeBaseInitTypeDef sttb;

    /* 初始化显示区 */
    mem_zero(s_screen1, sizeof(s_screen1));
    mem_zero(s_screen2, sizeof(s_screen2));
    s_front = s_screen1;
    s_backs = s_screen2;

    /* 初始化刷屏定时器 (0.5ms) */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    TIM_DeInit(TIM2);
    sttb.TIM_Period = 3599;
    sttb.TIM_Prescaler = 9;
    sttb.TIM_ClockDivision = TIM_CKD_DIV1;
    sttb.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &sttb);
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
}

/*
=======================================
    前后台切换
=======================================
*/
CR_API byte_t*
screen_flip (void_t)
{
    if (s_front == s_screen1) {
        s_front = s_screen2;
        s_backs = s_screen1;
        mem_zero(s_screen1, sizeof(s_screen1));
        return (s_screen1);
    }
    s_front = s_screen1;
    s_backs = s_screen2;
    mem_zero(s_screen2, sizeof(s_screen2));
    return (s_screen2);
}

/*
=======================================
    返回前台缓冲区
=======================================
*/
CR_API byte_t*
screen_main (void_t)
{
    return (s_front);
}

/*
=======================================
    返回后台缓冲区
=======================================
*/
CR_API byte_t*
screen_back (void_t)
{
    return (s_backs);
}

/*
=======================================
    读点
=======================================
*/
CR_API byte_t
pixel_get02z (
  __CR_IN__ sint_t  x,
  __CR_IN__ sint_t  y
    )
{
    leng_t  line, offs;

    if (x < 0 || x >= SCREEN_WIDTH ||
        y < 0 || y >= SCREEN_HEIGHT)
        return (DOT_BL);
    line = (leng_t)(y * SCREEN_BPL + (x / 4));
    offs = (leng_t)(6 - (x % 4) * 2);
    return ((s_backs[line] >> offs) & 3);
}

/*
=======================================
    写点
=======================================
*/
CR_API void_t
pixel_set02z (
  __CR_IN__ sint_t  x,
  __CR_IN__ sint_t  y,
  __CR_IN__ byte_t  c
    )
{
    leng_t  line, offs;

    if (x < 0 || x >= SCREEN_WIDTH ||
        y < 0 || y >= SCREEN_HEIGHT)
        return;
    line = (leng_t)(y * SCREEN_BPL + (x / 4));
    offs = (leng_t)(6 - (x % 4) * 2);
    s_backs[line] &= ~(3 << offs);
    s_backs[line] |= ((c & 3) << offs);
}

/*
---------------------------------------
    刷新显示
---------------------------------------
*/
static void_t
screen_refresh (void_t)
{
    uint_t          ii, jj;
    uint_t          line_ee;
    byte_t          tmp[4] = {0};
    static uint_t   line_ss = 0;

    line_ee = line_ss + SCREEN_BPL;

    HS08_SK_CLRB
    HS08_LT_CLRB
    HS08_EN_SETB
    for (ii = line_ss; ii < line_ee; ii++) {
        tmp[0] = s_front[0x000 + ii];
        tmp[1] = s_front[0x100 + ii];
        tmp[2] = s_front[0x200 + ii];
        tmp[3] = s_front[0x300 + ii];
        for (jj = 4; jj != 0; jj--) {
            if (tmp[0] & 0x80) HS08_G1A_SETB
            else               HS08_G1A_CLRB
            if (tmp[0] & 0x40) HS08_R1A_SETB
            else               HS08_R1A_CLRB
            if (tmp[1] & 0x80) HS08_G2A_SETB
            else               HS08_G2A_CLRB
            if (tmp[1] & 0x40) HS08_R2A_SETB
            else               HS08_R2A_CLRB
            if (tmp[2] & 0x80) HS08_G1B_SETB
            else               HS08_G1B_CLRB
            if (tmp[2] & 0x40) HS08_R1B_SETB
            else               HS08_R1B_CLRB
            if (tmp[3] & 0x80) HS08_G2B_SETB
            else               HS08_G2B_CLRB
            if (tmp[3] & 0x40) HS08_R2B_SETB
            else               HS08_R2B_CLRB
            HS08_SK_SETB
            tmp[0] <<= 2;
            tmp[1] <<= 2;
            tmp[2] <<= 2;
            tmp[3] <<= 2;
            HS08_SK_CLRB
        }
    }
    HS08_LT_SETB
    jj = line_ss >> 4;
    if (jj & 0x01) HS08_AA_SETB
    else           HS08_AA_CLRB
    if (jj & 0x02) HS08_BB_SETB
    else           HS08_BB_CLRB
    if (jj & 0x04) HS08_CC_SETB
    else           HS08_CC_CLRB
    if (jj & 0x08) HS08_DD_SETB
    else           HS08_DD_CLRB
    HS08_LT_CLRB
    if (jj == 15)
        line_ss = 0;
    else
        line_ss += SCREEN_BPL;
    HS08_EN_CLRB
}

/*
=======================================
    TIMER2 中断处理
=======================================
*/
CR_API void_t
TIM2_IRQHandler (void_t)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update)) {
        TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);
        screen_refresh();
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
