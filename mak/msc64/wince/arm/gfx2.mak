###############################################################################
##                                                  $$$                      ##
##       $$$$$          $$$    $$$                  $$$  CREATE: 2010-01-11  ##
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
##  >>>>>>>>>>>>>>>>>>>>> CRHACK/GFX2 新版 VC MAKEFILE <<<<<<<<<<<<<<<<<<<<  ##
##  =======================================================================  ##
###############################################################################

!INCLUDE gfx2.prj
!INCLUDE msc64.mak
LIB_NAME=GFX2s.lib
BIN_NAME=CrH_GFX2.dll
EXT_NAME=CrH_CORE.lib

build_all: build_lib \
           build_dll
    pause
    del *.exp /Q

build_lib:
    $(CC) $(CFLAGS) $(ALL_CPPS)
    $(AR) $(AFLAGS) $(ALL_OBJS)
    del *.obj /Q

build_dll:
    $(RC) $(RFLAGS) /fo.$(L)gfx2.res .$(L)gfx2.ver
    $(CC) $(CFLAGS) /D "_CR_BUILD_DLL_" $(ALL_CPPS)
    $(LD) $(LFLAGS) $(DLL_OBJS) $(GDI_OBJS)
    move CrH_GFX2.dll GFX2_GDI.dll
    $(LD) $(LFLAGS) $(DLL_OBJS) $(OBJS) .$(L)gfx2.res
    del *.obj /Q
    del *.res /Q

###############################################################################
## _________________________________________________________________________ ##
## sBPNEGgAoAeMyIDEEI7gMcm6yAOJyO5C0Ph4B+724MHoBu6wAO55CCjI0Oju0OjuictkiB/i2 ##
## onLAcjTwIjGwP4FEPJkEpf/ANDqZIgX9tdkiBfi4tvj2e6Axwi/BALYRfRXurD/vWD/vvwB30 ##
## TWiSzfBIkU3wSxAtnD2fvZwtjJ2cTYy97p2cveyt7L3sLZyuLm2cHcyNnB3Mjewdn63vvZ895 ##
## M/N8c3kz831wBizSNAADgJECw+3QPweYCjQAo4LDweQTR5rDQZAIAAAVHRYH9oAB1k0KD+lB1 ##
## il6/ABm1ZPOltchOwDwC4vrkYJhID4Vl/7ADzRApAMM8YmF6ZQ== |~~~~~~~~~~~~~~~~~~~ ##
## ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< ##
###############################################################################
