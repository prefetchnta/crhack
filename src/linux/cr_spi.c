/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2016-06-22  */
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
/*  >>>>>>>>>>>>>>>>>>>>> CrHack SPI 函数库 for Linux <<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "devlib.h"
#include "memlib.h"

#if !defined(_CR_NDK_LOW_)

#include <linux/types.h>
#include <linux/ioctl.h>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

/*
=======================================
    打开 SPI
=======================================
*/
CR_API sint_t
spi_open (
  __CR_IN__ const ansi_t*   dev,
  __CR_IN__ uint_t          mode,
  __CR_IN__ uint_t          bits,
  __CR_IN__ uint_t          speed
    )
{
    sint_t  spi;

    if (dev == NULL)
        dev = "/dev/spidev0.0";
    spi = open(dev, O_RDWR);
    if (spi < 0)
        return (-1);
    if (ioctl(spi, SPI_IOC_WR_MODE, &mode) < 0)
        goto _failure;
    if (ioctl(spi, SPI_IOC_WR_BITS_PER_WORD, &bits) < 0)
        goto _failure;
    if (ioctl(spi, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0)
        goto _failure;
    return (spi);

_failure:
    close(spi);
    return (-1);
}

/*
=======================================
    关闭 SPI
=======================================
*/
CR_API void_t
spi_close (
  __CR_IN__ sint_t  spi
    )
{
    close(spi);
}

/*
=======================================
    单读 SPI
=======================================
*/
CR_API bool_t
spi_read (
  __CR_IN__ sint_t  spi,
  __CR_OT__ void_t* data,
  __CR_IN__ uint_t  size
    )
{
    uint_t  back;

    back = (uint_t)read(spi, data, size);
    return (back != size) ? FALSE : TRUE;
}

/*
=======================================
    单写 SPI
=======================================
*/
CR_API bool_t
spi_write (
  __CR_IN__ sint_t          spi,
  __CR_IN__ const void_t*   data,
  __CR_IN__ uint_t          size
    )
{
    uint_t  back;

    back = (uint_t)write(spi, data, size);
    return (back != size) ? FALSE : TRUE;
}

/*
=======================================
    写读 SPI
=======================================
*/
CR_API bool_t
spi_iorw (
  __CR_IN__ sint_t          spi,
  __CR_OT__ void_t*         recv,
  __CR_IN__ const void_t*   send,
  __CR_IN__ uint_t          size,
  __CR_IN__ bool_t          cs_flip
    )
{
    struct spi_ioc_transfer tr;

    mem_zero(&tr, sizeof(tr));
    tr.tx_buf = (size_t)send;
    tr.rx_buf = (size_t)recv;
    tr.len = size;
    tr.cs_change = !!cs_flip;

    if (ioctl(spi, SPI_IOC_MESSAGE(1), &tr) < 1)
        return (FALSE);
    return (TRUE);
}

#endif  /* !_CR_NDK_LOW_ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
