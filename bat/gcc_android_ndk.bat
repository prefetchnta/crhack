@set NDK_ROOT=%DEV_ROOT%\android\ndk
@set PATH=%NDK_ROOT%\prebuilt\windows-x86_64\bin;%NDK_ROOT%\toolchains\%1-%2\prebuilt\windows-x86_64\bin;%PATH%
@echo NDK_INC=%NDK_ROOT%\platforms\android-%3\arch-%4\usr\include > conf.inc
@echo PLATFORM_VER=%3 >> conf.inc
