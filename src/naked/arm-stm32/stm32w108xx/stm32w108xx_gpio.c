/**
  ******************************************************************************
  * @file    stm32w108xx_gpio.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    30-November-2012
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the GPIO peripheral:           
  *           + Initialization and Configuration
  *           + GPIO Read and Write
  *  @verbatim
  *
================================================================================
                         ##### How to use this driver #####
================================================================================
      [..]
      (#) Configure the GPIO pin(s) using GPIO_Init().
          Four possible configuration are available for each pin:
         (++) Input: Floating, Pull-up, Pull-down.
         (++) Output: Push-Pull (Pull-up, Pull-down or no Pull)
                      Open Drain (Pull-up, Pull-down or no Pull).
         (++) Alternate Function: Push-Pull (PP or SPI mode)
                                 Open Drain (Pull-up, Pull-down or no Pull).
         (++) Analog
         
      (#) To get the level of a pin configured in input mode use GPIO_ReadInputDataBit()
      (#) To set/reset the level of a pin configured in output mode use
          GPIO_SetBits()/GPIO_ResetBits()
      (#) During and just after reset, the alternate functions are not active and 
          the GPIO pins are configured in input floating mode (except JTAG pins).
      (#) A full chip reset affects the GPIO configuration as follows:
          (++) All pins are configured as floating inputs.
          (++) The GPIO_EXTREGEN bit is set which overrides the normal configuration for PA7.
          (++) The GPIO_DBGDIS bit is cleared, allowing Serial Wire/JTAG access 
              to override the normal configuration of PC0, PC2, PC3, and PC4.
              
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
#include "stm32w108xx_gpio.h"

/** @addtogroup STM32W108xx_StdPeriph_Driver
  * @{
  */

/** @defgroup GPIO 
  * @brief GPIO driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/** @defgroup GPIO_Private_Functions
  * @{
  */

/** @defgroup GPIO_Group1 Initialization and Configuration
 *  @brief   Initialization and Configuration
 *
@verbatim   
 ===============================================================================
                        Initialization and Configuration
 ===============================================================================  

@endverbatim
  * @{
  */

/**
  * @brief  Deinitializes the GPIOx peripheral registers to their default reset values.
  * @param  GPIOx: where x can be (A..C) to select the GPIO peripheral.
  * @retval None
  */
void GPIO_DeInit(GPIO_TypeDef* GPIOx)
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));

  GPIOx->CRL = 0x00004444;
  GPIOx->CRH = 0x00004444;
  GPIOx->IDR = 0x00000000;
  GPIOx->ODR = 0x00000000;
  GPIOx->BSR = 0x00000000;
  GPIOx->BRR = 0x00000000;

  GPIO_DBG->PCTRACECR = 0x00000000;
  GPIO_DBG->DBGCR = 0x00000010;
}

/**
  * @brief  Initializes the GPIOx peripheral according to the specified 
  *         parameters in the GPIO_InitStruct.
  * @param  GPIOx: where x can be (A, B or C) to select the GPIO peripheral.
  * @param  GPIO_InitStruct: pointer to a GPIO_InitTypeDef structure that 
  *         contains the configuration information for the specified GPIO
  *         peripheral.
  *         GPIO_Pin: selects the pin to be configured: GPIO_Pin_0 -> GPIO_Pin_7
  *         GPIO_Mode: selects the mode of the pin: 
  *                          - GPIO Analog Mode: GPIO_Mode_AN 
  *                          - GPIO Output Mode PP: GPIO_Mode_OUT_PP
  *                          - GPIO Input Mode NOPULL: GPIO_Mode_IN
  *                          - GPIO Output Mode OD: GPIO_Mode_OUT_OD
  *                          - GPIO Input Mode PuPd: GPIO_Mode_IN_PUD 
  *                          - GPIO Alternate function Mode PP: GPIO_Mode_AF_PP
  *                          - GPIO Alternate function Mode SPI SCLK PP: GPIO_Mode_AF_PP_SPI
  *                          - GPIO Alternate function Mode OD: GPIO_Mode_AF_OD
  * @retval None
  */
void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct)
{
  uint32_t currentmode = 0x00, currentpin = 0x00, pinpos = 0x00, pos = 0x00;
  uint32_t tmpreg = 0x00, pinmask = 0x00;
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GPIO_MODE(GPIO_InitStruct->GPIO_Mode));
  assert_param(IS_GPIO_PIN(GPIO_InitStruct->GPIO_Pin));
  
  /*---------------------------- GPIO Mode Configuration -----------------------*/
  currentmode = ((uint32_t)GPIO_InitStruct->GPIO_Mode) & ((uint32_t)0x0F);
  
  /*---------------------------- GPIO CRL Configuration ------------------------*/
  /* Configure the four low port pins */
  if (((uint32_t)GPIO_InitStruct->GPIO_Pin & ((uint32_t)0x0F)) != 0x00)
  {
    tmpreg = GPIOx->CRL;
    for (pinpos = 0x00; pinpos < 0x04; pinpos++)
    {
      pos = ((uint32_t)0x01) << pinpos;
      /* Get the port pins position */
      currentpin = (GPIO_InitStruct->GPIO_Pin) & pos;
      if (currentpin == pos)
      {
        pos = pinpos << 2;
        /* Clear the corresponding low control register bits */
        pinmask = ((uint32_t)0x0F) << pos;
        tmpreg &= ~pinmask;
        /* Write the mode configuration in the corresponding bits */
        tmpreg |= (currentmode << pos);
      }
    }
    GPIOx->CRL = tmpreg;
  }
  /*---------------------------- GPIO CRH Configuration ------------------------*/
  /* Configure the four high port pins */
  if (GPIO_InitStruct->GPIO_Pin > 0x0F)
  {
    tmpreg = GPIOx->CRH;
    for (pinpos = 0x00; pinpos < 0x04; pinpos++)
    {
      pos = (((uint32_t)0x01) << (pinpos + 0x04));
      /* Get the port pins position */
      currentpin = ((GPIO_InitStruct->GPIO_Pin) & pos);
      if (currentpin == pos)
      {
        pos = pinpos << 2;
        /* Clear the corresponding high control register bits */
        pinmask = ((uint32_t)0x0F) << pos;
        tmpreg &= ~pinmask;
        /* Write the mode configuration in the corresponding bits */
        tmpreg |= (currentmode << pos);
      } 
    }
    GPIOx->CRH = tmpreg;
  }
}

/**
  * @brief  Fills each GPIO_InitStruct member with its default value.
  * @param  GPIO_InitStruct : pointer to a GPIO_InitTypeDef structure which will 
  *         be initialized.
  * @retval None
  */
void GPIO_StructInit(GPIO_InitTypeDef* GPIO_InitStruct)
{
  /* Reset GPIO init structure parameters values */
  GPIO_InitStruct->GPIO_Pin  = GPIO_Pin_All;
  GPIO_InitStruct->GPIO_Mode = GPIO_Mode_IN;
}

/**
  * @}
  */

/** @defgroup GPIO_Group2 GPIO Read and Write
 *  @brief   GPIO Read and Write
 *
@verbatim   
 ===============================================================================
                              GPIO Read and Write
 ===============================================================================  

@endverbatim
  * @{
  */

/**
  * @brief  Reads the specified input port pin.
  * @param  GPIOx: where x can be (A, B or C) to select the GPIO peripheral.
  * @param  GPIO_Pin: specifies the port bit to read.
  *   This parameter can be GPIO_Pin_x where x can be (0..7).
  * @retval The input port pin value.
  */
uint32_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin)
{
  uint32_t bitstatus = 0x00;
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GET_GPIO_PIN(GPIO_Pin));
  
  if ((GPIOx->IDR & GPIO_Pin) != (uint32_t)Bit_RESET)
  {
    bitstatus = (uint32_t)Bit_SET;
  }
  else
  {
    bitstatus = (uint32_t)Bit_RESET;
  }
  return bitstatus;
}

/**
  * @brief  Reads the specified GPIO input data port.
  * @param  GPIOx: where x can be (A..C) to select the GPIO peripheral.
  * @retval GPIO input data port value.
  */
uint32_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx)
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  
  return ((uint32_t)GPIOx->IDR);
}

/**
  * @brief  Reads the specified output data port bit.
  * @param  GPIOx: where x can be (A, B or C) to select the GPIO peripheral.
  * @param  GPIO_Pin: Specifies the port bit to read.
  *   This parameter can be GPIO_Pin_x where x can be (0..7).
  * @retval The output port pin value.
  */
uint32_t GPIO_ReadOutputDataBit(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin)
{
  uint32_t bitstatus = 0x00;
  
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GET_GPIO_PIN(GPIO_Pin));
  
  if ((GPIOx->ODR & GPIO_Pin) != (uint32_t)Bit_RESET)
  {
    bitstatus = (uint32_t)Bit_SET;
  }
  else
  {
    bitstatus = (uint32_t)Bit_RESET;
  }
  return bitstatus;
}

/**
  * @brief  Reads the specified GPIO output data port.
  * @param  GPIOx: where x can be (A, B or C) to select the GPIO peripheral.
  * @retval GPIO output data port value.
  */
uint32_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx)
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  
  return ((uint32_t)GPIOx->ODR);
}

/**
  * @brief  Sets the selected data port bits.
  * @param  GPIOx: where x can be (A, B or C) to select the GPIO peripheral.
  * @param  GPIO_Pin: specifies the port bits to be written.
  *   This parameter can be any combination of GPIO_Pin_x where x can be (0..7).
  * @note  This functions uses GPIOx_SET register to allow atomic read/modify 
  *        accesses. In this way, there is no risk of an IRQ occurring between
  *        the read and the modify access.
  * @retval None
  */
void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin)
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GPIO_PIN(GPIO_Pin));
  
  GPIOx->BSR = GPIO_Pin;
}

/**
  * @brief  Clears the selected data port bits.
  * @param  GPIOx: where x can be (A, B or C) to select the GPIO peripheral.
  * @param  GPIO_Pin: specifies the port bits to be written.
  *   This parameter can be any combination of GPIO_Pin_x where x can be (0..7).
  * @note  This functions uses GPIOx_CLR register to allow atomic read/modify 
  *        accesses. In this way, there is no risk of an IRQ occurring between
  *        the read and the modify access.
  * @retval None
  */
void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin)
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GPIO_PIN(GPIO_Pin));
  
  GPIOx->BRR = GPIO_Pin;
}

/**
  * @brief  Sets or clears the selected data port bit.
  * @param  GPIOx: where x can be (A, B or C) to select the GPIO peripheral.
  * @param  GPIO_Pin: specifies the port bit to be written.
  *   This parameter can be one of GPIO_Pin_x where x can be (0..7).
  * @param  BitVal: specifies the value to be written to the selected bit.
  *   This parameter can be one of the BitAction enum values:
  *     @arg Bit_RESET: to clear the port pin
  *     @arg Bit_SET: to set the port pin
  * @retval None
  */
void GPIO_WriteBit(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin, BitAction BitVal)
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GET_GPIO_PIN(GPIO_Pin));
  assert_param(IS_GPIO_BIT_ACTION(BitVal));
  
  if (BitVal != Bit_RESET)
  {
    GPIOx->BSR = GPIO_Pin;
  }
  else
  {
    GPIOx->BRR = GPIO_Pin ;
  }
}

/**
  * @brief  Writes data to the specified GPIO data port.
  * @param  GPIOx: where x can be (A, B or C) to select the GPIO peripheral.
  * @param  PortVal: specifies the value to be written to the port output data 
  *                  register.
  * @retval None
  */
void GPIO_Write(GPIO_TypeDef* GPIOx, uint16_t PortVal)
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  
  GPIOx->ODR = PortVal;
}
/**
  * @}
  */

/** @defgroup GPIO_Group3 GPIO Wake and Debug Configuration 
 *  @brief  GPIO Wake and Debug Configuration 
 *
@verbatim   
 ===============================================================================
                        Debug Configuration
 ===============================================================================  

@endverbatim
  * @{
  */
  
/**
  * @brief  Selects PC_TRACE source on bb_debug GPIO pins.
  * @param  PCTRACE_SEL: specifies the PC_TRACE source on bb_debug GPIO pins.
  *   This parameter can be :
  *     @arg GPIO_BBDEBUG: bb debug.
  *     @arg GPIO_PCTRACE: pc trace.
  * @retval None.
  */
void GPIO_PCTraceConfig(uint32_t PCTRACE_SEL)
{
  /* Check the parameters */
  assert_param(IS_GPIO_PCTRACE(PCTRACE_SEL));

  GPIO_DBG->PCTRACECR = PCTRACE_SEL;
}

/**
  * @brief  Enables or disables the debug interface.
  * @param  NewState: new state of the debug interface.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void GPIO_DebugInterfaceCmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* Clear the DEBUGDIS bit to Enable the debug interface */
    GPIO_DBG->DBGCR &= (uint32_t)~GPIO_DBGCR_DBGDIS;

  }
  else
  {
    /* Set the DEBUGDIS bit to Disable the debug interface */
    GPIO_DBG->DBGCR |= (uint32_t)GPIO_DBGCR_DBGDIS;
  }
}

/**
  * @brief  Enables or Disable REG_EN override of PA7's normal GPIO configuration.
  * @param  NewState: new state of the REG_EN.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void GPIO_ExternalOverrideCmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* Clear the GPIO_EXTREGEN bit to Enable the debug interface */
    GPIO_DBG->DBGCR &= (uint32_t)~GPIO_DBGCR_EXTREGEN;

  }
  else
  {
    /* Set the GPIO_EXTREGEN bit to Disable the debug interface */
    GPIO_DBG->DBGCR |= (uint32_t)GPIO_DBGCR_EXTREGEN;
  }
}

/**
  * @brief  Checks whether the specified GPIO debug flag is set or not.
  * @param  GPIO_DBGFLAG: specifies the flag to check. 
  *   This parameter can be one of the following values:
  *     @arg GPIO_DBGSR_SWEN: Serial Wire interface flag
  *     @arg GPIO_DBGSR_FORCEDBG: Debugger interface flag
  *     @arg GPIO_DBGSR_BOOTMODE: nBOOTMODE signal sampled at the end of reset flag
  * @retval The new state of GPIO_DBGFLAG (SET or RESET).
  */
FlagStatus GPIO_GetDebugFlagStatus(uint16_t GPIO_DBGFLAG)
{
  FlagStatus bitstatus = RESET;

  /* Check the parameters */
  assert_param(IS_GPIO_GET_DBGFLAG(GPIO_DBGFLAG));

  /* Check the status of the specified GPIO debug flag */
  if ((GPIO_DBG->DBGSR & GPIO_DBGFLAG) != (uint32_t)RESET)
  {
    /* GPIO_DBGFLAG is set */
    bitstatus = SET;
  }
  else
  {
    /* GPIO_DBGFLAG is reset */
    bitstatus = RESET;
  }
  /* Return the GPIO_DBGFLAG status */
  return  bitstatus;
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
