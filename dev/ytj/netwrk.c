/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2017-03-08  */
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
/*  >>>>>>>>>>>>>>>>>>> YTJ 一体机 LED 屏网络驱动函数库 <<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

/* 函数名会冲突 */
#define mem_init    cr_mem_init
#define mem_malloc  cr_mem_malloc
#define mem_calloc  cr_mem_calloc
#define mem_free    cr_mem_free
#include "hash.h"
#include "applib.h"
#include "device.h"
#undef  mem_init
#undef  mem_malloc
#undef  mem_calloc
#undef  mem_free

#include "stm32_eth.h"
#include <lwip/init.h>
#include <lwip/dhcp.h>
#include <lwip/udp.h>
#include <lwip/tcp.h>
#include <lwip/igmp.h>
#include <lwip/priv/tcp_priv.h>
#include <netif/etharp.h>
#include "ethernetif.h"

/* 网卡参数 */
static ip4_addr_t   s_ip;
static ip4_addr_t   s_msk;
static ip4_addr_t   s_gw;

/* 任务用的计数器 */
static int32u   s_base_tcp = 0;
static int32u   s_base_arp = 0;
static int32u   s_base_igmp = 0;
static int32u   s_base_dhcp = 0;
static int32u   s_base_dhcp_do = 0;
static int32u   s_base_dhcp_coarse = 0;

/* 网卡上下文 */
struct netif    g_netif;

/* DHCP 启用标志 */
static retc_t   s_is_dhcp;

/* DHCP 相关参数 */
#define DHCP_MAX_TRIES      8
#define DHCP_TIMER_MSECS    250

/*
=======================================
    初始化网络
=======================================
*/
CR_API void_t
netwrk_init (
  __CR_IN__ const void_t*   cfg
    )
{
    uint_t  idx;
    int16u  hash;
    int32u  cpuid[3];
    byte_t  mac[6], *ptr;

    /* 初始化 */
    eth0_init();
    lwip_init();

    /* 参数为空表示 DHCP */
    if (cfg == NULL) {
        s_ip.addr = 0;
        s_msk.addr = 0;
        s_gw.addr = 0;
        s_is_dhcp = TRUE;
    }
    else {
        mem_cpy(cpuid, cfg, sizeof(cpuid));
        s_ip.addr = cpuid[0];
        s_msk.addr = cpuid[1];
        s_gw.addr = cpuid[2];
        s_is_dhcp = FALSE;
    }

    /* MAC 地址设置 */
    cpuid[0] = *(volatile int32u*)(0x1FFFF7E8UL);
    cpuid[1] = *(volatile int32u*)(0x1FFFF7ECUL);
    cpuid[2] = *(volatile int32u*)(0x1FFFF7F0UL);
    hash = hash_crc16h_total(cpuid, sizeof(cpuid));
    mac[0] = 0x00;
    mac[1] = 0x04;
    mac[2] = 0xA5;
    mac[3] = (byte_t)(hash >> 8);
    mac[4] = (byte_t)(hash);
    mac[5] = 0x00;
    ptr = (byte_t*)cpuid;
    for (idx = 0; idx < sizeof(cpuid); idx++)
        mac[5] ^= ptr[idx];
    Set_MAC_Address(mac);

    /* 注册并启动网卡 */
    netif_add(&g_netif, &s_ip, &s_msk, &s_gw, NULL,
        &ethernetif_init, &ethernet_input);
    netif_set_default(&g_netif);
    netif_set_up(&g_netif);

    /* 计数值置值 */
    s_base_tcp = timer_get32();
    s_base_arp = s_base_tcp;
    s_base_igmp = s_base_tcp;
    s_base_dhcp = s_base_tcp;
    s_base_dhcp_do = s_base_tcp;
    s_base_dhcp_coarse = s_base_tcp;
}

/*
=======================================
    网络任务处理
=======================================
*/
CR_API void_t
netwrk_func (void_t)
{
    /* TCP */
    if (timer_delta32(s_base_tcp) >= TCP_TMR_INTERVAL) {
        s_base_tcp = timer_get32();
        tcp_tmr();
    }

    /* ARP */
    if (timer_delta32(s_base_arp) >= ARP_TMR_INTERVAL) {
        s_base_arp = timer_get32();
        etharp_tmr();
    }

    /* IGMP */
    if (timer_delta32(s_base_igmp) >= IGMP_TMR_INTERVAL) {
        s_base_igmp = timer_get32();
        igmp_tmr();
    }

    /* DHCP */
    if (!s_is_dhcp || s_ip.addr != 0)
        return;
    if (timer_delta32(s_base_dhcp) >= DHCP_FINE_TIMER_MSECS) {
        s_base_dhcp = timer_get32();
        dhcp_fine_tmr();
    }
    if (timer_delta32(s_base_dhcp_coarse) >= DHCP_COARSE_TIMER_MSECS) {
        s_base_dhcp_coarse = timer_get32();
        dhcp_coarse_tmr();
    }
}

/*
=======================================
    判断网络是否上线
=======================================
*/
CR_API bool_t
netwrk_online (
  __CR_OT__ byte_t* ip,
  __CR_OT__ uint_t* type
    )
{
    static bool_t   linked = FALSE;

    /* 没有 IP 地址表示在 DHCP 中 */
    if (s_ip.addr == 0)
        *type = DHCP_STT_RUNNING;
    else
        *type = DHCP_STT_NONE;

    /* 断线后要清 IP 重新获取 */
    if (!eth0_linked()) {
        linked = FALSE;
        return (FALSE);
    }
    if (!linked) {
        linked = TRUE;
        if (s_is_dhcp) {
            s_ip.addr = 0;
            s_msk.addr = 0;
            s_gw.addr = 0;
            netif_set_addr(&g_netif, &s_ip, &s_msk, &s_gw);
            netif_set_up(&g_netif);
            dhcp_start(&g_netif);
        }
        else {
            *type = DHCP_STT_NSTATIC;
        }
    }

    /* 处理 DHCP 过程 */
    if (s_is_dhcp) {
        if (timer_delta32(s_base_dhcp_do) >= DHCP_TIMER_MSECS) {
            s_base_dhcp_do = timer_get32();
            if (s_ip.addr != g_netif.ip_addr.addr)
            {
                /* 获取到了新的 IP 地址 */
                s_ip.addr = g_netif.ip_addr.addr;
                s_msk.addr = g_netif.netmask.addr;
                s_gw.addr = g_netif.gw.addr;
                *type = DHCP_STT_ALLDONE;
            }
            else
            if (s_ip.addr == 0)
            {
                /* 重试几次不成功后转默认的静态 IP 地址 */
                if (netif_dhcp_data(&g_netif)->tries > DHCP_MAX_TRIES) {
                    dhcp_stop(&g_netif);
                    IP4_ADDR(&s_ip, 192, 168, 10, 111);
                    IP4_ADDR(&s_msk, 255, 255, 255, 0);
                    IP4_ADDR(&s_gw, 192, 168, 10, 254);
                    netif_set_addr(&g_netif, &s_ip, &s_msk, &s_gw);
                    s_is_dhcp = FALSE;
                    *type = DHCP_STT_NSTATIC;
                }
            }
        }
    }

    /* 返回当前 IP 地址 */
    mem_cpy(ip, &s_ip.addr, sizeof(int32u));
    return (TRUE);
}

/*
=======================================
    LwIP 计时函数
=======================================
*/
CR_API int32u
sys_now (void_t)
{
    return (timer_get32());
}

/*
=======================================
    ETH 中断处理
=======================================
*/
CR_API void_t
ETH_IRQHandler (void_t)
{
    while (ETH_GetRxPktSize() != 0)
        ethernetif_input(&g_netif);
    ETH_DMAClearITPendingBit(ETH_DMA_IT_R);
    ETH_DMAClearITPendingBit(ETH_DMA_IT_NIS);
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
