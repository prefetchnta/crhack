@echo off
echo R_ADDONS= > conf.inc
echo C_ADDONS=-m32 -msse -mfpmath=sse -DWIN32 -D_WIN32 >> conf.inc
echo A_ADDONS= >> conf.inc
echo L_ADDONS=-static-libgcc >> conf.inc
echo CROSS_COMPILE= >> conf.inc
echo RFLAGS=$(RFLAGS_WIN32) >> conf.inc
echo CFLAGS=$(CFLAGS_WIN32) >> conf.inc
echo AFLAGS=$(AFLAGS_WIN32) >> conf.inc
echo LFLAGS=$(LFLAGS_WIN32) >> conf.inc
echo LFLAGS_1=$(LFLAGS_WIN32_1) >> conf.inc
echo SFLAGS=$(SFLAGS_WIN32) >> conf.inc
echo ALL_OBJS=$(GCC_WIN32_X86_OBJS) >> conf.inc
echo ALL_CPPS=$(GCC_WIN32_X86_CPPS) >> conf.inc
call "..\bat\mingw_win32_x86.bat"
mingw32-make -fgcc\win32\x86\core.mak
mingw32-make -fgcc\win32\x86\gfx2.mak
mingw32-make -fgcc\win32\x86\gfx3.mak
mingw32-make -fgcc\win32\x86\math.mak
mingw32-make -fgcc\win32\x86\fmtz.mak
mingw32-make -fgcc\win32\x86\util.mak
move *.dll ..\bin\mingw_win32_x86
move *.a   ..\bin\mingw_win32_x86
del conf.inc
