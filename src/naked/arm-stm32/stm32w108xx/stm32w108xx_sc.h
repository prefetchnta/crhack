/**
  ******************************************************************************
  * @file    stm32w108xx_sc.h
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    30-November-2012
  * @brief   This file contains all the functions prototypes for the serial controller  
  *          firmware library.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32W108XX_SC_H
#define __STM32W108XX_SC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32w108xx.h"

/** @addtogroup STM32W108xx_StdPeriph_Driver
  * @{
  */

/** @addtogroup SERIAL_CONTROLLER
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/** 
  * @brief  SPI Init structure definition  
  */
typedef struct
{

  uint16_t SPI_Mode;                /*!< Specifies the SPI mode (Master/Slave).
                                         This parameter can be a value of @ref SPI_mode */

  uint16_t SPI_CPOL;                /*!< Specifies the serial clock steady state.
                                         This parameter can be a value of @ref SPI_Clock_Polarity */

  uint16_t SPI_CPHA;                /*!< Specifies the clock active edge for the bit capture.
                                         This parameter can be a value of @ref SPI_Clock_Phase */

  uint32_t SPI_ClockRate;          /*!< This member configures the SPI communication clock rate.
                                           The clock rate is computed using the following formula:
                                            - clock  rate = 12MHz/((LIN+1)*(2^EXP)*/

  uint16_t SPI_FirstBit;            /*!< Specifies whether data transfers start from MSB or LSB bit.
                                         This parameter can be a value of @ref SPI_MSB_LSB_transmission */
}SPI_InitTypeDef;

/** 
  * @brief  UART Init Structure definition  
  */
typedef struct
{
  uint32_t UART_BaudRate;            /*!< This member configures the UART communication baud rate.
                                           The baud rate is computed using the following formula:
                                            - Baudrate = 24MHz/(2*N+F)*/

  uint32_t UART_WordLength;          /*!< Specifies the number of data bits transmitted or received in a frame.
                                           This parameter can be a value of @ref UART_Word_Length */

  uint32_t UART_StopBits;            /*!< Specifies the number of stop bits transmitted.
                                           This parameter can be a value of @ref UART_Stop_Bits */

  uint32_t UART_Parity;              /*!< Specifies the parity mode.
                                           @note When parity is enabled, the computed parity is inserted
                                                 at the MSB position of the transmitted data (9th bit when
                                                 the word length is set to 9 data bits; 8th bit when the
                                                 word length is set to 8 data bits)
                                           This parameter can be a value of @ref UART_Parity */


  uint32_t UART_HardwareFlowControl; /*!< Specifies wether the hardware flow control mode is enabled or disabled.
                                           This parameter can be a value of @ref UART_Hardware_Flow_Control */
}UART_InitTypeDef;


/** 
  * @brief  I2C Init structure definition  
  */
typedef struct
{
  uint32_t I2C_ClockRate;          /*!< This member configures the I2C communication clock rate.
                                          The clock rate is computed using the following formula:
                                            - clock  rate = 12MHz/((LIN+1)*(2^EXP)
                                          This parameter must be set to a value lower than 400kHz */

}I2C_InitTypeDef;

/** 
  * @brief  DMA Init structure definition  
  */
typedef struct
{
  uint32_t DMA_BeginAddrA;         /*!< Specifies the peripheral begin address A for the selected
                                        DMA_channel */
  uint32_t DMA_EndAddrA;           /*!< Specifies the peripheral end address A for the selected
                                        DMA_channel */
  uint32_t DMA_BeginAddrB;         /*!< Specifies the peripheral begin address B for the selected
                                        DMA_channel */
  uint32_t DMA_EndAddrB;           /*!< Specifies the peripheral end address B for the selected
                                        DMA_channel */
}SC_DMA_InitTypeDef;

/** @defgroup SC_Exported_Constants
  * @{
  */

#define IS_UART_PERIPH(PERIPH) (((PERIPH) == SC1_UART))

#define IS_SPI_PERIPH(PERIPH) (((PERIPH) == SC1_SPI) || \
                               ((PERIPH) == SC2_SPI))
#define IS_I2C_PERIPH(PERIPH) (((PERIPH) == SC1_I2C) || \
                               ((PERIPH) == SC2_I2C))
#define IS_DMA_PERIPH(PERIPH) (((PERIPH) == SC1_DMA) || \
                               ((PERIPH) == SC2_DMA))

#define IS_I2C_SPI_DMA_IT_PERIPH(PERIPH) (((PERIPH) == SC1_IT) || \
                                          ((PERIPH) == SC2_IT))

#define IS_UART_IT_PERIPH(PERIPH) (((PERIPH) == SC1_IT))

/** @defgroup SC_mode 
  * @{
  */

#define SC_Mode_Disable              ((uint32_t)0x00000000)
#define SC_Mode_UART                 ((uint32_t)0x00000001)
#define SC_Mode_SPI                  ((uint32_t)0x00000002)
#define SC_Mode_I2C                  ((uint32_t)0x00000003)

#define IS_SC_MODE(MODE) (((MODE) == SC_Mode_Disable) || ((MODE) == SC_Mode_SPI) || \
                          ((MODE) == SC_Mode_I2C) || ((MODE) == SC_Mode_UART))

/**
  * @}
  */

/** @defgroup Trigger_Event 
  * @{
  */

#define SC_TriggerEvent_Edge         ((uint32_t)0x00000000)
#define SC_TriggerEvent_Level        ((uint32_t)0x00000001)

#define IS_SC_TRIGGEREVENT(EVENT) (((EVENT) == SC_TriggerEvent_Edge) || ((EVENT) == SC_TriggerEvent_Level))

/**
  * @}
  */
  
/** @defgroup SC_SPI_Exported_Constants
  * @{
  */

/** @defgroup SPI_mode 
  * @{
  */

#define SPI_Mode_Slave                  ((uint32_t)0x00000000)
#define SPI_Mode_Master                 ((uint32_t)SC_SPICR_MSTR)
#define IS_SPI_MODE(MODE) (((MODE) == SPI_Mode_Master) || \
                           ((MODE) == SPI_Mode_Slave))
/**
  * @}
  */


/** @defgroup SPI_Clock_Polarity 
  * @{
  */

#define SPI_CPOL_Low                    ((uint32_t)0x00000000)
#define SPI_CPOL_High                   ((uint32_t)SC_SPICR_CPOL)
#define IS_SPI_CPOL(CPOL) (((CPOL) == SPI_CPOL_Low) || \
                           ((CPOL) == SPI_CPOL_High))
/**
  * @}
  */

/** @defgroup SPI_Clock_Phase 
  * @{
  */

#define SPI_CPHA_1Edge                  ((uint32_t)0x00000000)
#define SPI_CPHA_2Edge                  ((uint32_t)SC_SPICR_CPHA)
#define IS_SPI_CPHA(CPHA) (((CPHA) == SPI_CPHA_1Edge) || \
                           ((CPHA) == SPI_CPHA_2Edge))
/**
  * @}
  */

/** @defgroup SPI_MSB_LSB_transmission 
  * @{
  */

#define SPI_FirstBit_MSB                ((uint32_t)0x00000000)
#define SPI_FirstBit_LSB                ((uint32_t)SC_SPICR_LSBFIRST)
#define IS_SPI_FIRST_BIT(BIT) (((BIT) == SPI_FirstBit_MSB) || \
                               ((BIT) == SPI_FirstBit_LSB))
/**
  * @}
  */

/** @defgroup SPI_Receiver_driven_Mode 
  * @{
  */ 
  
#define SPI_ReceiverMode_TxDataReady              ((uint32_t)0x00000000)
#define SPI_ReceiverMode_RxFIFOFree               ((uint32_t)SC_SPICR_RXMODE)
#define IS_SPI_RECEIVER_DRIVEN_MODE(MODE) (((MODE) == SPI_ReceiverMode_TxDataReady) || \
                                           ((MODE) == SPI_ReceiverMode_RxFIFOFree))
/**
  * @}
  */

/** @defgroup SPI_flags_definition 
  * @{
  */

#define SPI_FLAG_OVR               ((uint32_t)SC_SPISR_OVR)
#define SPI_FLAG_TXE               ((uint32_t)SC_SPISR_TXE)
#define SPI_FLAG_RXNE              ((uint32_t)SC_SPISR_RXNE)
#define SPI_FLAG_IDLE              ((uint32_t)SC_SPISR_IDLE)
#define IS_SPI_GET_FLAG(FLAG) (((FLAG) == SPI_FLAG_IDLE) || ((FLAG) == SPI_FLAG_RXNE) || \
                               ((FLAG) == SPI_FLAG_TXE) || ((FLAG) == SPI_FLAG_OVR))
/**
  * @}
  */

/** @defgroup SPI_interrupts_definition 
  * @{
  */
#define SPI_IT_UND                    ((uint32_t)SC_ISR_UND)
#define SPI_IT_OVR                    ((uint32_t)SC_ISR_OVR)
#define SPI_IT_IDLE                   ((uint32_t)SC_ISR_IDLE)
#define SPI_IT_TXE                    ((uint32_t)SC_ISR_TXE)
#define SPI_IT_RXNE                   ((uint32_t)SC_ISR_RXNE)
#define IS_SPI_IT(IT) (((IT) == SPI_IT_UND) || ((IT) == SPI_IT_OVR) || \
                       ((IT) == SPI_IT_IDLE) || ((IT) == SPI_IT_TXE) || \
                       ((IT) == SPI_IT_RXNE))
#define IS_SPI_TRIGGEREVENT_IT(IT) (((IT) == SPI_IT_IDLE) || ((IT) == SPI_IT_TXE) || \
                                    ((IT) == SPI_IT_RXNE))
/**
  * @}
  */


#define IS_SPI_CLOCK_RATE(RATE) (((RATE) > 0) && ((RATE) <= 6000000))

/**
  * @}
  */

/** @defgroup SC_UART_Exported_Constants
  * @{
  */

/** @defgroup UART_Word_Length 
  * @{
  */ 

#define UART_WordLength_7b                 ((uint32_t)0x00000000)
#define UART_WordLength_8b                 ((uint32_t)SC_UARTCR_M)
#define IS_UART_WORD_LENGTH(LENGTH) (((LENGTH) == UART_WordLength_7b) || \
                                     ((LENGTH) == UART_WordLength_8b))
/**
  * @}
  */ 

/** @defgroup UART_Stop_Bits 
  * @{
  */ 

#define UART_StopBits_1                     ((uint32_t)0x00000000)
#define UART_StopBits_2                     ((uint32_t)SC_UARTCR_STOP)

#define IS_UART_STOPBITS(STOPBITS) (((STOPBITS) == UART_StopBits_1) || \
                                    ((STOPBITS) == UART_StopBits_2))
/**
  * @}
  */ 

/** @defgroup UART_Parity 
  * @{
  */ 

#define UART_Parity_No                      ((uint32_t)0x00000000)
#define UART_Parity_Even                    ((uint32_t)SC_UARTCR_PCE)
#define UART_Parity_Odd                     ((uint32_t)SC_UARTCR_PCE | SC_UARTCR_PS) 
#define IS_UART_PARITY(PARITY) (((PARITY) == UART_Parity_No) || \
                                ((PARITY) == UART_Parity_Even) || \
                                ((PARITY) == UART_Parity_Odd))
/**
  * @}
  */

/** @defgroup UART_Hardware_Flow_Control 
  * @{
  */ 

#define UART_HardwareFlowControl_Disable       ((uint32_t)0x00000000)
#define UART_HardwareFlowControl_Enable        ((uint32_t)0x00000020)
#define UART_HardwareFlowControl_Automatic     ((uint32_t)0x00000060)
#define IS_UART_HARDWARE_FLOW_CONTROL(CONTROL) (((CONTROL) == UART_HardwareFlowControl_Disable) || \
                                                ((CONTROL) == UART_HardwareFlowControl_Enable) || \
                                                ((CONTROL) == UART_HardwareFlowControl_Automatic))
/**
  * @}
  */ 



/** @defgroup UART_flags_definition 
  * @{
  */
#define UART_FLAG_CTS                ((uint32_t)SC_UARTSR_CTS)
#define UART_FLAG_RXNE               ((uint32_t)SC_UARTSR_RXNE)
#define UART_FLAG_TXE                ((uint32_t)SC_UARTSR_TXE)
#define UART_FLAG_OVR                ((uint32_t)SC_UARTSR_OVR)
#define UART_FLAG_FE                 ((uint32_t)SC_UARTSR_FE)
#define UART_FLAG_PE                 ((uint32_t)SC_UARTSR_PE)
#define UART_FLAG_IDLE               ((uint32_t)SC_UARTSR_IDLE)

#define IS_UART_GET_FLAG(FLAG) (((FLAG) == UART_FLAG_CTS) || ((FLAG) == UART_FLAG_RXNE) || \
                                ((FLAG) == UART_FLAG_TXE) || ((FLAG) == UART_FLAG_OVR) || \
                                ((FLAG) == UART_FLAG_FE) || ((FLAG) == UART_FLAG_PE)|| \
                                ((FLAG) == UART_FLAG_IDLE))
/**
  * @}
  */

/** @defgroup UART_interrupts_definition 
  * @{
  */

#define UART_IT_PE                     ((uint32_t)SC_ISR_PE)
#define UART_IT_FE                     ((uint32_t)SC_ISR_FE)
#define UART_IT_UND                    ((uint32_t)SC_ISR_UND)
#define UART_IT_OVR                    ((uint32_t)SC_ISR_OVR)
#define UART_IT_IDLE                   ((uint32_t)SC_ISR_IDLE)
#define UART_IT_TXE                    ((uint32_t)SC_ISR_TXE)
#define UART_IT_RXNE                   ((uint32_t)SC_ISR_RXNE)

#define IS_UART_IT(IT) (((IT) == UART_IT_PE) || ((IT) == UART_IT_FE) || \
                        ((IT) == UART_IT_UND) || ((IT) == UART_IT_OVR) || \
                        ((IT) == UART_IT_IDLE) || ((IT) == UART_IT_TXE) || \
                        ((IT) == UART_IT_RXNE))
#define IS_UART_TRIGGEREVENT_IT(IT) (((IT) == UART_IT_IDLE) || ((IT) == UART_IT_TXE) || \
                                     ((IT) == UART_IT_RXNE))
/**
  * @}
  */

#define IS_UART_BAUDRATE(BAUDRATE) (((BAUDRATE) > 0) && ((BAUDRATE) <= 1500000))

/**
  * @}
  */


/** @defgroup SC_I2C_Exported_Constants
  * @{
  */

/** @defgroup I2C_transfer_direction 
  * @{
  */

#define  I2C_Direction_Transmitter      ((uint8_t)0x00)
#define  I2C_Direction_Receiver         ((uint8_t)0x01)
#define IS_I2C_DIRECTION(DIRECTION) (((DIRECTION) == I2C_Direction_Transmitter) || \
                                     ((DIRECTION) == I2C_Direction_Receiver))
/**
  * @}
  */

/** @defgroup I2C_flags_definition 
  * @{
  */
#define I2C_FLAG_NACK                 ((uint32_t)SC_I2CSR_NACK)
#define I2C_FLAG_BTF                  ((uint32_t)SC_I2CSR_BTF)
#define I2C_FLAG_BRF                  ((uint32_t)SC_I2CSR_BRF)
#define I2C_FLAG_CMDFIN               ((uint32_t)SC_I2CSR_CMDFIN)

#define IS_I2C_GET_FLAG(FLAG) (((FLAG) == SC_I2CSR_NACK) || ((FLAG) == SC_I2CSR_BTF) || \
                               ((FLAG) == SC_I2CSR_BRF) || ((FLAG) == SC_I2CSR_CMDFIN))
/**
  * @}
  */

/** @defgroup I2C_interrupts_definition 
  * @{
  */
#define I2C_IT_NACK                    ((uint32_t)SC_ISR_NACK)
#define I2C_IT_CMDFIN                  ((uint32_t)SC_ISR_CMDFIN)
#define I2C_IT_BTF                     ((uint32_t)SC_ISR_BTF)
#define I2C_IT_BRF                     ((uint32_t)SC_ISR_BRF)

#define IS_I2C_IT(IT) (((IT) == I2C_IT_NACK) || ((IT) == I2C_IT_CMDFIN) || \
                       ((IT) == I2C_IT_BTF) || ((IT) == I2C_IT_BRF))
/**
  * @}
  */

#define IS_I2C_CLOCK_RATE(RATE) (((RATE) > 0) && ((RATE) <= 400000))

/**
  * @}
  */

/** @defgroup SC_DMA_Exported_Constants
  * @{
  */

/** @defgroup DMA_Channel_Load 
  * @{
  */

#define DMA_ChannelLoad_BTx                  ((uint32_t)SC_DMACR_TXLODB)
#define DMA_ChannelLoad_ATx                  ((uint32_t)SC_DMACR_TXLODA)
#define DMA_ChannelLoad_BRx                  ((uint32_t)SC_DMACR_RXLODB)
#define DMA_ChannelLoad_ARx                  ((uint32_t)SC_DMACR_RXLODA)

#define IS_DMA_CHANNEL_LOAD(CHANNEL) (((CHANNEL) == DMA_ChannelLoad_BTx) || ((CHANNEL) == DMA_ChannelLoad_ATx) || \
                                      ((CHANNEL) == DMA_ChannelLoad_BRx) || ((CHANNEL) == DMA_ChannelLoad_ARx))
/**
  * @}
  */

/** @defgroup DMA_Counter_Register 
  * @{
  */

#define DMA_Counter_RXCNTA                   ((uint32_t)0x00000000)
#define DMA_Counter_RXCNTB                   ((uint32_t)0x00000004)
#define DMA_Counter_TXCNT                    ((uint32_t)0x00000008)
#define DMA_Counter_RXCNTSAVED               ((uint32_t)0x00000050)

#define IS_DMA_COUNTER(COUNTER) (((COUNTER) == DMA_Counter_RXCNTA) || ((COUNTER) == DMA_Counter_RXCNTB) || \
                                 ((COUNTER) == DMA_Counter_TXCNT) || ((COUNTER) == DMA_Counter_RXCNTSAVED))

/**
  * @}
  */

/** @defgroup DMA_Receiver_Error_Register 
  * @{
  */

#define DMA_ReceiverError_CNTA                   ((uint32_t)0x00000014)
#define DMA_ReceiverError_CNTB                   ((uint32_t)0x00000018)


#define IS_DMA_RECEIVER_ERROR(REGISTER) (((REGISTER) == DMA_ReceiverError_CNTA) || ((REGISTER) == DMA_ReceiverError_CNTB))

/**
  * @}
  */

/** @defgroup DMA_Channel_Reset 
  * @{
  */

#define DMA_ChannelReset_Tx                   ((uint32_t)SC_DMACR_TXRST)
#define DMA_ChannelReset_Rx                   ((uint32_t)SC_DMACR_RXRST)

#define IS_DMA_CHANNEL_RESET(CHANNEL) (((CHANNEL) == DMA_ChannelReset_Tx) || ((CHANNEL) == DMA_ChannelReset_Rx))

/**
  * @}
  */

/** @defgroup DMA_flags_definition 
  * @{
  */
#define DMA_FLAG_RXAACK                  ((uint32_t)SC_DMASR_RXAACK)
#define DMA_FLAG_RXBACK                  ((uint32_t)SC_DMASR_RXBACK)
#define DMA_FLAG_TXAACK                  ((uint32_t)SC_DMASR_TXAACK)
#define DMA_FLAG_TXBACK                  ((uint32_t)SC_DMASR_TXBACK)
#define DMA_FLAG_OVRA                    ((uint32_t)SC_DMASR_OVRA)
#define DMA_FLAG_OVRB                    ((uint32_t)SC_DMASR_OVRB)
#define DMA_FLAG_PEA                     ((uint32_t)SC_DMASR_PEA)
#define DMA_FLAG_PEB                     ((uint32_t)SC_DMASR_PEB)
#define DMA_FLAG_FEA                     ((uint32_t)SC_DMASR_FEA)
#define DMA_FLAG_FEB                     ((uint32_t)SC_DMASR_FEB)
#define DMA_FLAG_NSSS                    ((uint32_t)SC_DMASR_NSSS)

#define IS_DMA_FLAG(FLAG) (((FLAG) == DMA_FLAG_RXAACK) || ((FLAG) == DMA_FLAG_RXBACK) || \
                           ((FLAG) == DMA_FLAG_TXAACK) || ((FLAG) == DMA_FLAG_TXBACK) || \
                           ((FLAG) == DMA_FLAG_OVRA) || ((FLAG) == DMA_FLAG_OVRB) || \
                           ((FLAG) == DMA_FLAG_PEA) || ((FLAG) == DMA_FLAG_PEB) || \
                           ((FLAG) == DMA_FLAG_FEA) || ((FLAG) == DMA_FLAG_FEB) || \
                           ((FLAG) == DMA_FLAG_NSSS))
/**
  * @}
  */

/** @defgroup DMA_interrupts_definition 
  * @{
  */
#define DMA_IT_TXULODB                     ((uint32_t)SC_ISR_TXULODB)
#define DMA_IT_TXULODA                     ((uint32_t)SC_ISR_TXULODA)
#define DMA_IT_RXULODB                     ((uint32_t)SC_ISR_RXULODB)
#define DMA_IT_RXULODA                     ((uint32_t)SC_ISR_RXULODA)

#define IS_DMA_IT(IT) (((IT) == DMA_IT_TXULODB) || ((IT) == DMA_IT_TXULODA) || \
                       ((IT) == DMA_IT_RXULODB) || ((IT) == DMA_IT_RXULODA))
/**
  * @}
  */

#define IS_DMA_CHANNEL_PERIPH(PERIPH) (((PERIPH) == SC1_DMA_ChannelTx) || \
                                       ((PERIPH) == SC1_DMA_ChannelRx) || \
                                       ((PERIPH) == SC2_DMA_ChannelTx) || \
                                       ((PERIPH) == SC2_DMA_ChannelRx))

#define IS_DMA_VALID_ADDRESS(ADDRESS) (((ADDRESS) >= 0x20000000) && ((ADDRESS) <= 0x20000FFF))

/**
  * @}
  */

/**
  * @}
  */

/** @defgroup SC_Exported_Macros
  * @{
  */

/**
  * @}
  */
  
/** @defgroup SC_Exported_Functions
  * @{
  */
void UART_DeInit(SC_UART_TypeDef* SCx_UART);
void UART_Init(SC_UART_TypeDef* SCx_UART, UART_InitTypeDef* UART_InitStruct);
void UART_StructInit(UART_InitTypeDef* UART_InitStruct);
void UART_Cmd(SC_UART_TypeDef* SCx_UART, FunctionalState NewState);
void UART_ITConfig(SC_IT_TypeDef* SCx_IT, uint32_t UART_IT, FunctionalState NewState);
void UART_TriggerEventConfig(SC_IT_TypeDef* SCx_IT, uint32_t UART_IT, uint32_t TriggerEvent);
void UART_RTSAssertionCmd(SC_UART_TypeDef* SCx_UART, FunctionalState NewState);
void UART_SendData(SC_UART_TypeDef* SCx_UART, uint8_t Data);
uint8_t UART_ReceiveData(SC_UART_TypeDef* SCx_UART);
FlagStatus UART_GetFlagStatus(SC_UART_TypeDef* SCx_UART, uint32_t UART_FLAG);
ITStatus UART_GetITStatus(SC_IT_TypeDef* SCx_IT, uint32_t UART_IT);
void UART_ClearITPendingBit(SC_IT_TypeDef* SCx_IT, uint32_t UART_IT);

void SPI_DeInit(SC_SPI_TypeDef* SCx_SPI);
void SPI_Init(SC_SPI_TypeDef* SCx_SPI, SPI_InitTypeDef* SPI_InitStruct);
void SPI_StructInit(SPI_InitTypeDef* SPI_InitStruct);
void SPI_ReceiverModeConfig(SC_SPI_TypeDef* SCx_SPI, uint32_t SPI_ReceiverMode);
void SPI_LastByteRepeatCmd(SC_SPI_TypeDef* SCx_SPI, FunctionalState NewState);
void SPI_Cmd(SC_SPI_TypeDef* SCx_SPI, FunctionalState NewState);
void SPI_ITConfig(SC_IT_TypeDef* SCx_IT, uint32_t SPI_IT, FunctionalState NewState);
void SPI_TriggerEventConfig(SC_IT_TypeDef* SCx_IT, uint32_t SPI_IT, uint32_t TriggerEvent);
void SPI_SendData(SC_SPI_TypeDef* SCx_SPI, uint8_t Data);
uint8_t SPI_ReceiveData(SC_SPI_TypeDef* SCx_SPI);
FlagStatus SPI_GetFlagStatus(SC_SPI_TypeDef* SCx_SPI, uint32_t SPI_FLAG);
ITStatus SPI_GetITStatus(SC_IT_TypeDef* SCx_IT, uint32_t SPI_IT);
void SPI_ClearITPendingBit(SC_IT_TypeDef* SCx_IT, uint32_t SPI_IT);

void I2C_DeInit(SC_I2C_TypeDef* SCx_I2C);
void I2C_Init(SC_I2C_TypeDef* SCx_I2C, I2C_InitTypeDef* I2C_InitStruct);
void I2C_StructInit(I2C_InitTypeDef* I2C_InitStruct);
void I2C_Cmd(SC_I2C_TypeDef* SCx_I2C, FunctionalState NewState);
void I2C_ITConfig(SC_IT_TypeDef* SCx_IT, uint32_t I2C_IT, FunctionalState NewState);
void I2C_GenerateSTART(SC_I2C_TypeDef* SCx_I2C);
void I2C_GenerateSTOP(SC_I2C_TypeDef* SCx_I2C);
void I2C_AcknowledgeConfig(SC_I2C_TypeDef* SCx_I2C, FunctionalState NewState);
void I2C_Send7bitAddress(SC_I2C_TypeDef* SCx_I2C, uint8_t Address, uint8_t I2C_Direction);
void I2C_SendData(SC_I2C_TypeDef* SCx_I2C, uint8_t Data);
uint8_t I2C_ReceiveData(SC_I2C_TypeDef* SCx_I2C);
FlagStatus I2C_GetFlagStatus(SC_I2C_TypeDef* SCx_I2C, uint32_t I2C_FLAG);
ITStatus I2C_GetITStatus(SC_IT_TypeDef* SCx_IT, uint32_t I2C_IT);
void I2C_ClearITPendingBit(SC_IT_TypeDef* SCx_IT, uint32_t I2C_IT);

void SC_DMA_ChannelReset(SC_DMA_TypeDef* SCx_DMA, uint32_t Channely);
void SC_DMA_Init(SC_DMA_Channel_TypeDef* SCx_DMA_Channely, SC_DMA_InitTypeDef* SC_DMA_InitStruct);
void SC_DMA_StructInit(SC_DMA_InitTypeDef* SC_DMA_InitStruct);
void SC_DMA_ChannelLoadEnable(SC_DMA_TypeDef* SCx_DMA, uint32_t Channelxy);
void SC_DMA_ITConfig(SC_IT_TypeDef* SCx_IT, uint32_t DMA_IT, FunctionalState NewState);
uint32_t SC_DMA_GetCounter(SC_DMA_TypeDef* SCx_DMA, uint32_t Counter);
uint32_t SC_DMA_GetReceiverErrorOffset(SC_DMA_TypeDef* SCx_DMA, uint32_t RegisterError);
FlagStatus SC_DMA_GetFlagStatus(SC_DMA_TypeDef* SCx_DMA, uint32_t DMA_FLAG);
ITStatus SC_DMA_GetITStatus(SC_IT_TypeDef* SCx_IT, uint32_t DMA_IT);
void SC_DMA_ClearITPendingBit(SC_IT_TypeDef* SCx_IT, uint32_t DMA_IT);

/**
  * @}
  */
  
#ifdef __cplusplus
}

#endif

#endif /*__STM32W108XX_SC_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

