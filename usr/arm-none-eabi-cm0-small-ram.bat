@echo off
cd ..\mak
call naked.bat "-mcpu=cortex-m0 -mthumb -D_CR_USE_FP32_ -D_BJNETCOMM_SMALL_ROM_ -D_BJNETCOMM_SMALL_RAM_ -D_CR_NAKED_NO_WSTRING_" arm-none-eabi
md ..\bin\arm-cm0-small-ram
move ..\bin\gcc_naked_all\*.a ..\bin\arm-cm0-small-ram
cd ..\usr
