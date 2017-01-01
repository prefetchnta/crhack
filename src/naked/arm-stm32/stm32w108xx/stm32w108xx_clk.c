/**
  ******************************************************************************
  * @file    stm32w108xx_clk.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    30-November-2012
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the clock peripheral:
  *           + Internal/external clocks, 
  *           + Modes management
  *
  *  @verbatim
  * 
 ===============================================================================
                        ##### CLK specific features #####
 ===============================================================================
    [..] After reset the device is running from OSCHF (12 MHz) 
                    
    [..] Once the device started from reset, the user application has to:
         (#) Configure the clock source to be used to drive the System clock
         (#) Configure the System clock frequency: 24Mhz/12Mhz  
         (#) Configure the Flash clock frequency: 24Mhz/12Mhz/6Mhz


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
#include "stm32w108xx_clk.h"

/** @addtogroup STM32W108xx_StdPeriph_Driver
  * @{
  */

/** @defgroup CLK 
  * @brief CLK driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define SLOWRC_PERIOD_SETTLE_TIME 4250
#define SLOWRC_PERIOD_SAMPLES 8
#define CLK1K_NUMERATOR 384000000

#define FASTRC_PERIOD_SETTLE_TIME 128

/* CLK_HSECR2 register Mask */
#define CLK_HSECR2_Mask            ((uint32_t)0x00000003)

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup CLK_Private_Functions 
  * @{
  */
  
/** @defgroup CLK_Group1 Internal and external clocks
  *  @brief   Internal and external clocks configuration functions 
  *
@verbatim
 ===============================================================================
 ##### Internal-external clocks configuration functions #####
 ===============================================================================
    [..] This section provides functions allowing to configure the internal/external clocks,
         
         (#) HSI (high-frequency RC oscillator (OSCHF)), is used as the default system clock 
             source when power is applied to the core domain. The nominal frequency coming 
             out of reset is 12 MHz.
         (#) HSE (high-frequency crystal oscillator), 24 MHz crystal oscillator 

@endverbatim
  * @{
  */

/**
  * @brief  Resets the CLOCK configuration to the default reset state.
  * @note   The default reset state of the clock configuration is given below:
  *         HSI ON and used as system clock source 
  * @param  None
  * @retval None
  */
void CLK_DeInit(void)
{
  CLK->SLEEPCR = 0x00000002;
  CLK->LSI10KCR = 0x00000000;
  CLK->LSI1KCR = 0x00005000;
  CLK->HSECR1 = 0x0000000F;
  CLK->HSICR = 0x00000017;
  CLK->PERIODCR = 0x00000000;
  CLK->DITHERCR = 0x00000000;
  CLK->HSECR2 = 0x00000000;
  CLK->CPUCR = 0x00000000; 
}

/**
  * @brief  Calibrate the low speed internal clock (LSI) to be close to 10KHZ in
  *         order to generate 1KHZ clock.
  * @param  None
  * @retval None
  */
void CLK_InternalCalibrateLSI(void)
{
  uint8_t i = 0;
  uint32_t average = 0;
  int16_t delta = 0;
  uint32_t period = 0;
  __IO uint32_t StartUpCounter = 0;
  __IO uint32_t LSI10KCR_RESET = 0x0, LSI1KCR_RESET = 0x5000;
  
  /* The slowest frequency for the 10kHz RC source is 8kHz (125us).  The PERIODSR
  register updates every 16 cycles, so to be safe 17 cycles = 2125us.  But,
  we need twice this maximum time because the period measurement runs
  asynchronously, and the value of LSI10KCR is changed immediately before
  the delay.
  SLOWRC_PERIOD_SETTLE_TIME 4250
  The CLK_PERIOD register measures the number of 12MHz clock cycles that
  occur in 16 cycles of the SlowRC clock.  This is meant to smooth out the the
  noise inherently present in the analog RC source.  While these 16 cycles
  smooths out most noise, there is still some jitter in the bottom bits of
  PERIODSR.  To further smooth out the noise, we take several readings of
  PERIODSR and average them out.  Testing has shown that the bottom 3 and 4
  bits of PERIODSR contain most of the jitter.  Averaging 8 samples will
  smooth out 3 bits of jitter and provide a realiable and stable reading useful
  in the calculations, while taking much less time than 16 or 32 samples.
  SLOWRC_PERIOD_SAMPLES 8
  The register LSI1KCR is a fractional divider that divides the 10kHz analog
  source with the goal of generating a 1024Hz, clk1k output.
  10000Hz / LSI1KCR = 1024Hz.
  Since the PERIODSR register measures the number of 12MHz cycles in 16
  cycles of the RC:
  16 * 12000000
  ------------- = ~10kHz
  PERIODSR
  and
  ~10kHz / 1024 = X
  where X is the fractional number that belongs in LSI1KCR.  Since the
  integer portion of LSI1KCR is bits 15:11 and the fractional is 10:0,
  multiplying X by 2048 (bit shift left by 11) generates the proper LSI1KCR
  register value.
  
  Putting this all together:
  16 * 12000000 * 2048     384000000
  --------------------  = ------------  =  LSI1KCR
  PERIODSR * 1024       PERIODSR
  
  CLK1K_NUMERATOR 384000000 */
  
  /* ---- STEP 1: coarsely tune SlowRC in analog section to ~10kHz ---- */
  /* To operate properly across the full temperature and voltage range,
  the RC source in the analog section needs to be first coarsely tuned
  to 10kHz.  The LSI10KCR register, which is 2's compliment, provides 16
  steps at ~400Hz per step yielding approximate frequences of 8kHz at 7
  and 15kHz at -8. */
  /* Start with our reset values for TUNE and CAL */
  CLK->PERIODCR = 0; /* measure SlowRC */
  CLK->LSI10KCR = LSI10KCR_RESET;
  CLK->LSI1KCR = LSI1KCR_RESET;
  
  /* wait for the PERIODSR register to properly update */
  do
  {
    StartUpCounter++;  
  } while(StartUpCounter != SLOWRC_PERIOD_SETTLE_TIME);
  
  /* Measure the current PERIODSR to obtain a baseline
  For 10kHz, the ideal PERIODSR value is 19200. Calculate the PERIOD delta.
  It's possible for a chip's 10kHz source RC to be too far out of range
  for the LSI10KCR to bring it back to 10kHz.  Therefore, we have to
  ensure that our delta correction does not exceed the tune range so
  tune has to be capped to the end of the vailable range so it does not
  wrap.  Even if we cannot achieve 10kHz, the 1kHz calibration can still
  properly correct to 1kHz.
  Each LSI10KCR step yields a PERIODSR delta of *approximately* 800.
  Calculate how many steps we are off.  While dividing by 800 may seem
  like an ugly calculation, the precision of the result is worth the small
  bit of code and time needed to do a divide. */
  period = CLK->PERIODSR;
  
  /* Round to the nearest integer */
  delta = (19200+400) - period;
  delta /= 800;
  
  /* LSI10KCR is a 4 bit signed number.  cap the delta to 7/-8 */
  if(delta > 7) {
    delta = 7;
  }
  if(delta < -8) {
    delta = -8;
  }
  CLK->LSI10KCR = delta;
  
  /* Wait for PERIOD to update before taking another sample */
  StartUpCounter = 0;
  do
  {
    StartUpCounter++;  
  } while(StartUpCounter != SLOWRC_PERIOD_SETTLE_TIME);
  
  /* The analog section should now be producing an output of ~10kHz */
  
  /* ---- STEP 2: fine tune the SlowRC to 1024Hz ---- */
  /* Our goal is to generate a 1024Hz source.  The register LSI1KCR is a
  fractional divider that divides the 10kHz analog source and generates
  the clk1k output.  At reset, the default value is 0x5000 which yields a
  division of 10.000.  By averaging several samples of CLK_PERIOD, we
  can then calculate the proper divisor need for LSI1KCR to make 1024Hz. */
  for(i=0;i<SLOWRC_PERIOD_SAMPLES;i++) {
    StartUpCounter = 0;
    do
    {
      StartUpCounter++;  
    } while(StartUpCounter != SLOWRC_PERIOD_SETTLE_TIME);
    average += CLK->PERIODSR;
  }
  
  /* Calculate the average, with proper rounding */
  average = (average+(SLOWRC_PERIOD_SAMPLES/2))/SLOWRC_PERIOD_SAMPLES;
  
  /* Using an average period sample, calculate the clk1k divisor */
  CLK->LSI1KCR = (uint16_t)(CLK1K_NUMERATOR/average);
  
  /* The SlowRC timer is now producing a 1024Hz tick (+/-2Hz). */ 
}

/**
  * @brief  Calibrate the high speed internal clock (HSI) to be close to 12MHZ.
  * @note   To calibrate the HSI, the high speed external clock (HSE) must be the 
  *         system clock.
  * @param  None
  * @retval None
  */
void CLK_InternalCalibrateHSI(void)
{
  __IO uint32_t StartUpCounter = 0, CLK_PERIOD = 0;
  __IO int32_t newTune = -16;
  /* ---- coarsely tune FastRC in analog section to ~12MHz ---- */
  /* The slowest frequency for the FastRC source is 4MHz (250ns).  The PERIODSR
  register updates every 256 cycles, so to be safe 257 cycles = 64us.  But,
  we need twice this maximum time because the period measurement runs
  asynchronously, and the value of HSICR1 is changed immediately before
  the delay.
  The CLK_PERIODSR register measures the number of 12MHz cycles in 256
  cycles of OSCHF:
  
  256 * 12000000
  ------------- = ~12MHz
  CLK_PERIOD 
  
  The RC source in the analog section needs to be coarsely tuned
  to 12MHz.  The HSICR1 register, which is 2's compliment, provides 32
  steps at ~0.5MHz per step yielding approximate frequences of 4MHz at 15
  and 20MHz at -16. */

  CLK->PERIODCR = 1; /* Measure FastRC */

  /* Start at the fastest possible frequency */
  CLK->HSICR = newTune;

  /* Wait for the PERIOD register to properly update */
  do
  {
    StartUpCounter++;  
  } while(StartUpCounter != FASTRC_PERIOD_SETTLE_TIME);

  /* For 12MHz, the ideal CLK_PERIOD is 256.  Tune the frequency down until
  the period is <= 256, which says the frequency is as close to 12MHz as
  possible (without going over 12MHz)
  Start at the fastest possible frequency (-16) and increase to the slowest
  possible (15).  When CLK_PERIOD is <=256 or we run out of tune values,
  we're done. */
  for(;newTune<16;newTune++) 
  {
    StartUpCounter = 0;
    /* Decrease frequency by one step (by increasing tune value) */
    CLK->HSICR = newTune;

    /* Wait for the PERIOD register to properly update */
    do
    {
      StartUpCounter++;
    } while(StartUpCounter != FASTRC_PERIOD_SETTLE_TIME);

    /* Kickout if we're tuned */
    CLK_PERIOD = CLK->PERIODSR;
    if(CLK_PERIOD >= 256) {
      break;
    }
  }
  /* The analog section should now be producing an output of 11.5MHz - 12.0MHz */
}

/**
  * @brief  Configures the clock mode to use:
  * @param MODE: specifies the frequency mode to use.
  *   This parameter can be one of the following values:
  *     @arg MODE0: Normal CPU, SCLK =12MHZ, PCLK=6MHZ, Flash Program/Erase Inactive =6Mhz,
  *                 FlashProgram/Erase Active = 12Mhz. 
  *     @arg MODE1: Fast CPU, SCLK =12MHZ, PCLK=6MHZ, Flash Program/Erase Inactive =12Mhz,
  *                 FlashProgram/Erase Active = 12Mhz. 
  *     @arg MODE2: Normal CPU, SCLK =24MHZ, PCLK=12MHZ, Flash Program/Erase Inactive =12Mhz,
  *                 FlashProgram/Erase Active = 12Mhz. 
  *     @arg MODE3: Fast CPU, SCLK =24MHZ, PCLK=12MHZ, Flash Program/Erase Inactive =24Mhz,
  *                 FlashProgram/Erase Active = 12Mhz. 
  * @retval None
  */
void CLK_Config(uint8_t MODE)
{
   __IO uint32_t StartUpCounter = 0;

  /* Check the parameters */
  assert_param(IS_CLK_MODE(MODE));
 
  switch (MODE)
  {
    case CLK_MODE0:  /* HSI used as system clock : Normal CPU*/
      CLK->HSECR2 &= ~CLK_HSECR2_SW1; 
      CLK->CPUCR &= ~CLK_CPUCR_SW2;
      break;

    case CLK_MODE1:  /* HSI used as system clock : Fast CPU */
      CLK->HSECR2 &= ~CLK_HSECR2_SW1; 
      CLK->CPUCR |=  CLK_CPUCR_SW2;       
      break;

    case CLK_MODE2:  /* HSE used as system clock : Normal CPU*/
      /* Enable HSE */
      CLK->HSECR2 |= CLK_HSECR2_EN;
 
    /* Wait till HSE is ready and if Time out is reached exit */
      do
      {
        StartUpCounter++;
      } while(StartUpCounter != HSE_STARTUP_TIMEOUT);

      CLK->HSECR2 |= CLK_HSECR2_SW1;
      CLK->CPUCR &= ~CLK_CPUCR_SW2;
      break;
      
    case CLK_MODE3:  /* HSE used as system clock : Fast CPU*/
     /* Enable HSE */
      CLK->HSECR2 |= CLK_HSECR2_EN;
 
    /* Wait till HSE is ready and if Time out is reached exit */
      do
      {
        StartUpCounter++;
      } while(StartUpCounter != HSE_STARTUP_TIMEOUT);

      CLK->HSECR2 |= CLK_HSECR2_SW1;
      CLK->CPUCR |= CLK_CPUCR_SW2;
      break;
    default: /* HSI used as system clock */
      CLK->HSECR2 &= ~CLK_HSECR2_SW1;
      CLK->CPUCR &= ~CLK_CPUCR_SW2;
      break;
  }
}

/**
  * @brief  Enables or disables the External High Speed oscillator (HSE).
  * @note   After enabling HSE the user should wait for HSE_STARTUP_TIMEOUT 
  * @note   to be sure that the clok is stabilized.     
  * @param  NewState: new state of the HSE.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void CLK_HSECmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    CLK->HSECR2 |= CLK_HSECR2_EN; 
  }
  else
  {
    CLK->HSECR2 = 0x00;
  }
}

/**
  * @brief  Enables or disables the specified SLPTIM clock.
  * @param  CLK_SLPTIM: specifies the SLPTIM clock to be enabled or disabled.
  *   This parameter can be any combination of the following values:
  *     @arg SLPTIM_CLK_32KH: 32kHz external XTAL
  *     @arg SLPTIM_CLK_10KH: 10kHz internal RC (during deep sleep)
  * @param  NewState: new state of the SLPTIM clock.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void CLK_SLPTIMClockConfig(uint32_t CLK_SLPTIM, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_SLPTIM_GET_CLK(CLK_SLPTIM));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
      if (CLK_SLPTIM == SLPTIM_CLK_32KH)
      {
        CLK->SLEEPCR &= 0x0;
      }
      /* Enable the clock */
      CLK->SLEEPCR |= (uint32_t)CLK_SLPTIM;
    }
    else
    {
      /* Disable the clock */
      CLK->SLEEPCR &= (uint32_t)~CLK_SLPTIM;
    }
}

/**
  * @brief  Calibration of CLK1K clock.
  * @param  CALINT: specifies the divider value integer portion.
  *   This parameter can be a value between 0x0 and 0x1F.
  * @param  CALFRAC: specifies the divider value fractional portion.
  *   This parameter can be a value between 0x0 and 0x7FF.
  * @retval None.
  */
void CLK_1KClockCalibration(uint32_t CALINT, uint32_t CALFRAC)
{
  uint32_t tmpclk1k;
  /* Check the parameters */
  assert_param(IS_LSI1KCRINT(CALINT));
  assert_param(IS_LSI1KCRFRAC(CALFRAC));

  CLK->LSI1KCR = 0x00000000;

  /* set the divider value integer portion */
  tmpclk1k = (uint32_t)(CALINT <<11);

  /* set the divider value fractional portion */  
  tmpclk1k |= CALFRAC;

  CLK->LSI1KCR = tmpclk1k;  
}

/**
  * @brief  Set tune value for CLKRC clock.
  * @param  TUNE_VALUE: specifies the tune value for CLKRC clock.
  *   This parameter can be a value between 0x0 and 0xF.
  * @retval None.
  */
void CLK_RCTuneConfig(uint32_t TUNE_VALUE)
{
  /* Check the parameters */
  assert_param(IS_CLK_TUNE_VALUE(TUNE_VALUE));

  CLK->LSI10KCR = 0x00000000;

  /* set the tune value for CLKRC */  
  CLK->LSI10KCR = TUNE_VALUE;  
}

/**
  * @brief  Select the clock period to be measured.
  * @param  CLK_MEASURED: specifies the clock for which the period will be measured.
  *   This parameter can be :
  *     @arg MEASURE_CLKRC: Measure CLKRC.
  *     @arg MEASURE_OSCHF: Measure OSCHF.
  *     @arg MEASURE_TUNEFILT: Measure TUNE_FILTER_RESULT.
  * @retval None.
  */
void CLK_MeasurePeriod(uint32_t CLK_MEASURED)
{
  /* Check the parameters */
  assert_param(IS_CLK_MEASURE(CLK_MEASURED));

  CLK->PERIODCR = CLK_MEASURED;
}

/**
  * @brief  Returns the clock period measured depend on clock selected.
  *@note    measured period is equal to:
  *       16 x Clock period in clk12m cycles (CLKRC/TUNE_FILTER_RESULT modes) 
  *       256 x clock period in clk12m cycles (OSCHF mode)
  * @param  None.
  * @retval None.
  */
uint32_t CLK_GetMeasurePeriod(void)
{
  return CLK->PERIODSR;
}

/**
  * @brief  Returns the clock source used as system clock.
  * @param  None
  * @retval The clock source used as system clock. The returned value can
  *   be one of the following:
  *     - 0x00,0x01,0x10: HSI used as system clock
  *     - 0x03: HSE used as system clock
  */
uint32_t CLK_GetClocksFreq(void)
{
  uint32_t tmp = 0;
  uint32_t clockvalue = 0;
  /* Get SYSCLK source -------------------------------------------------------*/
  tmp = CLK->HSECR2 & CLK_HSECR2_Mask;

  switch (tmp)
  {
    case 0x00:
    case 0x02:  /* HSI used as system clock */
      clockvalue = (uint32_t)HSI_VALUE;
      break;
    case 0x03:  /* HSE used as system clock */
      clockvalue = (uint32_t)HSE_VALUE;
      break;
      
    default: /* HSI used as system clock */
      clockvalue = (uint32_t)HSI_VALUE;
      break;
   }
   return clockvalue;
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
