/*****************************************************************************/
/*                                                  ###                      */
/*       #####          ###    ###                  ###  CREATE: 2010-09-08  */
/*     #######          ###    ###      [CORE]      ###  ~~~~~~~~~~~~~~~~~~  */
/*    ########          ###    ###                  ###  MODIFY: XXXX-XX-XX  */
/*    ####  ##          ###    ###                  ###  ~~~~~~~~~~~~~~~~~~  */
/*   ###       ### ###  ###    ###    ####    ####  ###   ##  +-----------+  */
/*  ####       ######## ##########  #######  ###### ###  ###  |  A NEW C  |  */
/*  ###        ######## ########## ########  ###### ### ###   | FRAMEWORK |  */
/*  ###     ## #### ### ########## ###  ### ###     ######    |  FOR ALL  |  */
/*  ####   ### ###  ### ###    ### ###  ### ###     ######    | PLATFORMS |  */
/*  ########## ###      ###    ### ######## ####### #######   |  AND ALL  |  */
/*   #######   ###      ###    ### ########  ###### ###  ###  | COMPILERS |  */
/*    #####    ###      ###    ###  #### ##   ####  ###   ##  +-----------+  */
/*  =======================================================================  */
/*  >>>>>>>>>>>>>>>>>>>>>>>> CrHack 循环延时函数库 <<<<<<<<<<<<<<<<<<<<<<<<  */
/*  =======================================================================  */
/*****************************************************************************/

#include "msclib.h"

#if !defined(__no_delay08)
/*
=======================================
    8 位循环延时
=======================================
*/
CR_API void_t
delay08 (
  __CR_IN__ byte_t  num
    )
{
/*
    ===================================
    [KeilC51]       (8 + 6 * num) ck
    -----------------------------------
            MOV     R7,#07BH    1 ck
            LCALL   _delay08    2 ck
    -----------------------------------
    ?C0001:
            MOV     A,R7        1 ck
            JZ      ?C0004      2 ck
            DEC     R7          1 ck
            SJMP    ?C0001      2 ck
    ?C0004:
            RET                 2 ck
    ===================================
*/
    for (; num != 0; num--);
}

#endif  /* !__no_delay08 */

#if !defined(__no_delay16)
/*
=======================================
    16位循环延时
=======================================
*/
CR_API void_t
delay16 (
  __CR_IN__ int16u  num
    )
{
/*
    ===================================
    [KeilC51]     (10 + 11 * num) ck
    -----------------------------------
            MOV     R7,#07BH    1 ck
            MOV     R6,#000H    1 ck
            LCALL   _delay16    2 ck
    -----------------------------------
    ?C0005:
            MOV     A,R7        1 ck
            ORL     A,R6        1 ck
            JZ      ?C0008      2 ck
            MOV     A,R7        1 ck
            DEC     R7          1 ck
            JNZ     ?C0005      2 ck
            DEC     R6          1 ck
            SJMP    ?C0005      2 ck
    ?C0008:
            RET                 2 ck
    ===================================
*/
    for (; num != 0; num--);
}

#endif  /* !__no_delay16 */

#if !defined(__no_delay32)
/*
=======================================
    32位循环延时
=======================================
*/
CR_API void_t
delay32 (
  __CR_IN__ int32u  num
    )
{
/*
    ===================================
    [KeilC51]     (48 + 46 * num) ck
    -----------------------------------
            MOV     R7,#07BH    1 ck
            MOV     R6,#000H    1 ck
            MOV     R5,#000H    1 ck
            MOV     R4,#000H    1 ck
            LCALL   _delay32    2 ck
    -----------------------------------
            MOV     num+03H,R7  2 ck
            MOV     num+02H,R6  2 ck
            MOV     num+01H,R5  2 ck
            MOV     num+00H,R4  2 ck
    ?C0009:
            CLR     A           1 ck
            MOV     R7,A        1 ck
            MOV     R6,A        1 ck
            MOV     R5,A        1 ck
            MOV     R4,A        1 ck
            MOV     R3,num+03H  2 ck
            MOV     R2,num+02H  2 ck
            MOV     R1,num+01H  2 ck
            MOV     R0,num+00H  2 ck
            CLR     C           1 ck
            LCALL   ?C?ULCMP    2 ck + 14 ck
            JZ      ?C0012      2 ck
            MOV     A,num+03H   1 ck
            ADD     A,#0FFH     1 ck
            MOV     num+03H,A   1 ck
            MOV     A,num+02H   1 ck
            ADDC    A,#0FFH     1 ck
            MOV     num+02H,A   1 ck
            MOV     A,num+01H   1 ck
            ADDC    A,#0FFH     1 ck
            MOV     num+01H,A   1 ck
            MOV     A,num+00H   1 ck
            ADDC    A,#0FFH     1 ck
            MOV     num+00H,A   1 ck
            SJMP    ?C0009      2 ck
    ?C0012:
            RET                 2 ck
    -----------------------------------
    ?C?ULCMP:
            MOV     A,R3        1 ck
            SUBB    A,R7        1 ck
            MOV     B,A         1 ck
            MOV     A,R2        1 ck
            SUBB    A,R6        1 ck
            ORL     B,A         1 ck
            MOV     A,R1        1 ck
            SUBB    A,R5        1 ck
            ORL     B,A         1 ck
            MOV     A,R0        1 ck
            SUBB    A,R4        1 ck
            ORL     A,B         1 ck
            RET                 2 ck
    ===================================
*/
    for (; num != 0; num--);
}

#endif  /* !__no_delay32 */

/*****************************************************************************/
/* _________________________________________________________________________ */
/* uBMAzRBoAKAHaACQD6FoAIAPqbgA/7rIA+5CM9uKw8D4Au7u7mSIJ0t18mYz0mYz9rAQZCgHc */
/* wRIZIgHZovGBXUAZg+v0GbB+gRmgcJ7BAAAisIlAwB1Av7LSHUC/s9IdQL+w0h1Av7HZkZmgf */
/* 4JLgIAdb262gPsqAh0+zP/uQB9ZYsFZYktq+L3sMi/AAK7gAKJAUtLdfq5IANXvT8BiQzfBIv */
/* FLaAAweAEmff53wb+Adjx3kQE2xwy5Io8ithkigcFgACJBN8E3sneNvwB2xyLHDkdfA2JHSyA */
/* adtAAQPdZYgHR0dNdbZfSYP5UHWr/kQEtAHNFg+Eef/DWAKgDw== |~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< */
/*****************************************************************************/
