#!/bin/sh
cd ../mak
./linux.sh -march=armv7-a -Wl,--no-wchar-size-warning arm-fsl-linux-gnueabi-
cd ../usr
