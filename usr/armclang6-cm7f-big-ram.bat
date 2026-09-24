@echo off
cd ..\mak
call mdk50_ac6.bat "--target=arm-arm-none-eabi -mcpu=cortex-m7 -mthumb -mfpu=fpv5-d16 -mfloat-abi=hard -D_BJNETCOMM_SMALL_ROM_ -D_CR_NAKED_NO_WSTRING_"
md ..\bin\arm-cm7f-big-ram
md ..\bin\arm-cm7f-big-ram\ac6
move ..\bin\armclang_naked_arm\*.a ..\bin\arm-cm7f-big-ram\ac6
cd ..\usr
