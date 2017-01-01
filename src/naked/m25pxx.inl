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

/* 器件的命令字 */
#define M25PXX_WREN     0x06    /*  关闭写保护  */
#define M25PXX_WRDI     0x04    /*  打开写保护  */
#define M25PXX_RDID     0x9F    /*  读器件标志  */
#define M25PXX_RDSR     0x05    /* 读状态寄存器 */
#define M25PXX_WRSR     0x01    /* 写状态寄存器 */
#define M25PXX_READ     0x03    /* 读取内存内容 */
#define M25PXX_FREAD    0x0B    /* 快速读取内容 */
#define M25PXX_PAGEP    0x02    /* 写入内存内容 */
#define M25PXX_BSECE    0xD8    /* 大扇区的擦除 */
#define M25PXX_BULKE    0xC7    /* 全芯片的擦除 */
#define M25PXX_DDOWN    0xB9    /* 进入掉电模式 */
#define M25PXX_RDOWN    0xAB    /* 退出掉电模式 */

/* 额外等待 (可选) */
#ifndef M25PXX_EX_WAIT_IDLE
    #define M25PXX_EX_WAIT_IDLE
#endif

#if !defined(__no_m25pxx_unlock)
/*
=======================================
    M25PXX 写入解锁
=======================================
*/
CR_API void_t
m25pxx_unlock (void_t)
{
    byte_t  cmd;

    cmd = M25PXX_WREN;
    spi_send_data(&cmd, 1);
}

#endif  /* !__no_m25pxx_unlock */

#if !defined(__no_m25pxx_lock)
/*
=======================================
    M25PXX 写入加锁
=======================================
*/
CR_API void_t
m25pxx_lock (void_t)
{
    byte_t  cmd;

    cmd = M25PXX_WRDI;
    spi_send_data(&cmd, 1);
}

#endif  /* !__no_m25pxx_lock */

#if !defined(__no_m25pxx_read_id)
/*
=======================================
    M25PXX 读取器件标志
=======================================
*/
CR_API void_t
m25pxx_read_id (
  __CR_OT__ void_t* id
    )
{
    byte_t  cmd = M25PXX_RDID;

    spi_send_recv(id, M25PXX_ID_LEN, &cmd, 1);
}

#endif  /* !__no_m25pxx_read_id */

#if !defined(__no_m25pxx_get_status)
/*
=======================================
    M25PXX 读状态寄存器
=======================================
*/
CR_API byte_t
m25pxx_get_status (void_t)
{
    byte_t  status;
    byte_t  cmd = M25PXX_RDSR;

    spi_send_recv(&status, 1, &cmd, 1);
    return (status);
}

#endif  /* !__no_m25pxx_get_status */

#if !defined(__no_m25pxx_wait_idle)
/*
=======================================
    M25PXX 等待写空闲
=======================================
*/
CR_API retc_t
m25pxx_wait_idle (
  __CR_IN__ uint_t  times
    )
{
    byte_t  status;

    for (; times != 0; times--)
    {
        M25PXX_EX_WAIT_IDLE
        status = m25pxx_get_status();
        if (!(status & M25PXX_WIP))
            return (TRUE);
    }
    return (FALSE);
}

#endif  /* !__no_m25pxx_wait_idle */

#if !defined(__no_m25pxx_set_status)
/*
=======================================
    M25PXX 写状态寄存器
=======================================
*/
CR_API retc_t
m25pxx_set_status (
  __CR_IN__ byte_t  status,
  __CR_IN__ uint_t  t1ms
    )
{
    byte_t  cmd[2];

    cmd[0] = M25PXX_WRSR;
    cmd[1] = status;
    m25pxx_unlock();
    spi_send_data(cmd, 2);
    return (m25pxx_wait_idle(t1ms));
}

#endif  /* !__no_m25pxx_set_status */

#if !defined(__no_m25pxx_read)
/*
=======================================
    M25PXX 读取数据
=======================================
*/
CR_API spi_leng_t
m25pxx_read (
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
    cmd[0] = M25PXX_READ;
    cmd[1] = (byte_t)(addr >> 16);
    cmd[2] = (byte_t)(addr >>  8);
    cmd[3] = (byte_t)(addr >>  0);
    spi_send_recv(data, size, cmd, 4);
    return (size);
}

#endif  /* !__no_m25pxx_read */

#if !defined(__no_m25pxx_fread)
/*
=======================================
    M25PXX 读取数据 (快速)
=======================================
*/
CR_API spi_leng_t
m25pxx_fread (
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
    cmd[0] = M25PXX_FREAD;
    cmd[1] = (byte_t)(addr >> 16);
    cmd[2] = (byte_t)(addr >>  8);
    cmd[3] = (byte_t)(addr >>  0);
    cmd[4] = 0x00;
    spi_send_recv(data, size, cmd, 5);
    return (size);
}

#endif  /* !__no_m25pxx_fread */

#if !defined(__no_m25pxx_bsec_erase)
/*
=======================================
    M25PXX 大扇区擦除
=======================================
*/
CR_API retc_t
m25pxx_bsec_erase (
  __CR_IN__ spi_addr_t  addr,
  __CR_IN__ uint_t      time,
  __CR_IN__ uint_t      t5s
    )
{
    byte_t  cmd[4];

    if (addr >= _SPI_SIZE_)
        return (FALSE);
    if (time == 0) time = 1;
    cmd[0] = M25PXX_BSECE;
    cmd[1] = (byte_t)(addr >> 16);
    cmd[2] = (byte_t)(addr >>  8);
    cmd[3] = (byte_t)(addr >>  0);
    m25pxx_unlock();
    spi_send_data(cmd, 4);
    for (; time != 0; time--) {
        if (m25pxx_wait_idle(t5s))
            return (TRUE);
    }
    return (FALSE);
}

#endif  /* !__no_m25pxx_bsec_erase */

#if !defined(__no_m25pxx_bulk_erase)
/*
=======================================
    M25PXX 全芯片擦除
=======================================
*/
CR_API retc_t
m25pxx_bulk_erase (
  __CR_IN__ uint_t  time,
  __CR_IN__ uint_t  t10s
    )
{
    byte_t  cmd;

    cmd = M25PXX_BULKE;
    if (time == 0) time = 1;
    m25pxx_unlock();
    spi_send_data(&cmd, 1);
    for (; time != 0; time--) {
        if (m25pxx_wait_idle(t10s))
            return (TRUE);
    }
    return (FALSE);
}

#endif  /* !__no_m25pxx_bulk_erase */

#if !defined(__no_m25pxx_program)
/*
---------------------------------------
    M25PXX 内部页写 (直接)
---------------------------------------
*/
static retc_t
m25pxx_page_prog (
  __CR_IN__ spi_addr_t      addr,
  __CR_IN__ const void_t*   data,
  __CR_IN__ spi_leng_t      size,
  __CR_IN__ uint_t          t5ms
    )
{
    byte_t  cmd[4];

    cmd[0] = M25PXX_PAGEP;
    cmd[1] = (byte_t)(addr >> 16);
    cmd[2] = (byte_t)(addr >>  8);
    cmd[3] = (byte_t)(addr >>  0);
    m25pxx_unlock();
    spi_send_send(cmd, 4, data, size);
    return (m25pxx_wait_idle(t5ms));
}

/*
=======================================
    M25PXX 写入数据 (直接)
=======================================
*/
CR_API spi_leng_t
m25pxx_program (
  __CR_IN__ spi_addr_t      addr,
  __CR_IN__ const void_t*   data,
  __CR_IN__ spi_leng_t      size,
  __CR_IN__ uint_t          t5ms
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
        if (!m25pxx_page_prog(addr, ptr, size, t5ms))
            return (0);
        return (size);
    }
    if (!m25pxx_page_prog(addr, ptr, rst, t5ms))
        return (0);
    ptr  += rst;
    addr += rst;
    size -= rst;
    total = rst;

    /* 分块和尾部 */
    blk = (spi_blks_t)(size / _SPI_PAGE_);
    for (; blk != 0; blk--) {
        if (!m25pxx_page_prog(addr, ptr, _SPI_PAGE_, t5ms))
            return (total);
        ptr   += _SPI_PAGE_;
        addr  += _SPI_PAGE_;
        total += _SPI_PAGE_;
    }
    rst = (spi_page_t)(size % _SPI_PAGE_);
    if (rst != 0) {
        if (!m25pxx_page_prog(addr, ptr, rst, t5ms))
            return (total);
        total += rst;
    }
    return (total);
}

#endif  /* !__no_m25pxx_program */

#if !defined(__no_m25pxx_pwr_off)
/*
=======================================
    M25PXX 进入掉电模式
=======================================
*/
CR_API void_t
m25pxx_pwr_off (void_t)
{
    byte_t  cmd;

    cmd = M25PXX_DDOWN;
    spi_send_data(&cmd, 1);
}

#endif  /* !__no_m25pxx_pwr_off */

#if !defined(__no_m25pxx_pwr_xon)
/*
=======================================
    M25PXX 退出掉电模式
=======================================
*/
CR_API void_t
m25pxx_pwr_xon (void_t)
{
    byte_t  cmd;

    cmd = M25PXX_RDOWN;
    spi_send_data(&cmd, 1);
}

#endif  /* !__no_m25pxx_pwr_xon */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
