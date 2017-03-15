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
#include "memlib.h"
#include "stm32f10x_conf.h"

/* 音量值 */
uint_t  g_volume = 1;

/* 音频数据环形队列 */
#define AUDIO_BUF_SIZE  2048
static leng_t   s_head, s_tail, s_size;
static byte_t   s_audio[AUDIO_BUF_SIZE + 1];

/*
=======================================
    音频初始化
=======================================
*/
CR_API void_t
audio_init (void_t)
{
    TIM_TimeBaseInitTypeDef sttb;

    /* 初始化全局变量 */
    g_volume = 1;
    s_size = s_head = s_tail = 0;

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
CR_API leng_t
audio_play (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size,
  __CR_IN__ leng_t          total
    )
{
    /* 复制初始数据 */
    if (s_size != 0 ||
        total == 0 || size > total)
        return (0);
    s_size = total;
    if (size > AUDIO_BUF_SIZE)
        size = AUDIO_BUF_SIZE;
    s_head = 0;
    s_tail = AUDIO_BUF_SIZE;
    mem_cpy(s_audio, data, size);

    /* 开始播放 */
    audio_xon();
    TIM_Cmd(TIM4, ENABLE);
#if !defined(YTJ_NEW)
    TIM_Cmd(TIM1, ENABLE);
#endif
    return (size);
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

/*
=======================================
    返回剩余空间
=======================================
*/
CR_API leng_t
audio_space (void_t)
{
    leng_t  count;

    if (s_tail >= s_head)
        count = s_tail - s_head;
    else
        count = sizeof(s_audio) - s_head + s_tail;
    if (count >= AUDIO_BUF_SIZE)
        return (0);
    return (AUDIO_BUF_SIZE - count);
}

/*
=======================================
    填充音频数据
=======================================
*/
CR_API leng_t
audio_append (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    leng_t  count;

    /* 能添加多少是多少 */
    if (size == 0)
        return (0);
    count = audio_space();
    if (count == 0)
        return (0);
    if (size > count)
        size = count;
    for (count = size; count != 0; count--) {
        s_audio[s_tail++] = *(byte_t*)data;
        if (s_tail >= sizeof(s_audio))
            s_tail = 0;
        data = (byte_t*)data + 1;
    }
    return (size);
}

/*
=======================================
    判断是否播放完成
=======================================
*/
CR_API retc_t
audio_is_over (void_t)
{
    return (s_size == 0 ? TRUE : FALSE);
}

/*
=======================================
    TIMER4 中断处理
=======================================
*/
CR_API void_t
TIM4_IRQHandler (void_t)
{
    if (TIM_GetITStatus(TIM4, TIM_IT_Update))
    {
        /* 从环形队列里取一个字节播放 */
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
        if (s_size == 0 || s_head == s_tail)
            return;
        aud0_output(s_audio[s_head++]);
        if (s_head >= sizeof(s_audio))
            s_head = 0;
        if (--s_size == 0)
            audio_stop();
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
