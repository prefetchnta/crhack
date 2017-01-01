/**
  ******************************************************************************
  * @file    stm32w108xx_pwr.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    30-November-2012
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the power management (PWR):
  *           + Voltage Regulator control
  *           + WakeUp Pin/Source Configuration
  *           + DeepSleep mode
  *           + WakeUp status
  *
  *  @verbatim
  ===============================================================================
                       ##### How to use this driver #####
  ===============================================================================
  [..] This driver provides the Low level functions to manage the low level power
       registers. These functions are split in 4 groups

       (#) Voltage Regulator control functions: this group includes the 
           management of following features using PWR_VREGInit() function:
           (++) Configure the regulator Trim values
           (++) Enable/Disable VREF, V1.8 and V1.2 voltage regulators 

       (#) WakeUp Pin/Source Configuration functions: this group includes all 
           needed to configure an interrupt as WakeUp source:
           (++) To control the GPIO pin to WakeUp the system from low power mode use the 
                PWR_GPIOWakeUpPinCmd() function.
           (++) To configure the WakeUp method to wake the system from low power mode use 
                the PWR_WakeUpSourceConfig() function.
           (++) To command the WakeUp source filter use PWR_WakeUpFilterConfig() function.

       (#) DeepSleep mode functions: this group includes the deep sleep feature 
           configuration:
           (++) To freeze the GPIO state before entering in low power mode use
                the PWR_FreezestateLVoutput() function.
           (++) To control the deep sleep mode 0 when debugger is attached use
                the PWR_DeepSleepMode0Cmd() function.
           (++) To Wake the core from deep sleep 0  the WakeUp source filter use
                 PWR_CoreWake() function.
           (++) To Disable the system access to the ACK bit in the CSYSPWRUPACKSR use
                 PWR_InhibitCSYSPWRUPACK() function.

       (#) WakeUp Status functions: this group includes the required functions to 
           manage the WakeUp interrupt status:
           (++) When the system wake up from low power mode use PWR_GetFlagStatus()
                to check witch interrupt is the source for WakeUp.
           (++) After check the user should clear the WakeUp source in the low power
                status register using PWR_ClearFlag() function.

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
#include "stm32w108xx_pwr.h"

/** @addtogroup STM32W108xx_StdPeriph_Driver
  * @{
  */

/** @defgroup POWER_MANAGEMENT
  * @brief PWR driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup PWR_Private_Functions
  * @{
  */

/** @defgroup PWR_Group1 Voltage Regulator control
 *  @brief  Voltage regulator VREF, V1.8 and V1.2 control 
 *
@verbatim   
 ===============================================================================
                 #####  Voltage Regulator control function #####
 ===============================================================================  

@endverbatim
  * @{
  */

/**
  * @brief  Deinitializes the PWR peripheral registers to their default reset values.
  * @param  None
  * @retval None
  */
void PWR_DeInit(void)
{
  PWR->DSLEEPCR1 = 0x00000000;
  PWR->DSLEEPCR2 = 0x00000001;
  PWR->VREGCR = 0x00000207;
  PWR->WAKECR1 = 0x00000200;
  PWR->WAKECR2 = 0x00000000;
  PWR->WAKESR = 0x000003FF;
  PWR->CSYSPWRUPACKCR = 0x00000000;
  PWR->WAKEPAR = 0x00000000;
  PWR->WAKEPBR = 0x00000000;
  PWR->WAKEPCR = 0x00000000;
  PWR->WAKEFILTR = 0x00000000;
}

/**
  * @brief  Fills each VREG_InitStruct member with its default value.
  * @param  VREG_InitStruct: pointer to a PWR_VREG_InitTypeDef structure
  *   which will be initialized.
  * @retval None
  */
void PWR_VREGStructInit(PWR_VREG_InitTypeDef* VREG_InitStruct)
{
  /* VREG_InitStruct members default value */
  VREG_InitStruct->PWR_VREFCmd = POWER_ENABLE; /* by default enable */
  VREG_InitStruct->PWR_1V8Cmd = POWER_ENABLE;  /* by default enable */
  VREG_InitStruct->PWR_1V8TRIM = 4;
  VREG_InitStruct->PWR_1V2Cmd = POWER_ENABLE;  /* by default enable */
  VREG_InitStruct->PWR_1V2TRIM = 7;  
}

/**
  * @brief Initializes the VREG peripheral according to the specified
  *   parameters in the VREG_InitStruct.
  * @param  VREG_InitStruct: pointer to a PWR_VREG_InitTypeDef structure
  *   that contains the configuration information for the specified VREG.
  * @retval None
  */
void PWR_VREGInit(PWR_VREG_InitTypeDef* VREG_InitStruct)
{
  uint32_t temp = 0;

  /* Check the parameters */
  assert_param(IS_POWER_FUNCTIONAL_STATE(VREG_InitStruct->PWR_VREFCmd));
  assert_param(IS_POWER_FUNCTIONAL_STATE(VREG_InitStruct->PWR_1V8Cmd));
  assert_param(IS_TRIM_VALUE(VREG_InitStruct->PWR_1V8TRIM));
  assert_param(IS_POWER_FUNCTIONAL_STATE(VREG_InitStruct->PWR_1V2Cmd));
  assert_param(IS_TRIM_VALUE(VREG_InitStruct->PWR_1V2TRIM));

  temp = (uint32_t)(((VREG_InitStruct->PWR_1V8TRIM) << 7) | (VREG_InitStruct->PWR_1V2TRIM));

  /* Check the new VREF status */
  if (VREG_InitStruct->PWR_VREFCmd == POWER_DISABLE)
  {
    /* Disable VREF */
    temp |= PWR_VREGCR_VREFEN;
  }

  /* Check the new 1V8 status */
  if (VREG_InitStruct->PWR_1V8Cmd == POWER_DISABLE)
  {
    /* Disable 1V8 */
    temp |= PWR_VREGCR_1V8EN;
  }
  
  /* Check the new 1V2 status */
  if (VREG_InitStruct->PWR_1V2Cmd == POWER_DISABLE)
  {
    /* Disable 1V2 */
    temp |= PWR_VREGCR_1V2EN;
  }

  /* Set the new VREG configuration */
  PWR->VREGCR = temp;
}

/**
  * @}
  */

/** @defgroup PWR_Group2 WakeUp Pin/Source Configuration
 *  @brief  Low Power mode source WakeUp and method configuration
 *
@verbatim   
 ===============================================================================
                 ##### WakeUp Pin-Source Configuration function #####
 ===============================================================================  

@endverbatim
  * @{
  */

/**
  * @brief  Enables or disables the GPIO WakeUp pin.
  * @param  GPIOx: where x can be (A, B or C) to select the GPIO peripheral.
  * @param  GPIO_Pin: specifies the port bit to be written.
  *   This parameter can be one of GPIO_Pin_x where x can be (0..7).
  * @param  NewState: new state of the GPIO WakeUp pin source.
  *         This parameter can be: ENABLE or DISABLE.
  * @note The GPIO WakeUp monitoring should be enabled before enabling the GPIO WakeUp pin.
  *       To enable the GPIO WakeUp monitoring use PWR_WakeUpSourceConfig() function.
  * @retval None
  */
void PWR_GPIOWakeUpPinCmd(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GET_GPIO_PIN(GPIO_Pin));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (GPIOx == GPIOA)
  {
    if (NewState != DISABLE)
    {
      PWR->WAKEPAR |= (uint32_t)GPIO_Pin;
    }
    else
    {
      PWR->WAKEPAR &= (uint32_t)~GPIO_Pin;
    }
  }
  else if (GPIOx == GPIOB)
  {
    if (NewState != DISABLE)
    {
      PWR->WAKEPBR |= (uint32_t)GPIO_Pin;
    }
    else
    {
      PWR->WAKEPBR &= (uint32_t)~GPIO_Pin;
    }
  }
  else
  {
    if (GPIOx == GPIOC)
    {
      if (NewState != DISABLE)
      {
        PWR->WAKEPCR |= (uint32_t)GPIO_Pin;
      }
      else
      {
        PWR->WAKEPCR &= (uint32_t)~GPIO_Pin;
      }
    }
  }
}

/**
  * @brief  Enables or disables the WakeUp source filter.
  * @param  PWR_WakeUpSource: specifies the selected PWR WakeUp source.
  *          This parameter can be one of the following values:
  *            @arg PWR_WAKEFILTER_GPIO: filter active on GPIO monitoring.
  *            @arg PWR_WAKEFILTER_SC1: filter active on SC1.
  *            @arg PWR_WAKEFILTER_SC2: filter active on SC2.
  *            @arg PWR_WAKEFILTER_IRQD: filter active on IRQD.
  * @param  NewState: new state of the WakeUp source.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void PWR_WakeUpFilterConfig(uint32_t PWR_WakeUpSource, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_PWR_WAKEUPFILTERSOURCE(PWR_WakeUpSource));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the dedicated WakeUp filter by setting the dedicated bit in the 
       WAKEFILTR register */
    PWR->WAKEFILTR |= PWR_WakeUpSource;
  }
  else
  {
    /* Disable the dedicated WakeUp filter by clearing the dedicated bit in the WAKEFILTR register */
    PWR->WAKEFILTR &= (uint32_t)~((uint32_t)PWR_WakeUpSource);
  }
}

/**
  * @brief  Enables or disables the WakeUp method form low power mode.
  * @param  PWR_WakeUpSource: specifies the selected PWR wakeup method.
  *          This parameter can be one of the following values:
  *            @arg PWR_WAKEUP_CSYSPWRRUPREQ: Wake up active on CSYSPWRUPREQ event.
  *            @arg PWR_WAKEUP_CPWRRUPREQ: Wake up active on CPWRRUPREQ event.
  *            @arg PWR_WAKEUP_CORE: Wake up active on COREWAKE event.
  *            @arg PWR_WAKEUP_WRAP: Wake up active on sleep timer compare wrap/overflow event.
  *            @arg PWR_WAKEUP_COMPB: Wake up active on sleep timer compare B event.
  *            @arg PWR_WAKEUP_COMPA: Wake up active on sleep timer compare A event.
  *            @arg PWR_WAKEUP_IRQD: Wake up active on falling/rising edge of pin PC0.
  *            @arg PWR_WAKEUP_SC2: Wake up active on falling/rising edge of pin PA2 for SC2.
  *            @arg PWR_WAKEUP_SC1: Wake up active on falling/rising edge of pin PB2 for SC12.
  *            @arg PWR_WAKEUP_MON: Wake up active on GPIO monitoring.
  * @param  NewState: new state of the WakeUp source.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void PWR_WakeUpSourceConfig(uint32_t PWR_WakeUpSource, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_PWR_WAKEUPSOURCE(PWR_WakeUpSource));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the WakeUp from low power method by setting the dedicated bit in the WAKECR1 register */
    PWR->WAKECR1 |= PWR_WakeUpSource;
  }
  else
  {
    /* Disable the WakeUp from low power method by clearing the dedicated bit in the WAKECR1 register */
    PWR->WAKECR1 &= (uint32_t)~((uint32_t)PWR_WakeUpSource);
  }
}

/**
  * @}
  */

/** @defgroup PWR_Group3 DeepSleep mode
 *  @brief control the DeepSleep mode features
 *
@verbatim   
 ===============================================================================
                         #####  DeepSleep mode function #####
 ===============================================================================  

@endverbatim
  * @{
  */

/**
  * @brief  Enables or disables the freeze GPIO state LV output.
  * @param  NewState: new freeze state of the GPIO state LV output.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void PWR_FreezestateLVoutput(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable freeze GPIO state LV output by setting the LVFREEZE bit in the DSLEEPCR1 register */
    PWR->DSLEEPCR1 |= PWR_DSLEEPCR1_LVFREEZE;
  }
  else
  {
    /* Disable freeze GPIO state LV output from low power method by clearing the LVFREEZE bit in the DSLEEPCR1 register */
    PWR->DSLEEPCR1 &= (uint32_t)~((uint32_t)PWR_DSLEEPCR1_LVFREEZE);
  }
}

/**
  * @brief  Enables or disables the deep sleep mode 0 when debugger is attached.
  * @param  NewState: new freeze state of the GPIO state LV output.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void PWR_DeepSleepMode0Cmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable deep sleep mode 0 if debugger attached by setting the MODE bit in the DSLEEPCR2 register */
    PWR->DSLEEPCR2 |= PWR_DSLEEPCR2_MODE;
  }
  else
  {
    /* Disable deep sleep mode 0 if debugger by clearing the MODE bit in the DSLEEPCR2 register */
    PWR->DSLEEPCR2 &= (uint32_t)~((uint32_t)PWR_DSLEEPCR2_MODE);
  }
}

/**
  * @brief  Wake core form a deep sleep 0.
  * @param  None
  * @retval None
  */
void PWR_CoreWake(void)
{
  /* Wake core from deep sleep 0 by setting the COREWAKE bit in the WAKECR2 register */
  PWR->WAKECR2 |= PWR_WAKECR2_COREWAKE;
}

/**
  * @brief  Disables the cortex-M3 system access to the ACK bit in the CSYSPWRUPACKSR register.
  * @param  None
  * @retval None
  */
void PWR_InhibitCSYSPWRUPACK(void)
{
  /* Disable the system access to the ACK bit in the CSYSPWRUPACKSR by setting 
  the INHIBIT bit in the CSYSPWRUPACKCR register */
  PWR->CSYSPWRUPACKCR |= PWR_CSYSPWRUPACKCR_INHIBIT;
}

/**
  * @}
  */

/** @defgroup PWR_Group4 WakeUp status
 *  @brief  Control the low power WakeUp source status 
 *
@verbatim   
 ===============================================================================
                          #####  WakeUp status function #####
 ===============================================================================  

@endverbatim
  * @{
  */

/**
  * @brief  Checks whether the specified PWR flag is set or not.
  * @param  PWR_FLAG: specifies the low power wake up flag to check.
  *   This parameter can be one of the following values:
  *     @arg PWR_FLAG_CSYSPWRRUPREQ: Wake up done using the DAP access to SYS registers flag
  *     @arg PWR_FLAG_CPWRRUPREQ: Wake up done using the DAP access to DBG registers flag
  *     @arg PWR_FLAG_CORE: Wake up done using debug port activity flag
  *     @arg PWR_FLAG_WRAP: Wake up done using sleep timer wrap flag
  *     @arg PWR_FLAG_COMPB: Wake up done using sleep timer compare B flag
  *     @arg PWR_FLAG_COMPA: Wake up done using sleep timer compare A flag
  *     @arg PWR_FLAG_IRQD: Wake up done using external interrupt IRQD flag
  *     @arg PWR_FLAG_SC2: Wake up done using serial controller 2 (PA2) flag
  *     @arg PWR_FLAG_SC1: Wake up done using serial controller 1 (PB2) flag
  *     @arg PWR_FLAG_MON: Wake up done using GPIO monitoring flag
  *     @arg PWR_FLAG_CPWRUPREQ: REQ flag in the CPWRUPREQSR register
  *     @arg PWR_FLAG_CSYSPWRUPREQ: REQ flag in the CSYSPWRUPREQSR register
  *     @arg PWR_FLAG_CSYSPWRUPREQ: ACK flag in the CSYSPWRUPREQSR register
  * @retval The new state of PWR_FLAG (SET or RESET).
  */
FlagStatus PWR_GetFlagStatus(uint32_t PWR_FLAG)
{
  FlagStatus bitstatus = RESET;

  /* Check the parameters */
  assert_param(IS_PWR_GET_FLAG(PWR_FLAG));

  /* Check the REQ flag in the CPWRUPREQSR register */
  if (PWR_FLAG == PWR_FLAG_CPWRUPREQ)
  {
    if ((PWR->CPWRUPREQSR & 0x00000001) != (uint32_t)RESET)
    {
      bitstatus = SET;
    }
    else
    {
      bitstatus = RESET;
    }
  }
  /* Check the REQ flag in the CSYSPWRUPREQSR register */
  else if (PWR_FLAG == PWR_FLAG_CSYSPWRUPREQ)
  {
    if ((PWR->CSYSPWRUPREQSR & 0x00000001) != (uint32_t)RESET)
    {
      bitstatus = SET;
    }
    else
    {
      bitstatus = RESET;
    }
  }
  /* Check the ACK flag in the CSYSPWRUPACKSR register */
  else if (PWR_FLAG == PWR_FLAG_CSYSPWRUPREQ)
  {
    if ((PWR->CSYSPWRUPACKSR & 0x00000001) != (uint32_t)RESET)
    {
      bitstatus = SET;
    }
    else
    {
      bitstatus = RESET;
    }
  }
  /* Check the Wake up flag in the WAKESR register */
  else
  {
    if ((PWR->WAKESR & PWR_FLAG) != (uint32_t)RESET)
    {
      bitstatus = SET;
    }
    else
    {
      bitstatus = RESET;
    }
  }
  return bitstatus;
}

/**
  * @brief  Clears the PWR pending flags.
  * @param  PWR_FLAG: specifies the low power wake up flag to clear.
  *   This parameter can be one of the following values:
  *     @arg PWR_FLAG_CSYSPWRRUPREQ: Wake up done using the DAP access to SYS registers flag
  *     @arg PWR_FLAG_CPWRRUPREQ: Wake up done using the DAP access to DBG registers flag
  *     @arg PWR_FLAG_CORE: Wake up done using debug port activity flag
  *     @arg PWR_FLAG_WRAP: Wake up done using sleep timer wrap flag
  *     @arg PWR_FLAG_COMPB: Wake up done using sleep timer compare B flag
  *     @arg PWR_FLAG_COMPA: Wake up done using sleep timer compare A flag
  *     @arg PWR_FLAG_IRQD: Wake up done using external interrupt IRQD flag
  *     @arg PWR_FLAG_SC2: Wake up done using serial controller 2 (PA2) flag
  *     @arg PWR_FLAG_SC1: Wake up done using serial controller 1 (PB2) flag
  *     @arg PWR_FLAG_MON: Wake up done using GPIO monitoring flag
  * @retval The new state of PWR_FLAG (SET or RESET).
  */
void PWR_ClearFlag(uint32_t PWR_FLAG)
{
  /* Check the parameters */
  assert_param(IS_PWR_CLEAR_FLAG(PWR_FLAG));

  PWR->WAKESR = PWR_FLAG;
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
