@echo off
cd ..\mak
call naked.bat "-mcpu=cortex-m3 -mthumb -D_BJNETCOMM_SMALL_ROM_ -D_CR_NAKED_NO_WSTRING_" arm-none-eabi
md ..\bin\arm-cm3-big-ram
move ..\bin\gcc_naked_all\*.a ..\bin\arm-cm3-big-ram
cd ..\usr
