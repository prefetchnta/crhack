@echo off
echo R_ADDONS= > conf.inc
echo C_ADDONS=/GS /GL /Wp64 /arch:SSE /Zc:forScope >> conf.inc
echo A_ADDONS=/LTCG >> conf.inc
echo L_ADDONS=/LTCG >> conf.inc
echo SUBSYSTEM=WINDOWS >> conf.inc
echo ALL_OBJS=$(MSC32_WIN32_X86_OBJS) >> conf.inc
echo ALL_CPPS=$(MSC32_WIN32_X86_CPPS) >> conf.inc
call "%VS71COMNTOOLS%vsvars32.bat"
nmake /NOLOGO /Fmsc32\win32\x86\core.mak
nmake /NOLOGO /Fmsc32\win32\x86\gfx2.mak
nmake /NOLOGO /Fmsc32\win32\x86\gfx3.mak
nmake /NOLOGO /Fmsc32\win32\x86\math.mak
nmake /NOLOGO /Fmsc32\win32\x86\fmtz.mak
nmake /NOLOGO /Fmsc32\win32\x86\util.mak
move *.dll ..\bin\vc2003_win32_x86
move *.lib ..\bin\vc2003_win32_x86
del conf.inc
