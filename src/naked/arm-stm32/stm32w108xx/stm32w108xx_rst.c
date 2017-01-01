/**
  ******************************************************************************
  * @file    stm32w108xx_rst.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    30-November-2012
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the RST peripheral
  *
  *  @verbatim
  *  
 ===============================================================================
                        ##### RST specific features #####
 ===============================================================================
    [..] This driver provide the information about reset source
                    
    [..] The reset can be due to:
         (#) Core lockup 
         (#) Option byte load failure (may be set with other bits  
         (#) Wake-up from Deep Sleep
         (#) Software reset
         (#) Watchdog expiration
         (#) External reset pin signal
         (#) The application of a Core power supply (or previously failed)
         (#) Normal power applied 


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
#include "stm32w108xx_rst.h"

/** @addtogroup STM32W108xx_StdPeriph_Driver
  * @{
  */

/** @defgroup RESET
  * @brief RST driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup RST_Private_Functions 
  * @{
  */
  
/** @defgroup RST_Group1 
 *  @brief   Reset event sources 
 *
@verbatim
 ===============================================================================
                        ##### RST EVENT SOURCES #####
 ===============================================================================
    [..] This section provides function allowing to get reset event source

@endverbatim
  * @{
  */

/**
  * @brief  Checks whether the specified RST flag is set or not.
  * @param  RST_FLAG: specifies the RST_FLAG flag to check.
  *   This parameter can be one of the following values:
  *     @arg RST_FLAG_PWRHV: Normal power applied
  *     @arg RST_FLAG_PWRLV: The application of a Core power supply (or previously failed)
  *     @arg RST_FLAG_PIN: External reset pin signal.    
  *     @arg RST_FLAG_WDG: Watchdog expiration
  *     @arg RST_FLAG_SWRST: Software reset.
  *     @arg RST_FLAG_WKUP: Wake-up from Deep Sleep
  *     @arg RST_FLAG_OBFAIL: Option byte load failure (may be set with other bits)
  *     @arg RST_FLAG_LKUP: Core lockup          
  *   
  * @retval The new state of RST_FLAG (SET or RESET)
  */
FlagStatus RST_GetFlagStatus(uint32_t RST_FLAG)
{
  FlagStatus bitstatus = RESET;  
  /* Check the parameters */
  assert_param(IS_RST_FLAG(RST_FLAG));
  
  if ((RST->SR & RST_FLAG) != (uint32_t)RESET)
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
