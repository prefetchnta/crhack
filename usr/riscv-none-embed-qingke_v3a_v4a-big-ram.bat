@echo off
cd ..\mak
call naked.bat "-march=rv32imac -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -D_CR_USE_FP32_ -D_BJNETCOMM_SMALL_ROM_ -D_CR_NAKED_NO_WSTRING_" riscv-none-embed rv32imac\ilp32
md ..\bin\qingke_v3a_v4a-big-ram
move ..\bin\gcc_naked_all\*.a ..\bin\qingke_v3a_v4a-big-ram
cd ..\usr
