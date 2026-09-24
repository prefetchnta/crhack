@echo off
cd ..\mak
call naked.bat "-mcpu=cortex-m33 -mthumb -mfpu=fpv5-sp-d16 -mfloat-abi=hard -D_CR_USE_FP32_ -D_BJNETCOMM_SMALL_ROM_ -D_CR_NAKED_NO_WSTRING_" arm-none-eabi
md ..\bin\arm-cm33f-big-ram
md ..\bin\arm-cm33f-big-ram\gcc
move ..\bin\gcc_naked_all\*.a ..\bin\arm-cm33f-big-ram\gcc
cd ..\usr
