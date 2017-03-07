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

/* LED 灯控制 */
#define led_xon()   GPIOC->BRR = GPIO_Pin_13
#define led_off()   GPIOC->BSRR = GPIO_Pin_13
CR_API void_t   screen_init (void_t);
CR_API byte_t*  screen_flip (void_t);
CR_API byte_t*  screen_main (void_t);

/* 屏幕缓冲区的参数 */
#define SCREEN_BPL      (16)
#define SCREEN_WIDTH    (64)
#define SCREEN_HEIGHT   (64)
#define SCREEN_SIZE     (SCREEN_BPL * SCREEN_HEIGHT)

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
