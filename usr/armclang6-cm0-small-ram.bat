@echo off
cd ..\mak
call mdk50_ac6.bat "--target=arm-arm-none-eabi -mcpu=cortex-m0 -mthumb -D_CR_USE_FP32_ -D_BJNETCOMM_SMALL_ROM_ -D_BJNETCOMM_SMALL_RAM_ -D_CR_NAKED_NO_WSTRING_"
md ..\bin\arm-cm0-small-ram
md ..\bin\arm-cm0-small-ram\ac6
move ..\bin\armclang_naked_arm\*.a ..\bin\arm-cm0-small-ram\ac6
cd ..\usr
