@echo off
echo R_ADDONS= > conf.inc
echo C_ADDONS=-m64 -DWIN64 -D_WIN64 >> conf.inc
echo A_ADDONS= >> conf.inc
echo L_ADDONS=-static-libgcc >> conf.inc
echo CROSS_COMPILE= >> conf.inc
echo RFLAGS=$(RFLAGS_WIN32) >> conf.inc
echo CFLAGS=$(CFLAGS_WIN32) >> conf.inc
echo AFLAGS=$(AFLAGS_WIN32) >> conf.inc
echo LFLAGS=$(LFLAGS_WIN32) >> conf.inc
echo LFLAGS_1=$(LFLAGS_WIN32_1) >> conf.inc
echo SFLAGS=$(SFLAGS_WIN32) >> conf.inc
echo ALL_OBJS=$(GCC_WIN32_X64_OBJS) >> conf.inc
echo ALL_CPPS=$(GCC_WIN32_X64_CPPS) >> conf.inc
call "..\bat\mingw_win32_x64.bat"
mingw32-make -fgcc\win32\x64\core.mak
mingw32-make -fgcc\win32\x64\gfx2.mak
mingw32-make -fgcc\win32\x64\gfx3.mak
mingw32-make -fgcc\win32\x64\math.mak
mingw32-make -fgcc\win32\x64\fmtz.mak
mingw32-make -fgcc\win32\x64\util.mak
move *.dll ..\bin\mingw_win32_x64
move *.a   ..\bin\mingw_win32_x64
del conf.inc
