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
    显示后台缓冲
=======================================
*/
CR_API void_t
screen_copy (void_t)
{
    mem_cpy(s_front, s_backs, SCREEN_SIZE);
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
        return (s_screen1);
    }
    s_front = s_screen1;
    s_backs = s_screen2;
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
=======================================
    像素填充
=======================================
*/
CR_API void_t
line_fill (
  __CR_IN__ uint_t  line,
  __CR_IN__ byte_t  c
    )
{
    /* 颜色扩展到1个字节 */
    c &= 3;
    c |= (c << 2);
    c |= (c << 4);

    /* 填充整个屏幕 */
    if (line >= 4) {
        mem_set(s_backs, c, SCREEN_SIZE);
        return;
    }

    /* 只填充一行 */
    line *= (SCREEN_BPL * 16);
    mem_set(&s_backs[line], c, SCREEN_BPL * 16);
}

/*
=======================================
    像素滚动
=======================================
*/
CR_API void_t
line_scroll (
  __CR_IN__ uint_t  line,
  __CR_IN__ uint_t  type
    )
{
    sint_t  xx, yy;
    sint_t  height;

    /* 决定滚动的范围 */
    if (line >= 4) {
        line = 0;
        height = SCREEN_HEIGHT;
    }
    else {
        line *= (SCREEN_BPL * 16);
        height = 16;
    }

    /* 根据滚动类型滚动 */
    switch (type)
    {
        default:
            break;

        case SCRLL_LEFT:    /* 左滚 */
            for (yy = 0; yy < height; yy++) {
                s_backs[line] <<= 2;
                for (xx = 1; xx < SCREEN_BPL; xx++) {
                    s_backs[line + xx - 1] |= (s_backs[line + xx] >> 6);
                    s_backs[line + xx] <<= 2;
                }
                line += SCREEN_BPL;
            }
            break;

        case SCRLL_RIGHT:   /* 右滚 */
            for (yy = 0; yy < height; yy++) {
                s_backs[line + SCREEN_BPL - 1] >>= 2;
                for (xx = SCREEN_BPL - 2; xx >= 0; xx--) {
                    s_backs[line + xx + 1] |= (s_backs[line + xx] << 6);
                    s_backs[line + xx] >>= 2;
                }
                line += SCREEN_BPL;
            }
            break;

        case SCRLL_UP:      /* 上滚 */
            for (yy = 0; yy < height - 1; yy++) {
                mem_cpy(&s_backs[line],
                        &s_backs[line + SCREEN_BPL],
                        SCREEN_BPL);
                line += SCREEN_BPL;
            }
            mem_zero(&s_backs[line], SCREEN_BPL);
            break;

        case SCRLL_DOWN:    /* 下滚 */
            line += (height - 1) * SCREEN_BPL;
            for (yy = 0; yy < height - 1; yy++) {
                mem_cpy(&s_backs[line],
                        &s_backs[line - SCREEN_BPL],
                        SCREEN_BPL);
                line -= SCREEN_BPL;
            }
            mem_zero(&s_backs[line], SCREEN_BPL);
            break;
    }
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
