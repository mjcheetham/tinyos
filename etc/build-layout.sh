#!/bin/bash
set -e

# Script globals
ROOT="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"/..
SRC=$ROOT/src
PKG=$ROOT/packages
ETC=$ROOT/etc
OUT=$ROOT/out
OUT_BIN=$OUT/bin
OUT_ISO=$OUT/iso

# Build layout
mkdir -p $OUT_ISO/boot/grub
cp $SRC/boot/menu.lst $OUT_ISO/boot/grub/
cp $PKG/grubstage2/stage2_eltorito $OUT_ISO/boot/grub/
cp $OUT_BIN/kernel.elf $OUT_ISO/boot/
