/**
  ******************************************************************************
  * @file    stm32w108xx_pwr.h
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    30-November-2012
  * @brief   This file contains all the functions prototypes for the power  
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
#ifndef __STM32W108XX_PWR_H
#define __STM32W108XX_PWR_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32w108xx.h"

/** @addtogroup STM32W108xx_StdPeriph_Driver
  * @{
  */

/** @addtogroup POWER_MANAGEMENT
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/** 
  * @brief  VREG Init structure definition  
  */
typedef struct
{
  uint32_t PWR_VREFCmd;        /*!< Specifies the new state of the selected VREF.
                                    This parameter can be set either to ENABLE or DISABLE */
  uint32_t PWR_1V8Cmd;         /*!< Specifies the new state of the selected 1V8.
                                    This parameter can be set either to ENABLE or DISABLE */
  uint32_t PWR_1V8TRIM;        /*!< Specifies wether the 1V8 regulator trim value.*/
  uint32_t PWR_1V2Cmd;         /*!< Specifies the new state of the selected 1V2.
                                    This parameter can be set either to ENABLE or DISABLE */
  uint32_t PWR_1V2TRIM;        /*!< Specifies wether the 1V2 regulator trim value.*/
}PWR_VREG_InitTypeDef;
   

/** @defgroup PWR_Exported_Constants
  * @{
  */

/** @defgroup LowPower_VoltageCommand_definition 
  * @{
  */
#define POWER_ENABLE                         ((uint32_t)0x00)
#define POWER_DISABLE                        ((uint32_t)!POWER_ENABLE)

#define IS_POWER_FUNCTIONAL_STATE(STATE) (((STATE) == POWER_ENABLE) || ((STATE) == POWER_DISABLE))
/**
  * @}
  */

/** @defgroup LowPower_WakeUpFilter_definition 
  * @{
  */
#define PWR_WAKEFILTER_IRQD         ((uint32_t)PWR_WAKEFILTR_IRQD)
#define PWR_WAKEFILTER_SC2          ((uint32_t)PWR_WAKEFILTR_SC2)
#define PWR_WAKEFILTER_SC1          ((uint32_t)PWR_WAKEFILTR_SC1)
#define PWR_WAKEFILTER_GPIO         ((uint32_t)PWR_WAKEFILTR_GPIO)

#define IS_PWR_WAKEUPFILTERSOURCE(WAKEUP) (((WAKEUP) == PWR_WAKEFILTER_GPIO) || \
                                           ((WAKEUP) == PWR_WAKEFILTER_SC1) || \
                                           ((WAKEUP) == PWR_WAKEFILTER_SC2) || \
                                           ((WAKEUP) == PWR_WAKEFILTER_IRQD ))
/**
  * @}
  */

/** @defgroup LowPower_WakeUpMethod_definition 
  * @{
  */
#define PWR_WAKEUP_CSYSPWRRUPREQ            ((uint32_t)PWR_WAKECR1_CSYSPWRRUPREQ)
#define PWR_WAKEUP_CPWRRUPREQ               ((uint32_t)PWR_WAKECR1_CPWRRUPREQ)
#define PWR_WAKEUP_CORE                     ((uint32_t)PWR_WAKECR1_CORE)
#define PWR_WAKEUP_WRAP                     ((uint32_t)PWR_WAKECR1_WRAP)
#define PWR_WAKEUP_COMPB                    ((uint32_t)PWR_WAKECR1_COMPB)
#define PWR_WAKEUP_COMPA                    ((uint32_t)PWR_WAKECR1_COMPA)
#define PWR_WAKEUP_IRQD                     ((uint32_t)PWR_WAKECR1_IRQD)
#define PWR_WAKEUP_SC2                      ((uint32_t)PWR_WAKECR1_SC2)
#define PWR_WAKEUP_SC1                      ((uint32_t)PWR_WAKECR1_SC1)
#define PWR_WAKEUP_MON                      ((uint32_t)PWR_WAKECR1_MONEN)


#define IS_PWR_WAKEUPSOURCE(WAKEUP) (((WAKEUP) == PWR_WAKEUP_CSYSPWRRUPREQ) || \
                                     ((WAKEUP) == PWR_WAKEUP_CPWRRUPREQ) || \
                                     ((WAKEUP) == PWR_WAKEUP_CORE) || \
                                     ((WAKEUP) == PWR_WAKEUP_WRAP) || \
                                     ((WAKEUP) == PWR_WAKEUP_COMPB) || \
                                     ((WAKEUP) == PWR_WAKEUP_COMPA) || \
                                     ((WAKEUP) == PWR_WAKEUP_IRQD) || \
                                     ((WAKEUP) == PWR_WAKEUP_SC2) || \
                                     ((WAKEUP) == PWR_WAKEUP_SC1) || \
                                     ((WAKEUP) == PWR_WAKEUP_MON))
/**
  * @}
  */

/** @defgroup LowPower_flags_definition 
  * @{
  */
#define PWR_FLAG_CSYSPWRRUPREQ            ((uint32_t)PWR_WAKESR_CSYSPWRRUPREQ)
#define PWR_FLAG_CPWRRUPREQ               ((uint32_t)PWR_WAKESR_CPWRRUPREQ)
#define PWR_FLAG_CORE                     ((uint32_t)PWR_WAKESR_CORE)
#define PWR_FLAG_WRAP                     ((uint32_t)PWR_WAKESR_WRAP)
#define PWR_FLAG_COMPB                    ((uint32_t)PWR_WAKESR_COMPB)
#define PWR_FLAG_COMPA                    ((uint32_t)PWR_WAKESR_COMPA)
#define PWR_FLAG_IRQD                     ((uint32_t)PWR_WAKESR_IRQD)
#define PWR_FLAG_SC2                      ((uint32_t)PWR_WAKESR_SC2)
#define PWR_FLAG_SC1                      ((uint32_t)PWR_WAKESR_SC1)
#define PWR_FLAG_MON                      ((uint32_t)PWR_WAKESR_MON)

#define PWR_FLAG_CPWRUPREQ                ((uint32_t)0x10000001)
#define PWR_FLAG_CSYSPWRUPREQ             ((uint32_t)0x20000001)
#define PWR_FLAG_CSYSPWRUPACK             ((uint32_t)0x30000001)

#define IS_PWR_GET_FLAG(FLAG) (((FLAG) == PWR_FLAG_CSYSPWRRUPREQ) || ((FLAG) == PWR_FLAG_CPWRRUPREQ) || \
                               ((FLAG) == PWR_FLAG_CORE) || ((FLAG) == PWR_FLAG_WRAP) || \
                               ((FLAG) == PWR_FLAG_COMPB) || ((FLAG) == PWR_FLAG_COMPA) || \
                               ((FLAG) == PWR_FLAG_IRQD) || ((FLAG) == PWR_FLAG_SC2) || \
                               ((FLAG) == PWR_FLAG_SC1) || ((FLAG) == PWR_FLAG_MON) || \
                               ((FLAG) == PWR_FLAG_CPWRUPREQ) || ((FLAG) == PWR_FLAG_CSYSPWRUPREQ) || \
                               ((FLAG) == PWR_FLAG_CSYSPWRUPACK))

#define IS_PWR_CLEAR_FLAG(FLAG) (((FLAG) == PWR_FLAG_CSYSPWRRUPREQ) || ((FLAG) == PWR_FLAG_CPWRRUPREQ) || \
                                 ((FLAG) == PWR_FLAG_CORE) || ((FLAG) == PWR_FLAG_WRAP) || \
                                 ((FLAG) == PWR_FLAG_COMPB) || ((FLAG) == PWR_FLAG_COMPA) || \
                                 ((FLAG) == PWR_FLAG_IRQD) || ((FLAG) == PWR_FLAG_SC2) || \
                                 ((FLAG) == PWR_FLAG_SC1) || ((FLAG) == PWR_FLAG_MON))
/**
  * @}
  */

#define IS_TRIM_VALUE(TRIM) ((TRIM) < 8)

/**
  * @}
  */
  
/** @defgroup PWR_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup PWR_Exported_Functions
  * @{
  */
  
void PWR_DeInit(void);
void PWR_VREGStructInit(PWR_VREG_InitTypeDef* VREG_InitStruct);
void PWR_VREGInit(PWR_VREG_InitTypeDef* VREG_InitStruct);

void PWR_GPIOWakeUpPinCmd(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin, FunctionalState NewState);
void PWR_WakeUpFilterConfig(uint32_t PWR_WakeUpSource, FunctionalState NewState);
void PWR_WakeUpSourceConfig(uint32_t PWR_WakeUpSource, FunctionalState NewState);
void PWR_DeepSleepMode0Cmd(FunctionalState NewState);
void PWR_FreezestateLVoutput(FunctionalState NewState);
void PWR_CoreWake(void);
void PWR_InhibitCSYSPWRUPACK(void);
FlagStatus PWR_GetFlagStatus(uint32_t PWR_FLAG);
void PWR_ClearFlag(uint32_t PWR_FLAG);

/**
  * @}
  */
  
#ifdef __cplusplus
}

#endif

#endif /*__STM32W108XX_PWR_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

