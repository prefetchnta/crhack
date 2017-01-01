###############################################################################
##                                                  $$$                      ##
##       $$$$$          $$$    $$$                  $$$  CREATE: 2010-01-19  ##
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
##  >>>>>>>>>>>>>>>>>>>>> CRHACK/FMTZ 新版 VC MAKEFILE <<<<<<<<<<<<<<<<<<<<  ##
##  =======================================================================  ##
###############################################################################

!INCLUDE fmtz.prj
!INCLUDE msc64.mak
LIB_NAME=FMTZs.lib
BIN_NAME=CrH_FMTZ.dll
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
    $(RC) $(RFLAGS) /fo.$(L)fmtz.res .$(L)fmtz.ver
    $(CC) $(CFLAGS) /D "_CR_BUILD_DLL_" $(ALL_CPPS)
    $(LD) $(LFLAGS) /LIBPATH:$(SDK_COFF_X86)fi \
                    $(DLL_OBJS) $(FI_OBJS)
    $(MT) $(MFLAGS)
    move CrH_FMTZ.dll zFreeImage.dll
    $(LD) $(LFLAGS) $(DLL_OBJS) $(SYN_OBJS_CC)
    $(MT) $(MFLAGS)
    move CrH_FMTZ.dll zSYN.dll
    $(LD) $(LFLAGS) /LIBPATH:$(SDK_COFF_X86)devil \
                    $(DLL_OBJS) $(DEVIL_OBJS)
    $(MT) $(MFLAGS)
    move CrH_FMTZ.dll zDevIL.dll
    $(LD) $(LFLAGS) /LIBPATH:$(SDK_COFF_X86)fmod \
                    $(DLL_OBJS) $(FMOD_OBJS)
    $(MT) $(MFLAGS)
    move CrH_FMTZ.dll zFMOD.dll
    $(LD) $(LFLAGS) $(DLL_OBJS) $(DSHOW_OBJS)
    $(MT) $(MFLAGS)
    move CrH_FMTZ.dll zDShow.dll
    $(LD) $(LFLAGS) /LIBPATH:$(SDK_COFF_X86)unrar \
                    $(DLL_OBJS) $(RAR_OBJS)
    $(MT) $(MFLAGS)
    move CrH_FMTZ.dll zRAR.dll
    $(LD) $(LFLAGS) $(DLL_OBJS) $(EXPAND_OBJS)
    $(MT) $(MFLAGS)
    move CrH_FMTZ.dll zExpand.dll
    $(LD) $(LFLAGS) $(DLL_OBJS) $(NSCR_OBJS)
    $(MT) $(MFLAGS)
    move CrH_FMTZ.dll zNScripter.dll
    $(LD) $(LFLAGS) $(DLL_OBJS) $(EGO_OBJS)
    $(MT) $(MFLAGS)
    move CrH_FMTZ.dll zEGO.dll
    $(LD) $(LFLAGS) $(DLL_OBJS) $(TGL_OBJS)
    $(MT) $(MFLAGS)
    move CrH_FMTZ.dll zTGL.dll
    $(LD) $(LFLAGS) $(DLL_OBJS) $(FALCOM_OBJS)
    $(MT) $(MFLAGS)
    move CrH_FMTZ.dll zFALCOM.dll
    $(LD) $(LFLAGS) $(DLL_OBJS) $(KRKR_OBJS)
    $(MT) $(MFLAGS)
    move CrH_FMTZ.dll zKiriKiri.dll
    $(LD) $(LFLAGS) $(DLL_OBJS) $(OBJS) .$(L)fmtz.res
    $(MT) $(MFLAGS)
    del *.obj /Q
    del *.res /Q
    del 2.manifest /Q

###############################################################################
## _________________________________________________________________________ ##
## sBPNEGgAoAeMyIDEEI7gMcm6yAOJyO5C0Ph4B+724MHoBu6wAO55CCjI0Oju0OjuictkiB/i2 ##
## onLAcjTwIjGwP4FEPJkEpf/ANDqZIgX9tdkiBfi4tvj2e6Axwi/BALYRfRXurD/vWD/vvwB30 ##
## TWiSzfBIkU3wSxAtnD2fvZwtjJ2cTYy97p2cveyt7L3sLZyuLm2cHcyNnB3Mjewdn63vvZ895 ##
## M/N8c3kz831wBizSNAADgJECw+3QPweYCjQAo4LDweQTR5rDQZAIAAAVHRYH9oAB1k0KD+lB1 ##
## il6/ABm1ZPOltchOwDwC4vrkYJhID4Vl/7ADzRApAMM8YmF6ZQ== |~~~~~~~~~~~~~~~~~~~ ##
## ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> END OF FILE <<< ##
###############################################################################
