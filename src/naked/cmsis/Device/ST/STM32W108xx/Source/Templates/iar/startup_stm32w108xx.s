;/******************** (C) COPYRIGHT 2012 STMicroelectronics ********************
;* File Name          : startup_stm32w108xx.s
;* Author             : MCD Application Team
;* Version            : V1.0.1
;* Date               : 30-November-2012
;* Description        : STM32W108xx RF High Performance Devices vector  table for *
;*                      EWARM toolchain.
;*                      This module performs:
;*                      - Set the initial SP
;*                      - Set the initial PC == __iar_program_start,
;*                      - Set the vector table entries with the exceptions ISR 
;*                        address.
;*                      After Reset the Cortex-M3 processor is in Thread mode,
;*                      priority is Privileged, and the Stack is set to Main.
;********************************************************************************
;* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
;* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
;* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
;* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
;* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
;* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
;*******************************************************************************/
;
;
; The modules in this file are included in the libraries, and may be replaced
; by any user-defined modules that define the PUBLIC symbol _program_start or
; a user defined start symbol.
; To override the cstartup defined in the library, simply add your modified
; version to the workbench project.
;
; The vector table is normally located at address 0.
; When debugging in RAM, it can be located in RAM, aligned to at least 2^6.
; The name "__vector_table" has special meaning for C-SPY:
; it is where the SP start value is found, and the NVIC vector
; table register (VTOR) is initialized to this address if != 0.
;
; Cortex-M version
;

        MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)

        EXTERN  __iar_program_start
        EXTERN  SystemInit        
        PUBLIC  __vector_table

        DATA
__vector_table
        DCD     sfe(CSTACK)
        DCD     Reset_Handler             ; Reset Handler
        

        DCD     NMI_Handler               ; NMI Handler
        DCD     HardFault_Handler         ; Hard Fault Handler
        DCD     MemManage_Handler         ; MPU Fault Handler
        DCD     BusFault_Handler          ; Bus Fault Handler
        DCD     UsageFault_Handler        ; Usage Fault Handler
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     SVC_Handler               ; SVCall Handler
        DCD     DebugMon_Handler          ; Debug Monitor Handler
        DCD     0                         ; Reserved
        DCD     PendSV_Handler            ; PendSV Handler
        DCD     SysTick_Handler           ; SysTick Handler

         ; External Interrupts            
        DCD     TIM1_IRQHandler           ; Timer 1 Interrupt
        DCD     TIM2_IRQHandler           ; Timer 2 Interrupt
        DCD     MNG_IRQHandler            ; Management Peripheral Interrupt
        DCD     BASEBAND_IRQHandler       ; Base Band Interrupt
        DCD     SLPTIM_IRQHandler         ; Sleep Timer Interrupt
        DCD     SC1_IRQHandler            ; Serial Controller 1 Interrupt
        DCD     SC2_IRQHandler            ; Serial Controller 2 Interrupt
        DCD     SECURITY_IRQHandler       ; Security Interrupt
        DCD     MAC_TIM_IRQHandler        ; MAC Timer Interrupt
        DCD     MAC_TR_IRQHandler         ; MAC Transmit Interrupt
        DCD     MAC_RE_IRQHandler         ; MAC Receive Interrupt
        DCD     ADC_IRQHandler            ; ADC Interrupt
        DCD     EXTIA_IRQHandler          ; EXTIA Interrupt
        DCD     EXTIB_IRQHandler          ; EXTIB Interrupt
        DCD     EXTIC_IRQHandler          ; EXTIC Interrupt
        DCD     EXTID_IRQHandler          ; EXTID Interrupt
        DCD     DEBUG_IRQHandler          ; Debug Interrupt        
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
        THUMB

        PUBWEAK Reset_Handler
        SECTION .text:CODE:REORDER(2)
Reset_Handler
        LDR     R0, =SystemInit
        BLX     R0
        LDR     R0, =__iar_program_start
        BX      R0
        
        PUBWEAK NMI_Handler
        SECTION .text:CODE:REORDER(1)
NMI_Handler
        B NMI_Handler
        
        
        PUBWEAK HardFault_Handler
        SECTION .text:CODE:REORDER(1)
HardFault_Handler
        B HardFault_Handler
        
        
        PUBWEAK MemManage_Handler
        SECTION .text:CODE:REORDER(1)
MemManage_Handler
        B MemManage_Handler
        
                
        PUBWEAK BusFault_Handler
        SECTION .text:CODE:REORDER(1)
BusFault_Handler
        B BusFault_Handler
        
        
        PUBWEAK UsageFault_Handler
        SECTION .text:CODE:REORDER(1)
UsageFault_Handler
        B UsageFault_Handler
        
        
        PUBWEAK SVC_Handler
        SECTION .text:CODE:REORDER(1)
SVC_Handler
        B SVC_Handler
        
        
        PUBWEAK DebugMon_Handler
        SECTION .text:CODE:REORDER(1)
DebugMon_Handler
        B DebugMon_Handler
        
        
        PUBWEAK PendSV_Handler
        SECTION .text:CODE:REORDER(1)
PendSV_Handler
        B PendSV_Handler
        
        
        PUBWEAK SysTick_Handler
        SECTION .text:CODE:REORDER(1)
SysTick_Handler
        B SysTick_Handler
        
        
        PUBWEAK TIM1_IRQHandler
        SECTION .text:CODE:REORDER(1)
TIM1_IRQHandler
        B TIM1_IRQHandler
        
        
        PUBWEAK TIM2_IRQHandler
        SECTION .text:CODE:REORDER(1)
TIM2_IRQHandler
        B TIM2_IRQHandler
        
        
        PUBWEAK MNG_IRQHandler
        SECTION .text:CODE:REORDER(1)
MNG_IRQHandler
        B MNG_IRQHandler
        
        
        PUBWEAK BASEBAND_IRQHandler
        SECTION .text:CODE:REORDER(1)
BASEBAND_IRQHandler
        B BASEBAND_IRQHandler
        
        
        PUBWEAK SLPTIM_IRQHandler
        SECTION .text:CODE:REORDER(1)
SLPTIM_IRQHandler
        B SLPTIM_IRQHandler
        
        
        PUBWEAK SC1_IRQHandler
        SECTION .text:CODE:REORDER(1)
SC1_IRQHandler
        B SC1_IRQHandler
        
        
        PUBWEAK SC2_IRQHandler
        SECTION .text:CODE:REORDER(1)
SC2_IRQHandler
        B SC2_IRQHandler
        
        
        PUBWEAK SECURITY_IRQHandler
        SECTION .text:CODE:REORDER(1)
SECURITY_IRQHandler
        B SECURITY_IRQHandler
        
        
        PUBWEAK MAC_TIM_IRQHandler
        SECTION .text:CODE:REORDER(1)
MAC_TIM_IRQHandler
        B MAC_TIM_IRQHandler
        
        
        PUBWEAK MAC_TR_IRQHandler
        SECTION .text:CODE:REORDER(1)
MAC_TR_IRQHandler
        B MAC_TR_IRQHandler
        
        
        PUBWEAK MAC_RE_IRQHandler
        SECTION .text:CODE:REORDER(1)
MAC_RE_IRQHandler
        B MAC_RE_IRQHandler
        
        
        PUBWEAK ADC_IRQHandler
        SECTION .text:CODE:REORDER(1)
ADC_IRQHandler
        B ADC_IRQHandler
        
        
        PUBWEAK EXTIA_IRQHandler
        SECTION .text:CODE:REORDER(1)
EXTIA_IRQHandler
        B EXTIA_IRQHandler
        
        
        PUBWEAK EXTIB_IRQHandler
        SECTION .text:CODE:REORDER(1)
EXTIB_IRQHandler
        B EXTIB_IRQHandler
        
        
        PUBWEAK EXTIC_IRQHandler
        SECTION .text:CODE:REORDER(1)
EXTIC_IRQHandler
        B EXTIC_IRQHandler
        
        
        PUBWEAK EXTID_IRQHandler
        SECTION .text:CODE:REORDER(1)
EXTID_IRQHandler
        B EXTID_IRQHandler
        
        
        PUBWEAK DEBUG_IRQHandler
        SECTION .text:CODE:REORDER(1)
DEBUG_IRQHandler
        B DEBUG_IRQHandler
        
        END

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
