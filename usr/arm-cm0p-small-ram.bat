@echo off
cd ..\mak
echo C_ADDONS=--cpu Cortex-M0plus -D_BJNETCOMM_SMALL_ROM_ -D_BJNETCOMM_SMALL_RAM_ -D_CR_NAKED_NO_WSTRING_ > conf.inc
echo A_ADDONS= >> conf.inc
echo ALL_OBJS=$(ARMCC_NAKED_ARM_OBJS) >> conf.inc
echo ALL_CPPS=$(ARMCC_NAKED_ARM_CPPS) >> conf.inc
call "..\bat\mdk40_naked_arm.bat"
make54 -farmcc\naked\arm\core.mak
make54 -farmcc\naked\arm\gfx2.mak
make54 -farmcc\naked\arm\gfx3.mak
make54 -farmcc\naked\arm\math.mak
make54 -farmcc\naked\arm\fmtz.mak
make54 -farmcc\naked\arm\util.mak
md ..\bin\arm-cm0p-small-ram
move *.a ..\bin\arm-cm0p-small-ram
del conf.inc
cd ..\usr
