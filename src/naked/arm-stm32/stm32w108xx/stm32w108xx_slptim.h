/**
  ******************************************************************************
  * @file    stm32w108xx_slptim.h
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    30-November-2012
  * @brief   This file contains all the functions prototypes for the TIM 
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
#ifndef __STM32W108XX_SLPTIM_H
#define __STM32W108XX_SLPTIM_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32w108xx.h"

/** @addtogroup STM32W108xx_StdPeriph_Driver
  * @{
  */

/** @addtogroup SLPTIM
  * @{
  */
   
/* Exported types ------------------------------------------------------------*/
  
 /** 
   * @brief  SLPTIM Init structure definition
   */
typedef struct
{
  uint32_t SLPTIM_Clock;            /*!< Specifies the clock to be used.                                       
                                       This parameter must be a value of @ref SLPTIM_Clocks_Select  */

  uint32_t SLPTIM_Prescaler;         /*!< Specifies the prescaler value used to divide the TIM clock.
                                       This parameter can be a value of @ref SLPTIM_Clock_Division */
  
  uint32_t SLPTIM_DebugMode;        /*!< Specifies whether the timer is running or paused during debug mode.                                       
                                       This parameter must be a value of @ref SLPTIM_Debug_Mode  */

  uint32_t SLPTIM_CounterMode;       /*!< Specifies the counter mode.
                                       This parameter can be a value of @ref SLPTIM_Counter_Mode */  
} SLPTIM_InitTypeDef;  

/** @defgroup SLPTIM_Exported_Constants
  * @{
  */
  
/** @defgroup SLPTIM_Clocks_Select 
  * @{
  */
#define SLPTIM_CLK_32KHZ         ((uint32_t)SLPTMR_CR_CLKSEL)
#define SLPTIM_CLK_1KHZ          ((uint32_t)0x00000000)
#define IS_SLPTIM_GET_CLKSEL(CLK) (((CLK) == SLPTIM_CLK_32KHZ) || \
                                   ((CLK) == SLPTIM_CLK_1KHZ))  
/**
  * @}
  */

/** @defgroup SLPTIM_Clock_Division 
  * @{
  */
#define SLPTIM_CLK_DIV0               ((uint32_t)0x00000000)
#define SLPTIM_CLK_DIV1               ((uint32_t)0x00000010)
#define SLPTIM_CLK_DIV2               ((uint32_t)0x00000020)
#define SLPTIM_CLK_DIV3               ((uint32_t)0x00000030)
#define SLPTIM_CLK_DIV4               ((uint32_t)0x00000040)
#define SLPTIM_CLK_DIV5               ((uint32_t)0x00000050)
#define SLPTIM_CLK_DIV6               ((uint32_t)0x00000060)
#define SLPTIM_CLK_DIV7               ((uint32_t)0x00000070)
#define SLPTIM_CLK_DIV8               ((uint32_t)0x00000080)
#define SLPTIM_CLK_DIV9               ((uint32_t)0x00000090)
#define SLPTIM_CLK_DIV10              ((uint32_t)0x000000A0)
#define SLPTIM_CLK_DIV11              ((uint32_t)0x000000B0)
#define SLPTIM_CLK_DIV12              ((uint32_t)0x000000C0)
#define SLPTIM_CLK_DIV13              ((uint32_t)0x000000D0)
#define SLPTIM_CLK_DIV14              ((uint32_t)0x000000E0)
#define SLPTIM_CLK_DIV15              ((uint32_t)0x000000F0)
#define IS_SLPTIM_CLKDIV(CLKDIV) (((CLKDIV) == SLPTIM_CLK_DIV0) || ((CLKDIV) == SLPTIM_CLK_DIV1) || \
                                  ((CLKDIV) == SLPTIM_CLK_DIV2) || ((CLKDIV) == SLPTIM_CLK_DIV3) || \
                                  ((CLKDIV) == SLPTIM_CLK_DIV4) || ((CLKDIV) == SLPTIM_CLK_DIV5) || \
                                  ((CLKDIV) == SLPTIM_CLK_DIV6) || ((CLKDIV) == SLPTIM_CLK_DIV7) || \
                                  ((CLKDIV) == SLPTIM_CLK_DIV8) || ((CLKDIV) == SLPTIM_CLK_DIV9) || \
                                  ((CLKDIV) == SLPTIM_CLK_DIV10) || ((CLKDIV) == SLPTIM_CLK_DIV11) || \
                                  ((CLKDIV) == SLPTIM_CLK_DIV12) || ((CLKDIV) == SLPTIM_CLK_DIV13) || \
                                  ((CLKDIV) == SLPTIM_CLK_DIV14) || ((CLKDIV) == SLPTIM_CLK_DIV15))  
/**
  * @}
  */

/** @defgroup SLPTIM_Debug_Mode 
  * @{
  */
#define SLPTIM_DBGRUN              ((uint32_t)0x00000000)
#define SLPTIM_DBGPAUSE            ((uint32_t)SLPTMR_CR_DBGP)
#define IS_SLPTIM_DBGMODE(DBGMODE) (((DBGMODE) == SLPTIM_DBGRUN) || \
                                    ((DBGMODE) == SLPTIM_DBGPAUSE))
/**
  * @}
  */

/** @defgroup SLPTIM_Counter_Mode 
  * @{
  */
#define SLPTIM_CountForward           ((uint32_t)0x00000000)
#define SLPTIM_CountBackward          ((uint32_t)SLPTMR_CR_REVERSE)
#define IS_SLPTIM_COUNTER_MODE(MODE) (((MODE) == SLPTIM_CountForward) || \
                                      ((MODE) == SLPTIM_CountBackward))
/**
  * @}
  */

/** @defgroup SLPTIM_interrupt_sources 
  * @{
  */
#define SLPTIM_IT_WRAP        ((uint32_t)SLPTMR_IER_WRAP)
#define SLPTIM_IT_CMPA        ((uint32_t)SLPTMR_IER_CMPA)
#define SLPTIM_IT_CMPB        ((uint32_t)SLPTMR_IER_CMPB)
#define IS_SLPTIM_IT(IT) (((IT) == SLPTIM_IT_WRAP) || \
                          ((IT) == SLPTIM_IT_CMPA) || \
                          ((IT) == SLPTIM_IT_CMPB))  
/**
  * @}
  */

/** @defgroup SLPTIM_flags 
  * @{
  */
#define SLPTIM_FLAG_WRAP          ((uint32_t)SLPTMR_ISR_WRAP)
#define SLPTIM_FLAG_CMPA          ((uint32_t)SLPTMR_ISR_CMPA)
#define SLPTIM_FLAG_CMPB          ((uint32_t)SLPTMR_ISR_CMPB)
#define IS_SLPTIM_FLAG(FLAG) (((FLAG) == SLPTIM_FLAG_WRAP) || \
                              ((FLAG) == SLPTIM_FLAG_CMPA) || \
                              ((FLAG) == SLPTIM_FLAG_CMPB))  
/**
  * @}
  */
/**
  * @}
  */
  
/** @defgroup SLPTIM_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup SLPTIM_Exported_Functions
  * @{
  */
/* SLPTIM management functions ************************************************/ 
void SLPTIM_DeInit(void);
void SLPTIM_Init(SLPTIM_InitTypeDef* SLPTIM_InitStruct);
void SLPTIM_StructInit(SLPTIM_InitTypeDef* SLPTIM_InitStruct);
void SLPTIM_Cmd(FunctionalState NewState);
void SLPTIM_SetCompareA(uint32_t CompareA);
void SLPTIM_SetCompareB(uint32_t CompareB);
uint32_t SLPTIM_GetCounter(void);
uint32_t SLPTIM_GetCounterHigh(void);
uint32_t SLPTIM_GetCounterLow(void);
/* Interrupts and flags management functions **********************************/
void SLPTIM_ForceIT(uint32_t SLPTIM_IT);
void SLPTIM_ITConfig(uint32_t SLPTIM_IT, FunctionalState NewState);
FlagStatus SLPTIM_GetFlagStatus(uint32_t SLPTIM_FLAG);
void SLPTIM_ClearFlag(uint32_t SLPTIM_FLAG);
ITStatus SLPTIM_GetITStatus(uint32_t SLPTIM_IT);
void SLPTIM_ClearITPendingBit(uint32_t SLPTIM_IT);

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /*__STM32W108XX_SLPTIM_H */

/**
  * @}
  */ 

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

