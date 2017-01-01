  /**
  ******************************************************************************
  * @file    stm32w108xx_adc.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    30-November-2012
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the Analog to Digital Convertor (ADC) peripheral:
  *           + ADC initialization and Configuration 
  *           + DMA Configuration functions
  *           + Interrupts and flags management functions
  *         
  *  @verbatim
  * 
 ===============================================================================
                      ##### How to use this driver #####
 ===============================================================================                  
    [..] 
         (#) Configure GPIO pins to be used by the ADC in analog mode.
         (#) Configure the voltage reference (internal or external).
         (#) Set the offset and gain values.
         (#) Reset the ADC DMA, define the DMA buffer, and start the DMA in the 
             proper transfer mode. 
         (#) Write the ADC configuration register to define the inputs, voltage 
             range, sample time and start the conversions.
             
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

#include "stm32w108xx_adc.h"

/** @addtogroup STM32W108xx_StdPeriph_Driver
  * @{
  */

/** @defgroup ADC 
  * @brief ADC driver modules
  * @{
  */

/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* ADC INPUTN and INPUTP channel mask */
#define ADC_INPUT_NP_Reset  ((uint16_t)0xF807)

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/** @defgroup ADC_Private_Functions
  * @{
  */ 

/** @defgroup ADC_Group1 Initialization and Control functions
 *  @brief    Initialization and Control functions 
 *
@verbatim    
 ===============================================================================
                    ##### Initialization and Control functions #####
 ===============================================================================
  [..] This section provides functions allowing to:
       (+) Initialize and configure the ADC 
       (+) ADC Conversion Resolution (5bits --> 12bits)
       (+) Enable or disable the ADC peripheral
   
@endverbatim
  * @{
  */
  
/**
  * @brief  Deinitializes ADC peripheral registers to their default reset 
  *         values.
  * @param  None
  * @retval None
  */
void ADC_DeInit(void)
{
  ADC->ISR = 0x0000001F; 
  ADC->IER = 0x00000000; 
  ADC->CR = 0x00001800;
  ADC->OFFSETR = 0x00000000; 
  ADC->GAINR = 0x00008000;
  ADC->DMACR = 0x00000010;
  ADC->DMAMSAR = 0x20000000;
  ADC->DMANDTR = 0x00000000;
}

/**
  * @brief  Initializes the ADC peripheral according to the specified
  *         parameters in the ADC_InitStruct.
  * @param  ADC_InitStruct: pointer to ADC_InitTypeDef structure
  *         that contains the configuration information for the ADC peripheral.
  * @retval None
  */
void ADC_Init(ADC_InitTypeDef* ADC_InitStruct)
{
  uint32_t tmpreg = 0;
  
  /* Check the parameters */
  assert_param(IS_ADC_RESOLUTION(ADC_InitStruct->ADC_Resolution)); 
  assert_param(IS_ADC_VoltageP(ADC_InitStruct->ADC_VoltageP));
  assert_param(IS_ADC_VoltageN(ADC_InitStruct->ADC_VoltageN));
  assert_param(IS_ADC_CHANNEL(ADC_InitStruct->ADC_Input));
  assert_param(IS_ADC_DMA_MODE(ADC_InitStruct->ADC_DMAMode));  
  assert_param(IS_ADC_CLOCK(ADC_InitStruct->ADC_Clock));
  assert_param(IS_ADC_OFFSET(ADC_InitStruct->ADC_Offset));
  assert_param(IS_ADC_GAIN(ADC_InitStruct->ADC_Gain));  
            
  /*---------------------------- ADC CR Configuration -----------------------*/

  /* Configure ADC: scan conversion mode and resolution */
  /* Set ADC_1MHZCLK bit according to ADC_Clock value */
  /* Set ADC_MUX bits according to ADC_Input values */
  /* Set ADC_HVSELN bit according to ADC_VoltageN value */
  /* Set ADC_HVSELP bit according to ADC_VoltageP value */
  /* Set ADC_PERIOD[0:2] bits according to ADC_Clock value */
  tmpreg |= (uint32_t)(ADC_InitStruct->ADC_Resolution | ADC_InitStruct->ADC_VoltageP |
                       ADC_InitStruct->ADC_VoltageN | (uint32_t)((ADC_InitStruct->ADC_Input) << 0x3) | 
                       ADC_InitStruct->ADC_Clock);
                        
  /* Write to ADC CR */
  ADC->CR = (uint32_t)tmpreg;
 
  /*---------------------------- ADC DMA Configuration -----------------------*/
  /* Write to ADC DMA */
  ADC->DMACR = (uint32_t)(ADC_InitStruct->ADC_DMAMode);
 
  /*---------------------------- ADC ADC_OFFSETR Configuration ---------------*/
  /* Write to ADC ADC_OFFSETR */
  ADC->OFFSETR = (int32_t)(ADC_InitStruct->ADC_Offset);
  
  /*---------------------------- ADC ADC_GAINR Configuration -----------------*/
  /* Write to ADC ADC_GAINR */
  ADC->GAINR = (uint32_t)(ADC_InitStruct->ADC_Gain);
}

/**
  * @brief  Fills each ADC_InitStruct member with its default value
  * @param  ADC_InitStruct: pointer to a ADC_InitTypeDef structure
  *         which will be initialized.
  * @retval None
  */
void ADC_StructInit(ADC_InitTypeDef* ADC_InitStruct)
{
  /* Initialize the ADC resolution */
  ADC_InitStruct->ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStruct->ADC_VoltageP = ADC_VoltageP_Low;
  ADC_InitStruct->ADC_VoltageN = ADC_VoltageN_Low;
  ADC_InitStruct->ADC_Input = ADC_SOURCE_VREF;
  ADC_InitStruct->ADC_DMAMode = ADC_DMAMode_Linear;
  ADC_InitStruct->ADC_Clock = ADC_Clock_6MHz;
  ADC_InitStruct->ADC_Offset = 0;
  ADC_InitStruct->ADC_Gain = 0x00008000;
}

/**
  * @brief  Enables or disables the specified ADC peripheral
  * @param  NewState: new state of the ADC peripheral.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ADC_Cmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* Enable the ADC peripheral */
    ADC->CR |= (uint32_t)ADC_CR_ADON;
  }
  else
  {
    /* Disable the ADC peripheral */
    ADC->CR &= (uint32_t)(~ADC_CR_ADON); 
  }
}

/**
  * @}
  */

/** @defgroup ADC_Group2 ADC channel Configuration functions
 *  @brief   ADC channel Configuration functions
 *
@verbatim   
 ===============================================================================
                ##### ADC channel Configuration functions #####
 ===============================================================================
 [..] This section provides function allowing to configure the ADC channels

@endverbatim
  * @{
  */

/**
  * @brief  Selects the ADC channel
  * @param  ADC_Channels: specifies the ADC channel
  *         This parameter can be one of the following values:
  *           @arg ADC_SOURCE_ADC0_VREF2
  *           @arg ADC_SOURCE_ADC0_GND
  *           @arg ADC_SOURCE_ADC1_VREF2
  *           @arg ADC_SOURCE_ADC1_GND
  *           @arg ADC_SOURCE_ADC2_VREF2
  *           @arg ADC_SOURCE_ADC2_GND
  *           @arg ADC_SOURCE_ADC3_VREF2
  *           @arg ADC_SOURCE_ADC3_GND
  *           @arg ADC_SOURCE_ADC4_VREF2
  *           @arg ADC_SOURCE_ADC5_VREF2
  *           @arg ADC_SOURCE_ADC1_ADC0
  *           @arg ADC_SOURCE_ADC0_ADC1
  *           @arg ADC_SOURCE_ADC3_ADC2
  *           @arg ADC_SOURCE_ADC2_ADC3
  *           @arg ADC_SOURCE_ADC5_ADC4
  *           @arg ADC_SOURCE_GND_VREF2
  *           @arg ADC_SOURCE_VGND
  *           @arg ADC_SOURCE_VREF_VREF2
  *           @arg ADC_SOURCE_VREF
  * @retval None
  */
void ADC_ChannelConfig(uint32_t ADC_Channels)
{
  uint32_t tmpreg = 0;
  /* Check the parameters */
  assert_param(IS_ADC_CHANNEL(ADC_Channels));
  
  /* Get the old register value */
  tmpreg = ADC->CR;
  /* Clear the old channels */
  tmpreg &= ADC_INPUT_NP_Reset;
  /* Set the new channels */
  tmpreg |= (uint32_t)((ADC_Channels << 0x3));
  /* Store the new register value */
  ADC->CR = (uint32_t)tmpreg;
}

/**
  * @}
  */

/** @defgroup ADC_Group3 DMA Configuration functions
 *  @brief   DMA Configuration functions
 *
@verbatim
 ===============================================================================
                  ##### DMA Configuration functions #####
 ===============================================================================
 [..] This section provides functions allowing to:
        (+) initialize and configure the DMA
        (+) reset and enable the DMA
@endverbatim
  * @{
  */

/**
  * @brief  Selects the specified DMA configuration
  * @param  ADC_DMABeg: specifies the ADC buffer start address
  * @param  ADC_DMASize: specifies the ADC buffer size
  * @retval None
  */
  
void ADC_DMA_Config(uint32_t ADC_DMABeg, uint32_t ADC_DMASize)
{
  /* Check the parameters */
  assert_param(IS_ADC_BEGIN(ADC_DMABeg));
  assert_param(IS_ADC_SIZE(ADC_DMASize));

  ADC->DMAMSAR = (uint32_t)ADC_DMABeg;
  ADC->DMANDTR = (uint32_t)ADC_DMASize;
}

 /**
  * @brief  Enables specified ADC DMA Channel
  * @param  None
  * @retval None
  */
void ADC_DMA_ChannelLoadEnable(void)
{
  /* Start the ADC DMA */
  ADC->DMACR |= (uint32_t)ADC_DMACR_LOAD;
}

 /**
  * @brief  Reset specified ADC DMA
  * @param  None
  * @retval None
  */
void ADC_DMA_ChannelReset(void)
{
  /* Reset the ADC DMA */
  ADC->DMACR = (uint32_t)ADC_DMACR_RST;
}

 /**
  * @brief  Gets the the location that will be written next by the DMA
  * @param  None
  * @retval the current DMA address
  */
uint32_t ADC_DMA_GetNextAddress(void)
{
  /*return the next address buffer*/
  return (uint32_t) ADC->DMAMNAR;
}

 /**
  * @brief  Gets the number of 16-bit conversion results that have been
  *         written to the buffer
  * @param  None
  * @retval The number of conversions
  */
uint32_t ADC_DMA_GetCounter(void)
{
  /*return the number of conversions*/
  return (uint32_t) ADC->DMACNDTR;
}

/** @defgroup ADC_Group4 Interrupts and flags management functions
 *  @brief   Interrupts and flags management functions
 *
@verbatim
 ===============================================================================
            ##### Interrupts and flags management functions ##### 
 ===============================================================================

  [..] This section provides functions allowing to :
       (+) configure the ADC Interrupts and to get the status and clear flags 
           and Interrupts pending bits.
       (+) get the status of DMA and clear flags
  
  *** Flags and Interrupts for ADC ***
  ==============================================
  [..]   
   (+)Flags :  
    (##) ADC_IT_DMABHF: DMA buffer half full interrupt pending
    (##) ADC_IT_DMABF: DMA buffer full interrupt pending
    (##) ADC_IT_SAT: Gain correction saturation interrupt pending
    (##) ADC_IT_DMAOVF: DMA buffer overflow interrupt pending

   (+)Interrupts :  
    (##) ADC_IT_DMABHF: DMA buffer half full interrupt enable
    (##) ADC_IT_DMABF: DMA buffer full interrupt enable
    (##) ADC_IT_SAT: Gain correction saturation interrupt enable
    (##) ADC_IT_DMAOVF: DMA buffer overflow interrupt enable

  *** Flags for ADC_DMA ***
  ==============================================
  [..]
  (+)Flags :
    (##) ADC_FLAG_ACT: DMA active
    (##) ADC_FLAG_OVF: DMA over flow
    
@endverbatim

  * @{
  */

/**
  * @brief  Enables or disables the specified ADC interrupts
  * @param  ADC_IT: specifies the ADC interrupt sources to be enabled or disabled
  *         This parameter can be one of the following values:
  *            @arg ADC_IT_DMABHF: DMA buffer half full interrupt enable
  *            @arg ADC_IT_DMABF: DMA buffer full interrupt enable
  *            @arg ADC_IT_SAT: Gain correction saturation interrupt enable
  *            @arg ADC_IT_DMAOVF: DMA buffer overflow interrupt enable
  * @param  NewState: new state of the specified ADC interrupts
  *          This parameter can be: ENABLE or DISABLE
  * @retval None
  */
void ADC_ITConfig(uint32_t ADC_IT, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  assert_param(IS_ADC_IT(ADC_IT)); 

  if (NewState != DISABLE)
  {
    /* Enable the selected ADC interrupts */
    ADC->IER |= (uint32_t)ADC_IT;
  }
  else
  {
    /* Disable the selected ADC interrupts */
    ADC->IER &= ~(uint32_t)ADC_IT;
  }
}

/**
  * @brief  Checks whether the specified ADC interrupt has occurred or not
  * @param  ADC_DMA_FLAG: specifies the flag to check
  *         This parameter can be one of the following values:
  *            @arg ADC_FLAG_ACT: DMA active
  *            @arg ADC_FLAG_OVF: DMA over flow
  * @retval The new state of ADC_DMA_FLAG (SET or RESET).
  */
FlagStatus ADC_DMA_GetFlagStatus(uint32_t ADC_DMA_FLAG)
{
  FlagStatus bitstatus = RESET;

  /* Check the parameters */
  assert_param(IS_ADC_DMA_FLAG(ADC_DMA_FLAG));

  if ((ADC->ISR & ADC_DMA_FLAG) != RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  return  bitstatus;
}

/**
  * @brief  Checks whether the specified ADC pending interrupt has occurred or not
  * @param  ADC_IT: specifies the flag to check
  *         This parameter can be one of the following values:
  *            @arg ADC_IT_DMABHF: DMA buffer half full interrupt pending
  *            @arg ADC_IT_DMABF: DMA buffer full interrupt pending
  *            @arg ADC_IT_SAT: Gain correction saturation interrupt pending
  *            @arg ADC_IT_DMAOVF: DMA buffer overflow interrupt pending
  * @retval The new state of ADC_IT (SET or RESET)
  */
  
ITStatus ADC_GetITStatus(uint32_t ADC_IT)
{
  ITStatus bitstatus = RESET;
  uint32_t enablestatus = 0;
  
  /* Check the parameters */
  assert_param(IS_ADC_IT(ADC_IT));
  
  enablestatus = (uint32_t)(ADC->IER & ADC_IT);
  if (((ADC->ISR & ADC_IT) != (uint32_t)RESET) && enablestatus)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  return  bitstatus;
}

/**
  * @brief  Clears the ADC interrupt pending bits
  * @param  ADC_IT: specifies the flag to clear
  *         This parameter can be one of the following values:
  *            @arg ADC_IT_DMABHF: DMA buffer half full interrupt pending
  *            @arg ADC_IT_DMABF: DMA buffer full interrupt pending
  *            @arg ADC_IT_SAT: Gain correction saturation interrupt pending
  *            @arg ADC_IT_DMAOVF: DMA buffer overflow interrupt pending
  * @retval None
  */
void ADC_ClearITPendingBit(uint32_t ADC_IT)
{
  /* Check the parameters */
  assert_param(IS_ADC_IT(ADC_IT));

  /* Clear the selected ADC flags */
  ADC->ISR = (uint32_t)ADC_IT;
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
