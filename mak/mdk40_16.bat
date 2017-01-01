@echo off
echo C_ADDONS=--cpu %1 > conf.inc
echo A_ADDONS= >> conf.inc
echo ARMCC_THUMB=1 >> conf.inc
echo ALL_OBJS=$(ARMCC_NAKED_ARM_OBJS) >> conf.inc
echo ALL_CPPS=$(ARMCC_NAKED_ARM_CPPS) >> conf.inc
call "..\bat\mdk40_naked_arm.bat"
make54 -farmcc\naked\arm\core.mak
make54 -farmcc\naked\arm\gfx2.mak
make54 -farmcc\naked\arm\gfx3.mak
make54 -farmcc\naked\arm\math.mak
make54 -farmcc\naked\arm\fmtz.mak
make54 -farmcc\naked\arm\util.mak
move *.a ..\bin\armcc_naked_arm
del conf.inc
