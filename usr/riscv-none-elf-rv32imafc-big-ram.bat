@echo off
cd ..\mak
call naked.bat "-march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -D_CR_USE_FP32_ -D_BJNETCOMM_SMALL_ROM_ -D_CR_NAKED_NO_WSTRING_" riscv-none-elf rv32imafc_zicsr\ilp32f
md ..\bin\rv32imafc-big-ram
move ..\bin\gcc_naked_all\*.a ..\bin\rv32imafc-big-ram
cd ..\usr
