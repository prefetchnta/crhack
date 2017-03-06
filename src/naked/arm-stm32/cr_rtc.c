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
/*  >>>>>>>>>>>>>>>>>>> CrHack 日期时间函数库 for STM32 <<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "devlib.h"
#include "rtclib.h"
#include "stm32cpu.h"

#if defined(_CR_STM32F10X_)

/*
=======================================
    初始化 RTC (片内)
=======================================
*/
CR_API void_t
rtc_init (void_t)
{
    sDATETIME   temp;

    /* 判断电池是否掉电, 掉电要重新初始化 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR |
                           RCC_APB1Periph_BKP, ENABLE);
    PWR_BackupAccessCmd(ENABLE);
    if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5) {
        BKP_DeInit();
        RCC_LSEConfig(RCC_LSE_ON);
        while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
        RCC_RTCCLKCmd(ENABLE);
        RTC_WaitForSynchro();
        RTC_WaitForLastTask();
        RTC_SetPrescaler(32767);
        RTC_WaitForLastTask();
        RTC_ITConfig(RTC_IT_SEC, ENABLE);
        RTC_WaitForLastTask();

        /* 初始时间 */
        temp.year = 2010;
        temp.month = 3;
        temp.day = 22;
        temp.hour = 0;
        temp.minute = 0;
        temp.second = 0;
        temp.week = 1;
        datetime_set(&temp);
        BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
    }
    else {
        RCC_ClearFlag();
        RTC_ITConfig(RTC_IT_SEC, ENABLE);
        RTC_WaitForLastTask();
    }
}

/*
=======================================
    获取系统时间
=======================================
*/
CR_API bool_t
datetime_get (
  __CR_OT__ sDATETIME*  datetime
    )
{
    if (!datetime_from_unx(datetime, RTC_GetCounter()))
        return (FALSE);
    return (TRUE);
}

/*
=======================================
    设置系统时间
=======================================
*/
CR_API bool_t
datetime_set (
  __CR_IN__ const sDATETIME*    datetime
    )
{
    int64u  count;

    if (!datetime_to_unx(&count, datetime))
        return (FALSE);
    if (count > 0xFFFFFFFFUL)
        return (FALSE);
    RTC_SetCounter((int32u)count);
    RTC_WaitForLastTask();
    return (TRUE);
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
