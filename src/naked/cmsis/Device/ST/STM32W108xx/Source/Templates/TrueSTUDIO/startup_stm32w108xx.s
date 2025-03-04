/**
  ******************************************************************************
  * @file      : startup_stm32w108xx.s
  * Author     : MCD Application Team
  * Version    : V0.0.1RC1
  * Date       : 18-April-2012
  * @brief     : stm32w108xx Devices vector table for Atollic TrueSTUDIO toolchain.
  *              This module performs:
  *                - Set the initial SP
  *                - Set the initial PC == Reset_Handler,
  *                - Set the vector table entries with the exceptions ISR address
  *                - Configure the clock system
  *                - Branches to main in the C library (which eventually
  *                  calls main()).
  *              After Reset the cortex-m3 processor is in Thread mode,
  *              priority is Privileged, and the Stack is set to Main.
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

  .syntax unified
  .cpu cortex-m3
  .fpu softvfp
  .thumb

.global g_pfnVectors
.global Default_Handler

/* start address for the initialization values of the .data section.
defined in linker script */
.word _sidata
/* start address for the .data section. defined in linker script */
.word _sdata
/* end address for the .data section. defined in linker script */
.word _edata
/* start address for the .bss section. defined in linker script */
.word _sbss
/* end address for the .bss section. defined in linker script */
.word _ebss

.equ  BootRAM, 0xF108F85F
/**
 * @brief  This is the code that gets called when the processor first
 *          starts execution following a reset event. Only the absolutely
 *          necessary set is performed, after which the application
 *          supplied main() routine is called.
 * @param  None
 * @retval : None
*/

  .section .text.Reset_Handler
  .weak Reset_Handler
  .type Reset_Handler, %function
Reset_Handler:
  ldr   r0, =_estack
  mov   sp, r0          /* set stack pointer */

/* Copy the data segment initializers from flash to SRAM */
  movs r1, #0
  b LoopCopyDataInit

CopyDataInit:
  ldr r3, =_sidata
  ldr r3, [r3, r1]
  str r3, [r0, r1]
  adds r1, r1, #4

LoopCopyDataInit:
  ldr r0, =_sdata
  ldr r3, =_edata
  adds r2, r0, r1
  cmp r2, r3
  bcc CopyDataInit
  ldr r2, =_sbss
  b LoopFillZerobss
/* Zero fill the bss segment. */
FillZerobss:
  movs r3, #0
  str  r3, [r2]
  adds r2, r2, #4


LoopFillZerobss:
  ldr r3, = _ebss
  cmp r2, r3
  bcc FillZerobss

/* Call the clock system intitialization function.*/
    bl  SystemInit
/* Call static constructors */
    bl __libc_init_array
/* Call the application's entry point.*/
  bl main
  
LoopForever:
    b LoopForever


.size Reset_Handler, .-Reset_Handler

/**
 * @brief  This is the code that gets called when the processor receives an
 *         unexpected interrupt.  This simply enters an infinite loop, preserving
 *         the system state for examination by a debugger.
 *
 * @param  None
 * @retval : None
*/
    .section .text.Default_Handler,"ax",%progbits
Default_Handler:
Infinite_Loop:
  b Infinite_Loop
  .size Default_Handler, .-Default_Handler
/******************************************************************************
*
* The minimal vector table for a Cortex M0.  Note that the proper constructs
* must be placed on this to ensure that it ends up at physical address
* 0x0000.0000.
*
******************************************************************************/
   .section .isr_vector,"a",%progbits
  .type g_pfnVectors, %object
  .size g_pfnVectors, .-g_pfnVectors


g_pfnVectors:
  .word _estack
  .word Reset_Handler
  .word NMI_Handler
  .word HardFault_Handler
  .word	MemManage_Handler
  .word	BusFault_Handler
  .word	UsageFault_Handler
  .word 0
  .word 0
  .word 0
  .word 0
  .word SVC_Handler
  .word DebugMon_Handler
  .word 0
  .word PendSV_Handler
  .word SysTick_Handler
  .word TIM1_IRQHandler
  .word TIM2_IRQHandler
  .word MNG_IRQHandler
  .word BASEBAND_IRQHandler
  .word SLPTIM_IRQHandler
  .word SC1_IRQHandler
  .word SC2_IRQHandler
  .word SECURITY_IRQHandler
  .word MAC_TIM_IRQHandler
  .word MAC_TR_IRQHandler
  .word MAC_RE_IRQHandler
  .word ADC_IRQHandler
  .word EXTIA_IRQHandler 
  .word EXTIB_IRQHandler
  .word EXTIC_IRQHandler
  .word EXTID_IRQHandler
  .word DEBUG_IRQHandler
  .word BootRAM          /* @0x108. This is for boot in RAM mode for 
                            stm32w108xx devices. */

/*******************************************************************************
*
* Provide weak aliases for each Exception handler to the Default_Handler.
* As they are weak aliases, any function with the same name will override
* this definition.
*
*******************************************************************************/

  .weak NMI_Handler
  .thumb_set NMI_Handler,Default_Handler

  .weak HardFault_Handler
  .thumb_set HardFault_Handler,Default_Handler

  .weak	MemManage_Handler
  .thumb_set MemManage_Handler,Default_Handler

  .weak	BusFault_Handler
  .thumb_set BusFault_Handler,Default_Handler

  .weak	UsageFault_Handler
  .thumb_set UsageFault_Handler,Default_Handler

  .weak SVC_Handler
  .thumb_set SVC_Handler,Default_Handler

  .weak DebugMon_Handler
  .thumb_set DebugMon_Handler,Default_Handler

  .weak PendSV_Handler
  .thumb_set PendSV_Handler,Default_Handler

  .weak SysTick_Handler
  .thumb_set SysTick_Handler,Default_Handler

  .weak TIM1_IRQHandler
  .thumb_set TIM1_IRQHandler,Default_Handler

  .weak TIM2_IRQHandler
  .thumb_set TIM2_IRQHandler,Default_Handler
  
  .weak MNG_IRQHandler
  .thumb_set MNG_IRQHandler,Default_Handler
  
  .weak BASEBAND_IRQHandler
  .thumb_set BASEBAND_IRQHandler,Default_Handler
  
  .weak SLPTIM_IRQHandler
  .thumb_set SLPTIM_IRQHandler,Default_Handler
  
  .weak SC1_IRQHandler
  .thumb_set SC1_IRQHandler,Default_Handler
  
  .weak SC2_IRQHandler
  .thumb_set SC2_IRQHandler,Default_Handler
  
  .weak SECURITY_IRQHandler
  .thumb_set SECURITY_IRQHandler,Default_Handler
  
  .weak MAC_TIM_IRQHandler
  .thumb_set MAC_TIM_IRQHandler,Default_Handler
  
  .weak MAC_TR_IRQHandler
  .thumb_set MAC_TR_IRQHandler,Default_Handler
  
  .weak MAC_RE_IRQHandler
  .thumb_set MAC_RE_IRQHandler,Default_Handler
  
  .weak ADC_IRQHandler
  .thumb_set ADC_IRQHandler,Default_Handler
  
  .weak EXTIA_IRQHandler
  .thumb_set EXTIA_IRQHandler,Default_Handler
   
  .weak EXTIB_IRQHandler
  .thumb_set EXTIB_IRQHandler,Default_Handler
  
  .weak EXTIC_IRQHandler
  .thumb_set EXTIC_IRQHandler,Default_Handler
  
  .weak EXTID_IRQHandler
  .thumb_set EXTID_IRQHandler,Default_Handler
  
  .weak DEBUG_IRQHandler
  .thumb_set DEBUG_IRQHandler,Default_Handler

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

