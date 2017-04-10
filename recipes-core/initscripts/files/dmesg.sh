#!/bin/sh
### BEGIN INIT INFO
# Provides:             dmesg
# Required-Start:
# Required-Stop:
# Default-Start:        S
# Default-Stop:
### END INIT INFO

if [ "$(basename "${0}")" = "run" ]; then
	[ -r /etc/init.d/functions-ri ] && . /etc/init.d/functions-ri

	prereqok 1-populate-volatile || exit 1
fi

if [ -f /var/log/dmesg ]; then
	if LOGPATH=$(which logrotate); then
		$LOGPATH -f /etc/logrotate-dmesg.conf
	else
		mv -f /var/log/dmesg /var/log/dmesg.old
	fi
fi
dmesg -s 131072 > /var/log/dmesg

if [ "$(basename "${0}")" = "run" ]; then
    markran
    exit 0
fi
