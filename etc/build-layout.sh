#!/bin/bash
set -e

# Script globals
ROOT=`dirname $0`/..
SRC=$ROOT/src
PKG=$ROOT/packages
ETC=$ROOT/etc
OUT=$ROOT/out
OUT_BIN=$OUT/bin
OUT_ISO=$OUT/iso

# Build layout
echo "Building layout..."
mkdir -p $OUT_ISO/boot/grub
cp $ETC/menu.lst $OUT_ISO/boot/grub/
cp $PKG/grubstage2/stage2_eltorito $OUT_ISO/boot/grub/
cp $OUT_BIN/kernel $OUT_ISO/boot/
