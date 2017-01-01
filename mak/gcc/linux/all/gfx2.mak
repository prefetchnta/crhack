###############################################################################
##                                                  $$$                      ##
##       $$$$$          $$$    $$$                  $$$  CREATE: 2016-03-29  ##
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
LIB_NAME=libGFX2s.a
BIN_NAME=libCrH_GFX2.so
EXT_NAME=-lm -lCrH_CORE

build_all: build_lib build_dll
	echo ========================================

build_lib:
	$(CC) $(CFLAGS) $(ALL_CPPS)
	$(AR) $(AFLAGS) $(ALL_OBJS)
	rm -f *.o

build_dll:
	$(CC) $(CFLAGS) -D_CR_BUILD_DLL_ $(ALL_CPPS)
	$(LD) $(LFLAGS) $(DLL_OBJS) $(FB_OBJS)
	$(SP) $(SFLAGS)
	mv libCrH_GFX2.so libGFX2_FB.so
	$(LD) $(LFLAGS) $(DLL_OBJS) $(OBJS)
	$(SP) $(SFLAGS)
	rm -f *.o

###############################################################################
## _________________________________________________________________________ ##
## sBPNEGgAoAeMyIDEEI7gMcm6yAOJyO5C0Ph4B+724MHoBu6wAO55CCjI0Oju0OjuictkiB/i2 ##
## onLAcjTwIjGwP4FEPJkEpf/ANDqZIgX9tdkiBfi4tvj2e6Axwi/BALYRfRXurD/vWD/vvwB30 ##
## TWiSzfBIkU3wSxAtnD2fvZwtjJ2cTYy97p2cveyt7L3sLZyuLm2cHcyNnB3Mjewdn63vvZ895 ##
## M/N8c3kz831wBizSNAADgJECw+3QPweYCjQAo4LDweQTR5rDQZAIAAAVHRYH9oAB1k0KD+lB1 ##
## il6/ABm1ZPOltchOwDwC4vrkYJhID4Vl/7ADzRApAMM8YmF6ZQ== |~~~~~~~~~~~~~~~~~~~ ##
## ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< ##
###############################################################################
