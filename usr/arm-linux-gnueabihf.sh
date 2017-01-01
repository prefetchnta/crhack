#!/bin/sh
cd ../mak
./linux.sh -march=armv7-a -Wl,--no-wchar-size-warning arm-linux-gnueabihf-
cd ../usr
