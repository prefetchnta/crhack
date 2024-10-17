@echo off
cd ..\mak
call naked.bat "-march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -D_CR_USE_FP32_ -D_BJNETCOMM_SMALL_ROM_ -D_BJNETCOMM_SMALL_RAM_ -D_CR_NAKED_NO_WSTRING_" riscv-none-elf rv32imc\ilp32
md ..\bin\rv32imac-small-ram
move ..\bin\gcc_naked_all\*.a ..\bin\rv32imac-small-ram
cd ..\usr
