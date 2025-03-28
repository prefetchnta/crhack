/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2017-03-13  */
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
/*  >>>>>>>>>>>>>>>>>> RFGEO-SRV-2 采集器系统调用函数库 <<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "applib.h"
#include "device.h"
#include "mtplib.h"
#include "strlib.h"

/*****************************************************************************/
/*                                   RTC                                     */
/*****************************************************************************/

/*
=======================================
    RTC 初始化
=======================================
*/
CR_API void_t
rtc_init (void_t)
{
    uint_t  idx;

    /* 重试几次 */
    for (idx = 0; idx < 5; idx++) {
        if (rx8025_init_time())
            break;
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
    uint_t  idx;

    /* 重试几次 */
    for (idx = 0; idx < 5; idx++) {
        if (rx8025_get_time(datetime))
            return (TRUE);
    }
    return (FALSE);
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
    uint_t  idx;

    /* 强制设置星期 */
    if (!datetime_chk(datetime))
        return (FALSE);
    date_set_week((sDATETIME*)datetime);

    /* 重试几次 */
    for (idx = 0; idx < 5; idx++) {
        if (rx8025_set_time(datetime))
            return (TRUE);
    }
    return (FALSE);
}

/*****************************************************************************/
/*                                   NET                                     */
/*****************************************************************************/

/* 通讯超时 */
#define AT_TIMEOUT  1000

/* 通讯板类型 */
byte_t  g_net_type = SRV2NET_NONE;

iSOCKET g_socket;

/*
---------------------------------------
    设置 SOCKET 虚表
---------------------------------------
*/
static void_t
socket_vtbl_init (void_t)
{
    switch (g_net_type)
    {
        default:
            mem_zero(&g_socket, sizeof(g_socket));
            break;

        case SRV2NET_SIM7100:
            sim7100_setup(&g_socket);
            break;

        case SRV2NET_SIM6320:
            sim6320_setup(&g_socket);
            break;

        case SRV2NET_SIM5360:
            sim5360_setup(&g_socket);
            break;
    }
}

/*
=======================================
    初始化网络库
=======================================
*/
CR_API bool_t
socket_init (void_t)
{
    ansi_t  *bak;
    ansi_t  buf[128];

    /* 判断是否有通讯板 */
    bridge_kill();
    g_net_type = SRV2NET_NONE;
    socket_vtbl_init();
    if (!at_check(AT_TIMEOUT)) {
        bridge_reset();
        thread_sleep(AT_TIMEOUT);
        if (!at_check(AT_TIMEOUT))
            return (FALSE);
    }

    /* 是否切换到桥接板 */
    if (!at_cgmm(buf, sizeof(buf), AT_TIMEOUT))
        return (FALSE);
    if (str_strA(buf, "RFGEO_LN-BRIDGE") != NULL) {
        bridge_init();
        thread_sleep(AT_TIMEOUT);

        /* 再次判断通讯板类型 */
        if (!at_check(AT_TIMEOUT))
            return (FALSE);
        if (!at_cgmm(buf, sizeof(buf), AT_TIMEOUT))
            return (FALSE);
    }

    /* SIMCOM 系列 */
    bak = str_strA(buf, "SIMCOM_");
    if (bak != NULL) {
        if (!simcom_socket_init())
            return (FALSE);
        if (str_strA(&bak[7], "SIM7100") != NULL) {
            g_net_type = SRV2NET_SIM7100;
            socket_vtbl_init();
            return (TRUE);
        }
        if (str_strA(&bak[7], "SIM6320") != NULL) {
            g_net_type = SRV2NET_SIM6320;
            socket_vtbl_init();
            return (TRUE);
        }
        if (str_strA(&bak[7], "SIM5360") != NULL) {
            g_net_type = SRV2NET_SIM5360;
            socket_vtbl_init();
            return (TRUE);
        }
        return (FALSE);
    }

    /* 未知模块 */
    return (FALSE);
}

/*
=======================================
    释放网络库
=======================================
*/
CR_API void_t
socket_free (void_t)
{
    /* SIMCOM 系列 */
    if (g_net_type == SRV2NET_SIM7100 ||
        g_net_type == SRV2NET_SIM6320 ||
        g_net_type == SRV2NET_SIM5360)
        simcom_socket_free();

    /* 清空虚表 */
    g_net_type = SRV2NET_NONE;
    socket_vtbl_init();
}

/*****************************************************************************/
/*                                   MISC                                    */
/*****************************************************************************/

/*
=======================================
    延时一段时间
=======================================
*/
CR_API void_t
thread_sleep (
  __CR_IN__ uint_t  time_ms
    )
{
    int32u  base = timer_get32();

    for (;;)
    {
        /* 等待时间到 */
        if (timer_delta32(base) >= time_ms)
            break;
        wdt_task();
    }
}

/*
=======================================
    加载串口库
=======================================
*/
CR_API void_t
sio_init (void_t)
{
}

/*
=======================================
    释放串口库
=======================================
*/
CR_API void_t
sio_free (void_t)
{
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
