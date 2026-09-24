@echo off
cd ..\mak
call mdk50_ac6.bat "--target=arm-arm-none-eabi -mcpu=cortex-m33 -mthumb -mfpu=fpv5-sp-d16 -mfloat-abi=hard -D_CR_USE_FP32_ -D_BJNETCOMM_SMALL_ROM_ -D_CR_NAKED_NO_WSTRING_"
md ..\bin\arm-cm33f-big-ram
md ..\bin\arm-cm33f-big-ram\ac6
move ..\bin\armclang_naked_arm\*.a ..\bin\arm-cm33f-big-ram\ac6
cd ..\usr
