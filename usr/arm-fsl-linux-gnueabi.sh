#!/bin/sh
cd ../mak
PATH=/usr/local/fsl-linaro-toolchain/bin:$PATH
./linux.sh -march=armv7-a -Wl,--no-wchar-size-warning arm-fsl-linux-gnueabi-
cd ../usr
