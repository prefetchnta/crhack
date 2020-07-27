@echo off
cd ..\mak
call mdk40_32.bat "--cpu Cortex-M3 -D_BJNETCOMM_SMALL_ROM_ -D_CR_NAKED_NO_WSTRING_"
md ..\bin\arm-cm3-big-ram
move ..\bin\armcc_naked_arm\*.a ..\bin\arm-cm3-big-ram
cd ..\usr
