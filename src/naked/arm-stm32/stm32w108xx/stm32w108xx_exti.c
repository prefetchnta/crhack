/**
  ******************************************************************************
  * @file    stm32w108xx_exti.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    30-November-2012
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the EXTI peripheral:           
  *           + Initialization and Configuration
  *           + Interrupts and flags management
  *             
  *  @verbatim  
  *  
  ============================================================================== 
                          ##### EXTI features #####
  ==============================================================================              
    [..] External interrupt/event lines are mapped as following:
         (#) All available GPIO pins are connected to the 4 external 
             interrupt/event lines from EXTIA to EXTID.                             
         (#) EXTIA and EXTIB have fixed pins assignement (PB0 and PB6).
         (#) EXTIC and EXTID can use any GPIO pin.
                          
                          ##### How to use this driver ##### 
  ============================================================================== 
    [..] In order to use an I/O pin as an external interrupt source, follow
         steps below:
        (#) Configure the I/O in input mode using GPIO_Init()
        (#) Select the mode(interrupt, event) and configure the trigger selection 
            using EXTI_Init(). 
        (#) Configure NVIC IRQ channel mapped to the EXTI line using NVIC_Init().
                  
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
#include "stm32w108xx_exti.h"

/** @addtogroup STM32W108xx_StdPeriph_Driver
  * @{
  */

/** @defgroup EXTI 
  * @brief EXTI driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/** @defgroup EXTI_Private_Functions
  * @{
  */

/** @defgroup EXTI_Group1 EXTI Initialization and Configuration 
 *  @brief   Initialization and Configuration of External Interrupt
 *
@verbatim   
 ===============================================================================
                        EXTI Initialization  and Configuration
 ===============================================================================  

@endverbatim
  * @{
  */

/**
  * @brief  Deinitializes the EXTI_IRQn line registers to their default reset values.
  * @param  None
  * @retval None
  */
void EXTI_DeInit(void)
{
  uint8_t i;
  EXTI->PR = 0x00000000;
  EXTI->CR[0] = 0x0000000F;
  EXTI->CR[1] = 0x00000010;
  for (i=0; i<4; i++)
  {
    EXTI->TSR[i] = 0x00000000;
  }  
}

  
/**
  * @brief  Initializes the EXTI peripheral according to the specified
  *         parameters in the EXTI_InitStruct.
  * @param  EXTI_InitStruct: pointer to a EXTI_InitTypeDef structure
  *         that contains the configuration information for the EXTI peripheral.
  * @retval None
  */
void EXTI_Init(EXTI_InitTypeDef* EXTI_InitStruct)
{
  /* Check the parameters */
  assert_param(IS_EXTI_SOURCE(EXTI_InitStruct->EXTI_Source));
  assert_param(IS_EXTI_IRQ(EXTI_InitStruct->EXTI_IRQn));
  assert_param(IS_EXTI_TRIGGER(EXTI_InitStruct->EXTI_Trigger));
  assert_param(IS_FUNCTIONAL_STATE(EXTI_InitStruct->EXTI_LineCmd));
  assert_param(IS_FUNCTIONAL_STATE(EXTI_InitStruct->EXTI_DigitalFilterCmd));
  
  if (EXTI_InitStruct->EXTI_LineCmd != DISABLE)
  {
    /* Clear Rising Falling edge configuration */
    EXTI->TSR[(uint32_t)(EXTI_InitStruct->EXTI_IRQn & 0x000000F0) >>4] &= (uint32_t)(~EXTI_TSR_INTMOD);
    
    /* Rising Falling edge */
    EXTI->TSR[(uint32_t)(EXTI_InitStruct->EXTI_IRQn & 0x000000F0) >>4] |= (uint32_t)(EXTI_InitStruct->EXTI_Trigger);    
  }
  else
  {
    EXTI->TSR[(uint32_t)(EXTI_InitStruct->EXTI_IRQn & 0x000000F0) >>4] &= (uint32_t)(~EXTI_TSR_INTMOD);
  }
    
  if (EXTI_InitStruct->EXTI_DigitalFilterCmd != DISABLE)
  {
    /* Set the EXTI_TSR_FILTEN bit to Enable degital filtering on the EXTI_IRQn */
    EXTI->TSR[(uint32_t)(EXTI_InitStruct->EXTI_IRQn & 0x000000F0) >>4] |= (uint32_t)EXTI_TSR_FILTEN;
    
  }
  else
  {
    /* Clear the EXTI_TSR_FILTEN bit to disable degital filtering on the EXTI_IRQn */
    EXTI->TSR[(uint32_t)(EXTI_InitStruct->EXTI_IRQn & 0x000000F0) >>4] &= (uint32_t)~EXTI_TSR_FILTEN;
  }
  
  /* Connect The EXTI_PinSource to the EXTI_IRQn handler */
  if ((EXTI_InitStruct->EXTI_IRQn == EXTI_IRQC) || (EXTI_InitStruct->EXTI_IRQn == EXTI_IRQD))
  { 
    EXTI->CR[(uint32_t)(EXTI_InitStruct->EXTI_IRQn & 0x0000000F)] &= 0x00000000;
    EXTI->CR[(uint32_t)(EXTI_InitStruct->EXTI_IRQn & 0x0000000F)] |= (uint32_t)(EXTI_InitStruct->EXTI_Source);
  }
  else 
  {}
}
     
/**
  * @brief  Fills each EXTI_InitStruct member with its reset value.
  * @param  EXTI_InitStruct: pointer to a EXTI_InitTypeDef structure which will
  *         be initialized.
  * @retval None
  */
void EXTI_StructInit(EXTI_InitTypeDef* EXTI_InitStruct)
{
  EXTI_InitStruct->EXTI_Source = EXTI_SourcePB0;
  EXTI_InitStruct->EXTI_IRQn = EXTI_IRQA;
  EXTI_InitStruct->EXTI_Trigger = EXTI_Trigger_Disable;
  EXTI_InitStruct->EXTI_LineCmd = DISABLE;
  EXTI_InitStruct->EXTI_DigitalFilterCmd = DISABLE;
}

/** @defgroup EXTI_Group2 Interrupts and flags management functions
 *  @brief    Interrupts and flags management functions 
 *
@verbatim   
  ==============================================================================
             ##### Interrupts and flags management functions #####
  ==============================================================================
@endverbatim
  * @{
  */

/**
  * @brief  Checks whether the specified EXTI line is asserted or not.
  * @param  EXTI_IRQn: specifies the EXTI line to check.
  *   This parameter can be:
  *   EXTI_IRQn: External interrupt line n where x(A, B, C or D).
  * @retval The new state of EXTI_IRQn (SET or RESET).
  */
ITStatus EXTI_GetITStatus(uint32_t EXTI_IRQn)
{
  ITStatus bitstatus = RESET;
  /* Check the parameters */
  assert_param(IS_EXTI_IRQ(EXTI_IRQn));
  if ((EXTI->PR & (uint32_t)(1<< ((uint32_t)(EXTI_IRQn & 0x000000F0) >>4))) != (uint32_t)RESET)
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
  * @brief  Clears the EXTI's line pending bits.
  * @param  EXTI_IRQn: specifies the EXTI lines to clear.
  *   This parameter can be any combination of EXTI_IRQn where n can be (A, B, C or D).
  * @retval None
  */
void EXTI_ClearITPendingBit(uint32_t EXTI_IRQn)
{
  /* Check the parameters */
  assert_param(IS_EXTI_IRQ(EXTI_IRQn));
  
  EXTI->PR = (uint32_t)(1<< ((EXTI_IRQn & 0x000000F0) >>4));   
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
