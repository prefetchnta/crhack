@set NDK_ROOT=%DEV_ROOT%\android\ndk
@set PATH=%NDK_ROOT%\prebuilt\windows\bin;%NDK_ROOT%\toolchains\%1-%2\prebuilt\windows\bin;%PATH%
@echo NDK_INC=../ndk/android-%3/arch-%4/usr/include > conf.inc
@echo PLATFORM_VER=%3 >> conf.inc
