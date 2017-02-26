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
#define MX25LXX_WREN    0x06    /*  关闭写保护  */
#define MX25LXX_WRDI    0x04    /*  打开写保护  */
#define MX25LXX_RDID    0x9F    /*  读器件标志  */
#define MX25LXX_RMID    0x90    /*  读厂家标志  */
#define MX25LXX_RDSR    0x05    /* 读状态寄存器 */
#define MX25LXX_WRSR    0x01    /* 写状态寄存器 */
#define MX25LXX_READ    0x03    /* 读取内存内容 */
#define MX25LXX_FREAD   0x0B    /* 快速读取内容 */
#define MX25LXX_DREAD   0x3B    /* 双通道的读取 */
#define MX25LXX_PAGEP   0x02    /* 写入内存内容 */
#define MX25LXX_SSECE   0x20    /* 小扇区的擦除 */
#define MX25LXX_BSECE   0xD8    /* 大区块的擦除 */
#define MX25LXX_BULKE   0xC7    /* 全芯片的擦除 */
#define MX25LXX_DDOWN   0xB9    /* 进入掉电模式 */
#define MX25LXX_RDOWN   0xAB    /* 退出掉电模式 */
#define MX25LXX_ENSO    0xB1    /* 进入安全模式 */
#define MX25LXX_EXSO    0xC1    /* 退出安全模式 */
#define MX25LXX_RDSCUR  0x2B    /* 读安全寄存器 */
#define MX25LXX_WRSCUR  0x2F    /* 写安全寄存器 */
#define MX25LXX_RDSFDP  0x5A    /* 读取闪存信息 */

/* 额外等待 (可选) */
#ifndef MX25LXX_EX_WAIT_IDLE
    #define MX25LXX_EX_WAIT_IDLE
#endif

#if !defined(__no_mx25lxx_unlock)
/*
=======================================
    MX25LXX 写入解锁
=======================================
*/
CR_API void_t
mx25lxx_unlock (void_t)
{
    byte_t  cmd;

    cmd = MX25LXX_WREN;
    spi_send_data(&cmd, 1);
}

#endif  /* !__no_mx25lxx_unlock */

#if !defined(__no_mx25lxx_lock)
/*
=======================================
    MX25LXX 写入加锁
=======================================
*/
CR_API void_t
mx25lxx_lock (void_t)
{
    byte_t  cmd;

    cmd = MX25LXX_WRDI;
    spi_send_data(&cmd, 1);
}

#endif  /* !__no_mx25lxx_lock */

#if !defined(__no_mx25lxx_read_id)
/*
=======================================
    MX25LXX 读取器件标志
=======================================
*/
CR_API void_t
mx25lxx_read_id (
  __CR_OT__ sMX25LXX*   id
    )
{
    byte_t  cmd = MX25LXX_RDID;

    spi_send_recv(id, 3, &cmd, 1);
}

#endif  /* !__no_mx25lxx_read_id */

#if !defined(__no_mx25lxx_read_mid)
/*
=======================================
    MX25LXX 读取厂家标志
=======================================
*/
CR_API void_t
mx25lxx_read_mid (
  __CR_OT__ sMX25LXX_M* mid
    )
{
    byte_t  cmd[4];

    cmd[0] = MX25LXX_RMID;
    cmd[1] = 0x00;
    cmd[2] = 0x00;
    cmd[3] = 0x00;
    spi_send_recv(mid, 2, cmd, 4);
}

#endif  /* !__no_mx25lxx_read_mid */

#if !defined(__no_mx25lxx_get_status)
/*
=======================================
    MX25LXX 读状态寄存器
=======================================
*/
CR_API byte_t
mx25lxx_get_status (void_t)
{
    byte_t  status;
    byte_t  cmd = MX25LXX_RDSR;

    spi_send_recv(&status, 1, &cmd, 1);
    return (status);
}

#endif  /* !__no_mx25lxx_get_status */

#if !defined(__no_mx25lxx_wait_idle)
/*
=======================================
    MX25LXX 等待写空闲
=======================================
*/
CR_API retc_t
mx25lxx_wait_idle (
  __CR_IN__ uint_t  times
    )
{
    byte_t  status;

    for (; times != 0; times--)
    {
        MX25LXX_EX_WAIT_IDLE
        status = mx25lxx_get_status();
        if (!(status & MX25LXX_WIP))
            return (TRUE);
    }
    return (FALSE);
}

#endif  /* !__no_mx25lxx_wait_idle */

#if !defined(__no_mx25lxx_set_status)
/*
=======================================
    MX25LXX 写状态寄存器
=======================================
*/
CR_API retc_t
mx25lxx_set_status (
  __CR_IN__ byte_t  status,
  __CR_IN__ uint_t  timeout
    )
{
    byte_t  cmd[2];

    cmd[0] = MX25LXX_WRSR;
    cmd[1] = status;
    mx25lxx_unlock();
    spi_send_data(cmd, 2);
    return (mx25lxx_wait_idle(timeout));
}

#endif  /* !__no_mx25lxx_set_status */

#if !defined(__no_mx25lxx_read)
/*
=======================================
    MX25LXX 读取数据
=======================================
*/
CR_API spi_leng_t
mx25lxx_read (
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
    cmd[0] = MX25LXX_READ;
    cmd[1] = (byte_t)(addr >> 16);
    cmd[2] = (byte_t)(addr >>  8);
    cmd[3] = (byte_t)(addr >>  0);
    spi_send_recv(data, size, cmd, 4);
    return (size);
}

#endif  /* !__no_mx25lxx_read */

#if !defined(__no_mx25lxx_fread)
/*
=======================================
    MX25LXX 读取数据 (快速)
=======================================
*/
CR_API spi_leng_t
mx25lxx_fread (
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
    cmd[0] = MX25LXX_FREAD;
    cmd[1] = (byte_t)(addr >> 16);
    cmd[2] = (byte_t)(addr >>  8);
    cmd[3] = (byte_t)(addr >>  0);
    cmd[4] = 0x00;
    spi_send_recv(data, size, cmd, 5);
    return (size);
}

#endif  /* !__no_mx25lxx_fread */

#if !defined(__no_mx25lxx_ssec_erase) || \
    !defined(__no_mx25lxx_bsec_erase)
/*
---------------------------------------
    MX25LXX 扇区擦除 (内部)
---------------------------------------
*/
static retc_t
mx25lxx_sector_erase (
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
    mx25lxx_unlock();
    spi_send_data(cmd, 4);
    for (; time != 0; time--) {
        if (mx25lxx_wait_idle(tout))
            return (TRUE);
    }
    return (FALSE);
}

#endif  /* !__no_mx25lxx_ssec_erase ||
           !__no_mx25lxx_bsec_erase */

#if !defined(__no_mx25lxx_ssec_erase)
/*
=======================================
    MX25LXX 小扇区擦除
=======================================
*/
CR_API retc_t
mx25lxx_ssec_erase (
  __CR_IN__ spi_addr_t  addr,
  __CR_IN__ uint_t      time,
  __CR_IN__ uint_t      tout
    )
{
    return (mx25lxx_sector_erase(MX25LXX_SSECE, addr, time, tout));
}

#endif  /* !__no_mx25lxx_ssec_erase */

#if !defined(__no_mx25lxx_bsec_erase)
/*
=======================================
    MX25LXX 大区块擦除
=======================================
*/
CR_API retc_t
mx25lxx_bsec_erase (
  __CR_IN__ spi_addr_t  addr,
  __CR_IN__ uint_t      time,
  __CR_IN__ uint_t      tout
    )
{
    return (mx25lxx_sector_erase(MX25LXX_BSECE, addr, time, tout));
}

#endif  /* !__no_mx25lxx_bsec_erase */

#if !defined(__no_mx25lxx_bulk_erase)
/*
=======================================
    MX25LXX 全芯片擦除
=======================================
*/
CR_API retc_t
mx25lxx_bulk_erase (
  __CR_IN__ uint_t  time,
  __CR_IN__ uint_t  tout
    )
{
    byte_t  cmd;

    cmd = MX25LXX_BULKE;
    if (time == 0) time = 1;
    mx25lxx_unlock();
    spi_send_data(&cmd, 1);
    for (; time != 0; time--) {
        if (mx25lxx_wait_idle(tout))
            return (TRUE);
    }
    return (FALSE);
}

#endif  /* !__no_mx25lxx_bulk_erase */

#if !defined(__no_mx25lxx_program)
/*
---------------------------------------
    MX25LXX 内部页写 (直接)
---------------------------------------
*/
static retc_t
mx25lxx_page_prog (
  __CR_IN__ spi_addr_t      addr,
  __CR_IN__ const void_t*   data,
  __CR_IN__ spi_leng_t      size,
  __CR_IN__ uint_t          tout
    )
{
    byte_t  cmd[4];

    cmd[0] = MX25LXX_PAGEP;
    cmd[1] = (byte_t)(addr >> 16);
    cmd[2] = (byte_t)(addr >>  8);
    cmd[3] = (byte_t)(addr >>  0);
    mx25lxx_unlock();
    spi_send_send(cmd, 4, data, size);
    return (mx25lxx_wait_idle(tout));
}

/*
=======================================
    MX25LXX 写入数据 (直接)
=======================================
*/
CR_API spi_leng_t
mx25lxx_program (
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
        if (!mx25lxx_page_prog(addr, ptr, size, tout))
            return (0);
        return (size);
    }
    if (!mx25lxx_page_prog(addr, ptr, rst, tout))
        return (0);
    ptr  += rst;
    addr += rst;
    size -= rst;
    total = rst;

    /* 分块和尾部 */
    blk = (spi_blks_t)(size / _SPI_PAGE_);
    for (; blk != 0; blk--) {
        if (!mx25lxx_page_prog(addr, ptr, _SPI_PAGE_, tout))
            return (total);
        ptr   += _SPI_PAGE_;
        addr  += _SPI_PAGE_;
        total += _SPI_PAGE_;
    }
    rst = (spi_page_t)(size % _SPI_PAGE_);
    if (rst != 0) {
        if (!mx25lxx_page_prog(addr, ptr, rst, tout))
            return (total);
        total += rst;
    }
    return (total);
}

#endif  /* !__no_mx25lxx_program */

#if !defined(__no_mx25lxx_pwr_off)
/*
=======================================
    MX25LXX 进入掉电模式
=======================================
*/
CR_API void_t
mx25lxx_pwr_off (void_t)
{
    byte_t  cmd;

    cmd = MX25LXX_DDOWN;
    spi_send_data(&cmd, 1);
}

#endif  /* !__no_mx25lxx_pwr_off */

#if !defined(__no_mx25lxx_pwr_xon)
/*
=======================================
    MX25LXX 退出掉电模式
=======================================
*/
CR_API void_t
mx25lxx_pwr_xon (void_t)
{
    byte_t  cmd;

    cmd = MX25LXX_RDOWN;
    spi_send_data(&cmd, 1);
}

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
