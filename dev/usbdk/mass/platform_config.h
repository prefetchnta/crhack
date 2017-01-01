/**
  ******************************************************************************
  * @file    platform_config.h [port]
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   Evaluation board specific configuration file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
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
#ifndef __PLATFORM_CONFIG_H
#define __PLATFORM_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#if defined(STM32L1XX_MD) || defined(STM32L1XX_HD)|| defined(STM32L1XX_MD_PLUS)
 #include "stm32l1xx.h"
#elif defined (STM32F10X_MD) || defined (STM32F10X_HD) || defined (STM32F10X_XL)
 #include "stm32f10x.h"
#elif defined (STM32F37X)
 #include "stm32f37x.h"
#elif defined (STM32F30X)
 #include "stm32f30x.h"
#endif

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Uncomment the line corresponding to the STMicroelectronics evaluation board
   used to run the example */

/*Unique Devices IDs register set*/

#if defined(STM32L1XX_MD) || defined(STM32L1XX_HD) || defined(STM32L1XX_MD_PLUS)

#define         ID1          (0x1FF80050)
#define         ID2          (0x1FF80054)
#define         ID3          (0x1FF80064)

#elif defined (STM32F37X) || defined(STM32F30X)

#define         ID1          (0x1FFFF7AC)
#define         ID2          (0x1FFFF7B0)
#define         ID3          (0x1FFFF7B4)

#else /*STM32F1x*/

#define         ID1          (0x1FFFF7E8)
#define         ID2          (0x1FFFF7EC)
#define         ID3          (0x1FFFF7F0)

#endif

/* Define the STM32F10x hardware depending on the used evaluation board */
#if defined (USE_STM32L15XX) && !defined (USB_USE_EXTERNAL_PULLUP)
 /*
   For STM32L15xx devices it is possible to use the internal USB pullup
   controlled by register SYSCFG_PMC (refer to RM0038 reference manual for
   more details).
   It is also possible to use external pullup (and disable the internal pullup)
   by setting the define USB_USE_EXTERNAL_PULLUP in file platform_config.h
   and configuring the right pin to be used for the external pull up configuration.
   To have more details on how to use an external pull up, please refer to
   STM3210E-EVAL evaluation board manuals.
   */
 /* Uncomment the following define to use an external pull up instead of the
    integrated STM32L15xx internal pull up. In this case make sure to set up
    correctly the external required hardware and the GPIO defines below.*/
  #define STM32_USB_CONNECT     SYSCFG_USBPuCmd(ENABLE)
  #define STM32_USB_DISCONNECT  SYSCFG_USBPuCmd(DISABLE)
#else
  #define USB_DISCONNECT        GPIOA
  #define USB_DISCONNECT_PIN    GPIO_Pin_8  /* 高电平 USB 连上 */
  #define STM32_USB_CONNECT     GPIO_SetBits(USB_DISCONNECT, USB_DISCONNECT_PIN)
  #define STM32_USB_DISCONNECT  GPIO_ResetBits(USB_DISCONNECT, USB_DISCONNECT_PIN)
#endif /* USE_STM32L15XX && USB_USE_EXTERNAL_PULLUP */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __PLATFORM_CONFIG_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
