#!/bin/bash
set -e

# Useful control/escape sequences
CON_RMLINE="\033[0G\033[2K"
CON_FG_NOCOLOR="\033[0m"
CON_FG_RED="\033[0;31m"
CON_FG_CYAN="\033[0;36m"
CON_FG_LGREEN="\033[1;32m"

# Script globals
ROOT=`dirname $0`/..
PKG=$ROOT/packages
ETC=$ROOT/etc
PKG_DEF_FILE=$ETC/pkg.cfg
PKG_DEF_FILE_DARWIN=$ETC/pkg.darwin.cfg
PKG_DEF_FILE_LINUX=$ETC/pkg.linux.cfg

# Description
# 	Download a package of a given from a URL using wget
# Parameters
# 	$1 : package name
# 	$2 : URL of the package
# Returns
#	true on success, false otherwise
download_package()
{
	name=$1
	url=$2
	pkgDir="$PKG/$name/"

	# Clean up any old files
	rm -rf $pkgDir

	mkdir $pkgDir
	pushd $pkgDir > /dev/null

	# Download from URL
	curl -sL $url -O

	popd > /dev/null

	return $?
}

# Description
#	Check if the package with the given name exists in the local cache
# Parameters
#	$1 : package name
# Returns
#	true if the package exists in the cache, false otherwise
package_exists()
{
	name=$1
	pkgDir="$PKG/$name/"

	if [[ -e $pkgDir ]]
	then
		return 0
	fi

	return 1
}

restore_packages_from_config()
{
	pkgdef=$1

	while IFS= read -r line
	do
		name=$( echo $line | awk -F'=' '{print $1}' )
		url=$( echo $line | awk -F'=' '{print $2}' )
		if package_exists $name
		then
			echo -e " $name [${CON_FG_CYAN}SKIPPED${CON_FG_NOCOLOR}]"
		else
			echo -n -e " $name [${CON_FG_CYAN}RESTORING...${CON_FG_NOCOLOR}]"
			if download_package $name $url
			then
				echo -e "${CON_RMLINE} $name [${CON_FG_LGREEN}OK${CON_FG_NOCOLOR}]"
			else
				echo -e "${CON_RMLINE} $name [${CON_FG_RED}FAILED${CON_FG_NOCOLOR}]"
			fi
		fi
	done < $pkgdef

}

# script main
if [ $# -ne 1 ]
then
	echo "Must specify external package configuration file!"
	exit 1
fi

cfg=$1

if [ ! -e $cfg ]
then
	echo "$cfg does not exist!"
	exit 1
fi

echo "Restoring external packages from $cfg..."
mkdir -p $PKG

restore_packages_from_config $cfg

echo "Restore complete."
