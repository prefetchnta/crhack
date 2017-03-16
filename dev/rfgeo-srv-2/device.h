/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2017-03-09  */
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
/*  >>>>>>>>>>>>>>>>>> RFGEO-SRV-2 采集器芯片驱动头文件 <<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_DEVICE_H__
#define __CR_DEVICE_H__

#include "board.h"
#include "rtclib.h"
#include "chip/mx25lxx.h"

/*****************************************************************************/
/*                                MX25L1606E                                 */
/*****************************************************************************/

/* 写保护管脚 */
CR_API void_t   mx25l16_unlock (void_t);
CR_API byte_t   mx25l16_get_status (void_t);
CR_API retc_t   mx25l16_wait_idle (uint_t times);
CR_API void_t   mx25l16_read_id3 (sMX25LXX3 *id);
CR_API leng_t   mx25l16_fread (int32u addr, void_t *data, leng_t size);
CR_API retc_t   mx25l16_ssec_erase (int32u addr, uint_t time, uint_t tout);
CR_API leng_t   mx25l16_program (int32u addr, const void_t *data,
                                 leng_t size, uint_t tout);

/*****************************************************************************/
/*                                 RX-8025                                   */
/*****************************************************************************/

CR_API retc_t   rx8025_init_time (void_t);
CR_API retc_t   rx8025_get_time (sDATETIME *dttm);
CR_API retc_t   rx8025_set_time (const sDATETIME *dttm);

/*****************************************************************************/
/*                                  RS-485                                   */
/*****************************************************************************/

CR_API bool_t   rs485_baud (int32u baud);
CR_API void_t   rs485_write (const void_t *data, leng_t size);
#define rs485_send_str(str) rs485_write(str, str_lenA(str))
CR_API void_t   rs485_zero (void_t);
CR_API uint_t   rs485_rx_size (void_t);
CR_API void_t   rs485_rx_flush (void_t);
CR_API void_t   rs485_throw (uint_t size);
CR_API uint_t   rs485_peek (void_t *data, uint_t size);
CR_API uint_t   rs485_read (void_t *data, uint_t size);
CR_API uint_t   rs485_wait (void_t *data, uint_t step, uint_t tout);

/*****************************************************************************/
/*                                  RS-232                                   */
/*****************************************************************************/

CR_API bool_t   rs232_baud (int32u baud);
CR_API void_t   rs232_write (const void_t *data, leng_t size);
#define rs232_send_str(str) rs232_write(str, str_lenA(str))
CR_API void_t   rs232_zero (void_t);
CR_API uint_t   rs232_rx_size (void_t);
CR_API void_t   rs232_rx_flush (void_t);
CR_API void_t   rs232_throw (uint_t size);
CR_API uint_t   rs232_peek (void_t *data, uint_t size);
CR_API uint_t   rs232_read (void_t *data, uint_t size);
CR_API uint_t   rs232_wait (void_t *data, uint_t step, uint_t tout);

/*****************************************************************************/
/*                                  桥接板                                   */
/*****************************************************************************/

/* 通讯命令字 */
#define BRIDGE_SEND     0x00    /* 数据的转发：端口(1B) + 数据(NB)
                                                0x00 - CAN
                                                0x01 - 485
                                                0x02 - 桥接
                                                0x03 - 主板
                                */
#define BRIDGE_BAUD     0x01    /* 设置波特率：端口(1B) + 波特率(4B) */
#define BRIDGE_RESET    0x02    /* 复位 */
#define BRIDGE_GPIO     0x03    /* CTRL 管脚控制：电平(1B) */

/* 端口的类型 */
#define PORT_TYPE_CAN   0x00
#define PORT_TYPE_485   0x01
#define PORT_TYPE_PSS   0x02
#define PORT_TYPE_BRD   0x03

/* 命令返回值 */
#define RETURN_OKAY     0x00

CR_API void_t   bridge_init (void_t);
CR_API bool_t   bridge_baud (byte_t port, int32u baud);
CR_API bool_t   bridge_reset (void_t);
CR_API bool_t   bridge_gpio (byte_t level);
CR_API void_t   bridge_commit (byte_t port, const void_t *data, uint_t size);

/*****************************************************************************/
/*                                 通讯模块                                  */
/*****************************************************************************/

/* 模块类型 */
#define SRV2NET_NONE        0
#define SRV2NET_SIM7100     1
#define SRV2NET_SIM6320     2
#define SRV2NET_SIM5360     3

/* SIMCOM */

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
