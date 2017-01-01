/**
  ******************************************************************************
  * @file    stm32w108xx_exti.h
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    30-November-2012
  * @brief   This file contains all the functions prototypes for the EXTI 
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
#ifndef __STM32W108xx_EXTI_H
#define __STM32W108xx_EXTI_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32w108xx.h"

/** @addtogroup STM32W108xx_StdPeriph_Driver
  * @{
  */

/** @addtogroup EXTI
  * @{
  */
  
/* Exported types ------------------------------------------------------------*/

/** @brief EXTI Trigger enumeration 
  * @{
  */
typedef enum
{
  EXTI_Trigger_Disable = 0x00,
  EXTI_Trigger_Rising_Edge = 0x20,
  EXTI_Trigger_Falling_Edge = 0x40,  
  EXTI_Trigger_Rising_Falling_Edge = 0x60,
  EXTI_Trigger_High_Level = 0x80,
  EXTI_Trigger_Low_Level = 0xA0
}EXTITrigger_TypeDef;

#define IS_EXTI_TRIGGER(TRIGGER) (((TRIGGER) == EXTI_Trigger_Disable) || \
                                  ((TRIGGER) == EXTI_Trigger_Rising_Edge) || \
                                  ((TRIGGER) == EXTI_Trigger_Falling_Edge) || \
                                  ((TRIGGER) == EXTI_Trigger_Rising_Falling_Edge) || \
                                  ((TRIGGER) == EXTI_Trigger_High_Level) || \
                                  ((TRIGGER) == EXTI_Trigger_Low_Level))  
/** 
  * @brief  EXTI Init Structure definition  
  */
typedef struct
{
  uint32_t EXTI_Source;                        /*!< Specifies the EXTI source to be configured.
                                         This parameter can be GPIO_SourcePxy where x can be (A, B or C) and y can be (0..7). */

  uint8_t EXTI_IRQn;                           /*!< Specifies the GPIO IRQ handler for the EXTI source.
                                         This parameter can be EXTI_IRQn where n can be (A, B, C or D). */
    
  EXTITrigger_TypeDef EXTI_Trigger;            /*!< Specifies the trigger signal active edge for the EXTI lines.
                                         This parameter can be a value of @ref EXTITrigger_TypeDef */

  FunctionalState EXTI_LineCmd;                /*!< Specifies the new state of the selected EXTI line.
                                         This parameter can be set either to ENABLE or DISABLE */
  
  FunctionalState EXTI_DigitalFilterCmd;        /*!< Specifies the new state of the digital filter.
                                         This parameter can be set either to ENABLE or DISABLE */
   
}EXTI_InitTypeDef;

/** @defgroup EXTI_Exported_Constants
  * @{
  */
  
/** @defgroup EXTI_Pin_sources 
  * @{
  */ 
#define EXTI_SourcePA0            ((uint8_t)0x00)
#define EXTI_SourcePA1            ((uint8_t)0x01)
#define EXTI_SourcePA2            ((uint8_t)0x02)
#define EXTI_SourcePA3            ((uint8_t)0x03)
#define EXTI_SourcePA4            ((uint8_t)0x04)
#define EXTI_SourcePA5            ((uint8_t)0x05)
#define EXTI_SourcePA6            ((uint8_t)0x06)
#define EXTI_SourcePA7            ((uint8_t)0x07)

#define EXTI_SourcePB0            ((uint8_t)0x08)
#define EXTI_SourcePB1            ((uint8_t)0x09)
#define EXTI_SourcePB2            ((uint8_t)0x0A)
#define EXTI_SourcePB3            ((uint8_t)0x0B)
#define EXTI_SourcePB4            ((uint8_t)0x0C)
#define EXTI_SourcePB5            ((uint8_t)0x0D)
#define EXTI_SourcePB6            ((uint8_t)0x0E)
#define EXTI_SourcePB7            ((uint8_t)0x0F)

#define EXTI_SourcePC0            ((uint8_t)0x10)
#define EXTI_SourcePC1            ((uint8_t)0x11)
#define EXTI_SourcePC2            ((uint8_t)0x12)
#define EXTI_SourcePC3            ((uint8_t)0x13)
#define EXTI_SourcePC4            ((uint8_t)0x14)
#define EXTI_SourcePC5            ((uint8_t)0x15)
#define EXTI_SourcePC6            ((uint8_t)0x16)
#define EXTI_SourcePC7            ((uint8_t)0x17)

#define IS_EXTI_SOURCE(EXTI_SOURCE) (((EXTI_SOURCE) == EXTI_SourcePA0) || \
                                     ((EXTI_SOURCE) == EXTI_SourcePA1) || \
                                     ((EXTI_SOURCE) == EXTI_SourcePA2) || \
                                     ((EXTI_SOURCE) == EXTI_SourcePA3) || \
                                     ((EXTI_SOURCE) == EXTI_SourcePA4) || \
                                     ((EXTI_SOURCE) == EXTI_SourcePA5) || \
                                     ((EXTI_SOURCE) == EXTI_SourcePA6) || \
                                     ((EXTI_SOURCE) == EXTI_SourcePA7) || \
                                     ((EXTI_SOURCE) == EXTI_SourcePB0) || \
                                     ((EXTI_SOURCE) == EXTI_SourcePB1) || \
                                     ((EXTI_SOURCE) == EXTI_SourcePB2) || \
                                     ((EXTI_SOURCE) == EXTI_SourcePB3) || \
                                     ((EXTI_SOURCE) == EXTI_SourcePB4) || \
                                     ((EXTI_SOURCE) == EXTI_SourcePB5) || \
                                     ((EXTI_SOURCE) == EXTI_SourcePB6) || \
                                     ((EXTI_SOURCE) == EXTI_SourcePB7) || \
                                     ((EXTI_SOURCE) == EXTI_SourcePC0) || \
                                     ((EXTI_SOURCE) == EXTI_SourcePC1) || \
                                     ((EXTI_SOURCE) == EXTI_SourcePC2) || \
                                     ((EXTI_SOURCE) == EXTI_SourcePC3) || \
                                     ((EXTI_SOURCE) == EXTI_SourcePC4) || \
                                     ((EXTI_SOURCE) == EXTI_SourcePC5) || \
                                     ((EXTI_SOURCE) == EXTI_SourcePC6) || \
                                     ((EXTI_SOURCE) == EXTI_SourcePC7))
/**
  * @}
  */

/** @defgroup EXTI_IRQ_Sources 
  * @{
  */
#define EXTI_IRQA       ((uint32_t)0x00000000)
#define EXTI_IRQB       ((uint32_t)0x00000010)
#define EXTI_IRQC       ((uint32_t)0x00000020)
#define EXTI_IRQD       ((uint32_t)0x00000031)

#define IS_EXTI_IRQ(EXTI_IRQ) (((EXTI_IRQ) == EXTI_IRQA) || \
                               ((EXTI_IRQ) == EXTI_IRQB) || \
                               ((EXTI_IRQ) == EXTI_IRQC) || \
                               ((EXTI_IRQ) == EXTI_IRQD))                                                
/**
  * @}
  */
  
/**
  * @}
  */
  
/** @defgroup EXTI_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup EXTI_Exported_Functions
  * @{
  */

/* EXTI Initialization and Configuration **************************************/
void EXTI_DeInit(void);
void EXTI_Init(EXTI_InitTypeDef* EXTI_InitStruct);
void EXTI_StructInit(EXTI_InitTypeDef* EXTI_InitStruct);
/* Interrupts and flags management functions **********************************/
ITStatus EXTI_GetITStatus(uint32_t EXTI_IRQn);
void EXTI_ClearITPendingBit(uint32_t EXTI_IRQn);

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /*__STM32W108xx_EXTI_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

