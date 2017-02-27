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
/*  >>>>>>>>>>>>>>>>>>>> CrHack MX25LXX 驱动函数库模板 <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "spi_mm.h"
#include "chip/mx25lxx.h"

/* 器件的命令字 */
#define MX25LXX_ENSO    0xB1    /* 进入安全模式 */
#define MX25LXX_EXSO    0xC1    /* 退出安全模式 */
#define MX25LXX_RDSCUR  0x2B    /* 读安全寄存器 */
#define MX25LXX_WRSCUR  0x2F    /* 写安全寄存器 */
#define MX25LXX_RDSFDP  0x5A    /* 读取闪存信息 */

#if !defined(__no_mx25lxx_unlock)
    #define e25cxx_unlock       mx25lxx_unlock
#endif
#if !defined(__no_mx25lxx_lock)
    #define e25cxx_lock         mx25lxx_lock
#endif
#if !defined(__no_mx25lxx_read_id3)
    #define sE25CID3            sMX25LXX3
    #define e25cxx_read_id3     mx25lxx_read_id3
#endif
#if !defined(__no_mx25lxx_read_id2)
    #define sE25CID2            sMX25LXX2
    #define e25cxx_read_id2     mx25lxx_read_id2
#endif
#if !defined(__no_mx25lxx_get_status)
    #define e25cxx_get_status   mx25lxx_get_status
#endif
#if !defined(__no_mx25lxx_wait_idle)
    #define E25CXX_WIP          MX25LXX_WIP
    #define e25cxx_wait_idle    mx25lxx_wait_idle
#endif
#if !defined(__no_mx25lxx_set_status)
    #define e25cxx_set_status   mx25lxx_set_status
#endif
#if !defined(__no_mx25lxx_read)
    #define e25cxx_read         mx25lxx_read
#endif
#if !defined(__no_mx25lxx_fread)
    #define e25cxx_fread        mx25lxx_fread
#endif
#if !defined(__no_mx25lxx_ssec_erase)
    #define e25cxx_ssec_erase   mx25lxx_ssec_erase
#endif
#if !defined(__no_mx25lxx_bsec_erase)
    #define e25cxx_bsec_erase   mx25lxx_bsec_erase
#endif
#if !defined(__no_mx25lxx_bulk_erase)
    #define e25cxx_bulk_erase   mx25lxx_bulk_erase
#endif
#if !defined(__no_mx25lxx_program)
    #define e25cxx_program      mx25lxx_program
    #define e25cxx_page_prog    mx25lxx_page_prog
#endif
#if !defined(__no_mx25lxx_pwr_off)
    #define e25cxx_pwr_off      mx25lxx_pwr_off
#endif
#if !defined(__no_mx25lxx_pwr_xon)
    #define e25cxx_pwr_xon      mx25lxx_pwr_xon
#endif

#include "25cxx.inl"

#if !defined(__no_mx25lxx_otp_enter)
/*
=======================================
    MX25LXX 进入安全 OTP 模式
=======================================
*/
CR_API void_t
mx25lxx_otp_enter (void_t)
{
    byte_t  cmd;

    cmd = MX25LXX_ENSO;
    spi_send_data(&cmd, 1);
}

#endif  /* !__no_mx25lxx_otp_enter */

#if !defined(__no_mx25lxx_otp_leave)
/*
=======================================
    MX25LXX 离开安全 OTP 模式
=======================================
*/
CR_API void_t
mx25lxx_otp_leave (void_t)
{
    byte_t  cmd;

    cmd = MX25LXX_EXSO;
    spi_send_data(&cmd, 1);
}

#endif  /* !__no_mx25lxx_otp_leave */

#if !defined(__no_mx25lxx_get_security)
/*
=======================================
    MX25LXX 读状态寄存器
=======================================
*/
CR_API byte_t
mx25lxx_get_security (void_t)
{
    byte_t  status;
    byte_t  cmd = MX25LXX_RDSCUR;

    spi_send_recv(&status, 1, &cmd, 1);
    return (status);
}

#endif  /* !__no_mx25lxx_get_security */

#if !defined(__no_mx25lxx_set_security)
/*
=======================================
    MX25LXX 写安全寄存器
=======================================
*/
CR_API void_t
mx25lxx_set_security (void_t)
{
    byte_t  cmd;

    cmd = MX25LXX_WRSCUR;
    spi_send_data(&cmd, 1);
}

#endif  /* !__no_mx25lxx_set_security */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
