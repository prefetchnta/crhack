@echo off
cd ..\mak
call ndk-gcc.bat arm-linux-androideabi 4.9 14 arm
move *.a ..\bin\gcc_android_old\armeabi

call ndk-gcc.bat arm-linux-androideabi 4.9 14 arm "-march=armv7-a"
move *.a ..\bin\gcc_android_old\armeabi-v7a

call ndk-gcc.bat mipsel-linux-android 4.9 14 mips
move *.a ..\bin\gcc_android_old\mips

call ndk-gcc.bat x86 4.9 14 x86
move *.a ..\bin\gcc_android_old\x86
cd ..\usr
