#!/bin/sh
cd ../mak
./linux.sh "-D_GNU_SOURCE -D_CR_NO_LINUX_I2C_ -D_CR_NO_LINUX_SPI_ -D_CR_NO_FAST_ATOM_ -D_CR_USE_SYNC_LOCK_RELEASE_ -DLUA_C89_NUMBERS" -lgcc arm-linux-
cd ../usr
