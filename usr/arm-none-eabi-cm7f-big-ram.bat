@echo off
cd ..\mak
call naked.bat "-mcpu=cortex-m7 -mthumb -mfpu=fpv5-d16 -mfloat-abi=hard -D_BJNETCOMM_SMALL_ROM_ -D_CR_NAKED_NO_WSTRING_" arm-none-eabi thumb\v7e-m+dp\hard
md ..\bin\arm-cm7f-big-ram
move ..\bin\gcc_naked_all\*.a ..\bin\arm-cm7f-big-ram
cd ..\usr
