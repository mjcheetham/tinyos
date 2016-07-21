#!/bin/bash
set -e

# Script globals
ROOT="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"/..
OUT=$ROOT/out
OUT_ISO=$OUT/iso

# Build ISO image
genisoimage -R \
            -b boot/grub/stage2_eltorito \
            -no-emul-boot \
            -boot-load-size 4 \
            -A os \
            -input-charset utf8 \
            -quiet \
            -boot-info-table \
            -o $OUT/os.iso \
            $OUT_ISO
