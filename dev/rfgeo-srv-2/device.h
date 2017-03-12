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
