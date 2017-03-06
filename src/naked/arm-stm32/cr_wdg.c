/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2017-03-05  */
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
/*  >>>>>>>>>>>>>>>>>>>> CrHack 看门狗函数库 for STM32 <<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "devlib.h"
#include "stm32cpu.h"

#if defined(_CR_STM32F10X_)

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
    CR_NOUSE(dev);
    return (0);
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
    CR_NOUSE(wdg);
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
    CR_NOUSE(wdg);
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    if (time_ms <= 800)
    {
        /* 40KHz / 8 = 0.2ms */
        time_ms = (time_ms * 10) / 2;
        IWDG_SetPrescaler(IWDG_Prescaler_8);
    }
    else
    {
        /* 40KHz / 256 = 6.4ms */
        time_ms = (time_ms * 10) / 64;
        if (time_ms > 4095)
            time_ms = 4095;
        IWDG_SetPrescaler(IWDG_Prescaler_256);
    }
    IWDG_SetReload((int16u)time_ms);
    IWDG_ReloadCounter();
    IWDG_Enable();
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
    CR_NOUSE(wdg);
    IWDG_ReloadCounter();
}

#endif  /* _CR_STM32F10X_ */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
