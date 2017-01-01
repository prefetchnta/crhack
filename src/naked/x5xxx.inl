/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2011-09-06  */
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
/*  >>>>>>>>>>>>>>>>>> CrHack XICOR X5XXX 驱动函数库模板 <<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "spi_mm.h"
#include "chip/x5xxx.h"

/* 器件的命令字 */
#define X5XXX_WREN      0x06    /*  关闭写保护  */
#define X5XXX_WRDI      0x04    /*  打开写保护  */
#define X5XXX_SFLB      0x00    /*  设置标志位  */
#define X5XXX_RFLB      0x04    /*  清除标志位  */
#define X5XXX_RSDR      0x05    /* 读状态寄存器 */
#define X5XXX_WRSR      0x01    /* 写状态寄存器 */
#define X5XXX_READ      0x03    /* 读取内存内容 */
#define X5XXX_WRITE     0x02    /* 写入内存内容 */

/* 额外等待 (可选) */
#ifndef X5XXX_EX_WAIT_IDLE
    #define X5XXX_EX_WAIT_IDLE
#endif

#if defined(_X5XXX_HAVE_WDT_)
#if !defined(__no_x5xxx_clear_wdt)
/*
=======================================
    X5XXX 喂看门狗
=======================================
*/
CR_API void_t
x5xxx_clear_wdt (void_t)
{
    SPI_SSEL_CLRB
    X5XXX_WDT_NOP
    SPI_SSEL_SETB
    X5XXX_WDT_NOP
}
#endif  /* !__no_x5xxx_clear_wdt */
#endif  /* _X5XXX_HAVE_WDT_ */

#if !defined(__no_x5xxx_unlock)
/*
=======================================
    X5XXX 写入解锁
=======================================
*/
CR_API void_t
x5xxx_unlock (void_t)
{
    byte_t  cmd;

    cmd = X5XXX_WREN;
    spi_send_data(&cmd, 1);
}

#endif  /* !__no_x5xxx_unlock */

#if !defined(__no_x5xxx_lock)
/*
=======================================
    X5XXX 写入加锁
=======================================
*/
CR_API void_t
x5xxx_lock (void_t)
{
    byte_t  cmd;

    cmd = X5XXX_WRDI;
    spi_send_data(&cmd, 1);
}

#endif  /* !__no_x5xxx_lock */

#if defined(_X5XXX_HAVE_FLB_)
#if !defined(__no_x5xxx_clr_flb)
/*
=======================================
    X5XXX 清除标志位
=======================================
*/
CR_API void_t
x5xxx_clr_flb (void_t)
{
    byte_t  cmd;

    cmd = X5XXX_RFLB;
    spi_send_data(&cmd, 1);
}

#endif  /* !__no_x5xxx_clr_flb */

#if !defined(__no_x5xxx_set_flb)
/*
=======================================
    X5XXX 设置标志位
=======================================
*/
CR_API void_t
x5xxx_set_flb (void_t)
{
    byte_t  cmd;

    cmd = X5XXX_SFLB;
    spi_send_data(&cmd, 1);
}
#endif  /* !__no_x5xxx_set_flb */
#endif  /* _X5XXX_HAVE_FLB_ */

#if !defined(__no_x5xxx_get_status)
/*
=======================================
    X5XXX 读状态寄存器
=======================================
*/
CR_API byte_t
x5xxx_get_status (void_t)
{
    byte_t  status;
    byte_t  cmd = X5XXX_RSDR;

    spi_send_recv(&status, 1, &cmd, 1);
    return (status);
}

#endif  /* !__no_x5xxx_get_status */

#if !defined(__no_x5xxx_wait_idle)
/*
=======================================
    X5XXX 等待写空闲
=======================================
*/
CR_API retc_t
x5xxx_wait_idle (
  __CR_IN__ uint_t  times
    )
{
    byte_t  status;

    for (; times != 0; times--)
    {
        X5XXX_EX_WAIT_IDLE
        status = x5xxx_get_status();
        if (!(status & X5XXX_WIP))
            return (TRUE);
    }
    return (FALSE);
}

#endif  /* !__no_x5xxx_wait_idle */

#if !defined(__no_x5xxx_set_status)
/*
=======================================
    X5XXX 写状态寄存器
=======================================
*/
CR_API retc_t
x5xxx_set_status (
  __CR_IN__ byte_t  status,
  __CR_IN__ uint_t  t5ms
    )
{
    byte_t  cmd[2];

    cmd[0] = X5XXX_WRSR;
    cmd[1] = status;
    x5xxx_unlock();
    spi_send_data(cmd, 2);
    return (x5xxx_wait_idle(t5ms));
}

#endif  /* !__no_x5xxx_set_status */

#if !defined(__no_x5xxx_read)
/*
=======================================
    X5XXX 读取数据
=======================================
*/
CR_API spi_leng_t
x5xxx_read (
  __CR_IN__ spi_addr_t  addr,
  __CR_OT__ void_t*     data,
  __CR_IN__ spi_leng_t  size
    )
{
    byte_t  cmd[3];

    /* 读不需要分页 */
    if (size == 0 || addr >= _SPI_SIZE_ ||
        size > _SPI_SIZE_ - addr)
        return (0);

    /* 填充命令字 */
    cmd[0]  = X5XXX_READ;
#if (_SPI_SIZE_ == 512)
    cmd[0] |= (byte_t)(addr >> 5) & 8;
    cmd[1]  = (byte_t)(addr >> 0);
    spi_send_recv(data, size, cmd, 2);
#else
    cmd[1]  = (byte_t)(addr >> 8);
    cmd[2]  = (byte_t)(addr >> 0);
    spi_send_recv(data, size, cmd, 3);
#endif
    return (size);
}

#endif  /* !__no_x5xxx_read */

#if !defined(__no_x5xxx_write)
/*
---------------------------------------
    X5XXX 内部页写
---------------------------------------
*/
static retc_t
x5xxx_page_write (
  __CR_IN__ spi_addr_t      addr,
  __CR_IN__ const void_t*   data,
  __CR_IN__ spi_leng_t      size,
  __CR_IN__ uint_t          t5ms
    )
{
    byte_t  cmd[3];

    /* 填充命令字 */
    cmd[0]  = X5XXX_WRITE;
#if (_SPI_SIZE_ == 512)
    cmd[0] |= (byte_t)(addr >> 5) & 8;
    cmd[1]  = (byte_t)(addr >> 0);
    x5xxx_unlock();
    spi_send_send(cmd, 2, data, size);
#else
    cmd[1]  = (byte_t)(addr >> 8);
    cmd[2]  = (byte_t)(addr >> 0);
    x5xxx_unlock();
    spi_send_send(cmd, 3, data, size);
#endif
    return (x5xxx_wait_idle(t5ms));
}

/*
=======================================
    X5XXX 写入数据
=======================================
*/
CR_API spi_leng_t
x5xxx_write (
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
        if (!x5xxx_page_write(addr, ptr, size, t5ms))
            return (0);
        return (size);
    }
    if (!x5xxx_page_write(addr, ptr, rst, t5ms))
        return (0);
    ptr  += rst;
    addr += rst;
    size -= rst;
    total = rst;

    /* 分块和尾部 */
    blk = (spi_blks_t)(size / _SPI_PAGE_);
    for (; blk != 0; blk--) {
        if (!x5xxx_page_write(addr, ptr, _SPI_PAGE_, t5ms))
            return (total);
        ptr   += _SPI_PAGE_;
        addr  += _SPI_PAGE_;
        total += _SPI_PAGE_;
    }
    rst = (spi_page_t)(size % _SPI_PAGE_);
    if (rst != 0) {
        if (!x5xxx_page_write(addr, ptr, rst, t5ms))
            return (total);
        total += rst;
    }
    return (total);
}

#endif  /* !__no_x5xxx_write */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
