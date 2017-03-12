/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2017-03-12  */
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
/*  >>>>>>>>>>>>>>>>> RFGEO-SRV-2 采集器 SPI1 接口函数库 <<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#define _SRV2_SPI1_
#include "board.h"
#include "stm32f10x_conf.h"

/*
=======================================
    SPI1 初始化
=======================================
*/
CR_API void_t
spi1_init (void_t)
{
    SPI_InitTypeDef     sspi;
    GPIO_InitTypeDef    gpio;

    /* SPI 端口配置 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
                           RCC_APB2Periph_GPIOB |
                           RCC_APB2Periph_GPIOC |
                           RCC_APB2Periph_SPI1, ENABLE);
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    gpio.GPIO_Pin = GPIO_Pin_6;
    GPIO_Init(GPIOA, &gpio);
    gpio.GPIO_Mode = GPIO_Mode_AF_PP;
    gpio.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
    GPIO_Init(GPIOA, &gpio);
    gpio.GPIO_Speed = GPIO_Speed_10MHz;
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Pin = GPIO_Pin_4;
    GPIO_Init(GPIOA, &gpio);
    GPIO_SetBits(GPIOA, GPIO_Pin_4);

    /* 模块控制引脚 */
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_Init(GPIOB, &gpio);
    GPIO_SetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_1);
    gpio.GPIO_Pin = GPIO_Pin_5;
    GPIO_Init(GPIOC, &gpio);
    GPIO_ResetBits(GPIOC, GPIO_Pin_5);
    gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    gpio.GPIO_Pin = GPIO_Pin_4;
    GPIO_Init(GPIOC, &gpio);

    /* SPI 外设配置 */
    sspi.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    sspi.SPI_Mode = SPI_Mode_Master;
    sspi.SPI_DataSize = SPI_DataSize_8b;
    sspi.SPI_CPOL = SPI_CPOL_Low;
    sspi.SPI_CPHA = SPI_CPHA_1Edge;
    sspi.SPI_NSS = SPI_NSS_Soft;
    sspi.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
    sspi.SPI_FirstBit = SPI_FirstBit_MSB;
    sspi.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &sspi);
    SPI_Cmd(SPI1, ENABLE);
}

/*
---------------------------------------
    SPI1 收发字节
---------------------------------------
*/
static byte_t
spi1_iorw (
  __CR_IN__ byte_t  ch
    )
{
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, ch);
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
    return ((byte_t)SPI_I2S_ReceiveData(SPI1));
}

/*
=======================================
    SPI1 发送数据到从机
=======================================
*/
CR_API void_t
spi1_send_data (
  __CR_IN__ const void_t*   data,
  __CR_IN__ leng_t          size
    )
{
    GPIO_ResetBits(GPIOA, GPIO_Pin_4);
    for (; size != 0; size--) {
        spi1_iorw(*(byte_t*)data);
        data = (byte_t*)data + 1;
    }
    GPIO_SetBits(GPIOA, GPIO_Pin_4);
}

/*
=======================================
    SPI1 发送后接收数据
=======================================
*/
CR_API void_t
spi1_send_recv (
  __CR_OT__ void_t*         recv_data,
  __CR_IN__ leng_t          recv_size,
  __CR_IN__ const void_t*   send_data,
  __CR_IN__ leng_t          send_size
    )
{
    GPIO_ResetBits(GPIOA, GPIO_Pin_4);
    for (; send_size != 0; send_size--) {
        spi1_iorw(*(byte_t*)send_data);
        send_data = (byte_t*)send_data + 1;
    }
    for (; recv_size != 0; recv_size--) {
        *(byte_t*)recv_data = spi1_iorw(0xFF);
        recv_data = (byte_t*)recv_data + 1;
    }
    GPIO_SetBits(GPIOA, GPIO_Pin_4);
}

/*
=======================================
    SPI1 发送数据到从机
=======================================
*/
CR_API void_t
spi1_send_send (
  __CR_IN__ const void_t*   send1_data,
  __CR_IN__ leng_t          send1_size,
  __CR_IN__ const void_t*   send2_data,
  __CR_IN__ leng_t          send2_size
    )
{
    GPIO_ResetBits(GPIOA, GPIO_Pin_4);
    for (; send1_size != 0; send1_size--) {
        spi1_iorw(*(byte_t*)send1_data);
        send1_data = (byte_t*)send1_data + 1;
    }
    for (; send2_size != 0; send2_size--) {
        spi1_iorw(*(byte_t*)send2_data);
        send2_data = (byte_t*)send2_data + 1;
    }
    GPIO_SetBits(GPIOA, GPIO_Pin_4);
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
