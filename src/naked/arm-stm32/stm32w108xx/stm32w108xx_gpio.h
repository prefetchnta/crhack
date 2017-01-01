/**
  ******************************************************************************
  * @file    stm32w108xx_gpio.h
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    30-November-2012
  * @brief   This file contains all the functions prototypes for the GPIO 
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
#ifndef __STM32W108xx_GPIO_H
#define __STM32W108xx_GPIO_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32w108xx.h"

/** @addtogroup STM32W108xx_StdPeriph_Driver
  * @{
  */

/** @addtogroup GPIO
  * @{
  */

/* Exported types ------------------------------------------------------------*/     

/** @brief GPIO Mode enumeration 
  * @{
  */
typedef enum
{  
  GPIO_Mode_AN           = 0x00, /*!< GPIO Analog Mode */
  GPIO_Mode_OUT_PP       = 0x01, /*!< GPIO Output Mode PP */ 
  GPIO_Mode_IN           = 0x04, /*!< GPIO Input Mode NOPULL */
  GPIO_Mode_OUT_OD       = 0x05, /*!< GPIO Output Mode OD */ 
  GPIO_Mode_IN_PUD       = 0x08, /*!< GPIO Input Mode PuPd */
  GPIO_Mode_AF_PP        = 0x09, /*!< GPIO Alternate function Mode PP */
  GPIO_Mode_AF_PP_SPI    = 0x0B, /*!< GPIO Alternate function Mode SPI SCLK PP */
  GPIO_Mode_AF_OD        = 0x0D  /*!< GPIO Alternate function Mode OD */    
}GPIOMode_TypeDef;

#define IS_GPIO_MODE(MODE) (((MODE) == GPIO_Mode_AN)  || ((MODE) == GPIO_Mode_OUT_PP) || \
                           ((MODE) == GPIO_Mode_IN) || ((MODE) == GPIO_Mode_OUT_OD) || \
                           ((MODE) == GPIO_Mode_IN_PUD)|| ((MODE) == GPIO_Mode_AF_PP) || \
                           ((MODE) == GPIO_Mode_AF_PP_SPI) ||((MODE) == GPIO_Mode_AF_OD))

/** 
  * @brief  GPIO Init structure definition
  */ 
typedef struct
{
  uint32_t GPIO_Pin;              /*!< Specifies the GPIO pins to be configured.
                                       This parameter can be any value of @ref GPIO_pins_define */

  GPIOMode_TypeDef GPIO_Mode;     /*!< Specifies the operating mode for the selected pins.
                                       This parameter can be a value of @ref GPIOMode_TypeDef */
}GPIO_InitTypeDef;
                                                                     
/** @defgroup GPIO_Exported_Constants
  * @{
  */
  
#define IS_GPIO_ALL_PERIPH(PERIPH) (((PERIPH) == GPIOA) || \
                                    ((PERIPH) == GPIOB) || \
                                    ((PERIPH) == GPIOC))    
  
/** @defgroup Bit_SET_and_Bit_RESET_enumeration
  * @{
  */
typedef enum
{ Bit_RESET = 0,
  Bit_SET 
}BitAction;

#define IS_GPIO_BIT_ACTION(ACTION) (((ACTION) == Bit_RESET) || ((ACTION) == Bit_SET))
/**
  * @}
  */

/** @defgroup GPIO_pins_define 
  * @{
  */
#define GPIO_Pin_0                 ((uint32_t)0x00000001)  /*!< Pin 0 selected */
#define GPIO_Pin_1                 ((uint32_t)0x00000002)  /*!< Pin 1 selected */
#define GPIO_Pin_2                 ((uint32_t)0x00000004)  /*!< Pin 2 selected */
#define GPIO_Pin_3                 ((uint32_t)0x00000008)  /*!< Pin 3 selected */
#define GPIO_Pin_4                 ((uint32_t)0x00000010)  /*!< Pin 4 selected */
#define GPIO_Pin_5                 ((uint32_t)0x00000020)  /*!< Pin 5 selected */
#define GPIO_Pin_6                 ((uint32_t)0x00000040)  /*!< Pin 6 selected */
#define GPIO_Pin_7                 ((uint32_t)0x00000080)  /*!< Pin 7 selected */
#define GPIO_Pin_All               ((uint32_t)0x000000FF)  /*!< All pins selected */

#define IS_GPIO_PIN(PIN) ((PIN) != (uint32_t)0x00000000)
#define IS_GET_GPIO_PIN(PIN) (((PIN) == GPIO_Pin_0) || \
                              ((PIN) == GPIO_Pin_1) || \
                              ((PIN) == GPIO_Pin_2) || \
                              ((PIN) == GPIO_Pin_3) || \
                              ((PIN) == GPIO_Pin_4) || \
                              ((PIN) == GPIO_Pin_5) || \
                              ((PIN) == GPIO_Pin_6) || \
                              ((PIN) == GPIO_Pin_7))
/**
  * @}
  */

#define IS_GPIO_GET_DBGFLAG(DBGFLAG) (((DBGFLAG) == GPIO_DBGSR_SWEN) || \
                                      ((DBGFLAG) == GPIO_DBGSR_FORCEDBG) || \
                                      ((DBGFLAG) == GPIO_DBGSR_BOOTMODE)) 
/**
  * @}
  */

/** @defgroup CLK_PC_Trace_Select 
  * @{
  */
#define GPIO_BBDEBUG        ((uint32_t)0x00000000)
#define GPIO_PCTRACE        ((uint32_t)0x00000001)
#define IS_GPIO_PCTRACE(PCTRACE) (((PCTRACE) == GPIO_BBDEBUG) || \
                                  ((PCTRACE) == GPIO_PCTRACE))  
/**
  * @}
  */
/** @defgroup GPIO_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup GPIO_Exported_Functions
  * @{
  */
/*  Function used to set the GPIO configuration to the default reset state ****/
void GPIO_DeInit(GPIO_TypeDef* GPIOx);
/* Initialization and Configuration functions *********************************/
void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct);
void GPIO_StructInit(GPIO_InitTypeDef* GPIO_InitStruct);
/* GPIO Read and Write functions **********************************************/
uint32_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin);
uint32_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx);
uint32_t GPIO_ReadOutputDataBit(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin);
uint32_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx);
void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin);
void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin);
void GPIO_WriteBit(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin, BitAction BitVal);
void GPIO_Write(GPIO_TypeDef* GPIOx, uint16_t PortVal);
/* Debug  functions ***********************************************************/
void GPIO_PCTraceConfig(uint32_t PCTRACE_SEL);
void GPIO_DebugInterfaceCmd(FunctionalState NewState);
void GPIO_ExternalOverrideCmd(FunctionalState NewState);
FlagStatus GPIO_GetDebugFlagStatus(uint16_t GPIO_DBGFLAG);

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /*__STM32W108xx_GPIO_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

