#!/bin/bash
set -e

# Script globals
ROOT=`dirname $0`/..
PKG=$ROOT/packages
ETC=$ROOT/etc
PKGROOT="$PKG/gnubinutils"
TARFILE="$PKGROOT/binutils-2.26.tar.gz"
GEN="$PKGROOT/gen"
OUT="$PKGROOT/out"
LD="$OUT/bin/i386-elf-ld"

# GNU binutils compilation options
TARGET=i386-elf

main()
{
	if [[ -e $LD ]]
	then
		echo "ELF linker found at $LD"
		exit 0
	fi
	echo "No ELF linker found. Building linker from source..."

	if [[ ! -e $TARFILE ]]
	then
		echo "Missing linker sources $TARFILE. Did all packages restore successfully?"
		exit 1
	fi

	if [[ ! -d "$PKGROOT/binutils-2.26" ]]
	then
		echo "Extracting linker sources from $TARFILE..."
		tar -xf $TARFILE --directory $PKGROOT
	fi

	mkdir -p $OUT
	prefix=`cd $OUT && pwd`

	mkdir -p $GEN
	pushd $GEN > /dev/null

	echo "Configuring compilation..."
	echo -e "\ttarget:\t$TARGET"
	echo -e "\tprefix:\t$prefix"
	../binutils-2.26/configure --target=$TARGET --enable-interwork --enable-multilib --disable-nls --disable-werror --prefix=$prefix > configure.log

	echo "Compiling linker..."
	make all install > make.log

	popd > /dev/null
}

main
