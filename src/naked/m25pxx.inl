/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2012-03-03  */
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
/*  >>>>>>>>>>>>>>>>>>>>> CrHack M25PXX 驱动函数库模板 <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "spi_mm.h"
#include "chip/m25pxx.h"

#if !defined(__no_m25pxx_unlock)
    #define e25cxx_unlock       m25pxx_unlock
#endif
#if !defined(__no_m25pxx_lock)
    #define e25cxx_lock         m25pxx_lock
#endif
#if !defined(__no_m25pxx_read_id)
    #define sE25CID3            sM25PXX3
    #define e25cxx_read_id3     m25pxx_read_id
#endif
#if !defined(__no_m25pxx_get_status)
    #define e25cxx_get_status   m25pxx_get_status
#endif
#if !defined(__no_m25pxx_wait_idle)
    #define E25CXX_WIP          M25PXX_WIP
    #define e25cxx_wait_idle    m25pxx_wait_idle
#endif
#if !defined(__no_m25pxx_set_status)
    #define e25cxx_set_status   m25pxx_set_status
#endif
#if !defined(__no_m25pxx_read)
    #define e25cxx_read         m25pxx_read
#endif
#if !defined(__no_m25pxx_fread)
    #define e25cxx_fread        m25pxx_fread
#endif
#if !defined(__no_m25pxx_bsec_erase)
    #define e25cxx_bsec_erase   m25pxx_bsec_erase
#endif
#if !defined(__no_m25pxx_bulk_erase)
    #define e25cxx_bulk_erase   m25pxx_bulk_erase
#endif
#if !defined(__no_m25pxx_program)
    #define e25cxx_program      m25pxx_program
    #define e25cxx_page_prog    m25pxx_page_prog
#endif
#if !defined(__no_m25pxx_pwr_off)
    #define e25cxx_pwr_off  m25pxx_pwr_off
#endif
#if !defined(__no_m25pxx_pwr_xon)
    #define e25cxx_pwr_xon  m25pxx_pwr_xon
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
