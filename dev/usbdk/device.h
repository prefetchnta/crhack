/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2012-02-02  */
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
/*  >>>>>>>>>>>>>>>>>>>>>>> USBDK U盘芯片驱动头文件 <<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#ifndef __CR_DEVICE_H__
#define __CR_DEVICE_H__

#include "board.h"
#include "chip/k9f1208c.h"

/*****************************************************************************/
/*                                K9F1208X0C                                 */
/*****************************************************************************/

CR_API void_t   k9f1208c_get_devid (sK9F1208C *devid);
CR_API retc_t   k9f1208c_page_read (int32u apage, void_t *buffer);
CR_API retc_t   k9f1208c_page_write (int32u apage, const void_t *buffer,
                                     uint_t timeout);
CR_API retc_t   k9f1208c_csec_read (int32u apage, void_t *buffer);
CR_API retc_t   k9f1208c_csec_write (int32u apage, const void_t *buffer,
                                     uint_t timeout);
CR_API retc_t   k9f1208c_block_check (uint_t ablock, int32u *page_addr);
CR_API retc_t   k9f1208c_block_erase (uint_t ablock, uint_t timeout);

/*****************************************************************************/
/*                               储存器1读写                                 */
/*****************************************************************************/

CR_API retc_t   nand_init (void_t);
CR_API uint_t   nand_format (void_t);
CR_API retc_t   nand_state (int32u *size);
CR_API retc_t   nand_read (int32u addr, void_t *data, int16u size);
CR_API retc_t   nand_write (int32u addr, const void_t *data, int16u size);

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
