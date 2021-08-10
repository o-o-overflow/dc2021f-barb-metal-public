#!/bin/bash
set +e
CC="gcc -static -nostdlib -m32 -DMRBC_USE_HAL_X86 -DMRBC_NO_TIMER -I/usr/include/newlib"  MRBC_USE_HAL_X86=1 MRBC_NO_TIMER=1 make
cp src/libmrubyc.a ..
