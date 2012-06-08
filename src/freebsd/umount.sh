#!/bin/sh 
#
# Script by Sergey V. Dyatko [ sergey.dyatko@gmail.com ]
# License: GPL v3

UMOUNT="/sbin/umount"
SUDO=`which sudo`
MOUNT_POINT=$1
DEVNAME=`mount | grep ${MOUNT_POINT} | awk '{print $1}')`
MDCONFIG_BIN="$(which mdconfig 2>&1)"

if [ $# -lt 1 ]; then
        echo "Usage: $0 MOUNT_POINT"
        exit 255;
fi 

if [ ! -d ${MOUNT_POINT} ]; then
    echo "Error: Cannot get mount point directory."
    exit 255
fi

if [ ! -x "${SUDO}" ]; then
    echo "Error: Cannot find sudo binary. Is it accessible by PATH variable?"
    exit 255
fi

if [ ! -x "${MDCONFIG_BIN}" ]; then
    echo "Error: Cannot find mdconfig binary. Is it accessible by PATH variable?"
    exit 255
fi

#Now it's time to umount device
${SUDO} ${UMOUNT} ${MOUNT_POINT}
if [ $? -ne 0 ]; then
    echo "Error: \"${UMOUNT} ${MOUNT_POINT}\" returns an a error."
    exit 255
fi

#Now it's time to get: Is it md device or not? If yes -- use mdconfig to remove it.

case "${DEVNAME}" in
    (*md*) ${SUDO} ${MDCONFIG_BIN} -d -u${DEVNAME};
            if [ $? -ne 0 ]; then
                echo "Error: \"mdconfig -d -u${DEVNAME}\" returns an a error."
                exit 255
            fi;;
     (*)        echo "${DEVNAME} does not match"
    		exit 255;;
esac
 
exit 0
