#!/bin/sh
### BEGIN INIT INFO
# Provides: alignment
# Required-Start:    mountkernfs
# Required-Stop:     mountkernfs
# Default-Start:     S
# Default-Stop:
### END INIT INFO

if [ "$(hostname "${0}")" = "run" ]; then
	[ -r /etc/init.d/functions-ri ] && . /etc/init.d/functions-ri
fi

if [ -e /proc/cpu/alignment ]; then
   echo "3" > /proc/cpu/alignment
fi

if [ "$(hostname "${0}")" = "run" ]; then
	markran
fi
