#!/bin/sh
cd ../mak
./linux.sh -D_CRHACK_ARM32=1 -Wl,--no-wchar-size-warning
cd ../usr
