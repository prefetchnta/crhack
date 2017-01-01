/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-08-09  */
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
/*  >>>>>>>>>>>>>>>>>>>>> CrHack 24CXX 驱动函数库模板 <<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "i2c_mm.h"

#if !defined(__no_e24cxx_read)
/*
=======================================
    读取 24CXX 地址数据
=======================================
*/
CR_API i2c_leng_t
e24cxx_read (
  __CR_IN__ byte_t      devs,
  __CR_IN__ i2c_addr_t  addr,
  __CR_OT__ void_t*     data,
  __CR_IN__ i2c_leng_t  size
    )
{
    /* 读不需要分页 */
    if (size == 0 || addr >= _I2C_SIZE_ ||
        size > _I2C_SIZE_ - addr)
        return (0);
    return (i2c_read_device(devs, addr, data, size));
}

#endif  /* !__no_e24cxx_read */

#if !defined(__no_e24cxx_write)
/*
=======================================
    写入 24CXX 地址数据
=======================================
*/
CR_API i2c_leng_t
e24cxx_write (
  __CR_IN__ byte_t          devs,
  __CR_IN__ i2c_addr_t      addr,
  __CR_IN__ const void_t*   data,
  __CR_IN__ i2c_leng_t      size
    )
{
#ifndef _I2C_PAGE_
    if (size == 0 || addr >= _I2C_SIZE_ ||
        size > _I2C_SIZE_ - addr)
        return (0);
    return (i2c_write_device(devs, addr, data, size));
#else
    byte_t*     ptr;
    i2c_blks_t  blk;
    i2c_page_t  rst;
    i2c_page_t  wrt;
    i2c_leng_t  total;

    if (size == 0 || addr >= _I2C_SIZE_ ||
        size > _I2C_SIZE_ - addr)
        return (0);

    /* 前地址对齐 */
    ptr = (byte_t*)data;
    rst = (i2c_page_t)(_I2C_PAGE_ - addr % _I2C_PAGE_);
    if (rst >= size) {
        wrt = (i2c_page_t)i2c_write_device(devs, addr, ptr, size);
        E24C_WR_DELAY
        return (wrt);
    }
    wrt = (i2c_page_t)i2c_write_device(devs, addr, ptr, rst);
    E24C_WR_DELAY
    if (wrt != rst)
        return (wrt);
    ptr  += wrt;
    addr += wrt;
    size -= wrt;
    total = wrt;

    /* 分块和尾部 */
    blk = (i2c_blks_t)(size / _I2C_PAGE_);
    for (; blk != 0; blk--) {
        wrt = (i2c_page_t)i2c_write_device(devs, addr, ptr, _I2C_PAGE_);
        E24C_WR_DELAY
        if (wrt != _I2C_PAGE_)
            return (total + wrt);
        ptr   += _I2C_PAGE_;
        addr  += _I2C_PAGE_;
        total += _I2C_PAGE_;
    }
    rst = (i2c_page_t)(size % _I2C_PAGE_);
    if (rst != 0) {
        wrt = (i2c_page_t)i2c_write_device(devs, addr, ptr, rst);
        E24C_WR_DELAY
        total += wrt;
    }
    return (total);
#endif
}

#endif  /* !__no_e24cxx_write */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
