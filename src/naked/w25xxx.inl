/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2017-02-26  */
/*     #######          ###    ###      [KRNL]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>>>>>> CrHack W25XXX 驱动函数库模板 <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "spi_mm.h"
#include "chip/w25xxx.h"

#if !defined(__no_w25xxx_unlock)
    #define e25cxx_unlock       w25xxx_unlock
#endif
#if !defined(__no_w25xxx_lock)
    #define e25cxx_lock         w25xxx_lock
#endif
#if !defined(__no_w25xxx_read_id3)
    #define sE25CID3            sW25XXX3
    #define e25cxx_read_id3     w25xxx_read_id3
#endif
#if !defined(__no_w25xxx_read_id2)
    #define sE25CID2            sW25XXX2
    #define e25cxx_read_id2     w25xxx_read_id2
#endif
#if !defined(__no_w25xxx_get_status)
    #define e25cxx_get_status   w25xxx_get_status
#endif
#if !defined(__no_w25xxx_wait_idle)
    #define E25CXX_WIP          W25XXX_WIP
    #define e25cxx_wait_idle    w25xxx_wait_idle
#endif
#if !defined(__no_w25xxx_set_status)
    #define e25cxx_set_status   w25xxx_set_status
#endif
#if !defined(__no_w25xxx_read)
    #define e25cxx_read         e25cxx_read
#endif
#if !defined(__no_w25xxx_fread)
    #define e25cxx_fread        w25xxx_fread
#endif
#if !defined(__no_w25xxx_ssec_erase)
    #define e25cxx_ssec_erase   w25xxx_ssec_erase
#endif
#if !defined(__no_w25xxx_bsec_erase)
    #define e25cxx_bsec_erase   w25xxx_bsec_erase
#endif
#if !defined(__no_w25xxx_bulk_erase)
    #define e25cxx_bulk_erase   w25xxx_bulk_erase
#endif
#if !defined(__no_w25xxx_program)
    #define e25cxx_program      w25xxx_program
    #define e25cxx_page_prog    w25xxx_page_prog
#endif
#if !defined(__no_w25xxx_pwr_off)
    #define e25cxx_pwr_off      w25xxx_pwr_off
#endif
#if !defined(__no_w25xxx_pwr_xon)
    #define e25cxx_pwr_xon      w25xxx_pwr_xon
#endif

#include "25cxx.inl"

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
