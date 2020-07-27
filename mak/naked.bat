echo C_ADDONS=%~1 > conf.inc
echo A_ADDONS= >> conf.inc
echo CROSS_COMPILE=%2->> conf.inc
echo CFLAGS=$(CFLAGS_NAKED) >> conf.inc
echo AFLAGS=$(AFLAGS_NAKED) >> conf.inc
echo LFLAGS=$(LFLAGS_NAKED) >> conf.inc
echo SFLAGS=$(SFLAGS_NAKED) >> conf.inc
echo ALL_OBJS=$(GCC_NAKED_ALL_OBJS) >> conf.inc
echo ALL_CPPS=$(GCC_NAKED_ALL_CPPS) >> conf.inc
call "..\bat\gcc_naked_all.bat" %2 %3
make -fgcc/naked/all/core.mak
make -fgcc/naked/all/gfx2.mak
make -fgcc/naked/all/gfx3.mak
make -fgcc/naked/all/math.mak
make -fgcc/naked/all/fmtz.mak
make -fgcc/naked/all/util.mak
move *.a ../bin/gcc_naked_all
del conf.inc
