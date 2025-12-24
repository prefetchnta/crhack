@echo off
cd ..\mak
call naked.bat "-march=rv32imafc_xw -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -D_CR_USE_FP32_ -D_BJNETCOMM_SMALL_ROM_ -D_CR_NAKED_NO_WSTRING_" riscv-wch-elf rv32imafc_xw\ilp32f
md ..\bin\qingke_v3f_v4f_v5f-big-ram\wch
move ..\bin\gcc_naked_all\*.a ..\bin\qingke_v3f_v4f_v5f-big-ram\wch
cd ..\usr
