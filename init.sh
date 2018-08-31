#!/bin/bash
set -e

# Script globals
ETC=etc

# script main
platform=$( uname -s )
echo "Platform is $platform"

# Platform independent scripts
$ETC/restore-packages.sh "$ETC/pkg.cfg"

# Platform specific scripts
case $platform in
[Dd]arwin)
	$ETC/restore-packages.sh "$ETC/pkg.darwin.cfg"
	$ETC/build-linker.sh
	;;
[Ll]inux)
	:
	;;
esac
