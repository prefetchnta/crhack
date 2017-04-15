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
/*  >>>>>>>>>>>>>>>>>>> YTJ 一体机 LED 屏芯片驱动头文件 <<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_DEVICE_H__
#define __CR_DEVICE_H__

#include "board.h"
#include "chip/mx25lxx.h"

/*****************************************************************************/
/*                                MX25L1606E                                 */
/*****************************************************************************/

/* 写保护管脚 */
#define mx25l16_wrdi()  GPIOC->BRR = GPIO_Pin_4
#define mx25l16_wren()  GPIOC->BSRR = GPIO_Pin_4
CR_API void_t   mx25l16_unlock (void_t);
CR_API byte_t   mx25l16_get_status (void_t);
CR_API retc_t   mx25l16_wait_idle (uint_t times);
CR_API void_t   mx25l16_read_id3 (sMX25LXX3 *id);
CR_API leng_t   mx25l16_fread (int32u addr, void_t *data, leng_t size);
CR_API retc_t   mx25l16_ssec_erase (int32u addr, uint_t time, uint_t tout);
CR_API leng_t   mx25l16_program (int32u addr, const void_t *data,
                                 leng_t size, uint_t tout);

/*****************************************************************************/
/*                                   AUDIO                                   */
/*****************************************************************************/

/* 音频总开关 */
#define audio_xon()     GPIOB->BSRR = GPIO_Pin_0
#define audio_off()     GPIOB->BRR = GPIO_Pin_0
CR_API void_t   audio_init (void_t);
CR_API void_t   audio_volume (uint_t value);
CR_API leng_t   audio_play (const void_t *data, leng_t size, leng_t total);
CR_API void_t   audio_stop (void_t);
CR_API leng_t   audio_space (void_t);
CR_API leng_t   audio_append (const void_t *data, leng_t size);
CR_API retc_t   audio_is_over (void_t);

/*****************************************************************************/
/*                                  SCREEN                                   */
/*****************************************************************************/

/* 屏幕缓冲区的参数 */
#define SCREEN_BPL      (16)
#define SCREEN_WIDTH    (64)
#define SCREEN_HEIGHT   (64)
#define SCREEN_SIZE     (SCREEN_BPL * SCREEN_HEIGHT)

CR_API void_t   screen_init (void_t);
CR_API void_t   screen_copy (void_t);
CR_API byte_t*  screen_flip (void_t);
CR_API byte_t*  screen_main (void_t);
CR_API byte_t*  screen_back (void_t);

/* 像素颜色 */
#define DOT_BL  0x00    /* 黑色 */
#define DOT_RR  0x01    /* 红色 */
#define DOT_GG  0x02    /* 绿色 */
#define DOT_YY  0x03    /* 黄色 */

CR_API byte_t   pixel_get02z (sint_t x, sint_t y);
CR_API void_t   pixel_set02z (sint_t x, sint_t y, byte_t c);

/* 滚屏类型 */
#define LINE_FULL   0xFF    /* 全屏 */
#define SCRLL_LEFT  0x00    /* 左滚 */
#define SCRLL_RIGHT 0x01    /* 右滚 */
#define SCRLL_UP    0x02    /* 上滚 */
#define SCRLL_DOWN  0x03    /* 下滚 */

CR_API void_t   line_fill (uint_t line, byte_t c);
CR_API void_t   line_scroll (uint_t line, uint_t type);

/*****************************************************************************/
/*                                 NETWORK                                   */
/*****************************************************************************/

/* DHCP 状态 */
#define DHCP_STT_NONE       0   /* 状态无变 */
#define DHCP_STT_RUNNING    1   /* DHCP 中 */
#define DHCP_STT_ALLDONE    2   /* DHCP 完成 */
#define DHCP_STT_NSTATIC    3   /* 静态地址 */

/* IP(4B) + MASK(4B) + GATEWAY(4B) */
CR_API void_t   netwrk_init (const void_t *cfg);
CR_API void_t   netwrk_func (void_t);
CR_API bool_t   netwrk_online (byte_t *ip, uint_t *type);

/*****************************************************************************/
/*                                储存器读写                                 */
/*****************************************************************************/

CR_API retc_t   store_init (void_t);
CR_API retc_t   store_check (void_t);
CR_API retc_t   store_read (int32u addr, void_t *data, leng_t size);
CR_API retc_t   store_write (int32u addr, const void_t *data, leng_t size);

#endif  /* !__CR_DEVICE_H__ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
