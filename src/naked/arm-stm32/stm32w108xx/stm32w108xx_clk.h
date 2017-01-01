/**
  ******************************************************************************
  * @file    stm32w108xx_clk.h
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    30-November-2012
  * @brief   This file contains all the functions prototypes for the CLK firmware 
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
#ifndef __STM32W108XX_CLK_H
#define __STM32W108XX_CLK_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32w108xx.h"

/** @addtogroup STM32W108xx_StdPeriph_Driver
  * @{
  */

/** @addtogroup CLK
  * @{
  */ 

/* Exported types ------------------------------------------------------------*/

/** @defgroup CLK_Exported_Constants
  * @{
  */ 

/** @defgroup CLK_HSE_configuration 
  * @{
  */
#define CLK_MODE0                       ((uint32_t)0x00000000)
#define CLK_MODE1                       ((uint32_t)0x00000001)
#define CLK_MODE2                       ((uint32_t)0x00000010)
#define CLK_MODE3                       ((uint32_t)0x00000011)

#define IS_CLK_MODE(MODE) (((MODE) == CLK_MODE0) || ((MODE) == CLK_MODE1) || \
                           ((MODE) == CLK_MODE2) || ((MODE) == CLK_MODE3))
/**
  * @}
  */ 


/** @defgroup SLPTIM_Clocks 
  * @{
  */
#define SLPTIM_CLK_32KH        ((uint32_t)CLK_SLEEPCR_LSEEN)
#define SLPTIM_CLK_10KH        ((uint32_t)CLK_SLEEPCR_LSI10KEN)
#define IS_SLPTIM_GET_CLK(CLK) (((CLK) == SLPTIM_CLK_32KH) || \
                                ((CLK) == SLPTIM_CLK_10KH))  
/**
  * @}
  */
   
   
/** @defgroup Period_Measering_Mode 
  * @{
  */
#define MEASURE_CLKRC                       ((uint32_t)0x00000000)
#define MEASURE_OSCHF                       ((uint32_t)0x00000001)
#define MEASURE_TUNEFILT                    ((uint32_t)0x00000002)
#define IS_CLK_MEASURE(MEASURE) (((MEASURE) == MEASURE_CLKRC) || ((MEASURE) == MEASURE_OSCHF) || \
                                 ((MEASURE) == MEASURE_TUNEFILT))
/**
  * @}
  */

#define IS_LSI1KCRINT(CALINT) (((CALINT) <= 0x0000001F))
#define IS_LSI1KCRFRAC(CALFRAC) (((CALFRAC) <= 0x000007FF))
   
#define IS_CLK_TUNE_VALUE(TUNE_VALUE) (((TUNE_VALUE) <= 0x0000000F))
/**
  * @}
  */ 

/** @defgroup CLK_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup CLK_Exported_Functions
  * @{
  */
/*  Function used to set the CLK configuration to the default reset state *****/ 
void CLK_DeInit(void);

/* Internal/external clocks configuration functions *********/
void CLK_InternalCalibrateLSI(void);
void CLK_InternalCalibrateHSI(void);
void CLK_Config(uint8_t MODE);
void CLK_HSECmd(FunctionalState NewState);
void CLK_SLPTIMClockConfig(uint32_t CLK_SLPTIM, FunctionalState NewState);
void CLK_1KClockCalibration(uint32_t CALINT, uint32_t CALFRAC);
void CLK_RCTuneConfig(uint32_t TUNE_VALUE);
void CLK_MeasurePeriod(uint32_t CLK_MEASURED);
uint32_t CLK_GetMeasurePeriod(void);
uint32_t CLK_GetClocksFreq(void);

/**
  * @}
  */
  
#ifdef __cplusplus
}
#endif

#endif /*__STM32W108XX_CLK_H */

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

