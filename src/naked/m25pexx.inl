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
#define M25PEXX_WREN    0x06    /*  关闭写保护  */
#define M25PEXX_WRDI    0x04    /*  打开写保护  */
#define M25PEXX_RDID    0x9F    /*  读器件标志  */
#define M25PEXX_RDSR    0x05    /* 读状态寄存器 */
#define M25PEXX_WRLR    0xE5    /* 写锁定寄存器 */
#define M25PEXX_WRSR    0x01    /* 写状态寄存器 */
#define M25PEXX_RDLR    0xE8    /* 读锁定寄存器 */
#define M25PEXX_READ    0x03    /* 读取内存内容 */
#define M25PEXX_FREAD   0x0B    /* 快速读取内容 */
#define M25PEXX_PAGEW   0x0A    /* 数据页的修改 */
#define M25PEXX_PAGEP   0x02    /* 写入内存内容 */
#define M25PEXX_PAGEE   0xDB    /* 数据页的擦除 */
#define M25PEXX_SSECE   0x20    /* 小扇区的擦除 */
#define M25PEXX_BSECE   0xD8    /* 大区块的擦除 */
#define M25PEXX_BULKE   0xC7    /* 全芯片的擦除 */
#define M25PEXX_DDOWN   0xB9    /* 进入掉电模式 */
#define M25PEXX_RDOWN   0xAB    /* 退出掉电模式 */

/* 额外等待 (可选) */
#ifndef M25PEXX_EX_WAIT_IDLE
    #define M25PEXX_EX_WAIT_IDLE
#endif

#if !defined(__no_m25pexx_unlock)
/*
=======================================
    M25PEXX 写入解锁
=======================================
*/
CR_API void_t
m25pexx_unlock (void_t)
{
    byte_t  cmd;

    cmd = M25PEXX_WREN;
    spi_send_data(&cmd, 1);
}

#endif  /* !__no_m25pexx_unlock */

#if !defined(__no_m25pexx_lock)
/*
=======================================
    M25PEXX 写入加锁
=======================================
*/
CR_API void_t
m25pexx_lock (void_t)
{
    byte_t  cmd;

    cmd = M25PEXX_WRDI;
    spi_send_data(&cmd, 1);
}

#endif  /* !__no_m25pexx_lock */

#if !defined(__no_m25pexx_read_id)
/*
=======================================
    M25PEXX 读取器件标志
=======================================
*/
CR_API void_t
m25pexx_read_id (
  __CR_OT__ sM25PEXX*   id
    )
{
    byte_t  cmd = M25PEXX_RDID;

    spi_send_recv(id, 3, &cmd, 1);
}

#endif  /* !__no_m25pexx_read_id */

#if !defined(__no_m25pexx_get_status)
/*
=======================================
    M25PEXX 读状态寄存器
=======================================
*/
CR_API byte_t
m25pexx_get_status (void_t)
{
    byte_t  status;
    byte_t  cmd = M25PEXX_RDSR;

    spi_send_recv(&status, 1, &cmd, 1);
    return (status);
}

#endif  /* !__no_m25pexx_get_status */

#if !defined(__no_m25pexx_wait_idle)
/*
=======================================
    M25PEXX 等待写空闲
=======================================
*/
CR_API retc_t
m25pexx_wait_idle (
  __CR_IN__ uint_t  times
    )
{
    byte_t  status;

    for (; times != 0; times--)
    {
        M25PEXX_EX_WAIT_IDLE
        status = m25pexx_get_status();
        if (!(status & M25PEXX_WIP))
            return (TRUE);
    }
    return (FALSE);
}

#endif  /* !__no_m25pexx_wait_idle */

#if !defined(__no_m25pexx_set_status)
/*
=======================================
    M25PEXX 写状态寄存器
=======================================
*/
CR_API retc_t
m25pexx_set_status (
  __CR_IN__ byte_t  status,
  __CR_IN__ uint_t  timeout
    )
{
    byte_t  cmd[2];

    cmd[0] = M25PEXX_WRSR;
    cmd[1] = status;
    m25pexx_unlock();
    spi_send_data(cmd, 2);
    return (m25pexx_wait_idle(timeout));
}

#endif  /* !__no_m25pexx_set_status */

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

#if !defined(__no_m25pexx_read)
/*
=======================================
    M25PEXX 读取数据
=======================================
*/
CR_API spi_leng_t
m25pexx_read (
  __CR_IN__ spi_addr_t  addr,
  __CR_OT__ void_t*     data,
  __CR_IN__ spi_leng_t  size
    )
{
    byte_t  cmd[4];

    /* 读不需要分页 */
    if (size == 0 || addr >= _SPI_SIZE_ ||
        size > _SPI_SIZE_ - addr)
        return (0);

    /* 填充命令字 */
    cmd[0] = M25PEXX_READ;
    cmd[1] = (byte_t)(addr >> 16);
    cmd[2] = (byte_t)(addr >>  8);
    cmd[3] = (byte_t)(addr >>  0);
    spi_send_recv(data, size, cmd, 4);
    return (size);
}

#endif  /* !__no_m25pexx_read */

#if !defined(__no_m25pexx_fread)
/*
=======================================
    M25PEXX 读取数据 (快速)
=======================================
*/
CR_API spi_leng_t
m25pexx_fread (
  __CR_IN__ spi_addr_t  addr,
  __CR_OT__ void_t*     data,
  __CR_IN__ spi_leng_t  size
    )
{
    byte_t  cmd[5];

    /* 读不需要分页 */
    if (size == 0 || addr >= _SPI_SIZE_ ||
        size > _SPI_SIZE_ - addr)
        return (0);

    /* 填充命令字 */
    cmd[0] = M25PEXX_FREAD;
    cmd[1] = (byte_t)(addr >> 16);
    cmd[2] = (byte_t)(addr >>  8);
    cmd[3] = (byte_t)(addr >>  0);
    cmd[4] = 0x00;
    spi_send_recv(data, size, cmd, 5);
    return (size);
}

#endif  /* !__no_m25pexx_fread */

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

#if !defined(__no_m25pexx_ssec_erase) || \
    !defined(__no_m25pexx_bsec_erase)
/*
---------------------------------------
    M25PEXX 扇区擦除 (内部)
---------------------------------------
*/
static retc_t
m25pexx_sector_erase (
  __CR_IN__ byte_t      type,
  __CR_IN__ spi_addr_t  addr,
  __CR_IN__ uint_t      time,
  __CR_IN__ uint_t      tout
    )
{
    byte_t  cmd[4];

    if (addr >= _SPI_SIZE_)
        return (FALSE);
    if (time == 0) time = 1;
    cmd[0] = type;
    cmd[1] = (byte_t)(addr >> 16);
    cmd[2] = (byte_t)(addr >>  8);
    cmd[3] = (byte_t)(addr >>  0);
    m25pexx_unlock();
    spi_send_data(cmd, 4);
    for (; time != 0; time--) {
        if (m25pexx_wait_idle(tout))
            return (TRUE);
    }
    return (FALSE);
}

#endif  /* !__no_m25pexx_ssec_erase ||
           !__no_m25pexx_bsec_erase */

#if !defined(__no_m25pexx_ssec_erase)
/*
=======================================
    M25PEXX 小扇区擦除
=======================================
*/
CR_API retc_t
m25pexx_ssec_erase (
  __CR_IN__ spi_addr_t  addr,
  __CR_IN__ uint_t      time,
  __CR_IN__ uint_t      tout
    )
{
    return (m25pexx_sector_erase(M25PEXX_SSECE, addr, time, tout));
}

#endif  /* !__no_m25pexx_ssec_erase */

#if !defined(__no_m25pexx_bsec_erase)
/*
=======================================
    M25PEXX 大区块擦除
=======================================
*/
CR_API retc_t
m25pexx_bsec_erase (
  __CR_IN__ spi_addr_t  addr,
  __CR_IN__ uint_t      time,
  __CR_IN__ uint_t      tout
    )
{
    return (m25pexx_sector_erase(M25PEXX_BSECE, addr, time, tout));
}

#endif  /* !__no_m25pexx_bsec_erase */

#if !defined(__no_m25pexx_bulk_erase)
/*
=======================================
    M25PEXX 全芯片擦除
=======================================
*/
CR_API retc_t
m25pexx_bulk_erase (
  __CR_IN__ uint_t  time,
  __CR_IN__ uint_t  tout
    )
{
    byte_t  cmd;

    cmd = M25PEXX_BULKE;
    if (time == 0) time = 1;
    m25pexx_unlock();
    spi_send_data(&cmd, 1);
    for (; time != 0; time--) {
        if (m25pexx_wait_idle(tout))
            return (TRUE);
    }
    return (FALSE);
}

#endif  /* !__no_m25pexx_bulk_erase */

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

#if !defined(__no_m25pexx_program)
/*
---------------------------------------
    M25PEXX 内部页写 (直接)
---------------------------------------
*/
static retc_t
m25pexx_page_prog (
  __CR_IN__ spi_addr_t      addr,
  __CR_IN__ const void_t*   data,
  __CR_IN__ spi_leng_t      size,
  __CR_IN__ uint_t          tout
    )
{
    byte_t  cmd[4];

    cmd[0] = M25PEXX_PAGEP;
    cmd[1] = (byte_t)(addr >> 16);
    cmd[2] = (byte_t)(addr >>  8);
    cmd[3] = (byte_t)(addr >>  0);
    m25pexx_unlock();
    spi_send_send(cmd, 4, data, size);
    return (m25pexx_wait_idle(tout));
}

/*
=======================================
    M25PEXX 写入数据 (直接)
=======================================
*/
CR_API spi_leng_t
m25pexx_program (
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
        if (!m25pexx_page_prog(addr, ptr, size, tout))
            return (0);
        return (size);
    }
    if (!m25pexx_page_prog(addr, ptr, rst, tout))
        return (0);
    ptr  += rst;
    addr += rst;
    size -= rst;
    total = rst;

    /* 分块和尾部 */
    blk = (spi_blks_t)(size / _SPI_PAGE_);
    for (; blk != 0; blk--) {
        if (!m25pexx_page_prog(addr, ptr, _SPI_PAGE_, tout))
            return (total);
        ptr   += _SPI_PAGE_;
        addr  += _SPI_PAGE_;
        total += _SPI_PAGE_;
    }
    rst = (spi_page_t)(size % _SPI_PAGE_);
    if (rst != 0) {
        if (!m25pexx_page_prog(addr, ptr, rst, tout))
            return (total);
        total += rst;
    }
    return (total);
}

#endif  /* !__no_m25pexx_program */

#if !defined(__no_m25pexx_pwr_off)
/*
=======================================
    M25PEXX 进入掉电模式
=======================================
*/
CR_API void_t
m25pexx_pwr_off (void_t)
{
    byte_t  cmd;

    cmd = M25PEXX_DDOWN;
    spi_send_data(&cmd, 1);
}

#endif  /* !__no_m25pexx_pwr_off */

#if !defined(__no_m25pexx_pwr_xon)
/*
=======================================
    M25PEXX 退出掉电模式
=======================================
*/
CR_API void_t
m25pexx_pwr_xon (void_t)
{
    byte_t  cmd;

    cmd = M25PEXX_RDOWN;
    spi_send_data(&cmd, 1);
}

#endif  /* !__no_m25pexx_pwr_xon */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
