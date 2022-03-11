@echo off
cd ..\mak
call mdk50_32.bat "--cpu Cortex-M0plus -D_CR_USE_FP32_ -D_BJNETCOMM_SMALL_ROM_ -D_BJNETCOMM_SMALL_RAM_ -D_CR_NAKED_NO_WSTRING_"
md ..\bin\arm-cm0p-small-ram
move ..\bin\armcc_naked_arm\*.a ..\bin\arm-cm0p-small-ram
cd ..\usr
