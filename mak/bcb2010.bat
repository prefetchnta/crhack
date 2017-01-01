@echo off
echo R_ADDONS= > conf.inc
echo C_ADDONS= >> conf.inc
echo A_ADDONS= >> conf.inc
echo L_ADDONS= >> conf.inc
echo ALL_OBJS=$(BCC32_WIN32_X86_OBJS) >> conf.inc
echo ALL_CPPS=$(BCC32_WIN32_X86_CPPS) >> conf.inc
call "..\bat\cb2010_win32_x86.bat"
make -fbcc32\win32\x86\core.mak
make -fbcc32\win32\x86\gfx2.mak
make -fbcc32\win32\x86\gfx3.mak
make -fbcc32\win32\x86\math.mak
make -fbcc32\win32\x86\fmtz.mak
make -fbcc32\win32\x86\util.mak
move *.dll ..\bin\cb2010_win32_x86
move *.lib ..\bin\cb2010_win32_x86
del conf.inc
