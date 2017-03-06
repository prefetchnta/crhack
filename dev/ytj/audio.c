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
/*  >>>>>>>>>>>>>>>>>>> YTJ 一体机 LED 屏音频驱动函数库 <<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "device.h"
#include "stm32f10x_conf.h"

/* 音量值 */
uint_t  g_volume = 1;

/*
=======================================
    音频初始化
=======================================
*/
CR_API void_t
audio_init (void_t)
{
    TIM_TimeBaseInitTypeDef sttb;

    /* 使用 TIM4 作为音频中断 (16KHz 采样) */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    TIM_DeInit(TIM4);
    sttb.TIM_Period = 8;   /* 17 = 8KHz, 8 = 16KHz */
    sttb.TIM_Prescaler = 499;
    sttb.TIM_ClockDivision = TIM_CKD_DIV1;
    sttb.TIM_CounterMode = TIM_CounterMode_Down;
    TIM_TimeBaseInit(TIM4, &sttb);
    TIM_ARRPreloadConfig(TIM4, ENABLE);
    TIM_Cmd(TIM4, DISABLE);
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
}

/*
=======================================
    设置音频音量
=======================================
*/
CR_API void_t
audio_volume (
  __CR_IN__ uint_t  value
    )
{
    if (value > 16)
        g_volume = 16;
    else
        g_volume = value;
}

/*
=======================================
    开始播放音频
=======================================
*/
CR_API void_t
audio_play (void_t)
{
    audio_xon();
    TIM_Cmd(TIM4, ENABLE);
#if !defined(YTJ_NEW)
    TIM_Cmd(TIM1, ENABLE);
#endif
}

/*
=======================================
    停止播放音频
=======================================
*/
CR_API void_t
audio_stop (void_t)
{
    audio_off();
    TIM_Cmd(TIM4, DISABLE);
    aud0_zero();
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
