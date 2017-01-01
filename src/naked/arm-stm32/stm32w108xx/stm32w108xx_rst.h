/**
  ******************************************************************************
  * @file    stm32w108xx_rst.h
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    30-November-2012
  * @brief   This file contains all the functions prototypes for the RST firmware 
  *          library.
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
#ifndef __STM32W108XX_RST_H
#define __STM32W108XX_RST_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32w108xx.h"

/** @addtogroup STM32W108xx_StdPeriph_Driver
  * @{
  */

/** @addtogroup RST
  * @{
  */ 

/* Exported types ------------------------------------------------------------*/

/** @defgroup RST_Exported_Constants
  * @{
  */ 

/** @defgroup RESET_Event 
  * @{
  */
#define RST_FLAG_PWRHV                       ((uint32_t)RST_SR_PWRHV)
#define RST_FLAG_PWRLV                       ((uint32_t)RST_SR_PWRLV)  
#define RST_FLAG_PIN                         ((uint32_t)RST_SR_PIN)           
#define RST_FLAG_WDG                         ((uint32_t)RST_SR_WDG)      
#define RST_FLAG_SWRST                       ((uint32_t)RST_SR_SWRST)  
#define RST_FLAG_WKUP                        ((uint32_t)RST_SR_WKUP)       
#define RST_FLAG_OBFAIL                      ((uint32_t)RST_SR_OBFAIL)  
#define RST_FLAG_LKUP                        ((uint32_t)RST_SR_LKUP)         

#define IS_RST_FLAG(FLAG) (((FLAG) == RST_FLAG_PWRHV) || ((FLAG) == RST_FLAG_PWRLV) || \
                           ((FLAG) == RST_FLAG_PIN) || ((FLAG) == RST_FLAG_WDG)     || \
                           ((FLAG) == RST_FLAG_SWRST) || ((FLAG) == RST_FLAG_WKUP)  || \
                           ((FLAG) == RST_FLAG_OBFAIL) || ((FLAG) == RST_FLAG_LKUP))
/**
  * @}
  */
   
/**
  * @}
  */
  
/** @defgroup RST_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup RST_Exported_Functions
  * @{
  */
FlagStatus RST_GetFlagStatus(uint32_t RST_FLAG);

/**
  * @}
  */
  
#ifdef __cplusplus
}
#endif

#endif /*__STM32W108XX_RST_H */

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

