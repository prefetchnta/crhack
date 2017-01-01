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
##  >>>>>>>>>>>>>>>>>>>>>>> CRHACK/FMTZ GCC MAKEFILE <<<<<<<<<<<<<<<<<<<<<<  ##
##  =======================================================================  ##
###############################################################################

include fmtz.prj
include gcc.mak
LIB_NAME=FMTZs.a
IMP_NAME=CrH_FMTZ.a
BIN_NAME=CrH_FMTZ.dll
EXT_NAME=CrH_CORE.a

build_all: build_lib \
           build_dll
	pause

build_lib:
	$(CC) $(CFLAGS) $(ALL_CPPS)
	$(AR) $(AFLAGS) $(ALL_OBJS)
	del *.o /Q

build_dll:
	$(RC) $(RFLAGS) -o.$(L)fmtz.res.o .$(L)fmtz.ver
	$(CC) $(CFLAGS) -D_CR_BUILD_DLL_ $(ALL_CPPS)
	$(LD) $(LFLAGS) $(DLL_OBJS) $(FI_OBJS) \
          $(SDK_COFF_X86)fi$(L)FreeImage.lib $(LFLAGS_1)
	$(SP) $(SFLAGS)
	mmvv CrH_FMTZ.dll zFreeImage.dll
	$(LD) $(LFLAGS) $(DLL_OBJS) $(SYN_OBJS_CC) $(LFLAGS_1)
	$(SP) $(SFLAGS)
	mmvv CrH_FMTZ.dll zSYN.dll
	$(LD) $(LFLAGS) $(DLL_OBJS) $(DEVIL_OBJS) \
          $(SDK_COFF_X86)devil$(L)ResIL.lib $(LFLAGS_1)
	$(SP) $(SFLAGS)
	mmvv CrH_FMTZ.dll zDevIL.dll
	$(LD) $(LFLAGS) $(DLL_OBJS) $(FMOD_OBJS) \
          $(SDK_COFF_X86)fmod$(L)libfmodex.a $(LFLAGS_1)
	$(SP) $(SFLAGS)
	mmvv CrH_FMTZ.dll zFMOD.dll
	$(LD) $(LFLAGS) $(DLL_OBJS) $(RAR_OBJS) \
          $(SDK_COFF_X86)unrar$(L)unrar.lib $(LFLAGS_1)
	$(SP) $(SFLAGS)
	mmvv CrH_FMTZ.dll zRAR.dll
	$(LD) $(LFLAGS) $(DLL_OBJS) $(EXPAND_OBJS) $(LFLAGS_1)
	$(SP) $(SFLAGS)
	mmvv CrH_FMTZ.dll zExpand.dll
	$(LD) $(LFLAGS) $(DLL_OBJS) $(NSCR_OBJS) $(LFLAGS_1)
	$(SP) $(SFLAGS)
	mmvv CrH_FMTZ.dll zNScripter.dll
	$(LD) $(LFLAGS) $(DLL_OBJS) $(EGO_OBJS) $(LFLAGS_1)
	$(SP) $(SFLAGS)
	mmvv CrH_FMTZ.dll zEGO.dll
	$(LD) $(LFLAGS) $(DLL_OBJS) $(TGL_OBJS) $(LFLAGS_1)
	$(SP) $(SFLAGS)
	mmvv CrH_FMTZ.dll zTGL.dll
	$(LD) $(LFLAGS) $(DLL_OBJS) $(FALCOM_OBJS) $(LFLAGS_1)
	$(SP) $(SFLAGS)
	mmvv CrH_FMTZ.dll zFALCOM.dll
	$(LD) $(LFLAGS) $(DLL_OBJS) $(KRKR_OBJS) $(LFLAGS_1)
	$(SP) $(SFLAGS)
	mmvv CrH_FMTZ.dll zKiriKiri.dll
	$(LD) $(LFLAGS) $(DLL_OBJS) $(OBJS) .$(L)fmtz.res.o $(LFLAGS_1)
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
