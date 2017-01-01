/**
  ******************************************************************************
  * @file    stm32w108xx_flash.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    30-November-2012
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the FLASH peripheral:
  *            + FLASH Interface configuration
  *            + FLASH Memory Programming
  *            + Option Bytes Programming
  *            + Interrupts and flags management
  * 
  *  @verbatim
  *    
 ===============================================================================
                    ##### How to use this driver #####
 ===============================================================================
    [..] This driver provides functions to configure and program the Flash 
         memory of all STM32W108xx devices. These functions are split in 4 groups
         (#) FLASH Interface configuration functions: this group includes the 
             management of following features:
             (++) Set the latency
             (++) Enable/Disable the prefetch buffer

         (#) FLASH Memory Programming functions: this group includes all needed 
             functions to erase and program the main memory:
             (++) Lock and Unlock the Flash interface.
             (++) Erase function: Erase Page, erase all pages.
             (++) Program functions: Half Word and Word write.

         (#) FLASH Option Bytes Programming functions: this group includes all 
             needed functions to:
             (++) Set/Reset the write protection
             (++) Set the Read protection Level
             (++) Get the Write protection
             (++) Get the read protection status

         (#) FLASH Interrupts and flag management functions: this group includes 
             all needed functions to:
             (++) Enable/Disable the flash interrupt sources
             (++) Get flags status
             (++) Clear flags
             (++) Get Flash operation status
             (++) Wait for last flash operation

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
#include "stm32w108xx_flash.h"

/** @addtogroup STM32W108xx_StdPeriph_Driver
  * @{
  */

/** @defgroup FLASH 
  * @brief FLASH driver modules
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Flash Access Control Register bits */
#define ACR_LATENCY_Mask         ((uint32_t)0x00000038)
#define ACR_HLFCYA_Mask          ((uint32_t)0xFFFFFFF7)
#define ACR_PRFTBE_Mask          ((uint32_t)0xFFFFFFEF)

/* Flash Access Control Register bits */
#define ACR_PRFTBS_Mask          ((uint32_t)0x00000020) 

/* Flash Control Register bits */
#define CR_PG_Set                ((uint32_t)0x00000001)
#define CR_PG_Reset              ((uint32_t)0x00001FFE) 
#define CR_PER_Set               ((uint32_t)0x00000002)
#define CR_PER_Reset             ((uint32_t)0x00001FFD)
#define CR_MER_Set               ((uint32_t)0x00000004)
#define CR_MER_Reset             ((uint32_t)0x00001FFB)
#define CR_OPTPG_Set             ((uint32_t)0x00000010)
#define CR_OPTPG_Reset           ((uint32_t)0x00001FEF)
#define CR_OPTER_Set             ((uint32_t)0x00000020)
#define CR_OPTER_Reset           ((uint32_t)0x00001FDF)
#define CR_STRT_Set              ((uint32_t)0x00000040)
#define CR_LOCK_Set              ((uint32_t)0x00000080)

/* FLASH Mask */
#define RDPRT_Mask               ((uint32_t)0x00000002)
#define WRP0_Mask                ((uint32_t)0x000000FF)
#define WRP1_Mask                ((uint32_t)0x0000FF00)
#define WRP2_Mask                ((uint32_t)0x00FF0000)
#define WRP3_Mask                ((uint32_t)0xFF000000)

/* FLASH Keys */
#define RDP_Key                  ((uint16_t)0x00A5)
#define FLASH_KEY1               ((uint32_t)0x45670123)
#define FLASH_KEY2               ((uint32_t)0xCDEF89AB)

/* Delay definition */   
#define EraseTimeout          ((uint32_t)0x000B0000)
#define ProgramTimeout        ((uint32_t)0x00002000)
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/** @defgroup FLASH_Private_Functions
  * @{
  */

/**
@code  
 
 This driver provides functions to configure and program the Flash memory of all STM32W108xx devices.

 STM32W108xx devices features only one bank with memory up to 256 Kbytes.
 
@endcode
*/

/** @defgroup FLASH_Group1 FLASH Interface configuration functions
  *  @brief   FLASH Interface configuration functions 
  *
@verbatim   
 ===============================================================================
               ##### FLASH Interface configuration functions #####
 ===============================================================================

    [..] FLASH_Interface configuration_Functions, includes the following functions:
       (+) void FLASH_SetLatency(uint32_t FLASH_Latency):
    [..] To correctly read data from Flash memory, the number of wait states (LATENCY) 
     must be correctly programmed according to the frequency of the CPU clock (SCLK) 
       (+) FlagStatus FLASH_GetPrefetchBufferStatus(void);
       (+) void FLASH_PrefetchBufferCmd(FunctionalState NewState);
    [..]
     All these functions don't need the unlock sequence.

@endverbatim
  * @{
  */
  
/**
  * @brief  Sets the code latency value.
  * @param  FLASH_Latency: specifies the FLASH Latency value.
  *   This parameter can be one of the following values:
  *     @arg FLASH_Latency_0: FLASH Zero Latency cycle
  *     @arg FLASH_Latency_1: FLASH One Latency cycle
  *     @arg FLASH_Latency_2: FLASH Two Latency cycles
  * @retval None
  */
void FLASH_SetLatency(uint32_t FLASH_Latency)
{
  uint32_t tmpreg = 0;
  
  /* Check the parameters */
  assert_param(IS_FLASH_LATENCY(FLASH_Latency));
  
  /* Read the ACR register */
  tmpreg = FLASH->ACR;  
  
  /* Sets the Latency value */
  tmpreg &= ACR_LATENCY_Mask;
  tmpreg |= FLASH_Latency;
  
  /* Write the ACR register */
  FLASH->ACR = tmpreg;
}

/**
  * @brief  Checks whether the FLASH Prefetch Buffer status is set or not.
  * @param  None
  * @retval FLASH Prefetch Buffer Status (SET or RESET).
  */
FlagStatus FLASH_GetPrefetchBufferStatus(void)
{
  FlagStatus bitstatus = RESET;
  
  if ((FLASH->ACR & ACR_PRFTBS_Mask) != (uint32_t)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  /* Return the new state of FLASH Prefetch Buffer Status (SET or RESET) */
  return bitstatus; 
}

/**
  * @brief  Enables or disables the Prefetch Buffer.
  * @param  FLASH_PrefetchBuffer: specifies the Prefetch buffer status.
  *   This parameter can be one of the following values:
  *     @arg FLASH_PrefetchBuffer_Enable: FLASH Prefetch Buffer Enable
  *     @arg FLASH_PrefetchBuffer_Disable: FLASH Prefetch Buffer Disable
  * @retval None
  */
void FLASH_PrefetchBufferCmd(uint32_t FLASH_PrefetchBuffer)
{
  /* Check the parameters */
  assert_param(IS_FLASH_PREFETCHBUFFER_STATE(FLASH_PrefetchBuffer));
  
  /* Enable or disable the Prefetch Buffer */
  FLASH->ACR &= ACR_PRFTBE_Mask;
  FLASH->ACR |= FLASH_PrefetchBuffer;
}
/**
  * @}
  */
/** @defgroup FLASH_Group2 FLASH Memory Programming functions
 *  @brief   FLASH Memory Programming functions
 *
@verbatim   
 ===============================================================================
                ##### FLASH Memory Programming functions #####
 ===============================================================================

    [..] The FLASH Memory Programming functions, includes the following functions:
       (+) void FPEC_ClockCmd(FunctionalState NewState)
       (+) void FLASH_Unlock(void);
       (+) void FLASH_Lock(void);
       (+) void FLASH_HalfCycleAccessCmd(uint32_t FLASH_HalfCycleAccess)
       (+) FLASH_Status FLASH_ErasePage(uint32_t Page_Address);
       (+) FLASH_Status FLASH_EraseAllPages(void);
       (+) FLASH_Status FLASH_ProgramWord(uint32_t Address, uint32_t Data);
       (+) FLASH_Status FLASH_ProgramHalfWord(uint32_t Address, uint16_t Data);

    [..] Any operation of erase or program should follow these steps:
       
       (#) Call the FPEC_ClockCmd() function to enable/disable the flash control 
          register and enable/disable flash clock.
       (#) Call the desired function to erase page or program data.
       
@endverbatim
  * @{
  */
    
/**
  * @brief  Enables or disables the FPEC clock.
  * @param  NewState: new state of the FPEC clock.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None.
  */
void FPEC_ClockCmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  
  if (NewState != DISABLE)
  {
    /* Unlock the FPEC */
    FLASH->KEYR = FLASH_KEY1;
    FLASH->KEYR = FLASH_KEY2;
    /* Unlock the Option bytes write enable (OPTWRE) */
    FLASH->OPTKEYR = FLASH_KEY1;
    FLASH->OPTKEYR = FLASH_KEY2;
    
    /* Enable the FPEC Clock */
    FLASH->CLKER |= (uint32_t)FLASH_CLKER_EN;
    
    /* Verifying that the FPEC clock is running before proceeding */
    while( (FLASH->CLKSR&FLASH_CLKSR_ACK) != FLASH_CLKSR_ACK) 
    {}
    
    /* Wait until the flash is no longer busy */
    while( (FLASH->SR&FLASH_SR_BSY) == (uint32_t)FLASH_SR_BSY ) 
    {}
  }
  else
  {
    /* Verifying the FPEC is completely idle before turning off the clock */
    while( (FLASH->CLKSR&FLASH_CLKSR_BSY) == FLASH_CLKSR_BSY) 
    {}
    /* Disable the FPEC Clock */
    FLASH->CLKER &= (uint32_t)~FLASH_CLKER_EN;
    /* Set the Lock Bit to lock the FPEC and the CR of flash */
    FLASH->CR |= (uint32_t)FLASH_CR_LOCK;
  }
}

/**
  * @brief  Unlocks the FLASH Program Erase Controller. 
  * @param  None
  * @retval None
  */
void FLASH_Unlock(void)
{
  /* Unlock the FPEC */
  FLASH->KEYR = FLASH_KEY1;
  FLASH->KEYR = FLASH_KEY2; 
}

/**
  * @brief  Locks the FLASH Program Erase Controller.
  * @param  None
  * @retval None
  */
void FLASH_Lock(void)
{
  /* Set the Lock Bit to lock the FPEC and the CR of flash */
  FLASH->CR |= CR_LOCK_Set;

}

/**
  * @brief  Enables or disables the Half cycle flash access.
  * @param  FLASH_HalfCycleAccess: specifies the FLASH Half cycle Access mode.
  *   This parameter can be one of the following values:
  *     @arg FLASH_HalfCycleAccess_Enable: FLASH Half Cycle Enable
  *     @arg FLASH_HalfCycleAccess_Disable: FLASH Half Cycle Disable
  * @retval None
  */
void FLASH_HalfCycleAccessCmd(uint32_t FLASH_HalfCycleAccess)
{
  /* Check the parameters */
  assert_param(IS_FLASH_HALFCYCLEACCESS_STATE(FLASH_HalfCycleAccess));
  
  /* Enable or disable the Half cycle access */
  FLASH->ACR &= ACR_HLFCYA_Mask;
  FLASH->ACR |= FLASH_HalfCycleAccess;
}

/**
  * @brief  Erases a specified FLASH page.
  * @param  Page_Address: The page address to be erased.
  * @retval FLASH Status: The returned value can be: FLASH_BUSY, FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_ErasePage(uint32_t Page_Address)
{
  FLASH_Status status = FLASH_COMPLETE;
  /* Check the parameters */
  assert_param(IS_FLASH_ADDRESS(Page_Address));

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(EraseTimeout);
  
  if(status == FLASH_COMPLETE)
  { 
    /* if the previous operation is completed, proceed to erase the page */
    FLASH->CR|= CR_PER_Set;
    FLASH->AR = Page_Address; 
    FLASH->CR|= CR_STRT_Set;
    
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(EraseTimeout);
    
    /* Disable the PER Bit */
    FLASH->CR &= CR_PER_Reset;
  }
  /* Return the Erase Status */
  return status;
}

/**
  * @brief  Erases all FLASH pages.
  * @param  None
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_EraseAllPages(void)
{
  FLASH_Status status = FLASH_COMPLETE;

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(EraseTimeout);
  if(status == FLASH_COMPLETE)
  {
    /* if the previous operation is completed, proceed to erase all pages */
     FLASH->CR |= CR_MER_Set;
     FLASH->CR |= CR_STRT_Set;
    
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(EraseTimeout);

    /* Disable the MER Bit */
    FLASH->CR &= CR_MER_Reset;
  }
  /* Return the Erase Status */
  return status;
}

/**
  * @brief  Programs a word at a specified address.
  * @param  Address: specifies the address to be programmed.
  * @param  Data: specifies the data to be programmed.
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT. 
  */
FLASH_Status FLASH_ProgramWord(uint32_t Address, uint32_t Data)
{
  FLASH_Status status = FLASH_COMPLETE;
  __IO uint32_t tmp = 0;

  /* Check the parameters */
  assert_param(IS_FLASH_ADDRESS(Address));

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(ProgramTimeout);
  
  if(status == FLASH_COMPLETE)
  {
    /* if the previous operation is completed, proceed to program the new first 
    half word */
    FLASH->CR |= CR_PG_Set;
  
    *(__IO uint16_t*)Address = (uint16_t)Data;
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(ProgramTimeout);
 
    if(status == FLASH_COMPLETE)
    {
      /* if the previous operation is completed, proceed to program the new second 
      half word */
      tmp = Address + 2;

      *(__IO uint16_t*) tmp = Data >> 16;
    
      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastOperation(ProgramTimeout);
        
      /* Disable the PG Bit */
      FLASH->CR &= CR_PG_Reset;
    }
    else
    {
      /* Disable the PG Bit */
      FLASH->CR &= CR_PG_Reset;
    }
  }    
  /* Return the Program Status */
  return status;
}

/**
  * @brief  Programs a half word at a specified address.
  * @param  Address: specifies the address to be programmed.
  * @param  Data: specifies the data to be programmed.
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT. 
  */
FLASH_Status FLASH_ProgramHalfWord(uint32_t Address, uint16_t Data)
{
  FLASH_Status status = FLASH_COMPLETE;
  /* Check the parameters */
  assert_param(IS_FLASH_ADDRESS(Address));

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(ProgramTimeout);
  
  if(status == FLASH_COMPLETE)
  {
    /* if the previous operation is completed, proceed to program the new data */
    FLASH->CR |= CR_PG_Set;
  
    *(__IO uint16_t*)Address = Data;
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(ProgramTimeout);
    
    /* Disable the PG Bit */
    FLASH->CR &= CR_PG_Reset;
  }   
  /* Return the Program Status */
  return status;
}

/**
  * @}
  */
  
/** @defgroup FLASH_Group3 Option Bytes Programming functions
 *  @brief   Option Bytes Programming functions 
 *
@verbatim   
 ===============================================================================
                ##### Option Bytes Programming functions #####
 ===============================================================================

    [..] The FLASH_Option Bytes Programming_functions, includes the following functions:
       (+) FLASH_Status FLASH_EraseOptionBytes(void);
       (+) FLASH_Status FLASH_EnableWriteProtection(uint32_t FLASH_Pages);
       (+) FLASH_Status FLASH_ReadOutProtection(FunctionalState NewState);
       (+) uint32_t FLASH_GetWriteProtectionOptionByte(void);
       (+) FlagStatus FLASH_GetReadOutProtectionStatus(void);

@endverbatim
  * @{
  */
  
/**
  * @brief  Erases the FLASH option bytes.
  * @note   This functions erases all option bytes except the Read protection (RDP). 
  * @param  None
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_EraseOptionBytes(void)
{
  uint16_t rdptmp = RDP_Key;

  FLASH_Status status = FLASH_COMPLETE;

  /* Get the actual read protection Option Byte value */ 
  if(FLASH_GetReadOutProtectionStatus() != RESET)
  {
    rdptmp = 0x00;  
  }

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(EraseTimeout);
  if(status == FLASH_COMPLETE)
  {
    /* Authorize the small information block programming */
    FLASH->OPTKEYR = FLASH_KEY1;
    FLASH->OPTKEYR = FLASH_KEY2;
    
    /* if the previous operation is completed, proceed to erase the option bytes */
    FLASH->CR |= CR_OPTER_Set;
    FLASH->CR |= CR_STRT_Set;
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(EraseTimeout);
    
    if(status == FLASH_COMPLETE)
    {
      /* if the erase operation is completed, disable the OPTER Bit */
      FLASH->CR &= CR_OPTER_Reset;
       
      /* Enable the Option Bytes Programming operation */
      FLASH->CR |= CR_OPTPG_Set;
      /* Restore the last read protection Option Byte value */
      OB->RDP = (uint16_t)rdptmp; 
      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastOperation(ProgramTimeout);
 
      if(status != FLASH_TIMEOUT)
      {
        /* if the program operation is completed, disable the OPTPG Bit */
        FLASH->CR &= CR_OPTPG_Reset;
      }
    }
    else
    {
      if (status != FLASH_TIMEOUT)
      {
        /* Disable the OPTPG Bit */
        FLASH->CR &= CR_OPTPG_Reset;
      }
    }  
  }
  /* Return the erase status */
  return status;
}

/**
  * @brief  Write protects the desired pages
  * @param  FLASH_Pages: specifies the address of the pages to be write protected.
  *   This parameter can be:
  *     @arg For @b STM32W_Low-density_devices: value between FLASH_WRProt_Pages0to3 and FLASH_WRProt_Pages60to63.  
  *     @arg For @b STM32W_Medium-density_devices: value between FLASH_WRProt_Pages0to3
  *       and FLASH_WRProt_Pages124to127.
  *     @arg For @b STM32W_High-density_devices: value between FLASH_WRProt_Pages0to1 and
  *       FLASH_WRProt_Pages126to127.
  *     @arg For @b STM32W_Connectivity_line_devices: value between FLASH_WRProt_Pages0to1 and
  *       FLASH_WRProt_Pages94to95.    
  *     @arg FLASH_WRProt_AllPages
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_EnableWriteProtection(uint32_t FLASH_Pages)
{
  uint16_t WRP0_Data = 0xFFFF, WRP1_Data = 0xFFFF, WRP2_Data = 0xFFFF, WRP3_Data = 0xFFFF;
  
  FLASH_Status status = FLASH_COMPLETE;
  
  /* Check the parameters */
  assert_param(IS_FLASH_WRPROT_PAGE(FLASH_Pages));
  
  FLASH_Pages = (uint32_t)(~FLASH_Pages);
  WRP0_Data = (uint16_t)(FLASH_Pages & WRP0_Mask);
  WRP1_Data = (uint16_t)((FLASH_Pages & WRP1_Mask) >> 8);
  WRP2_Data = (uint16_t)((FLASH_Pages & WRP2_Mask) >> 16);
  WRP3_Data = (uint16_t)((FLASH_Pages & WRP3_Mask) >> 24);
  
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(ProgramTimeout);
  
  if(status == FLASH_COMPLETE)
  {
    /* Authorizes the small information block programming */
    FLASH->OPTKEYR = FLASH_KEY1;
    FLASH->OPTKEYR = FLASH_KEY2;
    FLASH->CR |= CR_OPTPG_Set;
    if(WRP0_Data != 0xFF)
    {
      OB->WRP0 = WRP0_Data;
      
      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastOperation(ProgramTimeout);
    }
    if((status == FLASH_COMPLETE) && (WRP1_Data != 0xFF))
    {
      OB->WRP1 = WRP1_Data;
      
      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastOperation(ProgramTimeout);
    }
    if((status == FLASH_COMPLETE) && (WRP2_Data != 0xFF))
    {
      OB->WRP2 = WRP2_Data;
      
      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastOperation(ProgramTimeout);
    }
    
    if((status == FLASH_COMPLETE)&& (WRP3_Data != 0xFF))
    {
      OB->WRP3 = WRP3_Data;
     
      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastOperation(ProgramTimeout);
    }
          
    if(status != FLASH_TIMEOUT)
    {
      /* if the program operation is completed, disable the OPTPG Bit */
      FLASH->CR &= CR_OPTPG_Reset;
    }
  } 
  /* Return the write protection operation Status */
  return status;       
}

/**
  * @brief  Enables or disables the read out protection.
  * @param  NewState : new state of the ReadOut Protection.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_ReadOutProtection(FunctionalState NewState)
{
  FLASH_Status status = FLASH_COMPLETE;
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  status = FLASH_WaitForLastOperation(EraseTimeout);
  if(status == FLASH_COMPLETE)
  {
    /* Authorizes the small information block programming */
    FLASH->OPTKEYR = FLASH_KEY1;
    FLASH->OPTKEYR = FLASH_KEY2;
    FLASH->CR |= CR_OPTER_Set;
    FLASH->CR |= CR_STRT_Set;
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(EraseTimeout);
    if(status == FLASH_COMPLETE)
    {
      /* if the erase operation is completed, disable the OPTER Bit */
      FLASH->CR &= CR_OPTER_Reset;
      /* Enable the Option Bytes Programming operation */
      FLASH->CR |= CR_OPTPG_Set; 
      if(NewState != DISABLE)
      {
        OB->RDP = 0x00;
      }
      else
      {
        OB->RDP = RDP_Key;  
      }
      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastOperation(EraseTimeout); 
    
      if(status != FLASH_TIMEOUT)
      {
        /* if the program operation is completed, disable the OPTPG Bit */
        FLASH->CR &= CR_OPTPG_Reset;
      }
    }
    else 
    {
      if(status != FLASH_TIMEOUT)
      {
        /* Disable the OPTER Bit */
        FLASH->CR &= CR_OPTER_Reset;
      }
    }
  }
  /* Return the protection operation Status */
  return status;       
}

/**
  * @brief  Returns the FLASH Write Protection Option Bytes Register value.
  * @param  None
  * @retval The FLASH Write Protection  Option Bytes Register value
  */
uint32_t FLASH_GetWriteProtectionOptionByte(void)
{
  /* Return the Flash write protection Register value */
  return (uint32_t)(FLASH->WRPR);
}

/**
  * @brief  Checks whether the FLASH Read Out Protection Status is set or not.
  * @param  None
  * @retval FLASH ReadOut Protection Status(SET or RESET)
  */
FlagStatus FLASH_GetReadOutProtectionStatus(void)
{
  FlagStatus readoutstatus = RESET;
  if ((FLASH->OBR & RDPRT_Mask) != (uint32_t)RESET)
  {
    readoutstatus = SET;
  }
  else
  {
    readoutstatus = RESET;
  }
  return readoutstatus;
}

/**
  * @}
  */

/** @defgroup FLASH_Group4 Interrupts and flags management functions
 *  @brief   Interrupts and flags management functions
 *
@verbatim   
 ===============================================================================
             ##### Interrupts and flags management functions #####
 ===============================================================================  

@endverbatim
  * @{
  */
/**
  * @brief  Enables or disables the specified FLASH interrupts.
  * @param  FLASH_IT: specifies the FLASH interrupt sources to be enabled or disabled.
  *   This parameter can be any combination of the following values:
  *     @arg FLASH_IT_ERROR: FLASH Error Interrupt
  *     @arg FLASH_IT_EOP: FLASH end of operation Interrupt
  * @param  NewState: new state of the specified Flash interrupts.
  *   This parameter can be: ENABLE or DISABLE.      
  * @retval None 
  */
void FLASH_ITConfig(uint32_t FLASH_IT, FunctionalState NewState)
{

  /* Check the parameters */
  assert_param(IS_FLASH_IT(FLASH_IT)); 
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if(NewState != DISABLE)
  {
    /* Enable the interrupt sources */
    FLASH->CR |= FLASH_IT;
  }
  else
  {
    /* Disable the interrupt sources */
    FLASH->CR &= ~(uint32_t)FLASH_IT;
  }
}

/**
  * @brief  Checks whether the specified FLASH flag is set or not.
  * @param  FLASH_FLAG: specifies the FLASH flag to check.
  *   This parameter can be one of the following values:
  *     @arg FLASH_FLAG_BSY: FLASH Busy flag           
  *     @arg FLASH_FLAG_PGERR: FLASH Program error flag       
  *     @arg FLASH_FLAG_WRPRTERR: FLASH Write protected error flag      
  *     @arg FLASH_FLAG_EOP: FLASH End of Operation flag           
  *     @arg FLASH_FLAG_OPTERR:  FLASH Option Byte error flag     
  * @retval The new state of FLASH_FLAG (SET or RESET).
  */
FlagStatus FLASH_GetFlagStatus(uint32_t FLASH_FLAG)
{
  FlagStatus bitstatus = RESET;
  
  /* Check the parameters */
  assert_param(IS_FLASH_GET_FLAG(FLASH_FLAG)) ;
  if(FLASH_FLAG == FLASH_FLAG_OPTERR) 
  {
    if((FLASH->OBR & FLASH_FLAG_OPTERR) != (uint32_t)RESET)
    {
      bitstatus = SET;
    }
    else
    {
      bitstatus = RESET;
    }
  }
  else
  {
   if((FLASH->SR & FLASH_FLAG) != (uint32_t)RESET)
    {
      bitstatus = SET;
    }
    else
    {
      bitstatus = RESET;
    }
  }
  /* Return the new state of FLASH_FLAG (SET or RESET) */
  return bitstatus;
}

/**
  * @brief  Clears the FLASH's pending flags.
  * @param  FLASH_FLAG: specifies the FLASH flags to clear.
  *   This parameter can be any combination of the following values:         
  *     @arg FLASH_FLAG_PGERR: FLASH Program error flag       
  *     @arg FLASH_FLAG_WRPRTERR: FLASH Write protected error flag      
  *     @arg FLASH_FLAG_EOP: FLASH End of Operation flag           
  * @retval None
  */
void FLASH_ClearFlag(uint32_t FLASH_FLAG)
{
  /* Check the parameters */
  assert_param(IS_FLASH_CLEAR_FLAG(FLASH_FLAG)) ;
  
  /* Clear the flags */
  FLASH->SR = FLASH_FLAG;
}

/**
  * @brief  Returns the FLASH Status.
  * @param  None
  * @retval FLASH Status: The returned value can be: FLASH_BUSY, FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP or FLASH_COMPLETE
  */
FLASH_Status FLASH_GetStatus(void)
{
  FLASH_Status flashstatus = FLASH_COMPLETE;
  
  if((FLASH->SR & FLASH_FLAG_BSY) == FLASH_FLAG_BSY) 
  {
    flashstatus = FLASH_BUSY;
  }
  else 
  {  
    if((FLASH->SR & FLASH_FLAG_PGERR) != 0)
    { 
      flashstatus = FLASH_ERROR_PG;
    }
    else 
    {
      if((FLASH->SR & FLASH_FLAG_WRPRTERR) != 0 )
      {
        flashstatus = FLASH_ERROR_WRP;
      }
      else
      {
        flashstatus = FLASH_COMPLETE;
      }
    }
  }
  /* Return the Flash Status */
  return flashstatus;
}

/**
  * @brief  Waits for a Flash operation to complete or a TIMEOUT to occur.
  * @param  Timeout: FLASH programming Timeout
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_WaitForLastOperation(uint32_t Timeout)
{ 
  FLASH_Status status = FLASH_COMPLETE;
   
  /* Check for the Flash Status */
  status = FLASH_GetStatus();
  /* Wait for a Flash operation to complete or a TIMEOUT to occur */
  while((status == FLASH_BUSY) && (Timeout != 0x00))
  {
    status = FLASH_GetStatus();
    Timeout--;
  }
  if(Timeout == 0x00 )
  {
    status = FLASH_TIMEOUT;
  }
  /* Return the operation status */
  return status;
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
