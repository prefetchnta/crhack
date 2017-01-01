/**
  ******************************************************************************
  * @file    stm32w108xx_sc.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    30-November-2012
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the Serial Controller (SC):
  *           + Universal Asynchronous Receiver/Transmitter communication
  *           + Serial Peripheral Interface communication
  *           + Inter-Integrated Circuit communication
  *           + DMA transfers management in UART and SPI modes
  *
  *  @verbatim
  *
 ===============================================================================
                       ##### How to use this driver #####
 ===============================================================================
  (#) Peripherals GPIO Configuration: 
       (++) Select the desired pin GPIO_InitStruct->GPIO_Pin according to the
            defined Initialization and Configuration Tables for each serial control
            modes (UART, SPI master, SPI Salve and I2C).
       (++) Refer to the Initialization and Configuration Tables to configure 
            the GPIO_InitStruct->GPIO_Mode
       (++) Call GPIO_Init() function.
   (#) For the I2C mode Program the clock rate using the I2C_Init() function.
   (#) For the SPI mode Program the Polarity, Phase, First Data, Clcok rate
       and the Peripheral Mode rate using the SPI_Init() function.
   (#) For the UART mode Program the Baud Rate, Word Length , Stop Bit, Parity and Hardware 
       flow control using the UART_Init() function.
   (#) Enable the PPP using the PPP_Cmd() function. 
   (#) For UART Mode set pull-up resistors on Tx and Rx pins using GPIO_SetBits() function.
   (#) Enable the NVIC and the corresponding interrupt using the function. 
       PPP_ITConfig() if you need to use interrupt mode. 
   (#) When using the DMA mode 
       (++) Configure the DMA using SC_DMA_Init() function.
       (++) Active the needed channel Request using SC_DMA_ChannelLoadEnable() function.

   [..]
   (@) PPP can be UART, SPI or I2C.
   (@) The DMA is not support for I2C mode.

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
#include "stm32w108xx_sc.h"

/** @addtogroup STM32W108xx_StdPeriph_Driver
  * @{
  */

/** @defgroup SERIAL_CONTROLLER
  * @brief SC driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* I2C ADD0 mask */
#define OAR1_ADD0_Set           ((uint8_t)0x01)
#define OAR1_ADD0_Reset         ((uint8_t)0xFE)
#define SPICR_CLEAR_MASK        ((uint32_t)0xFFFFFFE8)

/* Private macro -------------------------------------------------------------*/
#define    ABS(x)         ((x) > 0) ? (x) : (-(x))
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup SC_Private_Functions
  * @{
  */

/** @defgroup SC_Group1 Universal Asynchronous Receiver/Transmitter communication
 *  @brief   universal asynchronous receiver transmitter functions 
 *
@verbatim   
 ===============================================================================
           ##### Universal Asynchronous Receiver-Transmitter functions #####
 ===============================================================================
    [..] This section provides a set of functions allowing to handles the Universal
         Asynchronous Receiver Transmitter communication.
    
    (@) Only SC1 include an universal asynchronous receiver transmitter (UART) controller.

*** Initialization and Configuration ***
 =======================================
    [..] The GPIO pins that can be assigned to UART interface are listed in the following table:
   +------------------------------------------------------+     
   |Parameter | Direction | GPIO configuration |  SC1 pin | 
   |----------|-----------|--------------------|----------|             
   |   TXD    |    Out    |  Alternate Output  |   PB1    |
   |          |           |    (push-pull)     |          |
   |----------|-----------|-------------------------------|  
   |   RXD    |    In     |        Input       |   PB2    |
   |----------|-----------|--------------------|----------|             
   |   nCTS   |    In     |        Input       |   PB3    |
   |----------|-----------|-------------------------------|  
   |   nRTS   |    Out    |  Alternate Output  |   PB4    |
   |          |           |    (push-pull)     |          |
   +------------------------------------------------------+

    [..] For the asynchronous mode these parameters can be configured: 
         (+) Baud Rate.
         (+) Word Length.
         (+) Stop Bit.
         (+) Parity: If the parity is enabled, then the MSB bit of the data written
              in the data register is transmitted but is changed by the parity bit.
              Depending on the frame length defined by the M bit (7-bits or 8-bits),
              the possible UART frame formats are as listed in the following table:

   +-------------------------------------------------------------+     
   |   M bit |  PCE bit  |            UART frame                 |
   |---------------------|---------------------------------------|             
   |    0    |    0      |    | SB | 7 bit data | STB |          |
   |---------|-----------|---------------------------------------|  
   |    0    |    1      |    | SB | 7 bit data | PB | STB |     |
   |---------|-----------|---------------------------------------|  
   |    1    |    0      |    | SB | 8 bit data | STB |          |
   |---------|-----------|---------------------------------------|  
   |    1    |    1      |    | SB | 8 bit data | PB | STB |     |
   +-------------------------------------------------------------+            

         (+) Hardware flow control.
    [..] The UART_Init() function follows the UART asynchronous configuration 
         procedure (details for the procedure is available in datasheet.

*** Data transfers ***
 =====================
    [..] In reception, data are received and then stored into an internal Rx buffer while 
         In transmission, data are first stored into an internal Tx buffer before being 
         transmitted.

    [..] The read access of the SCx_DR register can be done using 
         UART_ReceiveData() function and returns the Rx buffered value. Whereas a write 
         access to the SCx_DR can be done using UART_SendData() function and stores 
         the written data into Tx buffer.

 *** Interrupts and flags management ***
 =======================================
    [..] This subsection provides also a set of functions allowing to configure the 
         UART Interrupts sources, Requests and check or clear the flags or pending bits status. 
         The user should identify which mode will be used in his application to 
         manage the communication: Polling mode, Interrupt mode or DMA mode(refer SC_Group4).

    [..] In Polling Mode, the UART communication can be managed by these flags:
         (#) UART_FLAG_TXE: to indicate the status of the transmit buffer register.
         (#) UART_FLAG_RXNE: to indicate the status of the receive buffer register.
         (#) UART_FLAG_IDLE: to indicate the status of the Idle Line.
         (#) UART_FLAG_CTS: to indicate the status of the nCTS line.
         (#) UART_FLAG_FE: to indicate if a frame error occur.
         (#) UART_FLAG_PE: to indicate if a parity error occur.
         (#) UART_FLAG_OVR: to indicate if an Overrun error occur.
    [..] In this mode it is advised to use the following functions:
         (+) FlagStatus UART_GetFlagStatus(SC_UART_TypeDef* SCx_UART, uint32_t UART_FLAG).
    [..] In this mode all the UART flags are cleared by hardware.

    [..] In Interrupt Mode, the UART communication can be managed by 7 interrupt 
         sources and 7 pending bits:
         (+) Pending Bits:
             (##) UART_IT_PE: to indicate the status of Parity Error interrupt.  
             (##) UART_IT_FE: to indicate the status of Framing Error interrupt.
             (##) UART_IT_UND: to indicate the status of UnderRun Error interrupt.
             (##) UART_IT_OVR: to indicate the status of OverRun Error interrupt.
             (##) UART_IT_IDLE: to indicate the status of IDLE line detected interrupt.
             (##) UART_IT_TXE: to indicate the status of the Transmit data register empty interrupt.
             (##) UART_IT_RXNE: to indicate the status of the Data Register not empty interrupt.

         (+) Interrupt Source:
             (##) UART_IT_PE: specifies the interrupt source forParity Error pending interrupt.  
             (##) UART_IT_FE: specifies the interrupt source for Framing Error pending interrupt.
             (##) UART_IT_UND: specifies the interrupt source for UnderRun Error pending interrupt.
             (##) UART_IT_OVR: specifies the interrupt source for OverRun Error pending interrupt.
             (##) UART_IT_IDLE: specifies the interrupt source for IDLE line detected pending interrupt.
             (##) UART_IT_TXE: specifies the interrupt source for the Transmit data register empty pending interrupt.
             (##) UART_IT_RXNE: specifies the interrupt source for the Data Register not empty pending interrupt.
             -@@- These parameters are coded in order to use them as interrupt source 
                  or as pending bits.
    [..] In this Mode it is advised to use the following functions:
         (+) void UART_ITConfig(SC_IT_TypeDef* SCx_IT, uint32_t UART_IT, FunctionalState NewState).
         (+) ITStatus UART_GetITStatus(SC_IT_TypeDef* SCx_IT, uint32_t UART_IT).
         (+) void UART_ClearITPendingBit(SC_IT_TypeDef* SCx_IT, uint32_t UART_IT).

@endverbatim
  * @{
  */

/**
  * @brief  Deinitializes the SCx_UART peripheral registers to their default reset values.
  * @param  SCx_UART: where x can be 1 to select the Serial controller peripheral.
  * @retval None
  */
void UART_DeInit(SC_UART_TypeDef* SCx_UART)
{
  /* Check the parameters */
  assert_param(IS_UART_PERIPH(SCx_UART));
  
  SCx_UART->CR = 0;
  SCx_UART->UARTCR = 0;
  SCx_UART->UARTBRR1 = 0;
  SCx_UART->UARTBRR2 = 0;
}

/**
  * @brief  Initializes the SC1_UART peripheral according to the specified
  *   parameters in the UART_InitStruct .
  * @param  SCx_UART: where x can be 1 to select the Serial controller peripheral.
  * @param  UART_InitStruct: pointer to a UART_InitTypeDef structure
  *   that contains the configuration information for the specified SC1_UART peripheral.
  * @retval None
  */
void UART_Init(SC_UART_TypeDef* SCx_UART, UART_InitTypeDef* UART_InitStruct)
{
  int32_t temp1, temp2 = 0;
  uint32_t clockfrequency = 0;
  uint32_t N = 0;

  /* Check the parameters */
  assert_param(IS_UART_PERIPH(SCx_UART));
  assert_param(IS_UART_BAUDRATE(UART_InitStruct->UART_BaudRate));
  assert_param(IS_UART_WORD_LENGTH(UART_InitStruct->UART_WordLength));   
  assert_param(IS_UART_STOPBITS(UART_InitStruct->UART_StopBits));
  assert_param(IS_UART_PARITY(UART_InitStruct->UART_Parity));
  assert_param(IS_UART_HARDWARE_FLOW_CONTROL(UART_InitStruct->UART_HardwareFlowControl));
  
  /* Disable the selected UART by clearing the MODE bits in the CR register */
  SCx_UART->CR &= (uint16_t)~((uint16_t)SC_CR_MODE);
  
  /*---------------------------- UARTCR Configuration ------------------------*/
  /* Configure the USART Word Length, Parity and mode ------------------------*/
  /* Set STOP bit according to USART_StopBits value */
  /* Set the M bits according to UART_WordLength value */
  /* Set PCE and PS bits according to USART_Parity value */
  /* Set HFCE and AHFCE bits according to UART_HardwareFlowControl value */
  SCx_UART->UARTCR = (uint32_t)(UART_InitStruct->UART_WordLength | UART_InitStruct->UART_Parity |
                                UART_InitStruct->UART_StopBits | UART_InitStruct->UART_HardwareFlowControl);
  /*---------------------------- UART BRR Configuration ----------------------*/
  /* Configure the UART Baud Rate --------------------------------------------*/
  clockfrequency = CLK_GetClocksFreq();
  N = (uint16_t)(clockfrequency/(2*(UART_InitStruct->UART_BaudRate)));
  
  /* Write to UART BRR1 */
  SCx_UART->UARTBRR1 &= (uint32_t)~SC_UARTBRR1_N;
  SCx_UART->UARTBRR1 |= (uint32_t)N;

  temp1 = (int32_t)(clockfrequency/(2*N));
  temp2 = (int32_t)(clockfrequency/(2*N +1));

  temp1 = ABS((int32_t)((int32_t)temp1 - (int32_t)UART_InitStruct->UART_BaudRate));
  temp2 = ABS((int32_t)((int32_t)temp2 - (int32_t)UART_InitStruct->UART_BaudRate));
  
  /* Check the baud rate error and write to UART BRR2 */
  if(temp1 > temp2)
  {
    SCx_UART->UARTBRR2 |= (uint32_t)SC_UARTBRR1_F;
  }
  else
  {
    SCx_UART->UARTBRR2 &=(uint32_t)~SC_UARTBRR1_F;
  }
}

/**
  * @brief  Fills each UART_InitStruct member with its default value.
  * @param  UART_InitStruct: pointer to a UART_InitTypeDef structure
  *   which will be initialized.
  * @retval None
  */
void UART_StructInit(UART_InitTypeDef* UART_InitStruct)
{
  /* UART_InitStruct members default value */
  UART_InitStruct->UART_BaudRate = 115200;
  UART_InitStruct->UART_WordLength = UART_WordLength_8b;
  UART_InitStruct->UART_StopBits = UART_StopBits_1;
  UART_InitStruct->UART_Parity = UART_Parity_No;
  UART_InitStruct->UART_HardwareFlowControl = UART_HardwareFlowControl_Disable;  
}

/**
  * @brief  Enables or disables the RTS assertion for the specified SC1_UART peripheral.
  * @param  SCx_UART: where x can be 1 to select the Serial controller peripheral.
  * @param  NewState: new state of the SC1_UART peripheral.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART_RTSAssertionCmd(SC_UART_TypeDef* SCx_UART, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_UART_PERIPH(SCx_UART));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* The RTS signal is asserted by setting the nRTS bit in the UARTCR register */
    SCx_UART->UARTCR |= SC_UARTCR_nRTS;
  }
  else
  {
    /* The RTS signal is deasserted by clearing the nRTS bit in the UARTCR register */
    SCx_UART->UARTCR &= (uint16_t)~((uint16_t)SC_UARTCR_nRTS);
  }
}

/**
  * @brief  Enables or disables the specified SC1_UART peripheral.
  * @param  SCx_UART: where x can be 1 to select the Serial controller peripheral.
  * @param  NewState: new state of the SC1_UART peripheral.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART_Cmd(SC_UART_TypeDef* SCx_UART, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_UART_PERIPH(SCx_UART));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable the selected UART by setting the MODE bit 0 in the CR register */
    SCx_UART->CR &= (uint16_t)~((uint16_t)SC_CR_MODE);
    SCx_UART->CR |= SC_CR_MODE_0;
  }
  else
  {
    /* Disable the selected UART by clearing the MODE bits in the CR register */
    SCx_UART->CR &= (uint16_t)~((uint16_t)SC_CR_MODE);
  }
}

/**
  * @brief  Enables or disables the specified SCx_UART interrupts.
  * @param  SCx_IT: where x can be 1 or 2 to select the Serial controller peripheral.
  * @param  UART_IT: specifies the SCx_UART interrupt source to be enabled or disabled. 
  *   This parameter can be one of the following values:
  *     @arg UART_IT_PE: Parity error interrupt mask
  *     @arg UART_IT_FE: Frame error interrupt mask
  *     @arg UART_IT_UND: Underrun interrupt mask (to be checked)
  *     @arg UART_IT_OVR: Overrun interrupt mask
  *     @arg UART_IT_IDLE: Idle line detected interrupt mask
  *     @arg UART_IT_TXE: Transmit data register empty interrupt mask
  *     @arg UART_IT_RXNE: Data Register not empty interrupt mask
  * @param  NewState: new state of the specified SCx_UART interrupt source.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART_ITConfig(SC_IT_TypeDef* SCx_IT, uint32_t UART_IT, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_UART_IT_PERIPH(SCx_IT));
  assert_param(IS_UART_IT(UART_IT));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable the selected UART interrupt */
    SCx_IT->IER |= UART_IT;
  }
  else
  {
    /* Disable the selected UART interrupt */
    SCx_IT->IER &= (uint16_t)~((uint16_t)UART_IT);
  }
}

/**
  * @brief  Trigger event configuration to handle the specified SCx_UART interrupt.
  * @param  SCx_IT: where x can be 1 or 2 to select the Serial controller peripheral.
  * @param  UART_IT: specifies the SCx_UART interrupt to be configured. 
  *   This parameter can be one of the following values:
  *     @arg UART_IT_IDLE: Idle line detected interrupt
  *     @arg UART_IT_TXE: Transmit data register empty interrupt
  *     @arg UART_IT_RXNE: Data Register not empty interrupt
  * @param  TriggerEvent: Trigger event configuration of the specified SCx_UART interrupt.
  *   This parameter can be one of the following values:
  *     @arg SC_TriggerEvent_Edge: The specified SCx_UART interrupt will be generated on edge
  *     @arg SC_TriggerEvent_Level: The specified SCx_UART interrupt will be generated on level
  * @retval None
  */
void UART_TriggerEventConfig(SC_IT_TypeDef* SCx_IT, uint32_t UART_IT, uint32_t TriggerEvent)
{
  /* Check the parameters */
  assert_param(IS_UART_IT_PERIPH(SCx_IT));
  assert_param(IS_UART_TRIGGEREVENT_IT(UART_IT));
  assert_param(IS_SC_TRIGGEREVENT(TriggerEvent));
  
  if (TriggerEvent != SC_TriggerEvent_Edge)
  {
    /* The UART_IT interrupt will be handled on level */
    SCx_IT->ICR |= UART_IT;
  }
  else
  {
    /* The UART_IT interrupt will be handled on edge */
    SCx_IT->ICR &= (uint16_t)~((uint16_t)UART_IT);
  }
}

/**
  * @brief  Transmits a Data through the SC1_UART peripheral.
  * @param  SCx_UART: where x can be 1 to select the Serial controller peripheral.
  * @param  Data: Data to be transmitted.
  * @retval None
  */
void UART_SendData(SC_UART_TypeDef* SCx_UART, uint8_t Data)
{
  uint32_t scxbase = 0x00;

  /* Check the parameters */
  assert_param(IS_UART_PERIPH(SCx_UART));

  scxbase = (uint32_t)SCx_UART; 
  
  *(__IO uint32_t *) scxbase = Data;
}

/**
  * @brief  Returns the most recent received data by the SC1_UART peripheral. 
  * @param  SCx_UART: where x can be 1 to select the Serial controller peripheral.
  * @retval The value of the received data.
  */
uint8_t UART_ReceiveData(SC_UART_TypeDef* SCx_UART)
{
  uint32_t scxbase = 0x00;
  
  /* Check the parameters */
  assert_param(IS_UART_PERIPH(SCx_UART));
  
  scxbase = (uint32_t)SCx_UART; 
  
  return *(__IO uint32_t *) scxbase;
}

/**
  * @brief  Checks whether the specified SC1_UART flag is set or not.
  * @param  SCx_UART: where x can be 1 to select the Serial controller peripheral.
  * @param  UART_FLAG: specifies the SCx_UART flag to check.
  *   This parameter can be one of the following values:
  *     @arg UART_FLAG_CTS: Clear to send flag.
  *     @arg UART_FLAG_RXNE: Receive data register not empty flag.
  *     @arg UART_FLAG_TXE: Transmit data register empty flag.
  *     @arg UART_FLAG_ORE: OverRun Error flag.
  *     @arg UART_FLAG_FE: Framing Error flag.
  *     @arg UART_FLAG_PE: Parity Error flag.
  *     @arg UART_FLAG_IDLE: Idle Line detection flag.
  * @retval The new state of UART_FLAG (SET or RESET).
  */
FlagStatus UART_GetFlagStatus(SC_UART_TypeDef* SCx_UART, uint32_t UART_FLAG)
{
  FlagStatus bitstatus = RESET;
  
  /* Check the parameters */
  assert_param(IS_UART_PERIPH(SCx_UART));
  assert_param(IS_UART_GET_FLAG(UART_FLAG));
  
  if ((SCx_UART->UARTSR & UART_FLAG) != (uint32_t)RESET)
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
  * @brief  Checks whether the specified SC1_UART pending interrupt is set or not.
  * @param  SCx_IT: where x can be 1 to select the Serial controller peripheral.
  * @param  UART_IT: specifies the pending interrupt to check.
  *   This parameter can be one of the following values:
  *     @arg UART_IT_PE: Parity error interrupt pending
  *     @arg UART_IT_FE: Frame error interrupt pending
  *     @arg UART_IT_UND: Underrun interrupt pending (to be checked)
  *     @arg UART_IT_OVR: Overrun interrupt pending
  *     @arg UART_IT_IDLE: Idle line detected interrupt pending
  *     @arg UART_IT_TXE: Transmit data register empty interrupt pending
  *     @arg UART_IT_RXNE: Data Register not empty interrupt pending
  * @retval The new state of UART_IT (SET or RESET).
  */
ITStatus UART_GetITStatus(SC_IT_TypeDef* SCx_IT, uint32_t UART_IT)
{
  ITStatus bitstatus = RESET;
  uint32_t enablestatus = 0;
  /* Check the parameters */
  assert_param(IS_UART_IT_PERIPH(SCx_IT));
  assert_param(IS_UART_IT(UART_IT));

  enablestatus = (uint32_t)(SCx_IT->IER & UART_IT);
  if (((SCx_IT->ISR & UART_IT) != (uint32_t)RESET) && enablestatus)
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
  * @brief  Clears the SC1_UART interrupt pending bits.
  * @param  SCx_IT: where x can be 1 to select the Serial controller peripheral.
  * @param  UART_IT: specifies the pending interrupt to check.
  *   This parameter can be one of the following values:
  *     @arg UART_IT_PE: Parity error interrupt pending
  *     @arg UART_IT_FE: Frame error interrupt pending
  *     @arg UART_IT_UND: Underrun interrupt pending (to be checked)
  *     @arg UART_IT_OVR: Overrun interrupt pending
  *     @arg UART_IT_IDLE: Idle line detected interrupt pending
  *     @arg UART_IT_TXE: Transmit data register empty interrupt pending
  *     @arg UART_IT_RXNE: Data Register not empty interrupt pending
  * @retval None
  */
void UART_ClearITPendingBit(SC_IT_TypeDef* SCx_IT, uint32_t UART_IT)
{
  /* Check the parameters */
  assert_param(IS_UART_IT_PERIPH(SCx_IT));
  assert_param(IS_UART_IT(UART_IT));
  
  SCx_IT->ISR = UART_IT;
}

/**
  * @}
  */

/** @defgroup SC_Group2 Serial peripheral interface communication
 *  @brief   serial peripheral interface functions 
 *
@verbatim   
 ===============================================================================
           ##### Serial Peripheral Interface functions #####
 ===============================================================================
    [..] This section provides a set of functions allowing to handles the serial peripheral
         interface communication.

    (@) The SC1 and SC2 include an Serial Peripheral Interface (SPI) master/slave controller.

*** Initialization and Configuration ***
 =======================================
    [..] The GPIO pins that can be assigned to SPI interface are listed in the following tables:
     (##) SPI Master Mode:
   +-----------------------------------------------------------------+     
   |Parameter | Direction | GPIO configuration |  SC1 pin | SC2 pin  | 
   |----------|-----------|--------------------|----------|----------|             
   |   MOSI   |    Out    |  Alternate Output  |   PB1    |   PA0    |
   |          |           |    (push-pull)     |          |          |
   |----------|-----------|-------------------------------|----------|  
   |   MISO   |    In     |        Input       |   PB2    |   PA1    |
   |----------|-----------|--------------------|----------|----------|             
   |   SCLK   |    Out    | Alternate Output   |   PB3    |    PA2   |
   |          |           |      (push-pull)   |          |          |
   |          |           |  Special SCLK mode |          |          |
   +-----------------------------------------------------------------+
    (##) SPI Slave Mode:
   +-----------------------------------------------------------------+     
   |Parameter | Direction | GPIO configuration |  SC1 pin | SC2 pin  | 
   |----------|-----------|--------------------|----------|----------|             
   |   MOSI   |    In     |       Input        |   PB2    |   PA0    |
   |----------|-----------|-------------------------------|----------|  
   |   MISO   |    Out    |  Alternate Output  |   PB1    |   PA1    |
   |          |           |      (push-pull)   |          |          |
   |----------|-----------|--------------------|----------|----------|             
   |   SCLK   |    In     |       Input        |   PB3    |   PA2    |
   |----------|-----------|--------------------|----------|----------|             
   |   nSSEL  |    In     |       Input        |   PB4    |   PA3    |
   +-----------------------------------------------------------------+
    [..]  For the Serial Peripheral Interface mode these parameters can be configured: 
          (+) Mode.
          (+) Data Size.
          (+) Polarity.
          (+) Phase.
          (+) Baud Rate
          (+) First Bit Transmission
    [..] The SPI_Init() function follows the SPI configuration procedures for Master mode
         and Slave mode (details for these procedures are available in datasheet).

*** Data transfers ***
 =====================

    [..] In reception, data are received and then stored into an internal Rx buffer while 
         In transmission, data are first stored into an internal Tx buffer before being 
         transmitted.

    [..] The read access of the SCx_DR register can be done using SPI_ReceiveData() 
         function and returns the Rx buffered value. Whereas a write access to the SCx_DR 
         can be done using SPI_SendData() function and stores the written data into 
         Tx buffer. 

 *** Interrupts and flags management ***
 =======================================
    [..] This subsection provides also a set of functions allowing to configure the 
         SPI Interrupts sources, Requests and check or clear the flags or pending bits status. 
         The user should identify which mode will be used in his application to 
         manage the communication: Polling mode, Interrupt mode or DMA mode(refer SC_Group4).

    [..] In Polling Mode, the SPI communication can be managed by these flags:
         (#) SPI_FLAG_TXE: to indicate the status of the transmit buffer register.
         (#) SPI_FLAG_RXNE: to indicate the status of the receive buffer register.
         (#) SPI_FLAG_IDLE: to indicate the status of the Idle Line.
         (#) SPI_FLAG_OVR: to indicate if an Overrun error occur.
    [..] In this Mode it is advised to use the following functions:
         (+) FlagStatus SPI_GetFlagStatus(SC_SPI_TypeDef* SCx_SPI, uint32_t SPI_FLAG).
    [..] In this mode all the SPI flags are cleared by hardware.

    [..] In Interrupt Mode, the SPI communication can be managed by 5 interrupt 
         sources and 5 pending bits:
         (+) Pending Bits:
             (##) SPI_IT_UND: to indicate the status of UnderRun Error interrupt.
             (##) SPI_IT_OVR: to indicate the status of OverRun Error interrupt.
             (##) SPI_IT_IDLE: to indicate the status of IDLE line detected interrupt.
             (##) SPI_IT_TXE: to indicate the status of the Transmit data register empty interrupt.
             (##) SPI_IT_RXNE: to indicate the status of the Data Register not empty interrupt.

         (+) Interrupt Source:
             (##) SPI_IT_UND: specifies the interrupt source for UnderRun Error pending interrupt.
             (##) SPI_IT_OVR: specifies the interrupt source for OverRun Error pending interrupt.
             (##) SPI_IT_IDLE: specifies the interrupt source for IDLE line detected pending interrupt.
             (##) SPI_IT_TXE: specifies the interrupt source for the Transmit data register empty pending interrupt.
             (##) SPI_IT_RXNE: specifies the interrupt source for the Data Register not empty pending interrupt.
             -@@- These parameters are coded in order to use them as interrupt 
                 source or as pending bits.
    [..] In this mode it is advised to use the following functions:
         (+) void SPI_ITConfig(SC_IT_TypeDef* SCx_IT, uint32_t SPI_IT, FunctionalState NewState).
         (+) ITStatus SPI_GetITStatus(SC_IT_TypeDef* SCx_IT, uint32_t SPI_IT).
         (+) void SPI_ClearITPendingBit(SC_IT_TypeDef* SCx_IT, uint32_t SPI_IT).

@endverbatim
  * @{
  */

/**
  * @brief  Deinitializes the SCx_SPI peripheral registers to their default reset values.
  * @param  SCx_SPI: where x can be 1 or 2 to select the Serial controller peripheral.
  * @retval None
  */
void SPI_DeInit(SC_SPI_TypeDef* SCx_SPI)
{
  /* Check the parameters */
  assert_param(IS_SPI_PERIPH(SCx_SPI));
  
  SCx_SPI->CR = 0;
  SCx_SPI->SPICR = 0;
  SCx_SPI->CRR1 = 0;
  SCx_SPI->CRR2 = 0;
}

/**
  * @brief  Initializes the SCx_SPI peripheral according to the specified 
  *         parameters in the SPI_InitStruct.
  * @param  SCx_SPI: where x can be 1 or 2 to select the Serial controller peripheral.
  * @param  SPI_InitStruct: pointer to a SPI_InitTypeDef structure that
  *         contains the configuration information for the specified SPI peripheral.
  * @retval None
  */
void SPI_Init(SC_SPI_TypeDef* SCx_SPI, SPI_InitTypeDef* SPI_InitStruct)
{
  uint32_t tmpreg = 0;
  uint8_t expvalue = 0, linvalue = 0, tmpexp = 0, tmplin = 0, idx = 0;
  uint32_t tempclockrate = 0, clockrate = 0, power = 0;
  
  /* Check the SPI parameters */
  assert_param(IS_SPI_PERIPH(SCx_SPI));
  assert_param(IS_SPI_MODE(SPI_InitStruct->SPI_Mode));
  assert_param(IS_SPI_CPOL(SPI_InitStruct->SPI_CPOL));
  assert_param(IS_SPI_CPHA(SPI_InitStruct->SPI_CPHA));
  assert_param(IS_SPI_CLOCK_RATE(SPI_InitStruct->SPI_ClockRate));
  assert_param(IS_SPI_FIRST_BIT(SPI_InitStruct->SPI_FirstBit));
  
  /* Get the SCx SPICR register value */
  tmpreg = SCx_SPI->SPICR;
  
  /* Clear Mode, LSBFirst, CPOL and CPHA bits */
  tmpreg &= SPICR_CLEAR_MASK;
  /*---------------------------- SCx_SPICR Configuration ---------------------*/
  /* Set MSTR bits according to SPI_Mode values */
  /* Set LSBFirst bit according to SPI_FirstBit value */
  /* Set CPOL bit according to SPI_CPOL value */
  /* Set CPHA bit according to SPI_CPHA value */
  tmpreg |= (uint32_t)((uint32_t)SPI_InitStruct->SPI_Mode | (uint32_t)SPI_InitStruct->SPI_CPOL |
                       (uint32_t)SPI_InitStruct->SPI_CPHA | (uint32_t)SPI_InitStruct->SPI_FirstBit);
  /* Write to SCx SPICR register */
  SCx_SPI->SPICR = tmpreg;
  
  /*---------------------------- SCx_CRRx Configuration ----------------------*/
  for(linvalue = 0x00; linvalue < 16; linvalue++)
  {
    for(expvalue = 0x00; expvalue < 16; expvalue++)
    {
      power = 0x01;
      for (idx=1; idx <= expvalue; idx++)
      {
        power*=2;
      }
      tempclockrate = power * (uint32_t)(linvalue + 1);
      
      if (tempclockrate == ((uint32_t)((uint32_t)CLK_GetClocksFreq()/(uint32_t)(2*(SPI_InitStruct->SPI_ClockRate)))))
      {
        SCx_SPI->CRR1 = (uint32_t)linvalue;
        SCx_SPI->CRR2 = (uint32_t)expvalue;
      }
      else
      {
        /* Check the clock rate error and write to CRRx */
        if((ABS((int32_t)tempclockrate - (int32_t)(CLK_GetClocksFreq()/(2*(SPI_InitStruct->SPI_ClockRate))))) < (ABS((int32_t)clockrate - (int32_t)(CLK_GetClocksFreq()/(2*(SPI_InitStruct->SPI_ClockRate))))))
        {
          clockrate = tempclockrate;
          tmplin = linvalue;
          tmpexp = expvalue;
        }
      }
    }
  }
  
  SCx_SPI->CRR1 = (uint32_t)tmplin;
  SCx_SPI->CRR2 = (uint32_t)tmpexp;
}

/**
  * @brief  Fills each SPI_InitStruct member with its default value.
  * @param  SPI_InitStruct: pointer to a SPI_InitTypeDef structure which will be initialized.
  * @retval None
  */
void SPI_StructInit(SPI_InitTypeDef* SPI_InitStruct)
{
  /*--------------- Reset SPI init structure parameters values ---------------*/
  /* Initialize the SPI_Mode member */
  SPI_InitStruct->SPI_Mode = SPI_Mode_Slave;
  /* Initialize the SPI_CPOL member */
  SPI_InitStruct->SPI_CPOL = SPI_CPOL_Low;
  /* Initialize the SPI_CPHA member */
  SPI_InitStruct->SPI_CPHA = SPI_CPHA_1Edge;
  /* Initialize the SPI_ClockRate member */
  SPI_InitStruct->SPI_ClockRate = 3000000;
  /* Initialize the SPI_FirstBit member */
  SPI_InitStruct->SPI_FirstBit = SPI_FirstBit_MSB;
}

/**
  * @brief  Configures the Receiver driven mode for the selected SCx_SPI (Master mode only).
  * @param  SCx_SPI: where x can be 1 or 2 to select the Serial controller peripheral.
  * @param  SPI_ReceiverMode: specifies the Receiver driven mode to be configured.
  *   This parameter can be one of the following values:
  *     @arg SPI_ReceiverMode_TxDataReady: Initiate transactions when transmit data is available
  *     @arg SPI_ReceiverMode_RxFIFOFree: Initiate transactions when receive buffer has space
  * @retval None
  */
void SPI_ReceiverModeConfig(SC_SPI_TypeDef* SCx_SPI, uint32_t SPI_ReceiverMode)
{
  /* Check the parameters */
  assert_param(IS_SPI_PERIPH(SCx_SPI));
  assert_param(IS_SPI_RECEIVER_DRIVEN_MODE(SPI_ReceiverMode));
  
  /* Clear RXMODE bit */
  SCx_SPI->SPICR &= (uint16_t)~((uint16_t)SC_SPICR_RXMODE);
  
  /* Set new RXMODE bit value */
  SCx_SPI->SPICR |= SPI_ReceiverMode;
}

/**
  * @brief  Enables or disables the last byte repeat transmission feature  
  *         for the specified SCx_SPI peripheral (Slave mode only).
  * @param  SCx_SPI: where x can be 1 or 2 to select the Serial controller peripheral.
  * @param  NewState: new state of the SCx_SPI peripheral.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SPI_LastByteRepeatCmd(SC_SPI_TypeDef* SCx_SPI, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_SPI_PERIPH(SCx_SPI));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable the last byte repeat feature for the selected SPI by setting 
       the RPTEN bit 0 in the SPICR register */
    SCx_SPI->SPICR |= SC_SPICR_RPTEN;
  }
  else
  {
    /* Disable the last byte repeat feature for the selected SPI by clearing
       the RPTEN bit in the SPICR register */
    SCx_SPI->SPICR &= (uint16_t)~((uint16_t)SC_SPICR_RPTEN);
  }
}

/**
  * @brief  Enables or disables the specified SCx_SPI peripheral.
  * @param  SCx_SPI: where x can be 1 or 2 to select the Serial controller peripheral.
  * @param  NewState: new state of the SCx_SPI peripheral.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SPI_Cmd(SC_SPI_TypeDef* SCx_SPI, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_SPI_PERIPH(SCx_SPI));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable the selected SPI by setting the MODE bit 0 in the CR register */
    SCx_SPI->CR &= (uint16_t)~((uint16_t)SC_CR_MODE);
    SCx_SPI->CR |= SC_CR_MODE_1;
  }
  else
  {
    /* Disable the selected SPI by clearing the MODE bits in the CR register */
    SCx_SPI->CR &= (uint16_t)~((uint16_t)SC_CR_MODE);
  }
}

/**
  * @brief  Enables or disables the specified SCx_SPI interrupts.
  * @param  SCx_IT: where x can be 1 or 2 to select the Serial controller peripheral.
  * @param  SPI_IT: specifies the SCx_SPI interrupt source to be enabled or disabled. 
  *   This parameter can be one of the following values:
  *     @arg SPI_IT_UND: Underrun interrupt mask
  *     @arg SPI_IT_OVR: Overrun interrupt mask
  *     @arg SPI_IT_IDLE: Idle line detected interrupt mask
  *     @arg SPI_IT_TXE: Transmit data register empty interrupt mask
  *     @arg SPI_IT_RXNE: Data Register not empty interrupt mask
  * @param  NewState: new state of the specified SCx_SPI interrupt source.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SPI_ITConfig(SC_IT_TypeDef* SCx_IT, uint32_t SPI_IT, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_I2C_SPI_DMA_IT_PERIPH(SCx_IT));
  assert_param(IS_SPI_IT(SPI_IT));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable the selected SPI interrupt */
    SCx_IT->IER |= SPI_IT;
  }
  else
  {
    /* Disable the selected SPI interrupt */
    SCx_IT->IER &= (uint16_t)~((uint16_t)SPI_IT);
  }
}

/**
  * @brief  Trigger event configuration to handle the specified SCx_SPI interrupt.
  * @param  SCx_IT: where x can be 1 or 2 to select the Serial controller peripheral.
  * @param  SPI_IT: specifies the SCx_SPI interrupt to be configured. 
  *   This parameter can be one of the following values:
  *     @arg SPI_IT_IDLE: Idle line detected interrupt
  *     @arg SPI_IT_TXE: Transmit data register empty interrupt
  *     @arg SPI_IT_RXNE: Data Register not empty interrupt
  * @param  TriggerEvent: Trigger event configuration of the specified SCx_SPI interrupt.
  *   This parameter can be one of the following values:
  *     @arg SC_TriggerEvent_Edge: The specified SCx_SPI interrupt will be generated on edge
  *     @arg SC_TriggerEvent_Level: The specified SCx_SPI interrupt will be generated on level
  * @retval None
  */
void SPI_TriggerEventConfig(SC_IT_TypeDef* SCx_IT, uint32_t SPI_IT, uint32_t TriggerEvent)
{
  /* Check the parameters */
  assert_param(IS_I2C_SPI_DMA_IT_PERIPH(SCx_IT));
  assert_param(IS_SPI_TRIGGEREVENT_IT(SPI_IT));
  assert_param(IS_SC_TRIGGEREVENT(TriggerEvent));
  
  if (TriggerEvent != SC_TriggerEvent_Edge)
  {
    /* The SPI_IT interrupt will be handled on level */
    SCx_IT->ICR |= SPI_IT;
  }
  else
  {
    /* The SPI_IT interrupt will be handled on edge */
    SCx_IT->ICR &= (uint16_t)~((uint16_t)SPI_IT);
  }
}

/**
  * @brief  Transmits a Data through the SCx_SPI peripheral.
  * @param  SCx_SPI: where x can be 1 or 2 to select the Serial controller peripheral.
  * @param  Data: Data to be transmitted.
  * @retval None
  */
void SPI_SendData(SC_SPI_TypeDef* SCx_SPI, uint8_t Data)
{
  uint32_t scxbase = 0x00;

  /* Check the parameters */
  assert_param(IS_SPI_PERIPH(SCx_SPI));

  scxbase = (uint32_t)SCx_SPI; 
  
  *(__IO uint32_t *) scxbase = Data;
}

/**
  * @brief  Returns the most recent received data by the SCx_SPI peripheral. 
  * @param  SCx_SPI: where x can be 1 or 2 to select the Serial controller peripheral.
  * @retval The value of the received data.
  */
uint8_t SPI_ReceiveData(SC_SPI_TypeDef* SCx_SPI)
{
  uint32_t scxbase = 0x00;
  
  /* Check the parameters */
  assert_param(IS_SPI_PERIPH(SCx_SPI));
  
  scxbase = (uint32_t)SCx_SPI; 

  return *(__IO uint32_t *) scxbase;
}

/**
  * @brief  Checks whether the specified SCx_SPI flag is set or not.
  * @param  SCx_SPI: where x can be 1 or 2 to select the Serial controller peripheral.
  * @param  SPI_FLAG: specifies the SCx_SPI flag to check.
  *   This parameter can be one of the following values:
  *     @arg SPI_FLAG_OVR:  OverRun Error flag.
  *     @arg SPI_FLAG_TXE:  Transmit data register empty flag.
  *     @arg SPI_FLAG_RXNE:  Receive data register not empty flag.
  *     @arg SPI_FLAG_IDLE:  IDLE line flag.
  * @retval The new state of SPI_FLAG (SET or RESET).
  */
FlagStatus SPI_GetFlagStatus(SC_SPI_TypeDef* SCx_SPI, uint32_t SPI_FLAG)
{
  FlagStatus bitstatus = RESET;
  
  /* Check the parameters */
  assert_param(IS_SPI_PERIPH(SCx_SPI));
  assert_param(IS_SPI_GET_FLAG(SPI_FLAG));
  
  if ((SCx_SPI->SPISR & SPI_FLAG) != (uint32_t)RESET)
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
  * @brief  Checks whether the specified SCx_SPI pending interrupt is set or not.
  * @param  SCx_IT: where x can be 1 or 2 to select the Serial controller peripheral.
  * @param  SPI_IT: specifies the SCx_SPI pending interrupt to check.
  *   This parameter can be one of the following values:
  *     @arg SPI_IT_UND: Underrun interrupt pending
  *     @arg SPI_IT_OVR: Overrun interrupt pending
  *     @arg SPI_IT_IDLE: Idle line detected interrupt pending
  *     @arg SPI_IT_TXE: Transmit data register empty interrupt pending
  *     @arg SPI_IT_RXNE: Data Register not empty interrupt pending
  * @retval The new state of SPI_IT (SET or RESET).
  */
ITStatus SPI_GetITStatus(SC_IT_TypeDef* SCx_IT, uint32_t SPI_IT)
{
  ITStatus bitstatus = RESET;
  uint32_t enablestatus = 0;
  
  /* Check the parameters */
  assert_param(IS_I2C_SPI_DMA_IT_PERIPH(SCx_IT));
  assert_param(IS_SPI_IT(SPI_IT));
  
  enablestatus = (uint32_t)(SCx_IT->IER & SPI_IT);
  if (((SCx_IT->ISR & SPI_IT) != (uint32_t)RESET) && enablestatus)
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
  * @brief  Clears the SCx_SPI interrupt pending bits.
  * @param  SCx_IT: where x can be 1 to select the Serial controller peripheral.
  * @param  SPI_IT: specifies the SCx_SPI pending interrupt to check.
  *   This parameter can be one of the following values:
  *     @arg SPI_IT_UND: Underrun interrupt pending
  *     @arg SPI_IT_OVR: Overrun interrupt pending
  *     @arg SPI_IT_IDLE: Idle line detected interrupt pending
  *     @arg SPI_IT_TXE: Transmit data register empty interrupt pending
  *     @arg SPI_IT_RXNE: Data Register not empty interrupt pending
  * @retval None
  */
void SPI_ClearITPendingBit(SC_IT_TypeDef* SCx_IT, uint32_t SPI_IT)
{
  /* Check the parameters */
  assert_param(IS_I2C_SPI_DMA_IT_PERIPH(SCx_IT));
  assert_param(IS_SPI_IT(SPI_IT));
  
  SCx_IT->ISR = SPI_IT;
}

/**
  * @}
  */

/** @defgroup SC_Group3 Inter-Integrated Circuit communication
 *  @brief   inter-integrated circuit functions 
 *
@verbatim   
 ===============================================================================
           ##### Inter-Integrated Circuit functions #####
 ===============================================================================
    [..] This section provides a set of functions allowing to handles the Inter-Integrated 
         Circuit communication.

    (@) The SC1 and SC2 include an Inter-integrated circuit interface (I2C) master controller.

*** Initialization and Configuration ***
 =======================================

    [..] The GPIO pins that can be assigned to I2C interface are listed in the following table:
   +------------------------------------------------------------------+     
   |Parameter | Direction | GPIO configuration |  SC1 pin |  SC2 pin  |
   |----------|-----------|--------------------|----------|-----------|             
   |    SDA   |  In/Out   |  Alternate Output  |   PB1    |   PA1     |
   |          |           |    (open drain)    |          |           |
   |----------|-----------|-------------------------------------------|  
   |    SCL   |  In/Out   |  Alternate Output  |   PB2    |   PA2     |
   |          |           |    (open drain)    |          |           |
   +------------------------------------------------------------------+    

    [..] For the Inter-Integrated Circuit mode only the Baud Rate parameter can be configured: 
    
    [..] The I2C_Init() function follows the I2C configuration procedure (this procedure 
         is available in datasheet).

    [..] The generate START and STOP can be done respectively using I2C_GenerateSTART() and 
         I2C_GenerateSTOP() functions.

    [..] The command for the ACK generation can be done I2C_AcknowledgeConfig() function.

*** Data transfers ***
 =====================
    [..] To initiate a transmit segment, write the data to the SCx_DR data register,
         then set the BTE bit in the SCx_I2CCR1 register, and finally wait until 
         the BTE bit is clear and the BTF bit in the SCx_I2CSR register, these steps
         can be done using I2C_SendData() function. Alternatively to initiate the reception
         set the BRE bit in the SCx_I2CCR1 register and keep waiting until the BRE bit 
         is clear and the BTF bit in the SCx_I2CSR register is set then read the 
         Rx buffered value, these steps can be done using I2C_ReceiveData() function.

    [..] The Master transmit address byte to select the slave device in transmitter 
         or in receiver mode can be done using the I2C_Send7bitAddress() function

 *** Interrupts and flags management ***
 =======================================
    [..] This subsection provides also a set of functions allowing to configure the 
         I2C Interrupts sources, Requests and check or clear the flags or pending bits status. 
         The user should identify which mode will be used in his application to 
         manage the communication: Polling mode, Interrupt mode.

    [..] In Polling Mode, the I2C communication can be managed by 4 flags:
        (#) I2C_FLAG_NACK: to indicate the status of the not acknowledge flag.
        (#) I2C_FLAG_BTF: to indicate the status of the byte transfer finished flag.
        (#) I2C_FLAG_BRF: to indicate the status of the byte receive finished flag.
        (#) I2C_FLAG_CMDFIN: to indicate the status of the command finished flag.
      
    [..] In this Mode it is advised to use the following functions:
        (+) FlagStatus I2C_GetFlagStatus(SC_I2C_TypeDef* SCx_I2C, uint32_t I2C_FLAG).
    [..] In this mode all the I2C flags are cleared by hardware.

  *** Interrupt Mode ***
  ======================
    [..] In Interrupt Mode, the I2C communication can be managed by 4 interrupt sources
         and 4 pending bits:

    [..] Interrupt Source:
        (#) I2C_IT_NACK: specifies the interrupt source for the not acknowledge interrupt.
        (#) I2C_IT_CMDFIN: specifies the interrupt source for the command finished interrupt.
        (#) I2C_IT_BTF: specifies the interrupt source for the byte transfer finished interrupt.
        (#) I2C_IT_BRF: specifies the interrupt source for the byte receive finished interrupt.
        
    [..] Pending Bits:
        (#) I2C_IT_NACK: to indicate the status of not acknowledge pending interrupt.
        (#) I2C_IT_CMDFIN: to indicate the status of command finished pending interrupt.
        (#) I2C_IT_BTF: to indicate the status of byte transfer finished pending interrupt.
        (#) I2C_IT_BRF: to indicate the status of byte receive finished pending interrupt.
        
    [..] In this Mode it is advised to use the following functions:
        (+) void I2C_ClearITPendingBit(SC_IT_TypeDef* SCx_IT, uint32_t I2C_IT).
        (+) ITStatus I2C_GetITStatus(SC_IT_TypeDef* SCx_IT, uint32_t I2C_IT).

@endverbatim
  * @{
  */

/**
  * @brief  Deinitializes the SCx_I2C peripheral registers to their default reset values.
  * @param  SCx_I2C: where x can be 1 or 2 to select the Serial controller peripheral.
  * @retval None
  */
void I2C_DeInit(SC_I2C_TypeDef* SCx_I2C)
{
  /* Check the parameters */
  assert_param(IS_I2C_PERIPH(SCx_I2C));
  
  SCx_I2C->CR = 0;
  SCx_I2C->I2CCR1 = 0;
  SCx_I2C->I2CCR2 = 0;
  SCx_I2C->CRR1 = 0;
  SCx_I2C->CRR2 = 0;
}

/**
  * @brief  Initializes the SCx_I2C peripheral according to the specified
  *         parameters in the I2C_InitStruct.
  * @param  SCx_I2C: where x can be 1 or 2 to select the Serial controller peripheral.
  * @param  I2C_InitStruct: pointer to a I2C_InitTypeDef structure that
  *         contains the configuration information for the specified SCx_I2C peripheral.
  * @retval None
  */
void I2C_Init(SC_I2C_TypeDef* SCx_I2C, I2C_InitTypeDef* I2C_InitStruct)
{
  uint8_t expvalue = 0, lin = 0, tmpexp = 0, tmplin = 0, idx = 0;
  uint32_t tempclockrate = 0, clockrate = 0, power = 0;
  
  /* Check the parameters */
  assert_param(IS_I2C_PERIPH(SCx_I2C));
  assert_param(IS_I2C_CLOCK_RATE(I2C_InitStruct->I2C_ClockRate));
  
  /*---------------------------- SCx_CRRx Configuration ----------------------*/
  /*---------------------------- SCx_CRRx Configuration ----------------------*/
  for(lin = 0x00; lin < 16; lin++)
  {
    for(expvalue = 0x00; expvalue < 16; expvalue++)
    {
      power = 0x01;
      for (idx=1; idx <= expvalue; idx++)
      {
        power*=2;
      }
      tempclockrate = power * (uint32_t)(lin + 1);
      
      if (tempclockrate == ((uint32_t)((uint32_t)CLK_GetClocksFreq()/(uint32_t)(2*(I2C_InitStruct->I2C_ClockRate)))))
      {
        SCx_I2C->CRR1 = (uint32_t)lin;
        SCx_I2C->CRR2 = (uint32_t)expvalue;
      }
      else
      {
        /* Check the clock rate error and write to CRRx */
        if((ABS((int32_t)tempclockrate - (int32_t)(CLK_GetClocksFreq()/(2*(I2C_InitStruct->I2C_ClockRate))))) < (ABS((int32_t)clockrate - (int32_t)(CLK_GetClocksFreq()/(2*(I2C_InitStruct->I2C_ClockRate))))))
        {
          clockrate = tempclockrate;
          tmplin = lin;
          tmpexp = expvalue;
        }
      }
    }
  }
  
  SCx_I2C->CRR1 = (uint32_t)tmplin;
  SCx_I2C->CRR2 = (uint32_t)tmpexp;
}

/**
  * @brief  Fills each I2C_InitStruct member with its default value.
  * @param  I2C_InitStruct: pointer to a I2C_InitTypeDef structure which will be initialized.
  * @retval None
  */
void I2C_StructInit(I2C_InitTypeDef* I2C_InitStruct)
{
  /*--------------- Reset I2C init structure parameters values ---------------*/
  
  /* Initialize the I2C_ClockRate member */
  I2C_InitStruct->I2C_ClockRate = 400000;
}

/**
  * @brief  Generates SCx_I2C communication START condition.
  * @param  SCx_I2C: where x can be 1 or 2 to select the Serial controller peripheral.
  * @retval None
  */
void I2C_GenerateSTART(SC_I2C_TypeDef* SCx_I2C)
{
  /* Check the parameters */
  assert_param(IS_I2C_PERIPH(SCx_I2C));
  
  /* Generate a START condition */
  SCx_I2C->I2CCR1 |= SC_I2CCR1_START;
}  

/**
  * @brief  Generates SCx_I2C communication STOP condition.
  * @param  SCx_I2C: where x can be 1 or 2 to select the Serial controller peripheral.
  * @retval None
  */
void I2C_GenerateSTOP(SC_I2C_TypeDef* SCx_I2C)
{
  /* Check the parameters */
  assert_param(IS_I2C_PERIPH(SCx_I2C));
  
  /* Generate a STOP condition */
  SCx_I2C->I2CCR1 |= SC_I2CCR1_STOP;
} 

/**
  * @brief  Generates SCx_I2C communication Acknowledge.
  * @param  SCx_I2C: where x can be 1 or 2 to select the Serial controller peripheral.
  * @param  NewState: new state of the Acknowledge.
  *   This parameter can be: ENABLE or DISABLE.  
  * @retval None
  */
void I2C_AcknowledgeConfig(SC_I2C_TypeDef* SCx_I2C, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_I2C_PERIPH(SCx_I2C));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable ACK generation */
    SCx_I2C->I2CCR2 |= SC_I2CCR2_ACK; 
  }
  else
  {
    /* Enable NACK generation */
    SCx_I2C->I2CCR2 &= (uint32_t)~((uint32_t)SC_I2CCR2_ACK);
  }
}

/**
  * @brief  Transmits the address byte to select the slave device.
  * @param  SCx_I2C: where x can be 1 or 2 to select the Serial controller peripheral.
  * @param  Address: specifies the slave address which will be transmitted
  * @param  I2C_Direction: specifies whether the SCx_I2C device will be a
  *   Transmitter or a Receiver. This parameter can be one of the following values
  *     @arg I2C_Direction_Transmitter: Transmitter mode
  *     @arg I2C_Direction_Receiver: Receiver mode
  * @retval None.
  */
void I2C_Send7bitAddress(SC_I2C_TypeDef* SCx_I2C, uint8_t Address, uint8_t I2C_Direction)
{
  uint32_t scxbase = 0x00;
  /* Check the parameters */
  assert_param(IS_I2C_PERIPH(SCx_I2C));
  assert_param(IS_I2C_DIRECTION(I2C_Direction));
  /* Test on the direction to set/reset the read/write bit */
  if (I2C_Direction != I2C_Direction_Transmitter)
  {
    /* Set the address bit0 for read */
    Address |= OAR1_ADD0_Set;
  }
  else
  {
    /* Reset the address bit0 for write */
    Address &= OAR1_ADD0_Reset;
  }
  
  scxbase = (uint32_t)SCx_I2C; 
  /* Send the address */
  *(__IO uint32_t *) scxbase = Address;
  
  /* Enable the byte Send */
  SCx_I2C->I2CCR1 |= SC_I2CCR1_BTE;
  
  while ((SCx_I2C->I2CCR1 & SC_I2CCR1_BTE) != 0x00)
  {}
}

/**
  * @brief  Transmits a Data through the SCx_I2C peripheral.
  * @param  SCx_I2C: where x can be 1 or 2 to select the Serial controller peripheral.
  * @param  Data: Data to be transmitted.
  * @retval None
  */
void I2C_SendData(SC_I2C_TypeDef* SCx_I2C, uint8_t Data)
{
  uint32_t scxbase = 0x00;

  /* Check the parameters */
  assert_param(IS_I2C_PERIPH(SCx_I2C));

  scxbase = (uint32_t)SCx_I2C; 
  
  *(__IO uint32_t *) scxbase = Data;
  
  /* Enable the byte Send */
  SCx_I2C->I2CCR1 |= SC_I2CCR1_BTE;
  
  while ((SCx_I2C->I2CCR1 & SC_I2CCR1_BTE) != 0x00)
  {}
  while ((SCx_I2C->I2CSR & SC_I2CSR_BTF) == 0x00)
  {}
}

/**
  * @brief  Returns the most recent received data by the SCx_I2C peripheral. 
  * @param  SCx_I2C: where x can be 1 or 2 to select the Serial controller peripheral.
  * @retval The value of the received data.
  */
uint8_t I2C_ReceiveData(SC_I2C_TypeDef* SCx_I2C)
{
  uint32_t scxbase = 0x00;
  
  /* Check the parameters */
  assert_param(IS_I2C_PERIPH(SCx_I2C));
  
  scxbase = (uint32_t)SCx_I2C; 

  /* Enable the byte Receive */
  SCx_I2C->I2CCR1 |= SC_I2CCR1_BRE;
  
  while ((SCx_I2C->I2CCR1 & SC_I2CCR1_BRE) != 0x00)
  {}
  while ((SCx_I2C->I2CSR & SC_I2CSR_BRF) == 0x00)
  {}

  return *(__IO uint32_t *) scxbase;
}

/**
  * @brief  Enables or disables the specified SCx_I2C peripheral.
  * @param  SCx_I2C: where x can be 1 or 2 to select the Serial controller peripheral.
  * @param  NewState: new state of the SCx_I2C peripheral.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void I2C_Cmd(SC_I2C_TypeDef* SCx_I2C, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_I2C_PERIPH(SCx_I2C));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable the selected I2C by setting the MODE bits in the CR register */
    SCx_I2C->CR |= SC_CR_MODE;
  }
  else
  {
    /* Disable the selected I2C by clearing the MODE bits in the CR register */
    SCx_I2C->CR &= (uint16_t)~((uint16_t)SC_CR_MODE);
  }
}

/**
  * @brief  Enables or disables the specified SCx_I2C interrupts.
  * @param  SCx_IT: where x can be 1 or 2 to select the Serial controller peripheral.
  * @param  I2C_IT: specifies the SCx_I2C interrupt source to be enabled or disabled. 
  *   This parameter can be one of the following values:
  *     @arg I2C_IT_NACK: Not acknowledge interrupt mask
  *     @arg I2C_IT_CMDFIN: Command finished interrupt mask
  *     @arg I2C_IT_BTF: Byte transfer finished interrupt mask
  *     @arg I2C_IT_BRF: Byte receive finished interrupt mask
  * @param  NewState: new state of the specified SCx_I2C interrupt source.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void I2C_ITConfig(SC_IT_TypeDef* SCx_IT, uint32_t I2C_IT, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_I2C_SPI_DMA_IT_PERIPH(SCx_IT));
  assert_param(IS_I2C_IT(I2C_IT));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable the selected I2C interrupt */
    SCx_IT->IER |= I2C_IT;
  }
  else
  {
    /* Disable the selected I2C interrupt */
    SCx_IT->IER &= (uint16_t)~((uint16_t)I2C_IT);
  }
}

/**
  * @brief  Checks whether the specified I2C flag is set or not.
  * @param  SCx_I2C: where x can be 1 or 2 to select the Serial controller peripheral.
  * @param  I2C_FLAG: specifies the SCx_I2C flag to check.
  *   This parameter can be one of the following values:
  *     @arg I2C_FLAG_NACK: Not acknowledge flag
  *     @arg I2C_FLAG_BTF: Byte transfer finished flag
  *     @arg I2C_FLAG_BRF: Byte receive finished flag
  *     @arg I2C_FLAG_CMDFIN: Command finished flag
  * @retval The new state of I2C_FLAG (SET or RESET).
  */
FlagStatus I2C_GetFlagStatus(SC_I2C_TypeDef* SCx_I2C, uint32_t I2C_FLAG)
{
  FlagStatus bitstatus = RESET;
  
  /* Check the parameters */
  assert_param(IS_I2C_PERIPH(SCx_I2C));
  assert_param(IS_I2C_GET_FLAG(I2C_FLAG));
  
  if ((SCx_I2C->I2CSR & I2C_FLAG) != (uint32_t)RESET)
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
  * @brief  Checks whether the specified SCx_I2C pending interrupt is set or not.
  * @param  SCx_IT: where x can be 1 or 2 to select the Serial controller peripheral.
  * @param  I2C_IT: specifies the SCx_I2C interrupt pending to check.
  *   This parameter can be one of the following values:
  *     @arg I2C_IT_NACK: Not acknowledge interrupt pending
  *     @arg I2C_IT_CMDFIN: Command finished interrupt pending
  *     @arg I2C_IT_BTF: Byte transfer finished interrupt pending
  *     @arg I2C_IT_BRF: Byte receive finished interrupt pending
  * @retval The new state of I2C_IT (SET or RESET).
  */
ITStatus I2C_GetITStatus(SC_IT_TypeDef* SCx_IT, uint32_t I2C_IT)
{
  ITStatus bitstatus = RESET;
  uint32_t enablestatus = 0;
  
  /* Check the parameters */
  assert_param(IS_I2C_SPI_DMA_IT_PERIPH(SCx_IT));
  assert_param(IS_I2C_IT(I2C_IT));
  
  enablestatus = (uint32_t)(SCx_IT->IER & I2C_IT);
  if (((SCx_IT->ISR & I2C_IT) != (uint32_t)RESET) && enablestatus)
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
  * @brief  Clears the SCx_I2C interrupt pending bits.
  * @param  SCx_IT: where x can be 1 to select the Serial controller peripheral.
  * @param  I2C_IT: specifies the SCx_I2C interrupt pending to check.
  *   This parameter can be one of the following values:
  *     @arg I2C_IT_NACK: Not acknowledge interrupt pending
  *     @arg I2C_IT_CMDFIN: Command finished interrupt pending
  *     @arg I2C_IT_BTF: Byte transfer finished interrupt pending
  *     @arg I2C_IT_BRF: Byte receive finished interrupt pending
  * @retval None
  */
void I2C_ClearITPendingBit(SC_IT_TypeDef* SCx_IT, uint32_t I2C_IT)
{
  /* Check the parameters */
  assert_param(IS_I2C_SPI_DMA_IT_PERIPH(SCx_IT));
  assert_param(IS_I2C_IT(I2C_IT));
  
  SCx_IT->ISR = I2C_IT;
}

/**
  * @}
  */

/** @defgroup SC_Group4 DMA transfers management
 *  @brief   DMA transfers management functions 
 *
@verbatim   
 ===============================================================================
                 ##### DMA transfers management functions #####
 ===============================================================================
    [..] This section provides a set of functions that can be used only in DMA mode.
   
    (@) The SC1 and SC2 include a DMA controller that can be used to manage the UART
        and SPI communications.

*** Initialization and Configuration ***
 =======================================
    [..] For the DMA mode these parameters can be configured: 
         (+) Begin address buffer A.
         (+) End address buffer A.
         (+) Begin address buffer B.
         (+) End address buffer B.

    [..] The DMA_Init() function follows the DMA configuration procedure.

*** Data transfers ***
 =====================
    [..] In DMA Mode, the UART and SPI communications can be managed by 4 DMA Channel 
         requests:
         (#) DMA_ChannelLoad_BTx: specifies the DMA transmit channel buffer B transfer request.
         (#) DMA_ChannelLoad_ATx: specifies the DMA transmit channel buffer A transfer request.
         (#) DMA_ChannelLoad_BRx: specifies the DMA receive channel buffer B transfer request.
         (#) DMA_ChannelLoad_ARx: specifies the DMA receive channel buffer A transfer request.
    [..] In this Mode it is advised to use the following function to load and enables the specified DMA channel:
        (+) void SC_DMA_ChannelLoadEnable(SC_DMA_TypeDef* SCx_DMA, uint32_t Channelxy).

*** Interrupts and flags management ***
 =======================================
    [..] This subsection provides also a set of functions allowing to configure the 
         DMA Interrupts sources, Requests and check or clear the flags or pending bits status. 
         The user should identify which mode will be used in his application to 
         manage the communication: Polling mode, Interrupt mode.

    [..] In Polling Mode, the DMA communication can be managed by 4 flags:
        (#) DMA_FLAG_RXAACK: to indicate the status of the DMA receive buffer A acknowledge flag.
        (#) DMA_FLAG_RXBACK: to indicate the status of the DMA receive buffer B acknowledge flag.
        (#) DMA_FLAG_TXAACK: to indicate the status of the DMA transmit buffer A acknowledge flag.
        (#) DMA_FLAG_TXBACK: to indicate the status of the DMA transmit buffer B acknowledge flag.
        (#) DMA_FLAG_OVRA: to indicate the status of the DMA buffer B overrun flag.
        (#) DMA_FLAG_OVRB: to indicate the status of the DMA buffer B overrun flag.
        (#) DMA_FLAG_PEA: to indicate the status of the DMA Parity error A flag.
        (#) DMA_FLAG_PEB: to indicate the status of the DMA Parity error B flag.
        (#) DMA_FLAG_FEA: to indicate the status of the DMA Frame error A flag.
        (#) DMA_FLAG_FEB: to indicate the status of the DMA Frame error B flag.
        (#) DMA_FLAG_NSSS: to indicate the status of the status of the receive count flag.
    [..] In this Mode it is advised to use the following functions:
        (+) FlagStatus SC_DMA_GetFlagStatus(SC_DMA_TypeDef* SCx_DMA, uint32_t DMA_FLAG).
    [..] In this mode all the DMA flags are cleared by hardware.

  *** Interrupt Mode ***
  ======================
    [..] In Interrupt Mode, the DMA communication can be managed by 4 interrupt sources
         and 4 pending bits:

    [..] Interrupt Source:
        (#) DMA_IT_TXULODB: specifies the interrupt source for the transmit buffer B unloaded interrupt.
        (#) DMA_IT_TXULODA: specifies the interrupt source for the transmit buffer A unloaded interrupt.
        (#) DMA_IT_RXULODB: specifies the interrupt source for the receive buffer B unloaded interrupt.
        (#) DMA_IT_RXULODA: specifies the interrupt source for the receive buffer A unloaded interrupt.
        
    [..] Pending Bits:
        (#) DMA_IT_TXULODB: to indicate the status of transmit buffer B unloaded pending interrupt.
        (#) DMA_IT_TXULODA: to indicate the status of transmit buffer A unloaded pending interrupt.
        (#) DMA_IT_RXULODB: to indicate the status of receive buffer B unloaded pending interrupt.
        (#) DMA_IT_RXULODA: to indicate the status of receive buffer A unloaded pending interrupt.
        
    [..] In this Mode it is advised to use the following functions:
        (+) ITStatus SC_DMA_GetITStatus(SC_IT_TypeDef* SCx_IT, uint32_t DMA_IT).
        (+) SC_DMA_ClearITPendingBit(SC_IT_TypeDef* SCx_IT, uint32_t DMA_IT).

@endverbatim
  * @{
  */

/**
  * @brief  Reset the specified SCx_DMA Channely buffer addresses.
  * @param  SCx_DMA: where x can be 1 or 2 to select the Serial controller peripheral.
  * @param  Channely: specifies the SCx_DMA channel to be enabled. 
  *   This parameter can be one of the following values:
  *     @arg DMA_ChannelReset_Tx: DMA reset transmit channels mask
  *     @arg DMA_ChannelReset_Rx: DMA reset receive channels mask
  * @retval None
  */
void SC_DMA_ChannelReset(SC_DMA_TypeDef* SCx_DMA, uint32_t Channely)
{
  /* Check the parameters */
  assert_param(IS_DMA_PERIPH(SCx_DMA));
  assert_param(IS_DMA_CHANNEL_RESET(Channely));
  
  /* Reset the selected SCx_DMA Channelx */
  SCx_DMA->DMACR |= Channely;
}

/**
  * @brief  Initializes the SCx_DMA Channely according to the specified
  *         parameters in the DMA_InitStruct.
  * @param  SCx_DMA_Channely: where x can be 1 or 2 to select the SCx_DMA and
  *   y can be Tx or Rx to select the SCx_DMA Channel.
  * @param  SC_DMA_InitStruct: pointer to a DMA_InitTypeDef structure that
  *         contains the configuration information for the specified DMA Channel.
  * @retval None
  */
void SC_DMA_Init(SC_DMA_Channel_TypeDef* SCx_DMA_Channely, SC_DMA_InitTypeDef* SC_DMA_InitStruct)
{
  /* Check the parameters */
  assert_param(IS_DMA_CHANNEL_PERIPH(SCx_DMA_Channely));
  assert_param(IS_DMA_VALID_ADDRESS(SC_DMA_InitStruct->DMA_BeginAddrA));
  assert_param(IS_DMA_VALID_ADDRESS(SC_DMA_InitStruct->DMA_EndAddrA));
  assert_param(IS_DMA_VALID_ADDRESS(SC_DMA_InitStruct->DMA_BeginAddrB));
  assert_param(IS_DMA_VALID_ADDRESS(SC_DMA_InitStruct->DMA_EndAddrB));
  
/*---------------------- SCx_DMA Channely DMABEGADDAR Configuration ----------*/
  /* Write to SCx_DMA Channely Begin address A register */
  SCx_DMA_Channely->DMABEGADDAR = SC_DMA_InitStruct->DMA_BeginAddrA;
  
  /*---------------------- SCx_DMA Channely DMAENDADDAR Configuration --------*/
  /* Write to SCx_DMA Channely end address A register */
  SCx_DMA_Channely->DMAENDADDAR = SC_DMA_InitStruct->DMA_EndAddrA;
  
  /*---------------------- SCx_DMA Channely DMABEGADDBR Configuration --------*/
  /* Write to SCx_DMA Channely Begin address B register */
  SCx_DMA_Channely->DMABEGADDBR = SC_DMA_InitStruct->DMA_BeginAddrB;
  
  /*---------------------- SCx_DMA Channely DMAENDADDBR Configuration --------*/
  /* Write to SCx_DMA Channely end address B register */
  SCx_DMA_Channely->DMAENDADDBR = SC_DMA_InitStruct->DMA_EndAddrB;
}

/**
  * @brief  Fills each DMA_InitStruct member with its default value.
  * @param  SC_DMA_InitStruct: pointer to a DMA_InitTypeDef structure which will be initialized.
  * @retval None
  */
void SC_DMA_StructInit(SC_DMA_InitTypeDef* SC_DMA_InitStruct)
{
  /*--------------- Reset DMA init structure parameters values ---------------*/
  /* Initialize the SCx_DMA_Channely begin address A member */
  SC_DMA_InitStruct->DMA_BeginAddrA = 0x20000000;
  /* Initialize the SCx_DMA_Channely end address A member */
  SC_DMA_InitStruct->DMA_EndAddrA = 0x20000000;
  /* Initialize the SCx_DMA_Channely begin address B member */
  SC_DMA_InitStruct->DMA_BeginAddrB = 0x20000000;
  /* Initialize the SCx_DMA_Channely end address B member */
  SC_DMA_InitStruct->DMA_EndAddrB = 0x20000000;
}

/**
  * @brief  Enables or disables the specified SCx_DMA interrupts.
  * @param  SCx_IT: where x can be 1 or 2 to select the Serial controller peripheral.
  * @param  DMA_IT: specifies the SCx_DMA interrupt source to be enabled or disabled. 
  *   This parameter can be one of the following values:
  *     @arg DMA_IT_TXULODB: DMA transmit buffer B unloaded interrupt mask
  *     @arg DMA_IT_TXULODA: DMA transmit buffer A unloaded interrupt mask
  *     @arg DMA_IT_RXULODB: DMA receive buffer B unloaded interrupt mask
  *     @arg DMA_IT_RXULODA: DMA receive buffer A unloaded interrupt mask
  * @param  NewState: new state of the specified SCx_DMA interrupt source.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void SC_DMA_ITConfig(SC_IT_TypeDef* SCx_IT, uint32_t DMA_IT, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_I2C_SPI_DMA_IT_PERIPH(SCx_IT));
  assert_param(IS_DMA_IT(DMA_IT));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable the selected DMA interrupt */
    SCx_IT->IER |= DMA_IT;
  }
  else
  {
    /* Disable the selected DMA interrupt */
    SCx_IT->IER &= (uint16_t)~((uint16_t)DMA_IT);
  }
}

/**
  * @brief  Load and Enables the specified SCx_DMA Channelxy buffers addresses.
  * @param  SCx_DMA: where x can be 1 or 2 to select the Serial controller peripheral.
  * @param  Channelxy: specifies the SCx_DMA channel to be enabled. 
  *   This parameter can be one of the following values:
  *     @arg DMA_ChannelLoad_BTx: DMA transmit channel buffer B mask
  *     @arg DMA_ChannelLoad_ATx: DMA transmit channel buffer A mask
  *     @arg DMA_ChannelLoad_BRx: DMA receive channel buffer B mask
  *     @arg DMA_ChannelLoad_ARx: DMA receive channel buffer A mask
  * @retval None
  */
void SC_DMA_ChannelLoadEnable(SC_DMA_TypeDef* SCx_DMA, uint32_t Channelxy)
{
  /* Check the parameters */
  assert_param(IS_DMA_PERIPH(SCx_DMA));
  assert_param(IS_DMA_CHANNEL_LOAD(Channelxy));
  
  /* Load the selected SCx_DMA Channelxy buffer addresses and allow the DMA controller
     to start processing */
  SCx_DMA->DMACR |= Channelxy;
}

/**
  * @brief  Returns the most recent value for the specific SCx_DMA counter register.
  * @param  SCx_DMA: where x can be 1 or 2 to select the Serial controller peripheral.
  * @param  Counter: specifies the SCx_DMA counter register to be read. 
  *   This parameter can be one of the following values:
  *     @arg DMA_Counter_RXCNTA: DMA receive counter A register
  *     @arg DMA_Counter_RXCNTB: DMA receive counter B register
  *     @arg DMA_Counter_TXCNT: DMA transmit counter register
  *     @arg DMA_Counter_RXCNTSAVED: DMA receive counter saved register
  * @retval The DMA register counter value.
  */
uint32_t SC_DMA_GetCounter(SC_DMA_TypeDef* SCx_DMA, uint32_t Counter)
{
  uint32_t scxbase = 0x00;
  
  /* Check the parameters */
  assert_param(IS_DMA_PERIPH(SCx_DMA));
  assert_param(IS_DMA_COUNTER(Counter));
  
  scxbase = (uint32_t)SCx_DMA; 
  scxbase += Counter;

  return *(__IO uint32_t *) scxbase;
}

/**
  * @brief  Returns the specified SCx_DMA receive error register.
  * @param  SCx_DMA: where x can be 1 or 2 to select the Serial controller peripheral.
  * @param  RegisterError: specifies the SCx_DMA receiver error register to be read. 
  *   This parameter can be one of the following values:
  *     @arg DMA_ReceiverError_CNTA: DMA receive error register A
  *     @arg DMA_ReceiverError_CNTB: DMA receive error register B
  * @retval The DMA receive error register value.
  */
uint32_t SC_DMA_GetReceiverErrorOffset(SC_DMA_TypeDef* SCx_DMA, uint32_t RegisterError)
{
  uint32_t scxbase = 0x00;
  
  /* Check the parameters */
  assert_param(IS_DMA_PERIPH(SCx_DMA));
  assert_param(IS_DMA_RECEIVER_ERROR(RegisterError));
  
  scxbase = (uint32_t)SCx_DMA; 
  scxbase += RegisterError;

  return *(__IO uint32_t *) scxbase;
}

/**
  * @brief  Checks whether the specified DMA flag is set or not.
  * @param  SCx_DMA: where x can be 1 or 2 to select the Serial controller peripheral.
  * @param  DMA_FLAG: specifies the SCx_DMA flag to check.
  *   This parameter can be one of the following values:
  *     @arg DMA_FLAG_RXAACK: DMA receive buffer A acknowledge flag
  *     @arg DMA_FLAG_RXBACK: DMA receive buffer B acknowledge flag
  *     @arg DMA_FLAG_TXAACK: DMA transmit buffer A acknowledge flag
  *     @arg DMA_FLAG_TXBACK: DMA transmit buffer B acknowledge flag
  *     @arg DMA_FLAG_OVRA: DMA buffer B overrun flag
  *     @arg DMA_FLAG_OVRB: DMA buffer B overrun flag
  *     @arg DMA_FLAG_PEA: DMA Parity error A flag
  *     @arg DMA_FLAG_PEB: DMA Parity error B flag
  *     @arg DMA_FLAG_FEA: DMA Frame error A flag
  *     @arg DMA_FLAG_FEB: DMA Frame error B flag
  *     @arg DMA_FLAG_NSSS: DMA Status of the receive count flag
  * @retval The new state of DMA_FLAG (SET or RESET).
  */
FlagStatus SC_DMA_GetFlagStatus(SC_DMA_TypeDef* SCx_DMA, uint32_t DMA_FLAG)
{
  FlagStatus bitstatus = RESET;
  
  /* Check the parameters */
  assert_param(IS_DMA_PERIPH(SCx_DMA));
  assert_param(IS_DMA_FLAG(DMA_FLAG));
  
  if ((SCx_DMA->DMASR & DMA_FLAG) != (uint32_t)RESET)
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
  * @brief  Checks whether the specified SCx_DMA pending interrupt is set or not.
  * @param  SCx_IT: where x can be 1 or 2 to select the Serial controller peripheral.
  * @param  DMA_IT: specifies the SCx_DMA interrupt pending to check.
  *   This parameter can be one of the following values:
  *     @arg DMA_IT_TXULODB: DMA transmit buffer B unloaded interrupt pending
  *     @arg DMA_IT_TXULODA: DMA transmit buffer A unloaded interrupt pending
  *     @arg DMA_IT_RXULODB: DMA receive buffer B unloaded interrupt pending
  *     @arg DMA_IT_RXULODA: DMA receive buffer A unloaded interrupt pending
  * @retval The new state of DMA_IT (SET or RESET).
  */
ITStatus SC_DMA_GetITStatus(SC_IT_TypeDef* SCx_IT, uint32_t DMA_IT)
{
  ITStatus bitstatus = RESET;
  uint32_t enablestatus = 0;
  
  /* Check the parameters */
  assert_param(IS_I2C_SPI_DMA_IT_PERIPH(SCx_IT));
  assert_param(IS_DMA_IT(DMA_IT));
  
  enablestatus = (uint32_t)(SCx_IT->IER & DMA_IT);
  if (((SCx_IT->ISR & DMA_IT) != (uint32_t)RESET) && enablestatus)
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
  * @brief  Clears the SCx_DMA interrupt pending bits.
  * @param  SCx_IT: where x can be 1 to select the Serial controller peripheral.
  * @param  DMA_IT: specifies the SCx_DMA interrupt pending to check.
  *   This parameter can be one of the following values:
  *     @arg DMA_IT_TXULODB: DMA transmit buffer B unloaded interrupt pending
  *     @arg DMA_IT_TXULODA: DMA transmit buffer A unloaded interrupt pending
  *     @arg DMA_IT_RXULODB: DMA receive buffer B unloaded interrupt pending
  *     @arg DMA_IT_RXULODA: DMA receive buffer A unloaded interrupt pending
  * @retval None
  */
void SC_DMA_ClearITPendingBit(SC_IT_TypeDef* SCx_IT, uint32_t DMA_IT)
{
  /* Check the parameters */
  assert_param(IS_I2C_SPI_DMA_IT_PERIPH(SCx_IT));
  assert_param(IS_DMA_IT(DMA_IT));
  
  SCx_IT->ISR = DMA_IT;
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
