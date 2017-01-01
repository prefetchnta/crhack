@set VSINSTALLDIR=%DEV_ROOT%\Microsoft Visual Studio 9.0
@set VCINSTALLDIR=%DEV_ROOT%\Microsoft Visual Studio 9.0\VC
@set SDK_INC=%VSINSTALLDIR%\SmartDevices\SDK\PocketPC2003\Include
@set SDK_LIB=%VSINSTALLDIR%\SmartDevices\SDK\PocketPC2003\Lib\armv4
@if "%SDK_INC%"=="" goto error_no_SDK_INC
@if "%SDK_LIB%"=="" goto error_no_SDK_LIB
@if "%VSINSTALLDIR%"=="" goto error_no_VSINSTALLDIR
@if "%VCINSTALLDIR%"=="" goto error_no_VCINSTALLDIR

@echo Setting environment for using Microsoft Visual Studio 2008 ARMv4 cross tools.

@rem
@rem Root of Visual Studio IDE installed files.
@rem
@set DevEnvDir=%VSINSTALLDIR%\Common7\IDE

@set PATH=%VCINSTALLDIR%\ce\bin\x86_arm;%VCINSTALLDIR%\bin;%DevEnvDir%;%PATH%
@set INCLUDE=%SDK_INC%;%VCINSTALLDIR%\ce\atlmfc\include;%VCINSTALLDIR%\ce\include;%INCLUDE%
@set LIB=%SDK_LIB%;%VCINSTALLDIR%\ce\atlmfc\lib\armv4;%VCINSTALLDIR%\ce\lib\armv4;%LIB%
@goto end

:error_no_SDK_INC
@echo ERROR: SDK_INC variable is not set. 
@goto end

:error_no_SDK_LIB
@echo ERROR: SDK_LIB variable is not set. 
@goto end

:error_no_VSINSTALLDIR
@echo ERROR: VSINSTALLDIR variable is not set. 
@goto end

:error_no_VCINSTALLDIR
@echo ERROR: VCINSTALLDIR variable is not set. 
@goto end

:end
