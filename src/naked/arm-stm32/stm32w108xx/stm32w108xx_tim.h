/**
  ******************************************************************************
  * @file    stm32w108xx_tim.h
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
#ifndef __STM32W108XX_TIM_H
#define __STM32W108XX_TIM_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32w108xx.h"

/** @addtogroup STM32W108xx_StdPeriph_Driver
  * @{
  */

/** @addtogroup TIM
  * @{
  */ 

/* Exported types ------------------------------------------------------------*/

/** 
  * @brief  TIM Time Base Init structure definition
  * @note   This sturcture is used with all TIMx.
  */
typedef struct
{
  uint32_t TIM_Prescaler;         /*!< Specifies the prescaler value used to divide the TIM clock.
                                       This parameter can be a number between 0x0000 and 0x000F */

  uint32_t TIM_CounterMode;       /*!< Specifies the counter mode.
                                       This parameter can be a value of @ref TIM_Counter_Mode */

  uint32_t TIM_Period;            /*!< Specifies the period value to be loaded into the active
                                       Auto-Reload Register at the next update event.
                                       This parameter must be a number between 0x0000 and 0xFFFF.  */ 
} TIM_TimeBaseInitTypeDef;       

/** 
  * @brief  TIM Output Compare Init structure definition  
  */
typedef struct
{
  uint32_t TIM_OCMode;        /*!< Specifies the TIM mode.
                                   This parameter can be a value of @ref TIM_Output_Compare_and_PWM_modes */

  uint32_t TIM_OutputState;   /*!< Specifies the TIM Output Compare state.
                                   This parameter can be a value of @ref TIM_Output_Compare_state */

  uint32_t TIM_Pulse;         /*!< Specifies the pulse value to be loaded into the Capture Compare Register. 
                                   This parameter can be a number between 0x0000 and 0xFFFF */

  uint32_t TIM_OCPolarity;    /*!< Specifies the output polarity.
                                   This parameter can be a value of @ref TIM_Output_Compare_Polarity */
} TIM_OCInitTypeDef;

/** 
  * @brief  TIM Input Capture Init structure definition  
  */
typedef struct
{

  uint32_t TIM_Channel;      /*!< Specifies the TIM channel.
                                  This parameter can be a value of @ref TIM_Channel */

  uint32_t TIM_ICPolarity;   /*!< Specifies the active edge of the input signal.
                                  This parameter can be a value of @ref TIM_Input_Capture_Polarity */

  uint32_t TIM_ICSelection;  /*!< Specifies the input.
                                  This parameter can be a value of @ref TIM_Input_Capture_Selection */

  uint32_t TIM_ICPrescaler;  /*!< Specifies the Input Capture Prescaler.
                                  This parameter can be a value of @ref TIM_Input_Capture_Prescaler */

  uint32_t TIM_ICFilter;     /*!< Specifies the input capture filter.
                                  This parameter can be a number between 0x0 and 0xF */
} TIM_ICInitTypeDef;

  
/** @defgroup TIM_Exported_constants 
  * @{
  */

#define IS_TIM_ALL_PERIPH(PERIPH) (((PERIPH) == TIM1) || \
                                   ((PERIPH) == TIM2)) 

#define IS_TIM_LIST1_PERIPH(PERIPH) (((PERIPH) == TIM2))

#define IS_TIM_IT_ALL_PERIPH(PERIPH) (((PERIPH) == TIM1_IT) || \
                                      ((PERIPH) == TIM2_IT))

/** @defgroup TIM_Output_Compare_and_PWM_modes 
  * @{
  */
#define TIM_OCMode_Timing     ((uint32_t)0x00000000)
#define TIM_OCMode_Active     ((uint32_t)0x00000010)
#define TIM_OCMode_Inactive   ((uint32_t)0x00000020)
#define TIM_OCMode_Toggle     ((uint32_t)0x00000030)
#define TIM_OCMode_PWM1       ((uint32_t)0x00000060)
#define TIM_OCMode_PWM2       ((uint32_t)0x00000070)
#define IS_TIM_OC_MODE(MODE) (((MODE) == TIM_OCMode_Timing) || \
                              ((MODE) == TIM_OCMode_Active) || \
                              ((MODE) == TIM_OCMode_Inactive) || \
                              ((MODE) == TIM_OCMode_Toggle)|| \
                              ((MODE) == TIM_OCMode_PWM1) || \
                              ((MODE) == TIM_OCMode_PWM2))
#define IS_TIM_OCM(MODE) (((MODE) == TIM_OCMode_Timing) || \
                          ((MODE) == TIM_OCMode_Active) || \
                          ((MODE) == TIM_OCMode_Inactive) || \
                          ((MODE) == TIM_OCMode_Toggle)|| \
                          ((MODE) == TIM_OCMode_PWM1) || \
                          ((MODE) == TIM_OCMode_PWM2) ||	\
                          ((MODE) == TIM_ForcedAction_Active) || \
                          ((MODE) == TIM_ForcedAction_InActive))
/**
  * @}
  */

/** @defgroup TIM_One_Pulse_Mode 
  * @{
  */
#define TIM_OPMode_Single                  ((uint32_t)0x00000008)
#define TIM_OPMode_Repetitive              ((uint32_t)0x00000000)
#define IS_TIM_OPM_MODE(MODE) (((MODE) == TIM_OPMode_Single) || \
                               ((MODE) == TIM_OPMode_Repetitive))
/**
  * @}
  */ 

/** @defgroup TIM_Channel 
  * @{
  */
#define TIM_Channel_1                      ((uint32_t)0x00000000)
#define TIM_Channel_2                      ((uint32_t)0x00000004)
#define TIM_Channel_3                      ((uint32_t)0x00000008)
#define TIM_Channel_4                      ((uint32_t)0x0000000C)

#define IS_TIM_CHANNEL(CHANNEL) (((CHANNEL) == TIM_Channel_1) || \
                                 ((CHANNEL) == TIM_Channel_2) || \
                                 ((CHANNEL) == TIM_Channel_3) || \
                                 ((CHANNEL) == TIM_Channel_4))
#define IS_TIM_PWMI_CHANNEL(CHANNEL) (((CHANNEL) == TIM_Channel_1) || \
                                      ((CHANNEL) == TIM_Channel_2))
/**
  * @}
  */ 

/** @defgroup TIM_Counter_Mode 
  * @{
  */
#define TIM_CounterMode_Up                 ((uint32_t)0x00000000)
#define TIM_CounterMode_Down               ((uint32_t)0x00000010)
#define TIM_CounterMode_CenterAligned1     ((uint32_t)0x00000020)
#define TIM_CounterMode_CenterAligned2     ((uint32_t)0x00000040)
#define TIM_CounterMode_CenterAligned3     ((uint32_t)0x00000060)
#define IS_TIM_COUNTER_MODE(MODE) (((MODE) == TIM_CounterMode_Up) ||  \
                                   ((MODE) == TIM_CounterMode_Down) || \
                                   ((MODE) == TIM_CounterMode_CenterAligned1) || \
                                   ((MODE) == TIM_CounterMode_CenterAligned2) || \
                                   ((MODE) == TIM_CounterMode_CenterAligned3))
/**
  * @}
  */ 

/** @defgroup TIM_Output_Compare_Polarity 
  * @{
  */
#define TIM_OCPolarity_High                ((uint32_t)0x00000000)
#define TIM_OCPolarity_Low                 ((uint32_t)0x00000002)
#define IS_TIM_OC_POLARITY(POLARITY) (((POLARITY) == TIM_OCPolarity_High) || \
                                      ((POLARITY) == TIM_OCPolarity_Low))
/**
  * @}
  */


/** @defgroup TIM_Output_Compare_state
  * @{
  */
#define TIM_OutputState_Disable            ((uint32_t)0x00000000)
#define TIM_OutputState_Enable             ((uint32_t)0x00000001)
#define IS_TIM_OUTPUT_STATE(STATE) (((STATE) == TIM_OutputState_Disable) || \
                                    ((STATE) == TIM_OutputState_Enable))
/**
  * @}
  */ 


/** @defgroup TIM_Capture_Compare_state 
  * @{
  */
#define TIM_CCx_Enable                      ((uint32_t)0x00000001)
#define TIM_CCx_Disable                     ((uint32_t)0x00000000)
#define IS_TIM_CCX(CCX) (((CCX) == TIM_CCx_Enable) || \
                         ((CCX) == TIM_CCx_Disable))
/**
  * @}
  */ 


/** @defgroup TIM_Input_Capture_Polarity 
  * @{
  */
#define  TIM_ICPolarity_Rising             ((uint32_t)0x00000000)
#define  TIM_ICPolarity_Falling            ((uint32_t)0x00000002)
#define  TIM_ICPolarity_BothEdge           ((uint32_t)0x0000000A)
#define IS_TIM_IC_POLARITY(POLARITY) (((POLARITY) == TIM_ICPolarity_Rising) || \
                                      ((POLARITY) == TIM_ICPolarity_Falling)|| \
                                      ((POLARITY) == TIM_ICPolarity_BothEdge)) 
/**
  * @}
  */ 

/** @defgroup TIM_Input_Capture_Selection 
  * @{
  */
#define TIM_ICSelection_DirectTI           ((uint32_t)0x00000001) /*!< TIM Input 1, 2, 3 or 4 is selected to be 
                                                                   connected to IC1, IC2, IC3 or IC4, respectively */
#define TIM_ICSelection_IndirectTI         ((uint32_t)0x00000002) /*!< TIM Input 1, 2, 3 or 4 is selected to be
                                                                   connected to IC2, IC1, IC4 or IC3, respectively. */
#define TIM_ICSelection_TRGI                ((uint32_t)0x00000003) /*!< TIM Input 1, 2, 3 or 4 is selected to be connected to TRC. */
#define IS_TIM_IC_SELECTION(SELECTION) (((SELECTION) == TIM_ICSelection_DirectTI) || \
                                        ((SELECTION) == TIM_ICSelection_IndirectTI) || \
                                        ((SELECTION) == TIM_ICSelection_TRGI))
/**
  * @}
  */ 

/** @defgroup TIM_Input_Capture_Prescaler 
  * @{
  */
#define TIM_ICPSC_DIV1                     ((uint32_t)0x00000000) /*!< Capture performed each time an edge is detected on the capture input. */
#define TIM_ICPSC_DIV2                     ((uint32_t)0x00000004) /*!< Capture performed once every 2 events. */
#define TIM_ICPSC_DIV4                     ((uint32_t)0x00000008) /*!< Capture performed once every 4 events. */
#define TIM_ICPSC_DIV8                     ((uint32_t)0x0000000C) /*!< Capture performed once every 8 events. */
#define IS_TIM_IC_PRESCALER(PRESCALER) (((PRESCALER) == TIM_ICPSC_DIV1) || \
                                        ((PRESCALER) == TIM_ICPSC_DIV2) || \
                                        ((PRESCALER) == TIM_ICPSC_DIV4) || \
                                        ((PRESCALER) == TIM_ICPSC_DIV8))
/**
  * @}
  */ 

/** @defgroup TIM_interrupt_sources 
  * @{
  */
#define TIM_IT_Update                      ((uint32_t)0x00000001)
#define TIM_IT_CC1                         ((uint32_t)0x00000002)
#define TIM_IT_CC2                         ((uint32_t)0x00000004)
#define TIM_IT_CC3                         ((uint32_t)0x00000008)
#define TIM_IT_CC4                         ((uint32_t)0x00000010)
#define TIM_IT_Trigger                     ((uint32_t)0x00000040)
#define IS_TIM_ITRPT(IT) ((((IT) & (uint32_t)0xFFFFFF00) == 0x00000000) && ((IT) != 0x00000000))

#define IS_TIM_GET_IT(IT) (((IT) == TIM_IT_Update) || \
                           ((IT) == TIM_IT_CC1) || \
                           ((IT) == TIM_IT_CC2) || \
                           ((IT) == TIM_IT_CC3) || \
                           ((IT) == TIM_IT_CC4) || \
                           ((IT) == TIM_IT_Trigger))
/**
  * @}
  */ 

/** @defgroup TIM_External_Trigger_Prescaler 
  * @{
  */
#define TIM_ExtTRGPSC_OFF                  ((uint32_t)0x00000000)
#define TIM_ExtTRGPSC_DIV2                 ((uint32_t)0x00001000)
#define TIM_ExtTRGPSC_DIV4                 ((uint32_t)0x00002000)
#define TIM_ExtTRGPSC_DIV8                 ((uint32_t)0x00003000)
#define IS_TIM_EXT_PRESCALER(PRESCALER) (((PRESCALER) == TIM_ExtTRGPSC_OFF) || \
                                         ((PRESCALER) == TIM_ExtTRGPSC_DIV2) || \
                                         ((PRESCALER) == TIM_ExtTRGPSC_DIV4) || \
                                         ((PRESCALER) == TIM_ExtTRGPSC_DIV8))
/**
  * @}
  */ 

/** @defgroup TIM_Internal_Trigger_Selection 
  * @{
  */
#define TIM_TS_ITR0                        ((uint32_t)0x00000000)
#define TIM_TS_TI1F_ED                     ((uint32_t)0x00000040)
#define TIM_TS_TI1FP1                      ((uint32_t)0x00000050)
#define TIM_TS_TI2FP2                      ((uint32_t)0x00000060)
#define TIM_TS_ETRF                        ((uint32_t)0x00000070)
#define IS_TIM_TRIGGER_SELECTION(SELECTION) (((SELECTION) == TIM_TS_ITR0) || \
                                             ((SELECTION) == TIM_TS_TI1F_ED) || \
                                             ((SELECTION) == TIM_TS_TI1FP1) || \
                                             ((SELECTION) == TIM_TS_TI2FP2) || \
                                             ((SELECTION) == TIM_TS_ETRF))
#define IS_TIM_INTERNAL_TRIGGER_SELECTION(SELECTION) (((SELECTION) == TIM_TS_ITR0))
/**
  * @}
  */ 

/** @defgroup TIM_TIx_External_Clock_Source 
  * @{
  */
#define TIM_TIxExternalCLK1Source_TI1      ((uint32_t)0x00000050)
#define TIM_TIxExternalCLK1Source_TI2      ((uint32_t)0x00000060)
#define TIM_TIxExternalCLK1Source_TI1ED    ((uint32_t)0x00000040)

/**
  * @}
  */ 

/** @defgroup TIM_External_Trigger_Polarity 
  * @{
  */ 
#define TIM_ExtTRGPolarity_Inverted        ((uint32_t)0x00008000)
#define TIM_ExtTRGPolarity_NonInverted     ((uint32_t)0x00000000)
#define IS_TIM_EXT_POLARITY(POLARITY) (((POLARITY) == TIM_ExtTRGPolarity_Inverted) || \
                                       ((POLARITY) == TIM_ExtTRGPolarity_NonInverted))
/**
  * @}
  */

/** @defgroup TIM_Prescaler_Reload_Mode 
  * @{
  */
#define TIM_PSCReloadMode_Update           ((uint32_t)0x00000000)
#define TIM_PSCReloadMode_Immediate        ((uint32_t)0x00000001)
#define IS_TIM_PRESCALER_RELOAD(RELOAD) (((RELOAD) == TIM_PSCReloadMode_Update) || \
                                         ((RELOAD) == TIM_PSCReloadMode_Immediate))
/**
  * @}
  */ 

/** @defgroup TIM_Forced_Action 
  * @{
  */
#define TIM_ForcedAction_Active            ((uint32_t)0x00000050)
#define TIM_ForcedAction_InActive          ((uint32_t)0x00000040)
#define IS_TIM_FORCED_ACTION(ACTION) (((ACTION) == TIM_ForcedAction_Active) || \
                                      ((ACTION) == TIM_ForcedAction_InActive))
/**
  * @}
  */ 

/** @defgroup TIM_Encoder_Mode 
  * @{
  */
#define TIM_EncoderMode_TI1                ((uint32_t)0x00000001)
#define TIM_EncoderMode_TI2                ((uint32_t)0x00000002)
#define TIM_EncoderMode_TI12               ((uint32_t)0x00000003)
#define IS_TIM_ENCODER_MODE(MODE) (((MODE) == TIM_EncoderMode_TI1) || \
                                   ((MODE) == TIM_EncoderMode_TI2) || \
                                   ((MODE) == TIM_EncoderMode_TI12))
/**
  * @}
  */ 

/** @defgroup TIM_Event_Source 
  * @{
  */
#define TIM_EventSource_Update             ((uint32_t)0x00000001)
#define TIM_EventSource_CC1                ((uint32_t)0x00000002)
#define TIM_EventSource_CC2                ((uint32_t)0x00000004)
#define TIM_EventSource_CC3                ((uint32_t)0x00000008)
#define TIM_EventSource_CC4                ((uint32_t)0x00000010)
#define TIM_EventSource_Trigger            ((uint32_t)0x00000040)
#define IS_TIM_EVENT_SOURCE(SOURCE) ((((SOURCE) & (uint32_t)0xFFFFFF00) == 0x00000000) && ((SOURCE) != 0x00000000))

/**
  * @}
  */ 

/** @defgroup TIM_Update_Source 
  * @{
  */
#define TIM_UpdateSource_Global            ((uint32_t)0x00000000) /*!< Source of update is the counter overflow/underflow
                                                                   or the setting of UG bit, or an update generation
                                                                   through the slave mode controller. */
#define TIM_UpdateSource_Regular           ((uint32_t)0x00000001) /*!< Source of update is counter overflow/underflow. */
#define IS_TIM_UPDATE_SOURCE(SOURCE) (((SOURCE) == TIM_UpdateSource_Global) || \
                                      ((SOURCE) == TIM_UpdateSource_Regular))
/**
  * @}
  */ 

/** @defgroup TIM_Output_Compare_Preload_State 
  * @{
  */
#define TIM_OCPreload_Enable               ((uint32_t)0x00000008)
#define TIM_OCPreload_Disable              ((uint32_t)0x00000000)
#define IS_TIM_OCPRELOAD_STATE(STATE) (((STATE) == TIM_OCPreload_Enable) || \
                                       ((STATE) == TIM_OCPreload_Disable))
/**
  * @}
  */ 

/** @defgroup TIM_Output_Compare_Fast_State 
  * @{
  */
#define TIM_OCFast_Enable                  ((uint32_t)0x00000004)
#define TIM_OCFast_Disable                 ((uint32_t)0x00000000)
#define IS_TIM_OCFAST_STATE(STATE) (((STATE) == TIM_OCFast_Enable) || \
                                    ((STATE) == TIM_OCFast_Disable))
/**
  * @}
  */ 

/** @defgroup TIM_Trigger_Output_Source 
  * @{
  */
#define TIM_TRGOSource_Reset               ((uint32_t)0x00000000)
#define TIM_TRGOSource_Enable              ((uint32_t)0x00000010)
#define TIM_TRGOSource_Update              ((uint32_t)0x00000020)
#define TIM_TRGOSource_OC1                 ((uint32_t)0x00000030)
#define TIM_TRGOSource_OC1Ref              ((uint32_t)0x00000040)
#define TIM_TRGOSource_OC2Ref              ((uint32_t)0x00000050)
#define TIM_TRGOSource_OC3Ref              ((uint32_t)0x00000060)
#define TIM_TRGOSource_OC4Ref              ((uint32_t)0x00000070)
#define IS_TIM_TRGO_SOURCE(SOURCE) (((SOURCE) == TIM_TRGOSource_Reset) || \
                                    ((SOURCE) == TIM_TRGOSource_Enable) || \
                                    ((SOURCE) == TIM_TRGOSource_Update) || \
                                    ((SOURCE) == TIM_TRGOSource_OC1) || \
                                    ((SOURCE) == TIM_TRGOSource_OC1Ref) || \
                                    ((SOURCE) == TIM_TRGOSource_OC2Ref) || \
                                    ((SOURCE) == TIM_TRGOSource_OC3Ref) || \
                                    ((SOURCE) == TIM_TRGOSource_OC4Ref))
/**
  * @}
  */ 

/** @defgroup TIM_Slave_Mode 
  * @{
  */
#define TIM_SlaveMode_Reset                ((uint32_t)0x00000004)
#define TIM_SlaveMode_Gated                ((uint32_t)0x00000005)
#define TIM_SlaveMode_Trigger              ((uint32_t)0x00000006)
#define TIM_SlaveMode_External1            ((uint32_t)0x00000007)
#define IS_TIM_SLAVE_MODE(MODE) (((MODE) == TIM_SlaveMode_Reset) || \
                                 ((MODE) == TIM_SlaveMode_Gated) || \
                                 ((MODE) == TIM_SlaveMode_Trigger) || \
                                 ((MODE) == TIM_SlaveMode_External1))
/**
  * @}
  */ 

/** @defgroup TIM_Master_Slave_Mode 
  * @{
  */
#define TIM_MasterSlaveMode_Enable         ((uint32_t)0x00000080)
#define TIM_MasterSlaveMode_Disable        ((uint32_t)0x00000000)
#define IS_TIM_MSM_STATE(STATE) (((STATE) == TIM_MasterSlaveMode_Enable) || \
                                 ((STATE) == TIM_MasterSlaveMode_Disable))
/**
  * @}
  */ 

/** @defgroup TIM_Input_Capture_Filer_Value 
  * @{
  */
#define IS_TIM_IC_FILTER(ICFILTER) ((ICFILTER) <= 0xF) 
/**
  * @}
  */ 

/** @defgroup TIM_External_Trigger_Filter 
  * @{
  */
#define IS_TIM_EXT_FILTER(EXTFILTER) ((EXTFILTER) <= 0xF)
/**
  * @}
  */

/** @defgroup TIM_OCReferenceClear 
  * @{
  */
#define TIM_OCReferenceClear_ETRF          ((uint32_t)0x00000008)
#define TIM_OCReferenceClear_OCREFCLR      ((uint32_t)0x00000000)
#define TIM_OCREFERENCECECLEAR_SOURCE(SOURCE) (((SOURCE) == TIM_OCReferenceClear_ETRF) || \
                                              ((SOURCE) == TIM_OCReferenceClear_OCREFCLR)) 
/**
  * @}
  */

/** @defgroup TIM_Remap 
  * @{
  */
#define TIM_REMAPC1         ((uint32_t)0x00000010)
#define TIM_REMAPC2         ((uint32_t)0x00000020)
#define TIM_REMAPC3         ((uint32_t)0x00000040)
#define TIM_REMAPC4         ((uint32_t)0x00000080)
#define IS_TIM_REMAP(TIM_Remap) (((TIM_Remap) == TIM_REMAPC1) || \
                                 ((TIM_Remap) == TIM_REMAPC2) || \
                                 ((TIM_Remap) == TIM_REMAPC3) || \
                                 ((TIM_Remap) == TIM_REMAPC4))  
/**
  * @}
  */

/** @defgroup TIM_External_Trigger_Selection 
  * @{
  */
#define TIM_EXTRIGPCLK            ((uint32_t)0x00000000)
#define TIM_EXTRIG1KHCLK          ((uint32_t)0x00000001)
#define TIM_EXTRIG32KHCLK         ((uint32_t)0x00000002)
#define TIM_EXTRIGTIMxCLK         ((uint32_t)0x00000003)
#define IS_TIM_EXTRIGCLK(EXTRIGCLK) (((EXTRIGCLK) == TIM_EXTRIGPCLK) || \
                                     ((EXTRIGCLK) == TIM_EXTRIG1KHCLK) || \
                                     ((EXTRIGCLK) == TIM_EXTRIG32KHCLK) || \
                                     ((EXTRIGCLK) == TIM_EXTRIGTIMxCLK))  
/**
  * @}
  */

/**
  * @}
  */
  
/** @defgroup TIM_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup TIM_Exported_Functions
  * @{
  */
/* TimeBase management ********************************************************/
void TIM_DeInit(TIM_TypeDef* TIMx);
void TIM_TimeBaseInit(TIM_TypeDef* TIMx, TIM_TimeBaseInitTypeDef* TIM_TimeBaseInitStruct);
void TIM_TimeBaseStructInit(TIM_TimeBaseInitTypeDef* TIM_TimeBaseInitStruct);
void TIM_PrescalerConfig(TIM_TypeDef* TIMx, uint32_t Prescaler, uint32_t TIM_PSCReloadMode);
void TIM_CounterModeConfig(TIM_TypeDef* TIMx, uint32_t TIM_CounterMode);
void TIM_SetCounter(TIM_TypeDef* TIMx, uint32_t Counter);
void TIM_SetAutoreload(TIM_TypeDef* TIMx, uint32_t Autoreload);
uint32_t TIM_GetCounter(TIM_TypeDef* TIMx);
uint32_t TIM_GetPrescaler(TIM_TypeDef* TIMx);
void TIM_UpdateDisableConfig(TIM_TypeDef* TIMx, FunctionalState NewState);
void TIM_UpdateRequestConfig(TIM_TypeDef* TIMx, uint32_t TIM_UpdateSource);
void TIM_ARRPreloadConfig(TIM_TypeDef* TIMx, FunctionalState NewState);
void TIM_SelectOnePulseMode(TIM_TypeDef* TIMx, uint32_t TIM_OPMode);
void TIM_Cmd(TIM_TypeDef* TIMx, FunctionalState NewState);
/* Output Compare management **************************************************/
void TIM_OC1Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct);
void TIM_OC2Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct);
void TIM_OC3Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct);
void TIM_OC4Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct);
void TIM_OCStructInit(TIM_OCInitTypeDef* TIM_OCInitStruct);
void TIM_SelectOCxM(TIM_TypeDef* TIMx, uint32_t TIM_Channel, uint32_t TIM_OCMode);
void TIM_SetCompare1(TIM_TypeDef* TIMx, uint32_t Compare1);
void TIM_SetCompare2(TIM_TypeDef* TIMx, uint32_t Compare2);
void TIM_SetCompare3(TIM_TypeDef* TIMx, uint32_t Compare3);
void TIM_SetCompare4(TIM_TypeDef* TIMx, uint32_t Compare4);
void TIM_ForcedOC1Config(TIM_TypeDef* TIMx, uint32_t TIM_ForcedAction);
void TIM_ForcedOC2Config(TIM_TypeDef* TIMx, uint32_t TIM_ForcedAction);
void TIM_ForcedOC3Config(TIM_TypeDef* TIMx, uint32_t TIM_ForcedAction);
void TIM_ForcedOC4Config(TIM_TypeDef* TIMx, uint32_t TIM_ForcedAction);
void TIM_OC1PreloadConfig(TIM_TypeDef* TIMx, uint32_t TIM_OCPreload);
void TIM_OC2PreloadConfig(TIM_TypeDef* TIMx, uint32_t TIM_OCPreload);
void TIM_OC3PreloadConfig(TIM_TypeDef* TIMx, uint32_t TIM_OCPreload);
void TIM_OC4PreloadConfig(TIM_TypeDef* TIMx, uint32_t TIM_OCPreload);
void TIM_OC1FastConfig(TIM_TypeDef* TIMx, uint32_t TIM_OCFast);
void TIM_OC2FastConfig(TIM_TypeDef* TIMx, uint32_t TIM_OCFast);
void TIM_OC3FastConfig(TIM_TypeDef* TIMx, uint32_t TIM_OCFast);
void TIM_OC4FastConfig(TIM_TypeDef* TIMx, uint32_t TIM_OCFast);
void TIM_OC1PolarityConfig(TIM_TypeDef* TIMx, uint32_t TIM_OCPolarity);
void TIM_OC2PolarityConfig(TIM_TypeDef* TIMx, uint32_t TIM_OCPolarity);
void TIM_OC3PolarityConfig(TIM_TypeDef* TIMx, uint32_t TIM_OCPolarity);
void TIM_OC4PolarityConfig(TIM_TypeDef* TIMx, uint32_t TIM_OCPolarity);
void TIM_CCxCmd(TIM_TypeDef* TIMx, uint32_t TIM_Channel, uint32_t TIM_CCx);
/* Input Capture management ***************************************************/
void TIM_ICInit(TIM_TypeDef* TIMx, TIM_ICInitTypeDef* TIM_ICInitStruct);
void TIM_ICStructInit(TIM_ICInitTypeDef* TIM_ICInitStruct);
void TIM_PWMIConfig(TIM_TypeDef* TIMx, TIM_ICInitTypeDef* TIM_ICInitStruct);
uint32_t TIM_GetCapture1(TIM_TypeDef* TIMx);
uint32_t TIM_GetCapture2(TIM_TypeDef* TIMx);
uint32_t TIM_GetCapture3(TIM_TypeDef* TIMx);
uint32_t TIM_GetCapture4(TIM_TypeDef* TIMx);
void TIM_SetIC1Prescaler(TIM_TypeDef* TIMx, uint32_t TIM_ICPSC);
void TIM_SetIC2Prescaler(TIM_TypeDef* TIMx, uint32_t TIM_ICPSC);
void TIM_SetIC3Prescaler(TIM_TypeDef* TIMx, uint32_t TIM_ICPSC);
void TIM_SetIC4Prescaler(TIM_TypeDef* TIMx, uint32_t TIM_ICPSC);
/* Interrupts and flags management functions **********************************/
void TIM_ITConfig(TIM_IT_TypeDef* TIMx_IT, uint32_t TIM_ITRPT, FunctionalState NewState);
ITStatus TIM_GetITStatus(TIM_IT_TypeDef* TIMx_IT, uint32_t TIM_ITRPT);
void TIM_ClearITPendingBit(TIM_IT_TypeDef* TIMx_IT, uint32_t TIM_ITRPT);
void TIM_GenerateEvent(TIM_TypeDef* TIMx, uint32_t TIM_EventSource);
/* Clocks management **********************************************************/
void TIM_InternalClockConfig(TIM_TypeDef* TIMx);
void TIM_ITRxExternalClockConfig(TIM_TypeDef* TIMx, uint32_t TIM_InputTriggerSource);
void TIM_TIxExternalClockConfig(TIM_TypeDef* TIMx, uint32_t TIM_TIxExternalCLKSource,
                                uint32_t TIM_ICPolarity, uint32_t ICFilter);
void TIM_ETRClockMode1Config(TIM_TypeDef* TIMx, uint32_t TIM_ExtTRGPrescaler, uint32_t TIM_ExtTRGPolarity,
                             uint32_t ExtTRGFilter);
void TIM_ETRClockMode2Config(TIM_TypeDef* TIMx, uint32_t TIM_ExtTRGPrescaler, 
                             uint32_t TIM_ExtTRGPolarity, uint32_t ExtTRGFilter);
/* Synchronization management *************************************************/
void TIM_SelectInputTrigger(TIM_TypeDef* TIMx, uint32_t TIM_InputTriggerSource);
void TIM_SelectOutputTrigger(TIM_TypeDef* TIMx, uint32_t TIM_TRGOSource);
void TIM_SelectSlaveMode(TIM_TypeDef* TIMx, uint32_t TIM_SlaveMode);
void TIM_SelectMasterSlaveMode(TIM_TypeDef* TIMx, uint32_t TIM_MasterSlaveMode);
void TIM_ETRConfig(TIM_TypeDef* TIMx, uint32_t TIM_ExtTRGPrescaler, uint32_t TIM_ExtTRGPolarity,
                   uint32_t ExtTRGFilter);
/* Specific interface management **********************************************/                   
void TIM_EncoderInterfaceConfig(TIM_TypeDef* TIMx, uint32_t TIM_EncoderMode,
                                uint32_t TIM_IC1Polarity, uint32_t TIM_IC2Polarity);
void TIM_SelectHallSensor(TIM_TypeDef* TIMx, FunctionalState NewState);
/* Specific remapping management **********************************************/
void TIM_ClockMaskConfig(TIM_TypeDef* TIMx, FunctionalState NewState);
void TIM_SelectExternalTriggerClock(TIM_TypeDef* TIMx, uint32_t TIM_EXTRIGCLK);
void TIM_RemapCmd(TIM_TypeDef* TIMx, uint32_t TIM_Remap, FunctionalState NewState);

/**
  * @}
  */
  
#ifdef __cplusplus
}
#endif

#endif /*__STM32W108XX_TIM_H */

/**
  * @}
  */ 

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

