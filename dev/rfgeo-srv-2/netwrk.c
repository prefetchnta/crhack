/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2017-03-16  */
/*     #######          ###    ###      [HARD]      ###  ~~~~~~~~~~~~~~~~~~  */
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
/*  >>>>>>>>>>>>>>>>>> RFGEO-SRV-2 采集器网络驱动函数库 <<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "device.h"
#include "mtplib.h"
#include "stm32f10x_conf.h"

/* 通讯板类型 */
extern byte_t   g_net_type;

/* 是否通过桥接板 */
extern bool_t   g_is_bridge;

/*****************************************************************************/
/*                               通用 AT 指令                                */
/*****************************************************************************/

/* 串口 API 映射 */
#define uart_wait       rs232_wait
#define uart_read       rs232_read
#define uart_write      rs232_write
#define uart_throw      rs232_throw
#define uart_rx_flush   rs232_rx_flush

#include "at_cmd.inl"

/*****************************************************************************/
/*                                  SIMCOM                                   */
/*****************************************************************************/

/* 喂狗与不要发得太快 */
#define SIMCOM_YIELD    { delayms(100); wdt_task(); }

#include "atcmd/simcom.bsp"

/*****************************************************************************/
/*                                  NETWRK                                   */
/*****************************************************************************/

/*
=======================================
    模块复位
=======================================
*/
CR_API void_t
netwrk_reset (void_t)
{
    if (g_is_bridge) {
        bridge_gpio(0);
        thread_sleep(1000);
        bridge_gpio(1);
    }
    else {
        ctl_set();
        thread_sleep(1000);
        ctl_clr();
    }
    thread_sleep(1000);
}

/*
=======================================
    模块拨号
=======================================
*/
CR_API bool_t
netwrk_online (
  __CR_IN__ const ansi_t*   apn,
  __CR_IN__ const ansi_t*   user,
  __CR_IN__ const ansi_t*   password,
  __CR_IN__ int32u          timeout
    )
{
    /* 根据模块类型区分 */
    switch (g_net_type)
    {
        default:
        case SRV2NET_SIM5360:
            break;

        case SRV2NET_SIM7100:
            return (sim7100_online(apn, timeout));

        case SRV2NET_SIM6320:
            return (sim6320_online(user, password, timeout));
    }

    /* 未知模块 */
    return (FALSE);
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
