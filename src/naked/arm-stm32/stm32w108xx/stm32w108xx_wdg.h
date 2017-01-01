/**
  ******************************************************************************
  * @file    stm32w108xx_wdg.h
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    30-November-2012
  * @brief   This file contains all the functions prototypes for the WDG 
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
#ifndef __STM32W108XX_WDG_H
#define __STM32W108XX_WDG_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32w108xx.h"

/** @addtogroup STM32W108xx_StdPeriph_Driver
  * @{
  */

/** @addtogroup WDG
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/** @defgroup WDG_Exported_Constants
  * @{
  */

/** @defgroup WDG_DebugStatus
  * @{
  */
#define WDG_DBG_RUN          ((uint32_t)0x00000000)
#define WDG_DBG_PAUSE        ((uint32_t)0x00000400)
#define IS_WDG_DEBUG_STATUS(STATUS)  (((STATUS) == WDG_DBG_RUN) || ((STATUS) == WDG_DBG_PAUSE)) 
/**
  * @}
  */

/**
  * @}
  */

/** @defgroup WDG_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup WDG_Exported_Functions
  * @{
  */
/* WDG activation function ****************************************************/
void WDG_DeInit(void);
void WDG_ReloadCounter(void);
void WDG_Cmd(FunctionalState NewState);
void WDG_DebugConfig(uint32_t DBG_STATUS);
FunctionalState WDG_GetStatus(void);

/**
  * @}
  */
  
#ifdef __cplusplus
}
#endif

#endif /* __STM32W108XX_WDG_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

