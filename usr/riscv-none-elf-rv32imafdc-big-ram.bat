@echo off
cd ..\mak
call naked.bat "-march=rv32imafdc -mabi=ilp32 -msmall-data-limit=8 -mno-save-restore -D_BJNETCOMM_SMALL_ROM_ -D_CR_NAKED_NO_WSTRING_" riscv-none-elf rv32imfdc_zicsr\ilp32d
md ..\bin\rv32imafdc-big-ram
move ..\bin\gcc_naked_all\*.a ..\bin\rv32imafdc-big-ram
cd ..\usr
