/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2020-12-16  */
/*     #######          ###    ###      [CORE]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>>>>>> CrHack I2C 函数库 for Linux <<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "devlib.h"
#include "memlib.h"

#if defined(_CR_NDK_LOW_)
    #define _CR_NO_LINUX_I2C_
#endif
#include <linux/types.h>
#include <linux/ioctl.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#if defined(_CR_NO_LINUX_I2C_)
    #include "miss/i2c.h"
    #include "miss/i2c-dev.h"
#else
    #include <linux/i2c.h>
    #include <linux/i2c-dev.h>
#endif

/*
=======================================
    打开 I2C
=======================================
*/
CR_API sint_t
i2c_open (
  __CR_IN__ const ansi_t*   dev
    )
{
    if (dev == NULL)
        dev = "/dev/i2c-0";
    return (open(dev, O_RDWR));
}

/*
=======================================
    关闭 I2C
=======================================
*/
CR_API void_t
i2c_close (
  __CR_IN__ sint_t  i2c
    )
{
    close(i2c);
}

/*
=======================================
    读取 I2C
=======================================
*/
CR_API bool_t
i2c_read (
  __CR_IN__ sint_t          i2c,
  __CR_IN__ int16u          mode,
  __CR_IN__ int16u          devs,
  __CR_OT__ void_t*         rdata,
  __CR_IN__ int16u          nrecv,
  __CR_IN__ const void_t*   sdata,
  __CR_IN__ int16u          nsend
    )
{
    struct i2c_msg              m[2];
    struct i2c_rdwr_ioctl_data  idat;

    idat.msgs = m;
    idat.nmsgs = 2;

    mode &= ~I2C_M_RD;
    m[0].addr = devs;
    m[0].flags = mode;
    m[0].len = nsend;
    m[0].buf = (byte_t*)sdata;

    m[1].addr = devs;
    m[1].flags = mode | I2C_M_RD;
    m[1].len = nrecv;
    m[1].buf = (byte_t*)rdata;
    mem_zero(rdata, nrecv);

    if (ioctl(i2c, I2C_RDWR, &idat) < 0)
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    写入 I2C
=======================================
*/
CR_API bool_t
i2c_write (
  __CR_IN__ sint_t          i2c,
  __CR_IN__ int16u          mode,
  __CR_IN__ int16u          devs,
  __CR_IN__ const void_t*   data,
  __CR_IN__ int16u          size
    )
{
    struct i2c_msg              imsg;
    struct i2c_rdwr_ioctl_data  idat;

    idat.msgs = &imsg;
    idat.nmsgs = 1;

    imsg.addr = devs;
    imsg.flags = mode & (~I2C_M_RD);
    imsg.len = size;
    imsg.buf = (byte_t*)data;

    if (ioctl(i2c, I2C_RDWR, &idat) < 0)
        return (FALSE);
    return (TRUE);
}

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
