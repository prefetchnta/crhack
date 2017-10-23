#!/bin/sh
cd ../mak
./linux.sh "-D_GNU_SOURCE=1 -D_CR_NO_FAST_ATOM_ -D_CR_USE_SYNC_LOCK_RELEASE_ -DLUA_C89_NUMBERS" -lgcc arm-linux-
cd ../usr
