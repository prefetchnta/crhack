;/**************************************************************************//**
; * @file     startup_stm32w108.s
; * @brief    CMSIS Core Device Startup File for
; *           STM32W108 Device Series
; * @version  V1.0.1
; * @date     30 November 2012
; *
; * @note
; * Copyright (C) 2012 ARM Limited. All rights reserved.
; *
; * @par
; * ARM Limited (ARM) is supplying this software for use with Cortex-M 
; * processor based microcontrollers.  This file can be freely distributed 
; * within development tools that are supporting such ARM based processors. 
; *
; * @par
; * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
; * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
; * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
; * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
; * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
; *
; ******************************************************************************/
;/*
;//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
;*/

; Amount of memory (in bytes) allocated for Stack
; Tailor this value to your application needs
; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>


Stack_Size      EQU     0x00001000

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000000

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit

                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset
                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp                      ; Top of Stack
                DCD     Reset_Handler                     ; Reset Handler
                DCD     NMI_Handler                       ; NMI Handler
                DCD     HardFault_Handler                 ; Hard Fault Handler
                DCD     MemManage_Handler                 ; MPU Fault Handler
                DCD     BusFault_Handler                  ; Bus Fault Handler
                DCD     UsageFault_Handler                ; Usage Fault Handler
				DCD     0                         		  ; Reserved
                DCD     0                                 ; Reserved
                DCD     0                                 ; Reserved
                DCD     0                                 ; Reserved
                DCD     SVC_Handler                       ; SVCall Handler
                DCD     DebugMon_Handler                  ; Debug Monitor Handler
                DCD     0                                 ; Reserved
                DCD     PendSV_Handler                    ; PendSV Handler
                DCD     SysTick_Handler                   ; SysTick Handler

                ; External Interrupts
                DCD     TIM1_IRQHandler                   ; 16+ 0   Timer 1 Interrupt
                DCD     TIM2_IRQHandler                   ; 16+ 1   Timer 2 Interrupt
                DCD     MNG_IRQHandler                    ; 16+ 2   Management Peripheral Interrupt
                DCD     BASEBAND_IRQHandler               ; 16+ 3   Base Band Interrupt
                DCD     SLPTIM_IRQHandler                 ; 16+ 4   Sleep Timer Interrupt
                DCD     SC1_IRQHandler                    ; 16+ 5   Serial Controller 1 Interrupt
                DCD     SC2_IRQHandler                    ; 16+ 6   Serial Controller 2 Interrupt
                DCD     SECURITY_IRQHandler               ; 16+ 7   Security Interrupt
                DCD     MAC_TIM_IRQHandler                ; 16+ 8   MAC Timer Interrupt
                DCD     MAC_TR_IRQHandler                 ; 16+ 9   MAC Transmit Interrupt
                DCD     MAC_RE_IRQHandler                 ; 16+10   MAC Receive Interrupt
                DCD     ADC_IRQHandler                    ; 16+11   ADC Interrupt
                DCD     EXTIA_IRQHandler                  ; 16+12   EXTIA Interrupt
                DCD     EXTIB_IRQHandler                  ; 16+13   EXTIB Interrupt
                DCD     EXTIC_IRQHandler                  ; 16+14   EXTIC Interrupt
                DCD     EXTID_IRQHandler                  ; 16+15   EXTID Interrupt
                DCD     DEBUG_IRQHandler                  ; 16+16   Debug Interrupt

__Vectors_End

__Vectors_Size  EQU  __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY

; Reset handler
Reset_Handler    PROC
                 EXPORT  Reset_Handler             [WEAK]
				 IMPORT  SystemInit
        IMPORT  __main

                 LDR     R0, =SystemInit
                 BLX     R0
                 LDR     R0, =__main
                 BX      R0
                 ENDP

; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler                [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler          [WEAK]
                B       .
                ENDP
MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler          [WEAK]
                B       .
                ENDP
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler           [WEAK]
                B       .
                ENDP
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler         [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler                [WEAK]
                B       .
                ENDP
DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler           [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler             [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler            [WEAK]
                B       .
                ENDP

Default_Handler PROC

                EXPORT  TIM1_IRQHandler            [WEAK]
                EXPORT  TIM2_IRQHandler            [WEAK]
                EXPORT  MNG_IRQHandler             [WEAK]
                EXPORT  BASEBAND_IRQHandler        [WEAK]
                EXPORT  SLPTIM_IRQHandler          [WEAK]
                EXPORT  SC1_IRQHandler             [WEAK]
                EXPORT  SC2_IRQHandler             [WEAK]
                EXPORT  SECURITY_IRQHandler        [WEAK]
                EXPORT  MAC_TIM_IRQHandler         [WEAK]
                EXPORT  MAC_TR_IRQHandler          [WEAK]
                EXPORT  MAC_RE_IRQHandler          [WEAK]
                EXPORT  ADC_IRQHandler             [WEAK]
                EXPORT  EXTIA_IRQHandler           [WEAK]
                EXPORT  EXTIB_IRQHandler           [WEAK]
                EXPORT  EXTIC_IRQHandler           [WEAK]
                EXPORT  EXTID_IRQHandler           [WEAK]
                EXPORT  DEBUG_IRQHandler           [WEAK]

TIM1_IRQHandler
TIM2_IRQHandler
MNG_IRQHandler
BASEBAND_IRQHandler
SLPTIM_IRQHandler
SC1_IRQHandler
SC2_IRQHandler
SECURITY_IRQHandler
MAC_TIM_IRQHandler
MAC_TR_IRQHandler
MAC_RE_IRQHandler
ADC_IRQHandler
EXTIA_IRQHandler
EXTIB_IRQHandler
EXTIC_IRQHandler
EXTID_IRQHandler
DEBUG_IRQHandler

                B       .

                ENDP

                ALIGN

;*******************************************************************************
; User Stack and Heap initialization
;*******************************************************************************
                 IF      :DEF:__MICROLIB

                 EXPORT  __initial_sp
                 EXPORT  __heap_base
                 EXPORT  __heap_limit

                 ELSE

                 IMPORT  __use_two_region_memory
                 EXPORT  __user_initial_stackheap

__user_initial_stackheap

                 LDR     R0, =  Heap_Mem
                 LDR     R1, =(Stack_Mem + Stack_Size)
                 LDR     R2, = (Heap_Mem +  Heap_Size)
                 LDR     R3, = Stack_Mem
                 BX      LR

                 ALIGN

                 ENDIF

                 END
