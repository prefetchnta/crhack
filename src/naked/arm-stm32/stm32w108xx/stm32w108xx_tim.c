/**
  ******************************************************************************
  * @file    stm32w108xx_tim.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    30-November-2012
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the TIM peripheral:
  *            + TimeBase management
  *            + Output Compare management
  *            + Input Capture management
  *            + Interrupts management  
  *            + Clocks management
  *            + Synchronization management
  *            + Specific interface management
  *            + Specific remapping management      
  *              
  *  @verbatim  
 ===============================================================================
                    ##### How to use this driver #####
 ===============================================================================
    [..] This driver provides functions to configure and program the TIM 
         of all STM32W108xx devices These functions are split in 8 groups: 
         (#) TIM TimeBase management: this group includes all needed functions 
             to configure the TM Timebase unit:
             (++) Set/Get Prescaler.
             (++) Set/Get Autoreload.
             (++) Counter modes configuration.
             (++) Set Clock division.
             (++) Select the One Pulse mode.
             (++) Update Request Configuration.
             (++) Update Disable Configuration.
             (++) Auto-Preload Configuration.
             (++) Enable/Disable the counter.
  
         (#) TIM Output Compare management: this group includes all needed 
             functions to configure the Capture/Compare unit used in Output 
             compare mode: 
             (++) Configure each channel, independently, in Output Compare mode.
             (++) Select the output compare modes.
             (++) Select the Polarities of each channel.
             (++) Set/Get the Capture/Compare register values.
             (++) Select the Output Compare Fast mode. 
             (++) Select the Output Compare Forced mode.  
             (++) Output Compare-Preload Configuration. 
             (++) Clear Output Compare Reference.
             (++) Select the OCREF Clear signal.
             (++) Enable/Disable the Capture/Compare Channels.    
  
         (#) TIM Input Capture management: this group includes all needed 
             functions to configure the Capture/Compare unit used in 
             Input Capture mode:
             (++) Configure each channel in input capture mode.
             (++) Configure Channel1/2 in PWM Input mode.
             (++) Set the Input Capture Prescaler.
             (++) Get the Capture/Compare values.   
  
         (#) Interrupts and flags management functions: this group includes 
             all needed 
             functions to manage interrupts:
             (++) Enables or disables the specified TIM interrupts.
             (++) Configures the TIMx event to be generate by software.
             (++) Checks whether the TIM interrupt has occurred or not.
             (++) Clears the TIMx's interrupt pending bits.
             
         (#) TIM clocks management: this group includes all needed functions 
             to configure the clock controller unit:
             (++) Select internal/External clock.
             (++) Select the external clock mode: ETR(Mode1/Mode2), TIx or ITRx.
  
         (#) TIM synchronization management: this group includes all needed. 
             functions to configure the Synchronization unit:
             (++) Select Input Trigger.  
             (++) Select Output Trigger.  
             (++) Select Master Slave Mode. 
             (++) ETR Configuration when used as external trigger.   
  
         (#) TIM specific interface management, this group includes all 
             needed functions to use the specific TIM interface:
             (++) Encoder Interface Configuration.
             (++) Select Hall Sensor.   
  
         (#) TIM specific remapping management includes the Remapping 
             configuration of specific timers
  
    @endverbatim
  *    
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

/* Includes ------------------------------------------------------------------*/
#include "stm32w108xx_tim.h"

/** @addtogroup STM32W108xx_StdPeriph_Driver
  * @{
  */
/** @defgroup TIM 
  * @brief TIM driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* ---------------------- TIM registers bit mask ------------------------ */
#define SMCR_ETR_MASK               ((uint32_t)0x00FF) 
#define CCMR_OFFSET                 ((uint32_t)0x0018)
#define CCER_CCE_SET                ((uint32_t)0x0001)
#define TIM_ClockMask_Disable       ((uint32_t)0x0000)
#define TIM_ClockMask_Enable        ((uint32_t)0x0004)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

static void TI1_Config(TIM_TypeDef* TIMx, uint32_t TIM_ICPolarity, uint32_t TIM_ICSelection,
                       uint32_t TIM_ICFilter);
static void TI2_Config(TIM_TypeDef* TIMx, uint32_t TIM_ICPolarity, uint32_t TIM_ICSelection,
                       uint32_t TIM_ICFilter);
static void TI3_Config(TIM_TypeDef* TIMx, uint32_t TIM_ICPolarity, uint32_t TIM_ICSelection,
                       uint32_t TIM_ICFilter);
static void TI4_Config(TIM_TypeDef* TIMx, uint32_t TIM_ICPolarity, uint32_t TIM_ICSelection,
                       uint32_t TIM_ICFilter);
/* Private functions ---------------------------------------------------------*/

/** @defgroup TIM_Private_Functions
  * @{
  */

/** @defgroup TIM_Group1 TimeBase management functions
 *  @brief   TimeBase management functions 
 *
 @verbatim
 ===============================================================================
                 ##### TimeBase management functions #####
 ===============================================================================  
        *** TIM Driver: how to use it in Timing(Time base) Mode ***
 ===============================================================================
    [..] To use the Timer in Timing(Time base) mode, the following steps are 
         mandatory:
         (#) Fill the TIM_TimeBaseInitStruct with the desired parameters.
         (#) Call TIM_TimeBaseInit(TIMx, &TIM_TimeBaseInitStruct) to configure 
             the Time Base unit with the corresponding configuration.
         (#) Enable the NVIC if you need to generate the update interrupt. 
         (#) Enable the corresponding interrupt using the function 
             TIM_ITConfig(TIMx, TIM_IT_Update). 
         (#) Call the TIM_Cmd(ENABLE) function to enable the TIM counter.
    [..]
        (@) All other functions can be used seperatly to modify, if needed,
            a specific feature of the Timer. 

@endverbatim
  * @{
  */

/**
  * @brief  Deinitializes the TIMx peripheral registers to their default reset values.
  * @param  TIMx: where x can be 1 and 2 to select the TIM peripheral.
  * @retval None
  *   
  */
void TIM_DeInit(TIM_TypeDef* TIMx)
{
  
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx)); 
  
  TIMx->CR1 = 0x00000000;
  TIMx->CR2 = 0x00000000;
  TIMx->SMCR = 0x00000000;
  TIMx->EGR = 0x00000000;
  TIMx->CCMR2 = 0x00000000;     
  TIMx->CCER = 0x00000000;
  TIMx->CNT = 0x00000000;
  TIMx->PSC = 0x00000000;
  TIMx->ARR = 0x0000FFFF;
  TIMx->CCMR1 = 0x00000000;
  TIMx->CCR3 = 0x00000000;
  TIMx->CCR4 = 0x00000000;
  TIMx->CCR1 = 0x00000000;
  TIMx->CCR2 = 0x00000000;
  TIMx->OR = 0x00000000;
  
  if (TIMx == TIM1)
  {
    TIM1_IT->IER = 0x00000000; 
    TIM1_IT->ISR |= 0x0000005F; 
    TIM1_IT->IMR |= 0x00001E00; 
  }
  else
  {
    if (TIMx == TIM2)
    {
      TIM2_IT->IER = 0x00000000;  
      TIM2_IT->ISR |= 0x0000005F; 
      TIM2_IT->IMR |= 0x00001E00; 
    }
  }
}

/**
  * @brief  Initializes the TIMx Time Base Unit peripheral according to 
  *         the specified parameters in the TIM_TimeBaseInitStruct.
  * @param  TIMx: where x can be 1 and 2 to select the TIM 
  *         peripheral.
  * @param  TIM_TimeBaseInitStruct: pointer to a TIM_TimeBaseInitTypeDef
  *         structure that contains the configuration information for
  *         the specified TIM peripheral.
  * @retval None
  */
void TIM_TimeBaseInit(TIM_TypeDef* TIMx, TIM_TimeBaseInitTypeDef* TIM_TimeBaseInitStruct)
{
  uint32_t tmpcr1 = 0;

  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx)); 
  assert_param(IS_TIM_COUNTER_MODE(TIM_TimeBaseInitStruct->TIM_CounterMode));

  tmpcr1 = TIMx->CR1;  

    /* Select the Counter Mode */
    tmpcr1 &= (uint32_t)(~((uint32_t)(TIM_CR1_DIR | TIM_CR1_CMS)));
    tmpcr1 |= (uint32_t)TIM_TimeBaseInitStruct->TIM_CounterMode;
 
 

  TIMx->CR1 = tmpcr1;

  /* Set the Autoreload value */
  TIMx->ARR = TIM_TimeBaseInitStruct->TIM_Period ;
 
  /* Set the Prescaler value */
  TIMx->PSC = TIM_TimeBaseInitStruct->TIM_Prescaler;
    
  /* Generate an update event to reload the Prescaler and the Repetition counter
     values immediately */
  TIMx->EGR = TIM_PSCReloadMode_Immediate;           
}

/**
  * @brief  Fills each TIM_TimeBaseInitStruct member with its default value.
  * @param  TIM_TimeBaseInitStruct : pointer to a TIM_TimeBaseInitTypeDef
  *         structure which will be initialized.
  * @retval None
  */
void TIM_TimeBaseStructInit(TIM_TimeBaseInitTypeDef* TIM_TimeBaseInitStruct)
{
  /* Set the default configuration */
  TIM_TimeBaseInitStruct->TIM_Period = 0xFFFF;
  TIM_TimeBaseInitStruct->TIM_Prescaler = 0x0000;
  TIM_TimeBaseInitStruct->TIM_CounterMode = TIM_CounterMode_Up;
}

/**
  * @brief  Configures the TIMx Prescaler.
  * @param  TIMx: where x can be 1 and 2 to select the TIM peripheral.
  * @param  Prescaler: specifies the Prescaler Register value
  * @param  TIM_PSCReloadMode: specifies the TIM Prescaler Reload mode
  *   This parameter can be one of the following values:
  *     @arg TIM_PSCReloadMode_Update: The Prescaler is loaded at the update event.
  *     @arg TIM_PSCReloadMode_Immediate: The Prescaler is loaded immediatly.
  * @retval None
  */
void TIM_PrescalerConfig(TIM_TypeDef* TIMx, uint32_t Prescaler, uint32_t TIM_PSCReloadMode)
{
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_PRESCALER_RELOAD(TIM_PSCReloadMode));
  
  /* Set the Prescaler value */
  TIMx->PSC = Prescaler;
  /* Set or reset the UG Bit */
  TIMx->EGR = TIM_PSCReloadMode;
}

/**
  * @brief  Specifies the TIMx Counter Mode to be used.
  * @param  TIMx: where x can be 1 or 2 to select the TIM peripheral.
  * @param  TIM_CounterMode: specifies the Counter Mode to be used
  *   This parameter can be one of the following values:
  *     @arg TIM_CounterMode_Up: TIM Up Counting Mode
  *     @arg TIM_CounterMode_Down: TIM Down Counting Mode
  *     @arg TIM_CounterMode_CenterAligned1: TIM Center Aligned Mode1
  *     @arg TIM_CounterMode_CenterAligned2: TIM Center Aligned Mode2
  *     @arg TIM_CounterMode_CenterAligned3: TIM Center Aligned Mode3
  * @retval None
  */
void TIM_CounterModeConfig(TIM_TypeDef* TIMx, uint32_t TIM_CounterMode)
{
  uint32_t tmpcr1 = 0;
  
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_COUNTER_MODE(TIM_CounterMode));
  
  tmpcr1 = TIMx->CR1;
  /* Reset the CMS and DIR Bits */
  tmpcr1 &= (uint32_t)(~((uint32_t)(TIM_CR1_DIR | TIM_CR1_CMS)));
  /* Set the Counter Mode */
  tmpcr1 |= TIM_CounterMode;
  /* Write to TIMx CR1 register */
  TIMx->CR1 = tmpcr1;
}

/**
  * @brief  Sets the TIMx Counter Register value
  * @param  TIMx: where x can be 1 and 2 to select the TIM 
  *          peripheral.
  * @param  Counter: specifies the Counter register new value.
  * @retval None
  */
void TIM_SetCounter(TIM_TypeDef* TIMx, uint32_t Counter)
{
  /* Check the parameters */
   assert_param(IS_TIM_ALL_PERIPH(TIMx));
   
  /* Set the Counter Register value */
  TIMx->CNT = Counter;
}

/**
  * @brief  Sets the TIMx Autoreload Register value
  * @param  TIMx: where x can be 1 and 2 to select the TIM peripheral.
  * @param  Autoreload: specifies the Autoreload register new value.
  * @retval None
  */
void TIM_SetAutoreload(TIM_TypeDef* TIMx, uint32_t Autoreload)
{
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  
  /* Set the Autoreload Register value */
  TIMx->ARR = Autoreload;
}

/**
  * @brief  Gets the TIMx Counter value.
  * @param  TIMx: where x can be 1 and 2 to select the TIM 
  *         peripheral.
  * @retval Counter Register value.
  */
uint32_t TIM_GetCounter(TIM_TypeDef* TIMx)
{
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  
  /* Get the Counter Register value */
  return TIMx->CNT;
}

/**
  * @brief  Gets the TIMx Prescaler value.
  * @param  TIMx: where x can be 1 and 2 to select the TIM 
  *         peripheral.
  * @retval Prescaler Register value.
  */
uint32_t TIM_GetPrescaler(TIM_TypeDef* TIMx)
{
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  
  /* Get the Prescaler Register value */
  return TIMx->PSC;
}

/**
  * @brief  Enables or Disables the TIMx Update event.
  * @param  TIMx: where x can be 1 and 2 to select the TIM 
  *         peripheral.
  * @param  NewState: new state of the TIMx UDIS bit
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void TIM_UpdateDisableConfig(TIM_TypeDef* TIMx, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Set the Update Disable Bit */
    TIMx->CR1 |= TIM_CR1_UDIS;
  }
  else
  {
    /* Reset the Update Disable Bit */
    TIMx->CR1 &= (uint32_t)~((uint32_t)TIM_CR1_UDIS);
  }
}

/**
  * @brief  Configures the TIMx Update Request Interrupt source.
  * @param  TIMx: where x can be 1 and 2 to select the TIM 
  *         peripheral.
  * @param  TIM_UpdateSource: specifies the Update source.
  *   This parameter can be one of the following values:
  *     @arg TIM_UpdateSource_Regular: Source of update is the counter overflow/underflow
                                       or the setting of UG bit, or an update generation
                                       through the slave mode controller.
  *     @arg TIM_UpdateSource_Global: Source of update is counter overflow/underflow.
  * @retval None
  */
void TIM_UpdateRequestConfig(TIM_TypeDef* TIMx, uint32_t TIM_UpdateSource)
{
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_UPDATE_SOURCE(TIM_UpdateSource));
  
  if (TIM_UpdateSource != TIM_UpdateSource_Global)
  {
    /* Set the URS Bit */
    TIMx->CR1 |= TIM_CR1_URS;
  }
  else
  {
    /* Reset the URS Bit */
    TIMx->CR1 &= (uint32_t)~((uint32_t)TIM_CR1_URS);
  }
}

/**
  * @brief  Enables or disables TIMx peripheral Preload register on ARR.
  * @param  TIMx: where x can be  1 and 2 to select the TIM 
  *         peripheral.
  * @param  NewState: new state of the TIMx peripheral Preload register
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void TIM_ARRPreloadConfig(TIM_TypeDef* TIMx, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Set the ARR Preload Bit */
    TIMx->CR1 |= TIM_CR1_ARPE;
  }
  else
  {
    /* Reset the ARR Preload Bit */
    TIMx->CR1 &= (uint32_t)~((uint32_t)TIM_CR1_ARPE);
  }
}

/**
  * @brief  Selects the TIMx's One Pulse Mode.
  * @param  TIMx: where x can be 1 and 2 to select the TIM 
  *         peripheral.
  * @param  TIM_OPMode: specifies the OPM Mode to be used.
  *   This parameter can be one of the following values:
  *     @arg TIM_OPMode_Single
  *     @arg TIM_OPMode_Repetitive
  * @retval None
  */
void TIM_SelectOnePulseMode(TIM_TypeDef* TIMx, uint32_t TIM_OPMode)
{
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_OPM_MODE(TIM_OPMode));
  
  /* Reset the OPM Bit */
  TIMx->CR1 &= (uint32_t)~((uint32_t)TIM_CR1_OPM);
  /* Configure the OPM Mode */
  TIMx->CR1 |= TIM_OPMode;
}


/**
  * @brief  Enables or disables the specified TIM peripheral.
  * @param  TIMx: where x can be 1 and 2 and 17to select the TIMx
  *         peripheral.
  * @param  NewState: new state of the TIMx peripheral.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void TIM_Cmd(TIM_TypeDef* TIMx, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx)); 
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable the TIM Counter */
    TIMx->CR1 |= TIM_CR1_CEN;
  }
  else
  {
    /* Disable the TIM Counter */
    TIMx->CR1 &= (uint32_t)(~((uint32_t)TIM_CR1_CEN));
  }
}
   
/**
  * @}
  */

/** @defgroup TIM_Group2 Output Compare management functions
 *  @brief    Output Compare management functions 
 *
@verbatim
 ===============================================================================
                ##### Output Compare management functions #####
 ===============================================================================
        *** TIM Driver: how to use it in Output Compare Mode ***
 ===============================================================================
    [..] To use the Timer in Output Compare mode, the following steps are mandatory:
         (#) Configure the TIM pins by configuring the corresponding GPIO pins
         (#) Configure the Time base unit as described in the first part of this 
             driver, if needed, else the Timer will run with the default 
             configuration:
             (++) Autoreload value = 0xFFFF.
             (++) Prescaler value = 0x0000.
             (++) Counter mode = Up counting.
         (#) Fill the TIM_OCInitStruct with the desired parameters including:
             (++) The TIM Output Compare mode: TIM_OCMode.
             (++) TIM Output State: TIM_OutputState.
             (++) TIM Pulse value: TIM_Pulse.
             (++) TIM Output Compare Polarity : TIM_OCPolarity.
         (#) Call TIM_OCxInit(TIMx, &TIM_OCInitStruct) to configure the desired 
             channel with the corresponding configuration.
         (#) Call the TIM_Cmd(ENABLE) function to enable the TIM counter.
    [..]
        (@) All other functions can be used separately to modify, if needed,
          a specific feature of the Timer.
        (@) In case of PWM mode, this function is mandatory:
            TIM_OCxPreloadConfig(TIMx, TIM_OCPreload_ENABLE).
        (@) If the corresponding interrupt are needed, the user should:
            (#@) Enable the NVIC to use the TIM interrupts.
            (#@) Enable the corresponding interrupt using the function
                 TIM_ITConfig(TIMx, TIM_IT_CCx).

@endverbatim
  * @{
  */

/**
  * @brief  Initializes the TIMx Channel1 according to the specified
  *         parameters in the TIM_OCInitStruct.
  * @param  TIMx: where x can be 1 and 2 to select the TIM peripheral.
  * @param  TIM_OCInitStruct: pointer to a TIM_OCInitTypeDef structure
  *         that contains the configuration information for the specified TIM 
  *         peripheral.
  * @retval None
  */
void TIM_OC1Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct)
{
  uint32_t tmpccmrx = 0, tmpccer = 0, tmpcr2 = 0;
   
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_OC_MODE(TIM_OCInitStruct->TIM_OCMode));
  assert_param(IS_TIM_OUTPUT_STATE(TIM_OCInitStruct->TIM_OutputState));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCInitStruct->TIM_OCPolarity));   
 /* Disable the Channel 1: Reset the CC1E Bit */
  TIMx->CCER &= (uint32_t)(~(uint32_t)TIM_CCER_CC1E);
  /* Get the TIMx CCER register value */
  tmpccer = TIMx->CCER;
  /* Get the TIMx CR2 register value */
  tmpcr2 =  TIMx->CR2;
  
  /* Get the TIMx CCMR1 register value */
  tmpccmrx = TIMx->CCMR1;
    
  /* Reset the Output Compare Mode Bits */
  tmpccmrx &= (uint32_t)(~((uint32_t)TIM_CCMR1_OC1M));
  tmpccmrx &= (uint32_t)(~((uint32_t)TIM_CCMR1_CC1S));

  /* Select the Output Compare Mode */
  tmpccmrx |= TIM_OCInitStruct->TIM_OCMode;
  
  /* Reset the Output Polarity level */
  tmpccer &= (uint32_t)(~((uint32_t)TIM_CCER_CC1P));
  /* Set the Output Compare Polarity */
  tmpccer |= TIM_OCInitStruct->TIM_OCPolarity;
  
  /* Set the Output State */
  tmpccer |= TIM_OCInitStruct->TIM_OutputState;
    
  /* Write to TIMx CR2 */
  TIMx->CR2 = tmpcr2;
  
  /* Write to TIMx CCMR1 */
  TIMx->CCMR1 = tmpccmrx;

  /* Set the Capture Compare Register value */
  TIMx->CCR1 = TIM_OCInitStruct->TIM_Pulse; 
 
  /* Write to TIMx CCER */
  TIMx->CCER = tmpccer;
}

/**
  * @brief  Initializes the TIMx Channel2 according to the specified
  *         parameters in the TIM_OCInitStruct.
  * @param  TIMx: where x can be 1 or 2 to select the TIM peripheral.
  * @param  TIM_OCInitStruct: pointer to a TIM_OCInitTypeDef structure
  *         that contains the configuration information for the specified TIM 
  *         peripheral.
  * @retval None
  */
void TIM_OC2Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct)
{
  uint32_t tmpccmrx = 0, tmpccer = 0, tmpcr2 = 0;
   
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx)); 
  assert_param(IS_TIM_OC_MODE(TIM_OCInitStruct->TIM_OCMode));
  assert_param(IS_TIM_OUTPUT_STATE(TIM_OCInitStruct->TIM_OutputState));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCInitStruct->TIM_OCPolarity));   
   /* Disable the Channel 2: Reset the CC2E Bit */
  TIMx->CCER &= (uint32_t)(~((uint32_t)TIM_CCER_CC2E));
  
  /* Get the TIMx CCER register value */  
  tmpccer = TIMx->CCER;
  /* Get the TIMx CR2 register value */
  tmpcr2 =  TIMx->CR2;
  
  /* Get the TIMx CCMR1 register value */
  tmpccmrx = TIMx->CCMR1;
    
  /* Reset the Output Compare mode and Capture/Compare selection Bits */
  tmpccmrx &= (uint32_t)(~((uint32_t)TIM_CCMR1_OC2M));
  tmpccmrx &= (uint32_t)(~((uint32_t)TIM_CCMR1_CC2S));
  
  /* Select the Output Compare Mode */
  tmpccmrx |= (uint32_t)(TIM_OCInitStruct->TIM_OCMode << 8);
  
  /* Reset the Output Polarity level */
  tmpccer &= (uint32_t)(~((uint32_t)TIM_CCER_CC2P));
  /* Set the Output Compare Polarity */
  tmpccer |= (uint32_t)(TIM_OCInitStruct->TIM_OCPolarity << 4);
  
  /* Set the Output State */
  tmpccer |= (uint32_t)(TIM_OCInitStruct->TIM_OutputState << 4);
    
  /* Write to TIMx CR2 */
  TIMx->CR2 = tmpcr2;
  
  /* Write to TIMx CCMR1 */
  TIMx->CCMR1 = tmpccmrx;

  /* Set the Capture Compare Register value */
  TIMx->CCR2 = TIM_OCInitStruct->TIM_Pulse;
  
  /* Write to TIMx CCER */
  TIMx->CCER = tmpccer;
}

/**
  * @brief  Initializes the TIMx Channel3 according to the specified
  *         parameters in the TIM_OCInitStruct.
  * @param  TIMx: where x can be 1 or 2 to select the TIM peripheral.
  * @param  TIM_OCInitStruct: pointer to a TIM_OCInitTypeDef structure
  *         that contains the configuration information for the specified TIM 
  *         peripheral.
  * @retval None
  */
void TIM_OC3Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct)
{
  uint32_t tmpccmrx = 0, tmpccer = 0, tmpcr2 = 0;
   
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx)); 
  assert_param(IS_TIM_OC_MODE(TIM_OCInitStruct->TIM_OCMode));
  assert_param(IS_TIM_OUTPUT_STATE(TIM_OCInitStruct->TIM_OutputState));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCInitStruct->TIM_OCPolarity));   
  /* Disable the Channel 2: Reset the CC2E Bit */
  TIMx->CCER &= (uint32_t)(~((uint32_t)TIM_CCER_CC3E));
  
  /* Get the TIMx CCER register value */
  tmpccer = TIMx->CCER;
  /* Get the TIMx CR2 register value */
  tmpcr2 =  TIMx->CR2;
  
  /* Get the TIMx CCMR2 register value */
  tmpccmrx = TIMx->CCMR2;
    
  /* Reset the Output Compare mode and Capture/Compare selection Bits */
  tmpccmrx &= (uint32_t)(~((uint32_t)TIM_CCMR2_OC3M));
  tmpccmrx &= (uint32_t)(~((uint32_t)TIM_CCMR2_CC3S));  
  /* Select the Output Compare Mode */
  tmpccmrx |= TIM_OCInitStruct->TIM_OCMode;
  
  /* Reset the Output Polarity level */
  tmpccer &= (uint32_t)(~((uint32_t)TIM_CCER_CC3P));
  /* Set the Output Compare Polarity */
  tmpccer |= (uint32_t)(TIM_OCInitStruct->TIM_OCPolarity << 8);
  
  /* Set the Output State */
  tmpccer |= (uint32_t)(TIM_OCInitStruct->TIM_OutputState << 8);
    
  /* Write to TIMx CR2 */
  TIMx->CR2 = tmpcr2;
  
  /* Write to TIMx CCMR2 */
  TIMx->CCMR2 = tmpccmrx;

  /* Set the Capture Compare Register value */
  TIMx->CCR3 = TIM_OCInitStruct->TIM_Pulse;
  
  /* Write to TIMx CCER */
  TIMx->CCER = tmpccer;
}

/**
  * @brief  Initializes the TIMx Channel4 according to the specified
  *         parameters in the TIM_OCInitStruct.
  * @param  TIMx: where x can be 1 or 2 to select the TIM peripheral.
  * @param  TIM_OCInitStruct: pointer to a TIM_OCInitTypeDef structure
  *         that contains the configuration information for the specified TIM 
  *         peripheral.
  * @retval None
  */
void TIM_OC4Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct)
{
  uint32_t tmpccmrx = 0, tmpccer = 0, tmpcr2 = 0;
   
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx)); 
  assert_param(IS_TIM_OC_MODE(TIM_OCInitStruct->TIM_OCMode));
  assert_param(IS_TIM_OUTPUT_STATE(TIM_OCInitStruct->TIM_OutputState));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCInitStruct->TIM_OCPolarity));   
  /* Disable the Channel 2: Reset the CC4E Bit */
  TIMx->CCER &= (uint32_t)(~((uint32_t)TIM_CCER_CC4E));
  
  /* Get the TIMx CCER register value */
  tmpccer = TIMx->CCER;
  /* Get the TIMx CR2 register value */
  tmpcr2 =  TIMx->CR2;
  
  /* Get the TIMx CCMR2 register value */
  tmpccmrx = TIMx->CCMR2;
    
  /* Reset the Output Compare mode and Capture/Compare selection Bits */
  tmpccmrx &= (uint32_t)(~((uint32_t)TIM_CCMR2_OC4M));
  tmpccmrx &= (uint32_t)(~((uint32_t)TIM_CCMR2_CC4S));
  
  /* Select the Output Compare Mode */
  tmpccmrx |= (uint32_t)(TIM_OCInitStruct->TIM_OCMode << 8);
  
  /* Reset the Output Polarity level */
  tmpccer &= (uint32_t)(~((uint32_t)TIM_CCER_CC4P));
  /* Set the Output Compare Polarity */
  tmpccer |= (uint32_t)(TIM_OCInitStruct->TIM_OCPolarity << 12);
  
  /* Set the Output State */
  tmpccer |= (uint32_t)(TIM_OCInitStruct->TIM_OutputState << 12);
    
  /* Write to TIMx CR2 */
  TIMx->CR2 = tmpcr2;
  
  /* Write to TIMx CCMR2 */  
  TIMx->CCMR2 = tmpccmrx;

  /* Set the Capture Compare Register value */
  TIMx->CCR4 = TIM_OCInitStruct->TIM_Pulse;
  
  /* Write to TIMx CCER */
  TIMx->CCER = tmpccer;
}

/**
  * @brief  Fills each TIM_OCInitStruct member with its default value.
  * @param  TIM_OCInitStruct : pointer to a TIM_OCInitTypeDef structure which will
  *         be initialized.
  * @retval None
  */
void TIM_OCStructInit(TIM_OCInitTypeDef* TIM_OCInitStruct)
{
  /* Set the default configuration */
  TIM_OCInitStruct->TIM_OCMode = TIM_OCMode_Timing;
  TIM_OCInitStruct->TIM_OutputState = TIM_OutputState_Disable;
  TIM_OCInitStruct->TIM_Pulse = 0x0000000;
  TIM_OCInitStruct->TIM_OCPolarity = TIM_OCPolarity_High;
}

/**
  * @brief  Selects the TIM Output Compare Mode.
  * @note   This function disables the selected channel before changing the Output
  *         Compare Mode.
  *         User has to enable this channel using TIM_CCxCmd and TIM_CCxNCmd functions.
  * @param  TIMx: where x can be 1 or 2 to select the TIM peripheral.
  * @param  TIM_Channel: specifies the TIM Channel
  *   This parameter can be one of the following values:
  *     @arg TIM_Channel_1: TIM Channel 1
  *     @arg TIM_Channel_2: TIM Channel 2
  *     @arg TIM_Channel_3: TIM Channel 3
  *     @arg TIM_Channel_4: TIM Channel 4
  * @param  TIM_OCMode: specifies the TIM Output Compare Mode.
  *   This parameter can be one of the following values:
  *     @arg TIM_OCMode_Timing
  *     @arg TIM_OCMode_Active
  *     @arg TIM_OCMode_Toggle
  *     @arg TIM_OCMode_PWM1
  *     @arg TIM_OCMode_PWM2
  *     @arg TIM_ForcedAction_Active
  *     @arg TIM_ForcedAction_InActive
  * @retval None
  */
void TIM_SelectOCxM(TIM_TypeDef* TIMx, uint32_t TIM_Channel, uint32_t TIM_OCMode)
{
  uint32_t tmp = 0;
  uint32_t tmp1 = 0;

  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx)); 
  assert_param(IS_TIM_OCM(TIM_OCMode));
  
  tmp = (uint32_t) TIMx;
  tmp += CCMR_OFFSET;

  tmp1 = CCER_CCE_SET << (uint32_t)TIM_Channel;

  /* Disable the Channel: Reset the CCxE Bit */
  TIMx->CCER &= (uint32_t) ~tmp1;

  if((TIM_Channel == TIM_Channel_1) ||(TIM_Channel == TIM_Channel_3))
  {
    tmp += (TIM_Channel>>1);

    /* Reset the OCxM bits in the CCMRx register */
    *(__IO uint32_t *) tmp &= (uint32_t)~((uint32_t)TIM_CCMR1_OC1M);
   
    /* Configure the OCxM bits in the CCMRx register */
    *(__IO uint32_t *) tmp |= TIM_OCMode;
  }
  else
  {
    tmp += (uint32_t)(TIM_Channel - (uint32_t)4)>> (uint32_t)1;

    /* Reset the OCxM bits in the CCMRx register */
    *(__IO uint32_t *) tmp &= (uint32_t)~((uint32_t)TIM_CCMR1_OC2M);
    
    /* Configure the OCxM bits in the CCMRx register */
    *(__IO uint32_t *) tmp |= (uint32_t)(TIM_OCMode << 8);
  }
}

/**
  * @brief  Sets the TIMx Capture Compare1 Register value
  * @param  TIMx: where x can be 1 or 2 to select the TIM peripheral.
  * @param  Compare1: specifies the Capture Compare1 register new value.
  * @retval None
  */
void TIM_SetCompare1(TIM_TypeDef* TIMx, uint32_t Compare1)
{
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  
  /* Set the Capture Compare1 Register value */
  TIMx->CCR1 = Compare1;
}

/**
  * @brief  Sets the TIMx Capture Compare2 Register value
  * @param  TIMx: where x can be 1 or 2 to select the TIM peripheral.
  * @param  Compare2: specifies the Capture Compare2 register new value.
  * @retval None
  */
void TIM_SetCompare2(TIM_TypeDef* TIMx, uint32_t Compare2)
{
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  
  /* Set the Capture Compare2 Register value */
  TIMx->CCR2 = Compare2;
}

/**
  * @brief  Sets the TIMx Capture Compare3 Register value
  * @param  TIMx: where x can be 1 or 2 to select the TIM peripheral.
  * @param  Compare3: specifies the Capture Compare3 register new value.
  * @retval None
  */
void TIM_SetCompare3(TIM_TypeDef* TIMx, uint32_t Compare3)
{
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  
  /* Set the Capture Compare3 Register value */
  TIMx->CCR3 = Compare3;
}

/**
  * @brief  Sets the TIMx Capture Compare4 Register value
  * @param  TIMx: where x can be 1 or 2 to select the TIM peripheral.
  * @param  Compare4: specifies the Capture Compare4 register new value.
  * @retval None
  */
void TIM_SetCompare4(TIM_TypeDef* TIMx, uint32_t Compare4)
{
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  
  /* Set the Capture Compare4 Register value */
  TIMx->CCR4 = Compare4;
}

/**
  * @brief  Forces the TIMx output 1 waveform to active or inactive level.
  * @param  TIMx: where x can be 1 or 2 to select the TIM peripheral.
  * @param  TIM_ForcedAction: specifies the forced Action to be set to the output waveform.
  *   This parameter can be one of the following values:
  *     @arg TIM_ForcedAction_Active: Force active level on OC1REF
  *     @arg TIM_ForcedAction_InActive: Force inactive level on OC1REF.
  * @retval None
  */
void TIM_ForcedOC1Config(TIM_TypeDef* TIMx, uint32_t TIM_ForcedAction)
{
  uint32_t tmpccmr1 = 0;
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_FORCED_ACTION(TIM_ForcedAction));
  tmpccmr1 = TIMx->CCMR1;
  /* Reset the OC1M Bits */
  tmpccmr1 &= (uint32_t)~((uint32_t)TIM_CCMR1_OC1M);
  /* Configure The Forced output Mode */
  tmpccmr1 |= TIM_ForcedAction;
  /* Write to TIMx CCMR1 register */
  TIMx->CCMR1 = tmpccmr1;
}
 
/**
  * @brief  Forces the TIMx output 2 waveform to active or inactive level.
  * @param  TIMx: where x can be 1 or 2 to select the TIM 
  *   peripheral.
  * @param  TIM_ForcedAction: specifies the forced Action to be set to the output waveform.
  *   This parameter can be one of the following values:
  *     @arg TIM_ForcedAction_Active: Force active level on OC2REF
  *     @arg TIM_ForcedAction_InActive: Force inactive level on OC2REF.
  * @retval None
  */
void TIM_ForcedOC2Config(TIM_TypeDef* TIMx, uint32_t TIM_ForcedAction)
{
  uint32_t tmpccmr1 = 0;
  
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_FORCED_ACTION(TIM_ForcedAction));
  
  tmpccmr1 = TIMx->CCMR1;
  /* Reset the OC2M Bits */
  tmpccmr1 &= (uint32_t)~((uint32_t)TIM_CCMR1_OC2M);
  /* Configure The Forced output Mode */
  tmpccmr1 |= (uint32_t)(TIM_ForcedAction << 8);
  /* Write to TIMx CCMR1 register */
  TIMx->CCMR1 = tmpccmr1;
}

/**
  * @brief  Forces the TIMx output 3 waveform to active or inactive level.
  * @param  TIMx: where x can be 1 or 2 to select the TIM peripheral.
  * @param  TIM_ForcedAction: specifies the forced Action to be set to the output waveform.
  *   This parameter can be one of the following values:
  *     @arg TIM_ForcedAction_Active: Force active level on OC3REF
  *     @arg TIM_ForcedAction_InActive: Force inactive level on OC3REF.
  * @retval None
  */
void TIM_ForcedOC3Config(TIM_TypeDef* TIMx, uint32_t TIM_ForcedAction)
{
  uint32_t tmpccmr2 = 0;
  
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_FORCED_ACTION(TIM_ForcedAction));
  
  tmpccmr2 = TIMx->CCMR2;
  /* Reset the OC1M Bits */
  tmpccmr2 &= (uint32_t)~((uint32_t)TIM_CCMR2_OC3M);
  /* Configure The Forced output Mode */
  tmpccmr2 |= TIM_ForcedAction;
  /* Write to TIMx CCMR2 register */
  TIMx->CCMR2 = tmpccmr2;
}

/**
  * @brief  Forces the TIMx output 4 waveform to active or inactive level.
  * @param  TIMx: where x can be 1 or 2 to select the TIM peripheral.
  * @param  TIM_ForcedAction: specifies the forced Action to be set to the output waveform.
  *   This parameter can be one of the following values:
  *     @arg TIM_ForcedAction_Active: Force active level on OC4REF
  *     @arg TIM_ForcedAction_InActive: Force inactive level on OC4REF.
  * @retval None
  */
void TIM_ForcedOC4Config(TIM_TypeDef* TIMx, uint32_t TIM_ForcedAction)
{
  uint32_t tmpccmr2 = 0;
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_FORCED_ACTION(TIM_ForcedAction));
  
  tmpccmr2 = TIMx->CCMR2;
  /* Reset the OC2M Bits */
  tmpccmr2 &= (uint32_t)~((uint32_t)TIM_CCMR2_OC4M);
  /* Configure The Forced output Mode */
  tmpccmr2 |= (uint32_t)(TIM_ForcedAction << 8);
  /* Write to TIMx CCMR2 register */
  TIMx->CCMR2 = tmpccmr2;
}

/**
  * @brief  Enables or disables the TIMx peripheral Preload register on CCR1.
  * @param  TIMx: where x can be 1 or 2 to select the TIM peripheral.
  * @param  TIM_OCPreload: new state of the TIMx peripheral Preload register
  *   This parameter can be one of the following values:
  *     @arg TIM_OCPreload_Enable
  *     @arg TIM_OCPreload_Disable
  * @retval None
  */
void TIM_OC1PreloadConfig(TIM_TypeDef* TIMx, uint32_t TIM_OCPreload)
{
  uint32_t tmpccmr1 = 0;
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_OCPRELOAD_STATE(TIM_OCPreload));
  
  tmpccmr1 = TIMx->CCMR1;
  /* Reset the OC1PE Bit */
  tmpccmr1 &= (uint32_t)~((uint32_t)TIM_CCMR1_OC1PE);
  /* Enable or Disable the Output Compare Preload feature */
  tmpccmr1 |= TIM_OCPreload;
  /* Write to TIMx CCMR1 register */
  TIMx->CCMR1 = tmpccmr1;
}

/**
  * @brief  Enables or disables the TIMx peripheral Preload register on CCR2.
  * @param  TIMx: where x can be 1 or 2 to select the TIM peripheral.
  * @param  TIM_OCPreload: new state of the TIMx peripheral Preload register
  *   This parameter can be one of the following values:
  *     @arg TIM_OCPreload_Enable
  *     @arg TIM_OCPreload_Disable
  * @retval None
  */
void TIM_OC2PreloadConfig(TIM_TypeDef* TIMx, uint32_t TIM_OCPreload)
{
  uint32_t tmpccmr1 = 0;
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_OCPRELOAD_STATE(TIM_OCPreload));
  
  tmpccmr1 = TIMx->CCMR1;
  /* Reset the OC2PE Bit */
  tmpccmr1 &= (uint32_t)~((uint32_t)TIM_CCMR1_OC2PE);
  /* Enable or Disable the Output Compare Preload feature */
  tmpccmr1 |= (uint32_t)(TIM_OCPreload << 8);
  /* Write to TIMx CCMR1 register */
  TIMx->CCMR1 = tmpccmr1;
}

/**
  * @brief  Enables or disables the TIMx peripheral Preload register on CCR3.
  * @param  TIMx: where x can be 1 or 2 to select the TIM peripheral.
  * @param  TIM_OCPreload: new state of the TIMx peripheral Preload register
  *   This parameter can be one of the following values:
  *     @arg TIM_OCPreload_Enable
  *     @arg TIM_OCPreload_Disable
  * @retval None
  */
void TIM_OC3PreloadConfig(TIM_TypeDef* TIMx, uint32_t TIM_OCPreload)
{
  uint32_t tmpccmr2 = 0;
  
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_OCPRELOAD_STATE(TIM_OCPreload));
  
  tmpccmr2 = TIMx->CCMR2;
  /* Reset the OC3PE Bit */
  tmpccmr2 &= (uint32_t)~((uint32_t)TIM_CCMR2_OC3PE);
  /* Enable or Disable the Output Compare Preload feature */
  tmpccmr2 |= TIM_OCPreload;
  /* Write to TIMx CCMR2 register */
  TIMx->CCMR2 = tmpccmr2;
}

/**
  * @brief  Enables or disables the TIMx peripheral Preload register on CCR4.
  * @param  TIMx: where x can be 1 or 2 to select the TIM peripheral.
  * @param  TIM_OCPreload: new state of the TIMx peripheral Preload register
  *   This parameter can be one of the following values:
  *     @arg TIM_OCPreload_Enable
  *     @arg TIM_OCPreload_Disable
  * @retval None
  */
void TIM_OC4PreloadConfig(TIM_TypeDef* TIMx, uint32_t TIM_OCPreload)
{
  uint32_t tmpccmr2 = 0;
  
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_OCPRELOAD_STATE(TIM_OCPreload));
  
  tmpccmr2 = TIMx->CCMR2;
  /* Reset the OC4PE Bit */
  tmpccmr2 &= (uint32_t)~((uint32_t)TIM_CCMR2_OC4PE);
  /* Enable or Disable the Output Compare Preload feature */
  tmpccmr2 |= (uint32_t)(TIM_OCPreload << 8);
  /* Write to TIMx CCMR2 register */
  TIMx->CCMR2 = tmpccmr2;
}

/**
  * @brief  Configures the TIMx Output Compare 1 Fast feature.
  * @param  TIMx: where x can be 1 or 2 to select the TIM peripheral.
  * @param  TIM_OCFast: new state of the Output Compare Fast Enable Bit.
  *   This parameter can be one of the following values:
  *     @arg TIM_OCFast_Enable: TIM output compare fast enable
  *     @arg TIM_OCFast_Disable: TIM output compare fast disable
  * @retval None
  */
void TIM_OC1FastConfig(TIM_TypeDef* TIMx, uint32_t TIM_OCFast)
{
  uint32_t tmpccmr1 = 0;
  
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_OCFAST_STATE(TIM_OCFast));
  
  /* Get the TIMx CCMR1 register value */
  tmpccmr1 = TIMx->CCMR1;
  /* Reset the OC1FE Bit */
  tmpccmr1 &= (uint32_t)~((uint32_t)TIM_CCMR1_OC1FE);
  /* Enable or Disable the Output Compare Fast Bit */
  tmpccmr1 |= TIM_OCFast;
  /* Write to TIMx CCMR1 */
  TIMx->CCMR1 = tmpccmr1;
}

/**
  * @brief  Configures the TIMx Output Compare 2 Fast feature.
  * @param  TIMx: where x can be 1 or 2 to select the TIM peripheral.
  * @param  TIM_OCFast: new state of the Output Compare Fast Enable Bit.
  *   This parameter can be one of the following values:
  *     @arg TIM_OCFast_Enable: TIM output compare fast enable
  *     @arg TIM_OCFast_Disable: TIM output compare fast disable
  * @retval None
  */
void TIM_OC2FastConfig(TIM_TypeDef* TIMx, uint32_t TIM_OCFast)
{
  uint32_t tmpccmr1 = 0;
  
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_OCFAST_STATE(TIM_OCFast));
  
  /* Get the TIMx CCMR1 register value */
  tmpccmr1 = TIMx->CCMR1;
  /* Reset the OC2FE Bit */
  tmpccmr1 &= (uint32_t)~((uint32_t)TIM_CCMR1_OC2FE);
  /* Enable or Disable the Output Compare Fast Bit */
  tmpccmr1 |= (uint32_t)(TIM_OCFast << 8);
  /* Write to TIMx CCMR1 */
  TIMx->CCMR1 = tmpccmr1;
}

/**
  * @brief  Configures the TIMx Output Compare 3 Fast feature.
  * @param  TIMx: where x can be 1 or 2 to select the TIM peripheral.
  * @param  TIM_OCFast: new state of the Output Compare Fast Enable Bit.
  *   This parameter can be one of the following values:
  *     @arg TIM_OCFast_Enable: TIM output compare fast enable
  *     @arg TIM_OCFast_Disable: TIM output compare fast disable
  * @retval None
  */
void TIM_OC3FastConfig(TIM_TypeDef* TIMx, uint32_t TIM_OCFast)
{
  uint32_t tmpccmr2 = 0;
  
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_OCFAST_STATE(TIM_OCFast));
  
  /* Get the TIMx CCMR2 register value */
  tmpccmr2 = TIMx->CCMR2;
  /* Reset the OC3FE Bit */
  tmpccmr2 &= (uint32_t)~((uint32_t)TIM_CCMR2_OC3FE);
  /* Enable or Disable the Output Compare Fast Bit */
  tmpccmr2 |= TIM_OCFast;
  /* Write to TIMx CCMR2 */
  TIMx->CCMR2 = tmpccmr2;
}

/**
  * @brief  Configures the TIMx Output Compare 4 Fast feature.
  * @param  TIMx: where x can be 1 or 2 to select the TIM peripheral.
  * @param  TIM_OCFast: new state of the Output Compare Fast Enable Bit.
  *   This parameter can be one of the following values:
  *     @arg TIM_OCFast_Enable: TIM output compare fast enable
  *     @arg TIM_OCFast_Disable: TIM output compare fast disable
  * @retval None
  */
void TIM_OC4FastConfig(TIM_TypeDef* TIMx, uint32_t TIM_OCFast)
{
  uint32_t tmpccmr2 = 0;
  
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_OCFAST_STATE(TIM_OCFast));
  
  /* Get the TIMx CCMR2 register value */
  tmpccmr2 = TIMx->CCMR2;
  /* Reset the OC4FE Bit */
  tmpccmr2 &= (uint32_t)~((uint32_t)TIM_CCMR2_OC4FE);
  /* Enable or Disable the Output Compare Fast Bit */
  tmpccmr2 |= (uint32_t)(TIM_OCFast << 8);
  /* Write to TIMx CCMR2 */
  TIMx->CCMR2 = tmpccmr2;
}

/**
  * @brief  Configures the TIMx channel 1 polarity.
  * @param  TIMx: where x can be 1 or 2 to select the TIM peripheral.
  * @param  TIM_OCPolarity: specifies the OC1 Polarity
  *   This parmeter can be one of the following values:
  *     @arg TIM_OCPolarity_High: Output Compare active high
  *     @arg TIM_OCPolarity_Low: Output Compare active low
  * @retval None
  */
void TIM_OC1PolarityConfig(TIM_TypeDef* TIMx, uint32_t TIM_OCPolarity)
{
  uint32_t tmpccer = 0;
  
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCPolarity));
  
  tmpccer = TIMx->CCER;
  /* Set or Reset the CC1P Bit */
  tmpccer &= (uint32_t)~((uint32_t)TIM_CCER_CC1P);
  tmpccer |= TIM_OCPolarity;
  /* Write to TIMx CCER register */
  TIMx->CCER = tmpccer;
}

/**
  * @brief  Configures the TIMx channel 2 polarity.
  * @param  TIMx: where x can be 1 or 2 to select the TIM peripheral.
  * @param  TIM_OCPolarity: specifies the OC2 Polarity
  *   This parmeter can be one of the following values:
  *     @arg TIM_OCPolarity_High: Output Compare active high
  *     @arg TIM_OCPolarity_Low: Output Compare active low
  * @retval None
  */
void TIM_OC2PolarityConfig(TIM_TypeDef* TIMx, uint32_t TIM_OCPolarity)
{
  uint32_t tmpccer = 0;
  
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCPolarity));
  
  tmpccer = TIMx->CCER;
  /* Set or Reset the CC2P Bit */
  tmpccer &= (uint32_t)~((uint32_t)TIM_CCER_CC2P);
  tmpccer |= (uint32_t)(TIM_OCPolarity << 4);
  /* Write to TIMx CCER register */
  TIMx->CCER = tmpccer;
}

/**
  * @brief  Configures the TIMx channel 3 polarity.
  * @param  TIMx: where x can be 1 or 2 to select the TIM peripheral.
  * @param  TIM_OCPolarity: specifies the OC3 Polarity
  *   This parmeter can be one of the following values:
  *     @arg TIM_OCPolarity_High: Output Compare active high
  *     @arg TIM_OCPolarity_Low: Output Compare active low
  * @retval None
  */
void TIM_OC3PolarityConfig(TIM_TypeDef* TIMx, uint32_t TIM_OCPolarity)
{
  uint32_t tmpccer = 0;
  
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCPolarity));
  
  tmpccer = TIMx->CCER;
  /* Set or Reset the CC3P Bit */
  tmpccer &= (uint32_t)~((uint32_t)TIM_CCER_CC3P);
  tmpccer |= (uint32_t)(TIM_OCPolarity << 8);
  /* Write to TIMx CCER register */
  TIMx->CCER = tmpccer;
}

/**
  * @brief  Configures the TIMx channel 4 polarity.
  * @param  TIMx: where x can be 1 or 2 to select the TIM peripheral.
  * @param  TIM_OCPolarity: specifies the OC4 Polarity
  *   This parmeter can be one of the following values:
  *     @arg TIM_OCPolarity_High: Output Compare active high
  *     @arg TIM_OCPolarity_Low: Output Compare active low
  * @retval None
  */
void TIM_OC4PolarityConfig(TIM_TypeDef* TIMx, uint32_t TIM_OCPolarity)
{
  uint32_t tmpccer = 0;
  
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCPolarity));
  
  tmpccer = TIMx->CCER;
  /* Set or Reset the CC4P Bit */
  tmpccer &= (uint32_t)~((uint32_t)TIM_CCER_CC4P);
  tmpccer |= (uint32_t)(TIM_OCPolarity << 12);
  /* Write to TIMx CCER register */
  TIMx->CCER = tmpccer;
}

/**
  * @brief  Enables or disables the TIM Capture Compare Channel x.
  * @param  TIMx: where x can be 1 or 2 to select the TIM peripheral.
  * @param  TIM_Channel: specifies the TIM Channel
  *   This parameter can be one of the following values:
  *     @arg TIM_Channel_1: TIM Channel 1
  *     @arg TIM_Channel_2: TIM Channel 2
  *     @arg TIM_Channel_3: TIM Channel 3
  *     @arg TIM_Channel_4: TIM Channel 4
  * @param  TIM_CCx: specifies the TIM Channel CCxE bit new state.
  *   This parameter can be: TIM_CCx_Enable or TIM_CCx_Disable. 
  * @retval None
  */
void TIM_CCxCmd(TIM_TypeDef* TIMx, uint32_t TIM_Channel, uint32_t TIM_CCx)
{
  uint32_t tmp = 0;

  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx)); 
  assert_param(IS_TIM_CCX(TIM_CCx));

  tmp = CCER_CCE_SET << TIM_Channel;

  /* Reset the CCxE Bit */
  TIMx->CCER &= (uint32_t)~ tmp;

  /* Set or reset the CCxE Bit */ 
  TIMx->CCER |=  (uint32_t)(TIM_CCx << TIM_Channel);
}

/**
  * @}
  */

/** @defgroup TIM_Group3 Input Capture management functions
 *  @brief    Input Capture management functions 
 *
@verbatim
 ===============================================================================
               ##### Input Capture management functions #####
 ===============================================================================
   
          *** TIM Driver: how to use it in Input Capture Mode ***
 ===============================================================================
    [..] To use the Timer in Input Capture mode, the following steps are mandatory:
         (#) Configure the TIM pins by configuring the corresponding GPIO pins.
         (#) Configure the Time base unit as described in the first part of this 
             driver, if needed, else the Timer will run with the default configuration:
             (++) Autoreload value = 0xFFFF.
             (++) Prescaler value = 0x0000.
             (++) Counter mode = Up counting.
         (#) Fill the TIM_ICInitStruct with the desired parameters including:
             (++) TIM Channel: TIM_Channel.
             (++) TIM Input Capture polarity: TIM_ICPolarity.
             (++) TIM Input Capture selection: TIM_ICSelection.
             (++) TIM Input Capture Prescaler: TIM_ICPrescaler.
             (++) TIM Input CApture filter value: TIM_ICFilter.
         (#) Call TIM_ICInit(TIMx, &TIM_ICInitStruct) to configure the desired 
             channel with the corresponding configuration and to measure only 
             frequency or duty cycle of the input signal,or, Call 
             TIM_PWMIConfig(TIMx, &TIM_ICInitStruct) to configure the desired 
             channels with the corresponding configuration and to measure the 
             frequency and the duty cycle of the input signal.
         (#) Enable the NVIC to read the measured frequency.
         (#) Enable the corresponding interrupt to read 
             the Captured value, using the function TIM_ITConfig(TIMx, TIM_IT_CCx).
         (#) Call the TIM_Cmd(ENABLE) function to enable the TIM counter.
         (#) Use TIM_GetCapturex(TIMx); to read the captured value.
    [..]
        (@) All other functions can be used separately to modify, if needed,
            a specific feature of the Timer. 

@endverbatim
  * @{
  */

/**
  * @brief  Initializes the TIM peripheral according to the specified
  *         parameters in the TIM_ICInitStruct.
  * @param  TIMx: where x can be 1 or 2 to select the TIM peripheral.
  * @param  TIM_ICInitStruct: pointer to a TIM_ICInitTypeDef structure
  *         that contains the configuration information for the specified TIM 
  *         peripheral.
  * @retval None
  */
void TIM_ICInit(TIM_TypeDef* TIMx, TIM_ICInitTypeDef* TIM_ICInitStruct)
{
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_CHANNEL(TIM_ICInitStruct->TIM_Channel));  
  assert_param(IS_TIM_IC_SELECTION(TIM_ICInitStruct->TIM_ICSelection));
  assert_param(IS_TIM_IC_PRESCALER(TIM_ICInitStruct->TIM_ICPrescaler));
  assert_param(IS_TIM_IC_FILTER(TIM_ICInitStruct->TIM_ICFilter));
  assert_param(IS_TIM_IC_POLARITY(TIM_ICInitStruct->TIM_ICPolarity));

  if (TIM_ICInitStruct->TIM_Channel == TIM_Channel_1)
  {
    /* TI1 Configuration */
    TI1_Config(TIMx, TIM_ICInitStruct->TIM_ICPolarity,
               TIM_ICInitStruct->TIM_ICSelection,
               TIM_ICInitStruct->TIM_ICFilter);
    /* Set the Input Capture Prescaler value */
    TIM_SetIC1Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
  }
  else if (TIM_ICInitStruct->TIM_Channel == TIM_Channel_2)
  {
    /* TI2 Configuration */
    TI2_Config(TIMx, TIM_ICInitStruct->TIM_ICPolarity,
               TIM_ICInitStruct->TIM_ICSelection,
               TIM_ICInitStruct->TIM_ICFilter);
    /* Set the Input Capture Prescaler value */
    TIM_SetIC2Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
  }
  else if (TIM_ICInitStruct->TIM_Channel == TIM_Channel_3)
  {
    /* TI3 Configuration */
    TI3_Config(TIMx,  TIM_ICInitStruct->TIM_ICPolarity,
               TIM_ICInitStruct->TIM_ICSelection,
               TIM_ICInitStruct->TIM_ICFilter);
    /* Set the Input Capture Prescaler value */
    TIM_SetIC3Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
  }
  else
  {
    /* TI4 Configuration */
    TI4_Config(TIMx, TIM_ICInitStruct->TIM_ICPolarity,
               TIM_ICInitStruct->TIM_ICSelection,
               TIM_ICInitStruct->TIM_ICFilter);
    /* Set the Input Capture Prescaler value */
    TIM_SetIC4Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
  }
}

/**
  * @brief  Fills each TIM_ICInitStruct member with its default value.
  * @param  TIM_ICInitStruct : pointer to a TIM_ICInitTypeDef structure which will
  *         be initialized.
  * @retval None
  */
void TIM_ICStructInit(TIM_ICInitTypeDef* TIM_ICInitStruct)
{
  /* Set the default configuration */
  TIM_ICInitStruct->TIM_Channel = TIM_Channel_1;
  TIM_ICInitStruct->TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitStruct->TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStruct->TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStruct->TIM_ICFilter = 0x00;
}

/**
  * @brief  Configures the TIM peripheral according to the specified
  *         parameters in the TIM_ICInitStruct to measure an external PWM signal.
  * @param  TIMx: where x can be 1 or 2 to select the TIM peripheral.
  * @param  TIM_ICInitStruct: pointer to a TIM_ICInitTypeDef structure
  *         that contains the configuration information for the specified TIM 
  *         peripheral.
  * @retval None
  */
void TIM_PWMIConfig(TIM_TypeDef* TIMx, TIM_ICInitTypeDef* TIM_ICInitStruct)
{
  uint32_t icoppositepolarity = TIM_ICPolarity_Rising;
  uint32_t icoppositeselection = TIM_ICSelection_DirectTI;
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  /* Select the Opposite Input Polarity */
  if (TIM_ICInitStruct->TIM_ICPolarity == TIM_ICPolarity_Rising)
  {
    icoppositepolarity = TIM_ICPolarity_Falling;
  }
  else
  {
    icoppositepolarity = TIM_ICPolarity_Rising;
  }
  /* Select the Opposite Input */
  if (TIM_ICInitStruct->TIM_ICSelection == TIM_ICSelection_DirectTI)
  {
    icoppositeselection = TIM_ICSelection_IndirectTI;
  }
  else
  {
    icoppositeselection = TIM_ICSelection_DirectTI;
  }
  if (TIM_ICInitStruct->TIM_Channel == TIM_Channel_1)
  {
    /* TI1 Configuration */
    TI1_Config(TIMx, TIM_ICInitStruct->TIM_ICPolarity, TIM_ICInitStruct->TIM_ICSelection,
               TIM_ICInitStruct->TIM_ICFilter);
    /* Set the Input Capture Prescaler value */
    TIM_SetIC1Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
    /* TI2 Configuration */
    TI2_Config(TIMx, icoppositepolarity, icoppositeselection, TIM_ICInitStruct->TIM_ICFilter);
    /* Set the Input Capture Prescaler value */
    TIM_SetIC2Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
  }
  else
  { 
    /* TI2 Configuration */
    TI2_Config(TIMx, TIM_ICInitStruct->TIM_ICPolarity, TIM_ICInitStruct->TIM_ICSelection,
               TIM_ICInitStruct->TIM_ICFilter);
    /* Set the Input Capture Prescaler value */
    TIM_SetIC2Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
    /* TI1 Configuration */
    TI1_Config(TIMx, icoppositepolarity, icoppositeselection, TIM_ICInitStruct->TIM_ICFilter);
    /* Set the Input Capture Prescaler value */
    TIM_SetIC1Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
  }
}

/**
  * @brief  Gets the TIMx Input Capture 1 value.
  * @param  TIMx: where x can be 1 or 2 to select the TIM peripheral.
  * @retval Capture Compare 1 Register value.
  */
uint32_t TIM_GetCapture1(TIM_TypeDef* TIMx)
{
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  
  /* Get the Capture 1 Register value */
  return TIMx->CCR1;
}

/**
  * @brief  Gets the TIMx Input Capture 2 value.
  * @param  TIMx: where x can be 1 or 2 to select the TIM peripheral.
  * @retval Capture Compare 2 Register value.
  */
uint32_t TIM_GetCapture2(TIM_TypeDef* TIMx)
{
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  
  /* Get the Capture 2 Register value */
  return TIMx->CCR2;
}

/**
  * @brief  Gets the TIMx Input Capture 3 value.
  * @param  TIMx: where x can be 1 or 2 to select the TIM peripheral.
  * @retval Capture Compare 3 Register value.
  */
uint32_t TIM_GetCapture3(TIM_TypeDef* TIMx)
{
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  
  /* Get the Capture 3 Register value */
  return TIMx->CCR3;
}

/**
  * @brief  Gets the TIMx Input Capture 4 value.
  * @param  TIMx: where x can be 1 or 2 to select the TIM peripheral.
  * @retval Capture Compare 4 Register value.
  */
uint32_t TIM_GetCapture4(TIM_TypeDef* TIMx)
{
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  
  /* Get the Capture 4 Register value */
  return TIMx->CCR4;
}

/**
  * @brief  Sets the TIMx Input Capture 1 prescaler.
  * @param  TIMx: where x can be 1 or 2 to select the TIM peripheral.
  * @param  TIM_ICPSC: specifies the Input Capture1 prescaler new value.
  *   This parameter can be one of the following values:
  *     @arg TIM_ICPSC_DIV1: no prescaler
  *     @arg TIM_ICPSC_DIV2: capture is done once every 2 events
  *     @arg TIM_ICPSC_DIV4: capture is done once every 4 events
  *     @arg TIM_ICPSC_DIV8: capture is done once every 8 events
  * @retval None
  */
void TIM_SetIC1Prescaler(TIM_TypeDef* TIMx, uint32_t TIM_ICPSC)
{
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_IC_PRESCALER(TIM_ICPSC));
  
  /* Reset the IC1PSC Bits */
  TIMx->CCMR1 &= (uint32_t)~((uint32_t)TIM_CCMR1_IC1PSC);
  /* Set the IC1PSC value */
  TIMx->CCMR1 |= TIM_ICPSC;
}

/**
  * @brief  Sets the TIMx Input Capture 2 prescaler.
  * @param  TIMx: where x can be 1 or 2 to select the TIM peripheral.
  * @param  TIM_ICPSC: specifies the Input Capture2 prescaler new value.
  *   This parameter can be one of the following values:
  *     @arg TIM_ICPSC_DIV1: no prescaler
  *     @arg TIM_ICPSC_DIV2: capture is done once every 2 events
  *     @arg TIM_ICPSC_DIV4: capture is done once every 4 events
  *     @arg TIM_ICPSC_DIV8: capture is done once every 8 events
  * @retval None
  */
void TIM_SetIC2Prescaler(TIM_TypeDef* TIMx, uint32_t TIM_ICPSC)
{
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_IC_PRESCALER(TIM_ICPSC));
  
  /* Reset the IC2PSC Bits */
  TIMx->CCMR1 &= (uint32_t)~((uint32_t)TIM_CCMR1_IC2PSC);
  /* Set the IC2PSC value */
  TIMx->CCMR1 |= (uint32_t)(TIM_ICPSC << 8);
}

/**
  * @brief  Sets the TIMx Input Capture 3 prescaler.
  * @param  TIMx: where x can be 1 or 2 to select the TIM peripheral.
  * @param  TIM_ICPSC: specifies the Input Capture3 prescaler new value.
  *   This parameter can be one of the following values:
  *     @arg TIM_ICPSC_DIV1: no prescaler
  *     @arg TIM_ICPSC_DIV2: capture is done once every 2 events
  *     @arg TIM_ICPSC_DIV4: capture is done once every 4 events
  *     @arg TIM_ICPSC_DIV8: capture is done once every 8 events
  * @retval None
  */
void TIM_SetIC3Prescaler(TIM_TypeDef* TIMx, uint32_t TIM_ICPSC)
{
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_IC_PRESCALER(TIM_ICPSC));
  
  /* Reset the IC3PSC Bits */
  TIMx->CCMR2 &= (uint32_t)~((uint32_t)TIM_CCMR2_IC3PSC);
  /* Set the IC3PSC value */
  TIMx->CCMR2 |= TIM_ICPSC;
}

/**
  * @brief  Sets the TIMx Input Capture 4 prescaler.
  * @param  TIMx: where x can be 1 or 2 to select the TIM peripheral.
  * @param  TIM_ICPSC: specifies the Input Capture4 prescaler new value.
  *   This parameter can be one of the following values:
  *     @arg TIM_ICPSC_DIV1: no prescaler
  *     @arg TIM_ICPSC_DIV2: capture is done once every 2 events
  *     @arg TIM_ICPSC_DIV4: capture is done once every 4 events
  *     @arg TIM_ICPSC_DIV8: capture is done once every 8 events
  * @retval None
  */
void TIM_SetIC4Prescaler(TIM_TypeDef* TIMx, uint32_t TIM_ICPSC)
{  
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_IC_PRESCALER(TIM_ICPSC));
  
  /* Reset the IC4PSC Bits */
  TIMx->CCMR2 &= (uint32_t)~((uint32_t)TIM_CCMR2_IC4PSC);
  /* Set the IC4PSC value */
  TIMx->CCMR2 |= (uint32_t)(TIM_ICPSC << 8);
}

/**
  * @}
  */

/** @defgroup TIM_Group4 Interrupts and flags management functions
 *  @brief    Interrupts and flags management functions 
 *
@verbatim
 ===============================================================================
          ##### Interrupts and flags management functions #####
 ===============================================================================

@endverbatim
  * @{
  */

/**
  * @brief  Enables or disables the specified TIM interrupts.
  * @param  TIMx_IT: where x can be 1 or 2 to select the TIMx peripheral.
  * @param  TIM_ITRPT: specifies the TIM interrupts sources to be enabled or disabled.
  *   This parameter can be any combination of the following values:
  *     @arg TIM_IT_Update: TIM update Interrupt source
  *     @arg TIM_IT_CC1: TIM Capture Compare 1 Interrupt source
  *     @arg TIM_IT_CC2: TIM Capture Compare 2 Interrupt source
  *     @arg TIM_IT_CC3: TIM Capture Compare 3 Interrupt source
  *     @arg TIM_IT_CC4: TIM Capture Compare 4 Interrupt source
  *     @arg TIM_IT_Trigger: TIM Trigger Interrupt source    
  * @param  NewState: new state of the TIM interrupts.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void TIM_ITConfig(TIM_IT_TypeDef* TIMx_IT, uint32_t TIM_ITRPT, FunctionalState NewState)
{  
  /* Check the parameters */
  assert_param(IS_TIM_IT_ALL_PERIPH(TIMx_IT));
  assert_param(IS_TIM_ITRPT(TIM_ITRPT));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable the Interrupt sources */
    TIMx_IT->IER |= TIM_ITRPT;
  }
  else
  {
    /* Disable the Interrupt sources */
    TIMx_IT->IER &= (uint32_t)~TIM_ITRPT;
  }
}

/**
  * @brief  Configures the TIMx event to be generate by software.
  * @param  TIMx: where x can be 1 or 2 to select the 
  *         TIM peripheral.
  * @param  TIM_EventSource: specifies the event source.
  *   This parameter can be one or more of the following values:  
  *     @arg TIM_EventSource_Update: Timer update Event source
  *     @arg TIM_EventSource_CC1: Timer Capture Compare 1 Event source
  *     @arg TIM_EventSource_CC2: Timer Capture Compare 2 Event source
  *     @arg TIM_EventSource_CC3: Timer Capture Compare 3 Event source
  *     @arg TIM_EventSource_CC4: Timer Capture Compare 4 Event source 
  *     @arg TIM_EventSource_Trigger: Timer Trigger Event source         
  * @retval None
  */
void TIM_GenerateEvent(TIM_TypeDef* TIMx, uint32_t TIM_EventSource)
{ 
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_EVENT_SOURCE(TIM_EventSource)); 
  /* Set the event sources */
  TIMx->EGR = TIM_EventSource;
}

/**
  * @brief  Checks whether the TIM interrupt has occurred or not.
  * @param  TIMx_IT: where x can be 1 or 2 to select the TIM peripheral.
  * @param  TIM_ITRPT: specifies the TIM interrupt source to check.
  *   This parameter can be one of the following values:
  *     @arg TIM_IT_Update: TIM update Interrupt source
  *     @arg TIM_IT_CC1: TIM Capture Compare 1 Interrupt source
  *     @arg TIM_IT_CC2: TIM Capture Compare 2 Interrupt source
  *     @arg TIM_IT_CC3: TIM Capture Compare 3 Interrupt source
  *     @arg TIM_IT_CC4: TIM Capture Compare 4 Interrupt source
  *     @arg TIM_IT_Trigger: TIM Trigger Interrupt source
  * @retval The new state of the TIM_IT(SET or RESET).
  */
ITStatus TIM_GetITStatus(TIM_IT_TypeDef* TIMx_IT, uint32_t TIM_ITRPT)
{
  ITStatus bitstatus = RESET;  
  uint32_t itstatus = 0x0, itenable = 0x0;
  
  /* Check the parameters */
  assert_param(IS_TIM_IT_ALL_PERIPH(TIMx_IT));
  assert_param(IS_TIM_GET_IT(TIM_ITRPT));
  
  
  itstatus = TIMx_IT->ISR & TIM_ITRPT;  
  itenable = TIMx_IT->IER & TIM_ITRPT;
  
  if ((itstatus != (uint32_t)RESET) && (itenable != (uint32_t)RESET))
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  return bitstatus;
}

/**
  * @brief  Clears the TIMx's interrupt pending bits.
  * @param  TIMx_IT: where x can be 1 or 2 to select the TIM peripheral.
  * @param  TIM_ITRPT: specifies the pending bit to clear.
  *   This parameter can be any combination of the following values:
  *     @arg TIM_IT_Update: TIM1 update Interrupt source
  *     @arg TIM_IT_CC1: TIM Capture Compare 1 Interrupt source
  *     @arg TIM_IT_CC2: TIM Capture Compare 2 Interrupt source
  *     @arg TIM_IT_CC3: TIM Capture Compare 3 Interrupt source
  *     @arg TIM_IT_CC4: TIM Capture Compare 4 Interrupt source
  *     @arg TIM_IT_Trigger: TIM Trigger Interrupt source
  * @retval None
  */
void TIM_ClearITPendingBit(TIM_IT_TypeDef* TIMx_IT, uint32_t TIM_ITRPT)
{
  /* Check the parameters */
  assert_param(IS_TIM_IT_ALL_PERIPH(TIMx_IT));
  assert_param(IS_TIM_ITRPT(TIM_ITRPT));
  
  /* Clear the IT pending Bit */

    TIMx_IT->ISR = TIM_ITRPT;
}

/**
  * @}
  */

/** @defgroup TIM_Group5 Clocks management functions
 *  @brief    Clocks management functions
 *
@verbatim
 ===============================================================================
                     ##### Clocks management functions #####
 ===============================================================================

@endverbatim
  * @{
  */

/**
  * @brief  Configures the TIMx internal Clock
  * @param  TIMx: where x can be 1 or 2 to select the TIM peripheral.
  * @retval None
  */
void TIM_InternalClockConfig(TIM_TypeDef* TIMx)
{
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  /* Disable slave mode to clock the prescaler directly with the internal clock */
  TIMx->SMCR &=  (uint32_t)(~((uint32_t)TIM_SMCR_SMS));
}

/**
  * @brief  Configures the TIMx Internal Trigger as External Clock
  * @param  TIMx: where x can be 1 or 2 to select the TIM peripheral.
  * @param  TIM_InputTriggerSource: Trigger source.
  *   This parameter can only be:
  *   @arg  TIM_TS_ITR0: Internal Trigger 0
  * @retval None
  */
void TIM_ITRxExternalClockConfig(TIM_TypeDef* TIMx, uint32_t TIM_InputTriggerSource)
{
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_INTERNAL_TRIGGER_SELECTION(TIM_InputTriggerSource));
  /* Select the Internal Trigger */
  TIM_SelectInputTrigger(TIMx, TIM_InputTriggerSource);
  /* Select the External clock mode1 */
  TIMx->SMCR |= TIM_SlaveMode_External1;
}

/**
  * @brief  Configures the TIMx Trigger as External Clock
  * @param  TIMx: where x can be 1 or 2 to select the TIM peripheral.
  * @param  TIM_TIxExternalCLKSource: Trigger source.
  *   This parameter can be one of the following values:
  *     @arg TIM_TIxExternalCLK1Source_TI1ED: TI1 Edge Detector
  *     @arg TIM_TIxExternalCLK1Source_TI1: Filtered Timer Input 1
  *     @arg TIM_TIxExternalCLK1Source_TI2: Filtered Timer Input 2
  * @param  TIM_ICPolarity: specifies the TIx Polarity.
  *   This parameter can be one of the following values:
  *     @arg TIM_ICPolarity_Rising
  *     @arg TIM_ICPolarity_Falling
  * @param  ICFilter : specifies the filter value.
  *   This parameter must be a value between 0x0 and 0xF.
  * @retval None
  */
void TIM_TIxExternalClockConfig(TIM_TypeDef* TIMx, uint32_t TIM_TIxExternalCLKSource,
                                uint32_t TIM_ICPolarity, uint32_t ICFilter)
{
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_IC_POLARITY(TIM_ICPolarity));
  assert_param(IS_TIM_IC_FILTER(ICFilter));
  
  /* Configure the Timer Input Clock Source */
  if (TIM_TIxExternalCLKSource == TIM_TIxExternalCLK1Source_TI2)
  {
    TI2_Config(TIMx, TIM_ICPolarity, TIM_ICSelection_DirectTI, ICFilter);
  }
  else
  {
    TI1_Config(TIMx, TIM_ICPolarity, TIM_ICSelection_DirectTI, ICFilter);
  }
  /* Select the Trigger source */
  TIM_SelectInputTrigger(TIMx, TIM_TIxExternalCLKSource);
  /* Select the External clock mode1 */
  TIMx->SMCR |= TIM_SlaveMode_External1;
}

/**
  * @brief  Configures the External clock Mode1
  * @param  TIMx: where x can be 1 or 2 to select the TIM peripheral.
  * @param  TIM_ExtTRGPrescaler: The external Trigger Prescaler.
  *   This parameter can be one of the following values:
  *     @arg TIM_ExtTRGPSC_OFF: ETRP Prescaler OFF.
  *     @arg TIM_ExtTRGPSC_DIV2: ETRP frequency divided by 2.
  *     @arg TIM_ExtTRGPSC_DIV4: ETRP frequency divided by 4.
  *     @arg TIM_ExtTRGPSC_DIV8: ETRP frequency divided by 8.
  * @param  TIM_ExtTRGPolarity: The external Trigger Polarity.
  *   This parameter can be one of the following values:
  *     @arg TIM_ExtTRGPolarity_Inverted: active low or falling edge active.
  *     @arg TIM_ExtTRGPolarity_NonInverted: active high or rising edge active.
  * @param  ExtTRGFilter: External Trigger Filter.
  *   This parameter must be a value between 0x00 and 0x0F
  * @retval None
  */
void TIM_ETRClockMode1Config(TIM_TypeDef* TIMx, uint32_t TIM_ExtTRGPrescaler, uint32_t TIM_ExtTRGPolarity,
                             uint32_t ExtTRGFilter)
{
  uint32_t tmpsmcr = 0;
  
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_EXT_PRESCALER(TIM_ExtTRGPrescaler));
  assert_param(IS_TIM_EXT_POLARITY(TIM_ExtTRGPolarity));
  assert_param(IS_TIM_EXT_FILTER(ExtTRGFilter));
  
  /* Configure the ETR Clock source */
  TIM_ETRConfig(TIMx, TIM_ExtTRGPrescaler, TIM_ExtTRGPolarity, ExtTRGFilter);
  
  /* Get the TIMx SMCR register value */
  tmpsmcr = TIMx->SMCR;
  /* Reset the SMS Bits */
  tmpsmcr &= (uint32_t)(~((uint32_t)TIM_SMCR_SMS));
  /* Select the External clock mode1 */
  tmpsmcr |= TIM_SlaveMode_External1;
  /* Select the Trigger selection : ETRF */
  tmpsmcr &= (uint32_t)(~((uint32_t)TIM_SMCR_TS));
  tmpsmcr |= TIM_TS_ETRF;
  /* Write to TIMx SMCR */
  TIMx->SMCR = tmpsmcr;
}

/**
  * @brief  Configures the External clock Mode2
  * @param  TIMx: where x can be 1 or 2 to select the TIM peripheral.
  * @param  TIM_ExtTRGPrescaler: The external Trigger Prescaler.
  *   This parameter can be one of the following values:
  *     @arg TIM_ExtTRGPSC_OFF: ETRP Prescaler OFF.
  *     @arg TIM_ExtTRGPSC_DIV2: ETRP frequency divided by 2.
  *     @arg TIM_ExtTRGPSC_DIV4: ETRP frequency divided by 4.
  *     @arg TIM_ExtTRGPSC_DIV8: ETRP frequency divided by 8.
  * @param  TIM_ExtTRGPolarity: The external Trigger Polarity.
  *   This parameter can be one of the following values:
  *     @arg TIM_ExtTRGPolarity_Inverted: active low or falling edge active.
  *     @arg TIM_ExtTRGPolarity_NonInverted: active high or rising edge active.
  * @param  ExtTRGFilter: External Trigger Filter.
  *   This parameter must be a value between 0x00 and 0x0F
  * @retval None
  */
void TIM_ETRClockMode2Config(TIM_TypeDef* TIMx, uint32_t TIM_ExtTRGPrescaler, 
                             uint32_t TIM_ExtTRGPolarity, uint32_t ExtTRGFilter)
{
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_EXT_PRESCALER(TIM_ExtTRGPrescaler));
  assert_param(IS_TIM_EXT_POLARITY(TIM_ExtTRGPolarity));
  assert_param(IS_TIM_EXT_FILTER(ExtTRGFilter));
  
  /* Configure the ETR Clock source */
  TIM_ETRConfig(TIMx, TIM_ExtTRGPrescaler, TIM_ExtTRGPolarity, ExtTRGFilter);
  /* Enable the External clock mode2 */
  TIMx->SMCR |= TIM_SMCR_ECE;
}

/**
  * @}
  */

/** @defgroup TIM_Group6 Synchronization management functions
 *  @brief    Synchronization management functions 
 *
@verbatim
 ===============================================================================
               ##### Synchronization management functions #####
 ===============================================================================
        *** TIM Driver: how to use it in synchronization Mode ***
 ===============================================================================
    [..] Case of two/several Timers
         (#) Configure the Master Timers using the following functions:
             (++) void TIM_SelectOutputTrigger(TIM_TypeDef* TIMx,
                  uint32_t TIM_TRGOSource).
             (++) void TIM_SelectMasterSlaveMode(TIM_TypeDef* TIMx,
                  uint32_t TIM_MasterSlaveMode);  
         (#) Configure the Slave Timers using the following functions: 
             (++) void TIM_SelectInputTrigger(TIM_TypeDef* TIMx, 
                  uint32_t TIM_InputTriggerSource);  
             (++) void TIM_SelectSlaveMode(TIM_TypeDef* TIMx, uint32_t TIM_SlaveMode);
    [..] Case of Timers and external trigger(ETR pin)
         (#) Configure the Etrenal trigger using this function:
             (++) void TIM_ETRConfig(TIM_TypeDef* TIMx, uint32_t TIM_ExtTRGPrescaler,
                  uint32_t TIM_ExtTRGPolarity, uint32_t ExtTRGFilter);
         (#) Configure the Slave Timers using the following functions:
             (++) void TIM_SelectInputTrigger(TIM_TypeDef* TIMx,
                  uint32_t TIM_InputTriggerSource);
             (++) void TIM_SelectSlaveMode(TIM_TypeDef* TIMx, uint32_t TIM_SlaveMode);

@endverbatim
  * @{
  */
/**
  * @brief  Selects the Input Trigger source
  * @param  TIMx: where x can be 1 or 2 to select the TIM peripheral.
  * @param  TIM_InputTriggerSource: The Input Trigger source.
  *   This parameter can be one of the following values:
  *     @arg TIM_TS_ITR0: Internal Trigger 0
  *     @arg TIM_TS_TI1F_ED: TI1 Edge Detector
  *     @arg TIM_TS_TI1FP1: Filtered Timer Input 1
  *     @arg TIM_TS_TI2FP2: Filtered Timer Input 2
  *     @arg TIM_TS_ETRF: External Trigger input
  * @retval None
  */
void TIM_SelectInputTrigger(TIM_TypeDef* TIMx, uint32_t TIM_InputTriggerSource)
{
  uint32_t tmpsmcr = 0;

  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx)); 
  assert_param(IS_TIM_TRIGGER_SELECTION(TIM_InputTriggerSource));

  /* Get the TIMx SMCR register value */
  tmpsmcr = TIMx->SMCR;
  /* Reset the TS Bits */
  tmpsmcr &= (uint32_t)(~((uint32_t)TIM_SMCR_TS));
  /* Set the Input Trigger source */
  tmpsmcr |= TIM_InputTriggerSource;
  /* Write to TIMx SMCR */
  TIMx->SMCR = tmpsmcr;
}

/**
  * @brief  Selects the TIMx Trigger Output Mode.
  * @param  TIMx: where x can be 1 or 2 to select the TIM peripheral.
  * @param  TIM_TRGOSource: specifies the Trigger Output source.
  *   This paramter can be one of the following values:
  *
  *   For all TIMx
  *     @arg TIM_TRGOSource_Reset:  The UG bit in the TIM_EGR register is used as the trigger output (TRGO).
  *     @arg TIM_TRGOSource_Enable: The Counter Enable CEN is used as the trigger output (TRGO).
  *     @arg TIM_TRGOSource_Update: The update event is selected as the trigger output (TRGO).
  *     @arg TIM_TRGOSource_OC1: The trigger output sends a positive pulse when the CC1IF flag
  *                              is to be set, as soon as a capture or compare match occurs (TRGO).
  *     @arg TIM_TRGOSource_OC1Ref: OC1REF signal is used as the trigger output (TRGO).
  *     @arg TIM_TRGOSource_OC2Ref: OC2REF signal is used as the trigger output (TRGO).
  *     @arg TIM_TRGOSource_OC3Ref: OC3REF signal is used as the trigger output (TRGO).
  *     @arg TIM_TRGOSource_OC4Ref: OC4REF signal is used as the trigger output (TRGO).
  *
  * @retval None
  */
void TIM_SelectOutputTrigger(TIM_TypeDef* TIMx, uint32_t TIM_TRGOSource)
{
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_TRGO_SOURCE(TIM_TRGOSource));

  /* Reset the MMS Bits */
  TIMx->CR2 &= (uint32_t)~((uint32_t)TIM_CR2_MMS);
  /* Select the TRGO source */
  TIMx->CR2 |=  TIM_TRGOSource;
}

/**
  * @brief  Selects the TIMx Slave Mode.
  * @param  TIMx: where x can be 1 or 2 to select the TIM peripheral.
  * @param  TIM_SlaveMode: specifies the Timer Slave Mode.
  *   This paramter can be one of the following values:
  *     @arg TIM_SlaveMode_Reset: Rising edge of the selected trigger signal (TRGI) re-initializes
  *                               the counter and triggers an update of the registers.
  *     @arg TIM_SlaveMode_Gated:     The counter clock is enabled when the trigger signal (TRGI) is high.
  *     @arg TIM_SlaveMode_Trigger:   The counter starts at a rising edge of the trigger TRGI.
  *     @arg TIM_SlaveMode_External1: Rising edges of the selected trigger (TRGI) clock the counter.
  * @retval None
  */
void TIM_SelectSlaveMode(TIM_TypeDef* TIMx, uint32_t TIM_SlaveMode)
{
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx)); 
  assert_param(IS_TIM_SLAVE_MODE(TIM_SlaveMode));
  
  /* Reset the SMS Bits */
  TIMx->SMCR &= (uint32_t)~((uint32_t)TIM_SMCR_SMS);
  /* Select the Slave Mode */
  TIMx->SMCR |= TIM_SlaveMode;
}

/**
  * @brief  Sets or Resets the TIMx Master/Slave Mode.
  * @param  TIMx: where x can be 1 or 2 to select the TIM peripheral.
  * @param  TIM_MasterSlaveMode: specifies the Timer Master Slave Mode.
  *   This paramter can be one of the following values:
  *     @arg TIM_MasterSlaveMode_Enable: synchronization between the current timer
  *                                      and its slaves (through TRGO).
  *     @arg TIM_MasterSlaveMode_Disable: No action
  * @retval None
  */
void TIM_SelectMasterSlaveMode(TIM_TypeDef* TIMx, uint32_t TIM_MasterSlaveMode)
{
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_MSM_STATE(TIM_MasterSlaveMode));
  
  /* Reset the MSM Bit */
  TIMx->SMCR &= (uint32_t)~((uint32_t)TIM_SMCR_MSM);
  
  /* Set or Reset the MSM Bit */
  TIMx->SMCR |= TIM_MasterSlaveMode;
}

/**
  * @brief  Configures the TIMx External Trigger (ETR).
  * @param  TIMx: where x can be 1 or 2 to select the TIM peripheral.
  * @param  TIM_ExtTRGPrescaler: The external Trigger Prescaler.
  *   This parameter can be one of the following values:
  *     @arg TIM_ExtTRGPSC_OFF: ETRP Prescaler OFF.
  *     @arg TIM_ExtTRGPSC_DIV2: ETRP frequency divided by 2.
  *     @arg TIM_ExtTRGPSC_DIV4: ETRP frequency divided by 4.
  *     @arg TIM_ExtTRGPSC_DIV8: ETRP frequency divided by 8.
  * @param  TIM_ExtTRGPolarity: The external Trigger Polarity.
  *   This parameter can be one of the following values:
  *     @arg TIM_ExtTRGPolarity_Inverted: active low or falling edge active.
  *     @arg TIM_ExtTRGPolarity_NonInverted: active high or rising edge active.
  * @param  ExtTRGFilter: External Trigger Filter.
  *   This parameter must be a value between 0x00 and 0x0F
  * @retval None
  */
void TIM_ETRConfig(TIM_TypeDef* TIMx, uint32_t TIM_ExtTRGPrescaler, uint32_t TIM_ExtTRGPolarity,
                   uint32_t ExtTRGFilter)
{
  uint32_t tmpsmcr = 0;
  
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_EXT_PRESCALER(TIM_ExtTRGPrescaler));
  assert_param(IS_TIM_EXT_POLARITY(TIM_ExtTRGPolarity));
  assert_param(IS_TIM_EXT_FILTER(ExtTRGFilter));
  
  tmpsmcr = TIMx->SMCR;
  /* Reset the ETR Bits */
  tmpsmcr &= SMCR_ETR_MASK;
  /* Set the Prescaler, the Filter value and the Polarity */
  tmpsmcr |= (uint32_t)(TIM_ExtTRGPrescaler | (uint32_t)(TIM_ExtTRGPolarity | (uint32_t)(ExtTRGFilter << (uint32_t)8)));
  /* Write to TIMx SMCR */
  TIMx->SMCR = tmpsmcr;
}

/**
  * @}
  */

/** @defgroup TIM_Group7 Specific interface management functions
 *  @brief    Specific interface management functions 
 *
@verbatim
 ===============================================================================
             ##### Specific interface management functions #####
 ===============================================================================

@endverbatim
  * @{
  */

/**
  * @brief  Configures the TIMx Encoder Interface.
  * @param  TIMx: where x can be  1 or 2 to select the TIM peripheral.
  * @param  TIM_EncoderMode: specifies the TIMx Encoder Mode.
  *   This parameter can be one of the following values:
  *     @arg TIM_EncoderMode_TI1: Counter counts on TI1FP1 edge depending on TI2FP2 level.
  *     @arg TIM_EncoderMode_TI2: Counter counts on TI2FP2 edge depending on TI1FP1 level.
  *     @arg TIM_EncoderMode_TI12: Counter counts on both TI1FP1 and TI2FP2 edges depending
  *                                on the level of the other input.
  * @param  TIM_IC1Polarity: specifies the IC1 Polarity
  *   This parmeter can be one of the following values:
  *     @arg TIM_ICPolarity_Falling: IC Falling edge.
  *     @arg TIM_ICPolarity_Rising: IC Rising edge.
  * @param  TIM_IC2Polarity: specifies the IC2 Polarity
  *   This parmeter can be one of the following values:
  *     @arg TIM_ICPolarity_Falling: IC Falling edge.
  *     @arg TIM_ICPolarity_Rising: IC Rising edge.
  * @retval None
  */
void TIM_EncoderInterfaceConfig(TIM_TypeDef* TIMx, uint32_t TIM_EncoderMode,
                                uint32_t TIM_IC1Polarity, uint32_t TIM_IC2Polarity)
{
  uint32_t tmpsmcr = 0;
  uint32_t tmpccmr1 = 0;
  uint32_t tmpccer = 0;
    
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_ENCODER_MODE(TIM_EncoderMode));
  assert_param(IS_TIM_IC_POLARITY(TIM_IC1Polarity));
  assert_param(IS_TIM_IC_POLARITY(TIM_IC2Polarity));
  
  /* Get the TIMx SMCR register value */
  tmpsmcr = TIMx->SMCR;
  /* Get the TIMx CCMR1 register value */
  tmpccmr1 = TIMx->CCMR1;
  /* Get the TIMx CCER register value */
  tmpccer = TIMx->CCER;
  /* Set the encoder Mode */
  tmpsmcr &= (uint32_t)(~((uint32_t)TIM_SMCR_SMS));
  tmpsmcr |= TIM_EncoderMode;
  /* Select the Capture Compare 1 and the Capture Compare 2 as input */
  tmpccmr1 &= (uint32_t)(((uint32_t)~((uint32_t)TIM_CCMR1_CC1S)) & (uint32_t)(~((uint32_t)TIM_CCMR1_CC2S)));
  tmpccmr1 |= TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_0;
  /* Set the TI1 and the TI2 Polarities */
  tmpccer &= (uint32_t)(((uint32_t)~((uint32_t)TIM_CCER_CC1P)) & ((uint32_t)~((uint32_t)TIM_CCER_CC2P)));
   tmpccer |= (uint32_t)(TIM_IC1Polarity | (uint32_t)(TIM_IC2Polarity << (uint32_t)4));
  /* Write to TIMx SMCR */
  TIMx->SMCR = tmpsmcr;
  /* Write to TIMx CCMR1 */
  TIMx->CCMR1 = tmpccmr1;
  /* Write to TIMx CCER */
  TIMx->CCER = tmpccer;
}

/**
  * @brief  Enables or disables the TIMx's Hall sensor interface.
  * @param  TIMx: where x can be 1, 2 or 3 to select the TIM peripheral.
  * @param  NewState: new state of the TIMx Hall sensor interface.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void TIM_SelectHallSensor(TIM_TypeDef* TIMx, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Set the TI1S Bit */
    TIMx->CR2 |= TIM_CR2_TI1S;
  }
  else
  {
    /* Reset the TI1S Bit */
    TIMx->CR2 &= (uint32_t)~((uint32_t)TIM_CR2_TI1S);
  }
}

/**
  * @}
  */

/** @defgroup TIM_Group8 Specific remapping management function
 *  @brief   Specific remapping management function
 *
@verbatim
 ===============================================================================
               ##### Specific remapping management function #####
 ===============================================================================

@endverbatim
  * @{
  */
  
/**
  * @brief  Selects the TIMx Extenal trigger used in external clock mode 2.
  * @param TIMx: where x can be 1 or 2 to select the TIM peripheral.
  * @param NewState: new state of the TIMx CLKMSKEN bit
  *   This parameter can be: ENABLE or DISABLE.
  * @retval : None
  */
void TIM_ClockMaskConfig(TIM_TypeDef* TIMx, FunctionalState NewState)
{  
  /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
  /* Enable the clock mask */
  TIMx->OR |=  TIM_ClockMask_Enable;
  }
  else
  {
    /* Disable the clock mask */
   TIMx->OR &=  TIM_ClockMask_Disable; 
  }
}

/**
  * @brief  Selects the TIMx Extenal trigger used in external clock mode 2.
  * @param TIMx: where x can be 1 or 2 to select the TIM peripheral.
  * @param TIM_EXTRIGCLK: specifies the TIM input reampping source.
  *   This parameter can be one of the following values:
  *   @arg TIM_EXTRIGPCLK: PCLK.
  *   @arg TIM_EXTRIG1KHCLK:  calibrated 1 kHz clock.
  *   @arg TIM_EXTRIG32KHCLK: 32 kHz reference clock (if available).
  *   @arg TIM_EXTRIGTIMxCLK: TIMxCLK pin.
  * @retval : None
  */
void TIM_SelectExternalTriggerClock(TIM_TypeDef* TIMx, uint32_t TIM_EXTRIGCLK)
{
 /* Check the parameters */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_EXTRIGCLK(TIM_EXTRIGCLK));

  /* Set the Timer remapping configuration */
  TIMx->OR |=  TIM_EXTRIGCLK;
}

/**
  * @brief  Configures the TIM2 Remapping input Capabilities.
  * @param TIMx: where x can be 2 to select the TIM peripheral.
  * @param TIM_Remap: specifies the TIM input reampping source.
  *   This parameter can be one of the following values:
  *   @arg TIM_REMAPC1: TIM2 Channel 1 is connected to GPIOA (PA0) or to GPIOB (PB1).
  *   @arg TIM_REMAPC2: TIM2 Channel 2 is connected to GPIOA (PA3) or to GPIOB (PB2).
  *   @arg TIM_REMAPC3: TIM2 Channel 3 is connected to GPIOA (PA1) or to GPIOB (PB3).
  *   @arg TIM_REMAPC4: TIM2 Channel 4 is connected to GPIOA (PA2) or to GPIOB (PB4).
  * @param  NewState: new state of the TIMx TIM2_OR_REMAPCy bit (y can be 1..4).
  *   This parameter can be: ENABLE or DISABLE.  
  * @retval : None
  */
void TIM_RemapCmd(TIM_TypeDef* TIMx, uint32_t TIM_Remap, FunctionalState NewState)
{
 /* Check the parameters */
  assert_param(IS_TIM_LIST1_PERIPH(TIMx));
  assert_param(IS_TIM_REMAP(TIM_Remap));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
  /* Set the Timer remapping configuration */
  TIMx->OR |=  TIM_Remap;
  }
  else
  {
    TIMx->OR &= (uint32_t)~TIM_Remap;
  }
}

/**
  * @}
  */
  
/**
  * @brief  Configure the TI1 as Input.
  * @param  TIMx: where x can be 1 or 2 to select the TIM peripheral.
  * @param  TIM_ICPolarity : The Input Polarity.
  *   This parameter can be one of the following values:
  *     @arg TIM_ICPolarity_Rising:
  *     @arg TIM_ICPolarity_Falling:
  * @param  TIM_ICSelection: specifies the input to be used.
  *   This parameter can be one of the following values:
  *     @arg TIM_ICSelection_DirectTI: TIM Input 1 is selected to be connected to IC1.
  *     @arg TIM_ICSelection_IndirectTI: TIM Input 1 is selected to be connected to IC2.
  *     @arg TIM_ICSelection_TRGI: TIM Input 1 is selected to be connected to TRGI.
  * @param  TIM_ICFilter: Specifies the Input Capture Filter.
  *   This parameter must be a value between 0x00 and 0x0F.
  * @retval None
  */
static void TI1_Config(TIM_TypeDef* TIMx, uint32_t TIM_ICPolarity, uint32_t TIM_ICSelection,
                       uint32_t TIM_ICFilter)
{

  uint32_t tmpccmr1 = 0, tmpccer = 0;
  /* Disable the Channel 1: Reset the CC1E Bit */
  TIMx->CCER &= (uint32_t)~((uint32_t)TIM_CCER_CC1E);
  tmpccmr1 = TIMx->CCMR1;
  tmpccer = TIMx->CCER;
  /* Select the Input and set the filter */
  tmpccmr1 &= (uint32_t)(((uint32_t)~((uint32_t)TIM_CCMR1_CC1S)) & ((uint32_t)~((uint32_t)TIM_CCMR1_IC1F)));
  tmpccmr1 |= (uint32_t)(TIM_ICSelection | (uint32_t)(TIM_ICFilter << (uint32_t)4));
 
  /* Select the Polarity and set the CC1E Bit */
  tmpccer &= (uint32_t)~((uint32_t)(TIM_CCER_CC1P));
  tmpccer |= (uint32_t)(TIM_ICPolarity | (uint32_t)TIM_CCER_CC1E);
  /* Write to TIMx CCMR1 and CCER registers */
  TIMx->CCMR1 = tmpccmr1;
  TIMx->CCER = tmpccer;
}

/**
  * @brief  Configure the TI2 as Input.
  * @param  TIMx: where x can be 1 or 2 to select the TIM peripheral.
  * @param  TIM_ICPolarity : The Input Polarity.
  *   This parameter can be one of the following values:
  *     @arg TIM_ICPolarity_Rising
  *     @arg TIM_ICPolarity_Falling
  * @param  TIM_ICSelection: specifies the input to be used.
  *   This parameter can be one of the following values:
  *     @arg TIM_ICSelection_DirectTI: TIM Input 2 is selected to be connected to IC2.
  *     @arg TIM_ICSelection_IndirectTI: TIM Input 2 is selected to be connected to IC1.
  *     @arg TIM_ICSelection_TRGI: TIM Input 2 is selected to be connected to TRGI.
  * @param  TIM_ICFilter: Specifies the Input Capture Filter.
  *   This parameter must be a value between 0x00 and 0x0F.
  * @retval None
  */
static void TI2_Config(TIM_TypeDef* TIMx, uint32_t TIM_ICPolarity, uint32_t TIM_ICSelection,
                       uint32_t TIM_ICFilter)
{
  uint32_t tmpccmr1 = 0, tmpccer = 0, tmp = 0;
  /* Disable the Channel 2: Reset the CC2E Bit */
  TIMx->CCER &= (uint32_t)~((uint32_t)TIM_CCER_CC2E);
  tmpccmr1 = TIMx->CCMR1;
  tmpccer = TIMx->CCER;
  tmp = (uint32_t)(TIM_ICPolarity << 4);
  /* Select the Input and set the filter */
  tmpccmr1 &= (uint32_t)(((uint32_t)~((uint32_t)TIM_CCMR1_CC2S)) & ((uint32_t)~((uint32_t)TIM_CCMR1_IC2F)));
  tmpccmr1 |= (uint32_t)(TIM_ICFilter << 12);
  tmpccmr1 |= (uint32_t)(TIM_ICSelection << 8); 
  /* Select the Polarity and set the CC2E Bit */
  tmpccer &= (uint32_t)~((uint32_t)(TIM_CCER_CC2P));
  tmpccer |= (uint32_t)(tmp | (uint32_t)TIM_CCER_CC2E);  
  /* Write to TIMx CCMR1 and CCER registers */
  TIMx->CCMR1 = tmpccmr1 ;
  TIMx->CCER = tmpccer;
}

/**
  * @brief  Configure the TI3 as Input.
  * @param  TIMx: where x can be 1, 2 or 3 to select the TIM peripheral.
  * @param  TIM_ICPolarity : The Input Polarity.
  *   This parameter can be one of the following values:
  *     @arg TIM_ICPolarity_Rising
  *     @arg TIM_ICPolarity_Falling
  * @param  TIM_ICSelection: specifies the input to be used.
  *   This parameter can be one of the following values:
  *     @arg TIM_ICSelection_DirectTI: TIM Input 3 is selected to be connected to IC3.
  *     @arg TIM_ICSelection_IndirectTI: TIM Input 3 is selected to be connected to IC4.
  *     @arg TIM_ICSelection_TRGI: TIM Input 3 is selected to be connected to TRGI.
  * @param  TIM_ICFilter: Specifies the Input Capture Filter.
  *   This parameter must be a value between 0x00 and 0x0F.
  * @retval None
  */
static void TI3_Config(TIM_TypeDef* TIMx, uint32_t TIM_ICPolarity, uint32_t TIM_ICSelection,
                       uint32_t TIM_ICFilter)
{
  uint32_t tmpccmr2 = 0, tmpccer = 0, tmp = 0;
  /* Disable the Channel 3: Reset the CC3E Bit */
  TIMx->CCER &= (uint32_t)~((uint32_t)TIM_CCER_CC3E);
  tmpccmr2 = TIMx->CCMR2;
  tmpccer = TIMx->CCER;
  tmp = (uint32_t)(TIM_ICPolarity << 8);
  /* Select the Input and set the filter */
  tmpccmr2 &= (uint32_t)(((uint32_t)~((uint32_t)TIM_CCMR2_CC3S)) & ((uint32_t)~((uint32_t)TIM_CCMR2_IC3F)));
  tmpccmr2 |= (uint32_t)(TIM_ICSelection | (uint32_t)(TIM_ICFilter << (uint32_t)4));
  /* Select the Polarity and set the CC3E Bit */
  tmpccer &= (uint32_t)~((uint32_t)(TIM_CCER_CC3P));
  tmpccer |= (uint32_t)(tmp | (uint32_t)TIM_CCER_CC3E);  
  /* Write to TIMx CCMR2 and CCER registers */
  TIMx->CCMR2 = tmpccmr2;
  TIMx->CCER = tmpccer;
}

/**
  * @brief  Configure the TI4 as Input.
  * @param  TIMx: where x can be 1, 2 or 3 to select the TIM peripheral.
  * @param  TIM_ICPolarity : The Input Polarity.
  *   This parameter can be one of the following values:
  *     @arg TIM_ICPolarity_Rising
  *     @arg TIM_ICPolarity_Falling
  * @param  TIM_ICSelection: specifies the input to be used.
  *   This parameter can be one of the following values:
  *     @arg TIM_ICSelection_DirectTI: TIM Input 4 is selected to be connected to IC4.
  *     @arg TIM_ICSelection_IndirectTI: TIM Input 4 is selected to be connected to IC3.
  *     @arg TIM_ICSelection_TRGI: TIM Input 4 is selected to be connected to TRGI.
  * @param  TIM_ICFilter: Specifies the Input Capture Filter.
  *   This parameter must be a value between 0x00 and 0x0F.
  * @retval None
  */
static void TI4_Config(TIM_TypeDef* TIMx, uint32_t TIM_ICPolarity, uint32_t TIM_ICSelection,
                       uint32_t TIM_ICFilter)
{
  uint32_t tmpccmr2 = 0, tmpccer = 0, tmp = 0;

   /* Disable the Channel 4: Reset the CC4E Bit */
  TIMx->CCER &= (uint32_t)~((uint32_t)TIM_CCER_CC4E);
  tmpccmr2 = TIMx->CCMR2;
  tmpccer = TIMx->CCER;
  tmp = (uint32_t)(TIM_ICPolarity << 12);
  /* Select the Input and set the filter */
  tmpccmr2 &= (uint32_t)((uint32_t)(~(uint32_t)TIM_CCMR2_CC4S) & ((uint32_t)~((uint32_t)TIM_CCMR2_IC4F)));
  tmpccmr2 |= (uint32_t)(TIM_ICSelection << 8);
  tmpccmr2 |= (uint32_t)(TIM_ICFilter << 12);  
  /* Select the Polarity and set the CC4E Bit */
  tmpccer &= (uint32_t)~((uint32_t)(TIM_CCER_CC3P));
  tmpccer |= (uint32_t)(tmp | (uint32_t)TIM_CCER_CC4E);
  /* Write to TIMx CCMR2 and CCER registers */
  TIMx->CCMR2 = tmpccmr2;
  TIMx->CCER = tmpccer;
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
