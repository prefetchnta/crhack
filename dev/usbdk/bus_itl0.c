/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2012-02-01  */
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
/*  >>>>>>>>>>>>>>>>>>>>>> USBDK U盘 ITL0 接口函数库 <<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#define _USBDK_ITL0_
#include "board.h"
#include "stm32f10x_conf.h"

/* 三个字节的 ECC 值 */
byte_t  g_nand_ecc[3];

/*
=======================================
    ITL0 接口初始化
=======================================
*/
CR_API void_t
ibus_init (void_t)
{
    GPIO_InitTypeDef                    gpio;
    FSMC_NANDInitTypeDef                fsmc;
    FSMC_NAND_PCCARDTimingInitTypeDef   nand;

    /* FSMC 时钟开启 */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);

    /* GPIO 管脚配置 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB |
                           RCC_APB2Periph_GPIOD |
                           RCC_APB2Periph_GPIOE |
                           RCC_APB2Periph_GPIOF |
                           RCC_APB2Periph_GPIOG, ENABLE);

    /* 管脚 CLE, ALE, D0->D3, NOE, NWE & NCE2 配置 */
    gpio.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_14 |
                    GPIO_Pin_15 | GPIO_Pin_0  | GPIO_Pin_1  |
                    GPIO_Pin_4  | GPIO_Pin_5  | GPIO_Pin_7;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOD, &gpio);

    /* 管脚 D4->D7 配置 */
    gpio.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_Init(GPIOE, &gpio);

    /* 管脚 NWAIT 配置 */
    gpio.GPIO_Pin = GPIO_Pin_6;
    gpio.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOD, &gpio);

    /* 管脚 INT2 配置 */
    gpio.GPIO_Pin = GPIO_Pin_6;
    GPIO_Init(GPIOG, &gpio);

    /* FSMC 设备配置 */
    nand.FSMC_SetupTime = 0x1;
    nand.FSMC_WaitSetupTime = 0x3;
    nand.FSMC_HoldSetupTime = 0x2;
    nand.FSMC_HiZSetupTime = 0x1;
    fsmc.FSMC_Bank = FSMC_Bank2_NAND;
    fsmc.FSMC_Waitfeature = FSMC_Waitfeature_Enable;
    fsmc.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_8b;
    fsmc.FSMC_ECC = FSMC_ECC_Enable;
    fsmc.FSMC_ECCPageSize = FSMC_ECCPageSize_512Bytes;
    fsmc.FSMC_TCLRSetupTime = 0x00;
    fsmc.FSMC_TARSetupTime = 0x00;
    fsmc.FSMC_CommonSpaceTimingStruct = &nand;
    fsmc.FSMC_AttributeSpaceTimingStruct = &nand;
    FSMC_NANDInit(&fsmc);

    /* NAND 设备启动 */
    FSMC_NANDCmd(FSMC_Bank2_NAND, ENABLE);

    /* 关闭写保护管脚 */
    gpio.GPIO_Pin = GPIO_Pin_5;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &gpio);
    GPIO_SetBits(GPIOB, GPIO_Pin_5);
}

/*
=======================================
    ITL0 硬件 ECC 计算开始
=======================================
*/
CR_API void_t
ibus_ecc_start (
  __CR_IN__ ufast_t size
    )
{
    FSMC_NANDECCCmd(FSMC_Bank2_NAND, DISABLE);
    if (size == 512 + 16)
        FSMC_NANDECCCmd(FSMC_Bank2_NAND, ENABLE);
}

/*
=======================================
    ITL0 硬件 ECC 结束检查
=======================================
*/
CR_API void_t
ibus_ecc_check (void_t)
{
    int32u  ecc;

    ecc = (int32u)FSMC_GetECC(FSMC_Bank2_NAND);
    ecc = ~ecc;
    g_nand_ecc[0] = (byte_t)(ecc >>  0);
    g_nand_ecc[1] = (byte_t)(ecc >>  8);
    g_nand_ecc[2] = (byte_t)(ecc >> 16);
    FSMC_NANDECCCmd(FSMC_Bank2_NAND, DISABLE);
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
