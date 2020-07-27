@echo off
cd ..\mak
call naked.bat "-mcpu=cortex-m4 -mfloat-abi=hard -D_BJNETCOMM_SMALL_ROM_ -D_CR_NAKED_NO_WSTRING_" arm-none-eabi hard
md ..\bin\arm-cm4f-big-ram
move ..\bin\gcc_naked_all\*.a ..\bin\arm-cm4f-big-ram
cd ..\usr
