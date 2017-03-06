/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2017-03-04  */
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
/*  >>>>>>>>>>>>>>>>>> YTJ 一体机 LED 屏 AUD0 接口函数库 <<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "board.h"
#include "stm32f10x_conf.h"

/* 音量值 */
extern uint_t   g_volume;

/*
=======================================
    AUD0 初始化
=======================================
*/
CR_API void_t
aud0_init (void_t)
{
    GPIO_InitTypeDef    gpio;

#if defined(YTJ_NEW)
    DAC_InitTypeDef     sdac;

    /* DAC 端口配置 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
                           RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
    gpio.GPIO_Speed = GPIO_Speed_2MHz;
    gpio.GPIO_Pin = GPIO_Pin_4;
    gpio.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &gpio);

    /* DAC 外设配置 */
    sdac.DAC_Trigger = DAC_Trigger_None;
    sdac.DAC_WaveGeneration = DAC_WaveGeneration_None;
    sdac.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bits11_0;
    sdac.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
    DAC_Init(DAC_Channel_1, &sdac);
    DAC_Cmd(DAC_Channel_1, ENABLE);
#else
    TIM_OCInitTypeDef       stoc;
    TIM_TimeBaseInitTypeDef sttb;

    /* PWM 端口配置 */
    TIM_DeInit(TIM1);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB |
                           RCC_APB2Periph_TIM1, ENABLE);
    gpio.GPIO_Speed = GPIO_Speed_2MHz;
    gpio.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
    gpio.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &gpio);

    /* PWM 外设配置 (8位 PCM 编码) */
    sttb.TIM_Period = 0xFF;
    sttb.TIM_Prescaler = 0;
    sttb.TIM_ClockDivision = TIM_CKD_DIV1;
    sttb.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &sttb);
    TIM_ARRPreloadConfig(TIM1, ENABLE);
    stoc.TIM_OCMode = TIM_OCMode_PWM1;
    stoc.TIM_OutputState = TIM_OutputState_Disable;
    stoc.TIM_OutputNState = TIM_OutputNState_Enable;
    stoc.TIM_Pulse = 0x7F;
    stoc.TIM_OCPolarity = TIM_OCPolarity_Low;
    stoc.TIM_OCNPolarity = TIM_OCNPolarity_Low;
    stoc.TIM_OCIdleState = TIM_OCIdleState_Reset;
    stoc.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
    TIM_OC2Init(TIM1, &stoc);
    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_OC3Init(TIM1, &stoc);
    TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_Cmd(TIM1, DISABLE);
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
#endif
    /* 音频使能管脚 */
    gpio.GPIO_Pin = GPIO_Pin_0;
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &gpio);
    GPIO_ResetBits(GPIOB, GPIO_Pin_0);

    /* 输出零值 */
    aud0_zero();
}

/*
=======================================
    AUD0 输出值
=======================================
*/
CR_API void_t
aud0_output (
  __CR_IN__ byte_t  value
    )
{
#if defined(YTJ_NEW)
    int16u  temp = (int16u)value;

    if (value > 0x7F) {
        temp = (int16u)((temp - 0x80) * g_volume);
        temp = 0x800 + temp;
    }
    else {
        temp = (int16u)((0x80 - temp) * g_volume);
        temp = 0x800 - temp;
    }
    DAC_SetChannel1Data(DAC_Align_12b_R, temp);
    DAC_SoftwareTriggerCmd(DAC_Channel_1, ENABLE);
#else
    TIM_SetCompare2(TIM1, value);
    TIM_SetCompare3(TIM1, value);
#endif
}

/*
=======================================
    AUD0 输出零
=======================================
*/
CR_API void_t
aud0_zero (void_t)
{
#if defined(YTJ_NEW)
    DAC_SetChannel1Data(DAC_Align_12b_R, 0x800);
    DAC_SoftwareTriggerCmd(DAC_Channel_1, ENABLE);
#else
    TIM_SetCompare2(TIM1, 0x80);
    TIM_SetCompare3(TIM1, 0x80);
#endif
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
