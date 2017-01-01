/**
  ******************************************************************************
  * @file    stm32w108xx_slptim.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    30-November-2012
  * @brief   This file provides firmware functions to manage the Sleep Timer 
  *          peripheral.     
  *
  *  @verbatim
  *  
 =============================================================================== 
                          ##### SLPTIM features #####  
 ===============================================================================
    [..]  The sleep timer is dedicated to system timing and waking from sleep at 
          specific times.
    [..]  The sleep timer can use either the calibrated 1 kHz reference(CLK1K), 
          or the 32 kHz crystal clock (CLK32K). The default clock source is 
          the internal 1 kHz clock.
    [..]  The sleep timer has a prescaler that allows for very long periods of 
          sleep to be timed.
    [..]  The timer provides two compare outputs and wrap detection, all of which 
          can be used to generate an interrupt or a wake up event.
    [..]  The sleep timer is paused when the debugger halts the ARM Cortex-M3.
    
    
                    ##### How to use this driver #####
 ===============================================================================
    [..] This driver provides functions to configure and program the Sleep Timer 
         These functions are split in 2 groups:
         (#) SLPTIM management functions: this group includes all needed functions 
             to configure the Sleep Timer.
             (++) Enable/Disable the counter.
             (++) Get counter.
             (++) Select clock to be used as reference.
             (++) Set/Get compare (A or B) values.
         (#) Interrupts and flags management functions: this group includes all needed 
             functions to manage interrupts:
             (++) Enables or disables the specified SLPTIM interrupts.
             (++) Checks whether the specified SLPTIM flag is set or not.
             (++) Clears the specified SLPTIM flag.
         
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
#include "stm32w108xx_slptim.h"

/** @addtogroup STM32W108xx_StdPeriph_Driver
  * @{
  */

/** @defgroup SLPTIM 
  * @brief SLPTIM driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup SLPTIM_Private_Functions
  * @{
  */

/** @defgroup SLPTIM_Group1 SLPTIM management functions
 *  @brief   SLPTIM management functions 
 *
@verbatim
 ===============================================================================
                 ##### SLPTIM management functions #####
 ===============================================================================
    [..]  To use the Sleep Timer:
         (#) Fill the SLPTIM_InitStruct with the desired parameters.
             This must be done while the sleep timer is disabled. 
         (#) Call the SLPTIM_Cmd(ENABLE) function to enable the TIM counter.
         (#) Enable the clock to be used as reference by calling SLPTIM_ClockConfig()
             function.
          
    [..] 
        (@) All other functions can be used seperatly to set compareA or compareB value, 
            to get counter value... 
          
@endverbatim
  * @{
  */
  
/**
  * @brief  Deinitializes the SLPTIM peripheral registers to their default reset values.
  * @retval None
  *   
  */
void SLPTIM_DeInit(void)
{
  SLPTMR->CR = 0x00000400;
  SLPTMR->CMPAL = 0x0000FFFF;
  SLPTMR->CMPAH = 0x0000FFFF;
  SLPTMR->CMPBL = 0x0000FFFF;
  SLPTMR->CMPBH = 0x0000FFFF;
  SLPTMR->ISR = 0x00000007;
  SLPTMR->IER = 0x00000000;
}

/**
  * @brief  Initializes the SLPTIM Time peripheral according to 
  *         the specified parameters in the SLPTIM_InitStruct.
  * @param  SLPTIM_InitStruct: pointer to a SLPTIM_InitTypeDef
  *         structure that contains the configuration information for
  *         the specified TIM peripheral.
  * @retval None
  */
void SLPTIM_Init(SLPTIM_InitTypeDef* SLPTIM_InitStruct)
{
  uint32_t tmpcr = 0;

  /* Check the parameters */
  assert_param(IS_SLPTIM_COUNTER_MODE(SLPTIM_InitStruct->SLPTIM_CounterMode));
  assert_param(IS_SLPTIM_GET_CLKSEL(SLPTIM_InitStruct->SLPTIM_Clock));
  assert_param(IS_SLPTIM_CLKDIV(SLPTIM_InitStruct->SLPTIM_Prescaler));
  assert_param(IS_SLPTIM_DBGMODE(SLPTIM_InitStruct->SLPTIM_DebugMode));
  
  tmpcr = SLPTMR->CR;  
  
  /* Set the Sleep Timer Clock */
    tmpcr &= (uint32_t)~SLPTMR_CR_CLKSEL;
    tmpcr |= (uint32_t)SLPTIM_InitStruct->SLPTIM_Clock;
      
  /* Set the Prescaler value */
   tmpcr &= (uint32_t)~SLPTMR_CR_PSC; 
   tmpcr |= (uint32_t)SLPTIM_InitStruct->SLPTIM_Prescaler;
    
   /* Selects the timer's mode during debug */
   tmpcr &= (uint32_t)~SLPTMR_CR_DBGP; 
   tmpcr |= (uint32_t)SLPTIM_InitStruct->SLPTIM_DebugMode;

    /* Selects the Counter Mode */
    tmpcr &= (uint32_t)~SLPTMR_CR_REVERSE;
    tmpcr |= (uint32_t)SLPTIM_InitStruct->SLPTIM_CounterMode;
     
    SLPTMR->CR = tmpcr;
}

/**
  * @brief  Fills each SLPTIM_InitStruct member with its default value.
  * @param  SLPTIM_InitStruct : pointer to a SLPTIM_InitTypeDef
  *         structure which will be initialized.
  * @retval None
  */
void SLPTIM_StructInit(SLPTIM_InitTypeDef* SLPTIM_InitStruct)
{
  /* Set the default configuration */
  SLPTIM_InitStruct->SLPTIM_Clock = SLPTIM_CLK_1KHZ;
  SLPTIM_InitStruct->SLPTIM_Prescaler = SLPTIM_CLK_DIV0;
  SLPTIM_InitStruct->SLPTIM_DebugMode = SLPTIM_DBGRUN;
  SLPTIM_InitStruct->SLPTIM_CounterMode = SLPTIM_CountForward;
}

/**
  * @brief  Enables or disables the Sleep Timer.
  * @param  NewState: new state of the Sleep Timer.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SLPTIM_Cmd(FunctionalState NewState)
{  
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
    if (NewState != DISABLE)
    {
      /* Enable the Sleep Timer */
      SLPTMR->CR |= (uint32_t)SLPTMR_CR_EN;
    }
    else
    {
      /* Disable the clock */
      SLPTMR->CR &= (uint32_t)~SLPTMR_CR_EN;
    }
} 
    
/**
  * @brief  Sets the SLPTIM Compare A Register value
  * @param  CompareA: specifies the Compare A register new value.
  * @retval None
  */
void SLPTIM_SetCompareA(uint32_t CompareA)
{
  /* Set the Compare A Register value */
  SLPTMR->CMPAL = (uint32_t)(CompareA & 0x0000FFFF);
  SLPTMR->CMPAH = (uint32_t)((CompareA >> 16) & 0x0000FFFF);
}

/**
  * @brief  Sets the SLPTIM Compare B Register value
  * @param  CompareB: specifies the Compare B register new value.
  * @retval None
  */
void SLPTIM_SetCompareB(uint32_t CompareB)
{
  /* Set the Compare B Register value */
  SLPTMR->CMPBL = (uint32_t)(CompareB & 0x0000FFFF);
  SLPTMR->CMPBH = (uint32_t)((CompareB >> 16) & 0x0000FFFF);
}

/**
  * @brief  Gets the SLPTIM Counter value.
  * @retval Counter Register value.
  */
uint32_t SLPTIM_GetCounter(void)
{
  uint32_t counter = 0;
  
  /* Get the Counter Register value */
  counter = (uint32_t)(SLPTMR->CNTH << 16);
  counter |= (uint32_t)SLPTMR->CNTL;
  
  return counter;
}

/**
  * @brief  Gets the SLPTIM Counter high value.
  * @retval Counter Register high value.
  */
uint32_t SLPTIM_GetCounterHigh(void)
{
  __IO uint32_t counter = 0;
  
  /* Get the Counter Register value */
  counter = (uint32_t)SLPTMR->CNTH;
  
  return counter;
}

/**
  * @brief  Gets the SLPTIM Counter low value.
  * @retval Counter Register low value.
  */
uint32_t SLPTIM_GetCounterLow(void)
{
  __IO uint32_t counter = 0;
  
  /* Get the Counter Register value */
  counter = (uint32_t)SLPTMR->CNTH;
  counter = (uint32_t)SLPTMR->CNTL;
  
  return counter;
}

/**
  * @}
  */

/** @defgroup SLPTIM_Group2 Interrupts and flags management functions
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
  * @brief  Forces the specified SLPTIM interrupts.
  * @param  SLPTIM_IT: specifies the SLPTIM interrupts sources to be generated.
  *   This parameter can be any combination of the following values:
  *     @arg SLPTIM_IT_WRAP: Sleep timer overflow
  *     @arg SLPTIM_IT_CMPA: Sleep timer compare A
  *     @arg SLPTIM_IT_CMPB: Sleep timer compare B  
  * @retval None
  */
void SLPTIM_ForceIT(uint32_t SLPTIM_IT)
{  
  /* Check the parameters */
  assert_param(IS_SLPTIM_IT(SLPTIM_IT));
  
  /* Force the Interrupt by setting the dedicated interrupt in the IFR register */
  SLPTMR->IFR |= SLPTIM_IT;
}

/**
  * @brief  Enables or disables the specified SLPTIM interrupts.
  * @param  SLPTIM_IT: specifies the SLPTIM interrupts sources to be enabled or disabled.
  *   This parameter can be any combination of the following values:
  *     @arg SLPTIM_IT_WRAP: Sleep timer overflow
  *     @arg SLPTIM_IT_CMPA: Sleep timer compare A
  *     @arg SLPTIM_IT_CMPB: Sleep timer compare B  
  * @param  NewState: new state of the SLPTIM interrupts.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SLPTIM_ITConfig(uint32_t SLPTIM_IT, FunctionalState NewState)
{  
  /* Check the parameters */
  assert_param(IS_SLPTIM_IT(SLPTIM_IT));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
    if (NewState != DISABLE)
    {
      /* Enable the Interrupt sources */
      SLPTMR->IER |= SLPTIM_IT;
    }
    else
    {
      /* Disable the Interrupt sources */
      SLPTMR->IER &= (uint32_t)~SLPTIM_IT;
    }
}

/**
  * @brief  Checks whether the specified SLPTIM flag is set or not.
  * @param  SLPTIM_FLAG: specifies the flag to check.
  *   This parameter can be one of the following values:
  *     @arg SLPTIM_FLAG_WRAP: Sleep timer overflow
  *     @arg SLPTIM_FLAG_CMPA: Sleep timer compare A
  *     @arg SLPTIM_FLAG_CMPB: Sleep timer compare B
  * @note        
  * @retval The new state of SLPTIM_FLAG (SET or RESET).
  */
FlagStatus SLPTIM_GetFlagStatus(uint32_t SLPTIM_FLAG)
{ 
  ITStatus bitstatus = RESET; 
  
  /* Check the parameters */
  assert_param(IS_SLPTIM_FLAG(SLPTIM_FLAG));
  
    if ((SLPTMR->ISR & SLPTIM_FLAG) != (uint32_t)RESET)
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
  * @brief  Clears the specified SLPTIM flag.
  * @param  SLPTIM_FLAG: specifies the flag to clear.
  *   This parameter can be one of the following values:
  *     @arg SLPTIM_FLAG_WRAP: Sleep timer overflow
  *     @arg SLPTIM_FLAG_CMPA: Sleep timer compare A
  *     @arg SLPTIM_FLAG_CMPB: Sleep timer compare B
  * @note        
  * @retval The new state of SLPTIM_FLAG (SET or RESET).
  */
void SLPTIM_ClearFlag(uint32_t SLPTIM_FLAG)
{
  /* Check the parameters */
  assert_param(IS_SLPTIM_FLAG(SLPTIM_FLAG));
  
  /* Clear the flags */  
  SLPTMR->ISR |= (uint32_t)SLPTIM_FLAG;
}

/**
  * @brief  Checks whether the specified SLPTMR pending interrupt has occurred or not
  * @param  SLPTIM_IT: specifies the flag to check.
  *         This parameter can be one of the following values:
  *   This parameter can be one of the following values:
  *     @arg SLPTIM_FLAG_WRAP: Sleep timer overflow
  *     @arg SLPTIM_FLAG_CMPA: Sleep timer compare A
  *     @arg SLPTIM_FLAG_CMPB: Sleep timer compare B
  * @retval The new state of SLPTIM_IT (SET or RESET).
  */
ITStatus SLPTIM_GetITStatus(uint32_t SLPTIM_IT)
{
  ITStatus bitstatus = RESET;
  uint32_t enablestatus = 0;
  /* Check the parameters */
  assert_param(IS_SLPTIM_IT(SLPTIM_IT));

  enablestatus = (uint32_t)(SLPTMR->IER & SLPTIM_IT);
  if (((SLPTMR->ISR & SLPTIM_IT) != (uint32_t)RESET) && enablestatus) 
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  return  bitstatus;
}

/**
  * @brief  Clears the specified SLPTIM flag.
  * @param  SLPTIM_IT: specifies the flag to clear.
  *   This parameter can be one of the following values:
  *     @arg SLPTIM_FLAG_WRAP: Sleep timer overflow
  *     @arg SLPTIM_FLAG_CMPA: Sleep timer compare A
  *     @arg SLPTIM_FLAG_CMPB: Sleep timer compare B
  * @note        
  * @retval The new state of SLPTIM_FLAG (SET or RESET).
  */
void SLPTIM_ClearITPendingBit(uint32_t SLPTIM_IT)
{
  /* Check the parameters */
  assert_param(IS_SLPTIM_IT(SLPTIM_IT));
  
  /* Clear the pending interrupt */  
  SLPTMR->ISR |= (uint32_t)SLPTIM_IT;
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
  
/**
  * @}
  */
  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
