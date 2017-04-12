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
/*  >>>>>>>>>>>>>>>>>> YTJ 一体机 LED 屏 ETH0 接口函数库 <<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "board.h"
#include "stm32_eth.h"
#include "stm32f10x_conf.h"

/* PHY 地址 */
#if defined(YTJ_NEW)
    #define YTJ_DP83848
    #define PHY_ADDRESS     0x01    /* DP83848I */
#else
    #define YTJ_RTL8201
    #define PHY_ADDRESS     0x1C    /* RTL8201CP */
#endif

/* PHY 寄存器 */
#define PHY_BCR     0x00
#define PHY_BSR     0x01
#if defined(YTJ_DP83848)
    #define PHY_SR  0x10
#else
    #define PHY_SR  0x19
#endif

/* PHY_BCR 标志位 */
#define PHY_RESET               0x8000
#define PHY_FULLDUPLEX_100M     0x2100
#define PHY_HALFDUPLEX_100M     0x2000
#define PHY_FULLDUPLEX_10M      0x0100
#define PHY_HALFDUPLEX_10M      0x0000
#define PHY_AUTONEGOTIATION     0x1000

/* PHY_BSR 标志位 */
#define PHY_AUTONEGO_COMPLETE   0x0020
#define PHY_LINKED_STATUS       0x0004

/* PHY_SR 标志位 */
#if defined(YTJ_DP83848)
    #define PHY_SPEED_STATUS    0x0002
    #define PHY_DUPLEX_STATUS   0x0004
    #define is_speed_100m(reg)  !((reg) & PHY_SPEED_STATUS)
    #define is_fullduplex(reg)  ((reg) & PHY_DUPLEX_STATUS)
#else
    #define PHY_SPEED_100M      0x0001
    #define is_speed_100m(reg)  ((reg) & PHY_SPEED_100M)
    #define is_fullduplex(reg)  (TRUE)
#endif

/*
=======================================
    PHY 初始化
=======================================
*/
CR_API uint8_t
phy_init (
  __CR_IO__ ETH_InitTypeDef*    seth,
  __CR_IN__ uint16_t            address
    )
{
    uint16_t    tmp;
    uint32_t    reg;

    /* PHY 芯片复位 */
    if (!ETH_WritePHYRegister(address, PHY_BCR, PHY_RESET))
        return (FALSE);
    ETH_Delay(PHY_ResetDelay);

    /* 自适应模式 */
    if (seth->ETH_AutoNegotiation != ETH_AutoNegotiation_Disable)
    {
        /* 等到网线插上为止 */
        for (;;) {
            if (ETH_ReadPHYRegister(address, PHY_BSR) & PHY_LINKED_STATUS)
                break;
            wdt_task();
        }

        /* 开启自适应模式 */
        if (!ETH_WritePHYRegister(address, PHY_BCR, PHY_AUTONEGOTIATION))
            return (FALSE);

        /* 等到模式确定为止 */
        for (;;) {
            if (ETH_ReadPHYRegister(address, PHY_BSR) & PHY_AUTONEGO_COMPLETE)
                break;
            wdt_task();
        }

        /* 读取网络的模式 */
        reg = ETH_ReadPHYRegister(address, PHY_SR);
        if (is_fullduplex(reg))
            seth->ETH_Mode = ETH_Mode_FullDuplex;
        else
            seth->ETH_Mode = ETH_Mode_HalfDuplex;
        if (is_speed_100m(reg))
            seth->ETH_Speed = ETH_Speed_100M;
        else
            seth->ETH_Speed = ETH_Speed_10M;
    }
    else
    {
        /* 直接写 PHY 寄存器 */
        if (seth->ETH_Mode == ETH_Mode_FullDuplex) {
            if (seth->ETH_Speed == ETH_Speed_100M)
                tmp = PHY_FULLDUPLEX_100M;
            else
                tmp = PHY_FULLDUPLEX_10M;
        }
        else {
            if (seth->ETH_Speed == ETH_Speed_100M)
                tmp = PHY_HALFDUPLEX_100M;
            else
                tmp = PHY_HALFDUPLEX_10M;
        }
        if (!ETH_WritePHYRegister(address, PHY_BCR, tmp))
            return (FALSE);
        ETH_Delay(PHY_ConfigDelay);
    }
    return (TRUE);
}

/*
=======================================
    ETH0 初始化
=======================================
*/
CR_API void_t
eth0_init (void_t)
{
    ETH_InitTypeDef     seth;
    GPIO_InitTypeDef    gpio;

    /* ETH_MII 管脚配置 */
    /* PA8/MCO 和 PB5/ETH_PPS_OUT 没用 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
                           RCC_APB2Periph_GPIOB |
                           RCC_APB2Periph_GPIOC |
                           RCC_APB2Periph_GPIOD |
                           RCC_APB2Periph_AFIO, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ETH_MAC |
                          RCC_AHBPeriph_ETH_MAC_Tx |
                          RCC_AHBPeriph_ETH_MAC_Rx, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_ETH, ENABLE);
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_Mode = GPIO_Mode_AF_PP;
    gpio.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOA, &gpio);
    gpio.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
    GPIO_Init(GPIOB, &gpio);
    gpio.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_Init(GPIOC, &gpio);
    gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3;
    GPIO_Init(GPIOA, &gpio);
    gpio.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOB, &gpio);
    gpio.GPIO_Pin = GPIO_Pin_3;
    GPIO_Init(GPIOC, &gpio);
    gpio.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 |
                    GPIO_Pin_12;
    GPIO_Init(GPIOD, &gpio);

    /* ETH 外设配置 */
    GPIO_ETH_MediaInterfaceConfig(GPIO_ETH_MediaInterface_MII);
    ETH_DeInit();
    ETH_SoftwareReset();
    while (ETH_GetSoftwareResetStatus() == SET);
    ETH_StructInit(&seth);
    seth.ETH_AutoNegotiation = ETH_AutoNegotiation_Enable;
    seth.ETH_LoopbackMode = ETH_LoopbackMode_Disable;
    seth.ETH_RetryTransmission = ETH_RetryTransmission_Disable;
    seth.ETH_AutomaticPadCRCStrip = ETH_AutomaticPadCRCStrip_Disable;
    seth.ETH_ReceiveAll = ETH_ReceiveAll_Disable;
    seth.ETH_BroadcastFramesReception = ETH_BroadcastFramesReception_Enable;
    seth.ETH_PromiscuousMode = ETH_PromiscuousMode_Disable;
    seth.ETH_MulticastFramesFilter = ETH_MulticastFramesFilter_Perfect;
    seth.ETH_UnicastFramesFilter = ETH_UnicastFramesFilter_Perfect;
    seth.ETH_ChecksumOffload = ETH_ChecksumOffload_Enable;
    seth.ETH_DropTCPIPChecksumErrorFrame = ETH_DropTCPIPChecksumErrorFrame_Enable;
    seth.ETH_ReceiveStoreForward = ETH_ReceiveStoreForward_Enable;
    seth.ETH_TransmitStoreForward = ETH_TransmitStoreForward_Enable;
    seth.ETH_ForwardErrorFrames = ETH_ForwardErrorFrames_Disable;
    seth.ETH_ForwardUndersizedGoodFrames = ETH_ForwardUndersizedGoodFrames_Disable;
    seth.ETH_SecondFrameOperate = ETH_SecondFrameOperate_Enable;
    seth.ETH_AddressAlignedBeats = ETH_AddressAlignedBeats_Enable;
    seth.ETH_FixedBurst = ETH_FixedBurst_Enable;
    seth.ETH_RxDMABurstLength = ETH_RxDMABurstLength_32Beat;
    seth.ETH_TxDMABurstLength = ETH_TxDMABurstLength_32Beat;
    seth.ETH_DMAArbitration = ETH_DMAArbitration_RoundRobin_RxTx_2_1;
    ETH_Init(&seth, PHY_ADDRESS);
    ETH_DMAITConfig(ETH_DMA_IT_NIS | ETH_DMA_IT_R, ENABLE);
}

/*
=======================================
    ETH0 网线是否插上
=======================================
*/
CR_API retc_t
eth0_linked (void_t)
{
    if (ETH_ReadPHYRegister(PHY_ADDRESS, PHY_BSR) & PHY_LINKED_STATUS)
        return (TRUE);
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
