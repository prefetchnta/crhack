@echo off
cd ..\mak
call naked.bat "-march=rv32ecxw -mabi=ilp32e -msmall-data-limit=8 -mno-save-restore -D_CR_USE_FP32_ -D_BJNETCOMM_SMALL_ROM_ -D_BJNETCOMM_SMALL_RAM_ -D_CR_NAKED_NO_WSTRING_" riscv-none-embed rv32ecxw\ilp32e
md ..\bin\qingke_v2a-small-ram
move ..\bin\gcc_naked_all\*.a ..\bin\qingke_v2a-small-ram
cd ..\usr
