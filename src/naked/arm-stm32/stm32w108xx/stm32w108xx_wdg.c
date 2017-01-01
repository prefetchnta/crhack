/**
  ******************************************************************************
  * @file    stm32w108xx_wdg.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    30-November-2012
  * @brief   This file provides firmware functions to use the watchdog (WDG) peripheral           
  *
  *  @verbatim  
  *  
================================================================================ 
                          ##### WDG features #####
================================================================================              
    [..] The watchdog timer uses the calibrated 1 kHz clock (CLK1K) as its reference
         and provides a nominal 2.048 s timeout. A low water mark interrupt occurs 
         at 1.760 s and triggers an NMI to the Cortex-M3 NVIC as an early warning. 
         When enabled, periodically reset the watchdog timer before it expires.
                             
    [..] By default, the WDG is disabled at power up of the always-on power domain.
              
    [..] The watchdog timer can be paused when the debugger halts the core.
                          
                          ##### How to use this driver ##### 
================================================================================ 
    [..] This driver allows to use WDG peripheral.   
    [..] Start the WDG using WDG_Cmd() function.        
    [..] Restart the WDG timer using WDG_ReloadCounter() function.
    [..] Specifies the staus of WDG timer during debug mode using WDG_DebugConfig() function.

              
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
#include "stm32w108xx_wdg.h"

/** @addtogroup STM32W108xx_StdPeriph_Driver
  * @{
  */

/** @defgroup WDG 
  * @brief WDG driver modules
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* ---------------------- WDG registers bit mask -----------------------------*/
#define KICKSR_KEY_RELOAD    ((uint32_t)0xAAAAAAAA)
#define KR_KEY_ENABLE        ((uint32_t)0x0000EABE)
#define KR_KEY_DISABLE       ((uint32_t)0x0000DEAD)
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup WDG_Private_Functions
  * @{
  */

/** @defgroup WDG_Group1 WDG activation function
 *  @brief   WDG activation function 
 *
@verbatim   
 ==============================================================================
                          ##### WDG activation function #####
 ==============================================================================  

@endverbatim
  * @{
  */

/**
  * @brief  Deinitializes the WDG peripheral registers to their default reset values.
  * @param  None
  * @retval None
  */
void WDG_DeInit(void)
{
  WDG->CR = 0x00000002;
  WDG->KR = 0x00000000;
  WDG->KICKSR = 0x00000000;
}

/**
  * @brief  Reloads WDG counter with value defined in the restart register
  * @param  None
  * @retval None
  */
void WDG_ReloadCounter(void)
{
  WDG->KICKSR = KICKSR_KEY_RELOAD;
}

/**
  * @brief  Enables/Disables WDG.
  * @param  NewState: new state of the WDG timer.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None.
  */
void WDG_Cmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  { 
    WDG->KR = 0x00000000; 
    WDG->KR |= KR_KEY_ENABLE;
    WDG->CR = WDG_CR_WDGEN;
  }
  else
  {
    WDG->KR = 0x00000000; 
    WDG->KR |= KR_KEY_DISABLE;
    WDG->CR = WDG_CR_WDGDIS;
  }
}

/**
  * @brief  Status of WDG timer while in debug mode.
  * @param  DBG_STATUS: specifies the staus of WDG timer during debug mode.
  *   This parameter can be one of the following values:
  *     @arg WDG_DBG_RUN: The timer continues working in Debug mode.
  *     @arg WDG_DBG_PAUSE: The timer is paused in Debug mode when the CPU is halted.
  * @retval None.
  */
void WDG_DebugConfig(uint32_t DBG_STATUS)
{
  /* Check the parameters */
  assert_param(IS_WDG_DEBUG_STATUS(DBG_STATUS));
  
  SLPTMR->CR = DBG_STATUS;
}

/**
  * @brief  Returns the status of WDG timer.
  * @param  None
  * @retval None
  */
FunctionalState WDG_GetStatus(void) 
{ 
  FunctionalState wdgstatus = DISABLE;

  if ((WDG->CR & WDG_CR_WDGEN) == WDG_CR_WDGEN)
  {
    wdgstatus = ENABLE;
  }
  else
  {
    wdgstatus = DISABLE;
  }
  return wdgstatus;
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
