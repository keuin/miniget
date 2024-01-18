#!/bin/bash

OUTPUT_FILE='a.out'
CC_FLAGS='-s -Os -nostdlib -nolibc -ffreestanding -fno-ident -fno-asynchronous-unwind-tables -Wno-builtin-declaration-mismatch -Wno-implicit-function-declaration'

gcc $CC_FLAGS -o "$OUTPUT_FILE" main.S miniget.c

[ -f "$OUTPUT_FILE" ] && upx -9 "$OUTPUT_FILE"