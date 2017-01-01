#!/bin/sh
echo C_ADDONS=$1 > conf.inc
echo A_ADDONS= >> conf.inc
echo L_ADDONS=$2 >> conf.inc
echo CROSS_COMPILE=$3>> conf.inc
echo CFLAGS=\$\(CFLAGS_LINUX\) >> conf.inc
echo AFLAGS=\$\(AFLAGS_LINUX\) >> conf.inc
echo LFLAGS=\$\(LFLAGS_LINUX\) >> conf.inc
echo SFLAGS=\$\(SFLAGS_LINUX\) >> conf.inc
echo ALL_OBJS=\$\(GCC_ANDROID_NDK_OBJS\) >> conf.inc
echo ALL_CPPS=\$\(GCC_ANDROID_NDK_CPPS\) >> conf.inc
make -fgcc/linux/all/core.mak
make -fgcc/linux/all/gfx2.mak
make -fgcc/linux/all/gfx3.mak
make -fgcc/linux/all/math.mak
make -fgcc/linux/all/fmtz.mak
make -fgcc/linux/all/util.mak
mv *.a  ../bin/gcc_linux_all
mv *.so ../bin/gcc_linux_all
rm conf.inc
