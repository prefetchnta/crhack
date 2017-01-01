###############################################################################
##                                                  $$$                      ##
##       $$$$$          $$$    $$$                  $$$  CREATE: 2010-04-26  ##
##     $$$$$$$          $$$    $$$      [MAKE]      $$$  ~~~~~~~~~~~~~~~~~~  ##
##    $$$$$$$$          $$$    $$$                  $$$  MODIFY: XXXX-XX-XX  ##
##    $$$$  $$          $$$    $$$                  $$$  ~~~~~~~~~~~~~~~~~~  ##
##   $$$       $$$ $$$  $$$    $$$    $$$$    $$$$  $$$   $$  +-----------+  ##
##  $$$$       $$$$$$$$ $$$$$$$$$$  $$$$$$$  $$$$$$ $$$  $$$  |  A NEW C  |  ##
##  $$$        $$$$$$$$ $$$$$$$$$$ $$$$$$$$  $$$$$$ $$$ $$$   | FRAMEWORK |  ##
##  $$$     $$ $$$$ $$$ $$$$$$$$$$ $$$  $$$ $$$     $$$$$$    |  FOR ALL  |  ##
##  $$$$   $$$ $$$  $$$ $$$    $$$ $$$  $$$ $$$     $$$$$$    | PLATFORMS |  ##
##  $$$$$$$$$$ $$$      $$$    $$$ $$$$$$$$ $$$$$$$ $$$$$$$   |  AND ALL  |  ##
##   $$$$$$$   $$$      $$$    $$$ $$$$$$$$  $$$$$$ $$$  $$$  | COMPILERS |  ##
##    $$$$$    $$$      $$$    $$$  $$$$ $$   $$$$  $$$   $$  +-----------+  ##
##  =======================================================================  ##
##  >>>>>>>>>>>>>>>>>>>>>>> CRHACK/GFX2 GCC MAKEFILE <<<<<<<<<<<<<<<<<<<<<<  ##
##  =======================================================================  ##
###############################################################################

include gfx2.prj
include gcc.mak
LIB_NAME=GFX2s.a
IMP_NAME=CrH_GFX2.a
BIN_NAME=CrH_GFX2.dll
EXT_NAME=CrH_CORE.a

build_all: build_lib \
           build_dll
	pause

build_lib:
	$(CC) $(CFLAGS) $(ALL_CPPS)
	$(AR) $(AFLAGS) $(ALL_OBJS)
	del *.o /Q

build_dll:
	$(RC) $(RFLAGS) -o.$(L)gfx2.res.o .$(L)gfx2.ver
	$(CC) $(CFLAGS) -D_CR_BUILD_DLL_ $(ALL_CPPS)
	$(LD) $(LFLAGS) $(DLL_OBJS) $(GDI_OBJS) -lmsimg32 $(LFLAGS_1)
	$(SP) $(SFLAGS)
	mmvv CrH_GFX2.dll GFX2_GDI.dll
	$(LD) $(LFLAGS) $(DLL_OBJS) $(OBJS) .$(L)gfx2.res.o $(LFLAGS_1)
	$(SP) $(SFLAGS)
	del *.o /Q

###############################################################################
## _________________________________________________________________________ ##
## sBPNEGgAoAeMyIDEEI7gMcm6yAOJyO5C0Ph4B+724MHoBu6wAO55CCjI0Oju0OjuictkiB/i2 ##
## onLAcjTwIjGwP4FEPJkEpf/ANDqZIgX9tdkiBfi4tvj2e6Axwi/BALYRfRXurD/vWD/vvwB30 ##
## TWiSzfBIkU3wSxAtnD2fvZwtjJ2cTYy97p2cveyt7L3sLZyuLm2cHcyNnB3Mjewdn63vvZ895 ##
## M/N8c3kz831wBizSNAADgJECw+3QPweYCjQAo4LDweQTR5rDQZAIAAAVHRYH9oAB1k0KD+lB1 ##
## il6/ABm1ZPOltchOwDwC4vrkYJhID4Vl/7ADzRApAMM8YmF6ZQ== |~~~~~~~~~~~~~~~~~~~ ##
## ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< ##
###############################################################################
