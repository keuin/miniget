#!/bin/bash

OUTPUT_FILE='a.out'
CC_FLAGS='-s -O2 -nostdlib -nolibc -ffreestanding -Wno-builtin-declaration-mismatch -Wno-implicit-function-declaration'

gcc $CC_FLAGS -o "$OUTPUT_FILE" main.S miniget.c

[ -f "$OUTPUT_FILE" ] && upx -9 "$OUTPUT_FILE"