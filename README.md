
## 【构 建】
Command | Platform
------- | --------
mak/bcb60.bat | Borland C++ Builder 6.0 (Win32-X86)
mak/bcb2006.bat | Borland Developer Studio 2006 (Win32-X86)
mak/bcb2007.bat | CodeGear RAD Studio 2007 (Win32-X86)
mak/bcb2009.bat | CodeGear RAD Studio 2009 (Win32-X86)
mak/bcb2010.bat | Embarcadero RAD Studio 2010 (Win32-X86)
mak/ccblkfn50.bat | ADI Visual DSP++ 5.0 (Naked-Blackfin) [base]
mak/mdk40_16.bat | Keil MDK 4.0 ARMCC (Naked-THUMB) [base]
mak/mdk40_32.bat | Keil MDK 4.0 ARMCC (Naked-ARM32) [base]
mak/mdk50_16.bat | Keil MDK 5.0 ARMCC (Naked-THUMB) [base]
mak/mdk50_32.bat | Keil MDK 5.0 ARMCC (Naked-ARM32) [base]
mak/ndk-gcc.bat | Android NDK GCC (Android-ALL) [base]
mak/mingw32.bat | MinGW32 (Win32-X86)
mak/mingw64.bat | MinGW64 (Win32-X64)
mak/vc2002.bat | Microsoft Visual Studio 2002 (Win32-X86)
mak/vc2003.bat | Microsoft Visual Studio 2003 (Win32-X86)
mak/vc2005.bat | Microsoft Visual Studio 2005 (Win32-X86-X64, WinCE-ARMv4)
mak/vc2008.bat | Microsoft Visual Studio 2008 (Win32-X86-X64, WinCE-ARMv4)
mak/vc2010.bat | Microsoft Visual Studio 2010 (Win32-X86-X64)
mak/vc2012.bat | Microsoft Visual Studio 2012 (Win32-X86-X64)
mak/vc2013.bat | Microsoft Visual Studio 2013 (Win32-X86-X64)
mak/vc2015.bat | Microsoft Visual Studio 2015 (Win32-X86-X64)
mak/vc2017.bat | Microsoft Visual Studio 2017 (Win32-X86-X64)
mak/vc2019.bat | Microsoft Visual Studio 2019 (Win32-X86-X64)
mak/linux.sh | GCC (Linux-ALL) [base]
mak/naked.bat | GCC (Naked-ALL) [base]
usr/linux-arm32.sh | GCC (Linux for ARM32 non cross-compilation)
usr/arm-fsl-linux-gnueabi.sh | GCC (Linux for Freescale i.MX6)
usr/arm-linux-gnueabihf.sh | GCC (Linux ARM32 Hard Float ABI)
usr/n3292x.sh | GCC (Linux for Nuvoton N3292x)
usr/ndk-gcc-api14.bat | Android NDK GCC 4.9 (API-14)
usr/ndk-gcc-api24.bat | Android NDK GCC 4.9 (API-24)
usr/armcc-cm0p-small-ram.bat | Keil MDK 5.0 ARMCC (ARM Cortex-M0+ Small RAM)
usr/armcc-cm3-big-ram.bat | Keil MDK 5.0 ARMCC (ARM Cortex-M3 Big RAM)
usr/armcc-cm4f-big-ram.bat | Keil MDK 5.0 ARMCC (ARM Cortex-M4F-SP Big RAM)
usr/armcc-cm7f-big-ram.bat | Keil MDK 5.0 ARMCC (ARM Cortex-M7F-DP Big RAM)
usr/arm-none-eabi-cm0p-small-ram.bat | GCC (ARM Cortex-M0+ Small RAM)
usr/arm-none-eabi-cm3-big-ram.bat | GCC (ARM Cortex-M3 Big RAM)
usr/arm-none-eabi-cm4f-big-ram.bat | GCC (ARM Cortex-M4F-SP Big RAM)

## 【废 话】
　　这是一个近乎脑残的项目，试图用一套 **C** 代码支持所有的编译器平台。结果发现，维护这么一套代码的精力投入，要远大于分开独立的项目。可见，真正的跨平台就是浮云。纯粹用 **C** 来写所有东西，而且要照顾到很多变态的编译器，也是一件十分痛苦的事（有些东西根本不该包括进来的也包括进来了）。所以这个是一个失败的项目。但木已成舟，这个项目还将继续，不过不会是主要的项目了。走出自己给自己设下的笼子，是很重要的（从一个笼子换到另一个笼子，人生就是如此）。
