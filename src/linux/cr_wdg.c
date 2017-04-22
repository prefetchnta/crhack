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
/*  >>>>>>>>>>>>>>>>>>>> CrHack 看门狗函数库 for Linux <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "devlib.h"

#if !defined(_CR_NDK_LOW_)

#include <linux/types.h>
#include <linux/ioctl.h>
#include <linux/watchdog.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

/*
=======================================
    打开看门狗
=======================================
*/
CR_API sint_t
wdg_open (
  __CR_IN__ const ansi_t*   dev
    )
{
    if (dev == NULL)
        dev = "/dev/watchdog";
    return (open(dev, O_WRONLY));
}

/*
=======================================
    关闭看门狗
=======================================
*/
CR_API void_t
wdg_close (
  __CR_IN__ sint_t  wdg
    )
{
    close(wdg);
}

/*
=======================================
    设置看门狗超时
=======================================
*/
CR_API void_t
wdg_timeout (
  __CR_IN__ sint_t  wdg,
  __CR_IN__ uint_t  time_ms
    )
{
    time_ms /= 1000;
    ioctl(wdg, WDIOC_SETTIMEOUT, &time_ms);
}

/*
=======================================
    看门狗喂狗
=======================================
*/
CR_API void_t
wdg_feed (
  __CR_IN__ sint_t  wdg
    )
{
    ioctl(wdg, WDIOC_KEEPALIVE);
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
