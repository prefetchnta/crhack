/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2011-10-27  */
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
/*  >>>>>>>>>>>>>>>>>>>> CrHack M25PEXX 驱动函数库模板 <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "spi_mm.h"
#include "chip/m25pexx.h"

/* 器件的命令字 */
#define M25PEXX_WRLR    0xE5    /* 写锁定寄存器 */
#define M25PEXX_RDLR    0xE8    /* 读锁定寄存器 */
#define M25PEXX_PAGEW   0x0A    /* 数据页的修改 */
#define M25PEXX_PAGEE   0xDB    /* 数据页的擦除 */

#if !defined(__no_m25pexx_unlock)
    #define e25cxx_unlock       m25pexx_unlock
#endif
#if !defined(__no_m25pexx_lock)
    #define e25cxx_lock         m25pexx_lock
#endif
#if !defined(__no_m25pexx_read_id3)
    #define sE25CID3            sM25PEXX3
    #define e25cxx_read_id3     m25pexx_read_id
#endif
#if !defined(__no_m25pexx_get_status)
    #define e25cxx_get_status   m25pexx_get_status
#endif
#if !defined(__no_m25pexx_wait_idle)
    #define E25CXX_WIP          M25PEXX_WIP
    #define e25cxx_wait_idle    m25pexx_wait_idle
#endif
#if !defined(__no_m25pexx_set_status)
    #define e25cxx_set_status   m25pexx_set_status
#endif
#if !defined(__no_m25pexx_read)
    #define e25cxx_read         m25pexx_read
#endif
#if !defined(__no_m25pexx_fread)
    #define e25cxx_fread        m25pexx_fread
#endif
#if !defined(__no_m25pexx_ssec_erase)
    #define e25cxx_ssec_erase   m25pexx_ssec_erase
#endif
#if !defined(__no_m25pexx_bsec_erase)
    #define e25cxx_bsec_erase   m25pexx_bsec_erase
#endif
#if !defined(__no_m25pexx_bulk_erase)
    #define e25cxx_bulk_erase   m25pexx_bulk_erase
#endif
#if !defined(__no_m25pexx_program)
    #define e25cxx_program      m25pexx_program
    #define e25cxx_page_prog    m25pexx_page_prog
#endif
#if !defined(__no_m25pexx_pwr_off)
    #define e25cxx_pwr_off      m25pexx_pwr_off
#endif
#if !defined(__no_m25pexx_pwr_xon)
    #define e25cxx_pwr_xon      m25pexx_pwr_xon
#endif

#include "25cxx.inl"

#if !defined(__no_m25pexx_get_rlock)
/*
=======================================
    M25PEXX 读锁定寄存器
=======================================
*/
CR_API byte_t
m25pexx_get_rlock (
  __CR_IN__ spi_addr_t  addr
    )
{
    byte_t  rlock;
    byte_t  cmd[4];

    if (addr >= _SPI_SIZE_)
        addr = _SPI_SIZE_ - 1;
    cmd[0] = M25PEXX_RDLR;
    cmd[1] = (byte_t)(addr >> 16);
    cmd[2] = (byte_t)(addr >>  8);
    cmd[3] = (byte_t)(addr >>  0);
    spi_send_recv(&rlock, 1, cmd, 4);
    return (rlock);
}

#endif  /* !__no_m25pexx_get_rlock */

#if !defined(__no_m25pexx_set_rlock)
/*
=======================================
    M25PEXX 写锁定寄存器
=======================================
*/
CR_API retc_t
m25pexx_set_rlock (
  __CR_IN__ spi_addr_t  addr,
  __CR_IN__ byte_t      rlock,
  __CR_IN__ uint_t      tout
    )
{
    byte_t  cmd[5];

    if (addr >= _SPI_SIZE_)
        addr = _SPI_SIZE_ - 1;
    cmd[0] = M25PEXX_WRLR;
    cmd[1] = (byte_t)(addr >> 16);
    cmd[2] = (byte_t)(addr >>  8);
    cmd[3] = (byte_t)(addr >>  0);
    cmd[4] = rlock;
    m25pexx_unlock();
    spi_send_data(cmd, 5);
    return (m25pexx_wait_idle(tout));
}

#endif  /* !__no_m25pexx_set_rlock */

#if !defined(__no_m25pexx_page_erase)
/*
=======================================
    M25PEXX 数据页擦除
=======================================
*/
CR_API retc_t
m25pexx_page_erase (
  __CR_IN__ spi_addr_t  addr,
  __CR_IN__ uint_t      tout
    )
{
    byte_t  cmd[4];

    if (addr >= _SPI_SIZE_)
        return (FALSE);
    cmd[0] = M25PEXX_PAGEE;
    cmd[1] = (byte_t)(addr >> 16);
    cmd[2] = (byte_t)(addr >>  8);
    cmd[3] = (byte_t)(addr >>  0);
    m25pexx_unlock();
    spi_send_data(cmd, 4);
    return (m25pexx_wait_idle(tout));
}

#endif  /* !__no_m25pexx_page_erase */

#if !defined(__no_m25pexx_write)
/*
---------------------------------------
    M25PEXX 内部页写
---------------------------------------
*/
static retc_t
m25pexx_page_write (
  __CR_IN__ spi_addr_t      addr,
  __CR_IN__ const void_t*   data,
  __CR_IN__ spi_leng_t      size,
  __CR_IN__ uint_t          tout
    )
{
    byte_t  cmd[4];

    cmd[0] = M25PEXX_PAGEW;
    cmd[1] = (byte_t)(addr >> 16);
    cmd[2] = (byte_t)(addr >>  8);
    cmd[3] = (byte_t)(addr >>  0);
    m25pexx_unlock();
    spi_send_send(cmd, 4, data, size);
    return (m25pexx_wait_idle(tout));
}

/*
=======================================
    M25PEXX 写入数据
=======================================
*/
CR_API spi_leng_t
m25pexx_write (
  __CR_IN__ spi_addr_t      addr,
  __CR_IN__ const void_t*   data,
  __CR_IN__ spi_leng_t      size,
  __CR_IN__ uint_t          tout
    )
{
    byte_t*     ptr;
    spi_blks_t  blk;
    spi_page_t  rst;
    spi_leng_t  total;

    /* 参数过滤 */
    if (size == 0 || addr >= _SPI_SIZE_ ||
        size > _SPI_SIZE_ - addr)
        return (0);

    /* 前地址对齐 */
    ptr = (byte_t*)data;
    rst = (spi_page_t)(_SPI_PAGE_ - addr % _SPI_PAGE_);
    if (rst >= size) {
        if (!m25pexx_page_write(addr, ptr, size, tout))
            return (0);
        return (size);
    }
    if (!m25pexx_page_write(addr, ptr, rst, tout))
        return (0);
    ptr  += rst;
    addr += rst;
    size -= rst;
    total = rst;

    /* 分块和尾部 */
    blk = (spi_blks_t)(size / _SPI_PAGE_);
    for (; blk != 0; blk--) {
        if (!m25pexx_page_write(addr, ptr, _SPI_PAGE_, tout))
            return (total);
        ptr   += _SPI_PAGE_;
        addr  += _SPI_PAGE_;
        total += _SPI_PAGE_;
    }
    rst = (spi_page_t)(size % _SPI_PAGE_);
    if (rst != 0) {
        if (!m25pexx_page_write(addr, ptr, rst, tout))
            return (total);
        total += rst;
    }
    return (total);
}

#endif  /* !__no_m25pexx_write */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
