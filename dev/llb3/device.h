/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2011-09-20  */
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
/*  >>>>>>>>>>>>>>>>>> LLB3 MLR 流量结算仪芯片驱动头文件 <<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_DEVICE_H__
#define __CR_DEVICE_H__

#include "board.h"
#include "chip/x5xxx.h"
#include "chip/ks0108.h"
#include "chip/ds18b20.h"

/*****************************************************************************/
/*                                  X5045                                    */
/*****************************************************************************/

/* 写保护管脚 */
#define x5045_wrdi()    b_P1_4 = 0
#define x5045_wren()    b_P1_4 = 1
CR_API void_t   x5045_unlock (void_t);
CR_API void_t   x5045_clear_wdt (void_t);
CR_API byte_t   x5045_get_status (void_t);
CR_API retc_t   x5045_wait_idle (uint_t times);
CR_API leng_t   x5045_read (int16u addr, void_t *data, leng_t size);
CR_API leng_t   x5045_write (int16u addr, const void_t *data,
                             leng_t size, uint_t tout);

/*****************************************************************************/
/*                                 TLC5615                                   */
/*****************************************************************************/

CR_API void_t   tlc5615_dacvt (int16u val);

/*****************************************************************************/
/*                                 DS18B20                                   */
/*****************************************************************************/

/* 温度转换状态 */
#define ds18b20_okay()  b_P1_1
CR_API retc_t   ds18b20_do_cmd (byte_t cmd);
CR_API retc_t   ds18b20_ram_read (void_t *data, ufast_t size);

/*****************************************************************************/
/*                                 ICL7135                                   */
/*****************************************************************************/

/* 最大的通道数 */
#define ICL7135_MAX   3
#define ADC_VAL_MAX 20000
CR_API void_t   icl7135_start (void_t);
CR_API void_t   icl7135_refresh (void_t);
CR_API int16u   icl7135_get (byte_t chn);

/*****************************************************************************/
/*                               KS0108 (左)                                 */
/*****************************************************************************/

CR_API retc_t   ks0108l_lcd_switch (bool_t open, uint_t times);
CR_API retc_t   ks0108l_set_start (byte_t line, uint_t times);
CR_API retc_t   ks0108l_set_xy (byte_t dx, byte_t dy, uint_t times);
CR_API retc_t   ks0108l_write_dot (byte_t dot8, uint_t times);
CR_API retc_t   ks0108l_write_line (const byte_t *line, ufast_t size,
                                    uint_t times);
CR_API retc_t   ks0108l_pixel_set (byte_t x, byte_t y, byte_t color,
                                   uint_t times);

/*****************************************************************************/
/*                               KS0108 (右)                                 */
/*****************************************************************************/

CR_API retc_t   ks0108r_lcd_switch (bool_t open, uint_t times);
CR_API retc_t   ks0108r_set_start (byte_t line, uint_t times);
CR_API retc_t   ks0108r_set_xy (byte_t dx, byte_t dy, uint_t times);
CR_API retc_t   ks0108r_write_dot (byte_t dot8, uint_t times);
CR_API retc_t   ks0108r_write_line (const byte_t *line, ufast_t size,
                                    uint_t times);
CR_API retc_t   ks0108r_pixel_set (byte_t x, byte_t y, byte_t color,
                                   uint_t times);

/*****************************************************************************/
/*                                 KEYBOARD                                  */
/*****************************************************************************/

/* 键盘键值 */
#define KEY_ENTER   0x04    /* S1 */
#define KEY_LEAVE   0x20    /* S2 */
#define KEY_DOWN    0x02    /* S3 */
#define KEY_UP      0x10    /* S4 */
#define KEY_LEFT    0x01    /* S5 */
#define KEY_RIGHT   0x08    /* S6 */
CR_API void_t   key_init (void_t);
CR_API byte_t   key_get_up (uint_t times);
CR_API byte_t   key_get_down (uint_t times);

/*****************************************************************************/
/*                                 LED 显示                                  */
/*****************************************************************************/

CR_API void_t   led_dpy_scan (void_t);
CR_API void_t   led_dpy_init (uint_t tck);
CR_API void_t   led_refresh_0 (const ansi_t *str);
CR_API void_t   led_refresh_1 (const ansi_t *str);
CR_API void_t   led_refresh_2 (const ansi_t *str);

/*****************************************************************************/
/*                                  ALARM                                    */
/*****************************************************************************/

#define alarm_xon()     b_P1_0 = 0
#define alarm_off()     b_P1_0 = 1

/*****************************************************************************/
/*                                SN75LBC184                                 */
/*****************************************************************************/

#define rs485_recv()    b_P1_7 = 0
#define rs485_send()    b_P1_7 = 1

/*****************************************************************************/
/*                                储存器读写                                 */
/*****************************************************************************/

CR_API retc_t   store_read  (int16u addr, void_t *data, leng_t size);
CR_API retc_t   store_write (int16u addr, void_t *data, leng_t size);

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
