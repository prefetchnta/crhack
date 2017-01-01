@echo off
echo C_ADDONS=-proc %1 -si-revision %2 > conf.inc
echo A_ADDONS= >> conf.inc
echo ALL_OBJS=$(VDSP_NAKED_BFIN_OBJS) >> conf.inc
echo ALL_CPPS=$(VDSP_NAKED_BFIN_CPPS) >> conf.inc
call "..\bat\vdsp50_naked_adsp.bat"
gmake-378 -fvdsp\naked\bfin\core.mak
gmake-378 -fvdsp\naked\bfin\gfx2.mak
gmake-378 -fvdsp\naked\bfin\gfx3.mak
gmake-378 -fvdsp\naked\bfin\math.mak
gmake-378 -fvdsp\naked\bfin\fmtz.mak
gmake-378 -fvdsp\naked\bfin\util.mak
move *.dlb ..\bin\vdsp_naked_bfin
del conf.inc
