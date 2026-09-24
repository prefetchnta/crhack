@echo off
echo C_ADDONS=%~1 > conf.inc
echo A_ADDONS= >> conf.inc
echo ALL_OBJS=$(GCC_NAKED_ALL_OBJS) >> conf.inc
echo ALL_CPPS=$(GCC_NAKED_ALL_CPPS) >> conf.inc
call "..\bat\mdk50_naked_armclang.bat"
gmake -farmclang\naked\arm\core.mak
gmake -farmclang\naked\arm\gfx2.mak
gmake -farmclang\naked\arm\gfx3.mak
gmake -farmclang\naked\arm\math.mak
gmake -farmclang\naked\arm\fmtz.mak
gmake -farmclang\naked\arm\util.mak
move *.a ..\bin\armclang_naked_arm
del conf.inc
