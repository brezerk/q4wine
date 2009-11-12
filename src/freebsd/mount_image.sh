#!/bin/sh
#
# Script by Sergey V. Dyatko [ sergey.dyatko@gmail.com ]
# License: GPL v3

SUDO="/usr/local/bin/sudo"
MOUNT_BIN="mount_cd9660"
MDCONFIG_BIN="/sbin/mdconfig"
MOUNT_IMAGE=$1
MOUNT_POINT=$2
if [ ! -x $SUDO ]; then
	echo "Error: sudo isn't installed or not executable"
	exit 255
fi

$SUDO $MOUNT_BIN /dev/`$SUDO $MDCONFIG_BIN -f $MOUNT_IMAGE` $MOUNT_POINT

if [ $? -ne 0 ]; then 
	echo "Some Errors occurred:( command: $SUDO $MOUNT_BIN /dev/`$SUDO $MDCONFIG_BIN -f $MOUNT_IMAGE` $MOUNT_POINT"
	exit 255
fi

exit 0
