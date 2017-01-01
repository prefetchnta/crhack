@echo off
echo R_ADDONS= > conf.inc
echo C_ADDONS=/D "WIN32" /D "_WIN32" /I "..\inc\l3rd\dxsdk" /wd4819 /Oy- /WX- /Gm- /sdl /arch:SSE2 >> conf.inc
echo A_ADDONS= >> conf.inc
echo L_ADDONS=/MACHINE:X86 /LIBPATH:$(SDK_COFF_X86)dxsdk >> conf.inc
echo SUBSYSTEM=WINDOWS >> conf.inc
echo RFLAGS=$(RFLAGS_WIN32) >> conf.inc
echo MFLAGS=$(MFLAGS_WIN32) >> conf.inc
echo CFLAGS=$(CFLAGS_WIN32) >> conf.inc
echo AFLAGS=$(AFLAGS_WIN32) >> conf.inc
echo LFLAGS=$(LFLAGS_WIN32) >> conf.inc
echo ALL_OBJS=$(MSC64_WIN32_X86_OBJS) >> conf.inc
echo ALL_CPPS=$(MSC64_WIN32_X86_CPPS) >> conf.inc
call "%VS120COMNTOOLS%..\..\VC\vcvarsall.bat" x86
nmake /NOLOGO /Fmsc64\win32\x86\core.mak
nmake /NOLOGO /Fmsc64\win32\x86\gfx2.mak
nmake /NOLOGO /Fmsc64\win32\x86\gfx3.mak
nmake /NOLOGO /Fmsc64\win32\x86\math.mak
nmake /NOLOGO /Fmsc64\win32\x86\fmtz.mak
nmake /NOLOGO /Fmsc64\win32\x86\util.mak
move *.dll ..\bin\vc2013_win32_x86
move *.lib ..\bin\vc2013_win32_x86
del conf.inc

echo R_ADDONS= > conf.inc
echo C_ADDONS=/D "WIN64" /D "_WIN64" /I "..\inc\l3rd\dxsdk" /wd4819 /Oy- /WX- /Gm- /sdl >> conf.inc
echo A_ADDONS= >> conf.inc
echo L_ADDONS=/MACHINE:X64 /LIBPATH:$(SDK_COFF_X64)dxsdk >> conf.inc
echo SUBSYSTEM=WINDOWS >> conf.inc
echo RFLAGS=$(RFLAGS_WIN32) >> conf.inc
echo MFLAGS=$(MFLAGS_WIN32) >> conf.inc
echo CFLAGS=$(CFLAGS_WIN32) >> conf.inc
echo AFLAGS=$(AFLAGS_WIN32) >> conf.inc
echo LFLAGS=$(LFLAGS_WIN32) >> conf.inc
echo ALL_OBJS=$(MSC64_WIN32_X64_OBJS) >> conf.inc
echo ALL_CPPS=$(MSC64_WIN32_X64_CPPS) >> conf.inc
call "%VS120COMNTOOLS%..\..\VC\vcvarsall.bat" x86_amd64
nmake /NOLOGO /Fmsc64\win32\x64\core.mak
nmake /NOLOGO /Fmsc64\win32\x64\gfx2.mak
nmake /NOLOGO /Fmsc64\win32\x64\gfx3.mak
nmake /NOLOGO /Fmsc64\win32\x64\math.mak
nmake /NOLOGO /Fmsc64\win32\x64\fmtz.mak
nmake /NOLOGO /Fmsc64\win32\x64\util.mak
move *.dll ..\bin\vc2013_win32_x64
move *.lib ..\bin\vc2013_win32_x64
del conf.inc
