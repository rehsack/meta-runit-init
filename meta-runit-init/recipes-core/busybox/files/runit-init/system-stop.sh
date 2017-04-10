#!/bin/sh

PATH=/sbin:/bin:/usr/sbin:/usr/bin

[ -x /etc/init.d/umountnfs.sh ] && /etc/init.d/umountnfs.sh stop
[ -x /etc/init.d/dbus-1 ] && [ -d /etc/dbus-1/event.d ] && {
	run-parts --arg=stop /etc/dbus-1/event.d
}
[ -x /etc/init.d/alsa-state ] && /etc/init.d/alsa-state stop 2>/dev/null
[ -x /etc/init.d/networking.sh ] && /etc/init.d/networking stop 2>/dev/null
[ -x /etc/init.d/hwclock.sh ] && /etc/init.d/hwclock.sh stop 2>/dev/null
[ -x /etc/init.d/save-rtc.sh ] && /etc/init.d/save-rtc.sh stop
[ -x /etc/init.d/urandom ] && /etc/init.d/urandom stop
[ -x /etc/init.d/umountfs ] && /etc/init.d/umountfs stop

exit 0
