@echo off
echo R_ADDONS= > conf.inc
echo C_ADDONS=/D "WIN32" /D "_WIN32" /I "..\inc\l3rd\dxsdk" /I "..\src\win32\dshow" /wd4819 /Wp64 /arch:SSE >> conf.inc
echo A_ADDONS= >> conf.inc
echo L_ADDONS=/MACHINE:X86 /LIBPATH:$(SDK_COFF_X86)dxsdk /LIBPATH:$(SDK_COFF_X86)dshow >> conf.inc
echo SUBSYSTEM=WINDOWS >> conf.inc
echo RFLAGS=$(RFLAGS_WIN32) >> conf.inc
echo MFLAGS=$(MFLAGS_WIN32) >> conf.inc
echo CFLAGS=$(CFLAGS_WIN32) >> conf.inc
echo AFLAGS=$(AFLAGS_WIN32) >> conf.inc
echo LFLAGS=$(LFLAGS_WIN32) >> conf.inc
echo ALL_OBJS=$(MSC64_WIN32_X86_OBJS) >> conf.inc
echo ALL_CPPS=$(MSC64_WIN32_X86_CPPS) >> conf.inc
call "%VS80COMNTOOLS%..\..\VC\vcvarsall.bat" x86
nmake /NOLOGO /Fmsc64\win32\x86\core.mak
nmake /NOLOGO /Fmsc64\win32\x86\gfx2.mak
nmake /NOLOGO /Fmsc64\win32\x86\gfx3.mak
nmake /NOLOGO /Fmsc64\win32\x86\math.mak
nmake /NOLOGO /Fmsc64\win32\x86\fmtz.mak
nmake /NOLOGO /Fmsc64\win32\x86\util.mak
move *.dll ..\bin\vc2005_win32_x86
move *.lib ..\bin\vc2005_win32_x86
del conf.inc

echo R_ADDONS= > conf.inc
echo C_ADDONS=/D "WIN64" /D "_WIN64" /I "..\inc\l3rd\dxsdk" /I "..\src\win32\dshow" /wd4819 >> conf.inc
echo A_ADDONS= >> conf.inc
echo L_ADDONS=/MACHINE:X64 /LIBPATH:$(SDK_COFF_X64)dxsdk /LIBPATH:$(SDK_COFF_X64)dshow >> conf.inc
echo SUBSYSTEM=WINDOWS >> conf.inc
echo RFLAGS=$(RFLAGS_WIN32) >> conf.inc
echo MFLAGS=$(MFLAGS_WIN32) >> conf.inc
echo CFLAGS=$(CFLAGS_WIN32) >> conf.inc
echo AFLAGS=$(AFLAGS_WIN32) >> conf.inc
echo LFLAGS=$(LFLAGS_WIN32) >> conf.inc
echo ALL_OBJS=$(MSC64_WIN32_X64_OBJS) >> conf.inc
echo ALL_CPPS=$(MSC64_WIN32_X64_CPPS) >> conf.inc
call "%VS80COMNTOOLS%..\..\VC\vcvarsall.bat" x86_amd64
nmake /NOLOGO /Fmsc64\win32\x64\core.mak
nmake /NOLOGO /Fmsc64\win32\x64\gfx2.mak
nmake /NOLOGO /Fmsc64\win32\x64\gfx3.mak
nmake /NOLOGO /Fmsc64\win32\x64\math.mak
nmake /NOLOGO /Fmsc64\win32\x64\fmtz.mak
nmake /NOLOGO /Fmsc64\win32\x64\util.mak
move *.dll ..\bin\vc2005_win32_x64
move *.lib ..\bin\vc2005_win32_x64
del conf.inc

echo R_ADDONS= > conf.inc
echo C_ADDONS=/D "ARM" /D "_ARM_" /I "..\inc\l3rd\dxsdk\wm50" /wd4819 /QRarch4 /D "_WIN32_WCE=0x420" >> conf.inc
echo A_ADDONS= >> conf.inc
echo L_ADDONS=/MACHINE:ARM /ARMPADCODE /LIBPATH:$(SDK_COFF_ARM)dxsdk >> conf.inc
echo SUBSYSTEM=WINDOWSCE,4.20 >> conf.inc
echo RFLAGS=$(RFLAGS_WINCE) >> conf.inc
echo CFLAGS=$(CFLAGS_WINCE) >> conf.inc
echo AFLAGS=$(AFLAGS_WINCE) >> conf.inc
echo LFLAGS=$(LFLAGS_WINCE) >> conf.inc
echo ALL_OBJS=$(MSC64_WINCE_ALL_OBJS) >> conf.inc
echo ALL_CPPS=$(MSC64_WINCE_ALL_CPPS) >> conf.inc
call "..\bat\vc2005_ppc2k3_armv4.bat"
nmake /NOLOGO /Fmsc64\wince\all\core.mak
nmake /NOLOGO /Fmsc64\wince\all\gfx2.mak
nmake /NOLOGO /Fmsc64\wince\all\gfx3.mak
nmake /NOLOGO /Fmsc64\wince\all\math.mak
nmake /NOLOGO /Fmsc64\wince\all\fmtz.mak
nmake /NOLOGO /Fmsc64\wince\all\util.mak
move *.dll ..\bin\vc2005_wince_arm
move *.lib ..\bin\vc2005_wince_arm
del conf.inc
