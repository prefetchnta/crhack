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
    #define PHY_ADDRESS     0x01    /* DP83848I */
#else
    #define PHY_ADDRESS     0x1C    /* RTL8201CP */
#endif

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
    seth.ETH_AutoNegotiation = ETH_AutoNegotiation_Disable;
    seth.ETH_Speed = ETH_Speed_100M;
    seth.ETH_LoopbackMode = ETH_LoopbackMode_Disable;
    seth.ETH_RetryTransmission = ETH_RetryTransmission_Disable;
    seth.ETH_AutomaticPadCRCStrip = ETH_AutomaticPadCRCStrip_Disable;
    seth.ETH_ReceiveAll = ETH_ReceiveAll_Enable;
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

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
