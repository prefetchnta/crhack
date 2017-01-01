/**
  ******************************************************************************
  * @file    stm32w108xx_adc.h
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    30-November-2012
  * @brief   This file contains all the functions prototypes for the ADC firmware 
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
#ifndef __STM32W108XX_ADC_H
#define __STM32W108XX_ADC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32w108xx.h"

/** @addtogroup STM32W108xx_StdPeriph_Driver
  * @{
  */

/** @addtogroup  ADC
  * @{
  */ 

/* Exported types ------------------------------------------------------------*/

/** 
  * @brief  ADC Init structures definition  
  */ 
typedef struct
{
  uint32_t ADC_Resolution;                /*!< Selects the resolution of the conversion. 
                                              This parameter can be a value of ADC_Resolution */
  uint32_t ADC_VoltageP;                 /*!< Selects the voltage range for the P input channel.
                                              This parameter can be a Low voltage range or High voltage range */
  uint32_t ADC_VoltageN;                 /*!< Selects the voltage range for the N input channel. 
                                              This parameter can be a Low voltage range or High voltage range */
  uint32_t ADC_Input;                      /*!< Selects the channels. 
                                              This parameter must range from 0x0 to 0x5 and from 0x9 to 0xB. 
                                              The other values are reserved */
  uint32_t ADC_Clock;                     /*!< Selects the ADC clock. 
                                              This parameter must be 0 or 1 */ 
  uint32_t ADC_DMAMode;                     /*!< Selects the ADC DMA mode. 
                                              This parameter must be linear or auto wrap */ 
  int32_t ADC_Offset;                    /*!< Specifies the offset added to the basic ADC conversion result. 
                                              This parameter must be 16 bits signed offset */
  uint32_t ADC_Gain;                      /*!< Specifies the gain factor that is multiplied by the offset-corrected ADC result 
                                              to produce the output value. 
                                              This parameter must be 16 bits unsigned gain */
}ADC_InitTypeDef;


/** @defgroup ADC_Exported_Constants
  * @{
  */ 

/** @defgroup ADC_resolution 
  * @{
  */ 
#define ADC_Resolution_12b                         ((uint32_t)0x0000E000)
#define ADC_Resolution_11b                         ((uint32_t)0x0000C000)
#define ADC_Resolution_10b                         ((uint32_t)0x0000A000)
#define ADC_Resolution_9b                          ((uint32_t)0x00008000)
#define ADC_Resolution_8b                          ((uint32_t)0x00006000)
#define ADC_Resolution_7b                          ((uint32_t)0x00004000)
#define ADC_Resolution_6b                          ((uint32_t)0x00002000)
#define ADC_Resolution_5b                          ((uint32_t)0x00000000)

#define IS_ADC_RESOLUTION(RESOLUTION) (((RESOLUTION) == ADC_Resolution_12b) || \
                                       ((RESOLUTION) == ADC_Resolution_11b) || \
                                       ((RESOLUTION) == ADC_Resolution_10b) || \
                                       ((RESOLUTION) == ADC_Resolution_9b) || \
                                       ((RESOLUTION) == ADC_Resolution_8b) || \
                                       ((RESOLUTION) == ADC_Resolution_7b) || \
                                       ((RESOLUTION) == ADC_Resolution_6b) || \
                                       ((RESOLUTION) == ADC_Resolution_5b))

/**
  * @}
  */ 

/** @defgroup ADC_VoltageP 
  * @{
  */
#define ADC_VoltageP_Low                      ((uint32_t)0x00000000)
#define ADC_VoltageP_High                     ((uint32_t)0x00001000)

#define IS_ADC_VoltageP(VOLTAGE_P) (((VOLTAGE_P) == ADC_VoltageP_Low) || \
                                    ((VOLTAGE_P) == ADC_VoltageP_High))

/**
  * @}
  */ 

/** @defgroup ADC_VoltageN 
  * @{
  */ 
#define ADC_VoltageN_Low                      ((uint32_t)0x00000000)
#define ADC_VoltageN_High                     ((uint32_t)0x00000800)

#define IS_ADC_VoltageN(VOLTAGE_N) (((VOLTAGE_N) == ADC_VoltageN_Low) || \
                                    ((VOLTAGE_N) == ADC_VoltageN_High))
/**
  * @}
  */ 

/** @defgroup ADC_Channels
  * @{
  */ 
#define ADC_MUX_ADC0    ((uint32_t)0x00000000)   /* Channel 0 : ADC0 on PB5 */
#define ADC_MUX_ADC1    ((uint32_t)0x00000001)   /* Channel 1 : ADC0 on PB6 */
#define ADC_MUX_ADC2    ((uint32_t)0x00000002)   /* Channel 2 : ADC0 on PB7 */
#define ADC_MUX_ADC3    ((uint32_t)0x00000003)   /* Channel 3 : ADC0 on PC1 */
#define ADC_MUX_ADC4    ((uint32_t)0x00000004)   /* Channel 4 : ADC0 on PA4 */
#define ADC_MUX_ADC5    ((uint32_t)0x00000005)   /* Channel 5 : ADC0 on PA5 */
#define ADC_MUX_GND     ((uint32_t)0x00000008)   /* Channel 8 : VSS (0V) - not for high voltage range */
#define ADC_MUX_VREF2   ((uint32_t)0x00000009)   /* Channel 9 : VREF/2 (0.6V) */
#define ADC_MUX_VREF    ((uint32_t)0x0000000A)   /* Channel A : VREF (1.2V)*/
#define ADC_MUX_VREG2   ((uint32_t)0x0000000B)   /* Channel B : Regulator/2 (0.9V) - not for high voltage range */

#define ADC_MUXN_BITS   ((uint32_t)0x00000004)   /* number of bits to shift */

/* ADC_SOURCE_<pos>_<neg> selects <pos> as the positive input and <neg> as the negative input */

#define  ADC_SOURCE_ADC0_VREF2  ((ADC_MUX_ADC0 << ADC_MUXN_BITS) | ADC_MUX_VREF2)
#define  ADC_SOURCE_ADC0_GND    ((ADC_MUX_ADC0 << ADC_MUXN_BITS) | ADC_MUX_GND)

#define  ADC_SOURCE_ADC1_VREF2  ((ADC_MUX_ADC1 << ADC_MUXN_BITS) | ADC_MUX_VREF2)
#define  ADC_SOURCE_ADC1_GND    ((ADC_MUX_ADC1 << ADC_MUXN_BITS) | ADC_MUX_GND)

#define  ADC_SOURCE_ADC2_VREF2  ((ADC_MUX_ADC2 << ADC_MUXN_BITS) | ADC_MUX_VREF2)
#define  ADC_SOURCE_ADC2_GND    ((ADC_MUX_ADC2 << ADC_MUXN_BITS) | ADC_MUX_GND)

#define  ADC_SOURCE_ADC3_VREF2  ((ADC_MUX_ADC3 << ADC_MUXN_BITS) | ADC_MUX_VREF2)
#define  ADC_SOURCE_ADC3_GND    ((ADC_MUX_ADC3 << ADC_MUXN_BITS) | ADC_MUX_GND)

#define  ADC_SOURCE_ADC4_VREF2  ((ADC_MUX_ADC4 << ADC_MUXN_BITS) | ADC_MUX_VREF2)

#define  ADC_SOURCE_ADC5_VREF2  ((ADC_MUX_ADC5 << ADC_MUXN_BITS) | ADC_MUX_VREF2)

#define  ADC_SOURCE_ADC1_ADC0   ((ADC_MUX_ADC1 << ADC_MUXN_BITS) | ADC_MUX_ADC0)
#define  ADC_SOURCE_ADC0_ADC1   ((ADC_MUX_ADC1 << ADC_MUXN_BITS) | ADC_MUX_ADC0)

#define  ADC_SOURCE_ADC3_ADC2   ((ADC_MUX_ADC3 << ADC_MUXN_BITS) | ADC_MUX_ADC2)
#define  ADC_SOURCE_ADC2_ADC3   ((ADC_MUX_ADC3 << ADC_MUXN_BITS) | ADC_MUX_ADC2)

#define  ADC_SOURCE_ADC5_ADC4   ((ADC_MUX_ADC5 << ADC_MUXN_BITS) | ADC_MUX_ADC4)

#define  ADC_SOURCE_GND_VREF2   ((ADC_MUX_GND  << ADC_MUXN_BITS) | ADC_MUX_VREF2)
#define  ADC_SOURCE_GND         ((ADC_MUX_GND  << ADC_MUXN_BITS) | ADC_MUX_GND)

#define  ADC_SOURCE_VREF_VREF2  ((ADC_MUX_VREF << ADC_MUXN_BITS) | ADC_MUX_VREF2)
#define  ADC_SOURCE_VREF        ((ADC_MUX_VREF << ADC_MUXN_BITS) | ADC_MUX_GND)

#define  ADC_SOURCE_VREF2_VREF2 ((ADC_MUX_VREF2 << ADC_MUXN_BITS) | ADC_MUX_VREF2)
#define  ADC_SOURCE_VREF2       ((ADC_MUX_VREF2 << ADC_MUXN_BITS) | ADC_MUX_GND)

#define  ADC_SOURCE_VREG2_VREF2 ((ADC_MUX_VREG2 << ADC_MUXN_BITS) | ADC_MUX_VREF2)
#define  ADC_SOURCE_VDD_GND     ((ADC_MUX_VREG2 << ADC_MUXN_BITS) | ADC_MUX_GND)

#define IS_ADC_CHANNEL(CHANNELS) (((CHANNELS) == ADC_SOURCE_ADC0_VREF2) || \
                                 ((CHANNELS) == ADC_SOURCE_ADC0_GND) || \
                                 ((CHANNELS) == ADC_SOURCE_ADC1_VREF2) || \
                                 ((CHANNELS) == ADC_SOURCE_ADC1_GND) || \
                                 ((CHANNELS) == ADC_SOURCE_ADC2_VREF2) || \
                                 ((CHANNELS) == ADC_SOURCE_ADC2_GND) || \
                                 ((CHANNELS) == ADC_SOURCE_ADC3_VREF2) || \
                                 ((CHANNELS) == ADC_SOURCE_ADC3_GND) || \
                                 ((CHANNELS) == ADC_SOURCE_ADC4_VREF2) || \
                                 ((CHANNELS) == ADC_SOURCE_ADC5_VREF2) || \
                                 ((CHANNELS) == ADC_SOURCE_ADC1_ADC0) || \
                                 ((CHANNELS) == ADC_SOURCE_ADC0_ADC1) || \
                                 ((CHANNELS) == ADC_SOURCE_ADC3_ADC2) || \
                                 ((CHANNELS) == ADC_SOURCE_ADC2_ADC3) || \
                                 ((CHANNELS) == ADC_SOURCE_ADC5_ADC4) || \
                                 ((CHANNELS) == ADC_SOURCE_GND_VREF2) || \
                                 ((CHANNELS) == ADC_SOURCE_GND) || \
                                 ((CHANNELS) == ADC_SOURCE_VREF_VREF2) || \
                                 ((CHANNELS) == ADC_SOURCE_VREF) || \
                                 ((CHANNELS) == ADC_SOURCE_VREF2_VREF2 ) || \
                                 ((CHANNELS) == ADC_SOURCE_VREF2) || \
                                 ((CHANNELS) == ADC_SOURCE_VREG2_VREF2) || \
                                 ((CHANNELS) == ADC_SOURCE_VDD_GND))

/**
  * @}
  */ 

/** @defgroup ADC_DMAMode 
  * @{
  */

#define ADC_DMAMode_Linear                   ((uint32_t)0x00000000)
#define ADC_DMAMode_AutoWrap                 ((uint32_t)0x00000002)

#define IS_ADC_DMA_MODE(MODE) (((MODE) == ADC_DMAMode_Linear) || \
                                 ((MODE) == ADC_DMAMode_AutoWrap))
/**
  * @}
  */ 


/** @defgroup ADC_clock
  * @{
  */

#define ADC_Clock_1MHz                      ADC_CR_CLK
#define ADC_Clock_6MHz                      0x00000000

#define IS_ADC_CLOCK(CLOCK) (((CLOCK) == ADC_Clock_1MHz) || \
                             ((CLOCK) == ADC_Clock_6MHz))
/**
  * @}
  */ 

/** @defgroup ADC_offset 
  * @{
  */ 
#define IS_ADC_OFFSET(OFFSET) ((OFFSET) <= 0x0000FFFF)
/**
  * @}
  */ 


/** @defgroup ADC_gain 
  * @{
  */ 
#define IS_ADC_GAIN(GAIN) ((GAIN) <= 0x0000FFFF) 
/**
  * @}
  */ 

/** @defgroup ADC_begin
  * @{
  */ 
#if defined STM32W108CC
#define IS_ADC_BEGIN(BEGIN) (((BEGIN) <= 0x20003FFE) && ((BEGIN) >= 0x20000000) && (((BEGIN) & 0x00000001) == 0 )) 
#else
#define IS_ADC_BEGIN(BEGIN) (((BEGIN) <= 0x20001FFE) && ((BEGIN) >= 0x20000000) && (((BEGIN) & 0x00000001) == 0 )) 
#endif
/**
  * @}
  */ 

/** @defgroup ADC_size
  * @{
  */ 
#if defined STM32W108CC
#define IS_ADC_SIZE(SIZE) ((SIZE) <= 0x00001FFF)
#else
#define IS_ADC_SIZE(SIZE) ((SIZE) <= 0x00000FFF)
#endif
/**
  * @}
  */ 

/** @defgroup ADC_interrupts_definition 
  * @{
  */ 

#define  ADC_IT_DMABHF                ((uint32_t)0x00000002)
#define  ADC_IT_DMABF                 ((uint32_t)0x00000004)
#define  ADC_IT_SAT                   ((uint32_t)0x00000008)
#define  ADC_IT_DMAOVF                ((uint32_t)0x00000010)

#define IS_ADC_IT(IT) (((IT) == ADC_IT_DMABHF )|| \
                       ((IT) == ADC_IT_DMABF)|| \
                       ((IT) == ADC_IT_SAT)|| \
                       ((IT) == ADC_IT_DMAOVF))

/**
  * @}
  */ 

/** @defgroup ADC_DMA_flags_definition 
  * @{
  */
#define  ADC_FLAG_ACT                      ((uint32_t)0x00000001)
#define  ADC_FLAG_OVF                      ((uint32_t)0x00000002)

#define IS_ADC_DMA_FLAG(FLAG) (((FLAG) == ADC_FLAG_ACT) || \
                               ((FLAG) == ADC_FLAG_OVF))

/**
  * @}
  */ 

/** @defgroup ADC_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup ADC_Exported_Functions
  * @{
  */
  
/* Function used to set the ADC configuration to the default reset state *****/ 
void ADC_DeInit(void);

/* Initialization and Configuration functions *********************************/ 
void ADC_Init(ADC_InitTypeDef* ADC_InitStruct);
void ADC_StructInit(ADC_InitTypeDef* ADC_InitStruct);
void ADC_Cmd(FunctionalState NewState);

/* ADC Channels Configuration functions ***************************************/
void ADC_ChannelConfig(uint32_t ADC_Channels);

/* DMA Configuration functions ************************************************/
void ADC_DMA_ChannelLoadEnable(void);
void ADC_DMA_ChannelReset(void);
void ADC_DMA_Config(uint32_t ADC_DMABeg, uint32_t ADC_DMASize);
uint32_t ADC_DMA_GetNextAddress(void);
uint32_t ADC_DMA_GetCounter(void);

/* Interrupts and flags management functions **********************************/
void ADC_ITConfig(uint32_t ADC_IT, FunctionalState NewState);
FlagStatus ADC_DMA_GetFlagStatus(uint32_t ADC_DMA_FLAG);
ITStatus ADC_GetITStatus(uint32_t ADC_IT);
void ADC_ClearITPendingBit(uint32_t ADC_IT);

/**
  * @}
  */
  
#ifdef __cplusplus
}
#endif

#endif /*__STM32W108XX_ADC_H */

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

