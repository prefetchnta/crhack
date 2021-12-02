@echo off
cd ..\mak
call mdk50_32.bat "--cpu Cortex-M4.fp.sp -D_BJNETCOMM_SMALL_ROM_ -D_CR_NAKED_NO_WSTRING_"
md ..\bin\arm-cm4f-big-ram
move ..\bin\armcc_naked_arm\*.a ..\bin\arm-cm4f-big-ram
cd ..\usr
