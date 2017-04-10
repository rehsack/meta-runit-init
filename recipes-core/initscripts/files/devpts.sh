#!/bin/sh
### BEGIN INIT INFO
# Provides:          devpts
# Required-Start: udev
# Required-Stop:
# Default-Start:     S
# Default-Stop:
# Short-Description: Mount /dev/pts file systems.
### END INIT INFO

if [ "$(basename "${0}")" = "run" ]; then	
    [ -r /etc/init.d/functions-ri ] && . /etc/init.d/functions-ri

    prereqok 1-banner || exit 1
    prereqok 1-mdev 1 || prereqok 5-udev || exit 1
fi

. /etc/default/devpts

if grep -q devpts /proc/filesystems
then
	#
	#	Create multiplexor device.
	#
	test -c /dev/ptmx || mknod -m 666 /dev/ptmx c 5 2

	#
	#	Mount /dev/pts if needed.
	#
	if ! grep -q devpts /proc/mounts
	then
		mkdir -p /dev/pts
		mount -t devpts devpts /dev/pts -ogid=${TTYGRP},mode=${TTYMODE}
	fi
fi

if [ "$(basename "${0}")" = "run" ]; then	
	markran
	exit 0
fi
