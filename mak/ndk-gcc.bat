@echo off
call "..\bat\gcc_android_ndk.bat" %1 %2 %3 %4
echo C_ADDONS=%5 -D_CR_NO_LIBICONV_ >> conf.inc
echo A_ADDONS= >> conf.inc
if %1==x86 (echo CROSS_COMPILE=i686-linux-android->> conf.inc
) else (if %1==x86_64 (echo CROSS_COMPILE=x86_64-linux-android->> conf.inc
) else (echo CROSS_COMPILE=%1->> conf.inc
))
echo CFLAGS=$(CFLAGS_ANDROID) >> conf.inc
echo AFLAGS=$(AFLAGS_ANDROID) >> conf.inc
echo ALL_OBJS=$(GCC_ANDROID_NDK_OBJS) >> conf.inc
echo ALL_CPPS=$(GCC_ANDROID_NDK_CPPS) >> conf.inc
make -fgcc\linux\ndk\core.mak
make -fgcc\linux\ndk\gfx2.mak
make -fgcc\linux\ndk\gfx3.mak
make -fgcc\linux\ndk\math.mak
make -fgcc\linux\ndk\fmtz.mak
make -fgcc\linux\ndk\util.mak
del conf.inc
