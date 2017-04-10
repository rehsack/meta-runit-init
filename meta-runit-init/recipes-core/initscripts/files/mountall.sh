#!/bin/sh
### BEGIN INIT INFO
# Provides:          mountall
# Required-Start:    mountvirtfs
# Required-Stop: 
# Default-Start:     S
# Default-Stop:
# Short-Description: Mount all filesystems.
# Description:
### END INIT INFO

if [ "$(basename "${0}")" = "run" ]; then
    [ -r /etc/init.d/functions-ri ] && . /etc/init.d/functions-ri

    prereqok 1-checkfs || exit 1
    prereqok 1-checkroot || exit 1
fi

. /etc/default/rcS

#
# Mount local filesystems in /etc/fstab. For some reason, people
# might want to mount "proc" several times, and mount -v complains
# about this. So we mount "proc" filesystems without -v.
#
test "$VERBOSE" != no && echo "Mounting local filesystems..."
mount -at nonfs,nosmbfs,noncpfs 2>/dev/null

if [ "$(basename "${0}")" != "run" ]; then
#
# We might have mounted something over /dev, see if /dev/initctl is there.
#
if test ! -p /dev/initctl
then
	rm -f /dev/initctl
	mknod -m 600 /dev/initctl p
fi
kill -USR1 1
fi

#
# Execute swapon command again, in case we want to swap to
# a file on a now mounted filesystem.
#
[ -x /sbin/swapon ] && swapon -a

if [ "$(basename "${0}")" = "run" ]; then
    markran
fi
: exit 0

